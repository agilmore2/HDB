create or replace trigger hdb_ext_data_code_dt_load                                                                     
before insert or update on hdb_ext_data_code                                                                     
for each row                                                                                                            
begin                                                                                                                   
:new.date_time_loaded := sysdate; 
end;                                                                                  
                                                                                                                        
/                                                                                                                       
show errors trigger hdb_ext_data_code_dt_load;                                                                          


create or replace trigger hdb_ext_data_code_arch_upd                                                                    
after update on hdb_ext_data_code 
for each row 
begin 
insert into hdb_ext_data_code_archive (                     
EXT_DATA_CODE_SYS_ID,                                                                                                   
PRIMARY_DATA_CODE,                                                                                                      
SECONDARY_DATA_CODE,                                                                                                    
HDB_DATATYPE_ID,                                                                                                        
DATE_TIME_LOADED, 
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) 
values (                                           
:old.EXT_DATA_CODE_SYS_ID,                                                                                              
:old.PRIMARY_DATA_CODE,                                                                                                 
:old.SECONDARY_DATA_CODE,                                                                                               
:old.HDB_DATATYPE_ID,                                                                                                   
:old.date_time_loaded,
'UPDATE', 
sysdate, 
NULL); 
end;                                                                    
/                                                                                                                       
show errors trigger hdb_ext_data_code_arch_upd;                                                                         

                                                                                                                        
create or replace trigger hdb_ext_data_code_arch_del                                                                    
after delete on hdb_ext_data_code 
for each row 
begin 
insert into hdb_ext_data_code_archive (                     
EXT_DATA_CODE_SYS_ID,                                                                                                   
PRIMARY_DATA_CODE,                                                                                                      
SECONDARY_DATA_CODE,                                                                                                    
HDB_DATATYPE_ID,                                                                                                        
DATE_TIME_LOADED, 
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) values (                                           
:old.EXT_DATA_CODE_SYS_ID,                                                                                              
:old.PRIMARY_DATA_CODE,                                                                                                 
:old.SECONDARY_DATA_CODE,                                                                                               
:old.HDB_DATATYPE_ID,                                                                                                   
:old.date_time_loaded,
'DELETE', 
sysdate, 
NULL); 
end;                                                                    
/                                                                                                                       
show errors trigger hdb_ext_data_code_arch_del;                                                                         

