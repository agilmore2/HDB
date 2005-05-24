#!/bin/bash
# run hydromet Archive loader, and get data from hydromet

#Must check if user requested version number
if [ $1 ]; then
   if test $1 = "-v"
      then
      echo Version 1.0 #3.2
      exit 0
   fi
fi

DESTDIR=$DAYFILE_DIR

DATE=`/usr/local/bin/date +%Y%h%d -d'yesterday'|tr -s '[:upper:]' '[:lower:]'`
FILE=$DATE.seq
SRC=$DAYFILES/$FILE

# HOOK UP TO VAX AND CREATE .SEQ FILE
ssh -x  $HYD_HOST -l $HYD_USER @$DAY_COMFILE_DIR'dayfile_to_seq.com' $DATE

if [ $? -ne 0 ]; then
	echo " ssh to $HYD_HOST failed; EXITING. "
	exit 1
fi

#simpler, just need right passwordless public/private keypair
scp $HYD_USER@$HYD_HOST:$SRC $DESTDIR

#setup java commandline
#HDB_XFER_EMAIL is the email of the person (NOT uchdba) that is responsible
# for the HDB data loading applications
JAVA_ARGS="-Demailaddr=$HDB_XFER_EMAIL -Dstart.property=$HDB_ENV/dataloader/work/dayfiles/dayfileloader.property"

#Make sure file has been successfully copied to $DESTDIR
if [ $? -eq 0 ]; then
    java $JAVA_ARGS dataloader.ProcessDayfile $DESTDIR/$FILE
    exit 0;
else
    echo "  ERROR FROM $0 at `date`
    CANNOT COPY FILE $FILE.  EITHER $FILE DOES NOT EXIST OR THERE IS
 A PROBLEM ON THE VAX SIDE. "
    exit 1;
fi
