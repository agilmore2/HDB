set echo off
set heading off
set feedback off
set pagesize 0
set linesize 100
set verify off

spool trigger_script.sql
select distinct 'ALTER TRIGGER '||trigger_name||upper(' &1')||';'
from user_triggers where table_name like 'HDB_%' or table_name like 'REF_%'
/
 
spool off;

set echo on
set feedback on
@trigger_script
quit
