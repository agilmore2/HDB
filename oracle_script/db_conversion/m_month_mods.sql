

insert into m_month 
(model_run_id,site_datatype_id,start_date_time,end_date_time,value)
select model_run_id,site_datatype_id,
       date_month,
       add_months(date_month,1),
       value
from &1.m_month;



