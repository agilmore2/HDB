#!/bin/bash
#
# first line lets shell know that this is a C script
# the -f tells the shell not to source the .cshrc file
# thus none of the aliases in that file will be valid
# the -x causes an echo of commands to standard output
#
# go get data from El Paso
#
echo ' ' 
echo '     Attempting to log on to ElPaso computer'
echo ' '
umask 022

FTP_HOST=140.217.80.16 # elpsun1.rgpo.uc.usbr.gov
FTP_DIR=/usr1/home/jgrajeda/website
FTP_USER=waterguy
FTP_PASSWD=sea22sea

ncftpget -u $FTP_USER -p $FTP_PASSWD $FTP_HOST . $FTP_DIR/Dailyflo.htm

echo ' '
echo '     FTP Processing complete'
echo ' '
