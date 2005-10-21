PROMPT CREATE OR REPLACE PROCEDURE uchdba.modify_r_base

CREATE OR REPLACE PROCEDURE uchdba.modify_r_base ( SITE_DATATYPE_ID NUMBER,
			      INTERVAL VARCHAR2,
			      START_DATE_TIME DATE,
			      END_DATE_TIME DATE,
			      VALUE FLOAT,
                              AGEN_ID NUMBER,
			      OVERWRITE_FLAG VARCHAR2,
			      VALIDATION CHAR,
                              COLLECTION_SYSTEM_ID NUMBER,
                              LOADING_APPLICATION_ID NUMBER,
                              METHOD_ID NUMBER,
                              COMPUTATION_ID NUMBER,
			      DO_UPDATE_Y_OR_N VARCHAR2 ) IS
 END_DATE_TIME_NEW DATE;
BEGIN
    /*  set the end_date_time variable to the value passed in, this gets around the issue if you pass in a null */
    END_DATE_TIME_NEW := END_DATE_TIME;

    MODIFY_R_BASE_RAW ( SITE_DATATYPE_ID,
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
			COMPUTATION_ID,
			DO_UPDATE_Y_OR_N );

END;
/