#!/bin/sh

# we then create the new data, and then put it on the server.
site_data 451 19 100 |\grep -v ORACLE | ncftpput -V -o useCLNT=0 -c -e niipdata.ftperr -f account.dat niip.dat

