grant hdb_meta_role to meta_data_user;
alter user meta_data_user
default role connect, hdb_meta_role;

quit;