#! /bin/sh
ncftpput -m -f account.dat sevier/ /zipdata/slc/xfer/*.xfr
ncftpput -m -f account.dat weber/ /zipdata/slc/xfer/weber/*.xfr
ncftpput -m -f account.dat price/ /zipdata/slc/xfer/price/*.xfr
