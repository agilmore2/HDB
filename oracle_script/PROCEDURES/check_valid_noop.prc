create or replace procedure
     check_valid_noop (source_observation number, dest_observation number)
IS
         msg                      varchar2(200);
         cur_source_observation   number;
         cur_dest_observation     number;
BEGIN
         cur_source_observation := source_observation;
         cur_dest_observation   := dest_observation;
         if (source_observation <> dest_observation) then
             raise_application_error(-20001,'Error: When no operator, source and  destination intervals must be equal');
         end if;
END;
/
show errors;
/
grant execute on check_valid_noop to public;
/
