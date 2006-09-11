echo Enter name of local database:
read db_name

echo Enter dba name at local master installation:
read dba_name

echo Enter dba Oracle password:
read psswd

echo Enter sys Oracle password:
read sys_psswd

echo Enter password for meta_data_user at Snapshot Site:
read remote_mdu_psswd

echo Enter name of snapshot database:
read snapshot_db

echo Enter name of DBA at snapshot database:
read snapshot_dba


echo Running snapshot_link.sql...
sqlplus $dba_name/$psswd @snapshot_link.sql $snapshot_db $remote_mdu_psswd > snapshot_link.out 

echo "**********************"
echo "Check output in snapshot_link.out; ok to continue? (y or n)"
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

echo Running create_linked_syns.sql
sqlplus $dba_name/$psswd @create_linked_syns.sql $snapshot_dba $snapshot_db > create_linked_syns.out

echo "**********************"
echo "Check output in create_linked_syns.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running create_syn_syns_master.sql
sqlplus $dba_name/$psswd @create_syn_syns_master.sql $db_name $dba_name > create_syn_syns_master.out

echo "**********************"
echo "Check output in create_syn_syns_master.out; ok to continue? (y or n)"
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
sqlplus $dba_name/$psswd @../pop_pk_syns.ddl $dba_name $dba_name $db_name > pop_pk_syns.ddl.out

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

echo Ready to run gen_trigs.sql
echo Take a quick look and make sure the WHERE clause includes all tables
echo whose primary key is a single integer that should be
echo automatically generated on insert of a new row.
echo When you have verified that the script is correct, press any key.
read answer

echo Running gen_trigs.sql
sqlplus $dba_name/$psswd < gen_trigs.sql > gen_trigs.out

echo "**********************"
echo "Check output in gen_trigs.out; ok to continue? (y or n)"
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

echo Running grant_meta_data_user.sql
sqlplus $dba_name/$psswd < grant_meta_data_user.sql > grant_meta_data_user.out
 
echo "**********************"
echo "Check output in grant_meta_data_user.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running grant_meta_data_user_wrap.sql
sqlplus $dba_name/$psswd < ../grant_meta_data_user_wrap.sql > grant_meta_data_user_wrap.out

echo "**********************"
echo "Check output in grant_meta_data_user_wrap.out; ok to continue? (y or
n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo Running ../ref_installation.ddl
sqlplus $dba_name/$psswd  @../ref_installation.ddl master > ref_installation.out

echo "**********************"
echo "Check output in ref_installation.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo "** Master installation created. **"
exit






