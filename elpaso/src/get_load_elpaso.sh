#!/bin/bash
#delete the files
\rm -f Dailyflow.htm
#activate the get_snotel script which get the data from portland
$HDB_ENV/elpaso/src/get_elpaso.sh
#see if a file came back
if [ -f 'Dailyflow.htm' ]
then
  echo ' '
  echo '     new files were retrieved from ElPaso  -> continue processing'
  echo ' ' 
  . $HDB_ENV/.bashrc_hdb_app
  $HDB_ENV/bin/elpaso2hdb.pl Dailyflow.htm app_user uchdb2
else 
  echo 'FTP transfer failed!'
  exit 1
fi
echo ' ' 
echo '     The script - get_load_elpaso - is  complete'
echo ' '
