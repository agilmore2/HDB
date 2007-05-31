/* command line arguments */
define site = &1;

set verify off;
set feedback off;
set pagesize 9999;
column datatype format A24


select fir.hdb_site_datatype_id,lpad(fir.site_code,9) "site_code", fir.datatype "datatype",
fir.r_day "r_day", sec.r_month "r_month"
from (
select a.hdb_site_datatype_id, a.primary_site_code site_code, a.primary_data_code datatype,
count(d.site_datatype_id) r_day
 from
ref_ext_site_data_map a, r_day d
where 
UPPER(a.primary_site_code) = UPPER('&&site') and
a.hdb_site_datatype_id IS NOT NULL and
d.site_datatype_id(+) = a.hdb_site_datatype_id
group by a.hdb_site_datatype_id, a.primary_site_code, a.primary_data_code
) fir,
(
select a.hdb_site_datatype_id,  a.primary_site_code site_code,
a.primary_data_code datatype,count(m.site_datatype_id) r_month from
ref_ext_site_data_map a, r_month m
where 
UPPER(a.primary_site_code) = UPPER('&&site') and
a.hdb_site_datatype_id IS NOT NULL and
m.site_datatype_id(+) = a.hdb_site_datatype_id
group by a.hdb_site_datatype_id, a.primary_site_code, a.primary_data_code
) sec
where 
fir.hdb_site_datatype_id = sec.hdb_site_datatype_id and
fir.datatype = sec.datatype and
fir.site_code = sec.site_code
order by r_day desc,fir.datatype 
;

set termout off;
column sdi new_value sdi;
select count (distinct hdb_site_datatype_id) sdi from
ref_ext_site_data_map
where
UPPER(primary_site_code) = UPPER('&&site') and
hdb_site_datatype_id IS NOT NULL
;


exit &sdi;
