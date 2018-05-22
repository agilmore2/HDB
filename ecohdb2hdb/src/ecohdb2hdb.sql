set feedback off;
set role app_role;
set verify off;

spool ecohdb2hdb.out

select ecohdb_data_load(trunc(sysdate -&&1,'dd'),trunc(sysdate,'dd'))
  as "Values Loaded"
  from dual; 

spool off

exit;
