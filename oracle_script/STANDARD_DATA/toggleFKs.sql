set echo off
set heading off
set feedback off
set pagesize 0
set linesize 100
set verify off

spool ./STANDARD_DATA/fk_script.sql
select distinct 'ALTER TABLE '||table_name||upper(' &1')|| ' CONSTRAINT '||constraint_name||';'
from user_constraints where constraint_name like 'HDB_%_FK%' or 
constraint_name like 'REF_%_FK%'
/
 
spool off;

set echo on
set feedback on
@./STANDARD_DATA/fk_script
quit
