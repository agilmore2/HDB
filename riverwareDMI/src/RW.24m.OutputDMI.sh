#! /usr/bin/sh

# written by Chastang/Reitsma March 1998
 
# shell script to call the standard 24month study output DMI.
 
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

exit 0
