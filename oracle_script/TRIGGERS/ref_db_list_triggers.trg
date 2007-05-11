-- install the trigger to ensure that, if min or max 
-- coord_model_run_id is non-null, the other also
-- is, and vice-versa.

create or replace trigger ref_db_list_before_ins_upd
before insert or update
on ref_db_list
for each row
declare
  v_coord_count         number;
  v_date		date;
  v_where		varchar2(10);
  v_mri_is_coord        number;
  v_model_id            number;
  v_valid_coord_db      number;
  v_installation_type   varchar2(32);
  new_model_run_id      number;
  text                  varchar2(1000);
begin

  if ((:new.min_coord_model_run_id is null and 
       :new.max_coord_model_run_id is not null) OR 
      (:new.min_coord_model_run_id is not null and 
       :new.max_coord_model_run_id is null)) then
    deny_action ('Min and max_coord_model_run_id must both be either null or non-null for a given database.');
  end if;

  if (:new.min_coord_model_run_id is not null and
      (:new.min_coord_model_run_id >= :new.max_coord_model_run_id)) then
    deny_action ('Min_coord_model_run_id must be less than max.');
  end if;

  EXCEPTION
    WHEN others THEN
      if (v_where <> 'other') then
        text := 'Problem on '||v_where||' of coord run. '||sqlerrm;
      else
        text := sqlerrm;
      end if;

      deny_action (text);
  
end;
/
show errors trigger ref_db_list_before_ins_upd;
/

