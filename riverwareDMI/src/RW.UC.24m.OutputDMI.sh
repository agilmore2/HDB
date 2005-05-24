#! /usr/bin/sh

# written by Chastang/Reitsma March 1998
 
# shell script to call the UC 24month study output DMI
# The UC version is different from the LC version in that it
# calls aggdisagg to disaggregate monthly flow values at Powell
 
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

# Temporarily delete YAO from ref_db_list
# gotta be dba or someone with ref_meta_role
#

sqlplus -S uchdba/<PASSWORD>@uchdb2 <<EOF
alter table hdb_site disable constraint HDB_SITE_FK7;
delete from ref_db_list where db_site_code = 'YAO';
commit;
quit;
EOF

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

#now insert YAO back into the db.
sqlplus -S uchdba/<PASSWORD>@uchdb2 <<EOF
insert into ref_db_list values(3,'yaohdb','YAO',null,null);
alter table hdb_site enable constraint HDB_SITE_FK7;
commit;
quit;
EOF

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

# A little string formating to put the dates in the format required
# by aggDisagg. Changing from # YYYY-MM-DD into DD-MM-YYYY

echo $3 > /tmp/temp1$$
sed 's/-/ /g' /tmp/temp1$$ > /tmp/temp2$$
awk '{print $3"-"$2"-"$1}' /tmp/temp2$$ > /tmp/temp3$$
read date < /tmp/temp3$$
rm -f /tmp/temp1$$ /tmp/temp2$$ /tmp/temp3$$

# Call aggDisagg with the start date specified by RiverWare. Source
# model_run_id we just received from the Output DMI.
# Disagg 3 months; i.e., last month (start month in the 24mo study
# run), current month and next month.
# Check exit status.

#first add 1 day to the RiverWare startdate for aggDisagg

start_date=`tomorrow $date`

echo Disaggregating Powell monthly releases to daily values.
echo call: aggDisagg app_user $HDB_LOCAL 22 1 n n n m$dmi_result d4 3 $start_date 919
aggDisagg app_user $HDB_LOCAL 22 1 n n n m$dmi_result d4 3 $start_date 919
aggDisagg_result=$?

if test $aggDisagg_result != 0
then
  echo aggDisagg exited with status $aggDisagg_result error
  exit 1
fi
echo aggDisagg exited normally

echo Disaggregating Powell monthly elevations to daily values.
aggDisagg app_user $HDB_LOCAL 74 1 n n n m$dmi_result d4 3 $start_date 919
aggDisagg_result=$?

if test $aggDisagg_result != 0
then
  echo aggDisagg exited with status $aggDisagg_result error
  exit 1
fi
echo aggDisagg exited normally

echo Disaggregating Powell monthly peak inflows to daily values.
aggDisagg app_user $HDB_LOCAL 75 1 n n n m$dmi_result d4 3 $start_date 919
aggDisagg_result=$?

if test $aggDisagg_result != 0
then
  echo aggDisagg exited with status $aggDisagg_result error
  exit 1
fi
echo aggDisagg exited normally

echo Disaggregating Powell monthly storage to daily values.
aggDisagg app_user $HDB_LOCAL 76 1 n n n m$dmi_result d4 3 $start_date 919
aggDisagg_result=$?

if test $aggDisagg_result != 0
then
  echo aggDisagg exited with status $aggDisagg_result error
  exit 1
fi
echo aggDisagg exited normally

exit 0
