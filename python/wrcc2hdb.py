from datetime import datetime
import sys
import argparse
import os
import pandas as pd
from lib.hdb import Hdb

def main(args):
    parser = argparse.ArgumentParser(description='Program to load monthly average temperatures and cumulative precip for many WRCC climate sites used by UC CUL computations')
    parser.add_argument('-a', '--authFile', help='app_login containing database credentials', required=True)
    parser.add_argument('-m','--startMonth',help='start month (m/yyyy) to begin loading data for. Defaults to January of the current year')
    parser.add_argument('-s','--site',help='numeric ID (ex. 020750) of single site to load data for. Defaults to all sites')
    parser.add_argument('-d','--dataSource' ,help='name of data source to get site datatype mappings from',default='WRCC Load For CUL')
    parser.add_argument('--verbose', action='store_true', help='Show more detail about process')
    parser.add_argument('--nooverwrite', action='store_true', help='Do not write an O to the overwrite_flag field')
    args = parser.parse_args()

    db = Hdb()
    db.connect_from_file(args.authFile)
    app = db.get_loadingAppID(os.path.basename(sys.argv[0]))

    url_compstrings = {'avgt' : 'mave','pcpn' : 'msum'} # wrcc computation names used in URL query for temp (average) and precip (sum)
    
    # Load site map info dataframe
    # Many sites are no longer active. Future effort could identify these sites, mark them as not active in the database
    # and modify this code to only try queries at active sites
    SiteDataMap = db.get_siteDataMap(args.dataSource,'month')

    # fill default arguments
    # Write either None or 'O' to the overwrite flag field
    oFlag = None if args.nooverwrite else 'O'

    # Select a single site from the map if one is given
    if args.site is not None:
        SiteDataMap = SiteDataMap[SiteDataMap['PRIMARY_SITE_CODE'] == args.site]
        if len(SiteDataMap) == 0:
            raise Exception('no site found with numeric ID {}'.format(args.site))
    debug('--- Collecting data for {} sdi(s) ---'.format(len(SiteDataMap)),args.verbose)

    # Build start date for data to be loaded. Default to Jan of the current year if no argument was provided
    if args.startMonth is None:
        startDate = datetime(datetime.now().year,1,1)
    else:
        try:
            startDate = datetime.strptime(args.startMonth,'%m/%Y')
        except Exception as e:
            print('Error parsing start month. Expecting format m/yyyy')
            raise Exception(e)

    for index,row in SiteDataMap.iterrows():
        numericID = row['PRIMARY_SITE_CODE']
        hdbSite = row['SITE_NAME']
        sdi = row['HDB_SITE_DATATYPE_ID']
        dt_wrcc = row['PRIMARY_DATA_CODE']
        dt_hdb = row['DATATYPE_NAME']
        comp_wrcc = url_compstrings[dt_wrcc]

        # r_base metadata
        base_meta = {'agen_id' : row['HDB_AGEN_ID'],'collect_id' : row['COLLECTION_SYSTEM_ID'],'app_id' : app,'method_id' : row['HDB_METHOD_ID'],'comp_id' : row['HDB_COMPUTATION_ID']}

        url = 'https://wrcc.dri.edu/WRCCWrappers.py?sodxtrmts+{}+por+por+{}+none+{}+5+01+F'.format(numericID,dt_wrcc,comp_wrcc)
        debug('Site : {} || Datatype : {} || SDI : {}'.format(hdbSite,dt_hdb,sdi),args.verbose)

        raw_site_data = pd.DataFrame()
        site_data = pd.DataFrame()
    

        try:
            raw_site_data = pd.read_html(url,header=0,na_values=['-----'])[0]
            site_data = formatWRCCTable(raw_site_data,startDate)
        except Exception as e:
            print('Error retrieving {} data for site: {}'.format(dt_hdb,hdbSite))
            print('Attempted URL: {}'.format(url))
            print(e)
            #raise Exception(e)
        if len(site_data) == 0:
            debug('No data since {}'.format(datetime.strftime(startDate,'%m/%Y')),args.verbose)
        else:
            debug('Found {} data points starting in {}'.format(len(site_data),datetime.strftime(site_data.index.min(),'%m/%Y')),args.verbose)
            # This logic comes from xl2hdb.py, which can load time series w/ data flags to r_base by loading chunks at a time with consistent data flags
            allFlags = site_data['flag'].dropna().unique().tolist()
            for f in allFlags:
                dt_list = site_data[site_data['flag'] == f]['data'].dropna().index.tolist()
                val_list = site_data[site_data['flag'] == f]['data'].dropna().tolist()
                db.write_xfer(base_meta | {'sdi' : sdi,'inter' : 'month','overwrite_flag' : oFlag,'val' : None},dt_list,val_list,f)

            # repeat a similar step for blank data flags
            dt_list = site_data[site_data['flag'].isna()]['data'].dropna().index.tolist()
            val_list = site_data[site_data['flag'].isna()]['data'].dropna().tolist()
            db.write_xfer(base_meta | {'sdi' : sdi,'inter' : 'month','overwrite_flag' : oFlag,'val' : None},dt_list,val_list)
    
    if input('Commit? y/n: ') == 'y':
        db.commit()
        print('Loading complete at ' + datetime.now().strftime('%c'))

def debug(msg,v):
    if v: print(msg)

def getValueType(row):
    # function used to transfer raw wrcc table headers to value type
    if '.' in row['variable']:
        return 'flag'
    else:
        return 'data'

def getfirstofMonth(row):
    # function used to get month number (1-12) from wrcc table header strings
    month = datetime.strptime(row['variable'].replace('.1',''),'%b').month
    return datetime(int(row['yr']),month,1)

def formatWRCCTable(raw,startDate):
    # raw format has years in the first column, and then two columns for each month (JAN,JAN.1,...DEC,DEC.1,ANN,ANN.1)
    # The first month contains values (or NaN), the second month (Jan.1) column contains a letter or blank, which represents the number of days of missing data
    # The last few rows are summary statistics, to be deleted
    lastRow = raw.index[raw['YEAR(S)'] == 'Period of Record Statistics'].tolist()[0]
    raw = raw.loc[0:lastRow - 1]
    raw = raw.drop(['ANN','ANN.1'],axis=1)
    raw = raw.rename(columns={'YEAR(S)' : 'yr'})
    raw = raw.melt(id_vars=['yr'],value_vars = [s for s in raw.columns.to_list() if s != 'yr'])
    raw['type'] = raw.apply(getValueType,axis=1)
    raw['date'] = raw.apply(getfirstofMonth,axis=1)
    raw = raw.pivot(index='date',columns='type',values='value')
    raw['data'] = pd.to_numeric(raw['data'])

    # drop rows with nan in the data column
    raw = raw[raw['data'].notna()]

    # only return dates after or equal to the supplied start date, with numeric values
    return raw[raw.index >= startDate]

if __name__ == '__main__':
    main(sys.argv[:])



