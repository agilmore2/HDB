set echo off
set heading off
set feedback off
set pagesize 0
set linesize 200
set verify off

spool drop_snap_fk_script.sql

select 'ALTER TABLE '||table_name||' DROP CONSTRAINT '||constraint_name||';'
from all_constraints
where r_constraint_name like 'HDB_%_PK'
and constraint_name like '%FK%'
order by table_name;


spool off;
set feedback on
@drop_snap_fk_script
quit

