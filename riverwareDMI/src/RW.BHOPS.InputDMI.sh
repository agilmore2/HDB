#! /usr/bin/sh

# written by Chastang/Reitsma March 1998
 
# shell script to call the BHOPS (LC) input DMI
 
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

# Now call the input DMI passing in the arguments passed into
# this script from RiverWare. Check the exit status.

echo Calling riverwareDMI.In $1 $2 $3 $4 $5 $6 $7 $8
riverwareDMI.In $1 $2 $3 $4 $5 $6 $7 $8

dmi_result=$?
if test $dmi_result != 0
then
  echo Input DMI exited with status $dmi_result error
  exit 1
fi

exit 0
