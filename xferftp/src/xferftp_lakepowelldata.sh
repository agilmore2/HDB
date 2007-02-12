#!/bin/sh

# we create the new data, and then put it on the server.
res_data 919 100 |\grep -v ORACLE | ncftpput -o useCLNT=0 -c -e lakepowelldata.ftperr -f account.dat lakepowell.dat

