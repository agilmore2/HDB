alter table    r_monthstatrange
add
   constraint  r_monthstatrange_fk1
   foreign key (source_id)
   references  hdb_data_source(source_id);

alter table    r_monthstatrange
add
   constraint  r_monthstatrange_fk2
   foreign key (site_datatype_id)
   references  hdb_site_datatype(site_datatype_id);

alter table    r_monthstatrange
add
   constraint  r_monthstatrange_ck1
   check (start_month between 1 and 12);

alter table    r_monthstatrange
add
   constraint  r_monthstatrange_ck2
   check (end_month between 1 and 12);
