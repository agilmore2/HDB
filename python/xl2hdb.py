#!/usr/bin/python3
'''HDB writing from XLSX files, designed specifically for CUL project
Excel input files should be in the following format:
    - sheet/row/column => datatype/timestep/site
    - header rows => either one set of headers that apply to all sites in the sheet or one set of headers per site
    - data flags => for time series data flags, each site should have a column to the right with data flags and a column header <site>.data_flags


Built against Python 3.9 and cx_Oracle 5.1.2
Created February 2022

Has only been used for the CUL project, could use better error handling
@author: Andrew Gilmore, Brett Boyer
'''

from datetime import datetime
import sys
import argparse
import os
import pandas
from lib.hdb import Hdb

# Header rows in excel workbook need to have these labels
INT_ROW = "INTERVAL"
AGEN_ROW = "AGEN"
COLLECT_ROW = "COLLECT"
METHOD_ROW = "METHOD"
COMP_ROW = "COMP"
HEADER_ROWS = 5

def debug(msg,v):
    if v: print(msg)

def main(args):
    parser = argparse.ArgumentParser()
    parser.add_argument('-a', '--authFile', help='app_login containing database credentials', required=True)
    parser.add_argument('-f', '--file', help='File to load from', required=True)
    parser.add_argument('-s', '--sheet', help='Sheet to load from', required=True)
    parser.add_argument('-c', '--column', help='Optional, provide name of single site (column) to load', required=False)
    parser.add_argument('-l', '--loadingApp', help='Supply an optional loading application name, defaults to file name', required=False)
    parser.add_argument('-d','--dataCodeSys',help='Data code system name, used to map sheet names to hdb datatypes. Default = CUL Sheet Names',default='CUL Sheet Names')
    parser.add_argument('--verbose', action='store_true', help='Show more detail about process')
    parser.add_argument('--nooverwrite', action='store_true', help='Do not write an O to the overwrite_flag field')
    args = parser.parse_args()

    debug(args,args.verbose)

    # Write either None or 'O' to the overwrite flag field
    oFlag = None if args.nooverwrite else 'O'

    db = Hdb()
    db.connect_from_file(args.authFile)
    if args.loadingApp == None:
        db.app = os.path.basename(sys.argv[0]) 
    else:
        db.app = args.loadingApp

    data = pandas.read_excel(io=args.file, sheet_name=args.sheet,
                            index_col=0, parse_dates=True, skiprows=HEADER_ROWS)
    # header can either be one column and apply to the whole sheet, or there is a header for each site column
    header = pandas.read_excel(io=args.file, sheet_name=args.sheet,
                               index_col=0, header=None,names=data.columns.tolist(),nrows=HEADER_ROWS)
    
    # determine whether data includes flags. This will change the write behavior
    dataFlags = any('.data_flag' in s for s in data.columns.tolist())

    # sites are the column headers that don't contain ".data_flag"
    sites = [s for s in data.columns.tolist() if '.data_flag' not in s]

    if len(sites) > 1 and header[sites[1]].isna().sum() > 0: # if the second column of the header has more than 0 blanks, assume that the first column applies to all sites
        for site_name in sites[1:]:
            header[site_name] = header[sites[0]]

    # if the column argument is supplied, shorten the list of sites to just a single one
    if args.column is not None:
        sites = [args.column]              

    for site_name in sites:
        interval = header.loc[INT_ROW][site_name]
        db.agen = header.loc[AGEN_ROW][site_name]
        db.collect = header.loc[COLLECT_ROW][site_name]
        db.method = header.loc[METHOD_ROW][site_name]
        db.comp = header.loc[COMP_ROW][site_name]    

        sdi = db.lookup_sdi_ext_data_code(site_name, args.sheet,args.dataCodeSys)
        if sdi is None:
                db.hdbdie(f"Lookup of sdi for site: {site_name} datatype code: {args.sheet[0]} failed!")

        debug('{}/{} | site = {}; interval = {}; agency = {}; collection system = {}; method = {}; computation = {}; sdi = {}'
                    .format(sites.index(site_name) + 1,len(sites),site_name,interval,db.agen,db.collect,db.method,db.comp,sdi),args.verbose)

        # the write is handled very differently with and without data flags, because of the way that ts_xfer.write_real_data works
        # handle separately
        if not dataFlags:
            dt_list = data[site_name].dropna().index.tolist()
            val_list = data[site_name].dropna().tolist()            

            db.write_xfer(db.get_app_ids() | {'sdi': sdi, 'inter': interval,
                                        'overwrite_flag': oFlag, 'val': None},
                    dt_list, val_list)
        else:
            # ts_xfer.write_real_data takes a single character as the data_flags argument instead of an array
            # write chunks of data with constant data flags
            flagCol = site_name + '.data_flag'
            allFlags = data[flagCol].dropna().unique().tolist()
            for f in allFlags:
                dt_list = data[data[flagCol] == f][site_name].dropna().index.tolist()
                val_list = data[data[flagCol] == f][site_name].dropna().tolist()

                db.write_xfer(db.get_app_ids() | {'sdi': sdi, 'inter': interval,
                                            'overwrite_flag': oFlag, 'val': None},
                        dt_list, val_list,f)
            
            # repeat a similar step for blank data flags
            dt_list = data[data[flagCol].isna()][site_name].dropna().index.tolist()
            val_list = data[data[flagCol].isna()][site_name].dropna().tolist()

            db.write_xfer(db.get_app_ids() | {'sdi': sdi, 'inter': interval,
                                        'overwrite_flag': oFlag, 'val': None},
                    dt_list, val_list)


    if input('Commit? y/n: ') == 'y':
        db.commit()

    print('Loading complete at ' + datetime.now().strftime('%c'))


if __name__ == '__main__':
    main(sys.argv[:])
