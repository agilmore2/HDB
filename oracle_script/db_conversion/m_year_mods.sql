

insert into m_year 
(model_run_id,site_datatype_id,start_date_time,end_date_time,value)
select model_run_id,site_datatype_id,
       date_year,
       add_months(date_year,12),
       value
from &1.m_year;



