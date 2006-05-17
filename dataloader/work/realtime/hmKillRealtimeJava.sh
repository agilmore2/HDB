#!/bin/bash
#setup java commandline
#HDB_XFER_EMAIL is the email of the person (NOT uchdba) that is responsible
# for the HDB data loading application
JAVA_ARGS="-Demailaddr=$HDB_XFER_EMAIL -Dstart.property=$HDB_ENV/dataloader/work/realtime/realtimeloader.property"

java $JAVA_ARGS dataloader.KillServer $1
exit 0;
