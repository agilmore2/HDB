#!/bin/bash
#delete the files
\rm Dailyflo.htm
touch marker.delete
#activate the get_snotel script which get the data from portland
$HDB_ENV/elpaso/src/get_elpaso.sh
#see if new files have come back
newest=`ls -t |head -1`
if [ $newest != 'marker.delete' ]
then
  echo ' '
  echo '     new files were retrieved from ElPaso  -> continue processing'
  echo ' ' 
  . $HDB_ENV/.bashrc_hdb_app
  $HDB_ENV/bin/elpaso2hdb.pl Dailyflo.htm app_user uchdb2
else 
  echo 'FTP transfer failed!'
  exit 1
fi
\rm marker.delete
echo ' ' 
echo '     The script - get_load_elpaso - is  complete'
echo ' '
