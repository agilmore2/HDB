alter table ref_auth_site_datatype
add constraint ref_auth_site_dt_fk1
foreign key    (site_datatype_id)
references     hdb_site_datatype(site_datatype_id)
;
