alter table ref_site_coef
add
  constraint  ref_site_coef_fk1
  foreign key (site_id)
  references  hdb_site(site_id);

alter table ref_site_coef
add
  constraint  ref_site_coef_fk2
  foreign key (attr_id)
  references  hdb_attr(attr_id);
