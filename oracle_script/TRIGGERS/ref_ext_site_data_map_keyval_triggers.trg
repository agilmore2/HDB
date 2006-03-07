create or replace trigger ref_ext_site_data_mapk_dt_load 
before insert or update on ref_ext_site_data_map_keyval  
for each row 
begin
 :new.date_time_loaded := sysdate; 
end;   
/
show errors trigger ref_ext_site_data_mapk_dt_load;   



create or replace trigger site_data_map_keyval_arch_upd                                                                
after update on ref_ext_site_data_map_keyval 
for each row 
begin 
insert into ref_ext_site_data_map_key_arch (     
MAPPING_ID,                                                                                                             
KEY_NAME,                                                                                                               
KEY_VALUE,                                                                                                              
DATE_TIME_LOADED, 
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) 
values (                                           
:old.MAPPING_ID,                                                                                                        
:old.KEY_NAME,                                                                                                          
:old.KEY_VALUE,                                                                                                         
:old.date_time_loaded,
'UPDATE', 
sysdate, 
NULL); 
end;                                                                    
/                                                                                                                       
show errors trigger site_data_map_keyval_arch_upd;                                                                      

                                                                                                                        
create or replace trigger site_data_map_keyval_arch_del                                                                
after delete on ref_ext_site_data_map_keyval 
for each row 
begin 
insert into ref_ext_site_data_map_key_arch (     
MAPPING_ID,                                                                                                             
KEY_NAME,                                                                                                               
KEY_VALUE,                                                                                                              
DATE_TIME_LOADED, 
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) 
values (                                           
:old.MAPPING_ID,                                                                                                        
:old.KEY_NAME,                                                                                                          
:old.KEY_VALUE,                                                                                                         
:old.date_time_loaded,
'DELETE', 
sysdate, 
NULL); 
end;                                                                    
/                                                                                                                       
show errors trigger site_data_map_keyval_arch_del;                                                                      

