
CREATE OR REPLACE PROCEDURE model_is_coord
  (model_id_in IN number,
   is_coord OUT number) IS

text varchar2(1000);
BEGIN
  is_coord := 0;

  select 1
  into is_coord
  from hdb_model a
  where a.model_id = model_id_in
    and a.coordinated = 'Y';

  EXCEPTION
   WHEN NO_DATA_FOUND THEN
     text := 'no action; this is OK';
   WHEN OTHERS THEN
     text := 'ERROR: '||sqlcode||' '||substr(sqlerrm,1,100)||' when checking to see if model'||model_id_in||' is coordinated';
     deny_action (text);
END;

/
show errors;

grant execute on model_is_coord to public;
create public synonym model_is_coord for model_is_coord;
