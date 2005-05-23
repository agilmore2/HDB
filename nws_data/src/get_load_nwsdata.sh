#!/bin/bash
#delete the files
\rm -f data4usbr.unl
touch marker.delete
#activate the get_snotel script which get the data from CBRFC
$HDB_ENV/nws_data/src/get_nwsdata.sh
#see if new files have come back
newest=`ls -t |head -1`
if [ $newest != 'marker.delete' ]
then
  echo ' '
  echo '     new files were retrieved from NWS/CBRFC  -> continue processing'
  echo ' ' 
  . $HDB_ENV/.bashrc_hdb_app
  $HDB_ENV/nws_data/src/nwsdata2hdb.pl data4usbr.unl app_user uchdb2
else 
  echo 'FTP transfer failed!'
  exit 1
fi
\rm marker.delete
echo ' ' 
echo '     The script - get_load_nwsdata - is  complete'
echo ' '
