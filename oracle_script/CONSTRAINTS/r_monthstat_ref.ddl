alter table    r_monthstat
add
   constraint  r_monthstat_fk1
   foreign key (source_id)
   references  hdb_data_source(source_id);

alter table    r_monthstat
add
   constraint  r_monthstat_fk2
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);

alter table    r_monthstat
add
   constraint  r_monthstat_ck1
   check (month between 1 and 12);
