

insert into m_day 
(model_run_id,site_datatype_id,start_date_time,end_date_time,value)
select model_run_id,site_datatype_id,
       date_day,
       date_day + 1,
       value
from &1.m_day;



