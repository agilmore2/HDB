create or replace trigger hdb_unit_pk_trig 
before insert or update 
on hdb_unit
for each row 
begin
IF inserting THEN 
  IF populate_pk.pkval_pre_populated = FALSE THEN 
     :new.UNIT_ID := populate_pk.get_pk_val( 'HDB_UNIT', FALSE );  
  END IF; 
ELSIF updating THEN 
  :new.UNIT_ID := :old.UNIT_ID; 
END IF; 
end;
/
show errors trigger hdb_unit_pk_trig;
/
