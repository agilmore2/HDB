create or replace procedure deny_action (text varchar2)
IS
              check_val integer;
BEGIN
      raise_application_error(-20001, '"' || text || '"');
END;
/
show errors;
/
grant execute on deny_action to public;
/
