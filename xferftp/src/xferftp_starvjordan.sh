#!/bin/sh

# we create the new data in a pipe, and then put it on the server.
res_data 964 40 |\grep -v ORACLE | ncftpput -c -e jordanelle.ftperr -f account.dat COE/jordanelle.dat
res_data 928 40 |\grep -v ORACLE | ncftpput -c -e starvation.ftperr -f account.dat COE/starvation.dat

#run a couple of scripts, writes to hydromet's directory
# then sftp the files here, which removes the padding that
# scp would have copied (annoying!)
# then places those files on the ftp site
ssh -x -a zippy -l hydromet "@hour"
ssh -x -a zippy -l hydromet "@data"
sftp hydromet@zippy:hour.dat .
sftp hydromet@zippy:data.dat .
ncftpput -f account.dat COE/ hour.dat
ncftpput -f account.dat COE/ data.dat
