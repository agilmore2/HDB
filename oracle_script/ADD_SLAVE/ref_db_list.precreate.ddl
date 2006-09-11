create table ref_db_list (                            
session_no                     number(11) NOT NULL  ,         
db_site_db_name                varchar2(25) NOT NULL  ,      
db_site_code                   varchar2(3) NOT NULL  ,      
maxid                          number(11) NULL      ,      
max_sync_id                    number(11) NULL            
)                                                        
pctfree 10
pctused 40
tablespace HDB_data
storage (initial 50k
         next 50k
         pctincrease 0);
;                        
