#!/bin/bash
#  M. Bogner on 29-March 2009 added the following  2 lines to cleanup before this thing is run
rm *.txt
rm junk
#
touch junk
$HDB_ENV/basins/work/get_snotel.sh
newest=`ls -t |head -1`
if [ $newest != 'junk' ]
then
  echo ' '
  echo '     new files were retrieved from NRCS  -> continue processing'
  echo ' ' 
fi
\rm snotel.dat
cp colorado.txt snotel.dat
cat riogrand.txt >> snotel.dat
cat ut.txt >> snotel.dat
cat co.txt >> snotel.dat
# strip San Juan and Animas from new mexico file
  grep -vi 'san juan river headwaters' nm.txt | \
  grep -vi 'animas river basin' > nmex_temp.report
cat nmex_temp.report >> snotel.dat
echo ' ' 
echo '     Checking file, snotel.dat, for consistency of dates'
echo ' ' 
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
# following was changed by M. Bogner 24-MARCH 2011 for ISIS move test
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
echo ' ' 
echo ' ' 
echo '     The script - get_load_snotel.sh - is  complete'
echo ' '
