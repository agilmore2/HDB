/* command line arguments */
define site_code = &1;
define pcode = &2;

/* setup report variables */
set colsep '';
set pagesize 42;
set linesize 132;
set verify off;
set echo off;
set feedback off;
set termout off;

set numwidth 8;
set null '       ---';

column year new_value year;
select nvl('&3',to_char(sysdate,'YYYY')) year from dual;

/* get previous year, for use with water year */
column beginyear new_value beginyear nopri;
select to_char(&&year - 1) beginyear from dual;

/* get site_datatype_id needs work*/
column sdi new_value sdi;
select primary_site_code, primary_data_code, to_char(hdb_site_datatype_id) sdi from
ref_ext_site_data_map
where
UPPER(primary_site_code) = UPPER('&&site_code') and
UPPER(primary_data_code) = UPPER('&&pcode') and
hdb_site_datatype_id IS NOT NULL and
/*-- interval = 'day' and*/
rownum = 1;
/*select hm_site_code, hm_pcode, to_char(site_datatype_id) sdi from
ref_hm_site_pcode
where
hm_site_code = UPPER('&&site_code') and
hm_pcode = UPPER('&&pcode') and
site_datatype_id IS NOT NULL and
hm_filetype = 'A'
;*/

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
c.site_datatype_id = &&sdi
;

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

/* get these for selecting number formats, reselect later for format*/
select nvl(max(a.value),0) max, nvl(sum(a.value),0) sum
from r_day a
where a.site_datatype_id = &&sdi
and a.start_date_time between '01-oct-&beginyear' and last_day('01-sep-&year');

/* 10 character wide formats (remember space for minus sign)*/
column numformat new_value numformat;
select case 
when &max > 999999999 then '99.99EEEE'
when &max > 99999     then '999999999'
else                       '999990.00'
end numformat
from dual;

column sumformat new_value sumformat;
select case 
when &sum > 99999999 then '99.99EEEE'
when &sum > 99999  then   '999999999'
else                      '999990.00'
end sumformat
from dual;

/*get the statistics for the whole year*/
select to_char(min(a.value),'&numformat') min, to_char(max(a.value),'&numformat') max,
       to_char(avg(a.value),'&numformat') ave, to_char(sum(a.value),'&sumformat') sum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-oct-&beginyear' and last_day('01-sep-&year');

/*get statistics for each month.
  Only ave and sum are in here, the min and max are calculated by the compute
  statements lower down */
select to_char(avg(a.value),'&numformat') janave, to_char(sum(a.value),'&sumformat') jansum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-jan-&year' and last_day('01-jan-&year');

select to_char(avg(a.value),'&numformat') febave, to_char(sum(a.value),'&sumformat') febsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-feb-&year' and last_day('01-feb-&year');

select to_char(avg(a.value),'&numformat') marave, to_char(sum(a.value),'&sumformat') marsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-mar-&year' and last_day('01-mar-&year');

select to_char(avg(a.value),'&numformat') aprave, to_char(sum(a.value),'&sumformat') aprsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-apr-&year' and last_day('01-apr-&year');

select to_char(avg(a.value),'&numformat') mayave, to_char(sum(a.value),'&sumformat') maysum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-may-&year' and last_day('01-may-&year');

select to_char(avg(a.value),'&numformat') junave, to_char(sum(a.value),'&sumformat') junsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-jun-&year' and last_day('01-jun-&year');

select to_char(avg(a.value),'&numformat') julave, to_char(sum(a.value),'&sumformat') julsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-jul-&year' and last_day('01-jul-&year');

select to_char(avg(a.value),'&numformat') augave, to_char(sum(a.value),'&sumformat') augsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-aug-&year' and last_day('01-aug-&year');

select to_char(avg(a.value),'&numformat') sepave, to_char(sum(a.value),'&sumformat') sepsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-sep-&year' and last_day('01-sep-&year');

select to_char(avg(a.value),'&numformat') octave, to_char(sum(a.value),'&sumformat') octsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-oct-&beginyear' and last_day('01-oct-&beginyear');

select to_char(avg(a.value),'&numformat') novave, to_char(sum(a.value),'&sumformat') novsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-nov-&beginyear' and last_day('01-nov-&beginyear');

select to_char(avg(a.value),'&numformat') decave, to_char(sum(a.value),'&sumformat') decsum
from r_day a
where a.site_datatype_id = &sdi
and a.start_date_time between '01-dec-&beginyear' and last_day('01-dec-&beginyear');

/* now start the report*/
/* top title, datatype names can be really long!*/
ttitle center "site: &&name(&&site_code) datatype: &&datatype(&&pcode) sdi: &&sdi in &&unit" skip -
       center "Water Year October &beginyear to September &year";

/*bottom title, this is where the monthly average and sum statistics and the
  statistics for the whole yearare reported*/

btitle left 'Ave&octave&novave&decave&janave&febave&marave&aprave&mayave&junave&julave&augave&sepave' skip -
btitle left 'Sum&octsum&novsum&decsum&jansum&febsum&marsum&aprsum&maysum&junsum&julsum&augsum&sepsum' skip -
btitle left 'Water Year &year   Sum  &sum Average  &ave       Max&max       Min&min';

/*setup columns for report justify needed because columns are not numbers any more*/
column day heading "Day" format 99;
column jan heading "Jan" justify right;
column feb heading "Feb" justify right;
column mar heading "Mar" justify right;
column apr heading "Apr" justify right;
column may heading "May" justify right;
column jun heading "Jun" justify right;
column jul heading "Jul" justify right;
column aug heading "Aug" justify right;
column sep heading "Sep" justify right;
column oct heading "Oct" justify right;
column nov heading "Nov" justify right;
column dec heading "Dec" justify right;

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
select days.day day,
/* note that j, k, l are first, to get oct, nov, dec values in front
   everywhere else, these tables are done in order */
to_char(j.value,'&numformat') oct, to_char(k.value,'&numformat') nov,
to_char(l.value,'&numformat') dec, to_char(a.value,'&numformat') jan,
to_char(b.value,'&numformat') feb, to_char(c.value,'&numformat') mar,
to_char(d.value,'&numformat') apr, to_char(e.value,'&numformat') may,
to_char(f.value,'&numformat') jun, to_char(g.value,'&numformat') jul,
to_char(h.value,'&numformat') aug, to_char(i.value,'&numformat') sep
from 
r_day a, r_day b, r_day c, r_day d,
r_day e, r_day f, r_day g, r_day h,
r_day i, r_day j, r_day k, r_day l,
/* this COOL subquery just returns numbers 1 through 31
   there may be a better way to do this. */
(select 0+rownum day from r_day 
where rownum <= 31) days
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
a.start_date_time (+) between '01-jan-&year' and last_day('01-jan-&year') and
b.start_date_time (+) between '01-feb-&year' and last_day('01-feb-&year') and
c.start_date_time (+) between '01-mar-&year' and last_day('01-mar-&year') and
d.start_date_time (+) between '01-apr-&year' and last_day('01-apr-&year') and
e.start_date_time (+) between '01-may-&year' and last_day('01-may-&year') and
f.start_date_time (+) between '01-jun-&year' and last_day('01-jun-&year') and
g.start_date_time (+) between '01-jul-&year' and last_day('01-jul-&year') and
h.start_date_time (+) between '01-aug-&year' and last_day('01-aug-&year') and
i.start_date_time (+) between '01-sep-&year' and last_day('01-sep-&year') and
j.start_date_time (+) between '01-oct-&beginyear' and last_day('01-oct-&beginyear') and
k.start_date_time (+) between '01-nov-&beginyear' and last_day('01-nov-&beginyear') and
l.start_date_time (+) between '01-dec-&beginyear' and last_day('01-dec-&beginyear') and
/*finally, get the days in order*/
to_char(a.start_date_time(+),'DD') = days.day and
to_char(b.start_date_time(+),'DD') = days.day and
to_char(c.start_date_time(+),'DD') = days.day and
to_char(d.start_date_time(+),'DD') = days.day and
to_char(e.start_date_time(+),'DD') = days.day and
to_char(f.start_date_time(+),'DD') = days.day and
to_char(g.start_date_time(+),'DD') = days.day and
to_char(h.start_date_time(+),'DD') = days.day and
to_char(i.start_date_time(+),'DD') = days.day and
to_char(j.start_date_time(+),'DD') = days.day and
to_char(k.start_date_time(+),'DD') = days.day and
to_char(l.start_date_time(+),'DD') = days.day 
order by day;

quit;
