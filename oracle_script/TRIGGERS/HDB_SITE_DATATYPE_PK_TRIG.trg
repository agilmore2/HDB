  CREATE OR REPLACE TRIGGER HDB_SITE_DATATYPE_PK_TRIG
  BEFORE INSERT OR UPDATE ON HDB_SITE_DATATYPE
  REFERENCING FOR EACH ROW
  BEGIN IF inserting THEN IF populate_pk.pkval_pre_populated = FALSE THEN :new.SITE_DATATYPE_ID := populate_pk.get_pk_val( 'HDB_SITE_DATATYPE', FALSE );  END IF; ELSIF updating THEN :new.SITE_DATATYPE_ID := :old.SITE_DATATYPE_ID; END IF; END;
/

show errors trigger HDB_SITE_DATATYPE_PK_TRIG;
/

create or replace trigger hdb_site_datatype_arch_update                                                                    
after update on hdb_site_datatype 
for each row 
begin 
insert into hdb_site_datatype_archive (  
SITE_ID,
DATATYPE_ID,
SITE_DATATYPE_ID,     
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) 
values (                                           
:old.SITE_ID,                                                                                              
:old.DATATYPE_ID,                                                                                                 
:old.SITE_DATATYPE_ID,                                                                                                                                                                                                
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
show errors trigger hdb_site_datatype_arch_update;
/                                                                         

                                                                                                                        
create or replace trigger hdb_site_datatype_arch_delete                                                                    
after delete on hdb_site_datatype 
for each row 
begin 
insert into hdb_site_datatype_archive (                     
SITE_ID,
DATATYPE_ID,
SITE_DATATYPE_ID, 
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) values (                                           
:old.SITE_ID,                                                                                              
:old.DATATYPE_ID,                                                                                                 
:old.SITE_DATATYPE_ID,
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
show errors trigger hdb_site_datatype_arch_delete;  
/
