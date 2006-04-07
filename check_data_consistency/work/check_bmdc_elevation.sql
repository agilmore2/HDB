-- Query to check on consistency between Hydromet Dayfile (GOES) data 
-- and SCADA data for Blue Mesa Elevation

-- command line argument for number of days to search back
define days_back = &1;

-- setup output
set feedback off
set pagesize 9999
set linesize 100
set verify off

-- get the number of rows that had errors.
--   We exit with this as a signal so the email routine knows we had errors
set termout off;

column num_rows new_value num_rows
select count(*) num_rows from 
r_base a, r_base b, hdb_site_datatype c, hdb_site_datatype d where 
a.interval = 'instant' and 
b.interval = 'hour' and 
a.collection_system_id in (2,3) and -- Dayfile
b.collection_system_id = 7 and -- SCADA
((a.start_date_time = (b.start_date_time + 1/24) and a.start_date_time between '30-OCT-2005' and '01-APR-2006') or
(a.start_date_time = (b.start_date_time + 2/24) and (a.start_date_time < '30-OCT-2005' or a.start_date_time >= '02-apr-2006'))) and 
/* SCADA end date time is one hour off of instantaneous data
   DST issue, so when DST changes, this needs to get altered */
abs(a.value - b.value) > .031 and -- difference of 0.031 feet is alarm
a.start_date_time > sysdate - &&days_back and 
c.site_datatype_id = a.site_datatype_id and 
c.datatype_id in (24) and -- instant elevation
c.site_id in (913) and 
d.site_datatype_id = b.site_datatype_id and 
d.datatype_id in (49) and -- EOP elevation
d.site_id in (913) and 
b.start_date_time > '13-AUG-2005';
-- Data before this is bad

set termout on;
 
/* Now generate the actual report */
column site format A25
column datatype format A15
column sdi format 99999
select to_char(b.end_date_time,'DD-MON-YYYY HH24:MI') "SCADA Hour Ending", --print end date time here since that is what scada reports
round(a.value,3) "GOES", round(b.value,3) "SCADA",
round(abs(a.value - b.value),3) "Diff",
f.site_common_name site, e.datatype_common_name datatype, 
a.site_datatype_id sdi from
r_base a, r_base b, hdb_site_datatype c,
hdb_site_datatype d, hdb_datatype e, hdb_site f where
a.interval = 'instant' and
b.interval = 'hour' and
a.collection_system_id in (2,3) and
b.collection_system_id = 7 and
((a.start_date_time = (b.start_date_time + 1/24) and a.start_date_time between '30-OCT-2005' and '01-APR-2006') or
(a.start_date_time = (b.start_date_time + 2/24) and (a.start_date_time < '30-OCT-2005' or a.start_date_time >= '02-apr-2006'))) and 
abs(a.value - b.value) > .031 and
a.start_date_time > sysdate - &&days_back and
c.site_datatype_id = a.site_datatype_id and
c.datatype_id in (24) and
c.site_id in (913) and
d.site_datatype_id = b.site_datatype_id and
d.datatype_id in (49) and
d.site_id in (913) and
b.start_date_time > '13-AUG-2005' and
e.datatype_id = d.datatype_id and
f.site_id = d.site_id
order by a.start_date_time;

exit num_rows;
