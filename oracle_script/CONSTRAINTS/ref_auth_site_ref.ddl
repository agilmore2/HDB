alter table ref_auth_site
add constraint ref_auth_site_fk1
foreign key    (site_id)
references     hdb_site(site_id)
;
