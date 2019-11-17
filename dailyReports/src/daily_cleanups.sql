
spool daily_ops.out

-- this line finishes up partial calculations
-- execute hdb_utilities.computations_cleanup;

-- this line will add any datatypes that may have been recently created
--insert into datatype
--select datatype_id,'hdb',datatype_id from hdb_datatype
--minus select id,standard,id from datatype where standard = 'hdb';

-- new way of doing this plus new call to snapshot_manager procedure
-- change by M. Bogner April 30 2013

 execute hdb_utilities.daily_ops;
 commit;
 execute snapshot_manager.perform_refresh('%');
 commit;

spool off

exit;
