undefine tbl_owner
define tbl_owner = &1
set echo off

PROMPT Creating View 'REF_WA_SUPPLY_INFO_VIEW'
create or replace view &&tbl_owner..ref_wa_supply_info_view
(supply_id, supply_name, upstream_owner_id, upstream_owner_name, upstream_acct_id, upstream_acct_name, acct_reservoir_id,
acct_reservoir_name, upstream_objecttype_name, releasetype_id, releasetype_name)
as
select supply.site_id, supply.site_name, acct_info.water_owner_id,
owner.water_owner_name, upstream_acct.site_id, upstream_acct.site_name,
acct_parent.site_id, acct_parent.site_name,
upstream_objecttype.objecttype_name,
supply_info.releasetype_id, type.releasetype_name
from hdb_site_syn supply, hdb_site_syn upstream_acct,
ref_wa_water_owner owner, ref_wa_releasetype type, hdb_site_syn acct_parent,
ref_wa_supply supply_info, ref_wa_acct acct_info,
hdb_objecttype_syn upstream_objecttype
where supply_info.site_id = supply.site_id
and supply_info.source_acct_id = upstream_acct.site_id
and supply_info.releasetype_id = type.releasetype_id
and upstream_acct.site_id = acct_info.site_id
and upstream_acct.parent_site_id = acct_parent.site_id
and acct_parent.objecttype_id = upstream_objecttype.objecttype_id
and acct_info.water_owner_id = owner.water_owner_id;

CREATE PUBLIC SYNONYM REF_WA_SUPPLY_INFO_VIEW  for &&tbl_owner..REF_WA_SUPPLY_INFO_VIEW;
GRANT SELECT on &&tbl_owner..REF_WA_SUPPLY_INFO_VIEW to PUBLIC;


