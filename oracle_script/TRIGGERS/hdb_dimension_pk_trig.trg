create or replace trigger hdb_dimension_pk_trig 
before insert or update 
on hdb_dimension
for each row 
begin
IF inserting THEN 
  IF populate_pk.pkval_pre_populated = FALSE THEN 
     :new.DIMENSION_ID := populate_pk.get_pk_val( 'HDB_DIMENSION', FALSE );  
  END IF; 
ELSIF updating THEN 
  :new.DIMENSION_ID := :old.DIMENSION_ID; 
END IF; 
end;
/
show errors trigger hdb_dimension_pk_trig;
/
