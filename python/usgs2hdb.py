#!/usr/bin/env python3

from datetime import datetime
import sys
import argparse
import os
from lib.hdb import Hdb
import pandas as pd
import cx_Oracle
import dataretrieval.nwis as nwis

''' HDB writing from USGS NWIS data
Follows the same processing as usgs2hdb.pl, but uses Python and the cx_Oracle library.
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
    parser.add_argument('-i', '--site', action='append', help='USGS site number', required=False)
    parser.add_argument('-n', '--numdays', help='How many days to load', required=False)
    parser.add_argument('-b', '--begin', action='validate_date', help='Begin date (YYYY-MM-DD)', required=False)
    parser.add_argument('-e', '--end', action='validate_date', help='End date (YYYY-MM-DD)', required=False)
    parser.add_argument('-d', '--verbose', action='store_true', help='Show more detail about process')

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
            select b.primary_site_code as usgs_id, b.primary_data_code as data_code,
                b.hdb_interval_name as interval, b.hdb_method_id as meth_id,
                b.hdb_computation_id as comp_id, b.hdb_site_datatype_id as sdi,
                d.site_id, d.site_name
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


    # Python equivalent of Perl get_usgs_codes
    def get_usgs_codes(hdb, flowtype, usgs_sites):
        """
        Given a dict of usgs_sites, flowtype, and a db handle, return a comma-separated string of unique data codes for those sites.
        """
        # usgs_sites: dict of {site_num: {data_code: ...}}
        # flowtype: 'd' or 'u'
        # hdb: Hdb instance
        id_limit_clause = ""
        if usgs_sites:
            keys = ','.join(f"'{k}'" for k in usgs_sites.keys())
            id_limit_clause = f"b.primary_site_code in ({keys})"
        title = 'USGS Daily Values (Provisional/Official)' if flowtype == 'd' else 'USGS Unit Values (Realtime)'
        params = {'title': title}

        # Query for data codes for these sites and flowtype
        # This assumes Hdb has a method to run SQL and return results as list of dicts
        sql = f"""
            select distinct b.primary_data_code as data_id
            from hdb_ext_data_source a, ref_ext_site_data_map b 
            where {id_limit_clause}
              and a.ext_data_source_id = b.ext_data_source_id
              and a.ext_data_source_name = :title
              and b.is_active_y_n = 'Y'
              and b.hdb_interval_name = '{'day' if flowtype == 'd' else 'instant'}'
        """

        debug(f"SQL to get USGS codes: {sql} params: {params}", args.verbose)
        rows = hdb.query(sql, params)  # This should return a list of dicts with 'data_id' keys
        # Remove any _... suffixes to retrieve base datatype
        codes = set()
        for row in rows:
            code = row['data_id'].split('_')[0]
            codes.add(code)
        return ','.join(sorted(codes))


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

    sites=get_usgs_sites(db, args.flowType,
                   args.site if args.site else None)



    codes=get_usgs_codes(db, args.flowType, sites)
    debug(f"USGS Codes to retrieve: {codes}", args.verbose)


if __name__ == '__main__':
    main(sys.argv[:])

