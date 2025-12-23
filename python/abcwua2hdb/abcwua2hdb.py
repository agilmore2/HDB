#!/usr/bin/env python3

from datetime import datetime
import io
import sys
import argparse
import os
import warnings
from lib.hdb import Hdb
import pandas
import requests

#import logging
#from http.client import HTTPConnection

#logging.basicConfig(level=logging.DEBUG)
#logging.getLogger("urllib3").setLevel(logging.DEBUG)
#HTTPConnection.debuglevel = 1


''' HDB writing from ABCWUA data
    Only one or two sites, two SDIS, diversion and return flow volumes per hourly and daily
    daily - "We can ignore column B and D (units in KG). I just want to pull in columns C and E. C would go to SDI 31132, E would go to SDI 20071"
    hourly - "We only need columns E (Total Diverted (AF)) and I (Total Returned (AF)) It should be 31133 and 31134."
'''


def debug(msg,v):
    if v: print(msg)


def parse_args():
    parser = argparse.ArgumentParser()

    class ValidateDate(argparse.Action):
        def __call__(self, parser, namespace, values, option_string=None):
            try:
                setattr(namespace, self.dest, datetime.strptime(values, "%Y-%m-%d").date())
            except ValueError:
                parser.error(f"Invalid date value or format for {option_string}: '{values}'. Expected YYYY-MM-DD.")

    parser.register('action', 'validate_date', ValidateDate)

    parser.add_argument('-a', '--authFile', help='File containing database credentials', required=True)
    parser.add_argument('-l', '--flowType', help='Daily(d) or hourly(h) data?', required=True)
    parser.add_argument('--overwrite', action='store_true', help='Write an O to the overwrite_flag field')
    parser.add_argument('-n', '--numdays', help='How many days to load, some combination of -b, -e, -n is required to define date range')
    parser.add_argument('-b', '--begin', action='validate_date', help='Begin date (YYYY-MM-DD)')
    parser.add_argument('-e', '--end', action='validate_date', help='End date (YYYY-MM-DD)')
    parser.add_argument('-d', '--verbose', action='store_true', help='Show debug info')
    parser.add_argument('-t', '--test', action='store_true', help='Test, do not write to database')

    return parser.parse_args()


def get_abcwua_sites(hdb, flowtype):
    title = 'ABCWUA Daily Loader' if flowtype == 'd' else 'ABCWUA Hourly Loader'
    params = {'title': title}
    sql = f"""
        select b.primary_site_code as abcwua_id, b.primary_data_code as data_code,
            b.hdb_interval_name as inter, b.hdb_method_id as method_id,
            b.hdb_computation_id as comp_id, b.hdb_site_datatype_id as sdi,
            d.site_id, d.site_name, e.collection_system_id as collect_id,
            e.agen_id , e.description as url
        from hdb_site_datatype a, ref_ext_site_data_map b,
            hdb_site d, hdb_ext_data_source e
        where a.site_datatype_id = b.hdb_site_datatype_id and
            b.is_active_y_n = 'Y' and
            a.site_id = d.site_id and
            b.ext_data_source_id = e.ext_data_source_id and
            e.ext_data_source_name = :title
        order by abcwua_id
    """
    rows = hdb.query(sql, params)
    abcwua_sites = {}
    for row in rows:
        abcwua_id = row['abcwua_id']
        data_code = row['data_code']
        abcwua_sites.setdefault(abcwua_id, {})[data_code] = row
    return abcwua_sites

    # If neither begin nor end is given, use numdays to set end=today and begin=end-numdays+1
    # If only begin is given, end=begin
    # If only end is given, begin=end
    # If both are given, use as is
    # All dates are datetime.date
def determine_date_range(args):
    lastDay = datetime.today().date()
    # For daily data, use yesterday as the last available day
    if args.flowType == 'd':
        lastDay = lastDay - pandas.Timedelta(days=1)

    begin = args.begin
    end = args.end
    numdays = int(args.numdays) if args.numdays else None

    if not begin and not end:
        end = lastDay
        if numdays:
            begin = end - pandas.Timedelta(days=numdays - 1)
        else:
            raise SystemExit("You must specify at least one of -b, -e, or -n to define a date range.")
    elif begin and not end:
        if numdays:
            end = begin + pandas.Timedelta(days=numdays - 1)
        else:
            end = lastDay
    elif end and not begin:
        if numdays:
            begin = end - pandas.Timedelta(days=numdays - 1)
        else:
            raise SystemExit("Only end date specified: use -n or -b to define when to start")
    else:
        if numdays:
            raise SystemExit("Overspecified dates: all of -b, -e, -n specified")

    return begin, end


def fetch_dataframe(url, begin, end, flowtype):
    query = {"startTime": begin, "endTime": end}
    resp = requests.get(url, params=query)
    resp.raise_for_status()
    with warnings.catch_warnings():
        warnings.filterwarnings("ignore", message="Workbook contains no default style") # website doesn't set a default style
        # parse datetimes like "2025-09-15" for daily data or "2025-09-15 12:00 am" for hourly
        date_format = '%Y-%m-%d' if flowtype == 'd' else '%Y-%m-%d %I:%M %p'
        df = pandas.read_excel(io.BytesIO(resp.content), index_col=0, parse_dates=True, engine='openpyxl', date_format=date_format)
    return df


def match_and_slice(df, sites, verbose=False):
    codes = list(dict.fromkeys(k for s in sites.values() for k in s.keys()))
    df_cols = [str(c) for c in df.columns]
    matched = []
    for key in codes:
        key_s = str(key).strip().lower()
        for col in df_cols:
            if col.strip().lower() == key_s:
                matched.append(col)
                break

    if not matched and verbose:
        debug(f"No matching columns found in dataframe for site keys: {codes}", verbose)
    else:
        debug(f"Matched columns: {matched}", verbose)

    # cut out matching columns, timestamp index automatically kept
    if matched:
        df = df.loc[:, matched]
    return df


def write_all_series(db, sites, df, oFlag, app_id, test=False, verbose=False):
    dt_list = df.dropna().index.tolist()
    for site in sites:
        for code in sites[site]:
            col = next((c for c in df.columns if c.strip().lower() == str(code).strip().lower()), None)
            if col is None:
                if verbose:
                    debug(f"No column in dataframe for code {code}, skipping", verbose)
                continue
            data_list = df.dropna().loc[:, col].tolist()
            app_key = sites[site][code].copy()
            app_key.update({'overwrite_flag': oFlag, 'val': None, 'app_id': app_id})
            db.write_xfer(app_key, dt_list, data_list)
            if test:
                db.rollback()
                debug("Test mode: rolled back", verbose)
            else:
                db.commit()
                debug("Data written to database", verbose)


def main(argv=None):
    args = parse_args()
    debug(args, args.verbose)

    begin, end = determine_date_range(args)
    debug(f"Begin date: {begin}, End date: {end}", args.verbose)

    oFlag = 'O' if args.overwrite else None

    db = Hdb()
    db.connect_from_file(args.authFile)
    db.app = os.path.basename(sys.argv[0])
    db.app_id = db.get_loadingAppID(db.app)
    debug(f"Using loading application {db.app} with id {db.app_id}", args.verbose)

    sites = get_abcwua_sites(db, args.flowType)
    debug(sites, args.verbose)

    if not sites:
        db.hdbdie("No ABCWUA sites found in the database for the configured flow type; exiting.")
        return

    site = list(sites.keys())[0]
    firstcode = list(sites[site].keys())[0]
    url = sites[site][firstcode].get('url')
    debug(f"URL to fetch data from is {url}", args.verbose)

    try:
        df = fetch_dataframe(url, begin, end, args.flowType)
    except Exception as e:
        print(f"Error fetching data for {url}: {e}", file=sys.stderr)
        db.rollback()
        raise

    debug(df, args.verbose)
    df = match_and_slice(df, sites, args.verbose)
    debug(f"Sliced dataframe columns: {list(df.columns)}", args.verbose)

    write_all_series(db, sites, df, oFlag, db.app_id, test=args.test, verbose=args.verbose)


if __name__ == '__main__':
    main()

