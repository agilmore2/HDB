#!/bin/sh

#remove all the .csv files
/bin/rm *.csv

# we then create the new data, and then put it on the server.
sqlplus -s / @../src/fetchresdata.sql
#mv *csv /wrg/exec/pub/c5stagesvr_uc_water/res_data

#this put the res data file on our anonymous ftp site for anyone to grab
#ncftpput -V -o useCLNT=0 -e resdata.ftperr -f $HOME/.ftp_login_file /UC_Outgoing_Cron/ *.csv

#this put the res data file on secure.usbr.gov
sftp -oBatchMode=yes jzyu@usbr.gov@secure.usbr.gov<<EOF
cd UC_Outgoing_Cron
put *.csv
bye
EOF
if [$? -ne 0]
then
  echo "Failure on tranfer of csv files"
fi


