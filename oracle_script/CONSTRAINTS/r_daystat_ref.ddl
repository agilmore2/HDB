alter table    r_daystat
add
   constraint  r_daystat_fk1
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);

alter table    r_daystat
add
   constraint  r_daystat_fk2
   foreign key (source_id)
   references  hdb_data_source(source_id);

alter table    r_daystat
add
   constraint  r_daystat_ck1
   check       (day between 1 and 366);
