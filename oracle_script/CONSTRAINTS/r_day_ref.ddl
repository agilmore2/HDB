alter table    r_day
add
   constraint  r_day_fk1
   foreign key (validation)
   references  hdb_validation(validation);

alter table    r_day
add
   constraint  r_day_fk2
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);

alter table    r_day
add
   constraint  r_day_fk3
   foreign key (source_id)
   references  hdb_data_source(source_id);

alter table    r_day
add
   constraint  r_day_fk4
   foreign key (overwrite_flag)
   references  hdb_overwrite_flag(overwrite_flag);

alter table    r_day
add
   constraint  r_day_fk5
   foreign key (method_id)
   references  hdb_method(method_id);

