set echo off
set heading off
set feedback off
set pagesize 0
set linesize 200
set verify off

spool drop_old_hdb_tables_script.sql

select 'ALTER TABLE ' || a.table_name || ' DROP CONSTRAINT ' ||
a.constraint_name || ';'
from dba_constraints a, dba_constraints b
where a.r_constraint_name = b.constraint_name
and b.table_name like 'HDB_%OLD'
order by a.table_name;

select ' ' from dual;
select 'DROP TABLE ' || a.table_name || ';'
from dba_tables a
where a.table_name like 'HDB_%OLD'
order by a.table_name;
 
spool off;
set feedback on
@drop_old_hdb_tables_script
quit

