set verify off
spool wa_views.out

@REF_WA_ANNOTATION_VIEW  &1
@V_WA_AUXILIARY_ANNOTATION  &1
@V_WA_SITE_CONFIG  &1
@REF_WA_SUPPLY_INFO_VIEW  &1
@ref_wa_acct_flows_view  &1
@supplies_flow_view  &1
@account_supplies_rpt_view  &1

spool off
