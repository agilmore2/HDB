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
spool /usr/local/oracle/admin/newhdb/create/createNEWHDB.log

Rem              
Rem To start up the Oracle instance defined in initNEWHDB.ora file: 
Rem             
Rem connect internal
Rem startup pfile=/usr/local/oracle/admin/newhdb/create/initNEWHDB.ora nomount
Rem @$ORACLE_HOME/dbs/createNEWHDB.sql
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

 create database newhdb
        maxlogfiles    12
        maxdatafiles  100
        maxlogmembers   5
	character set "US7ASCII"
        noarchivelog
 datafile '/ora3/oradata/newhdb/newhdb_sys/newhdb_sys.dbf'  size 100m
 logfile group 1('/ora1/oradata/newhdb/newhdb_redoa/newhdb_redo1a.dbf',
             '/ora2/oradata/newhdb/newhdb_redob/newhdb_redo1b.dbf',
             '/ora3/oradata/newhdb/newhdb_redoc/newhdb_redo1c.dbf') size 1M,
     group 2('/ora1/oradata/newhdb/newhdb_redoa/newhdb_redo2a.dbf',
             '/ora2/oradata/newhdb/newhdb_redob/newhdb_redo2b.dbf', 
             '/ora3/oradata/newhdb/newhdb_redoc/newhdb_redo2c.dbf') size 1M,
     group 3('/ora1/oradata/newhdb/newhdb_redoa/newhdb_redo3a.dbf',
             '/ora2/oradata/newhdb/newhdb_redob/newhdb_redo3b.dbf',
             '/ora3/oradata/newhdb/newhdb_redoc/newhdb_redo3c.dbf') size 1M;

Rem
Rem Create dummy rollback segments in SYSTEM tablespace
Rem

create rollback segment HDB_rbs0
       tablespace SYSTEM
       storage (initial 20K next 20K optimal 50K);
alter rollback segment HDB_rbs0 online;

Rem 
Rem Create tablespaces
Rem

create tablespace HDB_rbs
datafile '/ora3/oradata/newhdb/newhdb_rbs/newhdb_rbs.dbf' size 4m
autoextend on maxsize 50M
default storage ( initial 1200k next 400K);

create tablespace HDB_user
datafile '/ora1/oradata/newhdb/newhdb_users/newhdb_user.dbf' size 5M;

create tablespace HDB_temp temporary
datafile '/ora3/oradata/newhdb/newhdb_temp/newhdb_temp.dbf' size 200M
autoextend on next 10M
maxsize 500M;

Rem
Rem Create real rollback segments
Rem
                                         
create public rollback segment HDB_rbs1
       tablespace HDB_rbs
       storage (initial 40k next 40k minextents 10 maxextents 90
                optimal 400k);

create public rollback segment HDB_rbs2
       tablespace HDB_rbs
       storage (initial 40k next 40k minextents 10 maxextents 90
                optimal 400k);

create public rollback segment HDB_rbs3
       tablespace HDB_rbs
       storage (initial 40k next 40k minextents 10 maxextents 90
                optimal 400k);

Rem
Rem Bring the system rollback segment offline
Rem and active the real rollback segments
Rem
 
alter rollback segment HDB_rbs1 online;
alter rollback segment HDB_rbs2 online;
alter rollback segment HDB_rbs3 online;
alter rollback segment HDB_rbs0 offline;


Rem
Rem  Create tablespace for HDB_data
Rem

create tablespace HDB_data
datafile '/ora1/oradata/newhdb/newhdb_data/newhdb_data.dbf' size 1000M
autoextend on next 50M
maxsize 1500M;

Rem 
Rem  Create index tablespace HDB_idx
Rem

create tablespace HDB_idx
datafile '/ora2/oradata/newhdb/newhdb_idx/newhdb_idx.dbf' size 300M
autoextend on next 50M
maxsize 500M;

Rem  Assign user sys and system to HDB tablespace 
Rem  to avoid the default to SYSTEM tablespace
Rem

alter user sys identified by newhdb
temporary tablespace HDB_temp;

alter user system identified by newhdb
temporary tablespace HDB_temp;

Rem
Rem To shutdown the database
Rem
Rem shutdown

spool off

spool /usr/local/oracle/admin/newhdb/create/sql_scripts.log

--  Create the system dictionaries (from Diep Le Tran's script)
@$ORACLE_HOME/rdbms/admin/catalog.sql
@$ORACLE_HOME/rdbms/admin/catexp.sql
@$ORACLE_HOME/rdbms/admin/cataudit.sql
@$ORACLE_HOME/rdbms/admin/catblock.sql
@$ORACLE_HOME/rdbms/admin/catproc.sql
@$ORACLE_HOME/rdbms/admin/catdbsyn.sql

spool off

