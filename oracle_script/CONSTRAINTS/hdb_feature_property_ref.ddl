alter table hdb_feature_property
add constraint hdb_feature_property_fk1
foreign key    (feature_id)
references     hdb_feature(feature_id)
;

alter table hdb_feature_property
add constraint hdb_feature_property_fk2
foreign key    (property_id)
references     hdb_property(property_id)
;

