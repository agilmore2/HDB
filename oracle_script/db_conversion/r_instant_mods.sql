
undefine olduser
define  olduser = &1
@do_tabletriggers disable r_instant

insert into r_instant 
(site_datatype_id,start_date_time,end_date_time,date_time_loaded,value,source_id,validation)
select site_datatype_id,
       date_time,
       date_time,
       trunc(sysdate,'YEAR'),
       value,
       source_id,
       validation
from &&olduser..r_instant;

@do_tabletriggers enable r_instant


