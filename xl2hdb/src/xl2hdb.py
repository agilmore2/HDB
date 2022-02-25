#!/usr/bin/python3
'''HDB writing from XLSX files, and more?

Built against Python 3.9, Pillow 2.0.0, and cx_Oracle 5.1.2

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
HEADER_ROWS = 5

def main(args):
    parser = argparse.ArgumentParser()
    parser.add_argument('-a', help='app_login containing database credentials', required=True)
    parser.add_argument('-f', '--file', help='File to load from', required=True)
    parser.add_argument('-s', '--sheet', help='Sheet to load from', required=True)
    parser.add_argument('-v', '--verbose', action='count', default=0, help='Show more detail about process')
    args = parser.parse_args()
    print(args)
    db = Hdb()
    db.connect_from_file(args.a)
    db.app = os.path.basename(sys.argv[0])

    header = pandas.read_excel(io=args.file, sheet_name=args.sheet,
                               index_col=0, usecols=[0,1], header=None, names=['field','value'],nrows=HEADER_ROWS)

    data = pandas.read_excel(io=args.file, sheet_name=args.sheet,
                             index_col=0, parse_dates=True, skiprows=HEADER_ROWS)

    interval = header.loc[INT_ROW]['value']
    db.agen = header.loc[AGEN_ROW]['value']
    db.collect = header.loc[COLLECT_ROW]['value']
    db.method = header.loc[METHOD_ROW]['value']
    db.comp = header.loc[COMP_ROW]['value']                   

    for site_name in data:
        dt_list = data[site_name].dropna().index.tolist()
        val_list = data[site_name].dropna().tolist()
        sdi = db.lookup_sdi(site_name, args.sheet)
        if sdi is None:
            db.hdbdie(f"Lookup of sdi for site: {site_name} datatype code: {args.sheet[0]} failed!")

        db.write_xfer(db.get_app_ids() | {'sdi': sdi, 'inter': interval,
                                      'overwrite_flag': None, 'val': None},
                  dt_list, val_list)

    if input('Commit? y/n: ') == 'y':
        db.commit()

    print('Loading complete at ' + datetime.now().strftime('%c'))


if __name__ == '__main__':
    main(sys.argv[:])
