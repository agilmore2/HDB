
undefine olduser
define  olduser = &1
@do_tabletriggers disable r_hour


--  Hourly for lc and yakima  stored their end of hour period as the date hour 
--  UC store the begin of the hour in date hour

insert into r_hour 
(site_datatype_id,start_date_time,end_date_time,date_time_loaded,value,source_id,validation)
select site_datatype_id,
       date_hour - decode(instr(user,'UCHDB'),1,0,1/24),
       date_hour + decode(instr(user,'UCHDB'),1,1/24,0),
       trunc(sysdate,'YEAR'),
       value,
       source_id,
       validation
from &&olduser..r_hour;

@do_tabletriggers enable r_hour

