create or replace trigger model_run_id_update
after             insert
on                ref_model_run
begin
	if not (DBMS_SESSION.Is_Role_Enabled ('MODEL_ROLE')) then
           update_new_modelrun_id;
        end if;
end;
/
show errors trigger model_run_id_update;
/
