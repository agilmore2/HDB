set pagesize 0
set heading off
set feedback off
set verify off
set echo off

spool alter_tabletriggers.sql

select 'ALTER trigger ' || trigger_name || ' ' || upper('&1') || ';'
from user_triggers where table_name  like upper('&2'); 

spool off

@alter_tabletriggers
