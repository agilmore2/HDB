undefine olduser
define  olduser = &1
@do_tabletriggers disable r_day

insert into r_day 
(site_datatype_id,start_date_time,end_date_time,date_time_loaded,value,source_id,validation)
select site_datatype_id,
       date_day,
       date_day + 1,
       trunc(sysdate,'YEAR'),
       value,
       source_id,
       validation
from &&olduser..r_day;

@do_tabletriggers enable r_day

