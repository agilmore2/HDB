/* set the sql*plus environment up so that
   a good data file is created
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

select  '"'||b.site_common_name||'"'||'|'||'"'||c.datatype_common_name||'"'||'|'
||'"'||to_char(d.start_date_time,'YYYY-MM-DD')||'"'||'|'||round(d.value,2) data
from hdb_site_datatype a, hdb_site b, hdb_datatype c, r_day d
where
-- Join section
a.datatype_id = c.datatype_id and 
a.site_datatype_id = d.site_datatype_id and
a.site_id = b.site_id and
-- Date selection section
 (d.start_date_time >= sysdate - 30 or -- last x days of data
  (d.start_date_time >= sysdate - 120 and -- plus the last y days
  d.date_time_loaded >= sysdate - 20)) and -- that have changed in the last z days
-- site and datatype selection section
((b.site_id in (451,716,719,720,721,722,723,1065,1301,1302,1303,1304,1482,1483) and c.datatype_id = 19) or
 (b.site_id in (924,951,955) and c.datatype_id in (17,49)) or
 (b.site_id = 959 and c.datatype_id in (17,29)) or
 (b.site_id in (912,925,927,928,930,931,932,936,938,939,944,945,948,949,952,956,957,958,960,961,963) and c.datatype_id in (17,29,42,49)) or
 (b.site_id in (933,934,935,940,941,942,946,947,953,958,962,964) and  c.datatype_id in (17,25,29,42,49)) or
 (b.site_id = 913 and c.datatype_id in (17,25,29,33,39,40,42,43,46,49,1197)) or
 (b.site_id = 914 and c.datatype_id in (17,25,29,31,33,39,40,42,46,49,1197)) or
 (b.site_id = 915 and c.datatype_id in (17,29,31,33,39,40,42,43,46,49,1197)) or
 (b.site_id = 916 and c.datatype_id in (17,25,29,39,40,42,43,46,49,1197)) OR
 (b.site_id in (917,919) and c.datatype_id in (15,17,25,29,33,39,40,42,43,46,49,1197)) or
 (b.site_id = 920 and c.datatype_id in (17,25,29,33,42,49,123)) )
-- sort section, get all sites and their datatypes together, then 
-- order by date ascending
order by b.site_id, c.datatype_id, d.start_date_time
;

/* Monthly Query. Format is same as above
 */
 
prompt # Monthly end of period storage data

select  '"'||b.site_common_name||'"'||'|'||'"'||c.datatype_common_name||'"'||'|'
||'"'||to_char(d.end_date_time-1,'YYYY-MM-DD')||'"'||'|'||round(d.value,2) data
from hdb_site_datatype a, hdb_site b, hdb_datatype c, r_month d
where 
-- join section
a.datatype_id = c.datatype_id and 
a.site_datatype_id = d.site_datatype_id and
a.site_id = b.site_id and
-- Date selection section
 (d.start_date_time >= add_months(sysdate,-4)  or -- last x months of data ( will only return x-1 months except at end of month)
  (d.start_date_time >= add_months(sysdate,-13) and -- plus the last y months of data
  d.date_time_loaded >= sysdate - 20)) and -- that have changed in the last z days
-- site and datatype selection section
(b.site_id in (924,951,955,959,912,925,927,928,930,931,932,936,938,939,944,
               945,948,949,952,956,957,958,960,961,963,933,934,935,940,941,
               942,946,947,953,958,962,964,913,914,915,916,917,919,920) and
 c.datatype_id = 17)
-- sort section, get all sites and their datatypes together, then 
-- order by date ascending
 order by b.site_id, c.datatype_id, d.start_date_time
 ;
 
 spool off;
 
 quit;
 