
undefine olduser
define  olduser = &1
@do_tabletriggers disable r_year 


insert into r_year 
(site_datatype_id,start_date_time,end_date_time,date_time_loaded,value,source_id,validation)
select site_datatype_id,
       date_year,
       ADD_MONTHS(date_year,12),
       trunc(sysdate,'YEAR'),
       value,
       source_id,
       validation
from &&olduser..r_year;


@do_tabletriggers enable  r_year

