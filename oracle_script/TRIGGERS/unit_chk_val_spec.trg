create or replace trigger unit_chk_val_spec
after             insert or update of unit_id
on                hdb_unit
for   each row
begin
     check_valid_unit_spec
 (:new.is_factor, :new.mult_factor, :new.from_stored_expression, :new.to_stored_expression, :new.month_year, :new.over_month_year);
end;
/
show errors trigger unit_chk_val_spec;
/
