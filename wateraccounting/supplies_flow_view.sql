undefine tbl_owner
define tbl_owner = &1
set echo off


PROMPT Creating View 'supplies_flow_view'

create or replace view &&tbl_owner..supplies_flow_view as
(select acct.site_id acct_id, get_hdb_site_common_name(acct.site_id) acct_name ,
rd.start_date_time,
rd.value,
rwas1.site_id supply_id,
get_hdb_site_name(rwas1.site_id) supply_name,
decode(rwas1.source_acct_id-acct.site_id,0,'Outflow','Inflow') inflow_outflow 
from r_day rd, hdb_site_datatype hsd, ref_wa_supply rwas1, ref_wa_acct acct
where 
      rd.site_datatype_id = hsd.site_datatype_id
 and  hsd.site_id = rwas1.site_id 
 and  (rwas1.source_acct_id =  acct.site_id or rwas1.downstream_acct_id = acct.site_id));


CREATE PUBLIC SYNONYM supplies_flow_view  for &&tbl_owner..supplies_flow_view;
GRANT SELECT on &&tbl_owner..supplies_flow_view to PUBLIC;



