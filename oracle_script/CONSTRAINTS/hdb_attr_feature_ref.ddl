ALTER TABLE hdb_attr_feature
add constraint hdb_attr_feature_fk1
foreign key (attr_id)
references hdb_attr (attr_id);

ALTER TABLE hdb_attr_feature
add constraint hdb_attr_feature_fk2
foreign key (feature_class_id, feature_id)
references hdb_feature (feature_class_id, feature_id);

