-- Oracle Consulting:  Gary Coy
-- THIS IS A SCRIPT THAT CREATES A SCRIPT
-- IT DOES NOT NEED TO BE RE-RUN

CREATE OR REPLACE PROCEDURE drop_pk_trigs IS

   CURSOR table_col IS
      SELECT table_name tab_name
      FROM user_constraints
      WHERE constraint_type = 'P'
      AND table_name IN
      ('HDB_AGEN'
       ,'HDB_ATTR'
       ,'HDB_COLLECTION_SYSTEM'
       ,'HDB_COMPUTED_DATATYPE'
       ,'HDB_DAMTYPE'
       ,'HDB_DATATYPE'
       ,'HDB_DATA_SOURCE'
       ,'HDB_DIMENSION'
       ,'HDB_GAGETYPE'
       ,'HDB_LOADING_APPLICATION'
       ,'HDB_METHOD'
       ,'HDB_METHOD_CLASS'
       ,'HDB_MODEL'
       ,'HDB_OBJECTTYPE'
       ,'HDB_RIVER'
       ,'HDB_SITE'
       ,'HDB_SITE_DATATYPE'
       ,'HDB_STATE'
       ,'HDB_UNIT'
       ,'HDB_USBR_OFF'
       ,'REF_AGG_DISAGG'
       ,'REF_MODEL_USER'
       ,'REF_DB_LIST')
      ORDER BY table_name;
BEGIN
   FOR one_trig IN table_col LOOP
      DBMS_OUTPUT.PUT_LINE('DROP TRIGGER '||one_trig.tab_name||'_PK_TRIG;');
   END LOOP;
END drop_pk_trigs;
/
