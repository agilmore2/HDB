/* use this script to drop all objects in the DBA's schema if 
   it is necessary to re-run the hdb schema creation scripts
   and no data in the schema need be preserved. Note comment
   on last line. */
drop user &1 cascade;
drop user app_user;
drop user psswd_user cascade;
@ ./SCHEMA/BASE_SCRIPTS/drop_role.ddl
@ ./dropDBASyns.sql &1
@ ./dropSynSyns.sql
/* Remember to drop public database link */
