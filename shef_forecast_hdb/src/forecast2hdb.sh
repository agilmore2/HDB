#! /bin/sh 

# This script controls the placement of the runoff season forecasts into the
# HDB database. 
# Version 2.0 checks for other errors than data format.

# Usage: forecast2hdb.sh <email_file_name> hdbuser hdbpassword

# This is a three part process:
#   1. run the shef decoder on the email file
#   2. parse out the resulting output files, check for errors
#   3. run shef_forecast_hdb on the result to place it into the database,
#         placing stderr into a log (overwritten every time)
#         and allowing std out to be read by the user

# $Id$

if test -z "$1"
then
    echo 
    echo "usage:  forecast2hdb.sh <email_file_name> hdbuser hdbpassword"
    echo " **OR** forecast2hdb.sh -v"
    echo
    exit 0
fi

if test $1 = "-v"
then
    echo VERSION 2.0
    exit 0
fi

#check to see that a non-zero length string was given as the password
# (and therefore that 3 arguments were given)

if test -z "$3" 
then
    echo
    echo "usage:  forecast2hdb.sh <email_file_name> hdbuser hdbpassword"
    echo " **OR** forecast2hdb.sh -v"
    echo
    exit 0
fi

# echo the first argument to a file for use in telling shef
# what to decode

echo $1 >/tmp/filename$$

#direct the output of shef to a file, so that we can 
# find the file names and whether any errors occurred
echo "Decoding forecast email with shef_decode."
shef_decode -t < /tmp/filename$$ >/tmp/shefoutput$$

#check shef_decode result for other nastiness before dataerrors:
#if test ! -z `grep shefop:error /tmp/shefoutput$$`
#then
#    echo "**** ERROR ****"
#    echo "Shef output indicates process error, permissions problem, or"
#    echo "other non-data errors."
#    echo "Do you have permission to write in the current directory?"
#    exit 1
#fi

# the name of the shef file that has the decoded output
# is in the 11th field (using space as a delimiter) of the line
# which has the characters ".out" in it, similarly for the
# error file (8th field of ".err" line, empty fields counted) 
shefoutfile=`fgrep .out /tmp/shefoutput$$ |cut -d " " -f11`
sheferrfile=`fgrep .err /tmp/shefoutput$$ |cut -d " " -f8`

#the number of errors is the 6th field of the line with "ERRORS" in it
num_errors=`fgrep ERRORS /tmp/shefoutput$$ | cut -d " " -f6`

# check for a non-zero number of errors
if test $num_errors != 0;
then
    echo "**** ERROR ****"
    echo Shef Decode process found $num_errors error\(s\), exiting!
    echo The format of the forecast email may not be correct.
    echo Check the following file for the shef result: $sheferrfile
    exit 1
fi

#make sure the files were created
if test ! -f $sheferrfile 
then
    echo "**** ERROR ****"
    echo Shef Decode did not create an error file.
    exit 1
elif test ! -f $shefoutfile 
then
    echo "**** ERROR ****"
    echo Shef Decode did not create an output file.
    exit 1
fi

# If we made it this far, we don't need these files any longer
\rm  /tmp/shefoutput$$ /tmp/filename$$

\rm $sheferrfile

#now, run the hdb application that actually places the values into the
# database. We checked earlier that $3 was a real string, so at least
# the username and password ($2 and $3) have been given

echo
echo Calling shef_forecast_hdb to place forecast into HDB

shef_forecast_hdb $shefoutfile $2 $3 2>shef_forecast_hdb.log

shef_forecast_result=$?

if test $shef_forecast_result != 0
then
 echo shef_forecast_hdb exited with status $shef_forecast_result error
 echo Leaving shef decoded output in place for debugging:
 echo $shefoutputfile
 exit 1
fi

#we succeeded, so remove the shef decoded output file
\rm $shefoutfile