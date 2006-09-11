#! /bin/tcsh -f

# The following calls create the users, table, and triggers/procedures
# needed to implement new HDB permissions scheme.

sqlplus 'uchdba/<PASSWORD> as sysdba' @./PERMISSIONS/create_psswd_user.sql $HDB_LOCAL > create_psswd_user.out;
sqlplus 'uchdba/<PASSWORD> as sysdba' < ./PERMISSIONS/set_role_passwords.sql > set_role_passwords.out;
sqlplus psswd_user/$HDB_LOCAL < ./PERMISSIONS/create_role_psswd.sql > create_role_psswd.out;
sqlplus psswd_user/$HDB_LOCAL < ./PERMISSIONS/check_valid_role_name.prc > check_valid_role_name.out;
sqlplus psswd_user/$HDB_LOCAL < ./PERMISSIONS/role_psswd_fk.trg > role_psswd_fk.out;
sqlplus 'uchdba/<PASSWORD> as sysdba' @./PERMISSIONS/create_app_user.sql $HDB_LOCAL > create_app_user.out;
sqlplus 'uchdba/<PASSWORD> as sysdba' < ./PERMISSIONS/local_user.sql > local_user.out
