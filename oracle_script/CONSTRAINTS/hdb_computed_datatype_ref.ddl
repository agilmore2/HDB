alter table hdb_computed_datatype
add constraint hdb_computed_datatype_fk1
foreign key    (datatype_id)
references     hdb_datatype (datatype_id);

