drop public synonym run_refresh_hdb_snap;
create public synonym run_refresh_hdb_snap for &1..run_refresh_hdb_snap@&2;

drop public synonym ref_refresh_monitor;
create public synonym ref_refresh_monitor for &1..ref_refresh_monitor@&2;
quit;
