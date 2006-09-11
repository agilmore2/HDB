-- Snapshot site installation script
-- Revised 2/03 to automatically include all hdb_ tables, rather
-- than requiring manual maintenance when and hdb_ table is added to the
-- scheam.
-- Arguments:
--   &1 master db name
--   &2 master meta data user password
--   &3 master DBA name
REM prompt 'Snapshot site installation script. ';

set echo off
set heading off
set feedback off
set pagesize 0
set linesize 220
set verify off

spool snap_ins_script.sql

-- The creation of this database link assumes a connect descriptor of
-- 'uchdb' or 'uchdb.world' for "logging in" to the Upper Colorado HDB
-- That is, if tnsnames.ora is being used, the connect info should have 
-- a descriptor of 'uchdb' in order for this to work.
-- This database link can be used by any user, but every user will 
-- connect to the linked database as meta_data_user. Hence, no new
-- users need be created on the remote DB in order to accomodate new
-- users on the local DB.

select 'CREATE PUBLIC DATABASE LINK &1 CONNECT TO meta_data_user IDENTIFIED BY &2 USING ''&1'';'
from dual;

select ' ' from dual;
-- Rename HDB_ tables
select 'rename ' || table_name || ' to ' || substr(table_name,1,26) || '_OLD;'
from dba_tables 
where table_name like 'HDB_%'
and table_name not like '%_OLD'
and table_name not in ('HDB_DIMENSION','HDB_UNIT','HDB_PHYSICAL_QUANTITY')
order by table_name;

select ' ' from dual;
-- Drop misc triggers; drop hdb_feature_old so the UK and FKs that
-- depend on it can be created.
select 'DROP TRIGGER sdi_chk_perm;' from dual;
select 'DROP TRIGGER hdb_site_site_perm;' from dual;
select 'DROP TABLE HDB_FEATURE_OLD;' from dual;

select ' ' from dual;
-- Drop public synonyms for the old HDB tables
select 'drop public synonym ' || synonym_name || ';'
from dba_synonyms where synonym_name like 'HDB_%'
and synonym_name not in ('HDB_DIMENSION','HDB_UNIT','HDB_PHYSICAL_QUANTITY');

select ' ' from dual;
-- Drop primary keys for the old HDB tables, so the PK names on
-- snapshots will be the same as on master installations
select 'alter table ' || table_name || ' drop primary key; '
from dba_constraints where table_name like 'HDB_%OLD'
and constraint_type = 'P';


select ' ' from dual;
-- Create snapshots based on the Master HDB tables;
-- ext_archives do not have PKs, so snapshots need to be
-- created with different structure. attr_feature and datatype_feature
-- use UK on hdb_feature, so create them separately later.
select 'CREATE SNAPSHOT ' || table_name ||
       ' PCTFREE 10 PCTUSED 40 TABLESPACE HDB_DATA storage (initial 50k next 50k pctincrease 0) ' ||
       ' AS SELECT * FROM  &3..' || table_name || '@&1;'
from dba_tables
where table_name like 'HDB_%'
and table_name not in ('HDB_DIMENSION','HDB_UNIT','HDB_PHYSICAL_QUANTITY',
                      'HDB_EXT_SITE_CODE_ARCHIVE','HDB_EXT_DATA_CODE_ARCHIVE', 
                      'HDB_EXT_DATA_SOURCE_ARCHIVE','HDB_ATTR_FEATURE',
                      'HDB_DATATYPE_FEATURE')
and table_name not like '%_OLD'
UNION
select 'CREATE SNAPSHOT ' || table_name ||
       ' PCTFREE 10 PCTUSED 40 TABLESPACE HDB_DATA storage (initial 50k next 50k pctincrease 0) ' ||
       'refresh complete with rowid' ||
       ' AS SELECT * FROM  &3..' || table_name || '@&1;'
from dba_tables
where table_name in ('HDB_EXT_SITE_CODE_ARCHIVE','HDB_EXT_DATA_CODE_ARCHIVE', 
                     'HDB_EXT_DATA_SOURCE_ARCHIVE');


select ' ' from dual;
/* Needed as key for FK of datatype_feature, attr_feature */
select 'ALTER TABLE hdb_feature ADD ( CONSTRAINT hdb_feature_uk ' ||
       'UNIQUE (feature_class_id, feature_id) using index tablespace hdb_idx ' ||
       'storage (initial 50k next 50k pctincrease 0));' from dual;  


select ' ' from dual;
/* Create hdb_attr_feature and hdb_datatype_feature snapshots */
select 'CREATE SNAPSHOT ' || table_name ||
       ' PCTFREE 10 PCTUSED 40 TABLESPACE HDB_DATA storage (initial 50k next 50k pctincrease 0) ' ||
       ' AS SELECT * FROM  &3..' || table_name || '@&1;'
from dba_tables
where table_name in ('HDB_ATTR_FEATURE', 'HDB_DATATYPE_FEATURE');

select ' ' from dual;
-- Create public synonyms for HDB snapshots
select 'CREATE PUBLIC SYNONYM ' || table_name || ' FOR ' || table_name ||
       ';'
from dba_tables
where table_name like 'HDB_%'
and table_name not like '%_OLD'
and table_name not in ('HDB_DIMENSION','HDB_UNIT','HDB_PHYSICAL_QUANTITY')
order by table_name;

select ' ' from dual;
-- Grant select rights on the snapshots
select 'GRANT SELECT ON ' || table_name || ' TO PUBLIC' || ';'
from dba_tables
where table_name like 'HDB_%'
and table_name not like '%_OLD'
and table_name not in ('HDB_DIMENSION','HDB_UNIT','HDB_PHYSICAL_QUANTITY')
order by table_name;

select ' ' from dual;
--  Create synonyms for use by meta-data app; these refer to master tables
select 'CREATE PUBLIC SYNONYM ' || substr(table_name,1,26) || '_SYN FOR &3..' || table_name ||
       '@&1;'
from dba_tables
where table_name like 'HDB_%'
and table_name not like '%_OLD'
and table_name not in ('HDB_DIMENSION','HDB_UNIT','HDB_PHYSICAL_QUANTITY')
order by table_name;

select ' ' from dual;
-- Create synonym for procedure on master
select 'create public synonym grant_meta_data_user for &3..grant_meta_data_user@&1;'
from dual;

spool off;
set feedback on
@snap_ins_script
quit


