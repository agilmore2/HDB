#!/bin/sh

# we first create the new data file with the sql script
#
sqlplus -sl $APP_USER/$HDB_LOCAL@$HDB_LOCAL @fetchNWSdata.sql 
#
# we then copy the file to a location where Denver can pick it up
# and move it to the production web server
#
\cp -f nwsdata.dat /wrg/exec/pub/c5stagesvr_uc_water/nws_data_file/.
#

