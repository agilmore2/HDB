CREATE OR REPLACE TRIGGER REF_AGG_DISAGG_PK_TRIG BEFORE INSERT OR UPDATE       
 ON REF_AGG_DISAGG                                                             
 FOR EACH ROW                                                                  
BEGIN                                                                          
   IF inserting THEN                                                           
      IF populate_pk.pkval_pre_populated = FALSE THEN                          
         :new.AGG_DISAGG_ID := populate_pk.get_pk_val( 'REF_AGG_DISAGG', FALSE 
);                                                                              
      END IF;                                                                  
   ELSIF updating THEN                                                         
      :new.AGG_DISAGG_ID := :old.AGG_DISAGG_ID;                                
   END IF;                                                                     
END;                                                                           
/                                                                              
                                                                               
CREATE OR REPLACE TRIGGER REF_DB_LIST_PK_TRIG BEFORE INSERT OR UPDATE          
 ON REF_DB_LIST                                                                
 FOR EACH ROW                                                                  
BEGIN                                                                          
   IF inserting THEN                                                           
      IF populate_pk.pkval_pre_populated = FALSE THEN                          
         :new.SESSION_NO := populate_pk.get_pk_val( 'REF_DB_LIST', FALSE );    
      END IF;                                                                  
   ELSIF updating THEN                                                         
      :new.SESSION_NO := :old.SESSION_NO;                                      
   END IF;                                                                     
END;                                                                           
/                                                                              
                                                                               
CREATE OR REPLACE TRIGGER REF_MODEL_USER_PK_TRIG BEFORE INSERT OR UPDATE       
 ON REF_MODEL_USER                                                             
 FOR EACH ROW                                                                  
BEGIN                                                                          
   IF inserting THEN                                                           
      IF populate_pk.pkval_pre_populated = FALSE THEN                          
         :new.USER_ID := populate_pk.get_pk_val( 'REF_MODEL_USER', FALSE );    
      END IF;                                                                  
   ELSIF updating THEN                                                         
      :new.USER_ID := :old.USER_ID;                                            
   END IF;                                                                     
END;                                                                           
/                                                                              




