#!/usr/bin/python3
"""HDB writing from NMSU Ziamet for Nambe Falls climate data."""

from datetime import datetime, timedelta
import argparse
import os
import sys
import io
import warnings

import pandas as pd
import requests

from lib.hdb import Hdb

ZIAMET_TITLE = 'NMSU ZiaMet/CoCoRaHS loader'


def debug(message, verbose):
    if verbose:
        print(message)


class ValidateDate(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        try:
            setattr(namespace, self.dest, datetime.strptime(values, "%Y-%m-%d").date())
        except ValueError:
            parser.error(
                f"Invalid date for {option_string}: '{values}'. Expected YYYY-MM-DD."
            )


def parse_args():
    parser = argparse.ArgumentParser(description='Ziamet -> HDB loader')
    parser.register('action', 'validate_date', ValidateDate)

    parser.add_argument('-a', '--authFile', required=True,
                        help='app_login containing database credentials')
    parser.add_argument('-n', '--numdays', help='Number of days to load', required=False)
    parser.add_argument('-b', '--begin', action='validate_date',
                        help='Begin date (YYYY-MM-DD)', required=False)
    parser.add_argument('-e', '--end', action='validate_date',
                        help='End date (YYYY-MM-DD)', required=False)
    parser.add_argument('-d', '--verbose', action='store_true',
                        help='Show more detail about process')
    parser.add_argument('-t', '--test', action='store_true',
                        help='Test, do not write to database')
    parser.add_argument('--overwrite', action='store_true',
                        help='Write an O to the overwrite_flag field')

    return parser.parse_args()


def determine_date_range(args):
    last_day = datetime.today().date()
    begin = args.begin
    end = args.end
    numdays = int(args.numdays) if args.numdays else None

    if not begin and not end:
        end = last_day
        if not numdays:
            raise SystemExit("Must specify -b, -e, or -n to define a date range.")
        begin = end - timedelta(days=numdays - 1)
    elif begin and not end:
        end = begin + timedelta(days=numdays - 1) if numdays else last_day
    elif end and not begin:
        if not numdays:
            raise SystemExit("Only end date specified; use -n or -b to define start date.")
        begin = end - timedelta(days=numdays - 1)
    elif numdays:
        raise SystemExit("Cannot specify both -b/-e and -n together.")

    return begin, end


def setup_database(authfile):
    db = Hdb()
    db.connect_from_file(authfile)
    db.app = os.path.basename(sys.argv[0])
    db.app_id = db.get_loadingAppID(db.app)
    return db


def get_ziamet_sites(db):
    sql = """
        select b.primary_site_code as site_code,
               b.primary_data_code as data_code,
               b.hdb_interval_name as inter,
               b.hdb_method_id as method_id,
               b.hdb_computation_id as comp_id,
               b.hdb_site_datatype_id as sdi,
               d.site_name,
               e.collection_system_id as collect_id,
               e.agen_id,
               e.description as url
        from hdb_site_datatype a
        join ref_ext_site_data_map b on a.site_datatype_id = b.hdb_site_datatype_id
        join hdb_site d on a.site_id = d.site_id
        join hdb_ext_data_source e on b.ext_data_source_id = e.ext_data_source_id
        where b.is_active_y_n = 'Y'
          and lower(e.ext_data_source_name) = lower(:title)
        order by site_code, data_code
    """
    rows = db.query(sql, {'title': ZIAMET_TITLE})
    sites = {}
    for row in rows:
        site_code = row['site_code']
        data_code = row['data_code']
        sites.setdefault(site_code, {})[data_code] = row
    return sites


def build_site_url(site_metadata):
    url = site_metadata.get('url')
    if not url or not url.startswith(('http://', 'https://')):
        raise ValueError(f"Invalid URL for site {site_metadata.get('site_code')}: {url}")
    return url.rstrip('/') + '/'


def fetch_site_data(base_url, site_code, columns, begin, end, verbose=False):
    endpoint = f"{base_url}sfwx/dly/{site_code}/"
    with requests.Session() as session:
        warnings.filterwarnings('ignore', message='Unverified HTTPS request')
        session.get(endpoint, verify=False)

        response = session.post(
            endpoint,
            verify=False,
            headers={'Referer': f"{base_url}data/{site_code}/"},
            data={
                'csrfmiddlewaretoken': session.cookies.get('csrftoken', ''),
                'dtype': 'sfwx',
                'sid': site_code,
                'sdate': begin.strftime('%Y-%m-%d'),
                'edate': end.strftime('%Y-%m-%d'),
                'output': 'csv',
                'units': 'iu',
            }
        )
        response.raise_for_status()

    if verbose:
        debug(f"Fetching {site_code} from {endpoint} for {begin}..{end}", verbose)

    df = pd.read_csv(io.StringIO(response.text))
    if df.empty or 'Date' not in df.columns:
        return pd.DataFrame(columns=['Date'] + columns)

    df = df.drop(index=0, errors='ignore').reset_index(drop=True)
    available_cols = ['Date'] + [col for col in columns if col in df.columns]
    df = df.loc[:, available_cols].copy()
    df['Date'] = pd.to_datetime(df['Date'])

    for col in available_cols:
        if col != 'Date':
            df[col] = pd.to_numeric(df[col], errors='coerce')
    df = df.dropna().reset_index(drop=True) # does this annoying thing where right after midnight, the current day values are NaN

    return df


def write_datatype_data(db, metadata, dates, values, verbose=False, test=False):
    if not dates:
        debug(f"No values for {metadata.get('site_code')} {metadata.get('data_code')}", verbose)
        return

    app_key = dict(metadata)
    app_key.update({
        'val': None,
        'app_id': db.app_id,
    })
    db.write_xfer(app_key, dates, values)

    if test:
        db.rollback()
        debug(f"Rolled back {metadata['site_code']} {metadata['data_code']}", verbose)
    else:
        db.commit()
        debug(f"Wrote {len(values)} values for {metadata['site_code']} {metadata['data_code']}", verbose)


def main(argv=None):
    args = parse_args()
    begin, end = determine_date_range(args)
    debug(f"Begin date: {begin}, End date: {end}", args.verbose)

    db = setup_database(args.authFile)
    debug(f"Using loading application {db.app} id {db.app_id}", args.verbose)

    sites = get_ziamet_sites(db)
    debug(f"Loaded {len(sites)} Ziamet sites", args.verbose)

    total_written = 0
    for site_code, datatypes in sites.items():
        sample_metadata = next(iter(datatypes.values()))
        try:
            base_url = build_site_url(sample_metadata)
        except ValueError as exc:
            print(exc, file=sys.stderr)
            continue

        columns = list(datatypes.keys())
        df = fetch_site_data(base_url, site_code, columns, begin, end, verbose=args.verbose)

        for data_code, metadata in datatypes.items():
            if data_code not in df.columns:
                debug(f"Skipping {site_code} {data_code}: column missing", args.verbose)
                continue

            dates = df['Date'].tolist()
            values = df[data_code].tolist()
            metadata['overwrite_flag'] = 'O' if args.overwrite else None
            write_datatype_data(db, metadata, dates, values, verbose=args.verbose, test=args.test)
            total_written += len(values)

    print(f"Wrote {total_written} total values.")


if __name__ == '__main__':
    main()
