#!/bin/bash

#function to check if database is up
check_db () {
sqlplus -L -S $APP_USER/$HDB_LOCAL@$HDB_LOCAL <<EOF
select * from dual
quit
EOF
stat=$?
return $stat
}

#first, check to see that database is up

until check_db
do
sleep 10
done

#now start the loader

#setup java commandline
#HDB_XFER_EMAIL is the email of the person (NOT uchdba) that is responsible
# for the HDB data loading application
JAVA_ARGS="-Demailaddr=$HDB_XFER_EMAIL -Dstart.property=$HDB_ENV/dataloader/realtimeloader.property"

java $JAVA_ARGS dataloader.Server 2>stderr >stdout
exit 0;
