create or replace procedure check_valid_datatype(cur_datatype_id number,
						 source_datatype_id number)
IS
     check_val   number;
     check_val2  number;
begin
     select count(*) into check_val from hdb_datatype
            where  hdb_datatype.datatype_id = cur_datatype_id;
     if  SQL%NOTFOUND then
        raise_application_error(-20001,'Error: Error selecting from hdb_datatype with datatype_id = ' || cur_datatype_id);
     end if;
     if check_val < 1 then
        raise_application_error(-20002,'Integrity Failure: No datatype_id = ' || cur_datatype_id);
     end if;
     /* now check to see if dimension of source and destination datatype_ids
	match, or if one is flow and the other volume, or one is power
	and the other is energy. */
     select count(*) into check_val2
     from hdb_datatype a, hdb_datatype b, hdb_unit aa, hdb_unit bb, 
	hdb_dimension aaa, hdb_dimension bbb
     where a.datatype_id = source_datatype_id
       and a.unit_id = aa.unit_id
       and aa.dimension_id = aaa.dimension_id
       and b.datatype_id = cur_datatype_id
       and b.unit_id = bb.unit_id
       and bb.dimension_id = bbb.dimension_id
       and (aaa.dimension_id = bbb.dimension_id OR
	    (aaa.dimension_name in ('flow', 'volume') AND
	     bbb.dimension_name in ('flow', 'volume')) OR
	    (aaa.dimension_name in ('energy', 'power') AND
	     bbb.dimension_name in ('energy', 'power')));
     if  SQL%NOTFOUND then
        raise_application_error(-20001,'Error: Error selecting from hdb_datatype with datatype_id = ' || cur_datatype_id || ' or ' || source_datatype_id);
     end if;
     if check_val2 < 1 then
        raise_application_error(-20002,'Integrity Failure: Source and destination datatypes must have same dimension_id, or they must be flow and volume OR power and energy');
     end if;
end;
/
show errors;
/
grant execute on check_valid_datatype to public;
/
