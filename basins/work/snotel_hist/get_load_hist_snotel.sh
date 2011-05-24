#!/bin/bash
touch junk
$HDB_ENV/basins/work/snotel_hist/get_hist_snotel.sh $1 $2

stat=$?
#if [ $stat -ne 0 ] 
#  then #DO NOTHING, TOO LOUD!
#  echo "Exiting get_load_hist_snotel due to ftp failure." >&2
#  exit $stat
#fi

newest=`ls -t |head -1`
if [ $newest != 'junk' ]
then
  echo ' '
  echo '     new files were retrieved from NRCS  -> continue processing'
  echo ' ' 
fi

touch snotel.dat
\rm snotel.dat
cp colorado.report snotel.dat
cat riogrand.report >> snotel.dat
cat utah.report >> snotel.dat
# strip San Juan and Animas from new mexico file
  grep -vi 'san juan river headwaters' nmex.report | \
  grep -vi 'animas river basin' > nmex_temp.report
cat nmex_temp.report >> snotel.dat
cat colo.report >> snotel.dat
echo ' ' 
echo '     Checking file, snotel.dat, for consistency of dates'
echo ' ' 
if [ ! -s snotel.dat ]
then
  echo "Yuck! snotel.dat is completely empty, exiting."
  exit 1
fi
grep 'As of' snotel.dat > hold_dates
echo ' '
echo '     Checking to see that the following date was found five times'
dateline=`head -n 1 hold_dates`
echo $dateline
echo ' '
number=`grep -c "$dateline" hold_dates`
if [ $number -eq 5 ]
then
	echo '     The exact date was found five times  -> continue processing'
	echo ' '
	echo '     Initiating load of data into UCHDB'
	. $HDB_ENV/.bashrc_hdb_app
# the following line was changed by M. Bogner on 25-MARCH-2011 for isis move and the change to 64 bit bin
	$HDB_ENV/bin64/basins app_user uchdb2 snotel.dat
else 
  	echo '     Files appear to have different dates -> check the following files for date consistency'
  	echo ' ' 
  	echo '        colorado.report'
  	echo '        riogrand.report'
  	echo '        utah.report'
  	echo '        nmex.report'
  	echo ' '
  	echo '     Data not loaded'
fi
sleep 2
echo ' ' 
echo ' ' 
echo '     The script - get_load_hist_snotel.sh - is  complete'
echo ' '
