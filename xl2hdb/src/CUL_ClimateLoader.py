from datetime import datetime
import sys
import argparse
import os
import pandas as pd
from hdb import Hdb

def main(args):
    parser = argparse.ArgumentParser(description='Program to load monthly average temperatures and cumulative precip for many WRCC climate sites used by UC CUL computations')
    parser.add_argument('-a', '--authFile', help='app_login containing database credentials', required=True)
    parser.add_argument('-m','--startMonth',help='start month (m/yyyy) to begin loading data for. Defaults to January of the current year')
    parser.add_argument('-s','--site',help='numeric ID (ex. 020750) of single site to load data for. Defaults to all sites')
    parser.add_argument('--verbose', action='store_true', help='Show more detail about process')
    parser.add_argument('--NoOverwrite', action='store_true', help='Do not write an O to the overwrite_flag field')
    args = parser.parse_args()

    db = Hdb()
    db.connect_from_file(args.authFile)
    db.app = os.path.basename(sys.argv[0])
    db.agen = 'Western Regional Climate Center'
    db.collect = 'NOAA coop station'
    db.method = 'unknown'
    db.comp = 'unknown'  

    datatypes = [
        {'wrcc' : 'avgt','comp' : 'mave','hdb' : 'average air temperature'},
        {'wrcc' : 'pcpn','comp' : 'msum','hdb' : 'precipitation (total over period)'}
    ]


    # Load site info dataframe
    allSites = pd.read_csv('CUL_ClimateSites.csv',dtype=str) # treat all values as string to avoid trimming leading zeros

    # fill default arguments
    # Write either None or 'O' to the overwrite flag field
    oFlag = None if args.NoOverwrite else 'O'

    # Select a single row of the allSites dataframe if one is given
    if args.site is not None:
        allSites = allSites[allSites['Numeric ID'] == args.site]
        if len(allSites) == 0:
            raise Exception('no site found with numeric ID {}'.format(args.site))
    debug('--- Collecting data for {} site(s) ---'.format(len(allSites)),args.verbose)

    # Build start date for data to be loaded. Default to Jan of the current year if no argument was provided
    if args.startMonth is None:
        startDate = datetime(datetime.now().year,1,1)
    else:
        try:
            startDate = datetime.strptime(args.startMonth,'%m/%Y')
        except Exception as e:
            print('Error parsing start month. Expecting format m/yyyy')
            raise Exception(e)

    for index,row in allSites.iterrows():
        numericID = row['Numeric ID']
        name = row['Name']
        hdbSite = row['HDB Site']

        for dt in datatypes: # average temp and sum precip
            raw_site_data = pd.DataFrame()
            site_data = pd.DataFrame()
            sdi = getSDI_fromSiteAndDt(db,hdbSite,dt['hdb'])        
            url = 'https://wrcc.dri.edu/WRCCWrappers.py?sodxtrmts+{}+por+por+{}+none+{}+5+01+F'.format(numericID,dt['wrcc'],dt['comp'])
            debug('Site : {} || Datatype : {} || SDI : {}'.format(hdbSite,dt['hdb'],sdi),args.verbose)
            try:
                raw_site_data = pd.read_html(url,header=0,na_values=['-----'])[0]
                site_data = formatWRCCTable(raw_site_data,startDate)
            except Exception as e:
                print('Error retrieving {} data for site: {}'.format(dt['hdb'],hdbSite))
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
                    db.write_xfer(db.get_app_ids() | {'sdi' : sdi,'inter' : 'month','overwrite_flag' : oFlag,'val' : None},dt_list,val_list,f)

                # repeat a similar step for blank data flags
                dt_list = site_data[site_data['flag'].isna()]['data'].dropna().index.tolist()
                val_list = site_data[site_data['flag'].isna()]['data'].dropna().tolist()
                db.write_xfer(db.get_app_ids() | {'sdi' : sdi,'inter' : 'month','overwrite_flag' : oFlag,'val' : None},dt_list,val_list)
    
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

def getSDI_fromSiteAndDt(db,site,dt):
    q=("SELECT sd.site_datatype_id FROM "
        "hdb_site_datatype sd "
        "INNER JOIN hdb_datatype d ON sd.datatype_id = d.datatype_id "
        "INNER JOIN hdb_site s ON sd.site_id = s.site_id "
        "WHERE s.site_name = :site and d.datatype_name = :dt")
    with db.conn.cursor() as cursor:
        try:
            cursor.execute(q, {'site': site, 'dt': dt})

        except Exception as ex:
            db.conn.rollback()
            print(ex)
            db.hdbdie("Errors occurred during selection of SDI!")

        return cursor.fetchone()[0]

if __name__ == '__main__':
    main(sys.argv[:])



