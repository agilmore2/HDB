'''This is a port of readdata.pc and parts of gentea.c (from USBR)

Built against Python 2.7.4 and cx_Oracle 5.1.2

Created on Apr 15, 2013

@author: hsk@RTi
'''

import cx_Oracle

def get_connection(user, passwd, host, port, sid):
    '''Get a connection to the Oracle database.'''

    conn = cx_Oracle.connect(user=user, password=passwd, 
                             dsn=cx_Oracle.makedsn(host=host,
                                                   port=port,
                                                   sid=sid))
    return conn

def readone(cursor, site_id=1, datatype_id=1, site_datatype_id=None):
    '''Read a single value from the database.

    This reads (using the given `cursor`) the single value with the most recent
    timestamp (START_DATE_TIME) that matches the given `site_id` and 
    `datatype_id`. If `site_datatype_id` is given, that will be used in 
    preference to (`site_id` and `datatype_id`).

    Returns string timestamp and float value as ('yyyy/mm/dd', value).
    '''
    if site_datatype_id is not None:
        query = ("SELECT TRIM(TO_CHAR(START_DATE_TIME, 'YYYY/MM/DD')), VALUE "
                 "FROM R_DAY "
                 "WHERE SITE_DATATYPE_ID = %s "
                 "AND START_DATE_TIME = (SELECT MAX(START_DATE_TIME) FROM "
                                         "R_DAY WHERE SITE_DATATYPE_ID = %s)")
        query = query % (site_datatype_id, site_datatype_id)
    else:
        query = ("SELECT TRIM(TO_CHAR(START_DATE_TIME, 'YYYY/MM/DD')), VALUE "
                 "FROM HDB_SITE_DATATYPE a, R_DAY b "
                 "WHERE SITE_ID = %s AND DATATYPE_ID = %s "
                 "AND B.SITE_DATATYPE_ID = A.SITE_DATATYPE_ID "
                 "AND B.START_DATE_TIME = (SELECT MAX(START_DATE_TIME) FROM "
                                "HDB_SITE_DATATYPE A, R_DAY B "
                                "WHERE SITE_ID = %s AND DATATYPE_ID = %s "
                                "AND B.SITE_DATATYPE_ID = A.SITE_DATATYPE_ID)")
        query = query % (site_id, datatype_id, site_id, datatype_id)
    cursor.execute(query)
    results = cursor.fetchall()
    if results:
        return results[0]
    raise ValueError('No results found for query "%s"' % query)


def main():
    '''Just for testing. The main module is gentea.py.'''

    conn = get_connection('riverside', 'changeme2012', 
                                    'XXX.XXX.XX.200', '1521', 'echdbdev')
    cursor = conn.cursor()
    print "results:\n\t", readone(cursor, site_datatype_id=100373)

if __name__ == '__main__':
    main()
