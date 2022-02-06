-- Added for Master Site 
-- to populate the ref_db_generic_list table for the snapshot_manager
-- added by IsmailO Dec-2021

create or replace TRIGGER  HDB_AGEN_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_AGEN
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_AGEN');

end;
/

create or replace TRIGGER  HDB_FEATURE_CLASS_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_FEATURE_CLASS
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_FEATURE_CLASS');

end;
/

create or replace TRIGGER  HDB_FEATURE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_FEATURE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_FEATURE');

end;
/

create or replace TRIGGER  HDB_PROPERTY_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_PROPERTY
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_PROPERTY');

end;
/

create or replace TRIGGER  HDB_FEATURE_PROPERTY_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_FEATURE_PROPERTY
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_FEATURE_PROPERTY');

end;
/

create or replace TRIGGER  HDB_METHOD_CLASS_TYPE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_METHOD_CLASS_TYPE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_METHOD_CLASS_TYPE');

end;
/

create or replace TRIGGER  HDB_METHOD_CLASS_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_METHOD_CLASS
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_METHOD_CLASS');

end;
/

create or replace TRIGGER  HDB_OPERATOR_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_OPERATOR
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_OPERATOR');

end;
/

create or replace TRIGGER  HDB_METHOD_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_METHOD
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_METHOD');

end;
/

create or replace TRIGGER  HDB_DATE_TIME_UNIT_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_DATE_TIME_UNIT
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_DATE_TIME_UNIT');

end;
/

create or replace TRIGGER  HDB_INTERVAL_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_INTERVAL
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_INTERVAL');

end;
/

create or replace TRIGGER  HDB_OBJECTTYPE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_OBJECTTYPE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_OBJECTTYPE');

end;
/

create or replace TRIGGER  HDB_ATTR_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_ATTR
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_ATTR');

end;
/

create or replace TRIGGER  HDB_ATTR_FEATURE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_ATTR_FEATURE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_ATTR_FEATURE');

end;
/

create or replace TRIGGER  HDB_DATATYPE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_DATATYPE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_DATATYPE');

end;
/

create or replace TRIGGER  HDB_DATATYPE_FEATURE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_DATATYPE_FEATURE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_DATATYPE_FEATURE');

end;
/

create or replace TRIGGER  HDB_DATA_SOURCE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_DATA_SOURCE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_DATA_SOURCE');

end;
/

create or replace TRIGGER  HDB_DIVTYPE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_DIVTYPE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_DIVTYPE');

end;
/

create or replace TRIGGER  HDB_GAGETYPE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_GAGETYPE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_GAGETYPE');

end;
/

create or replace TRIGGER  HDB_MODELTYPE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_MODELTYPE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_MODELTYPE');

end;
/

create or replace TRIGGER  HDB_RIVER_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_RIVER
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_RIVER');

end;
/

create or replace TRIGGER  HDB_STATE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_STATE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_STATE');

end;
/

create or replace TRIGGER  HDB_VALIDATION_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_VALIDATION
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_VALIDATION');

end;
/

create or replace TRIGGER  HDB_USBR_OFF_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_USBR_OFF
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_USBR_OFF');

end;
/

create or replace TRIGGER  HDB_RIVER_REACH_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_RIVER_REACH
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_RIVER_REACH');

end;
/

create or replace TRIGGER  HDB_MODEL_REACH_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_MODEL
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_MODEL');

end;
/

create or replace TRIGGER  HDB_DMI_UNIT_MAP_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_DMI_UNIT_MAP
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_DMI_UNIT_MAP');

end;
/

create or replace TRIGGER  HDB_DAMTYPE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_DAMTYPE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_DAMTYPE');

end;
/

create or replace TRIGGER  HDB_DERIVATION_FLAG_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_DERIVATION_FLAG
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_DERIVATION_FLAG');

end;
/

create or replace TRIGGER  HDB_OVERWRITE_FLAG_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_OVERWRITE_FLAG
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_OVERWRITE_FLAG');

end;
/

create or replace TRIGGER  HDB_COLLECTION_SYSTEM_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_COLLECTION_SYSTEM
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_COLLECTION_SYSTEM');

end;
/

create or replace TRIGGER  HDB_LOADING_APPLICATION_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_LOADING_APPLICATION
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_LOADING_APPLICATION');

end;
/

create or replace TRIGGER  HDB_EXT_SITE_CODE_SYS_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_EXT_SITE_CODE_SYS
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_EXT_SITE_CODE_SYS');

end;
/

create or replace TRIGGER  HDB_EXT_DATA_CODE_SYS_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_EXT_DATA_CODE_SYS
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_EXT_DATA_CODE_SYS');

end;
/

create or replace TRIGGER  HDB_EXT_SITE_CODE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_EXT_SITE_CODE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_EXT_SITE_CODE');

end;
/

create or replace TRIGGER  HDB_EXT_SITE_CODE_ARCHIVE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_EXT_SITE_CODE_ARCHIVE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_EXT_SITE_CODE_ARCHIVE');

end;
/

create or replace TRIGGER  HDB_EXT_DATA_CODE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_EXT_DATA_CODE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_EXT_DATA_CODE');

end;
/

create or replace TRIGGER  HDB_EXT_DATA_CODE_ARCHIVE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_EXT_DATA_CODE_ARCHIVE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_EXT_DATA_CODE_ARCHIVE');

end;
/

create or replace TRIGGER  HDB_EXT_DATA_SOURCE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_EXT_DATA_SOURCE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_EXT_DATA_SOURCE');

end;
/

create or replace TRIGGER  HDB_EXT_DATA_SOURCE_ARCHIVE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_EXT_DATA_SOURCE_ARCHIVE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_EXT_DATA_SOURCE_ARCHIVE');

end;
/

create or replace TRIGGER  HDB_SITE_ARCHIVE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_SITE_ARCHIVE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_SITE_ARCHIVE');

end;
/

create or replace TRIGGER  HDB_SITE_DATATYPE_ARCHIVE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_SITE_DATATYPE_ARCHIVE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_SITE_DATATYPE_ARCHIVE');

end;
/

create or replace TRIGGER  HDB_DATATYPE_ARCHIVE_SNAP_MODIFIED
after             insert OR update OR delete
on                HDB_DATATYPE_ARCHIVE
for   each row
declare
begin

     /* populate the ref_db_generic_list table for the snapshot_manager  */
     /* added by IsmailO Dec-2021  */
     snapshot_manager.snapshot_modified('HDB_DATATYPE_ARCHIVE');

end;
/













