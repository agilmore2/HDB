alter table ref_hm_pcode_objecttype
add
   constraint  ref_hm_pcode_objecttype_fk1
   foreign key (hm_pcode) 
   references  ref_hm_pcode(hm_pcode);

alter table ref_hm_pcode_objecttype
add
   constraint  ref_hm_pcode_objecttype_fk2
   foreign key (objecttype_id)
   references  hdb_objecttype(objecttype_id);
