create or replace trigger datatype_aft_row_all  
after insert or update 
               of datatype_id,
                  datatype_type,
                  method_source_datatype_id,
                  method_class_id,
                  compound_interval,
                  cumulative_interval
on                hdb_datatype
for each row  
begin  
check_interval_usage (:new.datatype_type, :new.method_class_id, 
                      :new.method_source_datatype_id, 
		      :new.compound_interval, :new.cumulative_interval);
check_source_data_codes (:new.method_source_datatype_id, :new.method_class_id);
end;  
/
show error trigger datatype_aft_row_all
/
