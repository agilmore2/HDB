create or replace trigger hdb_site_site_perm
after             insert OR update OR delete
on                hdb_site
for   each row
begin
     if not (DBMS_SESSION.Is_Role_Enabled ('SAVOIR_FAIRE')
             OR DBMS_SESSION.Is_Role_Enabled ('HDB_META_ROLE')) then
	   check_site_id_auth (:new.site_id);
	end if;
end;
/
show errors trigger hdb_site_site_perm;
/

create or replace trigger hdb_site_archive_update                                                                    
after update on hdb_site 
for each row 
begin 
insert into hdb_site_archive (  
SITE_ID,
SITE_NAME,
SITE_COMMON_NAME,
OBJECTTYPE_ID,
PARENT_SITE_ID,
PARENT_OBJECTTYPE_ID,
STATE_ID,
BASIN_ID,
LAT,
LONGI,
HYDROLOGIC_UNIT,
SEGMENT_NO,
RIVER_MILE,
ELEVATION,
DESCRIPTION,
NWS_CODE,
SCS_ID,
SHEF_CODE,
USGS_ID,
DB_SITE_CODE,
ARCHIVE_REASON,
DATE_TIME_ARCHIVED,
ARCHIVE_CMMNT) 
values (                                           
:old.SITE_ID,
:old.SITE_NAME,
:old.SITE_COMMON_NAME,
:old.OBJECTTYPE_ID,
:old.PARENT_SITE_ID,
:old.PARENT_OBJECTTYPE_ID,
:old.STATE_ID,
:old.BASIN_ID,
:old.LAT,
:old.LONGI,
:old.HYDROLOGIC_UNIT,
:old.SEGMENT_NO,
:old.RIVER_MILE,
:old.ELEVATION,
:old.DESCRIPTION,
:old.NWS_CODE,
:old.SCS_ID,
:old.SHEF_CODE,
:old.USGS_ID,
:old.DB_SITE_CODE,                                                                                                                                                                                                
'UPDATE', 
sysdate, 
sys_context('USERENV', 'SESSION_USER')
); 
end;                                                                    
/                                                                                                                       
show errors trigger hdb_site_archive_update;                                                                         
/
                                                                                                                        
create or replace trigger hdb_site_archive_delete                                                                    
after delete on hdb_site 
for each row 
begin 
insert into hdb_site_archive (                     
SITE_ID,
SITE_NAME,
SITE_COMMON_NAME,
OBJECTTYPE_ID,
PARENT_SITE_ID,
PARENT_OBJECTTYPE_ID,
STATE_ID,
BASIN_ID,
LAT,
LONGI,
HYDROLOGIC_UNIT,
SEGMENT_NO,
RIVER_MILE,
ELEVATION,
DESCRIPTION,
NWS_CODE,
SCS_ID,
SHEF_CODE,
USGS_ID,
DB_SITE_CODE,
ARCHIVE_REASON,
DATE_TIME_ARCHIVED,
ARCHIVE_CMMNT
) values (                                           
:old.SITE_ID,
:old.SITE_NAME,
:old.SITE_COMMON_NAME,
:old.OBJECTTYPE_ID,
:old.PARENT_SITE_ID,
:old.PARENT_OBJECTTYPE_ID,
:old.STATE_ID,
:old.BASIN_ID,
:old.LAT,
:old.LONGI,
:old.HYDROLOGIC_UNIT,
:old.SEGMENT_NO,
:old.RIVER_MILE,
:old.ELEVATION,
:old.DESCRIPTION,
:old.NWS_CODE,
:old.SCS_ID,
:old.SHEF_CODE,
:old.USGS_ID,
:old.DB_SITE_CODE,  
'DELETE', 
sysdate, 
sys_context('USERENV', 'SESSION_USER')
); 
end;                                                                    
/                                                                                                                       
show errors trigger hdb_site_archive_delete;   
/