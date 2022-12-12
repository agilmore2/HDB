'''
Testing ts_xfer package for CUL loading
'''

from datetime import date
import cx_Oracle
import os
import stat


class Hdb(object):
    def __init__(self):
        self.conn = None
        self.dbname = None
        self.agen = 'Bureau of Reclamation'
        self.collect = 'See loading application'
        self.app = None
        self.method = 'unknown'
        self.comp = 'unknown'
        self.agen_id = None
        self.collect_id = None
        self.app_id = None
        self.method_id = None
        self.comp_id = None

    def parse_login(self,authfile):
        """Read authentication file and ensure restrictive permissions

        Authfile Format
        should have three to five lines:
         username <username>
         password <password>
         database <databasename>
         and optionally
         hostname <hostname>
         port <TCP port for listener>
        Will fail if file contains anything else.
        """
        with open(authfile) as file:  # this will error if user does not have permission
            st = os.stat(authfile)
            if st.st_mode & (stat.S_IRGRP | stat.S_IROTH):
                print(f'{authfile} has incorrect permissions! Should not be readable by group or others')
                exit(1)

            auth = {'port': 1521} # default Oracle DB port
            for line in file:
                k, v = line.split(' ')
                if (k == "username" or k == "password" or k == "database"
                 or k == 'port' or k == 'hostname'):
                    auth[k] = v[:-1] # has newlines in it
                else:
                    print(f'Unrecognized line in {authfile}: {line}')
                    exit(1)

            if auth['hostname'] is None: #default to database name
                auth['hostname'] = auth['database']

        if len(auth) != 5:
            self.hdbdie(f"Missing information in authfile: {authfile}")

        self.dbname = auth['database']

        return auth

    def connect_to_db(self, auth):
        """Get a connection to the Oracle database."""
        try:
            self.conn = cx_Oracle.connect(user=auth['username'], password=auth['password'],
                                          dsn=cx_Oracle.makedsn(host=auth['hostname'],
                                                                port=1521,
                                                                sid=auth['database']))
        except Exception as ex:
            print(ex)
            exit(1)

        return self.conn

    def connect_from_file(self, authfile):
        """Connect to database as defined in authfile"""
        if self.conn:
            self.hdbdie(f"Attempted to connect while already connected to {self.dbname}")
        auth = self.parse_login(authfile)
        self.conn = self.connect_to_db(auth)

    def hdbdie(self, message):
        if self.conn and self.conn.ping():
            self.conn.rollback()
            self.conn.close()
        print(message)
        exit(1)

    def write_xfer(self, app_key, dates, values, dataflag=None):
        """Writes a single timeseries to the database

        This writes (using the given `cursor`) the timeseries represented by dates and values arrays
        Needs all metadata shown below in the app_key or meta_key dicts
        Using TS_XFER.write_real_data procedure with inputs
          sdi IN NUMBER
        , INTERVAL IN hdb_interval.interval_name%TYPE
        , dates IN DATEARRAY
        , ts_values IN number_array
        , agen_id NUMBER
        , overwrite_flag VARCHAR2
        , VALIDATION CHAR
        , COLLECTION_SYSTEM_ID NUMBER
        , LOADING_APPLICATION_ID NUMBER
        , METHOD_ID NUMBER
        , computation_id NUMBER
        , do_update_y_n VARCHAR2
        , data_flags IN VARCHAR2 DEFAULT NULL
        , TIME_ZONE IN VARCHAR2 DEFAULT NULL
        )

        Ignoring the optional timezone field

        """

        proc = ("begin cuhdba.ts_xfer.write_real_data(:sdi,:inter,:dates,:values,:agen_id,:overwrite_flag,"
                ":val,:collect_id,:app_id,:method_id,:comp_id,'Y',:data_flags); end;")

    #    example call with single date/value procedure
    #    proc_one = ("begin cuhdba.modify_r_base(:sdi,:inter,:dates,:edate,:values,:agen  Returns number of values written._id,:overwrite_flag  ,"
    #    ":val,:collect_id,:app_id,:method_id,:comp_id,'Y'); end;")

        #these are CASE sensitive!
        date_type = self.conn.gettype("DATEARRAY")
        val_type = self.conn.gettype("NUMBER_ARRAY")

        dates = date_type.newobject(dates)
        values = val_type.newobject(values)

        with self.conn.cursor() as cursor:
            try:
                #cursor.execute(proc_one, app_key|meta_key|{'dates':dates,'edate':None,'values':values})
                cursor.execute(proc, app_key | {'dates': dates, 'values': values, 'data_flags' : dataflag})
            except Exception as ex:
                self.conn.rollback()
                print(ex)
                print(proc)
                print(app_key | {'dates': len(dates), 'values': len(values)})

    REPS = 12

    def ruler(self, sep=' ', reps=REPS):
        for i in range(reps):
            print(str(i) + ' ' * 4 + sep + ' ' * 3, sep='', end='')
        print()
        print('0123456789' * reps)

    def rollback(self):
        self.conn.rollback()

    def commit(self):
        self.conn.commit()

    def get_app_ids(self):
        #if self.app_id is None:
        q=("BEGIN"
           "    lookup_application(:agen,:collect,:app,:method,:comp, /*names*/"
           "                       :agen_id,:collect_id,:app_id,:method_id,:comp_id); /* ids */"
           "END;")
        with self.conn.cursor() as cursor:
            try:
                self.agen_id = cursor.var(int)
                self.collect_id = cursor.var(int)
                self.app_id = cursor.var(int)
                self.method_id = cursor.var(int)
                self.comp_id = cursor.var(int)

                cursor.execute(q, {k: vars(self)[k] for k in
                                   ('agen', 'collect', 'app', 'method', 'comp',
                                    'agen_id', 'collect_id', 'app_id', 'method_id', 'comp_id')})
            except Exception as ex:
                self.conn.rollback()
                print(ex)
                self.hdbdie("Errors occurred during selection of application IDs!")

        return {k: vars(self)[k] for k in ('agen_id', 'collect_id', 'app_id', 'method_id', 'comp_id')}

    def lookup_sdi_ext_data_code(self, site, sheet, datacodesys):
        q=("select site_datatype_id FROM "
           "hdb_ext_data_code_sys NATURAL JOIN hdb_ext_data_code data, "
           "hdb_site_datatype NATURAL JOIN hdb_site sdi "
           "WHERE lower(site_name) = lower(:site) and primary_data_code = :sheet AND "
           "hdb_datatype_id = datatype_id AND "
           "ext_data_code_sys_name = :datacodesys")
        with self.conn.cursor() as cursor:
            try:
                cursor.execute(q, {'sheet': sheet, 'site': site,'datacodesys': datacodesys})

            except Exception as ex:
                self.conn.rollback()
                print(ex)
                self.hdbdie("Errors occurred during selection of SDI!")

            return cursor.fetchone()[0]

def main():
    '''Just for testing.'''
    pass

if __name__ == '__main__':
    main()
