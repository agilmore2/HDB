#!/bin/sh

# we then create the new data, and then put it on the server.
sqlplus -sil $APP_USER/$HDB_LOCAL@$HDB_LOCAL @fetchNWSdata.sql
ncftpput -o usePASV=0 -e nwsdata.ftperr ftp.cbrfc.noaa.gov /pub/incoming/bor nwsdata.dat

#this put the nwsdata file on our anonymous ftp site for the NRCS to grab
ncftpput -V -o useCLNT=0 -e NRCSdata.ftperr -f $HDB_ENV/xferftp/src/account.dat /NRCS/ nwsdata.dat

