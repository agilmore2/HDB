echo Enter dba name at local installation:
read dba_name

echo Enter dba Oracle password:
read psswd

echo Enter password for local meta_data_user to be created
read local_mdu_password

echo "Running metaDataUser.ddl"
sqlplus $dba_name/$psswd @metaDataUser.ddl $local_mdu_password > metaDataUser.out

echo "**********************"
echo "Check output in metaDataUser.out; ok to continue? (y or n)"
read answer
if test $answer != y; then
  echo Fix problems as necessary, then re-run.
  echo Exiting...
  exit
fi

