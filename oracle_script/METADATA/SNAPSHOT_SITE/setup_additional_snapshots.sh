\rm RUN_REMOTE.TXT

missing_info=0;

echo With the newest snapshot created, how many snapshots are now in the system?
read n_snapshots

echo "Enter name of local database (newest snapshot):"
read db_name

echo Enter dba name at local snapshot installation:
read dba_name

echo Enter dba Oracle password:
read psswd

echo Enter password for local meta_data_user:
read local_mdu_psswd

echo NOTE: the order in which you list snapshots is NOT important!
n=1

while [ $n -le $n_snapshots ]; do
   if [ $n -eq 1 ]; then
      db_type="master"
   else
      db_type="remote snapshot `expr $n - 1`"
   fi

   echo "N= " $n
   echo "dbtype = " $db_type

   echo Enter name of $db_type database
   read other_db_name
  
   echo "Enter name of DBA at $db_type database ('?' if don't know)"
   read other_dba

   echo "Enter password for DBA at $db_type database ('?' if don't know)"
   read other_psswd

   echo "Enter password for meta_data_user at $db_type database ('?' if don't know)"
   read other_mdu_psswd   

   if [ $other_dba = "?" -o $other_psswd = "?" -o $other_mdu_psswd = "?" ]; then

      if [ missing_info -eq 0 ]; then
        echo "CREATE PUBLIC DATABASE LINK " $db_name " CONNECT TO meta_data_user" >> RUN_REMOTE.TXT
        echo "    IDENTIFIED BY " $local_mdu_psswd "USING '"$db_name"';"
        echo "CREATE PUBLIC SYNONYM run_refresh_hdb_snap"$n_snapshots "for" $dba_name".run_refresh_hdb_snap@"$db_name";" >> RUN_REMOTE.TXT
        echo "CREATE PUBLIC SYNONYM ref_refresh_monitor"$n_snapshots "for" $dba_name".ref_refresh_monitor@"$db_name";" >> RUN_REMOTE.TXT
        echo " " >> RUN_REMOTE.TXT
        echo "Run the above commands on the following databases, as the DBA:" >> RUN_REMOTE.TXT
        echo " "
        missing_info=1;
      fi

      echo "     " $other_db_name >> RUN_REMOTE.TXT
   else
      sqlplus $other_dba/$other_psswd@$other_db_name @link_to_snap.sql $db_name $local_mdu_psswd $n_snapshots $dba_name > link_to_new_snap.out$n
   fi

   if [ $n -gt 1 ]; then
      sqlplus $dba_name/$psswd @link_to_snap.sql $other_db_name $other_mdu_psswd $n_snapshots $other_dba_name > link_to_existing_snap.out$n
   fi

   n=`expr $n + 1`

done

if [ missing_info > 0 ]; then
   echo "If file RUN_REMOTE.TXT exists, you must be sure that the indicated commands are run on the specified databases."


exit
