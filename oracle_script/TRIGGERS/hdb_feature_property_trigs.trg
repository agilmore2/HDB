-- install the trigger to:
-- check property value type
--

create or replace trigger hdb_feature_property_chk_val
before             insert or update
on                 hdb_feature_property
for   each row
begin

     check_valid_property_value(:new.property_id, :new.value, :new.string_value, :new.date_value);
end;
/
show errors trigger hdb_feature_property_chk_val;
/


