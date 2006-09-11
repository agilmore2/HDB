/***********************************************************************/
/*  grant privileges to czar_role                                      */
/***********************************************************************/
grant insert, delete, update on hdb_physical_quantity to czar_role;
grant insert, delete, update on hdb_unit to czar_role;
grant insert, delete, update on hdb_dimension to czar_role;
grant insert, delete, update on ref_hdb_installation to czar_role;
grant insert, delete, update on ref_phys_quan_refresh_monitor to czar_role;