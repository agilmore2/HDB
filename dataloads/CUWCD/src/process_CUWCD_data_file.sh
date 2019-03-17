#!/bin/bash
# this Bash shell script 
# 1. uses wget program to go get the Central Utah data file from their web page
# 2. Places that file in a  working directory
# 3. Processes the file via the java loader program, placing the data in the HDB database
# 4. If successful; zips the data file and moves it over to the data directory
# 5. If unsucessful,  create an error log indicating a loading problem
#
# this script written by M. Bogner, Sutron Corporation 09-FEB-2012
# this script modified by M. Bogner, Sutron Corporation 15-FEB-2012 to work on ISIS
# this script modified by M. Bogner, Sutron Corporation 18-APRIL-2012 for minor modifications


# get a timestamp to give the output file a unique number
EXTENSION=`date "+%Y%m%d%H"`;

# assign working data location
CUWCD_DATA_FILE=$HDB_ENV/dataloads/CUWCD/work/CUWCD_data.$EXTENSION; export CUWCD_DATA_FILE;
# assign the location for storing processed data files
CUWCD_DATA_LOC=$HDB_ENV/dataloads/CUWCD/data; export CUWCD_DATA_LOC;
# assign the location for storing processed data files
CUWCD_DATA_PROP=$HDB_ENV/dataloads/CUWCD/src/CUWCD_data.property; export CUWCD_DATA_PROP;
#assign the URL to go get the Central UTAH Water Conservancy District Data
CUWCD_DATA_URL=http://data.cuwcd.com/datasets/USBR; export CUWCD_DATA_URL;

echo $CUWCD_DATA_FILE
echo $CUWCD_DATA_LOC
echo $CUWCD_DATA_PROP
echo $CUWCD_DATA_URL

# remove all the data files in the working data directory
/bin/rm $HDB_ENV/dataloads/CUWCD/work/CUWCD_data.*

# remove all the log files in the working data directory
/bin/rm $HDB_ENV/dataloads/CUWCD/work/*.log

# go get the Central Utah data file from their web page
wget -O $CUWCD_DATA_FILE $CUWCD_DATA_URL

# process the data file using the USACE data file loader
# if the java routine successfully  processed this file then zip the file to indicate it was processed

  echo " Processing CUWCD file: $CUWCD_DATA_FILE "
  java -Dstart.property=$CUWCD_DATA_PROP dataloader.ProcessUSACEfile $CUWCD_DATA_FILE

  if [ $? -eq 0 ]
  then
#    The zipping and moving of the data files lines are commented out due to decision not to store these files
#    since they are easily retrievable  from the client
#     gzip $CUWCD_DATA_FILE
#     mv $CUWCD_DATA_FILE.gz $CUWCD_DATA_LOC/.
#     echo "`date` $CUWCD_DATA_FILE file processed and zipped"
     echo "`date` $CUWCD_DATA_FILE file processed"
  fi

