undefine tbl_owner
define tbl_owner = &1
set echo off
spool wa_delete_data.out



PROMPT Delete data from table 'REF_WA_ANNOTATION_LETTER'

Delete from &&tbl_owner..ref_wa_ANNOTATION_LETTER;
commit;

PROMPT Delete data from table 'REF_WA_ANNOTATION_BORROW'

Delete from &&tbl_owner..ref_wa_ANNOTATION_BORROW;
commit;

PROMPT Delete data from table 'REF_WA_ANNOTATION_LEASE'

Delete from &&tbl_owner..ref_wa_ANNOTATION_LEASE;
commit;

PROMPT Delete data from table 'REF_WA_ANNOTATION'

Delete from &&tbl_owner..ref_wa_ANNOTATION;
commit;
	

PROMPT Delete data from table 'REF_WA_EXCHANGE_PAYBACK'

Delete from &&tbl_owner..ref_wa_exchange_payback;
commit;
	

PROMPT Delete data from table 'REF_WA_EXCHANGE'

Delete from &&tbl_owner..ref_wa_exchange;
commit;
	

PROMPT Delete data from table 'REF_WA_SUPPLY'

Delete from &&tbl_owner..ref_wa_supply;
commit;
	

PROMPT Delete data from table 'REF_WA_ACCT'

Delete from &&tbl_owner..ref_wa_acct;
commit;
	

PROMPT Delete data from table 'REF_WA_MODEL_SITE_CONFIG'

Delete from &&tbl_owner..ref_wa_model_site_config;
commit;
	

PROMPT Delete data from table 'REF_WA_DMI_SITE_MAP'

Delete from &&tbl_owner..ref_wa_dmi_site_map;
commit;
	

PROMPT Delete Water Accounting data from HDB_SITE Table

delete from &&tbl_owner..hdb_site where objecttype_id in (select objecttype_id from &&tbl_owner..hdb_objecttype where objecttype_name like 'wa %');
commit;
	

spool off

