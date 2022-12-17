
create or replace trigger cp_algo_property_update                                                                    
after update on cp_algo_property 
for each row 
begin 
/*  This trigger created by M.  Bogner  04/04/2006
    This trigger archives any updates to the table
    cp_algo_property.
*/
insert into cp_algo_property_archive (                     
ALGORITHM_ID,
PROP_NAME,
PROP_VALUE,
ARCHIVE_REASON,
DATE_TIME_ARCHIVED,
ARCHIVE_CMMNT
) 
values (                                           
:old.ALGORITHM_ID,
:old.PROP_NAME,
:old.PROP_VALUE,
'UPDATE', 
sysdate, 
coalesce(
          sys_context('APEX$SESSION','app_user')
         ,regexp_substr(sys_context('userenv','client_identifier'),'^[^:]*')
         ,sys_context('userenv','session_user')
         ) || ':' || sys_context('userenv','os_user') 
         || ':' || sys_context('userenv','HOST') 
         || ':' || sys_context('userenv','CLIENT_PROGRAM_NAME')
); 
end;                                                                    
/                                                                                                                       
show errors trigger cp_algo_property_update;                                                                         

                                                                                                                        
create or replace trigger cp_algo_property_delete                                                                    
after delete on cp_algo_property 
for each row 
begin 
/*  This trigger created by M.  Bogner  04/04/2006
    This trigger archives any deletes to the table
    cp_algo_property.
*/
insert into cp_algo_property_archive (                     
ALGORITHM_ID,
PROP_NAME,
PROP_VALUE,
ARCHIVE_REASON,
DATE_TIME_ARCHIVED,
ARCHIVE_CMMNT
) 
values (                                           
:old.ALGORITHM_ID,
:old.PROP_NAME,
:old.PROP_VALUE,
'DELETE', 
sysdate, 
coalesce(
          sys_context('APEX$SESSION','app_user')
         ,regexp_substr(sys_context('userenv','client_identifier'),'^[^:]*')
         ,sys_context('userenv','session_user')
         ) || ':' || sys_context('userenv','os_user') 
         || ':' || sys_context('userenv','HOST') 
         || ':' || sys_context('userenv','CLIENT_PROGRAM_NAME')
); 
end;                                                                    
/                                                                                                                       
show errors trigger cp_algo_property_delete;                                                                         

