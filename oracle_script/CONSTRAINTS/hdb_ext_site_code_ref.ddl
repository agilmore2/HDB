alter table hdb_ext_site_code
add
  constraint  hdb_ext_site_code_fk1
  foreign key (ext_site_code_sys_id)
  references  hdb_ext_site_code_sys(ext_site_code_sys_id);

alter table hdb_ext_site_code
add
  constraint  hdb_ext_site_code_fk2
  foreign key (hdb_site_id)
  references  hdb_site(site_id);

