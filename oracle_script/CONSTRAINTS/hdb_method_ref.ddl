alter table hdb_method
add constraint hdb_method_fk1
foreign key    (method_class_id)
references     hdb_method_class (method_class_id);
