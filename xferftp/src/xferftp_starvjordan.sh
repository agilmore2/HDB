#!/bin/sh

# we create the new data in a pipe, and then put it on the server.
res_data 964 40 |\grep -v ORACLE | ncftpput -o useCLNT=0 -c -e jordanelle.ftperr -f account.dat COE/jordanelle.dat
res_data 928 40 |\grep -v ORACLE | ncftpput -o useCLNT=0 -c -e starvation.ftperr -f account.dat COE/starvation.dat


#run a couple of scripts, writes to hydromet's directory
# HAVE to use perl Expect in order to get archives and dayfiles to 
# not block waiting for interactive and terminal information
# grr. VMS still does a terminfo inquiry, which takes about 10 seconds
# we can live with that.
./run_coe_commands.pl

# then scp the files here,
# scp does not remove the VMS record based NUL (ASCII 0) padding, so
# we have to do so with the tr -d \0 command

scp hydromet@zippy:hour.dat .
tr -d '\0' < hour.dat > $$.dat
mv $$.dat hour.dat

scp hydromet@zippy:data.dat .
tr -d '\0' < data.dat > $$.dat
mv $$.dat data.dat

# then place those files on the ftp site

ncftpput -o useCLNT=0 -e coehour.ftperr -f account.dat COE/ hour.dat
ncftpput -o useCLNT=0 -e coedata.ftperr -f account.dat COE/ data.dat
