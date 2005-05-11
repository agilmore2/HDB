CREATE OR REPLACE PROCEDURE delete_r_base ( SITE_DATATYPE_ID_IN NUMBER,                       
			  INTERVAL_IN VARCHAR2,                                                      
			  START_DATE_TIME_IN DATE,                                                   
			  END_DATE_TIME_IN DATE,                                                     
			  AGEN_ID_IN NUMBER,                                                         
			  LOADING_APPLICATION_ID_IN NUMBER)                                                 
IS                                                                              
manual_edit CHAR := 'N';                                                        
old_manual_edit CHAR := 'N';                                                    
old_agen_id NUMBER;   
old_loading_application_id NUMBER;
old_priority number := 0;                                                       
new_priority NUMBER := 0;                                                       
BEGIN                                                                           

/* The calling procedure must check that a delete is required */
                                                                                
/* Data priority determination - delete the database row only if
   -- new row comes from a manual loading application OR
   -- (old row does not come from a manual loading application AND
   --  (both old and new rows have priority, and new is higher (closer to 0)
   --   OR new row has the same agen_id as the old row)) */

    begin /* Find if this loading_application is manual */                      
    select nvl(manual_edit_app,'N')                                             
      into manual_edit                                                          
      from hdb_loading_application                                              
      where loading_application_id = loading_application_id_in;                 
      exception when others then null;                                          
    end;                                                                        
                                                                                
    IF manual_edit = 'N' then                                                   
    begin                                                                       
	select nvl(priority_rank,0)
          into new_priority                                                     
	  from ref_source_priority                                                     
	 where site_datatype_id = SITE_DATATYPE_ID_IN                                  
	   and agen_id = AGEN_ID_IN;                                                   
 	exception when others THEN null; /*not an error to not have entry for this age
ncy and sdi*/                                                                   
                                                                                
    end;                                                                        

    begin /* get the old agency and priority */                         
        SELECT nvl(b.priority_rank,0), a.agen_id, a.loading_application_id
    	  INTO old_priority, old_agen_id, old_loading_application_id
    	  FROM r_base a, ref_source_priority b                                     
    	 WHERE a.site_datatype_id = site_datatype_id_in                            
    	   AND a.INTERVAL = interval_in                                            
    	   AND a.start_date_time = start_date_time_in                              
    	   and a.end_date_time = END_DATE_TIME_IN                                  
    	   and a.agen_id = b.agen_id(+)                                            
    	   and a.site_datatype_id = b.site_datatype_id(+);                         
    end;                                                                        
	
    /* see if old app was manual */
    begin 
    select nvl(manual_edit_app,'N')                                             
      into old_manual_edit                                                      
      from hdb_loading_application
      where loading_application_id = old_loading_application_id;                
      exception when others then null;                                          
    end;

    end if;                                                                     

                                                                                
                                                                        
/* DO THE DELETE IF:*/                                                          
    IF ((manual_edit = 'Y') OR 
	(old_manual_edit = 'N' AND 
	 (((old_priority >= new_priority) AND (old_priority > 0 and new_priority > 0)) OR 
 	   (agen_id_in = old_agen_id))))
     THEN

     DELETE FROM R_BASE                                                               
     WHERE site_datatype_id = SITE_DATATYPE_ID_IN                               
       AND interval = INTERVAL_IN                                              
       AND start_date_time = START_DATE_TIME_IN                                 
       and end_date_time = END_DATE_TIME_IN;                                    
    ELSE                                                                        
	NULL; /* delete was not done*/                                                 
    END IF;                                                                     
                                                                                
END;
/

show errors;
/
create public synonym delete_r_base for delete_r_base;
grant execute on delete_r_base to app_role;
