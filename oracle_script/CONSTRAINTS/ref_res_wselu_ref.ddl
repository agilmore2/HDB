alter table ref_res_wselu
add
  constraint  ref_res_wselu_fk1
  foreign key (site_id)
  references  hdb_site(site_id);
