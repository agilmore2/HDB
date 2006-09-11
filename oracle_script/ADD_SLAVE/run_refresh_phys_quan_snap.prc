CREATE OR REPLACE PROCEDURE RUN_REFRESH_PHYS_QUAN_SNAP
  AS job BINARY_INTEGER;
BEGIN
  dbms_job.submit (job, 'refresh_phys_quan_snap;', sysdate, null, false);
  commit;
END;
/
show errors;

grant execute on run_refresh_phys_quan_snap to public;
drop public synonym run_refresh_phys_quan_snap;
create public synonym run_refresh_phys_quan_snap for run_refresh_phys_quan_snap;

