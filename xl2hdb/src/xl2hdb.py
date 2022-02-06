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

    data = pandas.read_excel(io=args.file, sheet_name=args.sheet,
                             index_col=0, parse_dates=True, skiprows=0)

    one_col = data[args.sheet[0]][['CARBON_14060007']].copy().dropna()

    db.write_xfer(db.get_app_ids() | {'sdi': 25575, 'inter': 'month', 'overwrite_flag': None, 'val': None},
                  one_col.index.to_list(), one_col['CARBON_14060007'].to_list())


    #db.commit()

    print('Loading complete at ' + datetime.now().strftime('%c'))

if __name__ == '__main__':
    main(sys.argv[:])
