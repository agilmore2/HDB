
create or replace procedure lookup_hydromet_sdi
                ( SITE_CODE IN VARCHAR2,
                  DATATYPE_PCODE IN VARCHAR2,
		  FILE_TYPE IN VARCHAR2,
                  SITE_DATATYPE_ID OUT NUMBER
                ) IS
/* now go get the sdi if its hydromet data  */
BEGIN
        IF SITE_CODE IS NULL THEN DENY_ACTION ( 'INVALID <NULL> SITE_CODE' );
	        ELSIF DATATYPE_PCODE IS NULL THEN DENY_ACTION ( 'INVALID <NULL> DATATYPE_PCODE' );
                ELSIF FILE_TYPE IS NULL THEN DENY_ACTION ( 'INVALID <NULL> FILE TYPE FOR HYDROMET SITE CODE / P-CODES' );
        END IF;

        SELECT site_datatype_id
	        INTO SITE_DATATYPE_ID
                FROM ref_hm_site_pcode
                WHERE hm_site_code = SITE_CODE
                AND hm_pcode = DATATYPE_PCODE
                AND hm_filetype = FILE_TYPE;
	EXCEPTION
                WHEN OTHERS THEN DENY_ACTION ( 'INVALID SITE CODE: ' || SITE_CODE || ' and/or P-CODE: ' || DATATYPE_PCODE || ' COMBINATION FOR HYDROMET DATA' );
END;

/

show errors;

grant execute on lookup_hydromet_sdi to public;
create public synonym lookup_hydromet_sdi for lookup_hydromet_sdi;

