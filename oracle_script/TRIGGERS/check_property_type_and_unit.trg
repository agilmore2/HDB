create or replace trigger check_property_type_and_unit 
after insert or update
on hdb_property
for each row
begin
     if (:new.property_value_type <> 'number' and :new.unit_id is not null) then
	  raise_application_error(-20005,'Error: Unit_id appropriate only for property with type: number');
     end if;

     if (:new.property_value_type = 'number' and :new.unit_id is null) then
	  raise_application_error(-20005,'Error: Unit_id needs to be set for property with type: number');
     end if;

end;
/
show errors trigger check_property_type_and_unit;
/

