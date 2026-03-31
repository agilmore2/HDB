#!/usr/bin/env python3

from datetime import datetime
import sys
import argparse
import os
from lib.hdb import Hdb
import pandas as pd

import dataretrieval.nwis as nwis
import dataretrieval.codes as codes
import dataretrieval.utils as utils
import dataretrieval.waterdata as waterdata

''' HDB writing from USGS NWIS data
Follows the same processing as usgs2hdb.pl, but uses Python and the cx_Oracle library.
'''

# key needs to be set in the environment or the default limit of 50 calls will be applied.
# see https://api.waterdata.usgs.gov/signup/ for your own
apikey = os.getenv('API_USGS_PAT','ubTfXAxJDuQsQpXN4wgrnFxW0awRtfsUiQKH2J2m')


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

    parser.add_argument('-a', '--authFile', help='app_login containing database credentials', required=True)
    parser.add_argument('-l', '--flowType', help='Daily(d) or instantaneous(i) data?', required=True)
    parser.add_argument('--overwrite', action='store_true', help='Write an O to the overwrite_flag field')
    parser.add_argument('-i', '--site', action='append', help='USGS site number', required=False)
    parser.add_argument('-n', '--numdays', help='How many days to load', required=False)
    parser.add_argument('-b', '--begin', action='validate_date', help='Begin date (YYYY-MM-DD)', required=False)
    parser.add_argument('-e', '--end', action='validate_date', help='End date (YYYY-MM-DD)', required=False)
    parser.add_argument('-d', '--verbose', action='store_true', help='Show more detail about process')
    parser.add_argument('-t', '--test', action='store_true', help='Test, do not write to database')

    args = parser.parse_args()
    # Handle multiple site arguments
    if args.site:
        # Support multiple --site arguments by joining them with commas
        if isinstance(args.site, list):
            args.site = ','.join(args.site)
        sites = args.site.split(',')
        for site in sites:
            if not site.isdigit():
                parser.error(f"Invalid site number: '{site}'. Site numbers must be numeric.")

    debug(args,args.verbose)

    # Python equivalent of Perl build_site_num_list
    def build_site_num_list(usgs_sites):
        """
        Given a dict of usgs_sites (from DB or config), return a list of site numbers (keys).
        """
        return list(usgs_sites.keys())
    
    def build_site_code_list(usgs_sites):
        """
        Given a dict of usgs_sites (from DB or config), return a list of site codes.
        """
        # Flatten all codes from all sites and deduplicate
        # for all sites
        return list({code.split("_")[0] for site in usgs_sites.values() for code in site})

    # Python equivalent of Perl get_usgs_sites
    def get_usgs_sites(hdb, flowtype, site_num_list=None):
        """
        Query HDB for USGS sites and return a nested dict keyed by site_num and data_code.
        If site_num_list is provided, limit to those sites.
        """
        # Build id_limit_clause
        id_limit_clause = f"b.primary_site_code in ({site_num_list}) and " if site_num_list else ""
        title = 'USGS Daily Values (Provisional/Official)' if flowtype == 'd' else 'USGS Unit Values (Realtime)'
        params = {'title': title}
        # Set title based on flowtype
        sql = f"""
            select 'USGS-'||b.primary_site_code as usgs_id, b.primary_data_code as data_code,
                b.hdb_interval_name as inter, b.hdb_method_id as method_id,
                b.hdb_computation_id as comp_id, b.hdb_site_datatype_id as sdi,
                d.site_id, d.site_name, e.collection_system_id as collect_id,
                e.agen_id
            from hdb_site_datatype a, ref_ext_site_data_map b,
                hdb_site d, hdb_ext_data_source e
            where a.site_datatype_id = b.hdb_site_datatype_id and {id_limit_clause}
                b.is_active_y_n = 'Y' and
                a.site_id = d.site_id and
                b.ext_data_source_id = e.ext_data_source_id and
                e.ext_data_source_name = :title
            order by usgs_id
        """
        rows = hdb.query(sql, params)
        # Build nested dict: {usgs_id: {data_code: row_dict}}
        usgs_sites = {}
        for row in rows:
            usgs_id = row['usgs_id']
            data_code = row['data_code']
            if usgs_id not in usgs_sites:
                usgs_sites[usgs_id] = {}
            usgs_sites[usgs_id][data_code] = row
        return usgs_sites

    # If neither begin nor end is given, use numdays to set end=today and begin=end-numdays+1
    # If only begin is given, end=begin
    # If only end is given, begin=end
    # If both are given, use as is
    # All dates are datetime.date
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
                begin = end - pd.Timedelta(days=numdays-1)
            else:
                print("Error: You must specify at least one of -b, -e, or -n to define a date range.", file=sys.stderr)
                sys.exit(1)
        elif begin and not end:
            if numdays:
                end = begin + pd.Timedelta(days=numdays-1)
            else:
                end = lastDay
        elif end and not begin:
            if numdays:
                begin = end - pd.Timedelta(days=numdays-1)
            else:
                print("Error, only end date specified, use -n or -b to define when to start!", file=sys.stderr)
                sys.exit(1)
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
    db.app_id= db.get_loadingAppID(db.app)
    debug(f"Using loading application {db.app} with id {db.app_id}", args.verbose)

    sites=get_usgs_sites(db, args.flowType,
                   args.site if args.site else None)
    debug(f"USGS Sites to retrieve: {build_site_num_list(sites)}", args.verbose)
    debug(f"USGS Codes to retrieve: {build_site_code_list(sites)}", args.verbose)

    func = waterdata.get_daily if args.flowType == 'd' else waterdata.get_continuous
    rows_per_record = 1 if args.flowType == 'd' else 288
    max_rows_per_call = 10000
    max_sites_per_call = 50
    
    # Build list of (site, code) tuples for processing
    site_code_pairs = [(site, code) for site in sites for code in sites[site]]
    
    def calculate_max_days_per_batch(num_pairs, rows_per_day):
        """Calculate maximum days that can fit in a batch without exceeding row limit."""
        if rows_per_day == 0:
            return (end - begin).days
        return max(1, max_rows_per_call // (num_pairs * rows_per_day))
    
    def fetch_and_write_batch(batch_pairs, batch_begin, batch_end):
        """Fetch data for a batch of site/code pairs and write to database."""
        batch_sites = list(set(pair[0] for pair in batch_pairs))
        batch_codes = list(set(pair[1].split("_")[0] for pair in batch_pairs))
        
        debug(f"Retrieving batch: sites={batch_sites}, codes={batch_codes}, "
              f"period={batch_begin} to {batch_end}", args.verbose)
        
        df,md = func(monitoring_location_id=batch_sites,
                  parameter_code=batch_codes,
                  time=[batch_begin.strftime("%Y-%m-%d %H:%M:%S"), 
                        batch_end.strftime("%Y-%m-%d %H:%M:%S")])
        
        if df.empty:
            debug(f"No data returned for batch", args.verbose)
            return
        
        # Process results by individual site/code combination
        for site, code in batch_pairs:
            site_code_df = df[(df['monitoring_location_id'] == site) & 
                              (df['parameter_code'] == code.split("_")[0])].copy()
            
            if site_code_df.empty:
                debug(f"No data for site {site} code {code}", args.verbose)
                continue
            
            site_code_df.set_index('time', inplace=True)
            site_code_df.index = pd.to_datetime(site_code_df.index)
            site_code_df.sort_index(inplace=True)
            site_code_df.dropna(subset=['value'], inplace=True)
            
            if site_code_df.empty:
                continue
            
            dt_list = site_code_df.index.tolist()
            val_series = pd.to_numeric(site_code_df["value"], errors='coerce')
            val_list = val_series.tolist()
            
            debug(f"Writing {len(val_list)} rows for site {site} code {code}", args.verbose)
            
            db.write_xfer(sites[site][code] | {
                        'overwrite_flag': oFlag, 'val': None, 'app_id': db.app_id},
                        dt_list, val_list)
            
            if args.test:
                db.rollback()
                debug("Test mode active, database rollback executed.", args.verbose)
            else:
                db.commit()
                debug("Data written to database.", args.verbose)
    
    # Batch API calls with site limit and row limit
    pair_index = 0
    while pair_index < len(site_code_pairs):
        # Determine how many site/code pairs can fit in this batch
        remaining_pairs = site_code_pairs[pair_index:]
        batch_size = min(max_sites_per_call, len(remaining_pairs))
        
        # Check if this batch would exceed row limit for the full date range
        estimated_rows = batch_size * rows_per_record * (end - begin).days
        if estimated_rows > max_rows_per_call:
            # Calculate how many days we can safely process
            max_days = calculate_max_days_per_batch(batch_size, rows_per_record)
            
            # Process this batch in chunks of days
            current_date = begin
            while current_date < end:
                chunk_end = min(current_date + pd.Timedelta(days=max_days), end)
                batch_pairs = remaining_pairs[:batch_size]
                
                fetch_and_write_batch(batch_pairs, current_date, chunk_end)
                
                current_date = chunk_end
        else:
            # Batch fits within row limit for full date range
            batch_pairs = remaining_pairs[:batch_size]
            fetch_and_write_batch(batch_pairs, begin, end)
        
        # Advance to next batch of site/code pairs
        pair_index += batch_size

    #debug(nwis.get_record(sites=build_site_num_list(sites),parameterCd=build_site_code_list(sites),
    #                      start=begin,end=end,service='dv'), args.verbose)
    #debug(nwis.get_dv(sites=build_site_num_list(sites),parameterCd=build_site_code_list(sites),
    #                      start=begin,end=end), args.verbose)
                        


if __name__ == '__main__':
    main(sys.argv[:])

