
undefine olduser
define  olduser = &1
@do_tabletriggers disable r_wy


insert into r_wy 
(site_datatype_id,start_date_time,end_date_time,date_time_loaded,value,source_id,validation)
select site_datatype_id,
       date_wy,
       ADD_MONTHS(date_wy,12),
       trunc(sysdate,'YEAR'),
       value,
       source_id,
       validation
from &&olduser..r_wy;


@do_tabletriggers enable r_wy
