alter table hdb_attr
add constraint hdb_attr_fk1
foreign key    (objecttype_id)
references     hdb_objecttype(objecttype_id)
;

alter table hdb_attr
add constraint hdb_attr_fk2
foreign key    (unit_id)
references     hdb_unit(unit_id)
;
