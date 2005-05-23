/* Query to check on consistency between Hydromet archive data and SCADA data 
*/
-- command line argument for number of days to search back
define days_back = &1;

-- setup output
set feedback off
set markup html on table "border='1' width='90%' align='center'-
summary='SCADA/Hydromet Comparison'"
-- set the pagesize to 1 so that we can use the pno as a row count
set pagesize 9999
set linesize 100
set verify off

set termout off
column num_rows new_value num_rows
select count(*) num_rows
from r_base a, r_day b, hdb_site_datatype c where
a.interval = 'day' and
a.collection_system_id = 3 and
a.site_datatype_id = b.site_datatype_id and
a.start_date_time = b.start_date_time and
abs(a.value - b.value) > .5 and
a.start_date_time > sysdate - &&days_back and
c.site_datatype_id = a.site_datatype_id and
c.datatype_id in (39,49,46,1197) and
c.site_id in (913,914,915,916,917,919,920,965,966,967,968,969,970);

set termout on
select a.start_date_time "Day",
a.value "Hydromet", b.value "r_day", abs(a.value -b.value) "Diff",
d.site_common_name site, e.datatype_common_name datatype, a.site_datatype_id sdi
from r_base a, r_day b, hdb_site_datatype c, hdb_site d, hdb_datatype e where
a.interval = 'day' and
a.collection_system_id = 3 and
a.site_datatype_id = b.site_datatype_id and
a.start_date_time = b.start_date_time and
abs(a.value - b.value) > .5 and
a.start_date_time > sysdate - &&days_back and
c.site_datatype_id = a.site_datatype_id and
c.site_id = d.site_id and
c.datatype_id = e.datatype_id and
c.datatype_id in (39,49,46,1197) and
c.site_id in (913,914,915,916,917,919,920,965,966,967,968,969,970)
order by a.start_date_time, d.site_id, e.datatype_id;

exit num_rows;
