#! /usr/bin/sh

# written by Chastang/Reitsma March 1998
 
# shell script to call the 24month study input DMI
 
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

# Call the SYNC. Check exit status

SYNC
SYNC_result=$?

if test $SYNC_result != 0
then
 echo SYNC exited with status $SYNC_result error
 exit 1
fi

# Call the input DMI passing in the arguments passed into
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
