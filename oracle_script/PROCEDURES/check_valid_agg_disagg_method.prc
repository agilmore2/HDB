create or replace procedure
 check_valid_agg_disagg_method (method_or_fcn varchar2, method_id number, function varchar2)
IS
BEGIN
    if ( method_or_fcn = 'M') then
       if ((method_id is null) or (function is not null)) then
          raise_application_error(-20001,'Error: method_or_function value = ' || method_or_fcn ||  '. Method_Id must be non-null; function must be null.');
        end if;

	/* Data point methods max, min, ave, sum, div, eop, copy */
       if (method_id not in (11, 12, 2, 17, 20, 7, 6)) then
          raise_application_error(-20002,'Error: method_id ' || method_id ||  ' not valid for aggDisagg.');
       end if;

   else
       if (method_or_fcn = 'F') then
          if ((method_id is not null) or (function is null)) then
             raise_application_error(-20003,'Error: operator_or_function value = ' || method_or_fcn || '. Method_Id must be null; function must be non-null.');
          end if;
       else
          if (method_or_fcn = 'N') then
             if ((method_id is not null) or (function is not null)) then
                raise_application_error(-20004,'Error: operator_or_function value = ' || method_or_fcn || '. Method_Id and function must be null');
             end if;
         else
             raise_application_error(-20005,'Error: operator_or_function value ' || method_or_fcn || ' is not in (M, F, N)');
         end if;
      end if;
   end if;
END;
/
show errors;
/
grant execute on  check_valid_agg_disagg_method to public;
/
