#! /usr/bin/sh

# written by Chastang/Reitsma March 1998
# revised by N. Wilson May 2000
 
# shell script to call the LC 24month study output DMI.
# This LC version calls aggdisagg to disaggregate monthly reservoir
# evaporation and diversion (pumping) from reservoir for Lake Mead.
 
# script itself is called from RiverWare as a DMI executable
 
# This script relies on the DMI executable to reside in the
# same directory as this script.
 
# Must check if user requested version first.

if test $1 = "-v"
then
    echo VERSION 1.1
    exit 0
fi

# First, cd to the directory of this script

cd `dirname $0` 

# Run SYNC to synchronize the model_run_ids. Check the exit status.

echo Calling SYNC
SYNC
SYNC_result=$?

if test $SYNC_result != 0
then
 echo SYNC exited with status $SYNC_result error
 exit 1
fi

# Assigning a file for riverwareDMI.Out to write the model_run_id.
# Create the file with an empty echo statement.

echo > /tmp/rtemp$$

# Call  riverwareDMI.Out with the -f option. This makes it
# to write the selected model_run_id to the temp file.
# Pass in the parameters passed into this script from 
# Riverware. Check the exit status.

echo Calling riverwareDMI.Out $1 $2 $3 $4 $5 $6 $7 $8 -f/tmp/rtemp$$
riverwareDMI.Out $1 $2 $3 $4 $5 $6 $7 $8 -f/tmp/rtemp$$

#Read model_run_id from temp file.

read dmi_result < /tmp/rtemp$$

#Check dmi_result.

if test $dmi_result = 1
then
  echo Output DMI exited with status 1 error
  exit 1
fi

if test $dmi_result = -1
then
    echo user exited Output DMI without selecting model_run_id
    exit 0
fi

echo Output DMI exited normally with status 0

# Delete model_run_id temp file.
rm -rf /tmp/rtemp$$

# A little string formating to put the date in the format required
# by aggDisagg. Changing from # YYYY-MM-DD into DD-MM-YYYY

echo $3 > /tmp/temp1$$
sed 's/-/ /g' /tmp/temp1$$ > /tmp/temp2$$
awk '{print $3"-"$2"-"$1}' /tmp/temp2$$ > /tmp/temp3$$
read date < /tmp/temp3$$
rm -f /tmp/temp1$$ /tmp/temp2$$ /tmp/temp3$$

# Call aggDisagg with the start date specified by RiverWare. Source
# model_run_id we just received from the Output DMI.

#first add 1 day to the RiverWare startdate for aggDisagg

start_date=`tomorrow $date`

# Disagg current month from the m_month table to the m_day table.
# Check exit status.

echo Disaggregating Mead modeled monthly reservoir evaporation to daily values.
aggDisagg app_user $HDB_LOCAL 11 1 n n n m$dmi_result d4 2 $start_date 921
aggDisagg_result=$?

if test $aggDisagg_result != 0
then
  echo aggDisagg exited with status $aggDisagg_result error
  exit 1
fi
echo aggDisagg exited normally

echo Disaggregating Mead modeled monthly diversions from reservoir to daily values.
aggDisagg app_user $HDB_LOCAL 12 1 n n n m$dmi_result d4 2 $start_date 700
aggDisagg_result=$?

if test $aggDisagg_result != 0
then
  echo aggDisagg exited with status $aggDisagg_result error
  exit 1
fi
echo aggDisagg exited normally


# Disagg last two months from the r_month table to the r_day table.
# Check exit status.

# Manipulation to subtract 2 months from the startdate for the aggDisagg calls

echo $start_date > /tmp/temp4$$
sed 's/-/ /g' /tmp/temp4$$ > /tmp/temp5$$
month=`awk '{print $2}' /tmp/temp5$$`
year=`awk '{print $3}' /tmp/temp5$$`
month=`(echo $month; echo 2- p)|dc`
if test $month = 0
then
  month="12"
  year=`(echo $year; echo 1- p)|dc`
fi
if test $month = -1
then
  month="11"
  year=`(echo $year; echo 1- p)|dc`
fi
if test $month = 12 || test $month = 11 || test $month = 10
then
  echo `awk '{print $1"-"}' /tmp/temp5$$`$month"-"$year > /tmp/temp6$$
else
  echo `awk '{print $1"-0"}' /tmp/temp5$$`$month"-"$year > /tmp/temp6$$
fi
read before_date < /tmp/temp6$$
rm -f /tmp/temp4$$ /tmp/temp5$$ /tmp/temp6$$ 
 
echo Disaggregating Mead real monthly reservoir evaporation to daily values.
echo call: aggDisagg app_user $HDB_LOCAL 11 1 n n n r d 2 $before_date 921
aggDisagg app_user $HDB_LOCAL 11 1 n n n r d 2 $before_date 921
aggDisagg_result=$?

if test $aggDisagg_result != 0
then
  echo aggDisagg exited with status $aggDisagg_result error
  exit 1
fi
echo aggDisagg exited normally

echo Disaggregating Mead real monthly diversions from reservoir to daily values.
aggDisagg app_user $HDB_LOCAL 12 1 n n n r d 2 $before_date 700
aggDisagg_result=$?

if test $aggDisagg_result != 0
then
  echo aggDisagg exited with status $aggDisagg_result error
  exit 1
fi
echo aggDisagg exited normally


exit 0
