alter table    r_yearstat
add
   constraint  r_yearstat_fk1
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);

alter table    r_yearstat
add
   constraint  r_yearstat_fk2
   foreign key (source_id)
   references  hdb_data_source(source_id);

alter table    r_yearstat
add
   constraint  r_yearstat_ck1
   check (year > 1900);
