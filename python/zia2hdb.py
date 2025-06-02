#!/usr/bin/python3
"""HDB writing from NMSU Ziamet for Nambe Falls climate data

Built against Python 3.12 and cx_Oracle 8.3.0
Created May/June 2025

@author: Andrew Gilmore
after xl2hdb by Andrew Gilmore and Brett Boyer
"""
import io
from datetime import datetime, timedelta
import sys
import argparse
import os
import pandas as pd
import requests

from lib.hdb import Hdb


def debug(msg, v):
    if v: print(msg)


def generate_url():
    pass


def get_file(url):
    return url

def parse_dates(args):
    # handle dates:
    # Truth table and results for numdays, begindate and enddate
    # errors are all defined, only enddate defined, nothing defined
    # everything else gets fixed up.
    # Num Days | Beg Date | End Date | Result
    # nothing defined                   error
    #  def                              end date = now, beg date = enddate-numdays
    #  def        def                   end date = beg date + numdays
    #             def                   end date = now, check max num days?
    #             def         def       ok
    #                         def       error, what is beg date?
    #  def                    def       beg date = end date - numdays
    #  def        def         def       error, not checking to see if consistent
    args.numdays = int(args.numdays)

    if args.numdays:
        # let exceptions handle if arguments are nonsense
        if args.begindate and args.enddate:
            print ("Error, overspecified dates, all of -n, -b, -e specified!")
            exit(1)
        elif args.begindate:
            args.begindate = datetime.fromisoformat(args.begindate)
            args.enddate = args.begindate + timedelta(args.numdays)
        elif args.enddate:
            args.enddate = datetime.fromisoformat(args.enddate)
        else:
            args.enddate = datetime.now().date()
        # also handles if enddate is specified
        args.begindate = args.enddate + timedelta(days=-args.numdays)

    else:
        if args.begindate and args.enddate:
            args.begindate = datetime.fromisoformat(args.begindate)
            args.enddate = datetime.fromisoformat(args.enddate)
        elif args.begindate:
            args.begindate = datetime.fromisoformat(args.begindate)
            args.enddate = datetime.now()
        elif args.enddate:
            print("Error, cannot specify only end date!")
            exit(1)
        else:
            print("Error, dates are unspecified!")
            exit(1)
        args.numdays = args.enddate - args.begindate

    if not args.begindate or not args.enddate:
        print("Error, dates not specifed or error parsing dates!")
        exit(1)

    return args


def main(args):
    parser = argparse.ArgumentParser()
    parser.add_argument('-a', '--authFile', help='app_login containing database credentials', required=True)
    parser.add_argument('-f', '--file', help='File to load from', required=False)
    parser.add_argument('-n', '--numdays', help='Number of days to load', required=False)
    parser.add_argument('-b', '--begindate', help='Beginning date to load from, YYYY-MM-DD', required=False)
    parser.add_argument('-e', '--enddate', help='Ending date to load to, YYYY-MM-DD', required=False)
    parser.add_argument('--verbose', action='store_true', help='Show more detail about process')
    parser.add_argument('--overwrite', action='store_true', help='Write an O to the overwrite_flag field')

    args = parse_dates(parser.parse_args())

    debug(args, args.verbose)

    if not args.file:
        #url = generate_url(ids)
        url = 'https://weather.nmsu.edu/ziamet/station/sfwx/dly/sfnamb/'
        client=requests.session()
        client.get(url) #get CSRF cookie
        response = client.post(url,
                               headers={'Referer': 'https://weather.nmsu.edu/ziamet/station/data/sfnamb/', },
                               data={'csrfmiddlewaretoken': client.cookies['csrftoken'],
                                                   'dtype': 'sfwx',
                                                     'sid': 'sfnamb',
                                                   'sdate': args.begindate,
                                                   'edate': args.enddate,
                                                  'output': 'csv',
                                                   'units': 'iu',
                                    }
                               )

        csvfile = io.StringIO(response.content.decode('utf-8'))
    else:
        csvfile = args.file

    df = pd.read_csv(csvfile, skiprows=0)

    # delete unit row
    df = df.drop(0).reset_index(drop=True)

    #TODO: retrieve URL? and column names/SDIs/etc. from ref_site_data_map etc.

    #delete extra columns and any NaN rows
    df = df.loc[:, df.columns.str.contains('(?:Date)|(?:M.. Air Temperature)|(?:Rain)')].dropna()
    df['Date'] = pd.to_datetime(df['Date'])
    df[["Rain", "Max Air Temperature", "Min Air Temperature"]] =\
        df[["Rain", "Max Air Temperature", "Min Air Temperature"]].apply(pd.to_numeric)

    debug(df, args.verbose)

    dtList = df["Date"].tolist()
    rainList = df["Rain"].tolist()
    maxTList = df["Max Air Temperature"].tolist()
    minTList = df["Min Air Temperature"].tolist()

    #hardcoded for now
    rainSDI = 19698
    maxSDI = 20200
    minSDI = 20201

    db = Hdb()
    db.connect_from_file(args.authFile)

    interval = 'day'
    db.agen = 'New Mexico State University'
    db.app = os.path.basename(sys.argv[0])
    db.collect = '(see agency)'
    db.method = 'N/A'
    db.comp = 'unknown'
    # Write either None or 'O' to the overwrite flag field
    oFlag = 'O' if args.overwrite else None

    ids = db.get_app_ids()

    db.write_xfer(ids | {'sdi': rainSDI, 'inter': interval,
                                      'overwrite_flag': oFlag, 'val': 'Z'},
                  dtList, rainList)

    db.write_xfer(ids | {'sdi': maxSDI, 'inter': interval,
                                      'overwrite_flag': oFlag, 'val': 'Z'},
                  dtList, maxTList)

    db.write_xfer(ids | {'sdi': minSDI, 'inter': interval,
                                      'overwrite_flag': oFlag, 'val': 'Z'},
                  dtList, minTList)

    db.commit()

    print("Wrote %d values." % (len(rainList) + len(maxTList) + len(minTList)))


if __name__ == '__main__':
    main(sys.argv[:])
