/* command line arguments */
define site = &1;
define datatype = &2;
define year = &3;

set colsep '';
set pagesize 42;
set linesize 132;
set verify off;
set echo off;
set feedback off;
set termout off;

set numwidth 8;
set null '       ---';

column beginyear new_value beginyear nopri;
select to_char(&&year - 1) beginyear from dual;

column hm_site_code new_value name;
column hm_pcode new_value datatype;
column sdi new_value sdi;
select hm_site_code, hm_pcode, to_char(site_datatype_id) sdi from
ref_hm_site_pcode
where
hm_site_code like '%&&site%' and
hm_pcode like '%&&datatype%'
order by site_datatype_id desc;

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
from r_day a
where a.site_datatype_id = &&sdi
and a.date_day between '01-oct-&beginyear' and last_day('01-sep-&year');

column numformat new_value numformat;
select case 
when &max > 999999999 then '99.99EEEE'
when &max > 999999    then '999999999'
else                       '999999.00'
end numformat
from dual;

column sumformat new_value sumformat;
select case 
when &sum/12 > 50000000 then '99.99EEEE'
when &sum/12 > 500000 then   '999999999'
else '999999.00'
end sumformat
from dual;

select to_char(min(a.value),'&numformat') min, to_char(max(a.value),'&numformat') max,
       round(avg(a.value),2) ave, round(sum(a.value),2) sum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-oct-&beginyear' and last_day('01-sep-&year');

select to_char(round(avg(a.value),2),'&numformat') janave, to_char(round(sum(a.value),2),'&sumformat') jansum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-jan-&year' and last_day('01-jan-&year');

select to_char(round(avg(a.value),2),'&numformat') febave, to_char(round(sum(a.value),2),'&sumformat') febsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-feb-&year' and last_day('01-feb-&year');

select to_char(round(avg(a.value),2),'&numformat') marave, to_char(round(sum(a.value),2),'&sumformat') marsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-mar-&year' and last_day('01-mar-&year');

select to_char(round(avg(a.value),2),'&numformat') aprave, to_char(round(sum(a.value),2),'&sumformat') aprsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-apr-&year' and last_day('01-apr-&year');

select to_char(round(avg(a.value),2),'&numformat') mayave, to_char(round(sum(a.value),2),'&sumformat') maysum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-may-&year' and last_day('01-may-&year');

select to_char(round(avg(a.value),2),'&numformat') junave, to_char(round(sum(a.value),2),'&sumformat') junsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-jun-&year' and last_day('01-jun-&year');

select to_char(round(avg(a.value),2),'&numformat') julave, to_char(round(sum(a.value),2),'&sumformat') julsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-jul-&year' and last_day('01-jul-&year');

select to_char(round(avg(a.value),2),'&numformat') augave, to_char(round(sum(a.value),2),'&sumformat') augsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-aug-&year' and last_day('01-aug-&year');

select to_char(round(avg(a.value),2),'&numformat') sepave, to_char(round(sum(a.value),2),'&sumformat') sepsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-sep-&year' and last_day('01-sep-&year');

select to_char(round(avg(a.value),2),'&numformat') octave, to_char(round(sum(a.value),2),'&sumformat') octsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-oct-&beginyear' and last_day('01-oct-&beginyear');

select to_char(round(avg(a.value),2),'&numformat') novave, to_char(round(sum(a.value),2),'&sumformat') novsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-nov-&beginyear' and last_day('01-nov-&beginyear');

select to_char(round(avg(a.value),2),'&numformat') decave, to_char(round(sum(a.value),2),'&sumformat') decsum
from r_day a
where a.site_datatype_id = &sdi
and a.date_day between '01-dec-&beginyear' and last_day('01-dec-&beginyear');

column unit_name new_value unit;
select c.unit_name from 
hdb_site_datatype a, hdb_datatype b, hdb_unit c
where 
a.site_datatype_id = &sdi and
a.datatype_id = b.datatype_id and
b.unit_id = c.unit_id;

set termout on;

ttitle center "&name &datatype &unit" skip -
       center "Water Year October &beginyear to September &year";

btitle left 'Ave&octave&novave&decave&janave&febave&marave&aprave&mayave&junave&julave&augave&sepave' skip -
btitle left 'Sum&octsum&novsum&decsum&jansum&febsum&marsum&aprsum&maysum&junsum&julsum&augsum&sepsum' skip -
btitle left 'Water Year &year Sum &sum Average &ave Max &max Min &min';

column day heading "Day" format 99;
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

select days.day day,
to_char(round(j.value,2),'&numformat') oct, to_char(round(k.value,2),'&numformat') nov,
to_char(round(l.value,2),'&numformat') dec, to_char(round(a.value,2),'&numformat') jan,
to_char(round(b.value,2),'&numformat') feb, to_char(round(c.value,2),'&numformat') mar,
to_char(round(d.value,2),'&numformat') apr, to_char(round(e.value,2),'&numformat') may,
to_char(round(f.value,2),'&numformat') jun, to_char(round(g.value,2),'&numformat') jul,
to_char(round(h.value,2),'&numformat') aug, to_char(round(i.value,2),'&numformat') sep
from 
r_day a, r_day b, r_day c, r_day d,
r_day e, r_day f, r_day g, r_day h,
r_day i, r_day j, r_day k, r_day l,
(select 0+rownum day from r_day 
where rownum <= 31) days
where
a.site_datatype_id(+) = &sdi and b.site_datatype_id(+) = &sdi and
c.site_datatype_id(+) = &sdi and d.site_datatype_id(+) = &sdi and
e.site_datatype_id(+) = &sdi and f.site_datatype_id(+) = &sdi and
g.site_datatype_id(+) = &sdi and h.site_datatype_id(+) = &sdi and
i.site_datatype_id(+) = &sdi and j.site_datatype_id(+) = &sdi and
k.site_datatype_id(+) = &sdi and l.site_datatype_id(+) = &sdi and
a.date_day (+) between '01-jan-&year' and last_day('01-jan-&year') and
b.date_day (+) between '01-feb-&year' and last_day('01-feb-&year') and
c.date_day (+) between '01-mar-&year' and last_day('01-mar-&year') and
d.date_day (+) between '01-apr-&year' and last_day('01-apr-&year') and
e.date_day (+) between '01-may-&year' and last_day('01-may-&year') and
f.date_day (+) between '01-jun-&year' and last_day('01-jun-&year') and
g.date_day (+) between '01-jul-&year' and last_day('01-jul-&year') and
h.date_day (+) between '01-aug-&year' and last_day('01-aug-&year') and
i.date_day (+) between '01-sep-&year' and last_day('01-sep-&year') and
j.date_day (+) between '01-oct-&beginyear' and last_day('01-oct-&beginyear') and
k.date_day (+) between '01-nov-&beginyear' and last_day('01-nov-&beginyear') and
l.date_day (+) between '01-dec-&beginyear' and last_day('01-dec-&beginyear') and
to_char(a.date_day(+),'DD') = days.day and
to_char(b.date_day(+),'DD') = days.day and
to_char(c.date_day(+),'DD') = days.day and
to_char(d.date_day(+),'DD') = days.day and
to_char(e.date_day(+),'DD') = days.day and
to_char(f.date_day(+),'DD') = days.day and
to_char(g.date_day(+),'DD') = days.day and
to_char(h.date_day(+),'DD') = days.day and
to_char(i.date_day(+),'DD') = days.day and
to_char(j.date_day(+),'DD') = days.day and
to_char(k.date_day(+),'DD') = days.day and
to_char(l.date_day(+),'DD') = days.day 
order by day;
quit;
