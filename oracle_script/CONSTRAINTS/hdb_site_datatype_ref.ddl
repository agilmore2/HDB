alter table hdb_site_datatype
add constraint hdb_site_datatype_fk1
foreign key    (site_id)
references     hdb_site(site_id)
;

alter table hdb_site_datatype
add constraint hdb_site_datatype_fk2
foreign key   (datatype_id)
references    hdb_datatype ( datatype_id)
;
