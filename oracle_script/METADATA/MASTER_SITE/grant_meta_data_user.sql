CREATE OR REPLACE PROCEDURE grant_meta_data_user (table_name IN VARCHAR,
  do_grant IN BOOLEAN) IS
BEGIN
   IF do_grant THEN
     EXECUTE IMMEDIATE 'grant insert, update, delete on ' || table_name || ' to meta_data_user';
   ELSE
     EXECUTE IMMEDIATE 'revoke insert, update, delete on ' || table_name || '
from meta_data_user';
   END IF;
  
   commit;
END; 
/

grant execute on grant_meta_data_user to meta_data_user;

drop public synonym grant_meta_data_user;
create public synonym grant_meta_data_user for grant_meta_data_user;
