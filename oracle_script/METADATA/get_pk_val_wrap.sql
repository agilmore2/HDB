CREATE OR REPLACE FUNCTION get_pk_val_wrap ( table_name IN  VARCHAR2, set_pkval IN BOOLEAN ) RETURN number IS

	new_pk_val number(11) := NULL;

 BEGIN
   IF table_name LIKE 'HDB%' THEN
    	new_pk_val := populate_pk_hdb.get_pk_val (table_name, set_pkval);
   ELSE
        new_pk_val := populate_pk_ref.get_pk_val (table_name, set_pkval);
   END IF; 

   return new_pk_val;
 END get_pk_val_wrap;
/

grant execute on get_pk_val_wrap to public;

drop public synonym get_pk_val_wrap;
create public synonym get_pk_val_wrap for get_pk_val_wrap;

