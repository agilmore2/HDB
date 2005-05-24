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

DESTDIR=$ARCHIVE_DIR

set `date`
WY=$6

# increase the water year ONLY if the date is Oct 2 or later; 
# data is for prev. WY if date is Oct 1
if [ `date "+%d"` -gt 1 -a `date "+%m"` -eq 10 ] || [ `date "+%m"` -ge 11 ]; then
	WY=`expr $WY + 1`
	SET_WY=1;
fi

FILE=wy$WY.seq
SRC=$ARCHIVES/$FILE

# HOOK UP TO VAX AND CREATE .SEQ FILE
ssh -x  $HYD_HOST -l $HYD_USER @$ARC_COMFILE_DIR'archive_to_seq.com'

if [ $? -ne 0 ]; then
	echo " ssh to $HYD_HOST failed; EXITING. "
	exit
fi

#simpler, just need right passwordless public/private keypair
scp $HYD_USER@$HYD_HOST:$SRC $DESTDIR

#setup java commandline
#HDB_XFER_EMAIL is the email of the person (NOT uchdba) that is responsible
# for the HDB data loading applications
JAVA_ARGS="-Demailaddr=$HDB_XFER_EMAIL -Dstart.property=$HDB_ENV/dataloader/work/archives/archiveloader.property"

#Make sure file has been successfully copied to $DESTDIR
if [ $? -eq 0 ]; then
    java $JAVA_ARGS dataloader.ProcessArchfile filename=$DESTDIR/$FILE
    exit 0;
else
    echo "  ERROR FROM $0 at `date`
    CANNOT COPY FILE $FILE.  EITHER $FILE DOES NOT EXIST OR THERE IS
 A PROBLEM ON THE VAX SIDE. "
    exit 1;
fi
