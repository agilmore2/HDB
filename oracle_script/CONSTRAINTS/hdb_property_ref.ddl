alter table hdb_property
add constraint hdb_property_fk1
foreign key    (unit_id)
references     hdb_unit(unit_id)
;

alter table hdb_property add constraint
    check_property_value_type
      check (property_value_type in ('number', 'string', 'date'));

