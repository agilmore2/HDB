create or replace procedure check_source_data_codes(cur_source_datatype_id IN number, cur_method_class_id IN number)
IS
     check_val   number;
     method_class  varchar2(64);
begin
     select method_class_name into method_class from hdb_method_class
       where hdb_method_class.method_class_id = cur_method_class_id;

     check_val := 0;
     
     if (lower (substr (method_class, 1, 3)) <> 'n/a') then
        for i in 1..(datatype_pkg.datatype_index) loop
           if (datatype_pkg.dt_id_tab(i) = cur_source_datatype_id) then
                   check_val := 1; 
		   exit; 
           end if;
        end loop;

        if check_val < 1 then
          raise_application_error(-20002,'Integrity Failure: Method_source_datatype_id ' || cur_source_datatype_id || ' must be present as datatype_id in another row.');
        end if;
      end if;
end;
/
show errors;
/
grant execute on check_source_data_codes to public;
/



