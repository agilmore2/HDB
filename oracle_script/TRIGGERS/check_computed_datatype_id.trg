create or replace trigger check_computed_datatype_id 
after insert or update of datatype_id
on hdb_computed_datatype
for each row
begin
     /* 1 and 2 are only computations which can go without a datatype_id */
     if (:new.datatype_id is null and :new.computation_id > 2) then
	  raise_application_error(-20005,'Error: Datatype_id must be filled in');
     end if;

end;
/
show errors trigger check_computed_datatype_id;
/

