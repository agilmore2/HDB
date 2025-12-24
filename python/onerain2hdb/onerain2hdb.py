#!/usr/bin/env python3

from datetime import datetime
import sys
import argparse
import os
import io
import urllib.parse
import pandas as pd
import requests

from lib.hdb import Hdb


def debug(msg, v):
    if v:
        print(msg)


def parse_args():
    parser = argparse.ArgumentParser(description='OneRain -> HDB loader')

    class ValidateDate(argparse.Action):
        def __call__(self, parser, namespace, values, option_string=None):
            try:
                setattr(namespace, self.dest, datetime.strptime(values, "%Y-%m-%d").date())
            except ValueError:
                parser.error(f"Invalid date value or format for {option_string}: '{values}'. Expected YYYY-MM-DD.")

    parser.register('action', 'validate_date', ValidateDate)

    class ValidateSites(argparse.Action):
        def __call__(self, parser, namespace, values, option_string=None):
            if values is None or values.strip() == "":
                setattr(namespace, self.dest, None)
                return
            items = [s.strip() for s in values.split(',')]
            if not items:
                parser.error(f"Invalid sites value for {option_string}: '{values}'. Expected comma-separated numeric site ids.")
            validated = []
            for s in items:
                if not s.isdigit():
                    parser.error(f"Invalid site id for {option_string}: '{s}'. Site ids must be numeric and comma-separated.")
                validated.append(s)
            setattr(namespace, self.dest, ",".join(f"'{s}'" for s in validated))

    parser.register('action', 'validate_sites', ValidateSites)

    parser.add_argument('-a', '--authFile', help='app_login containing database credentials', required=True)
    parser.add_argument('-A', '--agency', action='store', help='Agency Abbreviation (EBID, MRGCD) code', required=True)
    parser.add_argument('-n', '--numdays', help='How many days to load', required=False)
    parser.add_argument('-b', '--begin', action='validate_date', help='Begin date (YYYY-MM-DD)', required=False)
    parser.add_argument('-e', '--end', action='validate_date', help='End date (YYYY-MM-DD)', required=False)
    parser.add_argument('-i', '--sites', action='validate_sites', help='Sites to load (comma-separated numeric ids)', required=False)
    parser.add_argument('-l', '--flowType', action='store', help='Flow type (d/i)', required=False, default='i')
    parser.add_argument('-d', '--verbose', action='store_true', help='Show more detail about process')
    parser.add_argument('-t', '--test', action='store_true', help='Test, do not write to database')
    parser.add_argument('--overwrite', action='store_true', help='Write an O to the overwrite_flag field')

    return parser.parse_args()


def determine_date_range(args):
    lastDay = datetime.today().date()
    if args.flowType == 'd':
        lastDay = lastDay - pd.Timedelta(days=1)
            
    begin = args.begin
    end = args.end
    numdays = int(args.numdays) if args.numdays else None

    if not begin and not end:
        end = lastDay
        if numdays:
            begin = end - pd.Timedelta(days=numdays - 1)
        else:
            raise SystemExit("You must specify at least one of -b, -e, or -n to define a date range.")
    elif begin and not end:
        if numdays:
            end = begin + pd.Timedelta(days=numdays - 1)
        else:
            end = lastDay
    elif end and not begin:
        if numdays:
            begin = end - pd.Timedelta(days=numdays - 1)
        else:
            raise SystemExit("Only end date specified: use -n or -b to define when to start")
    else:
        if numdays:
            raise SystemExit("Overspecified dates: all of -b, -e, -n specified")

    return begin, end


def get_onerain_sites(hdb, args):
    """Query HDB for OneRain using agency site mappings. Returns dict[a_id][dev_code] = row dict."""
    interval = 'Daily' if args.flowType == 'd' else 'Instant'
    # Build id_limit_clause
    id_limit_clause = f"b.primary_site_code in ({args.sites}) and " if args.sites else ""
    title = f"{args.agency} OneRain Website CSV Loader - {interval}"  # match the datasource naming convention
    params = {'title': title}
    sql = f"""
        select b.primary_site_code as a_id, b.primary_data_code as dev_code,
            b.hdb_interval_name as inter, b.hdb_method_id as method_id,
            b.hdb_computation_id as comp_id, b.hdb_site_datatype_id as sdi,
            d.site_id, d.site_name, e.collection_system_id as collect_id,
            e.agen_id, e.description as url
        from hdb_site_datatype a, ref_ext_site_data_map b,
            hdb_site d, hdb_ext_data_source e
        where a.site_datatype_id = b.hdb_site_datatype_id and
            b.is_active_y_n = 'Y' and
            a.site_id = d.site_id and {id_limit_clause}
            b.ext_data_source_id = e.ext_data_source_id and
            lower(e.ext_data_source_name) = lower(:title)
        order by a_id
    """
    rows = hdb.query(sql, params)
    sites = {}
    for row in rows:
        a_id = row.get('a_id')
        dev_code = row.get('dev_code')
        sites.setdefault(a_id, {})[dev_code] = row
    return sites


def build_url(base_url, site_code, dev_code, begin, end):
    # Dates in format 'YYYY-MM-DD HH:MM:SS'
    begin_str = begin.strftime('%Y-%m-%d %H:%M:%S')
    end_str = end.strftime('%Y-%m-%d %H:%M:%S')
    # Ensure proper encoding of timestamps
    params = {
        'or_site_id': site_code,
        'or_sensor_id': dev_code,
        'data_start': begin_str,
        'data_end': end_str
    }
    # If base_url already contains query params, requests will handle combining if we pass params
    return base_url, params


def fetch_json_dataframe(url, params, verbose=False):
    """Fetch OneRain JSON response and return a DataFrame.

    Expects JSON shaped like data.json: top-level object with key 'response' that is a list
    of records containing at least 'data_time', 'data_value', and 'data_quality'.
    """
    resp = requests.get(url, params=params)
    if verbose:
        print('Fetching', resp.url)
    resp.raise_for_status()
    j = resp.json()
   
    if isinstance(j, dict) and 'count' in j and j['count'] == 0:
        raise ValueError('No data returned from OneRain for site/device')

    # JSON may be either { 'response': [ ... ] } or directly a list
    records = None
    if isinstance(j, dict) and 'response' in j:
        records = j['response']
    elif isinstance(j, list):
        records = j
    else:
        # try to find any list value
        for v in (j.values() if isinstance(j, dict) else []):
            if isinstance(v, list):
                records = v
                break

    if records is None:
        raise ValueError('Unexpected JSON structure from OneRain')

    # Normalize into DataFrame
    df = pd.json_normalize(records)

    # Ensure expected columns exist; keep data_time, data_value, data_quality if present
    # normalize column names
    df.columns = [c.strip() for c in df.columns]

    # Prefer 'data_time' naming (case-insensitive)
    col_map = {c.lower(): c for c in df.columns}
    time_col = col_map['data_time']
    val_col = col_map['data_value']
    qual_col = col_map['data_quality']

    # keep only the columns we care about
    keep_cols = [time_col, val_col]
    if qual_col: # we ignore the quality field for now
        keep_cols.append(qual_col)

    df = df.loc[:, keep_cols].copy()

    # convert time column to datetime
    df[time_col] = pd.to_datetime(df[time_col])

    df.sort_values('data_time', inplace=True)
    df.reset_index(drop=True, inplace=True)
    return df, time_col, val_col

def main(argv=None):
    args = parse_args()
    begin, end = determine_date_range(args)
    debug(f'Begin date: {begin}, End date: {end}', args.verbose)

    oFlag = 'O' if args.overwrite else None

    db = Hdb()
    db.connect_from_file(args.authFile)
    db.app = os.path.basename(sys.argv[0])
    db.app_id = db.get_loadingAppID(db.app)
    debug(f'Using loading application {db.app} with id {db.app_id}', args.verbose)

    sites = get_onerain_sites(db, args)
    debug(sites, args.verbose)

    for a_id in sites:
        for dev_code in sites[a_id]:
            row = sites[a_id][dev_code]
            base_url = row.get('url')
            if not base_url:
                debug(f'No URL for site {a_id} dev {dev_code}, skipping', args.verbose)
                continue
            url, params = build_url(base_url, a_id, dev_code, begin, end)
            try:
                df, time_col, val_col = fetch_json_dataframe(url, params, verbose=args.verbose)
            except Exception as e:
                print(f'Error fetching data for {a_id} device {dev_code}: {e}', file=sys.stderr)
                # mark error on mapping
                row['error'] = str(e)
                continue

            dt_list = df[time_col].tolist()
            val_list = pd.to_numeric(df[val_col], errors='coerce').tolist()

            app_key = row.copy()
            app_key.update(sites[a_id][dev_code] | {'overwrite_flag': oFlag, 'val': None, 'app_id': db.app_id})

            db.write_xfer(app_key, dt_list, val_list)

            if args.test:
                db.rollback()
                debug('Test mode active, database rolled back.', args.verbose)
            else:
                db.commit()
                debug('Data written to database.', args.verbose)


if __name__ == '__main__':
    main()
