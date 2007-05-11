/* Will touch the indicated model_run_id, updating only its
   date_time_loaded and user_name. Coordinated model_run_id
   records are also touched; date_time_loaded and user_name
   are set to the same values as those on the local database. */

create or replace PROCEDURE touch_model_run_id ( 
   model_run_id_in              IN number)
IS
  v_count number;

  upd_stmt varchar2(2000);

  e_no_match  exception;
  PRAGMA EXCEPTION_INIT(e_no_match, -20102);

  text varchar2(1000);
BEGIN
  /*  First check for inappropriate NULL values */
  if (model_run_id_in is null) then
    deny_action ( 'Invalid <NULL> model_run_id');
  end if;

  /* Before going any further, make sure this model_run_id
     exists. If not, send back an error message. */
  select count(*)
  into v_count
  from ref_model_run
  where model_run_id = model_run_id_in;

  if (v_count = 0) then
    text := 'WARNING! There is no model_run_id = '||model_run_id_in||'. No touch processed.';
    raise_application_error (-20102, text);
  end if;

  /* Process a no-action update statement to cause table-level triggers to
     re-set date_time_loaded and user_name. Do not update ref_model_run_keyval;
     its date_time_loaded does not need to match that in ref_model_run. */
  update ref_model_run 
  set extra_keys_y_n = extra_keys_y_n
  where model_run_id = model_run_id_in;

  /* Got through all updates, local and remote; OK to commit */
  commit;

  EXCEPTION
   WHEN e_no_match THEN
     deny_action (text);
   WHEN OTHERS THEN 
     rollback;
     text := sqlerrm||' when trying to touch MRI '||model_run_id_in;
     deny_action (text);

end;
/

show errors;
/
create public synonym touch_model_run_id for touch_model_run_id;
grant execute on touch_model_run_id to model_priv_role;


