alter table    ref_str
add
   constraint  ref_str_fk1
   foreign key (site_id)
   references  hdb_site(site_id);

alter table    ref_str
add
   constraint  ref_str_fk2
   foreign key (owner_id)
   references  hdb_agen(agen_id);

alter table    ref_str
add
   constraint  ref_str_fk3
   foreign key (gagetype_id)
   references  hdb_gagetype(gagetype_id);
