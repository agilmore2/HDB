CREATE OR REPLACE PROCEDURE refresh_HDB_snap  (which_table IN varchar2)
   IS
      the_db_name    VARCHAR2(30) := NULL;
      my_mes         VARCHAR2(2000);

      PROCEDURE refresh_snapshot_table(the_table  IN   VARCHAR)
      IS
         -- Oracle Consulting:  Gary Coy, 7-JAN-1999
         -- This will get the primary key against the
         -- snapshot being refreshed.  
         CURSOR the_table_constraints IS
             SELECT constraint_name, constraint_type, owner
             FROM all_constraints
             WHERE table_name = upper(the_table)
             AND constraint_type = 'P';

         CURSOR fk_constraints ( pk_name VARCHAR) IS
             SELECT constraint_name, table_name
             FROM all_constraints
             WHERE r_constraint_name =  pk_name;

      alter_stmt      VARCHAR2(300) := NULL;
      curs_handle     INTEGER := NULL;
      curs_ret_val    INTEGER;
	  table_owner     VARCHAR2(100);

      BEGIN
         -- disable all constraints on "the_table"

         FOR a_constraint IN the_table_constraints LOOP

		 -- Set the constraint owner, assumed to be the table owner
		 -- Icky to do this once for every constraint, as it should 
		 -- be the same owner for all constraints on the table... but
		 -- also probably more efficient than opening the cursor again
		 table_owner := a_constraint.owner;

               FOR single_fk IN fk_constraints(a_constraint.constraint_name) LOOP
                  alter_stmt := 'ALTER TABLE '||single_fk.table_name||
                                     ' DISABLE CONSTRAINT '||single_fk.constraint_name;


                  curs_handle := DBMS_SQL.OPEN_CURSOR;


                  DBMS_SQL.PARSE( curs_handle, alter_stmt, DBMS_SQL.NATIVE );


                  curs_ret_val := DBMS_SQL.EXECUTE( curs_handle );


                  DBMS_SQL.CLOSE_CURSOR( curs_handle );


               END LOOP;
         END LOOP;



         DBMS_SNAPSHOT.REFRESH(table_owner||'.'||the_table, 'c');



         -- re-enable all the constraints on "the_table"
         FOR a_constraint IN the_table_constraints LOOP
               FOR single_fk IN fk_constraints(a_constraint.constraint_name) LOOP
                  alter_stmt := 'ALTER TABLE '||single_fk.table_name||
                                     ' ENABLE CONSTRAINT '||single_fk.constraint_name;
                  curs_handle := DBMS_SQL.OPEN_CURSOR;
                  DBMS_SQL.PARSE( curs_handle, alter_stmt, DBMS_SQL.NATIVE );
                  curs_ret_val := DBMS_SQL.EXECUTE( curs_handle );
                  DBMS_SQL.CLOSE_CURSOR( curs_handle );
               END LOOP;
         END LOOP;



      END;
   BEGIN
      BEGIN

	   DELETE FROM ref_refresh_monitor; 

         SELECT substr (value, 1, instr(value,'hdb',1,1) + 3)
         INTO the_db_name
         FROM v$parameter
         WHERE name = 'db_name';

         -- Oracle Consulting:  Gary Coy 29-OCT-1998
         -- This effectively 'hardcodes' the master HDB site
         -- to be Upper Colorado HDB in Salt Lake City
         -- This could be handled better, but for now, I think
         -- the odds of the master site moving are pretty slim.
         -- NOTE that the order of these refreshes IS important.
         -- There are foreign key dependencies that could fail if
         -- order is wrong.


		 -- Check to see if only one table is to be refreshed, or ALL
		    if upper(which_table) = 'ALL' THEN
               refresh_snapshot_table('HDB_AGEN');
               refresh_snapshot_table('HDB_DATATYPE_TYPE');
               refresh_snapshot_table('HDB_METHOD_CLASS_TYPE');
               refresh_snapshot_table('HDB_METHOD_CLASS'); 
               refresh_snapshot_table('HDB_OPERATOR'); 
               refresh_snapshot_table('HDB_METHOD');
               refresh_snapshot_table('HDB_DATE_TIME_UNIT');
               refresh_snapshot_table('HDB_INTERVAL');
     	       refresh_snapshot_table('HDB_OBJECTTYPE');
               refresh_snapshot_table('HDB_DIMENSION');
               refresh_snapshot_table('HDB_UNIT');
               refresh_snapshot_table('HDB_ATTR');
               refresh_snapshot_table('HDB_SITE');
               refresh_snapshot_table('HDB_DATATYPE');
               refresh_snapshot_table('HDB_COMPUTED_DATATYPE');
               refresh_snapshot_table('HDB_DATA_SOURCE');
               refresh_snapshot_table('HDB_DIVTYPE');
               refresh_snapshot_table('HDB_GAGETYPE');
               refresh_snapshot_table('HDB_MODELTYPE');
               refresh_snapshot_table('HDB_RIVER');
               refresh_snapshot_table('HDB_STATE');
               refresh_snapshot_table('HDB_SITE_DATATYPE');
               refresh_snapshot_table('HDB_VALIDATION');
               refresh_snapshot_table('HDB_USBR_OFF');
               refresh_snapshot_table('HDB_RIVER_REACH');
               refresh_snapshot_table('HDB_MODEL');
               refresh_snapshot_table('HDB_DMI_UNIT_MAP');
               refresh_snapshot_table('HDB_DAMTYPE');
               refresh_snapshot_table('HDB_DERIVATION_FLAG');
               refresh_snapshot_table('HDB_OVERWRITE_FLAG');
               refresh_snapshot_table('HDB_COLLECTION_SYSTEM');
               refresh_snapshot_table('HDB_LOADING_APPLICATION');
			else
               refresh_snapshot_table(upper(which_table));
            end if;

			-- Write message to table which is monitored by refresh
            -- trigger in forms app
	        my_mes := 'Refresh of ' || which_table || ' snapshot(s) completed successfully at ' || upper(the_db_name);
			INSERT INTO ref_refresh_monitor values (my_mes, 0);

      EXCEPTION
         WHEN others THEN
            my_mes := sqlerrm;
		INSERT INTO ref_refresh_monitor values (upper (the_db_name) || ': ' || my_mes, 1);
      END;
   END refresh_HDB_snap;
/


drop public synonym refresh_hdb_snap;

-- below for snapshot site
 create public synonym refresh_hdb_snap for refresh_hdb_snap;

grant execute on refresh_hdb_snap to public;
