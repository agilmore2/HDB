#!/bin/bash
#
#
# go out to the Colorado Basin River Forecast Center and get some
# data from nws sites.
#
echo ' ' 
echo '     Attempting to log on to CBRFC computer in Salt Lake'
echo ' '
umask 022

FTP_HOST=ftp.cbrfc.noaa.gov
FTP_DIR=/pub/outgoing/bor
FTP_USER=anonymous
FTP_PASSWD=$HDB_XFER_EMAIL

ncftpget -o useCLNT=0 -u $FTP_USER -p $FTP_PASSWD $FTP_HOST . $FTP_DIR/data4usbr.unl

echo ' '
echo '     FTP Processing complete'
echo ' '
