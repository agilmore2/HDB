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

COLUMN DAY NEW_VALUE DAY
select to_char(sysdate,'yyyymmdd') DAY from dual;

-- this names the data file
spool USBR.&DAY

/* Daily Query. Format is quote-delimited text, and pipe ("|") separated values in the form:
 * "SITE NAME"|"datatype name"|"YYYY-MM-DD"|round(value,2), ie
 * "FLAMING GORGE"|"release"|"2004-08-30"|999.67
 */

-- this just puts this text in the file
prompt : First try at COE data in SHEF format
prompt : mailto:hpatno@uc.usbr.gov,agilmore@uc.usbr.gov with questions

select '.A ' || b.PRIMARY_SITE_CODE || ' ' ||
to_char (c.START_DATE_TIME,'yyyymmdd')||' DH24 /'|| b.PRIMARY_DATA_CODE ||
' ' || round (c.VALUE,2) from
hdb_ext_data_source a, ref_ext_site_data_map b, r_day c where
a.ext_data_source_name = 'CORPS OF ENGINEERS DATA REPORT' and 
a.EXT_DATA_SOURCE_ID=b.ext_data_source_id and 
b.HDB_SITE_DATATYPE_ID=c.site_datatype_id and
c.START_DATE_TIME>sysdate-25
order by b.PRIMARY_SITE_CODE, b.PRIMARY_DATA_CODE, c.START_DATE_TIME;
 
 spool off;
 
 quit;
 
