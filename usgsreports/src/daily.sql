define site = '&&1';

set pagesize 9999;
set linesize 255;
set numwidth 8;
set termout off;
set verify off;
set feedback off;

column 1day new_value 1day;
column 2day new_value 2day;
column 3day new_value 3day;
column 4day new_value 4day;
column 5day new_value 5day;
column "site_code" format a9;
select to_char(sysdate-1,'Dy MONDD') "1day",
to_char(sysdate-2,'Dy MONDD') "2day",
to_char(sysdate-3,'Dy MONDD') "3day",
to_char(sysdate-4,'Dy MONDD') "4day",
to_char(sysdate-5,'Dy MONDD') "5day" from dual;

set termout on;

/* note that the selection order of the r_day tables is reversed*/
select lpad(a.hm_site_code,9) "site_code", a.hm_pcode "pcode",
round(f.value,2) "&5day", round(e.value,2) "&4day",
round(d.value,2) "&3day", round(c.value,2) "&2day",
round(b.value,2) "&1day"
from 
ref_hm_site_pcode a, r_day b, r_day c,
r_day d, r_day e, r_day f
where
a.hm_site_code = UPPER('&&site') and a.hm_filetype = 'A' and
a.site_datatype_id is not null and
b.site_datatype_id(+) = a.site_datatype_id and
c.site_datatype_id(+) = a.site_datatype_id and
d.site_datatype_id(+) = a.site_datatype_id and
e.site_datatype_id(+) = a.site_datatype_id and
f.site_datatype_id(+) = a.site_datatype_id and
b.date_day(+) = to_char(sysdate-1,'dd-mon-yyyy') and
c.date_day(+) = to_char(sysdate-2,'dd-mon-yyyy') and
d.date_day(+) = to_char(sysdate-3,'dd-mon-yyyy') and
e.date_day(+) = to_char(sysdate-4,'dd-mon-yyyy') and
f.date_day(+) = to_char(sysdate-5,'dd-mon-yyyy')
;
quit;
