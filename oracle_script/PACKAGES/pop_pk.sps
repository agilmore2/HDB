CREATE OR REPLACE PACKAGE populate_pk IS
   pkval_pre_populated	BOOLEAN := FALSE;
   FUNCTION get_pk_val ( table_name IN  VARCHAR2, set_pkval IN BOOLEAN ) RETURN number;
END;
/

GRANT EXECUTE ON populate_pk TO PUBLIC
/
