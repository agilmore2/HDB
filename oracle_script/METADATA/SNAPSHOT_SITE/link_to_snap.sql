CREATE PUBLIC DATABASE LINK &1 CONNECT TO meta_data_user
   IDENTIFIED BY &2 USING '&1';

CREATE PUBLIC SYNONYM run_refresh_hdb_snap&3 for &4..run_refresh_hdb_snap@&1;
CREATE PUBLIC SYNONYM ref_refresh_monitor&3 for &4..ref_refresh_monitor@&1;
quit;




