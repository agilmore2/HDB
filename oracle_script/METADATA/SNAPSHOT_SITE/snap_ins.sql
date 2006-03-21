-- Snapshot site installation script
-- Revised 2/03 to automatically include all hdb_ tables, rather
-- than requiring manual maintenance when and hdb_ table is added to the
-- scheam.
-- Arguments:
--   &1 master db name
--   &2 master meta data user password
--   &3 master DBA name
REM prompt 'Snapshot site installation script. ';

set echo off
set heading off
set feedback off
set pagesize 0
set linesize 200
set verify off

spool snap_ins_script.sql

-- The creation of this database link assumes a connect descriptor of
-- 'uchdb' or 'uchdb.world' for "logging in" to the Upper Colorado HDB
-- That is, if tnsnames.ora is being used, the connect info should have 
-- a descriptor of 'uchdb' in order for this to work.
-- This database link can be used by any user, but every user will 
-- connect to the linked database as meta_data_user. Hence, no new
-- users need be created on the remote DB in order to accomodate new
-- users on the local DB.
select 'DROP PUBLIC DATABASE LINK &1;' from dual;

select 'CREATE PUBLIC DATABASE LINK &1 CONNECT TO meta_data_user IDENTIFIED BY &2 USING ''&1'';'
from dual;

select ' ' from dual;
-- Rename HDB_ tables
select 'rename ' || table_name || ' to ' || table_name || '_OLD;'
from dba_tables 
where table_name like 'HDB_%'
and table_name not like '%_OLD'
order by table_name;

select ' ' from dual;
-- Drop misc triggers to be dropped.
select 'DROP TRIGGER sdi_chk_perm;' from dual;
select 'DROP TRIGGER unit_chk_val_spec;' from dual;
select 'DROP TRIGGER hdb_site_site_perm;' from dual;

select ' ' from dual;
-- Drop public synonyms for the old HDB tables
select 'drop public synonym ' || synonym_name || ';'
from dba_synonyms where synonym_name like 'HDB_%';

select ' ' from dual;
-- Create snapshots based on the Master HDB tables
select 'CREATE SNAPSHOT ' || table_name ||
       ' PCTFREE 10 PCTUSED 40 TABLESPACE HDB_DATA storage (initial 50k next 50k pctincrease 0) ' ||
       ' AS SELECT * FROM  &3..' || table_name || '@&1;'
from dba_tables
where table_name like 'HDB_%'
and table_name not like '%_OLD'
order by table_name;

select ' ' from dual;
-- Create public synonyms for HDB snapshots
select 'CREATE PUBLIC SYNONYM ' || table_name || ' FOR ' || table_name ||
       ';'
from dba_tables
where table_name like 'HDB_%'
and table_name not like '%_OLD'
order by table_name;

select ' ' from dual;
-- Grant select rights on the snapshots
select 'GRANT SELECT ON ' || table_name || ' TO PUBLIC' || ';'
from dba_tables
where table_name like 'HDB_%'
and table_name not like '%_OLD'
order by table_name;

select ' ' from dual;
--  Create synonyms for use by meta-data app; these refer to master tables
select 'CREATE PUBLIC SYNONYM ' || substr(table_name,1,26) || '_SYN FOR &3..' || table_name ||
       '@&1;'
from dba_tables
where table_name like 'HDB_%'
and table_name not like '%_OLD'
order by table_name;

select ' ' from dual;
-- Create synonym for procedure on master
select 'create public synonym grant_meta_data_user for &3..grant_meta_data_user@&1;'
from dual;

spool off;
set feedback on
@snap_ins_script
quit


