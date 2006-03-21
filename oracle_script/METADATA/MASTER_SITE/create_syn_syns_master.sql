set echo off
set heading off
set feedback off
set pagesize 0
set linesize 200
set verify off

spool create_syn_syns_master_script.sql

--  Create synonyms for use by meta-data app; these refer to master tables
select 'CREATE PUBLIC SYNONYM ' || substr(table_name,1,26) || '_SYN FOR &2..' || table_name ||
       '@&1;'
from dba_tables
where table_name like 'HDB_%'
order by table_name;

spool off;
set feedback on
@create_syn_syns_master_script
quit
