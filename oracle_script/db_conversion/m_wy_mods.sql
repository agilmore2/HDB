

insert into m_wy 
(model_run_id,site_datatype_id,start_date_time,end_date_time,value)
select model_run_id,site_datatype_id,
       date_wy,
       add_months(date_wy,12),
       value
from &1.m_wy;



