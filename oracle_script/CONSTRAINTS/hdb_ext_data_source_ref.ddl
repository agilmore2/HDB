alter table hdb_ext_data_source
add
  constraint  hdb_ext_data_source_fk1
  foreign key (agen_id)
  references  hdb_agen(agen_id);

alter table hdb_ext_data_source
add
  constraint  hdb_ext_data_source_fk2
  foreign key (model_id)
  references  hdb_model(model_id);

alter table hdb_ext_data_source
add
  constraint  hdb_ext_data_source_fk3
  foreign key (ext_site_code_sys_id)
  references  hdb_ext_site_code_sys(ext_site_code_sys_id);

alter table hdb_ext_data_source
add
  constraint  hdb_ext_data_source_fk4
  foreign key (ext_data_code_sys_id)
  references  hdb_ext_data_code_sys(ext_data_code_sys_id);

alter table hdb_ext_data_source
add
  constraint  hdb_ext_data_source_fk5
  foreign key (collection_system_id)
  references  hdb_collection_system(collection_system_id);

