alter table    ref_hm_pcode
add    
   constraint  ref_hm_pcode_fk1
   foreign key (unit_id)
   references  hdb_unit(unit_id);
