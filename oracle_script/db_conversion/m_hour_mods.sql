

insert into m_hour 
(model_run_id,site_datatype_id,start_date_time,end_date_time,value)
select model_run_id,site_datatype_id,
       date_hour,
       date_hour + 1/24,
       value
from &1.m_hour;



