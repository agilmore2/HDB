alter table hdb_model_coord
add constraint hdb_model_coord_fk1
foreign key (model_id)
references hdb_model (model_id);

alter table hdb_model_coord
add constraint hdb_model_coord_fk2
foreign key (db_site_code)
references ref_db_list (db_site_code);