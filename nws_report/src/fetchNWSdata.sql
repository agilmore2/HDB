/* set the sql*plus environment up so that
   a good data file is created
set pagesize 50000  -- no page breaks
set newpage none -- no page header
set linesize 255 -- no line breaks
set verify off -- don't repeat back variable subsitutions
set feedback off -- don't report rows selected
set trimspool on -- don't fill in to linesize with blanks (!!)
set echo off -- don't echo commands
set heading off -- no line headers
set termout off -- don't write data to the screen

 */
set pagesize 50000
set newpage none
set linesize 255
set verify off
set feedback off
set trimspool on
set echo off
set heading off
set termout off

-- this names the data file
spool nwsdata.dat

/* Daily Query. Format is quote-delimited text, and pipe ("|") separated values in the form:
 * "SITE NAME"|"datatype name"|"YYYY-MM-DD"|round(value,2), ie
 * "FLAMING GORGE"|"release"|"2004-08-30"|999.67
 */

-- this just puts this text in the file
prompt # Daily data

select  '"'||map.primary_site_code||'"|"'||map.primary_data_code||'"|"'||
to_char(day.start_date_time,'YYYY-MM-DD')||'"|'||round(day.value,2) data
from hdb_site_datatype sd, ref_ext_site_data_map map,
     hdb_ext_data_source eds, r_day day
where
-- Join section
day.site_datatype_id = map.hdb_site_datatype_id and
sd.site_datatype_id = map.hdb_site_datatype_id and
map.ext_data_source_id = eds.ext_data_source_id and
eds.ext_data_source_name = 'HDB NWS Daily Update' and
-- Date selection section
 (day.start_date_time >= sysdate - 30 or -- last x days of data
  (day.start_date_time >= sysdate - 120 and -- plus the last y days
  day.date_time_loaded >= sysdate - 20)) -- that have changed in the last z days
-- sort section, get all sites and their datatypes together, then 
-- order by date ascending
order by sd.site_id, sd.datatype_id, day.start_date_time
;

/* Monthly Query. Format is same as above
 */
 
prompt # Monthly end of period storage data

select  '"'||map.primary_site_code||'"|"'||map.primary_data_code||'"|"'||
to_char(add_months(month.start_date_time,1)-1,'YYYY-MM-DD')||'"|'||round(month.value,2) data
from hdb_site_datatype sd, ref_ext_site_data_map map,
     hdb_ext_data_source eds, r_month month
where
-- Join section
month.site_datatype_id = map.hdb_site_datatype_id and
sd.site_datatype_id = map.hdb_site_datatype_id and
map.ext_data_source_id = eds.ext_data_source_id and
eds.ext_data_source_name = 'HDB NWS Monthly Update' and
-- Date selection section
 (month.start_date_time >= add_months(sysdate,-4)  or -- last x months of data ( will only return x-1 months except at end of month)
  (month.start_date_time >= add_months(sysdate,-13) and -- plus the last y months of data
   month.date_time_loaded >= sysdate - 20)) -- that have changed in the last z days
-- sort section, get all sites and their datatypes together, then 
-- order by date ascending
 order by sd.site_id, sd.datatype_id, month.start_date_time
 ;


prompt # Hourly Powell Elevations, Al Martinelli 524-5710, ext 263

select  '"'||map.primary_site_code||'"|"'||map.primary_data_code||'"|"'||
to_char(hour.start_date_time+1/24,'YYYY-MM-DD HH24:MI')||'"|'||round(hour.value,2) data
from hdb_site_datatype sd, ref_ext_site_data_map map,
     hdb_ext_data_source eds, r_hour hour
where
-- Join section
hour.site_datatype_id = map.hdb_site_datatype_id and
sd.site_datatype_id = map.hdb_site_datatype_id and
map.ext_data_source_id = eds.ext_data_source_id and
eds.ext_data_source_name = 'HDB NWS Hourly Update' and
-- Date selection section
 (hour.start_date_time >= trunc(sysdate-4,'DD')  or -- last 3 days of data 
  (hour.start_date_time >= sysdate-14 and -- plus the last 2 weeks of data
   hour.date_time_loaded >= trunc(sysdate - 3, 'DD'))) -- that have changed in the last 2 days
-- sort section, get all sites and their datatypes together, then 
-- order by date ascending
 order by sd.site_id, sd.datatype_id, hour.start_date_time
 ;
 
 spool off;
 
 quit;
 
