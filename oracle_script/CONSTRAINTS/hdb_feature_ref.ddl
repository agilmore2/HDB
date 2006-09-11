ALTER TABLE hdb_feature
add constraint hdb_feature_fk1
foreign key (feature_class_id)
references hdb_feature_class (feature_class_id);

