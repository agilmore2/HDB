#!/bin/bash
# this Bash shell script 
# 1. uses ncftp program to go get all files in the ftp directory
# 2. moves them to the working directory
# 3. Processes them via the java loader program, placing the data in the HDB database
# 4. If successful; zips the data file and moves it over to the data directory
# 5. If unsucessful,  create an error log indicating a loading problem
#
# this script written by M. Bogner, Sutron Corporation 19-MAY-2011
# modified by M. Bogner, 15-FEB-2012 to shut off the zipping of the file for future storage

# assign working data location
USACE_FILES=$HDB_ENV/usace2hdb/cespa_work; export USACE_FILES;
# assign the name of the FTP file Location
USACE_FTP_LOC=UC_ALB_COE;  export USACE_FTP_LOC;
# assign the location for storing processed data files
USACE_DATA_LOC=$HDB_ENV/usace2hdb/cespa_data; export USACE_DATA_LOC;
# assign the location for storing processed data files
USACE_RES_DATA_PROP=$HDB_ENV/usace2hdb/src/usace_reservoir_data.property; export USACE_RES_DATA_PROP;
# assign the ftp login file to use for this ftp access
UCHDBA_FTP_LOGIN=$HOME/.ftp_login_file; export UCHDBA_FTP_LOGIN;

echo $USACE_FILES
echo $USACE_FTP_LOC
echo $USACE_DATA_LOC
echo $USACE_RES_DATA_PROP

# remove all the files in the working data directory
# may be not what we want to do so commented it out temporarily
# reinstated on 15-FEB-2012 since the files will no longer be saved
/bin/rm $USACE_FILES/*
/bin/rm $USACE_FILES/*.log

# go get all the files in the ftp UC_ALB_COE directory
#ncftpget -u jzyu@usbr.gov -p 4yuzhenda -F -DD -R ftp://ftp.usbr.gov/UC_ALB_COE/
#ncftpget -u jzyu@usbr.gov -p 4yuzhenda -R ftp://ftp.usbr.gov/UC_ALB_COE/
#
#  This is the production version of get that deletes the files from the FTP site

ncftpget -F -DD -f $UCHDBA_FTP_LOGIN $USACE_FILES $USACE_FTP_LOC/*

# now loop through all the files in the working data directory
 
for f in $USACE_FILES/*
do

# process the data file using the USACE data file loader
# if the java routine successfully  processed this file then zip the file to indicate it was processed

  echo " Processing USACE file: $f "
  java -Dstart.property=$USACE_RES_DATA_PROP dataloader.ProcessUSACEfile $f

  if [ $? -eq 0 ]
  then
     gzip $f
#  commented out to no longer move the zipped files
#     mv $f.gz $USACE_DATA_LOC/.
     echo "`date` $f file processed and zipped"
  fi

# we are done with the looping through the files
done
