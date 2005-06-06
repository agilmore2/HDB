set pagesize 0
set linesize 255
set heading off
set feedback off
set verify off
set echo off
set termout off
set trimspool on
alter session set NLS_DATE_FORMAT='DD-MON-YYYY HH24:MI:SS';

spool r_instant_data.csv
 
select site_datatype_id||','||
       date_time||','||
       date_time||','||
       trunc(sysdate,'YEAR')||','||
       value||','||
       source_id||','||
       validation
from r_instant;
 
spool off

spool r_hour_data.csv

select site_datatype_id||','||
       date_hour||','||
       (date_hour+1/24)||','||
       trunc(sysdate,'YEAR')||','||
       value||','||
       source_id||','||
       validation
from r_hour;

spool off

spool r_day_data.csv

select site_datatype_id||','||
       date_day||','||
       (date_day+1)||','||
       trunc(sysdate,'YEAR')||','||
       value||','||
       source_id||','||
       validation
from r_day;

spool off

spool r_month_data.csv

select site_datatype_id||','||
       date_month||','||
       add_months(date_month,1)||','||
       trunc(sysdate,'YEAR')||','||
       value||','||
       source_id||','||
       validation
from r_month;

spool off

spool m_day_data.csv

select model_run_id||','||
       site_datatype_id||','||
       date_day||','||
       (date_day+1)||','||
       trunc(sysdate,'YEAR')||','||
       value
from m_day;

spool off

spool m_month_data.csv

select model_run_id||','||
       site_datatype_id||','||
       date_month||','||
       add_months(date_month,1)||','||
       trunc(sysdate,'YEAR')||','||
       value
from m_month;

spool off

