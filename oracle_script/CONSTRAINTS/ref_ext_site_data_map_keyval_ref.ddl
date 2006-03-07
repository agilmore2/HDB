alter table ref_ext_site_data_map_keyval
add
  constraint ref_ext_site_data_map_keyv_fk1
  foreign key (mapping_id)
  references  ref_ext_site_data_map(mapping_id);
