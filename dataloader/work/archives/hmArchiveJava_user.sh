#!/bin/bash
# run hydromet Archive loader, and get data from hydromet for specified water year

#Must check if user requested version number
if [ $1 ]
then
    if test $1 = "-v"
    then
       echo Version 1.2 #3.2
        exit 0
    fi
    WY=$1
    shift
else 
    echo "Usage: `basename $0` <water year for loading, eg 2003> <sitecodes/pcode,sitecode/pcode,...>"
    exit 1
fi

#handle optional site code parameter, may not contain spaces
if [ $1 ]
then
    SITECODES=$1
else
    SITECODES='*'
fi

if [ $2 ]
then
    echo "Usage: "
    echo "`basename $0` <water year for loading, eg 2003> <sitecodes/pcode,sitecode/pcode,...>"
    echo "site/pcode arguments may NOT contain spaces unless quotes are used!"
    echo "Invalid arguments: $@"
    exit 1
fi

DESTDIR=$ARCHIVE_DIR


FILE=wy$WY.seq
SRC=$ARCHIVES/$FILE

# HOOK UP TO VAX AND CREATE .SEQ FILE
ssh -x  $HYD_HOST -l $HYD_USER @$ARC_COMFILE_DIR'archive_to_seq_user.com' $WY

if [ $? -ne 0 ]
then
    echo " ssh to $HYD_HOST failed; EXITING. "
    exit 1
fi

#simpler, just need right passwordless public/private keypair
scp $HYD_USER@$HYD_HOST:$SRC $DESTDIR

#setup java commandline
#HDB_XFER_EMAIL is the email of the person (NOT uchdba) that is responsible
# for the HDB data loading applications
JAVA_ARGS="-Demailaddr=$HDB_XFER_EMAIL -Dstart.property=$HDB_ENV/dataloader/work/archives/archiveloader.property"

#Make sure file has been successfully copied to $DESTDIR
if [ $? -eq 0 ]; then
    echo "Running archive loader, this may take a long while..."

    java $JAVA_ARGS dataloader.ProcessArchfile filename=$DESTDIR/$FILE SITE_CODES=$SITECODES logname=wy${WY}_user.log
    exit 0;
else
    echo "  ERROR FROM $0 at `date`
    CANNOT COPY FILE $SRC/$FILE.  EITHER $FILE DOES NOT EXIST OR THERE IS
 A PROBLEM ON THE VAX SIDE. "
    exit 1;
fi
