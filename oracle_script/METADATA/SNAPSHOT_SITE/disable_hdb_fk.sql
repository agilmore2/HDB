set echo off
set heading off
set feedback off
set pagesize 0
set linesize 200
set verify off

spool disable_hdb_fk_script.sql

select 'ALTER TABLE '||table_name||' DISABLE CONSTRAINT '||constraint_name||';'
from all_constraints where r_constraint_name like 'HDB_%_PK'
order by table_name;

spool off;
set feedback on
@disable_hdb_fk_script
quit

