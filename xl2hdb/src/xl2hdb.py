#!/usr/bin/python3
'''HDB writing from XLSX files, and more?

Built against Python 2.7.4, Pillow 2.0.0, and cx_Oracle 5.1.2

Created on Apr 15, 2013

@author: hsk@RTi
'''

from datetime import datetime
import sys
import argparse
import os
import pandas


from hdb import Hdb

INT_ROW = "INTERVAL"
AGEN_ROW = "AGEN"
COLLECT_ROW = "COLLECT"
METHOD_ROW = "METHOD"
COMP_ROW = "COMP"
SITE_ROW = 6

def main(args):
    parser = argparse.ArgumentParser()
    parser.add_argument('-a', help='app_login containing database credentials', required=True)
    parser.add_argument('-f', '--file', help='File to load from', required=True)
    parser.add_argument('--sheet', '-s', nargs='+', help='Sheet to load from, default first', default=0)
    parser.add_argument('-v', '--verbose', action='count', default=0, help='Show more detail about process')
    args = parser.parse_args()
    print(args)
    db = Hdb()
    db.connect_from_file(args.a)
    db.app = os.path.basename(sys.argv[0])

    #TODO read sheet once and split, keeping date index
    header = pandas.read_excel(io=args.file, sheet_name=args.sheet,
                               index_col=0,  nrows=SITE_ROW, )

    data = pandas.read_excel(io=args.file, sheet_name=args.sheet,
                             index_col=0, parse_dates=True, skiprows=SITE_ROW)

    COL=0 #replace with column iterator

    db.agen = header[args.sheet[0]].loc[AGEN_ROW][COL]
    db.collect = header[args.sheet[0]].loc[COLLECT_ROW][COL]
    db.method = header[args.sheet[0]].loc[METHOD_ROW][COL]
    db.comp = header[args.sheet[0]].loc[COMP_ROW][COL]

    site_name = header[args.sheet[0]].iloc[SITE_ROW-1][COL]
    sdi = db.lookup_sdi(site_name, args.sheet[0])
    if sdi is None:
        db.hdbdie(f"Lookup of sdi for site: {site_name} datatype code: {args.sheet[0]} failed!")

    one_col = data[args.sheet[0]][[site_name]].copy().dropna()

    db.write_xfer(db.get_app_ids() | {'sdi': sdi, 'inter': header[args.sheet[0]].loc([INT_ROW][COL]),
                                      'overwrite_flag': None, 'val': None},
                  one_col.index.to_list(), one_col[site_name].to_list())

    #db.commit()

    print('Loading complete at ' + datetime.now().strftime('%c'))


if __name__ == '__main__':
    main(sys.argv[:])
