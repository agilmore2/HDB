undefine tbl_owner
define tbl_owner = &1
set echo off

PROMPT Creating View 'ref_wa_annotation_view'

create or replace view &&tbl_owner..ref_wa_annotation_view as
select RWA.ANNOTATION_ID,RWA.ANNOTATION_DESCR,RWA.SUPPLY_ID,get_hdb_site_name(RWA.SUPPLY_ID) "SUPPLY_NAME",
RWA.RELEASE_START_DATE, RWA.ANNOTATIONTYPE_ID,
RWT.ANNOTATIONTYPE_NAME,RWA.RELATED_ANNOTATION_ID,RWA.AUXILIARY_ANNOTATION_ID,RWA.ANNOTATION_CMMNT,
to_number(null) "ANNOTATION_LETTER_ID",to_date(null) "LETTER_ISSUED_DATE",
to_number(null) "AMOUNT_REQUESTED_AF",
to_number(null) "VOLUME_ABOVE_OTOWI",
to_number(null) "VOLUME_BELOW_OTOWI",
to_number(null) "GIVEN_TO_WATER_OWNER_ID",
to_date(null)   "DEPLETION_START_DATE",
to_date(null)   "DEPLETION_END_DATE",
to_number(null) "RELEASED_TO_SITE_ID",
to_char(null)   "RELEASED_TO_SITE_NAME",
to_date(null)   "RELEASE_REQUIRED_COMPLETE_DATE",
to_date(null)   "RELEASE_COMPLETED_DATE",
to_number(null) "CONVEYANCE_LOSSES_AF",
to_char(null)   "CONTROL_NUMBER",
to_number(null) "ANNOTATION_LEASE_ID",
to_number(null) "PURCHASED_FROM_WATER_OWNER_ID",
to_char(null)   "CONTRACT_NUMBER",
to_number(null) "LEASE_AMOUNT_AF",
to_number(null) "LEASETYPE_ID",
to_char(null)   "LEASETYPE_NAME",
to_number(null) "ANNOTATION_BORROW_ID",
to_number(null) "BORROWED_FROM_WATER_OWNER_ID",
to_char(null)   "WATER_OWNER_NAME",
to_number(null) "BORROW_AMOUNT_AF",
to_number(null) "BORROWTYPE_ID",
to_char(null)   "BORROWTYPE_NAME",
to_date(null)   "BORROW_DATE"
from ref_wa_annotation rwa, ref_wa_annotationtype rwt
where rwa.ANNOTATIONTYPE_ID = rwt.ANNOTATIONTYPE_ID
and  rwa.AUXILIARY_ANNOTATION_ID is null
union
select RWA.ANNOTATION_ID,RWA.ANNOTATION_DESCR,RWA.SUPPLY_ID,get_hdb_site_name(RWA.SUPPLY_ID),
RWA.RELEASE_START_DATE, RWA.ANNOTATIONTYPE_ID,
RWT.ANNOTATIONTYPE_NAME,RWA.RELATED_ANNOTATION_ID,RWA.AUXILIARY_ANNOTATION_ID,
RWA.ANNOTATION_CMMNT, 
RWA.AUXILIARY_ANNOTATION_ID,RWL.LETTER_ISSUED_DATE,
RWL.AMOUNT_REQUESTED_AF,
RWL.VOLUME_ABOVE_OTOWI,
RWL.VOLUME_BELOW_OTOWI,
RWL.GIVEN_TO_WATER_OWNER_ID,
RWL.DEPLETION_START_DATE,
RWL.DEPLETION_END_DATE,
RWL.RELEASED_TO_SITE_ID,
get_hdb_site_name(RWL.RELEASED_TO_SITE_ID),
RWL.RELEASE_REQUIRED_COMPLETE_DATE,
RWL.RELEASE_COMPLETED_DATE,
RWL.CONVEYANCE_LOSSES_AF,
RWL.CONTROL_NUMBER,
to_number(null),
to_number(null),
to_char(null),
to_number(null),
to_number(null),
to_char(null),
to_number(null),
to_number(null),
RWO.water_owner_name,
to_number(null),
to_number(null),
to_char(null),
to_date(null)
from ref_wa_annotation rwa, ref_wa_annotationtype rwt, ref_wa_annotation_letter rwl, ref_wa_water_owner rwo
where rwa.ANNOTATIONTYPE_ID = rwt.ANNOTATIONTYPE_ID
and  rwt.annotationtype_name = 'letter'
and  rwa.AUXILIARY_ANNOTATION_ID = rwl.ANNOTATION_LETTER_ID
and  rwl.GIVEN_TO_WATER_OWNER_ID = rwo.water_owner_id
union
select RWA.ANNOTATION_ID,RWA.ANNOTATION_DESCR,RWA.SUPPLY_ID,get_hdb_site_name(RWA.SUPPLY_ID), 
RWA.RELEASE_START_DATE, RWA.ANNOTATIONTYPE_ID,
RWT.ANNOTATIONTYPE_NAME,RWA.RELATED_ANNOTATION_ID,RWA.AUXILIARY_ANNOTATION_ID,RWA.ANNOTATION_CMMNT,
to_number(null) ,to_date(null),
to_number(null) ,
to_number(null) ,
to_number(null) ,
to_number(null) ,
to_date(null)   ,
to_date(null)   ,
to_number(null) ,
to_char(null)   ,
to_date(null)   ,
to_date(null)   ,
to_number(null) ,
to_char(null)   ,
RWL.ANNOTATION_LEASE_ID,
RWL.PURCHASED_FROM_WATER_OWNER_ID,
RWL.CONTRACT_NUMBER,
RWL.LEASE_AMOUNT_AF,
RWL.LEASETYPE_ID,
RWLT.LEASETYPE_NAME,
to_number(null) ,
to_number(null) , 
RWO.WATER_OWNER_NAME,
to_number(null), 
to_number(null),
to_char(null),   
to_date(null)   
from ref_wa_annotation rwa, ref_wa_annotationtype rwt, ref_wa_annotation_lease rwl, ref_wa_leasetype rwlt,
ref_wa_water_owner rwo
where rwa.ANNOTATIONTYPE_ID = rwt.ANNOTATIONTYPE_ID
and  rwt.annotationtype_name = 'lease'
and  rwa.AUXILIARY_ANNOTATION_ID =  rwl.annotation_lease_id
and  rwl.leasetype_id = rwlt.leasetype_id 
and  rwl.PURCHASED_FROM_WATER_OWNER_ID = rwo.water_owner_id
union
select RWA.ANNOTATION_ID,RWA.ANNOTATION_DESCR,RWA.SUPPLY_ID,get_hdb_site_name(RWA.SUPPLY_ID), 
RWA.RELEASE_START_DATE, RWA.ANNOTATIONTYPE_ID,
RWT.ANNOTATIONTYPE_NAME,RWA.RELATED_ANNOTATION_ID,RWA.AUXILIARY_ANNOTATION_ID,RWA.ANNOTATION_CMMNT,
to_number(null) ,to_date(null),
to_number(null) ,
to_number(null) ,
to_number(null) ,
to_number(null) ,
to_date(null)   ,
to_date(null)   ,
to_number(null) ,
to_char(null)   ,
to_date(null)   ,
to_date(null)   ,
to_number(null) ,
to_char(null)   ,
to_number(null) ,
to_number(null) ,
to_char(null)   ,
to_number(null) ,
to_number(null) ,
to_char(null)   ,
RWB.ANNOTATION_BORROW_ID,
RWB.BORROWED_FROM_WATER_OWNER_ID,
RWO.WATER_OWNER_NAME,
RWB.BORROW_AMOUNT_AF,
RWB.BORROWTYPE_ID,
RWBT.BORROWTYPE_NAME,
RWB.BORROW_DATE
from ref_wa_annotation rwa, ref_wa_annotationtype rwt, ref_wa_annotation_borrow rwb, ref_wa_borrowtype rwbt,
ref_wa_water_owner rwo
where rwa.ANNOTATIONTYPE_ID = rwt.ANNOTATIONTYPE_ID
and  rwt.annotationtype_name = 'borrow'
and  rwa.AUXILIARY_ANNOTATION_ID = rwb.annotation_borrow_id 
and  rwb.borrowtype_id = rwbt.borrowtype_id
and  rwb.borrowed_from_water_owner_id = rwo.water_owner_id;

CREATE PUBLIC SYNONYM REF_WA_ANNOTATION_VIEW for &&tbl_owner..REF_WA_ANNOTATION_VIEW;
GRANT SELECT on &&tbl_owner..REF_WA_ANNOTATION_VIEW to PUBLIC;

