-- Snapshot site installation script
-- Revised 2/03 to automatically include all hdb_ tables, rather
-- than requiring manual maintenance when and hdb_ table is added to the
-- schema.

REM prompt 'Snapshot site de-installation script. <hit return to continue>';


set echo off
set heading off
set feedback off
set pagesize 0
set linesize 100
set verify off

spool snap_de_script.sql

-- Revoking select rights on the snapshots
select 'REVOKE SELECT ON ' || table_name || ' FROM PUBLIC;'
from dba_tables
where table_name like 'HDB_%'
and table_name not like '%_OLD'
order by table_name;

select ' ' from dual;
-- Drop public synonyms for HDB snapshots
select distinct 'DROP PUBLIC SYNONYM ' || table_name || ';'
from dba_synonyms
where owner = 'PUBLIC'
and table_name like 'HDB_%'
and table_name not like '%_OLD';


select ' ' from dual;
-- Dropping snapshots based on the Master HDB tables
select 'DROP SNAPSHOT ' || table_name || ';'
from dba_snapshots
where name like 'HDB_%'
order by name;

select ' ' from dual;
-- Dropping public synonyms for Master HDB tables
select 'DROP PUBLIC SYNONYM ' || synonym_name || ';'
from dba_synonyms
where synonym_name like 'HDB_%_SYN'
order by synonym_name;

select ' ' from dual;
-- Renaming the old HDB tables to original names
select 'RENAME ' || table_name || ' to ' || 
       substr (table_name, 1, length (table_name) - 4) || ';'
from dba_tables
where table_name like 'HDB_%_OLD'
order by table_name;

select ' ' from dual;
-- Creating the public synonyms for the HDB tables
select 'CREATE PUBLIC SYNONYM ' || table_name || ' for ' || table_name || ';'
from dba_tables
where table_name like 'HDB_%'
and table_name not like 'HDB_%_OLD'
order by table_name;

select ' ' from dual;
-- Granting select rights on the original HDB tables
select 'GRANT SELECT ON ' || table_name || ' TO PUBLIC;'
from dba_tables
where table_name like 'HDB_%'
and table_name not like 'HDB_%_OLD'
order by table_name;

select ' ' from dual;
select 'drop public synonym grant_meta_data_user;'
from dual;

REM PROMPT 'Remember to run scripts to restore original foreign keys!!! ';

spool off;
set feedback on
@snap_de_script
quit