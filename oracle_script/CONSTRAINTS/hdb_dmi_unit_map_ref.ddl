alter table hdb_dmi_unit_map
add
   constraint    hdb_dmi_unit_map_fk1
   foreign key   (unit_id)
   references    hdb_unit(unit_id);
