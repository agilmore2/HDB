
create or replace procedure lookup_application
			   (  AGEN_NAME IN VARCHAR2,
			      COLLECTION_SYSTEM_NAME IN VARCHAR2,
			      LOADING_APPLICATION_NAME IN VARCHAR2,
			      METHOD_NAME IN VARCHAR2,
			      COMPUTATION_NAME IN VARCHAR2,
                              AGEN_ID IN OUT NUMBER,
                              COLLECTION_SYSTEM_ID IN OUT NUMBER,
                              LOADING_APPLICATION_ID IN OUT NUMBER,
                              METHOD_ID IN OUT NUMBER,
                              COMPUTATION_ID IN OUT NUMBER
                           ) IS

    TEMP_NAME VARCHAR2 ( 64 );
BEGIN
        /* First check all inputs */
    IF AGEN_NAME IS NULL THEN DENY_ACTION ( 'INVALID <NULL> AGEN_NAME' );
	ELSIF COLLECTION_SYSTEM_NAME IS NULL THEN DENY_ACTION ( 'INVALID <NULL> COLLECTION_SYSTEM_NAME' );
	ELSIF LOADING_APPLICATION_NAME IS NULL THEN DENY_ACTION ( 'INVALID <NULL> LOADING_APPLICATION_NAME' );
	ELSIF METHOD_NAME IS NULL THEN DENY_ACTION ( 'INVALID <NULL> METHOD_NAME' );
	ELSIF COMPUTATION_NAME IS NULL THEN DENY_ACTION ( 'INVALID <NULL> COMPUTATION_NAME' );
    END IF;

    BEGIN
	temp_name := AGEN_NAME;
	SELECT agen_id
	  INTO AGEN_ID
	  FROM hdb_agen
	 WHERE agen_name = temp_name;
	EXCEPTION
	    WHEN OTHERS THEN DENY_ACTION ( 'INVALID AGENCY NAME:' || AGEN_NAME );
    END;

    /* now go get the collection_system_id  */
    BEGIN
	temp_name := collection_system_name;
	SELECT collection_system_ID
	  INTO COLLECTION_SYSTEM_ID
	  FROM hdb_collection_system
	 WHERE collection_system_name = TEMP_NAME;
	EXCEPTION
	    WHEN OTHERS THEN DENY_ACTION ( 'INVALID COLLECTION SYSTEM NAME:' || COLLECTION_SYSTEM_NAME );
    END;

    /* now go get the loading_application_id  */
    BEGIN
	temp_name := LOADING_APPLICATION_NAME;
	SELECT loading_application_id
	  INTO LOADING_APPLICATION_ID
	  FROM hdb_loading_application
	 WHERE loading_application_name = TEMP_NAME;
	EXCEPTION
	    WHEN OTHERS THEN DENY_ACTION ( 'INVALID LOADING APPLICATION NAME:' || LOADING_APPLICATION_NAME );
    END;

    /* now go get the method_id  */
    BEGIN
	temp_name := METHOD_NAME;
	SELECT method_id
	  INTO METHOD_ID
	  FROM hdb_method
	 WHERE method_name = TEMP_NAME;
	EXCEPTION
	    WHEN OTHERS THEN DENY_ACTION ( 'INVALID METHOD NAME:' || METHOD_NAME );
    END;

    /* now go get the computation_id  */
    BEGIN
	temp_name := COMPUTATION_NAME;
	SELECT computation_id
	  INTO COMPUTATION_ID
	  FROM hdb_computed_datatype
	 WHERE computation_name = temp_name;
	EXCEPTION
	    WHEN OTHERS THEN DENY_ACTION ( 'INVALID COMPUTATION NAME:' || COMPUTATION_NAME );
    END;
END;

/

show errors;

grant execute on lookup_application to public;
create public synonym lookup_application for lookup_application;
