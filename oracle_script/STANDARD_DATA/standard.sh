sqlplus $1/$2 @./STANDARD_DATA/toggleFKs.sql disable > ./STANDARD_DATA/disableFKs.out
sqlplus $1/$2 @./STANDARD_DATA/toggleTriggers.sql disable > ./STANDARD_DATA/disableTriggers.out
imp userid = $1/$2 file = ./STANDARD_DATA/standardData.dmp ignore = Y grants = N indexes = N rows = Y fromuser = standard touser = $1  log = ./STANDARD_DATA/standardDataImp.log 
sqlplus $1/$2 @./STANDARD_DATA/toggleFKs.sql enable > ./STANDARD_DATA/enableFKs.out
sqlplus $1/$2 @./STANDARD_DATA/toggleTriggers.sql enable > ./STANDARD_DATA/enableTriggers.out
