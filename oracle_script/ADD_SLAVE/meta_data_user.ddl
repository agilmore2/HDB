create user meta_data_user identified by &1
default tablespace HDB_user quota unlimited on HDB_user
temporary tablespace HDB_temp;

grant connect to meta_data_user;

quit;
