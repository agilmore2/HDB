alter table hdb_datatype
add constraint hdb_datatype_fk1
foreign key    (physical_quantity_name)
references     hdb_physical_quantity(physical_quantity_name);

alter table hdb_datatype
add constraint hdb_datatype_fk2
foreign key    (unit_id)
references     hdb_unit(unit_id);

alter table hdb_datatype
add constraint hdb_datatype_fk3
foreign key    (agen_id)
references     hdb_agen(agen_id);

alter table    hdb_datatype
add
   constraint  hdb_datatype_ck1
   check       (allowable_intervals in ('non-instant','instant','either'));


