undefine tbl_owner
define tbl_owner = &1
set echo off

PROMPT Creating View 'V_WA_SITE_CONFIG'
CREATE OR REPLACE VIEW &&tbl_owner..V_WA_SITE_CONFIG
 (SITE_ID
 ,CONFIG_ID
 ,object_type)
 AS SELECT site_id, config_id,'ACCT' from ref_wa_acct
 union 
   select site_id, config_id,'SUPPLY' from ref_wa_supply 
 union 
   select site_id, config_id,'EXCHANGE' from ref_wa_exchange 
 union 
   select site_id, config_id,'EXCHANGE_PAYBACK' from ref_wa_exchange_payback 
 union
   select site_id, config_id,'SUBBASIN' from ref_wa_subbasin_members;


CREATE PUBLIC SYNONYM V_WA_SITE_CONFIG for &&tbl_owner..V_WA_SITE_CONFIG;
GRANT SELECT on &&tbl_owner..V_WA_SITE_CONFIG to PUBLIC;


COMMENT ON TABLE V_WA_SITE_CONFIG IS 'View which is the union of all site/config combinations from ref_wa_acct, ref_wa_supply, ref_wa_exchange and ref_wa_subbasin_members. Used to implement FK on ref_wa_model_site_config';


