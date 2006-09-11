alter table hdb_physical_quantity
add constraint hdb_physical_quantity_fk1
foreign key    (dimension_id)
references     hdb_dimension(dimension_id);

alter table hdb_physical_quantity
add constraint hdb_physical_quantity_fk2
foreign key    (customary_unit_id)
references     hdb_unit(unit_id);
