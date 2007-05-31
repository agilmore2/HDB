/* command line arguments */
define site_code = &1;
define pcode = &2;

/* setup report variables */
set colsep '';
set linesize 132;
set verify off;
set echo off;
set feedback off;
set termout off;

set numwidth 8;
set null '       ---';

/* get site_datatype_id needs work*/
column sdi new_value sdi;
select primary_site_code, primary_data_code, to_char(hdb_site_datatype_id) sdi from
ref_ext_site_data_map
where
UPPER(primary_site_code) = UPPER('&&site_code') and
UPPER(primary_data_code) = UPPER('&&pcode') and
hdb_site_datatype_id IS NOT NULL and
/*-- interval = 'month' and*/
rownum = 1;

/*select hm_site_code, hm_pcode, to_char(site_datatype_id) sdi from
ref_hm_site_pcode
where
hm_site_code = UPPER('&&site_code') and
hm_pcode = UPPER('&&pcode') and
hm_filetype = 'A' and
site_datatype_id IS NOT NULL
;
*/
/* find HDB names for site and datatype for sdi */
column site_common_name new_value name;
column datatype_common_name new_value datatype;
column unit_common_name new_value unit;
select a.site_common_name, b.datatype_common_name, d.unit_common_name
from hdb_site a, hdb_datatype b, hdb_site_datatype c, hdb_unit d
where
a.site_id = c.site_id and
b.datatype_id = c.datatype_id and
b.unit_id = d.unit_id and
c.site_datatype_id = &&sdi;

/* get first and last water years
   note the +92 on the dates, this shifts everything forward
   to make 01-Oct between 01-Jan, which is what is needed to get
   the start and end years right*/
column beginyear new_value beginyear;
column year new_value year;
column size new_value size;
select to_number(nvl(to_char(min(a.start_date_time+92),'YYYY'),'9999')) beginyear, 
       to_number(nvl(to_char(max(a.start_date_time+92),'YYYY'),'9999')) year,
       to_number(nvl(to_char(max(a.start_date_time+92),'YYYY'),'9999'))-
       to_number(nvl(to_char(min(a.start_date_time+92),'YYYY'),'9999'))+ 12 as "size"
from r_month a
where a.site_datatype_id = &&sdi
;

set pagesize &size;

/* if no data exists for this sdi, change year display. I know this sucks!*/
column checkyear new_value checkyear;
column checkbeginyear new_value checkbeginyear;
select 
case when &year=9999 then 'NODATA' else to_char(&year) end checkyear, 
case when &year=9999 then 'NODATA' else to_char(&beginyear) end checkbeginyear
from dual;

/* setup fake columns for reporting values */
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

/* get these for selecting number formats, reselect later in right format*/
select nvl(max(a.value),0) max, nvl(sum(a.value),0) sum
from r_month a
where a.site_datatype_id = &&sdi
and a.start_date_time between '01-jan-&beginyear' and '01-dec-&year';

/* 10 character wide formats (remember space for minus sign)*/
column numformat new_value numformat;
select case 
when &max > 999999999 then '99.99EEEE'
when &max > 99999    then '999999999'
else                       '999990.00'
end numformat
from dual;

column sumformat new_value sumformat;
select case 
when &sum > 99999999 then '99.99EEEE'
when &sum > 99999 then   '999999999'
else                      '999990.00'
end sumformat
from dual;

/*get the statistics for the record*/
select to_char(min(a.value),'&numformat') min, to_char(max(a.value),'&numformat') max,
       avg(a.value) ave, sum(a.value) sum
from r_month a
where a.site_datatype_id = &sdi
;

/*get statistics for each month.
  Only ave and sum are in here, the min and max are calculated by the compute
  statements lower down */
select to_char(avg(a.value),'&numformat') janave, to_char(sum(a.value),'&sumformat') jansum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 1;

select to_char(avg(a.value),'&numformat') febave, to_char(sum(a.value),'&sumformat') febsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 2;

select to_char(avg(a.value),'&numformat') marave, to_char(sum(a.value),'&sumformat') marsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 3;

select to_char(avg(a.value),'&numformat') aprave, to_char(sum(a.value),'&sumformat') aprsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 4;

select to_char(avg(a.value),'&numformat') mayave, to_char(sum(a.value),'&sumformat') maysum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 5;

select to_char(avg(a.value),'&numformat') junave, to_char(sum(a.value),'&sumformat') junsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 6;

select to_char(avg(a.value),'&numformat') julave, to_char(sum(a.value),'&sumformat') julsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 7;

select to_char(avg(a.value),'&numformat') augave, to_char(sum(a.value),'&sumformat') augsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 8;

select to_char(avg(a.value),'&numformat') sepave, to_char(sum(a.value),'&sumformat') sepsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 9;

select to_char(avg(a.value),'&numformat') octave, to_char(sum(a.value),'&sumformat') octsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 10;

select to_char(avg(a.value),'&numformat') novave, to_char(sum(a.value),'&sumformat') novsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 11;

select to_char(avg(a.value),'&numformat') decave, to_char(sum(a.value),'&sumformat') decsum
from r_month a
where a.site_datatype_id = &sdi
and to_char(a.start_date_time,'MM') = 12;

/* now start the report*/
/* top title, datatype names can be really long!*/
ttitle center "site: &&name(&&site_code) datatype: &&datatype(&&pcode) sdi: &&sdi in &&unit" skip -
       center "Historical Record &checkbeginyear to &checkyear";

/*bottom title, this is where the monthly average and sum statistics and the
  statistics for the record are reported*/
btitle left 'Ave  &octave&novave&decave&janave&febave&marave&aprave&mayave&junave&julave&augave&sepave' skip -
btitle left 'Sum  &octsum&novsum&decsum&jansum&febsum&marsum&aprsum&maysum&junsum&julsum&augsum&sepsum' skip -
btitle left 'Historical Record     Sum  &sum   Average  &ave       Max&max       Min&min';

/*setup columns for report justify needed because columns are not numbers any more*/
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

/*at bottom of report, compute max and mins. These are lexical,
  because the columns are now character strings due to number format.
  Cannot control what order these statements show up!
  have to do sum elsewhere because cannot include number formats in
  these statements*/
compute max label "Max" min label "Min" of jan on report;
compute max label "Max" min label "Min" of feb on report;
compute max label "Max" min label "Min" of mar on report;
compute max label "Max" min label "Min" of apr on report;
compute max label "Max" min label "Min" of may on report;
compute max label "Max" min label "Min" of jun on report;
compute max label "Max" min label "Min" of jul on report;
compute max label "Max" min label "Min" of aug on report;
compute max label "Max" min label "Min" of sep on report;
compute max label "Max" min label "Min" of oct on report;
compute max label "Max" min label "Min" of nov on report;
compute max label "Max" min label "Min" of dec on report;

break on report;

/* this command has a bug in it, careful what is right before it*/
set termout on;

/* at last, the QUERY!*/
select years.year year,
/* note that j, k, l are first, to get oct, nov, dec values in front
   everywhere else, these tables are done in order */
to_char(j.value,'&numformat') oct, to_char(k.value,'&numformat') nov,
to_char(l.value,'&numformat') dec, to_char(a.value,'&numformat') jan,
to_char(b.value,'&numformat') feb, to_char(c.value,'&numformat') mar,
to_char(d.value,'&numformat') apr, to_char(e.value,'&numformat') may,
to_char(f.value,'&numformat') jun, to_char(g.value,'&numformat') jul,
to_char(h.value,'&numformat') aug, to_char(i.value,'&numformat') sep
from 
r_month a, r_month b, r_month c, r_month d,
r_month e, r_month f, r_month g, r_month h,
r_month i, r_month j, r_month k, r_month l,
/* this COOL subquery just returns numbers beginyear through year
   there may be a better way to do this. */
(select &beginyear-1+rownum year from r_month 
where rownum <= (&year-&beginyear+1)) years
where
/* notice the outer joins. These allow any time to not have a value, and
   a null will show up in the right places
   any faster to put all of this in one statement? */
a.site_datatype_id(+) = &sdi and b.site_datatype_id(+) = &sdi and
c.site_datatype_id(+) = &sdi and d.site_datatype_id(+) = &sdi and
e.site_datatype_id(+) = &sdi and f.site_datatype_id(+) = &sdi and
g.site_datatype_id(+) = &sdi and h.site_datatype_id(+) = &sdi and
i.site_datatype_id(+) = &sdi and j.site_datatype_id(+) = &sdi and
k.site_datatype_id(+) = &sdi and l.site_datatype_id(+) = &sdi and
/* if I use to_char(x,'yyyy') instead, this query is way too slow*/
a.start_date_time(+) = '01-jan-'||years.year and
b.start_date_time(+) = '01-feb-'||years.year and
c.start_date_time(+) = '01-mar-'||years.year and
d.start_date_time(+) = '01-apr-'||years.year and
e.start_date_time(+) = '01-may-'||years.year and
f.start_date_time(+) = '01-jun-'||years.year and
g.start_date_time(+) = '01-jul-'||years.year and
h.start_date_time(+) = '01-aug-'||years.year and
i.start_date_time(+) = '01-sep-'||years.year and
j.start_date_time(+) = '01-oct-'||to_char(years.year-1) and
k.start_date_time(+) = '01-nov-'||to_char(years.year-1) and
l.start_date_time(+) = '01-dec-'||to_char(years.year-1)
order by year
;
quit;
