
define  olduser = &1
@do_tabletriggers disable r_month


insert into r_month 
(site_datatype_id,start_date_time,end_date_time,date_time_loaded,value,source_id,validation)
select site_datatype_id,
       date_month,
       ADD_MONTHS(date_month,1),
       trunc(sysdate,'YEAR'),
       value,
       source_id,
       validation
from &&olduser..r_month;

@do_tabletriggers enable r_month


