
create or replace procedure lookup_sdi
                ( SITE_NAME IN VARCHAR2,
                  DATATYPE_NAME IN VARCHAR2,
                  SITE_DATATYPE_ID OUT NUMBER
                ) IS
BEGIN
        IF SITE_NAME IS NULL THEN DENY_ACTION ( 'INVALID <NULL> SITE_NAME' );
	        ELSIF DATATYPE_NAME IS NULL THEN DENY_ACTION ( 'INVALID <NULL> DATATYPE_NAME' );
        END IF;
	
	SELECT site_datatype_id
	      INTO SITE_DATATYPE_ID
	      FROM hdb_site_datatype hsd,
		   hdb_site hs,
		   hdb_datatype hd
	     WHERE hsd.site_id = hs.site_id
	       AND hd.datatype_id = hsd.datatype_id
	       AND hs.site_name = SITE_NAME
	       AND hd.datatype_name = DATATYPE_NAME;
	EXCEPTION
		WHEN OTHERS THEN DENY_ACTION ( 'INVALID SITE NAME: ' || SITE_NAME || ' and/or DATATYPE NAME: ' || DATATYPE_NAME || ' COMBINATION' );
END;

/
show errors;

grant execute on lookup_sdi to public;
create public synonym lookup_sdi for lookup_sdi;
