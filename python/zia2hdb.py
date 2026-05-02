#!/usr/bin/python3
"""HDB writing from NMSU Ziamet for Nambe Falls climate data

Built against Python 3.12 and cx_Oracle 8.3.0
Created May/June 2025

@author: Andrew Gilmore
after xl2hdb by Andrew Gilmore and Brett Boyer
"""
from datetime import datetime, timedelta
import sys
import argparse
import os
import io
import pandas as pd
import requests

from lib.hdb import Hdb


def debug(msg, v):
    if v:
        print(msg)


def parse_args():
    parser = argparse.ArgumentParser(description='Ziamet -> HDB loader')

    class ValidateDate(argparse.Action):
        def __call__(self, parser, namespace, values, option_string=None):
            try:
                setattr(namespace, self.dest, datetime.strptime(values, "%Y-%m-%d").date())
            except ValueError:
                parser.error(f"Invalid date value or format for {option_string}: '{values}'. Expected YYYY-MM-DD.")

    parser.register('action', 'validate_date', ValidateDate)

    parser.add_argument('-a', '--authFile', help='app_login containing database credentials', required=True)
    parser.add_argument('-n', '--numdays', help='Number of days to load', required=False)
    parser.add_argument('-b', '--begin', action='validate_date', help='Begin date (YYYY-MM-DD)', required=False)
    parser.add_argument('-e', '--end', action='validate_date', help='End date (YYYY-MM-DD)', required=False)
    parser.add_argument('-f', '--file', help='File to load from', required=False)
    parser.add_argument('-d', '--verbose', action='store_true', help='Show more detail about process')
    parser.add_argument('-t', '--test', action='store_true', help='Test, do not write to database')
    parser.add_argument('--overwrite', action='store_true', help='Write an O to the overwrite_flag field')

    return parser.parse_args()


def determine_date_range(args):
    """Determine begin and end dates based on arguments.
    
    Truth table:
    - If neither begin nor end: use numdays from end=today, or error
    - If only begin: end = begin + numdays or end = today
    - If only end: begin = end - numdays or error
    - If both: use as-is, ignore numdays if provided
    """
    lastDay = datetime.today().date()
    
    begin = args.begin
    end = args.end
    numdays = int(args.numdays) if args.numdays else None

    if not begin and not end:
        end = lastDay
        if numdays:
            begin = end - timedelta(days=numdays - 1)
        else:
            raise SystemExit("You must specify at least one of -b, -e, or -n to define a date range.")
    elif begin and not end:
        if numdays:
            end = begin + timedelta(days=numdays - 1)
        else:
            end = lastDay
    elif end and not begin:
        if numdays:
            begin = end - timedelta(days=numdays - 1)
        else:
            raise SystemExit("Only end date specified: use -n or -b to define when to start")
    else:
        if numdays:
            raise SystemExit("Overspecified dates: all of -b, -e, -n specified")

    return begin, end


def get_ziamet_sites(hdb):
    """Query HDB for Ziamet sites and return a nested dict keyed by site_code and data_code.
    
    Returns dict[site_code][data_code] = row dict with metadata
    """
    title = 'NMSU ZiaMet/CoCoRaHS loader'
    params = {'title': title}
    sql = f"""
        select b.primary_site_code as site_code, b.primary_data_code as data_code,
            b.hdb_interval_name as inter, b.hdb_method_id as method_id,
            b.hdb_computation_id as comp_id, b.hdb_site_datatype_id as sdi,
            d.site_name, e.collection_system_id as collect_id,
            e.agen_id, e.description as url
        from hdb_site_datatype a, ref_ext_site_data_map b,
            hdb_site d, hdb_ext_data_source e
        where a.site_datatype_id = b.hdb_site_datatype_id and
            b.is_active_y_n = 'Y' and
            a.site_id = d.site_id and
            b.ext_data_source_id = e.ext_data_source_id and
            lower(e.ext_data_source_name) = lower(:title)
        order by site_code, data_code
    """
    rows = hdb.query(sql, params)
    sites = {}
    for row in rows:
        site_code = row.get('site_code')
        data_code = row.get('data_code')
        sites.setdefault(site_code, {})[data_code] = row
    return sites


def fetch_ziamet_data(base_url, site_code, site, begin, end, verbose=False):
    """Fetch Ziamet CSV data from NMSU website.
    
    Args:
        base_url: Base URL for the Ziamet data service (e.g. https://weather.nmsu.edu/ziamet/station)
        site_code: Code for the site to fetch data from
        begin: Start date (datetime.date)
        end: End date (datetime.date)
        verbose: Enable verbose output
        
    Returns:
        pandas.DataFrame with columns: Date, Rain, Max Air Temperature, Min Air Temperature
    """
    url = f'{base_url}sfwx/dly/{site_code}/'
    
    try:
        client = requests.session()
        client.get(url, verify=False)  # Get CSRF cookie, verify false to ignore MitM by DOI

        response = client.post(
            url,
            verify=False,
            headers={'Referer': f'{base_url}data/{site_code}/'},
            data={
                'csrfmiddlewaretoken': client.cookies['csrftoken'],
                'dtype': 'sfwx',
                'sid': f'{site_code}',
                'sdate': begin.strftime('%Y-%m-%d'),
                'edate': end.strftime('%Y-%m-%d'),
                'output': 'csv',
                'units': 'iu',
            }
        )
        response.raise_for_status()
        
        if verbose:
            debug(f'Fetching data from {url} for {begin} to {end}', verbose)
        
        csvfile = io.StringIO(response.content.decode('utf-8'))
        df = pd.read_csv(csvfile, skiprows=0)
        
        # Delete unit row
        df = df.drop(0).reset_index(drop=True)
        data_codes = list(site.keys())
        # Filter to relevant columns and remove NaN rows
        df = df.loc[:, ['Date'] + [col for col in df.columns if col in data_codes]].dropna()
        
        # Convert date and numeric columns
        df['Date'] = pd.to_datetime(df['Date'])
        df[data_codes] = df[data_codes].apply(pd.to_numeric, errors='coerce')
        
        return df
        
    except Exception as e:
        raise SystemExit(f"Error fetching data from Ziamet: {e}")


def setup_database(args):
    """Initialize and connect to HDB."""
    db = Hdb()
    db.connect_from_file(args.authFile)
    db.app = os.path.basename(sys.argv[0])
    db.app_id = db.get_loadingAppID(db.app)
    return db


def write_site_data(db, app_key, dt_list, val_list, verbose=False, test=False):
    """Write a single site_datatype's data to the database.
    
    Args:app_key["site_code"]
        db: Hdb connection object
        app_key: Dict with metadata for write_xfer
        dt_list: List of datetime values
        val_list: List of data values
        verbose: Enable verbose output
        test: If True, rollback instead of commit
    """
    if not dt_list:
        debug(f'No data to write for {app_key["site_name"]}', verbose)
        return
    
    db.write_xfer(app_key, dt_list, val_list)
    
    if test:
        db.rollback()
        debug(f'Test mode active, database rolled back for {app_key["site_name"]}: {len(val_list)} values.', verbose)
    else:
        db.commit()
        debug(f'Data written to database for {app_key["site_name"]}: {len(val_list)} values.', verbose)


def main(argv=None):
    args = parse_args()
    begin, end = determine_date_range(args)
    debug(f'Begin date: {begin}, End date: {end}', args.verbose)

    oFlag = 'O' if args.overwrite else None

    # Setup database connection
    db = setup_database(args)
    debug(f'Using loading application {db.app} with id {db.app_id}', args.verbose)

    # Retrieve site mappings from database
    sites = get_ziamet_sites(db)
    debug(f'Sites retrieved from database: {sites}', args.verbose)

    total_written = 0
    for site_code in sites:
        row = next(iter(sites[site_code].values()))
        base_url = row.get('url')
        # Fetch data page for all site data (assumes all data on same page)

        if not base_url:
            debug(f'No URL for site {site_code}, skipping', args.verbose)
            continue
        if not base_url.startswith(('http://', 'https://')):
            print(f'Invalid URL for site {site_code}: {base_url}. URL must start with http:// or https://', file=sys.stderr)
            continue
        try:
            df = fetch_ziamet_data(base_url, site_code, sites[site_code],
                                begin, end, verbose=args.verbose)
            debug(f'Retrieved {len(df)} rows from Ziamet', args.verbose)
        except SystemExit as e:
            print(e, file=sys.stderr)
            return

        for data_code in sites[site_code]:

            if data_code not in df.columns:
                debug(f'Column {data_code} not found in data, skipping', args.verbose)
                continue
            
            # Build lists for this datatype
            dt_list = df['Date'].tolist()
            val_list = pd.to_numeric(df[data_code], errors='coerce').tolist()
            
            # Build app_key with metadata
            app_key = sites[site_code][data_code].copy()
            app_key.update({
                'overwrite_flag': oFlag,
                'val': None,
                'app_id': db.app_id
            })
            
            # Write data to database
            write_site_data(db, app_key, dt_list, val_list,
                          verbose=args.verbose, test=args.test)
            
            total_written += len(val_list)
    
    print(f"Wrote {total_written} total values.")


if __name__ == '__main__':
    main()
