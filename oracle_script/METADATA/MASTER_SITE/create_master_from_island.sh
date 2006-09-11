echo "You must run ../SNAPSHOT_SITE/precreate_snapshot_inst.sh on your first"
echo "snapshot installation before running this script!"

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

echo Running update_ref_installation.sql
sqlplus $dba_name/$psswd  @update_ref_installation.sql master > update_ref_installation.out

echo "**********************"
echo "Check output in update_ref_installation.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

echo "** Master installation created. **"
exit






