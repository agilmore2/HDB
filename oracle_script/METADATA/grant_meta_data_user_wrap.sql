CREATE OR REPLACE PROCEDURE	grant_meta_data_user_wrap (table_name IN VARCHAR2,
  do_grant IN BOOLEAN)
AS
BEGIN
  grant_meta_data_user (table_name, do_grant);
  commit;
END;
/

grant execute on grant_meta_data_user_wrap to public;

drop public synonym grant_meta_data_user_wrap;
create public synonym grant_meta_data_user_wrap for grant_meta_data_user_wrap;

