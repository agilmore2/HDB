/* Roles for czar_user */
grant czar_role to czar_user;
grant hdb_meta_role to czar_user;
alter user czar_user default role hdb_meta_role, czar_role;

/* Privs for meta_data_user */
grant hdb_meta_role to meta_data_user;
alter user meta_data_user
default role connect, hdb_meta_role;

grant insert, delete on ref_phys_quan_refresh_monitor to meta_data_user;