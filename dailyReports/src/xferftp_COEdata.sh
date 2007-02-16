#!/bin/sh

# we then create the new data, and then put it on the server.
sqlplus -sil $APP_USER/$HDB_LOCAL@$HDB_LOCAL @fetchCOEdata.sql
ncftpput -o useCLNT=0 -e COEdata.ftperr -f COEaccount.dat /slcusbr USBR.*
\rm -f USBR.*
