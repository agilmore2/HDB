#!/usr/bin/env python3
"""
hdb2report.py - Export HDB timeseries for a named ext_data_source.

Data source configuration lives in hdb_ext_data_source / ref_ext_site_data_map.
Writes one file per hdb_interval_name present in the data source, named
<datasource>_<interval>.<ext>, into the current directory. Each file is a
single pivoted table: one row per timestamp, one column per
(primary_site_code, primary_data_code) pair across every site in that
interval. Timestamps are shown only for sub-daily intervals (instant, hour,
other).

Usage:
  hdb2report.py -a <authfile> -s <datasource> [-n <days>] [-b YYYY-MM-DD] [-e YYYY-MM-DD]
                [-i <site_code>] [-f space|csv|html]
"""

import argparse
import csv
import os
import re
import sys
from collections import defaultdict
from datetime import datetime, timedelta

import pandas as pd

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..'))
from lib.hdb import Hdb

SUB_DAILY = {'instant', 'hour', 'other'}
COL_WIDTH  = 12
DT_FMT     = '%Y-%m-%d %H:%M'
DATE_FMT   = '%Y-%m-%d'
FORMAT_EXT = {'space': 'txt', 'csv': 'csv', 'html': 'html'}


def parse_args():
    p = argparse.ArgumentParser(
        description='Report HDB timeseries for a named ext_data_source'
    )

    class ValidateDate(argparse.Action):
        def __call__(self, parser, namespace, values, option_string=None):
            try:
                setattr(namespace, self.dest, datetime.strptime(str(values), '%Y-%m-%d').date())
            except ValueError:
                parser.error(f'Invalid date for {option_string}: {values!r}. Expected YYYY-MM-DD.')

    p.register('action', 'validate_date', ValidateDate)

    p.add_argument('-a', '--authfile', required=True,
                   help='HDB auth file')
    p.add_argument('-s', '--source', required=True, metavar='DATASOURCE',
                   help='ext_data_source_name to report')
    p.add_argument('-n', '--numdays', metavar='N',
                   help='number of days to report')
    p.add_argument('-b', '--begin', action='validate_date', metavar='YYYY-MM-DD',
                   help='begin date (inclusive)')
    p.add_argument('-e', '--end', action='validate_date', metavar='YYYY-MM-DD',
                   help='end date (inclusive)')
    p.add_argument('-i', '--site_code', action='append', metavar='SITE_CODE',
                   help='filter by primary_site_code (repeatable)')
    p.add_argument('-f', '--format', choices=['space', 'csv', 'html'], default='space',
                   help='output format (default: space)')
    return p.parse_args()


def determine_date_range(args):
    today = datetime.today().replace(hour=0, minute=0, second=0, microsecond=0)
    numdays = int(args.numdays) if args.numdays else None

    def to_dt(d):
        return datetime(d.year, d.month, d.day)

    begin = to_dt(args.begin) if args.begin else None
    end   = to_dt(args.end)   if args.end   else None

    if not begin and not end:
        end = today
        if numdays:
            begin = end - timedelta(days=numdays - 1)
        else:
            print('Error: specify at least one of -b, -e, or -n.', file=sys.stderr)
            sys.exit(1)
    elif begin and not end:
        end = begin + timedelta(days=numdays - 1) if numdays else today
    elif end and not begin:
        if numdays:
            begin = end - timedelta(days=numdays - 1)
        else:
            print('Error: only end date given; use -n or -b to define the start.', file=sys.stderr)
            sys.exit(1)
    else:
        if numdays:
            print('Error: -b, -e, and -n all specified.', file=sys.stderr)
            sys.exit(1)

    return begin, end


def get_site_intervals(hdb, datasource, site_codes=None):
    params = {'datasource': datasource}
    site_filter = ''
    if site_codes:
        placeholders = ', '.join(f':s{i}' for i in range(len(site_codes)))
        site_filter  = f'AND m.primary_site_code IN ({placeholders})'
        params.update({f's{i}': code for i, code in enumerate(site_codes)})

    return hdb.query(f"""
        SELECT DISTINCT m.primary_site_code, m.hdb_interval_name
        FROM ref_ext_site_data_map m
        JOIN hdb_ext_data_source d ON d.ext_data_source_id = m.ext_data_source_id
        WHERE LOWER(d.ext_data_source_name) = LOWER(:datasource)
        AND m.is_active_y_n = 'Y'
        {site_filter}
        ORDER BY m.primary_site_code, m.hdb_interval_name
    """, params)


def get_columns(hdb, datasource, site, interval):
    return hdb.query("""
        SELECT m.primary_data_code,
               u.unit_common_name,
               m.hdb_site_datatype_id,
               COALESCE(TO_NUMBER(kv.key_value), m.mapping_id) AS col_order
        FROM ref_ext_site_data_map m
        LEFT JOIN ref_ext_site_data_map_keyval kv
               ON kv.mapping_id = m.mapping_id AND kv.key_name = 'Column Order'
        JOIN hdb_ext_data_source d           ON d.ext_data_source_id = m.ext_data_source_id
        JOIN hdb_site_datatype sd            ON sd.site_datatype_id = m.hdb_site_datatype_id
        JOIN hdb_datatype dt                 ON dt.datatype_id = sd.datatype_id
        JOIN hdb_unit u                      ON u.unit_id = dt.unit_id
        WHERE LOWER(d.ext_data_source_name) = LOWER(:datasource)
        AND m.primary_site_code = :site
        AND m.hdb_interval_name = :interval
        AND m.is_active_y_n = 'Y'
        ORDER BY col_order
    """, {'datasource': datasource, 'site': site, 'interval': interval})


def get_interval_columns(hdb, datasource, interval, sites):
    """Column metadata for every site in this interval, tagged with site code and a combined column key."""
    columns = []
    for site in sites:
        for col in get_columns(hdb, datasource, site, interval):
            col['unit_common_name']  = fmt_unit(col['unit_common_name'])
            col['primary_site_code'] = site
            col['col_name']          = f"{site}_{col['primary_data_code']}"
            columns.append(col)
    return columns


def build_pivot(hdb, columns, interval, start_dt, end_dt):
    """Fetch each column's timeseries via ts_xfer.get_real_data and pivot into a single DataFrame keyed by col_name."""
    frames = {}
    for col in columns:
        df = hdb.query_ts(col['hdb_site_datatype_id'], interval, start_dt, end_dt)
        frames[col['col_name']] = df.set_index('start_date_time')['value']
    return pd.DataFrame(frames).sort_index(ascending=True)


def fmt_ts(ts, sub_daily):
    return ts.strftime(DT_FMT if sub_daily else DATE_FMT)


def ts_fmt_label(sub_daily):
    return 'YYYY-MM-DD HH:MI' if sub_daily else 'YYYY-MM-DD'


def fmt_unit(unit_common_name):
    return unit_common_name.replace('feet', 'ft')


def slugify(name):
    """Make a data source name safe for use as a filename component."""
    return re.sub(r'[^A-Za-z0-9]+', '_', name).strip('_')


def fmt_val_space(v, width=COL_WIDTH):
    """Right-justify a value into width chars; blank for missing."""
    if pd.isna(v):
        return ' ' * width
    v = float(v)
    if abs(v) < 10000:
        return f'{v:>{width}.2f}'
    return f'{int(v):>{width}d}'


def fmt_val_text(v):
    """Format a value for CSV/HTML; empty string for missing."""
    if pd.isna(v):
        return ''
    v = float(v)
    return f'{v:.2f}' if abs(v) < 10000 else str(int(v))


def provisional_lines(sub_daily):
    lines = ['DATA IS PROVISIONAL AND SUBJECT TO REVISION']
    if sub_daily:
        lines.append('Date/Time is in MST, add 1 hour during MDT')
    return lines


def write_space(out, columns, pivot, sub_daily):
    dt_width = 16 if sub_daily else 10
    dt_label = 'data'

    widths = {
        c['col_name']: max(COL_WIDTH, len(c['primary_site_code']), len(c['primary_data_code']), len(c['unit_common_name']) + 2)
        for c in columns
    }

    site_line = 'site'.ljust(dt_width + 2)
    code_line = dt_label.ljust(dt_width + 2)
    unit_line = ts_fmt_label(sub_daily).ljust(dt_width + 2)
    for col in columns:
        width = widths[col['col_name']]
        unit  = '(' + col['unit_common_name'] + ')'
        site_line += f'{col["primary_site_code"]:>{width}}'
        code_line += f'{col["primary_data_code"]:>{width}}'
        unit_line += f'{unit:>{width}}'
    out.write(site_line.rstrip() + '\n')
    out.write(code_line.rstrip() + '\n')
    out.write(unit_line.rstrip() + '\n\n')

    for ts, row in pivot.iterrows():
        line = fmt_ts(ts, sub_daily).ljust(dt_width + 2)
        line += ''.join(fmt_val_space(row[c['col_name']], widths[c['col_name']]) for c in columns)
        out.write(line.rstrip() + '\n')
    out.write('\n')


def write_csv(out, columns, pivot, sub_daily):
    writer = csv.writer(out)
    dt_label  = 'data'
    col_names = [c['col_name'] for c in columns]

    writer.writerow(['site'] + [c['primary_site_code'] for c in columns])
    writer.writerow([dt_label] + [c['primary_data_code'] for c in columns])
    writer.writerow([ts_fmt_label(sub_daily)] + [c['unit_common_name'] for c in columns])
    for ts, row in pivot.iterrows():
        writer.writerow([fmt_ts(ts, sub_daily)] + [fmt_val_text(row[n]) for n in col_names])
    writer.writerow([])


def write_html(out, columns, pivot, sub_daily):
    dt_label  = 'Data'
    col_names = [c['col_name'] for c in columns]

    out.write('<table border="1" cellpadding="4" style="border-collapse:collapse">\n')
    out.write('  <thead><tr>\n')
    out.write(f'    <th>Site<br>{dt_label}<br><em>{ts_fmt_label(sub_daily)}</em></th>\n')
    for col in columns:
        out.write(f'    <th>{col["primary_site_code"]}<br>{col["primary_data_code"]}<br>{col["unit_common_name"]}</th>\n')
    out.write('  </tr></thead>\n  <tbody>\n')

    for ts, row in pivot.iterrows():
        out.write('  <tr>\n')
        out.write(f'    <td>{fmt_ts(ts, sub_daily)}</td>\n')
        for n in col_names:
            v = fmt_val_text(row[n])
            align = ' style="text-align:right"' if v else ''
            out.write(f'    <td{align}>{v}</td>\n')
        out.write('  </tr>\n')

    out.write('  </tbody>\n</table>\n\n')


def main():
    args = parse_args()

    hdb = Hdb()
    hdb.connect_from_file(args.authfile)

    start_dt, end_dt = determine_date_range(args)

    site_codes = None
    if args.site_code:
        site_codes = [c for arg in args.site_code for c in arg.split(',')]

    site_intervals = get_site_intervals(hdb, args.source, site_codes)
    if not site_intervals:
        print(f'No active mappings found for data source: {args.source!r}', file=sys.stderr)
        sys.exit(1)

    sites_by_interval = defaultdict(list)
    for row in site_intervals:
        sites_by_interval[row['hdb_interval_name']].append(row['primary_site_code'])

    source_slug = slugify(args.source)

    for interval, sites in sites_by_interval.items():
        sub_daily = interval.lower() in SUB_DAILY

        columns = get_interval_columns(hdb, args.source, interval, sites)
        if not columns:
            continue

        pivot = build_pivot(hdb, columns, interval, start_dt, end_dt)

        out = open(f'{source_slug}_{interval}.{FORMAT_EXT[args.format]}', 'w')

        try:
            if args.format == 'space':
                out.write(f'Data from {args.source}\n')
                for line in provisional_lines(sub_daily):
                    out.write(line + '\n')
                out.write('\n')
                write_space(out, columns, pivot, sub_daily)
            elif args.format == 'csv':
                out.write(f'# Data from {args.source}\n')
                for line in provisional_lines(sub_daily):
                    out.write(f'# {line}\n')
                write_csv(out, columns, pivot, sub_daily)
            elif args.format == 'html':
                out.write('<!DOCTYPE html>\n<html>\n<head>\n')
                out.write('<meta charset="utf-8">\n')
                out.write(f'<title>{args.source} &mdash; {interval}</title>\n')
                out.write('</head>\n<body>\n')
                out.write(f'<h1>Data from {args.source}</h1>\n')
                for line in provisional_lines(sub_daily):
                    out.write(f'<p><em>{line}</em></p>\n')
                write_html(out, columns, pivot, sub_daily)
                out.write('</body>\n</html>\n')

        finally:
            out.close()


if __name__ == '__main__':
    main()
