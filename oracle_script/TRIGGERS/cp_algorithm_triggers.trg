
create or replace trigger cp_algorithm_update                                                                    
after update on cp_algorithm 
for each row 
begin 
/*  This trigger created by M.  Bogner  04/04/2006
    This trigger archives any updates to the table
    cp_algorithm.
	
	updated to add DB_OFFICE_CODE column in archive table by IsmailO. 08/26/2019
*/
insert into cp_algorithm_archive (                     
ALGORITHM_ID,
ALGORITHM_NAME,
EXEC_CLASS,
CMMNT,
ARCHIVE_REASON,
DATE_TIME_ARCHIVED,
ARCHIVE_CMMNT,
DB_OFFICE_CODE
) 
values (                                           
:old.ALGORITHM_ID,
:old.ALGORITHM_NAME,
:old.EXEC_CLASS,
:old.CMMNT,
'UPDATE', 
sysdate, 
coalesce(
          sys_context('APEX$SESSION','app_user')
         ,regexp_substr(sys_context('userenv','client_identifier'),'^[^:]*')
         ,sys_context('userenv','session_user')
         ) || ':' || sys_context('userenv','os_user') 
         || ':' || sys_context('userenv','HOST') 
         || ':' || sys_context('userenv','CLIENT_PROGRAM_NAME'),
:old.DB_OFFICE_CODE); 
end;                                                                    
/                                                                                                                       
show errors trigger cp_algorithm_update;                                                                         

                                                                                                                        
create or replace trigger cp_algorithm_delete                                                                    
after delete on cp_algorithm 
for each row 
begin 
/*  This trigger created by M.  Bogner  04/04/2006
    This trigger archives any deletes to the table
    cp_algorithm.
	
	updated to add DB_OFFICE_CODE column in archive table by IsmailO. 08/26/2019
*/
insert into cp_algorithm_archive (                     
ALGORITHM_ID,
ALGORITHM_NAME,
EXEC_CLASS,
CMMNT,
ARCHIVE_REASON,
DATE_TIME_ARCHIVED,
ARCHIVE_CMMNT,
DB_OFFICE_CODE
) 
values (                                           
:old.ALGORITHM_ID,
:old.ALGORITHM_NAME,
:old.EXEC_CLASS,
:old.CMMNT,
'DELETE', 
sysdate, 
coalesce(
          sys_context('APEX$SESSION','app_user')
         ,regexp_substr(sys_context('userenv','client_identifier'),'^[^:]*')
         ,sys_context('userenv','session_user')
         ) || ':' || sys_context('userenv','os_user') 
         || ':' || sys_context('userenv','HOST') 
         || ':' || sys_context('userenv','CLIENT_PROGRAM_NAME'),
:old.DB_OFFICE_CODE); 
end;                                                                    
/                                                                                                                       
show errors trigger cp_algorithm_delete;                                                                         

