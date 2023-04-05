
create or replace trigger cp_algo_ts_parm_update                                                                    
after update on cp_algo_ts_parm 
for each row 
begin 
/*  This trigger created by M.  Bogner  04/04/2006
    This trigger archives any updates to the table
    cp_algo_ts_parm.
*/
insert into cp_algo_ts_parm_archive (                     
ALGORITHM_ID,
ALGO_ROLE_NAME,
PARM_TYPE,
ARCHIVE_REASON,
DATE_TIME_ARCHIVED,
ARCHIVE_CMMNT
) 
values (                                           
:old.ALGORITHM_ID,
:old.ALGO_ROLE_NAME,
:old.PARM_TYPE,
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
show errors trigger cp_algo_ts_parm_update;                                                                         

                                                                                                                        
create or replace trigger cp_algo_ts_parm_delete                                                                    
after delete on cp_algo_ts_parm 
for each row 
begin 
/*  This trigger created by M.  Bogner  04/04/2006
    This trigger archives any deletes to the table
    cp_algo_ts_parm.
*/
insert into cp_algo_ts_parm_archive (                     
ALGORITHM_ID,
ALGO_ROLE_NAME,
PARM_TYPE,
ARCHIVE_REASON,
DATE_TIME_ARCHIVED,
ARCHIVE_CMMNT
) 
values (                                           
:old.ALGORITHM_ID,
:old.ALGO_ROLE_NAME,
:old.PARM_TYPE,
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
show errors trigger cp_algo_ts_parm_delete;                                                                         

