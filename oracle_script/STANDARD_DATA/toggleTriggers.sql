set echo off
set heading off
set feedback off
set pagesize 0
set linesize 100
set verify off

spool ./STANDARD_DATA/trigger_script.sql
select distinct 'ALTER TRIGGER '||trigger_name||upper(' &1')||';'
from user_triggers
/
 
spool off;

set echo on
set feedback on
@./STANDARD_DATA/trigger_script
quit
