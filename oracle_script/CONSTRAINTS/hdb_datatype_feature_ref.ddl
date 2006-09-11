ALTER TABLE hdb_datatype_feature
add constraint hdb_datatype_feature_fk1
foreign key (datatype_id)
references hdb_datatype (datatype_id);

ALTER TABLE hdb_datatype_feature
add constraint hdb_datatype_feature_fk2
foreign key (feature_class_id, feature_id)
references hdb_feature (feature_class_id, feature_id);

