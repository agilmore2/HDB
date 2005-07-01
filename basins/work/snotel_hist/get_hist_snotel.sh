#!/bin/bash -f
#
# must cut up the argument date and paste it back together
# so that it can become part of the file names to be 
# be retrieved
# go out on the internet and get the historical snotel data
#

FTP_HOST=ftp.wcc.nrcs.usda.gov
FTP_DIR=/data/snow/update
#FTP_USER=anonymous
FTP_PASSWD=$HDB_XFER_EMAIL # from hdb environment


if [ $# != 2 ]
then
	echo 'usage: get_hist_snotel water_year(2001) date(10/31/00)' >&2
	exit 1
fi

if [ $1 -lt 1997 ]
	then
	echo 'water year reports are only available for period from 1997' >&2
	exit 1
fi

echo $2

#lame attempt at verifying date
if [ `echo $2 | egrep '^[0-1][0-9]/[0-3][0-9]/[0-9][0-9]$' >/dev/null` ]
	then
	echo 'usage: date format (10/31/00)' >&2
	exit 1
fi

day=`echo $2 | cut -c4-5`
month=`echo $2 | cut -c1-2`
year=`echo $2 | cut -c7-8`
#echo $day 
#echo $month 
#echo $year
junk=`echo $year$month$day`
#echo $junk

touch colorado.report
touch riogrand.report
touch utah.report
touch wyom.report
touch nmex.report
touch colo.report
\rm -f colorado.report
\rm -f riogrand.report
\rm -f utah.report
\rm -f wyom.report
\rm -f nmex.report
\rm -f colo.report
\rm -f cl$junk.txt
\rm -f rg$junk.txt
\rm -f ut$junk.txt
\rm -f wy$junk.txt
\rm -f nm$junk.txt
\rm -f co$junk.txt

ncftpget -e snotel_hist.err -p $FTP_PASSWD $FTP_HOST . \
$FTP_DIR/colorado/wy$1/cl$junk.txt \
$FTP_DIR/riogrande/wy$1/rg$junk.txt \
$FTP_DIR/ut/wy$1/ut$junk.txt \
$FTP_DIR/wy/wy$1/wy$junk.txt \
$FTP_DIR/nm/wy$1/nm$junk.txt \
$FTP_DIR/co/wy$1/co$junk.txt

stat=$?
if [ $stat -ne 0 ] 
then
# Do nothing, this is too LOUD!
#   echo "FTP transaction failed with NRCS" >&2
#   exit $stat
fi

mv cl$junk.txt colorado.report
mv rg$junk.txt riogrand.report
mv ut$junk.txt utah.report
mv wy$junk.txt wyom.report
mv nm$junk.txt nmex.report
mv co$junk.txt colo.report

echo ' '
echo '     FTP Processing complete'
echo ' '



