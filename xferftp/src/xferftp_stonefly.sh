#! /bin/sh
#ncftpput -V -o useCLNT=0 -e sevier.ftperr -m -f account.dat sevier/ /zipdata/slc/xfer/sevier/*.xfr
#ncftpput -V -o useCLNT=0 -e weber.ftperr -m -f account.dat weber/ /zipdata/slc/xfer/weber/*.xfr
#ncftpput -V -o useCLNT=0 -e price.ftperr -m -f account.dat price/ /zipdata/slc/xfer/price/*.xfr
ncftpput -V -o useCLNT=0 -e paradox.ftperr -m -f account.dat paradox/ /zipdata/slc/xfer/paradox/*.xfr
ncftpput -V -o useCLNT=0 -e mancos.ftperr -m -f account.dat mancos/ /zipdata/slc/xfer/mancos/*.xfr
