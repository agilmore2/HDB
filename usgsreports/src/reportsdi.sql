/* command line arguments */
define site = &1;

set verify off;
set feedback off;
set pagesize 9999;

/* Can anyone tell me why this query takes SO long,
but the next query is reasonably fast?
*/

/*
select lpad(a.hm_site_code,9) site_code, a.hm_pcode "pcode",
count(d.site_datatype_id) "r_day", count(m.site_datatype_id) "r_month"  from
ref_hm_site_pcode a, r_day d, r_month m
where
a.hm_site_code = UPPER('&&site') and
a.site_datatype_id IS NOT NULL and
d.site_datatype_id(+) = a.site_datatype_id and 
m.site_datatype_id(+) = a.site_datatype_id 
group by a.site_datatype_id, a.hm_site_code, a.hm_pcode
;
*/

select lpad(fir.site_code,9) "site_code", fir.pcode "pcode",
fir.r_day "r_day", sec.r_month "r_month"
from (
select a.site_datatype_id, a.hm_site_code site_code, a.hm_pcode pcode,
count(d.site_datatype_id) r_day
 from
ref_hm_site_pcode a, r_day d
where 
a.hm_site_code = UPPER('&&site') and
a.site_datatype_id IS NOT NULL and
d.site_datatype_id(+) = a.site_datatype_id
group by a.site_datatype_id, a.hm_site_code, a.hm_pcode
) fir,
(
select a.site_datatype_id, count(m.site_datatype_id) r_month from
ref_hm_site_pcode a, r_month m
where 
a.hm_site_code = UPPER('&&site') and
a.site_datatype_id IS NOT NULL and
m.site_datatype_id(+) = a.site_datatype_id
group by a.site_datatype_id, a.hm_site_code, a.hm_pcode
) sec
where 
fir.site_datatype_id = sec.site_datatype_id
order by r_day desc
;
quit;
