alter table hdb_datatype
add constraint hdb_datatype_fk1
foreign key    (unit_id)
references     hdb_unit(unit_id);

alter table hdb_datatype
add constraint hdb_datatype_fk2
foreign key    (datatype_type)
references     hdb_datatype_type (datatype_type);

alter table hdb_datatype
add constraint hdb_datatype_fk3
foreign key    (method_class_id)
references     hdb_method_class (method_class_id);

alter table hdb_datatype
add constraint hdb_datatype_fk4
foreign key    (compound_interval)
references     hdb_interval(interval_name);

alter table hdb_datatype
add constraint hdb_datatype_fk5
foreign key    (cumulative_interval)
references     hdb_interval(interval_name);

