alter table ref_dmi_data_map
add
    constraint   ref_dmi_data_map_fk1
    foreign key  (model_id)
    references   hdb_model(model_id);

alter table ref_dmi_data_map
add
    constraint   ref_dmi_data_map_fk2
    foreign key  (site_datatype_id)
    references   hdb_site_datatype(site_datatype_id);
