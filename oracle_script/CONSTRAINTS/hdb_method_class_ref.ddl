alter table hdb_method_class
add constraint hdb_method_class_fk1
foreign key    (method_class_type)
references     hdb_method_class_type (method_class_type);
