undefine tbl_owner
define tbl_owner = &1
set echo off

PROMPT Creating View 'V_WA_AUXILIARY_ANNOTATION'
CREATE OR REPLACE VIEW &&tbl_owner..V_WA_AUXILIARY_ANNOTATION
 (ANNOTATIONTYPE_ID
 ,AUXILIARY_ANNOTATION_ID)
 AS SELECT t.annotationtype_id, a.annotation_letter_id
from ref_wa_annotationtype t, ref_wa_annotation_letter a
    where lower(t.annotationtype_name) like '%letter%'
union
select  t.annotationtype_id, a.annotation_borrow_id
from ref_wa_annotationtype t, ref_wa_annotation_borrow a
where lower(t.annotationtype_name) like '%borrow%'
union
select  t.annotationtype_id, a.annotation_lease_id
from ref_wa_annotationtype t, ref_wa_annotation_lease a
where lower(t.annotationtype_name) like '%lease%'
union
select  t.annotationtype_id, to_number(NULL)
from ref_wa_annotationtype t;

CREATE PUBLIC SYNONYM V_WA_AUXILIARY_ANNOTATION for &&tbl_owner..V_WA_AUXILIARY_ANNOTATION;
GRANT SELECT on &&tbl_owner..V_WA_AUXILIARY_ANNOTATION to PUBLIC;


COMMENT ON TABLE V_WA_AUXILIARY_ANNOTATION IS 'View which is the join of the annotation_ids from the letter, borrow and lease annotation tables, paired with the annotationtype_id appropriate to each (to create uniqueness). This is joined with a unique list of annotationtype_ids (without an auxiliary_a';


