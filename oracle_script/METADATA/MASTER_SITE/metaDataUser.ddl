create user meta_data_user identified by &1
default tablespace HDB_user quota unlimited on HDB_user
temporary tablespace HDB_temp quota unlimited on HDB_temp;

grant connect to meta_data_user;
grant hdb_meta_role to meta_data_user;
alter user meta_data_user
default role connect, hdb_meta_role;

quit;
