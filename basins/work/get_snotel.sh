#!/bin/bash
#
# go out on the internet and get the latest snotel data
#
echo ' ' 
echo '     Attempting to log on to NRCS computer in Portland'
echo ' '
umask 022

FTP_HOST=ftp.wcc.nrcs.usda.gov
FTP_DIR=/data/snow/update
#FTP_USER=anonymous
FTP_PASSWD=$HDB_XFER_EMAIL

ncftpget -o useCLNT=0 -p $FTP_PASSWD $FTP_HOST . \
$FTP_DIR/colorado.txt \
$FTP_DIR/riogrand.txt \
$FTP_DIR/ut.txt \
$FTP_DIR/nm.txt \
$FTP_DIR/co.txt 2>ncftp.stderr

# grep out the following error from the standard out, and put the results to stderr so if real errors occur,
# they show up in the error file
grep -v "local file appears to be the same as the remote file, no transfer necessary" ncftp.stderr 1>&2

echo ' '
echo '     FTP Processing complete'
echo ' '
