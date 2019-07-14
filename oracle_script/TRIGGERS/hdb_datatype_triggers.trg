CREATE OR REPLACE TRIGGER HDB_DATATYPE_PK_TRIG 
BEFORE INSERT OR UPDATE 
ON HDB_DATATYPE 
FOR EACH ROW 
BEGIN 
	IF inserting THEN 
	   IF populate_pk.pkval_pre_populated = FALSE THEN 
	      :new.DATATYPE_ID := populate_pk.get_pk_val( 'HDB_DATATYPE', FALSE );  
	   END IF;
       /* modified by M. Bogner, Sutron Corporation on 17-June_2011 to automatically 
          insert any new datatypes into the decodes datatype table to keep 
          datatype tables concurrent
       */   
       
       /* insert record into decodes.datatype table if record does not exist  */
       insert into datatype
       select :new.datatype_id,'HDB',:new.datatype_id from dual 
       minus select id,standard,id from datatype where standard = 'HDB';

    ELSIF updating THEN 
     :new.DATATYPE_ID := :old.DATATYPE_ID; 
    END IF; 
END;         
                                                                                
.
/
