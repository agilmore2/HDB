alter table hdb_ext_data_code
add
  constraint  hdb_ext_data_code_fk1
  foreign key (ext_data_code_sys_id)
  references  hdb_ext_data_code_sys(ext_data_code_sys_id);

alter table hdb_ext_data_code
add
  constraint  hdb_ext_data_code_fk2
  foreign key (hdb_datatype_id)
  references  hdb_datatype(datatype_id);
