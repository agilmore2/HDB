echo Enter name of local database:
read db_name

echo Enter dba name at local snapshot installation:
read dba_name

echo Enter dba Oracle password:
read psswd

echo Enter sys Oracle password:
read sys_psswd

echo Enter password for meta_data_user at Master Site:
read remote_mdu_psswd

echo Enter name of master database
read master_db

echo Enter name of DBA at Master Database
read master_dba


echo Running snap_ins.sql...
sqlplus $dba_name/$psswd @snap_ins.sql $master_db $remote_mdu_psswd $master_dba > snap_ins.out 

echo "**********************"
echo "Check output in snap_ins.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Recreating procedures invalidated by dropping HDB tables
cd ../../
sqlplus $dba_name/$psswd < ./PROCEDURES/procedure.script > ./METADATA/SNAPSHOT_SITE/procedure.out

echo "**********************"
echo "Check output in procedure.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

cd METADATA/SNAPSHOT_SITE/

echo Running new_hdb_fk.sql
sqlplus  $dba_name/$psswd @new_hdb_fk.sql > new_hdb_fk.out

echo "**********************"
echo "Check output in new_hdb_fk.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Now you must edit new_hdb_fk_script and remove duplicate references to foreign keys.
echo This will happen when there is a composite key, composed of more than one column
echo There is one such key on hdb_site, hdb_site_fk4; also check the FK2s on
echo hdb_attr_feature and hdb_datatype_feature.
echo Hit any key when you have done this
read cont

echo Running new_hdb_fk_script.sql
sqlplus  $dba_name/$psswd @new_hdb_fk_script.sql > new_hdb_fk_script.out

echo "**********************"
echo "Check output in new_hdb_fk_script.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

# removed as per Marra comments  16 DEC 2004
#echo Running v_grant.sql
#sqlplus '$dba_name/$psswd as sysdba' < v_grant.sql > v_grant.out

#echo "**********************"
#echo "Check output in v_grant.out; ok to continue? (y or n)"
#read answer
#if test $answer != y; then
#  echo Fix problems as necessary, then re-run.
#  echo Exiting...
#  exit
#fi

echo Running ref_refresh_monitor.sql
sqlplus $dba_name/$psswd @ref_refresh_monitor.sql $dba_name > ref_refresh_monitor.out

echo "**********************"
echo "Check output in ref_refresh_monitor.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running refresh_hdb_snap.sql
sqlplus $dba_name/$psswd < refresh_hdb_snap.sql > refresh_hdb_snap.out

echo "**********************"
echo "Check output in refresh_hdb_snap.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running run_refresh_hdb_snap.sql
sqlplus $dba_name/$psswd < run_refresh_hdb_snap.sql > run_refresh_hdb_snap.out

echo "**********************"
echo "Check output in run_refresh_hdb_snap.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running refresh_hdb_snap_wrap.sql
sqlplus $dba_name/$psswd < ../refresh_hdb_snap_wrap.sql > refresh_hdb_snap_wrap.out

echo "**********************"
echo "Check output in refresh_hdb_snap_wrap.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running checkSupersystemConnection.sql
sqlplus $dba_name/$psswd < ../checkSupersystemConnection.sql > checkSupersystemConnection.out

echo "**********************"
echo "Check output in checkSupersystemConnection.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running pop_pk_syns.ddl
sqlplus $dba_name/$psswd @../pop_pk_syns.ddl $dba_name $master_dba $master_db > pop_pk_syns.ddl.out

echo "**********************"
echo "Check output in pop_pk_syns.ddl.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running get_pk_val_wrap.sql
sqlplus $dba_name/$psswd < ../get_pk_val_wrap.sql > get_pk_val_wrap.out

echo "**********************"
echo "Check output in get_pk_val_wrap.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running cg_ref_codes.sql
sqlplus $dba_name/$psswd < ../cg_ref_codes.ddl > cg_ref_codes.out

echo "**********************"
echo "Check output in cg_ref_codes.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running V_DT_UT.sql
sqlplus $dba_name/$psswd < ../V_DT_UT.sql > V_DT_UT.out

echo "**********************"
echo "Check output in V_DT_UT.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running V_SITEDT.sql
sqlplus $dba_name/$psswd < ../V_SITEDT.sql > V_SITEDT.out

echo "**********************"
echo "Check output in V_SITEDT.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi


echo Running V_DBA_ROLES.sql 
sqlplus '$dba_name/$psswd as sysdba' < ../V_DBA_ROLES.sql > V_DBA_ROLES.out

echo "**********************"
echo "Check output in V_DBA_ROLES.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running grant_meta_data_user_wrap.sql
sqlplus $dba_name/$psswd < ../grant_meta_data_user_wrap.sql > grant_meta_data_user_wrap.out

echo "**********************"
echo "Check output in grant_meta_data_user_wrap.out; ok to continue? (y
or
n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running ../ref_installation.ddl
sqlplus $dba_name/$psswd  @../ref_installation.ddl snapshot > ref_installation.out

echo "**********************"
echo "Check output in ref_installation.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi


echo "**********************"
echo "YOUR SNAPSHOT INSTALLATION IS **NOT** COMPLETE YET"
echo "You must create a file called gen_trigs_script.sql in this (SNAPSHOT)"
echo "directory. This file will contain the CREATE TRIGGER statements"
echo "from MASTER_SITE/gen_trigs_script.sql for only the REF_ tables."
echo "Cut and paste these create trigger statements (probably 4 of them)"
echo "into your new file, and run it like this: "
echo "    sqlplus <dbaname>/<psswd>@<dbname> < gen_trigs_script.sql"

echo "AND, you must edit your init<DBNAME>.ora file and verify that:"
echo "    job_queue_processes = 1 (or more)."
echo "If you must make any changes, you must shutdown and restart the"
echo "database in order for these changes to take effect"

echo "When the output from gen_trigs_script.sql is without error, and you have"
echo "verified the job_queue_processes parameters on your installation,"
echo "your snapshot installation is complete"

echo "At your leisure, run drop_old_hdb_tables.sql"
echo "This will remove all old HDB_ tables created in the initial DB
creation"
exit

