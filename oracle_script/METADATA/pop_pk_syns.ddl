-- first create synonym for local version of pop_pk, then for master,
-- possibly remote version
CREATE PUBLIC SYNONYM populate_pk_ref FOR &1..populate_pk
/

CREATE PUBLIC SYNONYM populate_pk_hdb FOR &2..populate_pk@&3
/
quit;
