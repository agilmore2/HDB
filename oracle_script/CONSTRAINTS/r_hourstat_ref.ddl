alter table    r_hourstat
add
   constraint  r_hourstat_fk1
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);

alter table    r_hourstat
add
   constraint  r_hourstat_fk2
   foreign key (source_id)
   references  hdb_data_source(source_id);

alter table    r_hourstat
add
   constraint  r_hourstat_ck1
   check       (hour between 0 and 23);
