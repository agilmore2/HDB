create or replace procedure
          check_valid_unit_spec
              (is_factor             integer,
               factor                float,
               from_expression       varchar2,
               to_expression         varchar2,
               month_year            varchar2,
               over_month_year       varchar2)
IS
               check_val integer;
BEGIN
      if ((is_factor <> 0) and (is_factor <> 1)) then
         raise_application_error(-20001, 'Integrity Failure: Illegal value for is_factor = ' || is_factor);
      end if;
      if ((is_factor = 1) and (factor is null)) then
         raise_application_error(-20003, 'Integrity Failure: Null mult_factor when is_factor = 1');
      end if;
      if ((is_factor = 0) AND
          (from_expression is null OR
           to_expression   is null)) then
         raise_application_error(-20003, 'Integrity Failure: Null expression when is_factor = 0');
      end if;
      if( month_year      IS NOT NULL AND
          over_month_year IS NOT NULL) then
          raise_application_error(-20004, 'Integrity Failure: Month_year and over_month_year cannot both be set');
      end if;
      if ((month_year <> 'M') AND
          (month_year <> 'Y') AND
          (month_year <> 'W')) then
           raise_application_error(-20005, 'Integrity Failure: Month_year must have value of M, Y or W');
      end if;
      if ((over_month_year <> 'M') AND
          (month_year <> 'Y') AND
          (month_year <> 'W')) then
           raise_application_error(-20006, 'Integrity Failure: Over_month_year must have value of M, Y or W');
      end if;
end;
/
show errors;
/
grant execute on check_valid_unit_spec to public;
/
