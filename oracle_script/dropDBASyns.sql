set echo off
set heading off
set feedback off
set pagesize 0
set linesize 100
set verify off

spool syn_syn_script.sql
select distinct 'DROP PUBLIC SYNONYM '||synonym_name||';'
from dba_synonyms where table_owner = upper ('&1')
/
 
spool off;

set echo on
set feedback on
@syn_syn_script
quit
