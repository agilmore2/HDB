undefine tbl_owner
define tbl_owner = &1
set echo off

PROMPT Creating View 'account_supplies_rpt_view'

create or replace view &&tbl_owner..account_supplies_rpt_view as
(select acct_id, acct_name ,to_char(sfv.start_date_time,'dd') day,
round(cfs2acft(decode(sfv.value,0,null,sfv.value)),2)  value,
sfv.supply_id,
sfv.supply_name,
sfv.inflow_outflow,
rav.annotation_descr,
to_char(sfv.start_date_time,'MONTH YYYY') reporting_date,
to_char(sfv.start_date_time,'MON YYYY') selection_date
from  supplies_flow_view sfv, ref_wa_annotation_view rav
where sfv.supply_id = rav.supply_id(+)
and   sfv.start_date_time = rav.release_start_date(+));


CREATE PUBLIC SYNONYM account_supplies_rpt_view  for &&tbl_owner..account_supplies_rpt_view;
GRANT SELECT on &&tbl_owner..account_supplies_rpt_view to PUBLIC;


