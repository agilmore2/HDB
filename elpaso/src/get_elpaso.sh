#!/bin/bash
#
# go get data from El Paso
#
echo ' ' 
echo '     Attempting to log on to ElPaso computer'
echo ' '
umask 022

FTP_DIR=/usr1/home/jgrajeda/website

ncftpget -V -o useCLNT=0 -e elpaso.ftperr -f account.dat . $FTP_DIR/Dailyflow.htm

echo ' '
echo '     FTP Processing complete'
echo ' '
