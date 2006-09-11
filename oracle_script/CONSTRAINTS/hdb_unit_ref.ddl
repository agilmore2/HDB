alter table hdb_unit
add constraint hdb_unit_fk1
foreign key    (dimension_id)
references     hdb_dimension(dimension_id);

alter table hdb_unit
add constraint hdb_unit_fk2
foreign key    (base_unit_id)
references     hdb_unit(unit_id);

alter table hdb_unit
add constraint
check_is_factor
check (is_factor in (0,1));
