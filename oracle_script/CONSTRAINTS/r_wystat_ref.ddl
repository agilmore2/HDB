alter table    r_wystat
add
   constraint  r_wystat_fk1
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);

alter table    r_wystat
add
   constraint  r_wystat_fk2
   foreign key (source_id)
   references  hdb_data_source(source_id);

alter table    r_wystat
add
   constraint  r_wystat_ck1
   check (wy > 1900);
