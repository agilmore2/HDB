#!/usr/bin/env python3

from datetime import datetime
import sys
import argparse
import os
from lib.hdb import Hdb
import pandas as pd

import requests
import aquarius_webportal as aq

''' HDB writing from IBWC data
Follows the same processing as ibwc2hdb.pl, but uses Python and the Hdb library.
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

    parser.add_argument('-a', '--authFile', help='app_login containing database credentials', required=True)
    parser.add_argument('-l', '--flowType', help='Daily(d) or instantaneous(i) data?', required=True)
    parser.add_argument('--overwrite', action='store_true', help='Write an O to the overwrite_flag field')
    parser.add_argument('-i', '--site', action='append', help='IBWC site number', required=False)
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
    def build_site_num_list(ibwc_sites):
        """
        Given a dict of ibwc_sites (from DB or config), return a list of site numbers (keys).
        """
        return list(ibwc_sites.keys())
    
    def build_site_code_list(ibwc_sites):
        """
        Given a dict of ibwc_sites (from DB or config), return a list of site codes.
        """
        # Flatten all codes from all sites and deduplicate
        return list({code.split("_")[0] for site in ibwc_sites.values() for code in site})

    # Python equivalent of Perl get_ibwc_sites
    def get_ibwc_sites(hdb, flowtype, site_num_list=None):
        """
        Query HDB for ibwc sites and return a nested dict keyed by site_num and data_code.
        If site_num_list is provided, limit to those sites.
        """
        # Build id_limit_clause
        id_limit_clause = f"b.primary_site_code in ({site_num_list}) and " if site_num_list else ""
        title = 'IBWC Daily Loader' if flowtype == 'd' else 'IBWC Unit Values (Realtime)'
        params = {'title': title}
        # Set title based on flowtype
        sql = f"""
            select b.primary_site_code as ibwc_id, b.primary_data_code as data_code,
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
            order by ibwc_id
        """
        rows = hdb.query(sql, params)
        # Build nested dict: {ibwc_id: {data_code: row_dict}}
        ibwc_sites = {}
        for row in rows:
            ibwc_id = row['ibwc_id']
            data_code = row['data_code']
            if ibwc_id not in ibwc_sites:
                ibwc_sites[ibwc_id] = {}
            ibwc_sites[ibwc_id][data_code] = row
        return ibwc_sites

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

    sites=get_ibwc_sites(db, args.flowType,
                   args.site if args.site else None)
    debug(sites, args.verbose)
    debug(f"IBWC Sites to retrieve: {build_site_num_list(sites)}", args.verbose)
    debug(f"IBWC Codes to retrieve: {build_site_code_list(sites)}", args.verbose)

    sess=requests.Session()
    sess.cookies.set('disclaimer', 'accepted')
    ibwc=aq.AquariusWebPortal("waterdata.ibwc.gov", session=sess)

    #debug(ibwc.fetch_locations())
    #debug((sites=build_site_num_list(sites),parameterCd=build_site_code_list(sites),
    #                      start=begin,end=end,service='dv'), args.verbose)
    for site in sites:
        for data_code in sites[site]:
            debug(f"Writing data for site {site}", args.verbose)
            try:
                df = ibwc.fetch_dataset(dset_name=data_code+"@"+site, start=begin, finish=end)
            except Exception as e:
                print(f"Error fetching data for {data_code}@{site}: {e}: Entry ignored, data loads continue", file=sys.stderr)
                continue

            debug(df, args.verbose)

            dt_list = df.dropna().index.tolist()
            val_list = df.dropna().iloc[:, 0].tolist()
            debug(dt_list, args.verbose)
            debug(val_list, args.verbose)

            db.write_xfer(sites[site][data_code] | {
                        'overwrite_flag': oFlag, 'val': None, 'app_id': db.app_id},
                        dt_list, val_list)
        
    if args.test:
        db.rollback()
        debug("Database rollback executed.", args.verbose)
    else:
        db.commit()
        debug("Database commit executed.", args.verbose)

if __name__ == '__main__':
    main(sys.argv[:])

