undefine tbl_owner
define tbl_owner = &1
set echo off


PROMPT Creating View 'ref_wa_acct_flows_view'
create or replace view &&tbl_owner..ref_wa_acct_flows_view as
select 
hs.parent_site_id res_id,
get_hdb_site_name(hs.parent_site_id) res ,
to_char(rd.start_date_time,'dd') day,
to_number(decode(hdd.datatype_name,'average outflow',null,round(cfs2acft(decode(rd.value,0,null,rd.value)),2))) inflow_val,
to_number(decode(hdd.datatype_name,'average inflow',null,round(cfs2acft(decode(rd.value,0,null,rd.value)),2))) outflow_val,
nvl(to_number(decode(hdd.datatype_name,'average outflow',null,round(cfs2acft(decode(rd.value,0,null,rd.value)),2))),0) - 
nvl(to_number(decode(hdd.datatype_name,'average inflow',null,round(cfs2acft(decode(rd.value,0,null,rd.value)),2))),0) diff_val,
hs.site_common_name acct_name,
hs.site_id   acct_id,
to_char(rd.start_date_time,'MONTH YYYY') report_date,
to_char(rd.start_date_time,'MON YYYY') selection_date,
hdd.datatype_name datatype_name
from r_day rd, hdb_site_datatype hsd, ref_wa_acct rwa , hdb_site hs , hdb_datatype hdd
where
      rd.site_datatype_id = hsd.site_datatype_id
 and  hsd.site_id = hs.site_id
 and  hsd.site_id = rwa.site_id
 and  hsd.datatype_id = hdd.datatype_id;

CREATE PUBLIC SYNONYM ref_wa_acct_flows_view  for &&tbl_owner..ref_wa_acct_flows_view;
GRANT SELECT on &&tbl_owner..ref_wa_acct_flows_view to PUBLIC;
