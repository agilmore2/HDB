create or replace procedure check_interval_usage (cur_datatype_type varchar2, cur_method_class_id number, cur_method_source_datatype_id number, cur_compound_interval varchar2, cur_cumulative_interval varchar2)
IS
     is_instant  varchar2(1);
     method_class varchar2(64);
     method_class_type varchar2(32);
     source_datatype_type varchar2(16);
begin
     /* Select method_class_name and method_class_type for use in checks */
     select method_class_name into method_class from hdb_method_class 
     where method_class_id = cur_method_class_id;

     if method_class is null then
	  raise_application_error(-20005,'Error: Invalid method_class_id ' || cur_method_class_id);
     end if;

     select method_class_type into method_class_type from hdb_method_class 
     where method_class_id = cur_method_class_id;
   
     /* If type is simple or computed, method class must be N/A, and the 
        following must be NULL: method_source_datatype_id, compound_interval,
        cumulative_interval  */
     if lower (cur_datatype_type) in ('simple', 'computed', 'computed or simple') then

        if lower (method_class) <> 'n/a' then
	  raise_application_error(-20006,'Error: Method_class must be N/A when datatype_type is simple, computed, or computed or simple.');
        end if;

        if cur_method_source_datatype_id is not null then
	  raise_application_error(-20007,'Error: Method_source_datatype must be NULL when datatype_type is simple, computed, or computed or simple.');
        end if;

        if cur_cumulative_interval is not null or cur_compound_interval is not null then
	  raise_application_error(-20008,'Error: Cumulative_interval and compound_interval cannot be used when datatype_type is simple, computed, or computed or simple.');
        end if;
     end if; /* simple and computed*/


     /* If type is method-applied, method_class can't be N/A,
        method_source_datatype_id must be filled in, and
        compound or cumulative interval must be filled in if proper criteria
        are met */
     if lower (cur_datatype_type) in ('method-applied', 'computed or method-applied', 'method-applied or simple') then
        if lower (method_class) = 'n/a' then
	  raise_application_error(-20009,'Error: Method_class cannot be N/A when datatype_type is or includes method-applied.');
        end if;

        if cur_method_source_datatype_id is null then
	  raise_application_error(-20010,'Error: Method_source_datatype must be specified when datatype_type is or includes method-applied.');
        end if;

        /* Check type of method_source_datatype_id; if it is a method-applied
           datatype, and the current method is a time-aggregation method, then
           this is a compound datatype, and the compound_interval must be
           filled in. */
        for i in 1..(datatype_pkg.datatype_index) loop
             if (datatype_pkg.dt_id_tab(i) = cur_method_source_datatype_id) then
		     source_datatype_type := datatype_pkg.dt_type_tab(i);
	 	     exit; 
             end if;
        end loop;

        if lower (source_datatype_type) in ('method-applied', 'computed or method-applied', 'method-applied or simple') and lower (method_class_type) = 'time-aggregation' then
	  if cur_compound_interval is null then
		  raise_application_error(-20011,'Error: This is a compound datatype; you must fill in the compound_interval.');
          end if;
        else
	  if cur_compound_interval is not null then
		  raise_application_error(-20012,'Error: This is not a compound datatype; you cannot fill in the compound_interval.');
          end if;
        end if;

        if lower (method_class) = 'accumulation' then
	  if cur_cumulative_interval is null then
		  raise_application_error(-20011,'Error: This is a cumulative datatype; you must fill in the cumulative_interval.');
          end if;
        else
	  if cur_cumulative_interval is not null then
		  raise_application_error(-20012,'Error: This is not a cumulative datatype; you cannot fill in the cumulative_interval.');
          end if;
        end if;
     end if; /* method-applied */

end;
/
show errors;
/
grant execute on check_interval_usage to public;
/


