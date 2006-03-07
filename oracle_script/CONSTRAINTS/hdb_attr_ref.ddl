alter table hdb_attr
add constraint hdb_attr_fk1
foreign key    (unit_id)
references     hdb_unit(unit_id)
;

alter table hdb_attr add constraint
    check_attr_value_type
      check (attr_value_type in ('number', 'string', 'date'));

