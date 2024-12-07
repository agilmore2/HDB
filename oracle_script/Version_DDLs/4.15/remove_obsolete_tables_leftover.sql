--------------------------------------------------------------------------
-- This script removes Possible Leftover Obsolete derivation-source objects that are no longer to use
-- IMPORTANT!!! This script should be executed as main schema user that owns the objects.
-- created on 12/20/2022 by IsmailO
--------------------------------------------------------------------------

-- drop leftover objects
drop function GET_DEST_PROP;
drop procedure find_derivation_source;
drop procedure is_derivation_source;
drop procedure is_overwrite;
drop public synonym GET_DEST_PROP;
drop public synonym find_derivation_source;
drop public synonym is_derivation_source;
drop public synonym is_overwrite;




--update version table
INSERT INTO TSDB_PROPERTY (PROP_NAME, PROP_VALUE) VALUES ('4.15.3', ' Leftover Obsolete derivation-source objects have been removed. ');


--Commit
commit;

