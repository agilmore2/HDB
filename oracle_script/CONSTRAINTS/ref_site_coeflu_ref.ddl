alter table    ref_site_coeflu
add 
   constraint  ref_site_coeflu_fk1
   foreign key (site_id)
   references  hdb_site(site_id);

alter table    ref_site_coeflu
add 
   constraint  ref_site_coeflu_fk2
   foreign key (attr_id)
   references  hdb_attr(attr_id);

alter table    ref_site_coeflu
add 
   constraint  ref_site_coeflu_fk3
   foreign key (lu_attr_id)
   references  hdb_attr(attr_id);
