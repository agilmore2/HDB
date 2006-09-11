echo Setting up czar-slave relationship...

# Czar
echo Enter czar database name
read czar_db
echo Enter czar DBA name
read czar_dba
echo Enter czar DBA password
read czar_dba_passwd
echo Enter czar meta_data_user password
read czar_md_passwd

# Slave
echo Enter slave database name
read slave_db
echo Enter slave DBA name
read slave_dba
echo Enter slave DB 3-letter code
read slave_code
echo Enter slave DBA password
read slave_dba_passwd
echo Enter slave meta_data_user password
read slave_md_passwd

# Create meta_data_user on slave
sqlplus $slave_dba/$slave_dba_passwd@$slave_db  @./meta_data_user.ddl $slave_md_passwd > ./meta_data_user.out
echo Check ./meta_data_user.out for errors. Press any key to continue.
read anykey

# Create link from slave to czar
sqlplus $slave_dba/$slave_dba_passwd@$slave_db  @./create_db_link.sql $czar_db $czar_md_passwd > ./create_link_to_czar.out
echo Check ./create_link_to_czar.out for errors. Press any key to continue.
read anykey

# Create slave synonym to monitor table on czar
sqlplus $slave_dba/$slave_dba_passwd@$slave_db  @./slave_monitor_syn.ddl $czar_dba $czar_db > ./slave_monitor_syn.out
echo Check ./slave_monitor_syn.out for errors. Press any key to continue.
read anykey

# Create procedures on slave
sqlplus $slave_dba/$slave_dba_passwd@$slave_db  < ./slave_procedure.script > .//slave_procedure.out
echo Check ./slave_procedure.out for errors. Press any key to continue.
read anykey

# Create link from czar to slave
sqlplus $czar_dba/$czar_dba_passwd@$czar_db  @./create_db_link.sql $slave_db $slave_md_passwd > ./slave_create_db_link.out
echo Check ./slave_create_db_link.out for errors. Press any key to continue.
read anykey

# Create synonym from Czar to run_refresh procedure on slave
sqlplus $czar_dba/$czar_dba_passwd@$czar_db  @./czar_refresh_syn.ddl $slave_code $slave_dba $slave_db > ./czar_refresh_syn.out
echo Check ./czar_refresh_syn.out for errors. Press any key to continue.
read anykey

