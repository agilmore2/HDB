set echo off
set heading off
set feedback off
set pagesize 0
set linesize 100
set verify off

spool fk_script.sql
select distinct 'ALTER TABLE '||table_name||upper(' &1')|| ' CONSTRAINT '||constraint_name||';'
from user_constraints where (table_name like 'HDB_%' or table_name like 'REF_%')
and status = decode(upper('&1'),'DISABLE','ENABLED','DISABLED')
/
 
spool off;

set echo on
set feedback on
@fk_script
quit
