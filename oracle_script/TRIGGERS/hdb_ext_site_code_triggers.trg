create or replace trigger hdb_ext_site_code_dt_load                                                                     
before insert or update on hdb_ext_site_code                                                                     
for each row                                                                                                            
begin                                                                                                                   
:new.date_time_loaded := sysdate; end;                                                                                  
                                                                                                                        
/                                                                                                                       
show errors trigger hdb_ext_site_code_dt_load;                                                                          


create or replace trigger hdb_ext_site_code_arch_upd                                                                    
after update on hdb_ext_site_code 
for each row 
begin 
insert into hdb_ext_site_code_archive (                     
EXT_SITE_CODE_SYS_ID,                                                                                                   
PRIMARY_SITE_CODE,                                                                                                      
SECONDARY_SITE_CODE,                                                                                                    
HDB_SITE_ID,                                                                                                            
DATE_TIME_LOADED, 
ARCHIVE_REASON,
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) 
values (                                           
:old.EXT_SITE_CODE_SYS_ID,                                                                                              
:old.PRIMARY_SITE_CODE,                                                                                                 
:old.SECONDARY_SITE_CODE,                                                                                               
:old.HDB_SITE_ID,                                                                                                       
:old.date_time_loaded,
'UPDATE', 
sysdate, 
NULL); 
end;                                                                    
/                                                                                                                       
show errors trigger hdb_ext_site_code_arch_upd;                                                                         
/                                                                                                                       
commit;                                                                                                                 
                                                                                                                        
create or replace trigger hdb_ext_site_code_arch_del                                                                    
after delete on hdb_ext_site_code 
for each row 
begin 
insert into hdb_ext_site_code_archive (                     
EXT_SITE_CODE_SYS_ID,                                                                                                   
PRIMARY_SITE_CODE,                                                                                                      
SECONDARY_SITE_CODE,                                                                                                    
HDB_SITE_ID,                                                                                                            
DATE_TIME_LOADED, 
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) 
values (                                           
:old.EXT_SITE_CODE_SYS_ID,                                                                                              
:old.PRIMARY_SITE_CODE,                                                                                                 
:old.SECONDARY_SITE_CODE,                                                                                               
:old.HDB_SITE_ID,                                                                                                       
:old.date_time_loaded,
'DELETE', 
sysdate, 
NULL); 
end;                                                                    
/                                                                                                                       
show errors trigger hdb_ext_site_code_arch_del;                                                                         
/                                                                                                                       
