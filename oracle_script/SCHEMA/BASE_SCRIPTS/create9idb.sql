/******************************************************************
Rem   14-JAN-1997
Rem   Create HDB database for Hoover Dam 
Rem   by Diep Le Tran - Oracle Corporation
Rem
Rem   Edited by Kelly Bourke - 9/28/99
Rem
Rem UC:  Data: 500   Index: 750
Rem LC:  Data: 400   Index: 600
Rem Update sizes and max sizes for UC and LC on index and data
*******************************************************************/
set echo on
spool /u01/ora/oracle/admin/9idb/create/createALBHDB.log

Rem              
Rem To start up the Oracle instance defined in initNEWHDB.ora file: 
Rem             
Rem connect internal
Rem startup pfile=/usr/local/oracle/admin/9idb/create/initNEWHDB.ora nomount
Rem @$ORACLE_HOME/dbs/createALBHDB.sql
Rem
Rem After creating the db -> create system dictionaries:
Rem
Rem   @$ORACLE_HOME/rdbms/admin/catalog
Rem   @$ORACLE_HOME/rdbms/admin/catexp
Rem   @$ORACLE_HOME/rdbms/admin/cataudit
Rem   @$ORACLE_HOME/rdbms/admin/catblock
Rem   @$ORACLE_HOME/rdbms/admin/catproc
Rem   @$ORACLE_HOME/rdbms/admin/catdbsyn
Rem

 create database 9idb
        maxlogfiles    12
        maxdatafiles  100
        maxlogmembers   5
	character set "US7ASCII"
        noarchivelog
 datafile '/u02/oradata/9idb/9idb_sys/9idb_sys.dbf'  size 250m
 undo tablespace UNDOTS
datafile '/oradata/oradata/9idb/9idb_undots/undots_ts_1.dbf' size 500M
autoextend off
logfile group 1('/u02/oradata/9idb/9idb_redoa/9idb_redo1a.dbf',
             '/u02/oradata/9idb/9idb_redob/9idb_redo1b.dbf',
             '/u02/oradata/9idb/9idb_redoc/9idb_redo1c.dbf') size 1M,
     group 2('/oradata/oradata/9idb/9idb_redoa/9idb_redo2a.dbf',
             '/oradata/oradata/9idb/9idb_redob/9idb_redo2b.dbf', 
             '/oradata/oradata/9idb/9idb_redoc/9idb_redo2c.dbf') size 1M;


Rem 
Rem Create tablespaces
Rem
create tablespace HDB_user
datafile '/u02/oradata/9idb/9idb_users/9idb_user.dbf' size 5M;

create tablespace HDB_temp temporary
datafile '/u02/oradata/9idb/9idb_temp/9idb_temp.dbf' size 200M
autoextend on next 10M
maxsize 500M;

Rem
Rem  Create tablespace for HDB_data
Rem

create tablespace HDB_data
datafile '/ora/pnhdbII/data_datafile/pnhdbII_data_1.dbf' size 5000M
EXTENT MANAGEMENT LOCAL uniform size 256K;

 
Rem
Rem  Create index tablespace HDB_idx
Rem
 
create tablespace HDB_idx
datafile '/ora/pnhdbII/index_datafile/pnhdbII_idx_1.dbf' size 3000M
EXTENT MANAGEMENT LOCAL uniform size 256K;

Rem  Assign user sys and system to HDB tablespace 
Rem  to avoid the default to SYSTEM tablespace
Rem

alter user sys identified by sys 
temporary tablespace HDB_temp;

alter user system identified by system
temporary tablespace HDB_temp;

Rem
Rem To shutdown the database
Rem
Rem shutdown

spool off

spool /u01/ora/oracle/admin/9idb/create/sql_scripts.log

--  Create the system dictionaries (from Diep Le Tran's script)
@$ORACLE_HOME/rdbms/admin/catalog.sql
@$ORACLE_HOME/rdbms/admin/catexp.sql
@$ORACLE_HOME/rdbms/admin/cataudit.sql
@$ORACLE_HOME/rdbms/admin/catblock.sql
@$ORACLE_HOME/rdbms/admin/catproc.sql
@$ORACLE_HOME/rdbms/admin/catdbsyn.sql

spool off

