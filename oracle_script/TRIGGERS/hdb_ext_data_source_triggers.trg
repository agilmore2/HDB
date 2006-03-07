create or replace trigger hdb_ext_data_source_dt_load                                                                   
before insert or update on hdb_ext_data_source                                                                   
for each row                                                                                                            
begin                                                                                                                   
:new.date_time_loaded := sysdate; 
end;                                                                                  
/                                                                                                                       
show errors trigger hdb_ext_data_source_dt_load;                                                                        


create or replace trigger hdb_ext_data_source_arch_upd                                                                  
after update on hdb_ext_data_source 
for each row 
begin 
insert into hdb_ext_data_source_archive (                 
EXT_DATA_SOURCE_ID,                                                                                                     
EXT_DATA_SOURCE_NAME,                                                                                                   
AGEN_ID,                                                                                                                
MODEL_ID,                                                                                                               
EXT_SITE_CODE_SYS_ID,                                                                                                   
EXT_DATA_CODE_SYS_ID,                                                                                                   
COLLECTION_SYSTEM_ID,                                                                                                   
DATA_QUALITY,                                                                                                           
DESCRIPTION,                                                                                                            
DATE_TIME_LOADED, 
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) 
values (                                           
:old.EXT_DATA_SOURCE_ID,                                                                                                
:old.EXT_DATA_SOURCE_NAME,                                                                                              
:old.AGEN_ID,                                                                                                           
:old.MODEL_ID,                                                                                                          
:old.EXT_SITE_CODE_SYS_ID,                                                                                              
:old.EXT_DATA_CODE_SYS_ID,                                                                                              
:old.COLLECTION_SYSTEM_ID,                                                                                              
:old.DATA_QUALITY,                                                                                                      
:old.DESCRIPTION,                                                                                                       
:old.date_time_loaded,
'UPDATE', 
sysdate, 
NULL); 
end;                                                                    
/                                                                                                                       
show errors trigger hdb_ext_data_source_arch_upd;                                                                       
/                                                                                                                       
commit;                                                                                                                 
                                                                                                                        
create or replace trigger hdb_ext_data_source_arch_del                                                                  
after delete on hdb_ext_data_source 
for each row 
begin 
insert into hdb_ext_data_source_archive (                 
EXT_DATA_SOURCE_ID,                                                                                                     
EXT_DATA_SOURCE_NAME,                                                                                                   
AGEN_ID,                                                                                                                
MODEL_ID,                                                                                                               
EXT_SITE_CODE_SYS_ID,                                                                                                   
EXT_DATA_CODE_SYS_ID,                                                                                                   
COLLECTION_SYSTEM_ID,                                                                                                   
DATA_QUALITY,                                                                                                           
DESCRIPTION,                                                                                                            
DATE_TIME_LOADED, 
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) 
values (                                           
:old.EXT_DATA_SOURCE_ID,                                                                                                
:old.EXT_DATA_SOURCE_NAME,                                                                                              
:old.AGEN_ID,                                                                                                           
:old.MODEL_ID,                                                                                                          
:old.EXT_SITE_CODE_SYS_ID,                                                                                              
:old.EXT_DATA_CODE_SYS_ID,                                                                                              
:old.COLLECTION_SYSTEM_ID,                                                                                              
:old.DATA_QUALITY,                                                                                                      
:old.DESCRIPTION,                                                                                                       
:old.date_time_loaded,
'DELETE', 
sysdate, 
NULL); 
end;                                                                    
/                                                                                                                       
show errors trigger hdb_ext_data_source_arch_del;                                                                       
