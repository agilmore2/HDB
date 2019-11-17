set heading off
set linesize 175
set feedback off
set pagesize 0
set verify off

spool &1..csv

select UPPER('USBR UPPER COLORADO RESERVOIR DATA,'|| to_char(rd.start_date_time,'yyyy-mm-dd HH24:mi:')||
'MST,'||sd.site_common_name||','||hd.datatype_common_name||',day,'||
ltrim(replace(to_char(round(rd.value,5),'9999999999.09999'),'.0000000','.0'))||','||
hu.unit_common_name)
from r_day rd, hdb_site sd, hdb_site_datatype hsd, hdb_datatype hd, hdb_unit hu
where rd.site_datatype_id = hsd.site_datatype_id
and hsd.site_id = sd.site_id
and hsd.datatype_id = hd.datatype_id
and hd.unit_id = hu.unit_id
and hsd.site_id = &2
and hsd.datatype_id in (17,29,39,42,49)
and rd.start_date_time >= trunc(sysdate) - 40
order by rd.start_date_time;

spool off
