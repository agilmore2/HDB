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
import pandas as pd
from lib.hdb import Hdb
import csv

def debug(msg,v):
    if v: print(msg)


def generate_url():
    pass


def get_file(url):
    pass


def main(args):
    parser = argparse.ArgumentParser()
    #parser.add_argument('-a', '--authFile', help='app_login containing database credentials', required=True)
    parser.add_argument('-f', '--file', help='File to load from', required=False)
    parser.add_argument('-w', '--web', help='Load from web as defined in HDB', required=False)
    parser.add_argument('--verbose', action='store_true', help='Show more detail about process')
    parser.add_argument('--overwrite', action='store_true', help='Write an O to the overwrite_flag field')
    args = parser.parse_args()

    debug(args,args.verbose)

    if not args.file:
        url = generate_url(ids)
        csvfile = get_file(url)
    else:
        csvfile = open(args.file, newline='')

    df = pd.read_csv(args.file, skiprows=0)
    #print(df)

    #delete unit row
    df = df.drop(0).reset_index(drop=True)

    #make date time without deprecation warnings
    df['Date_Time'] = pd.to_datetime(df['Date'] + ' ' + df['Time'], format='%Y-%m-%d %H:%M')

    #delete extra columns and any NaN rows
    df=df.loc[:,df.columns.str.contains('(?:Date_Time)|(?:Air Temperature)|(?:Rain)')].dropna()

    exit()

    # Write either None or 'O' to the overwrite flag field
    oFlag = 'O' if args.overwrite else None

    db = Hdb()
    db.connect_from_file(args.authFile)

    ids = db.get_app_ids()



if __name__ == '__main__':
    main(sys.argv[:])
