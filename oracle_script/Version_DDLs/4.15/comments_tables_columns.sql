--------------------------------------------------------------------------
-- This script add comments on HDB_ REF_ tables and columns
-- IMPORTANT!!! This script should be executed as main schema user that owns the tables.
-- created on 12/21/2022 by IsmailO
--------------------------------------------------------------------------

-- comments on tables and columns




--update version table
INSERT INTO TSDB_PROPERTY (PROP_NAME, PROP_VALUE) VALUES ('4.15.4', 'comments on HDB_ REF_ tables and columns have been added. ');
/

--Commit
commit;

