#!/usr/bin/env python3

from datetime import datetime
import sys
import argparse
import os
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


def main(args):

    parser = argparse.ArgumentParser()
    class ValidateDate(argparse.Action):
        def __call__(self, parser, namespace, values, option_string=None):
            try:
                setattr(namespace, self.dest, datetime.strptime(values, "%Y-%m-%d").date())
            except ValueError as e:
                print(f"Exception: {e}")
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

    args = parser.parse_args()

    debug(args,args.verbose)

    # Python equivalent of Perl build_site_num_list
    def build_site_num_list(abcwua_sites):
        """
        Given a dict of abcwua_sites (from DB or config), return a list of site numbers (keys).
        """
        return list(abcwua_sites.keys())
    
    # Python equivalent of Perl get_abcwua_sites
    def get_abcwua_sites(hdb, flowtype):
        """
        Query HDB for abcwua sites and return a nested dict keyed by site_num and data_code.
        There is only one site from this source.
        """
        # Build id_limit_clause
        # choose title based on flow type (daily vs instantaneous)
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
        # Build nested dict: {abcwua_id: {data_code: row_dict}}
        abcwua_sites = {}
        for row in rows:
            abcwua_id = row['abcwua_id']
            data_code = row['data_code']
            if abcwua_id not in abcwua_sites:
                abcwua_sites[abcwua_id] = {}
            abcwua_sites[abcwua_id][data_code] = row
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
                begin = end - pandas.Timedelta(days=numdays-1)
            else:
                print("Error: You must specify at least one of -b, -e, or -n to define a date range.", file=sys.stderr)
                sys.exit(1)
        elif begin and not end:
            if numdays:
                end = begin + pandas.Timedelta(days=numdays-1)
            else:
                end = lastDay
        elif end and not begin:
            if numdays:
                begin = end - pandas.Timedelta(days=numdays-1)
            else:
                print("Error, only end date specified, use -n or -b to define when to start!", file=sys.stderr)
                sys.exit(1)
                begin = end
        else: 
            if numdays:
                print("Error, overspecified dates, all of -b, -e, -n specified!", file=sys.stderr)
                sys.exit(1)

        return begin, end

    begin, end = determine_date_range(args)
    debug(f"Begin date: {begin}, End date: {end}", args.verbose)

    oFlag = 'O' if args.overwrite else None
    
    db = Hdb()
    db.connect_from_file(args.authFile)
    db.app = os.path.basename(sys.argv[0])
    debug(f"Connected to database with app: {db.app}", args.verbose)

    db.app = os.path.basename(sys.argv[0])
    db.app_id= db.get_loadingAppID(db.app)
    debug(f"Using loading application {db.app} with id {db.app_id}", args.verbose)

    sites=get_abcwua_sites(db, args.flowType)
    debug(sites, args.verbose)
    debug(f"abcwua Sites to retrieve: {build_site_num_list(sites)}", args.verbose)

    query = {
            "startTime": begin,
            "endTime": end,
        }
    site = list(sites.keys())[0]
    firstcode = list(sites[site].keys())[0]
    url = sites[site][firstcode]["url"]
    # parse datetimes like "2025-09-15" for daily data or "2025-09-15 12:00 am" for hourly
    date_format = '%Y-%m-%d' if args.flowType == 'd' else '%Y-%m-%d %I:%M %p'
    try:
        response = requests.get(url, params=query)
        df = pandas.read_excel(response.content, index_col=0, parse_dates=True, date_format=date_format)
    except Exception as e:
        print(f"Error fetching data for {url}: {e}: ", file=sys.stderr)
        db.rollback()
        sys.exit(1)

    # Find columns that match mappings
    # Match is case-insensitive and trims whitespace.
    # Collect all unique data_code keys across all sites (preserving order)
    codes = list(dict.fromkeys(k for s in sites.values() for k in s.keys()))
    df_cols = [str(c) for c in df.columns]
    matched = []
    for key in codes:
        key_s = str(key).strip().lower()
        for col in df_cols:
            if col.strip().lower() == key_s:
                matched.append(col)
                break

    if not matched:
        debug(f"No matching columns found in dataframe for site keys: {codes}", args.verbose)
    else:
        debug(f"Matched columns: {matched}", args.verbose)

    # cut out matching columns, timestamp index automatically kept
    df = df.loc[:, matched]
    debug(f"Sliced dataframe columns: {list(df.columns)}", args.verbose)

    debug(df, args.verbose)
    print(f'Size of data retrieved for {site} is {len(df)}')
    dt_list = df.dropna().iloc[:].index.tolist()
    debug(dt_list, args.verbose)

    # try to handle cleanly when sites may be one or many sites
    for site in sites:
        for code in sites[site]:
            data_list = df.dropna().loc[:, code].tolist()
            db.write_xfer(sites[site][code] | {
                    'overwrite_flag': oFlag, 'val': None, 'app_id': db.app_id},
                    dt_list, data_list)
        
    if args.test:
        db.rollback()
        debug("Test mode active, database rolled back.", args.verbose)
    else:
        db.commit()
        debug("Data written to database.", args.verbose)

if __name__ == '__main__': 
    main(sys.argv[:])

