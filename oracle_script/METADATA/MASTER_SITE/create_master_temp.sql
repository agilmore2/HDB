echo Enter name of local database:
read db_name

echo Enter dba name at local master installation:
read dba_name

echo Enter dba Oracle password:
read psswd

echo Enter sys Oracle password:
read sys_psswd

echo Enter password for local meta_data_user to be created:
read local_mdu_psswd

echo Enter password for meta_data_user at Snapshot Site:
read remote_mdu_psswd

echo Enter name of snapshot database:
read snapshot_db

echo Enter name of DBA at snapshot database:
read snapshot_dba


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

echo "** Master installation created. **"
exit	
