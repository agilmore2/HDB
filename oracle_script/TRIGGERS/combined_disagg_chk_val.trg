create or replace trigger combined_disagg_chk_val
after             insert or update
               of dest_datatype_unit_ind,
                  dest_datatype_or_unit_id,
                  dest_observation_interval,
                  source_observation_interval,
                  method_id,
                  agg_disagg_function_name,
                  method_or_function
on                ref_agg_disagg
for   each row
BEGIN
check_valid_agg_disagg_method
(:new.method_or_function,:new.method_id,:new.agg_disagg_function_name);
if (:new.method_or_function = 'N') then
     check_valid_noop
     (:new.source_observation_interval,:new.dest_observation_interval);
end if;
if (:new.dest_datatype_unit_ind = 'D') then
     check_valid_datatype
     (:new.dest_datatype_or_unit_id, :new.source_datatype_id);
end if;
if (:new.dest_datatype_unit_ind = 'U') then
     check_valid_unit(:new.dest_datatype_or_unit_id);
end if;
END;
/
show error trigger combined_disagg_chk_val;
/
