set echo off
set heading off
set feedback off
set pagesize 0
set linesize 100
set verify off

spool drop_wa_objects_script.sql
select distinct 'DROP PUBLIC SYNONYM '||synonym_name||';'
from dba_synonyms where  synonym_name like '%\_WA\_%' ESCAPE '\' and table_owner = upper('&1');

select distinct 'DROP TRIGGER '||trigger_name||';'
from all_triggers where  table_name like '%\_WA\_%' ESCAPE '\' and owner = upper('&1');

select distinct 'DROP TABLE '||table_name||';'
from all_tables where  table_name like '%\_WA\_%' ESCAPE '\' and owner = upper('&1');

select distinct 'DROP VIEW '||VIEW_NAME||';'
from all_views where  view_name like '%\_WA\_%' ESCAPE '\' and owner = upper('&1');

select distinct 'DROP SEQUENCE '||sequence_name||';'
from all_sequences where  sequence_name like '%\_WA\_%' ESCAPE '\' and sequence_owner = upper('&1');

 
spool off;

set echo on
set feedback on
@drop_wa_objects_script
