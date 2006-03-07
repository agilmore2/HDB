alter table hdb_ext_data_code_sys
add
  constraint  hdb_ext_data_code_sys_fk1
  foreign key (agen_id)
  references  hdb_agen(agen_id);

alter table hdb_ext_data_code_sys
add
  constraint  hdb_ext_data_code_sys_fk2
  foreign key (model_id)
  references  hdb_model(model_id);

