grant hdb_meta_role to meta_data_user;
grant model_priv_role to meta_data_user;

alter user meta_data_user 
default roles connect, hdb_meta_role, model_priv_role;

quit;