CREATE OR REPLACE PROCEDURE RUN_REFRESH_HDB_SNAP (which_table IN VARCHAR2) 
  AS job BINARY_INTEGER;
BEGIN
  dbms_job.submit (job, 'refresh_hdb_snap (''' || which_table || ''');', sysdate, null, false);
  commit;
END;
/
grant execute on run_refresh_hdb_snap to public;
drop public synonym run_refresh_hdb_snap;
create public synonym run_refresh_hdb_snap for run_refresh_hdb_snap;

