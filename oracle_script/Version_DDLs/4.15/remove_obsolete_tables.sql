--------------------------------------------------------------------------
-- This script removes below obsolete HDB REF tables that are no longer to use
-- IMPORTANT!!! This script should be executed as main schema user that owns the tables.
-- created on 11/17/2022 by IsmailO
-- REF_DERIVATION_DESTINATION
-- REF_DERIVATION_DEST_ARCHIVE
-- REF_DERIVATION_SOURCE
-- REF_DERIVATION_SOURCE_ARCHIVE
-- REF_DMI_DATA_MAP
-- REF_AGG_DISAGG
--------------------------------------------------------------------------

-- drop tables and synonyms
drop table REF_DERIVATION_DESTINATION cascade constraints;
drop table REF_DERIVATION_DEST_ARCHIVE cascade constraints;
drop table REF_DERIVATION_SOURCE cascade constraints;
drop table REF_DERIVATION_SOURCE_ARCHIVE cascade constraints;
drop table REF_DMI_DATA_MAP cascade constraints;
drop table REF_AGG_DISAGG cascade constraints;
drop public synonym REF_DERIVATION_DESTINATION;
drop public synonym REF_DERIVATION_DEST_ARCHIVE;
drop public synonym REF_DERIVATION_SOURCE;
drop public synonym REF_DERIVATION_SOURCE_ARCHIVE;
drop public synonym REF_DMI_DATA_MAP;
drop public synonym REF_AGG_DISAGG;



--update version table
INSERT INTO TSDB_PROPERTY (PROP_NAME, PROP_VALUE) VALUES ('4.15.1', 'Obsolete REF Derivation,dmi_data_map, agg_disagg tables have been removed. ');
/

--Commit
commit;

