echo "Disabling the Foreign Keys.  Please wait..."
sqlplus $1/$2 @toggleFKs.sql disable > disableFKs.out
echo "Disabling the Triggers.  Please wait..."
sqlplus $1/$2 @toggleTriggers.sql disable > disableTriggers.out
echo "About to import the Data.  Please wait..."
imp userid = $1/$2 file = siteData.dmp ignore = Y FULL=Y log = siteDataImp.log 
echo "Update table columns.  Please wait..."
sqlplus $1/$2 @table_updates.sql > table_updates.out
echo "Enabling the Foreign Keys.  Please wait..."
sqlplus $1/$2 @toggleFKs.sql enable > enableFKs.out
sqlplus $1/$2 @toggleFKs.sql enable > enableFKs.out
echo "Enabling the Triggers.  Please wait..."
sqlplus $1/$2 @toggleTriggers.sql enable > enableTriggers.out
