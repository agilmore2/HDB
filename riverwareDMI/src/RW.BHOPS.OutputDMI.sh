#! /usr/bin/sh

# written by Chastang/Reitsma March 1998
 
# shell script to call the BHOPS (LC) output DMI
 
# script itself is called from RiverWare as a DMI executable
 
# This script relies on the DMI executable to reside in the
# same directory as this script.
 
# This script does NOT call the SYNC executable as for
# BHOPS input and output a fixed model_run_id (4) is used.
 
# Must check if user requested version first.

if test $1 = "-v"
then
    echo VERSION 1.1
    exit 0
fi

# First, cd to the directory of this script

cd `dirname $0`

# Assign a file for riverwareDMI.Out to write the model_run_id.
# Create the file with an empty echo statement.

echo > /tmp/rtemp$$

# Call riverwareDMI.Out with the -f option. This will make it
# to write the chosen model_run_id to the temp file.
# Also pass in the parameters passed into the script by RiverWare.
# Then check the exit status.

echo Calling riverwareDMI.Out $1 $2 $3 $4 $5 $6 $7 $8 -f/tmp/rtemp$$
riverwareDMI.Out $1 $2 $3 $4 $5 $6 $7 $8 -f/tmp/rtemp$$

#Read model_run_id from temp file.

read dmi_result < /tmp/rtemp$$

#Chk dmi_result.

if test $dmi_result = 1
then
  echo Output DMI exited with status 1 error
  exit 1
fi

if test $dmi_result = -1
then
    echo user exited DMI without selecting a model_run_id
    exit 0
fi

echo riverwareDMI.Out exited normally with status 0

exit 0
