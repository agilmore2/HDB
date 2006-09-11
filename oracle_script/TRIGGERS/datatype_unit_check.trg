create or replace trigger datatype_unit_check
after insert or update of unit_id
on hdb_datatype
for each row
declare
  v_count  number;
  status   number;
  buffer   varchar2(2000);
  unit_mismatch EXCEPTION;
begin
     /* See if this datatype's unit is same as its physical quantity
	customary unit. Warn only if it's not; don't stop the edit. */
     select count(*) 
     into v_count
     from hdb_physical_quantity pq
     where pq.physical_quantity_name = :new.physical_quantity_name
       and pq.customary_unit_id = :new.unit_id;

     if (v_count = 0) then
       dbms_output.put_line ('WARNING: Datatype unit_id does not match its physical quantity customary_unit_id. Allowing edit.'); 
     end if;

end;
/
show errors trigger datatype_unit_check.trg;
/

