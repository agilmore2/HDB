create or replace PROCEDURE modify_r_base_raw ( SITE_DATATYPE_ID NUMBER,
			      INTERVAL VARCHAR2,
			      START_DATE_TIME DATE,
			      END_DATE_TIME IN OUT DATE,
			      VALUE FLOAT,
                              AGEN_ID NUMBER,
			      OVERWRITE_FLAG VARCHAR2,
			      VALIDATION CHAR,
                              COLLECTION_SYSTEM_ID NUMBER,
                              LOADING_APPLICATION_ID NUMBER,
                              METHOD_ID NUMBER,
                              COMPUTATION_ID NUMBER,
			      DO_UPDATE_Y_OR_N VARCHAR2 ) IS
    TEMP_SDI R_BASE.SITE_DATATYPE_ID%TYPE;
    TEMP_INT R_BASE.INTERVAL%TYPE;
    TEMP_SDT R_BASE.START_DATE_TIME%TYPE;
    ROWCOUNT NUMBER;
    END_DATE_TIME_NEW DATE;
BEGIN
    /*  First check for any null field that where passed  */
    IF SITE_DATATYPE_ID IS NULL THEN DENY_ACTION ( 'INVALID <NULL> SITE_DATATYPE_ID' );
	ELSIF INTERVAL IS NULL THEN DENY_ACTION ( 'INVALID <NULL> INTERVAL' );
	ELSIF START_DATE_TIME IS NULL THEN DENY_ACTION ( 'INVALID <NULL> START_DATE_TIME' );
	ELSIF VALUE IS NULL THEN DENY_ACTION ( 'INVALID <NULL> VALUE' );
	ELSIF AGEN_ID IS NULL THEN DENY_ACTION ( 'INVALID <NULL> AGEN_ID' );
	ELSIF COLLECTION_SYSTEM_ID IS NULL THEN DENY_ACTION ( 'INVALID <NULL> COLLECTION_SYSTEM_ID' );
	ELSIF LOADING_APPLICATION_ID IS NULL THEN DENY_ACTION ( 'INVALID <NULL> LOADING_APPLICATION_ID' );
	ELSIF METHOD_ID IS NULL THEN DENY_ACTION ( 'INVALID <NULL> METHOD_ID' );
	ELSIF COMPUTATION_ID IS NULL THEN DENY_ACTION ( 'INVALID <NULL> COMPUTATION_ID' );
    END IF;

    /*  if user did not pass an end_date time, set the end date TIME based on START_DATE_TIME AND THE INTERVAL */
    END_DATE_TIME_NEW := END_DATE_TIME;
    IF END_DATE_TIME IS NULL THEN
	IF INTERVAL = 'hour' THEN
	    END_DATE_TIME_NEW := START_DATE_TIME + 1 / 24;
	    ELSIF INTERVAL = 'instant' THEN END_DATE_TIME_NEW := START_DATE_TIME;
	    ELSIF INTERVAL = 'day' THEN END_DATE_TIME_NEW := START_DATE_TIME + 1;
	    ELSIF INTERVAL = 'month' THEN END_DATE_TIME_NEW := ADD_MONTHS ( START_DATE_TIME,
									    1 );
	    ELSIF INTERVAL = 'year' OR INTERVAL = 'wy' THEN END_DATE_TIME_NEW := ADD_MONTHS ( START_DATE_TIME,
											      12 );
	ELSE
	    DENY_ACTION ( INTERVAL || ' IS AN INVALID INTERVAL WITH A NULL END DATE TIME.' );
	END IF;

        END_DATE_TIME := END_DATE_TIME_NEW;
    END IF;

    /*  go see if a record already exists ; if not do an insert otherwise do an update as long as do_update <> 'N'  */
    TEMP_SDI := SITE_DATATYPE_ID;
    TEMP_INT := INTERVAL;
    TEMP_SDT := START_DATE_TIME;
    SELECT count ( * )
      INTO rowcount
      FROM r_base
     WHERE site_datatype_id = TEMP_SDI
       AND INTERVAL = TEMP_INT
       AND start_date_time = TEMP_SDT
       and end_date_time = end_date_time_new;
    IF rowcount = 0 THEN
	/* insert the data into the database  */
	INSERT_R_BASE ( SITE_DATATYPE_ID,
			INTERVAL,
			START_DATE_TIME,
			END_DATE_TIME_NEW,
			VALUE,
			AGEN_ID,
			OVERWRITE_FLAG,
			VALIDATION,
			COLLECTION_SYSTEM_ID,
			LOADING_APPLICATION_ID,
			METHOD_ID,
			COMPUTATION_ID );
  /*  update the data into the database, if desired */
	ELSIF rowcount > 1 THEN
           DENY_ACTION ( 'RECORD with SDI: ' || to_char ( SITE_DATATYPE_ID ) ||
           ' INTERVAL: ' || INTERVAL || ' START_DATE_TIME: ' || to_char ( start_date_time,
           'dd-MON-yyyy HH24:MI:SS' ) || ' END_DATE_TIME: '|| to_char ( end_date_time_new,
           'dd-MON-yyyy HH24:MI:SS' ) || ' HAS MULTIPLE ENTRIES. DANGER! DANGER! DANGER!.' );
        ELSIF UPPER ( NVL ( DO_UPDATE_Y_OR_N,           
			    'Y' ) ) = 'Y' THEN UPDATE_R_BASE_RAW ( SITE_DATATYPE_ID,
							       INTERVAL,
							       START_DATE_TIME,
							       END_DATE_TIME_NEW,
							       VALUE,
							       AGEN_ID,
							       OVERWRITE_FLAG,
							       VALIDATION,
							       COLLECTION_SYSTEM_ID,
							       LOADING_APPLICATION_ID,
							       METHOD_ID,
							       COMPUTATION_ID );
    END IF;

END;
/


show errors;
/
create public synonym modify_r_base_raw for modify_r_base_raw;
grant execute on modify_r_base_raw to app_role;

