alter table ref_res_flowlu
add
  constraint  ref_res_flowlu_fk1
  foreign key (site_id)
  references  hdb_site(site_id)
  ;
