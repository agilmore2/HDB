CREATE OR REPLACE PACKAGE populate_pk IS
   pkval_pre_populated	BOOLEAN := FALSE;
   FUNCTION get_pk_val ( table_name IN  VARCHAR2, set_pkval IN BOOLEAN ) RETURN number;
END;
/

-- first create synonym for local version of pop_pk, then for master,
-- possibly remote version
CREATE PUBLIC SYNONYM populate_pk_ref FOR &1..populate_pk
/

CREATE PUBLIC SYNONYM populate_pk_hdb FOR &2..populate_pk@&3
/

GRANT EXECUTE ON populate_pk TO PUBLIC
/
quit;
