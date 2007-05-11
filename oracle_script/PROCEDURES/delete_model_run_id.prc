/* Will delete specified model_run_id and any associated key-value
   pairs. */


create or replace PROCEDURE delete_model_run_id ( 
   model_run_id_in              IN number)
IS
  text varchar2(1000);
BEGIN
  /*  First check for inappropriate NULL values */
  if (model_run_id_in is null) then
    deny_action ( 'Invalid <NULL> model_run_id');
  end if;

  delete from ref_model_run_keyval
  where model_run_id = model_run_id_in;

  delete from ref_model_run
  where model_run_id = model_run_id_in;

  commit;

  EXCEPTION
   WHEN OTHERS THEN 
     rollback;
     text := sqlerrm||' when trying to delete MRI '||model_run_id_in;
     deny_action (text);

end;
/

show errors;
/
create public synonym delete_model_run_id for delete_model_run_id;
grant execute on delete_model_run_id to model_priv_role;


