#! /bin/sh
ncftpput -e sevier.ftperr -m -f account.dat sevier/ /zipdata/slc/xfer/sevier/*.xfr
ncftpput -e weber.ftperr -m -f account.dat weber/ /zipdata/slc/xfer/weber/*.xfr
ncftpput -e price.ftperr -m -f account.dat price/ /zipdata/slc/xfer/price/*.xfr
