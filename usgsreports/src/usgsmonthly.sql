/* command line arguments */
define site_code = &1;
define pcode = &2;

set colsep '';
set linesize 132;
set verify off;
set echo off;
set feedback off;
set termout off;

set numwidth 8;
set null '       ---';

column sdi new_value sdi;
select hm_site_code, hm_pcode, to_char(site_datatype_id) sdi from
ref_hm_site_pcode
where
hm_site_code = '&&site_code' and
hm_pcode = '&&pcode' and
site_datatype_id IS NOT NULL
;

column site_name new_value name;
column datatype_name new_value datatype;
select a.site_name site_name, b.datatype_name datatype_name
from hdb_site a, hdb_datatype b, hdb_site_datatype c
where
a.site_id = c.site_id and
b.datatype_id = c.datatype_id and
c.site_datatype_id = &&sdi;


column beginyear new_value beginyear;
column year new_value year;
column size new_value size;
select to_number(to_char(min(a.date_month+92),'YYYY')) beginyear, 
       to_number(to_char(max(a.date_month+92),'YYYY')) year,
       to_number(to_char(max(a.date_month+92),'YYYY'))-
       to_number(to_char(min(a.date_month+92),'YYYY'))+ 12 as "size"
from r_month a
where a.site_datatype_id = &&sdi
;

select case when to_char(min(a.date_month),'MM') 

set pagesize &size;

column min new_value min;
column max new_value max;
column ave new_value ave;
column sum new_value sum;

column janave new_value janave;
column jansum new_value jansum;
column febave new_value febave;
column febsum new_value febsum;
column marave new_value marave;
column marsum new_value marsum;
column aprave new_value aprave;
column aprsum new_value aprsum;
column mayave new_value mayave;
column maysum new_value maysum;
column junave new_value junave;
column junsum new_value junsum;
column julave new_value julave;
column julsum new_value julsum;
column augave new_value augave;
column augsum new_value augsum;
column sepave new_value sepave;
column sepsum new_value sepsum;
column octave new_value octave;
column octsum new_value octsum;
column novave new_value novave;
column novsum new_value novsum;
column decave new_value decave;
column decsum new_value decsum;

select nvl(max(a.value),0) max, nvl(round(sum(a.value),2),0) sum
from r_month a
where a.site_datatype_id = &&sdi
and a.date_month between '01-jan-&beginyear' and '01-dec-&year';

column numformat new_value numformat;
select case 
when &max > 999999999 then '99.99EEEE'
when &max > 999999    then '999999999'
else                       '999990.00'
end numformat
from dual;


column sumformat new_value sumformat;
select case 
when &sum > 99999999 then '99.99EEEE'
when &sum > 999999 then   '999999999'
else                      '999990.00'
end sumformat
from dual;

select to_char(min(a.value),'&numformat') min, to_char(max(a.value),'&numformat') max,
       round(avg(a.value),2) ave, round(sum(a.value),2) sum
from r_month a
where a.site_datatype_id = &sdi
;

select to_char(round(avg(a.value),2),'&numformat') janave, to_char(round(sum(a.value),2),'&sumformat') jansum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 1;

select to_char(round(avg(a.value),2),'&numformat') febave, to_char(round(sum(a.value),2),'&sumformat') febsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 2;

select to_char(round(avg(a.value),2),'&numformat') marave, to_char(round(sum(a.value),2),'&sumformat') marsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 3;

select to_char(round(avg(a.value),2),'&numformat') aprave, to_char(round(sum(a.value),2),'&sumformat') aprsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 4;

select to_char(round(avg(a.value),2),'&numformat') mayave, to_char(round(sum(a.value),2),'&sumformat') maysum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 5;

select to_char(round(avg(a.value),2),'&numformat') junave, to_char(round(sum(a.value),2),'&sumformat') junsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 6;

select to_char(round(avg(a.value),2),'&numformat') julave, to_char(round(sum(a.value),2),'&sumformat') julsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 7;

select to_char(round(avg(a.value),2),'&numformat') augave, to_char(round(sum(a.value),2),'&sumformat') augsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 8;

select to_char(round(avg(a.value),2),'&numformat') sepave, to_char(round(sum(a.value),2),'&sumformat') sepsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 9;

select to_char(round(avg(a.value),2),'&numformat') octave, to_char(round(sum(a.value),2),'&sumformat') octsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 10;

select to_char(round(avg(a.value),2),'&numformat') novave, to_char(round(sum(a.value),2),'&sumformat') novsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 11;

select to_char(round(avg(a.value),2),'&numformat') decave, to_char(round(sum(a.value),2),'&sumformat') decsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.date_month,'MM') = 12;

column unit_name new_value unit;
select c.unit_name from 
hdb_site_datatype a, hdb_datatype b, hdb_unit c
where 
a.site_datatype_id = &sdi and
a.datatype_id = b.datatype_id and
b.unit_id = c.unit_id;

set termout on;

ttitle center "site: &&name(&&site_code) datatype: &&datatype(&&pcode) in &&unit" skip -
       center "Historical Record &beginyear to &year";

btitle left 'Ave  &octave&novave&decave&janave&febave&marave&aprave&mayave&junave&julave&augave&sepave' skip -
btitle left 'Sum  &octsum&novsum&decsum&jansum&febsum&marsum&aprsum&maysum&junsum&julsum&augsum&sepsum' skip -
btitle left 'Historical Record     Sum  &sum   Average  &ave       Max&max       Min&min';

column year heading "Year" format 9999;
column jan heading "Jan" jus right;
column feb heading "Feb" jus right;
column mar heading "Mar" jus right;
column apr heading "Apr" jus right;
column may heading "May" jus right;
column jun heading "Jun" jus right;
column jul heading "Jul" jus right;
column aug heading "Aug" jus right;
column sep heading "Sep" jus right;
column oct heading "Oct" jus right;
column nov heading "Nov" jus right;
column dec heading "Dec" jus right;

comp max label "Max" min label "Min" of jan on report;
comp max label "Max" min label "Min" of feb on report;
comp max label "Max" min label "Min" of mar on report;
comp max label "Max" min label "Min" of apr on report;
comp max label "Max" min label "Min" of may on report;
comp max label "Max" min label "Min" of jun on report;
comp max label "Max" min label "Min" of jul on report;
comp max label "Max" min label "Min" of aug on report;
comp max label "Max" min label "Min" of sep on report;
comp max label "Max" min label "Min" of oct on report;
comp max label "Max" min label "Min" of nov on report;
comp max label "Max" min label "Min" of dec on report;

break on report;

select years.year year,
to_char(round(j.value,2),'&numformat') oct, to_char(round(k.value,2),'&numformat') nov,
to_char(round(l.value,2),'&numformat') dec, to_char(round(a.value,2),'&numformat') jan,
to_char(round(b.value,2),'&numformat') feb, to_char(round(c.value,2),'&numformat') mar,
to_char(round(d.value,2),'&numformat') apr, to_char(round(e.value,2),'&numformat') may,
to_char(round(f.value,2),'&numformat') jun, to_char(round(g.value,2),'&numformat') jul,
to_char(round(h.value,2),'&numformat') aug, to_char(round(i.value,2),'&numformat') sep
from 
r_month a, r_month b, r_month c, r_month d,
r_month e, r_month f, r_month g, r_month h,
r_month i, r_month j, r_month k, r_month l,
(select &beginyear-1+rownum year from r_month 
where rownum <= (&year-&beginyear+1)) years
where
a.site_datatype_id(+) = &sdi and b.site_datatype_id(+) = &sdi and
c.site_datatype_id(+) = &sdi and d.site_datatype_id(+) = &sdi and
e.site_datatype_id(+) = &sdi and f.site_datatype_id(+) = &sdi and
g.site_datatype_id(+) = &sdi and h.site_datatype_id(+) = &sdi and
i.site_datatype_id(+) = &sdi and j.site_datatype_id(+) = &sdi and
k.site_datatype_id(+) = &sdi and l.site_datatype_id(+) = &sdi and
a.date_month(+) = '01-jan-'||years.year and
b.date_month(+) = '01-feb-'||years.year and
c.date_month(+) = '01-mar-'||years.year and
d.date_month(+) = '01-apr-'||years.year and
e.date_month(+) = '01-may-'||years.year and
f.date_month(+) = '01-jun-'||years.year and
g.date_month(+) = '01-jul-'||years.year and
h.date_month(+) = '01-aug-'||years.year and
i.date_month(+) = '01-sep-'||years.year and
j.date_month(+) = '01-oct-'||to_char(years.year-1) and
k.date_month(+) = '01-nov-'||to_char(years.year-1) and
l.date_month(+) = '01-dec-'||to_char(years.year-1)
order by year
;
quit;
