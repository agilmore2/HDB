alter table ref_res
add
   constraint   ref_res_fk1
   foreign key  (site_id)
   references   hdb_site(site_id)
   ;

alter table ref_res
add
   constraint   ref_res_fk2
   foreign key  (agen_id)
   references   hdb_agen(agen_id)
   ;

alter table ref_res
add
   constraint   ref_res_fk3
   foreign key  (damtype_id)
   references   hdb_damtype(damtype_id)
   ;

alter table ref_res
add
   constraint   ref_res_fk4
   foreign key  (off_id)
   references   hdb_usbr_off(off_id)
   ;
