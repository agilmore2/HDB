/* 
This sqlscript does the following:
store date of lookback period (a command line variable)

grab all data for defined sites/datatypes 
   from yesterday back to the lookback period
   and write them to a file named wapa_dateoflookback.dat

It is expected that the calling shell script, if any, would
create the glen_ data files by grepping through the output
It will have to determine whether the glen_update file is empty or not.

Data for this query is defined by an entry in hdb_ext_data_source
with the name 'HDB WAPA/GCMRC Update' and a loading application id exists 
with the same name. 
Further, we'll reuse the SCADA(?) site name mappings
and use the datatype_common_names to populate the ref_ext_site_data_map
table. These site_data mappings will specify which site_datatype_ids to
query for this application.

Assumptions: all queries are for hourly data

DDL/DML to generate metadata to drive this report:


insert into hdb_loading_application values(null, 'HDB WAPA/GCMRC Update',null,
'Reporting application for data from r_hour for WAPA and GCMRC');

CREATE TABLE ref_loading_application_data
(
        LOADING_APPLICATION_ID NUMBER NOT NULL,
        LAST_RETRIEVAL DATE  -- Null means no retrieval yet.
)
tablespace HDB_data
;

-- primary key for table cp_comp_proc_lock
alter table ref_loading_application_data  add constraint rlad_loading_appl_id_pk 
primary key (loading_application_id) 
using index tablespace hdb_idx;

--  foreign key loading_application_id
alter table ref_loading_application_data add constraint rlad_loading_appl_id_pk 
foreign key (loading_application_id) 
references hdb_loading_application (loading_application_id);

grant select,update,insert on ref_loading_application_data to app_user;
create public synonym ref_loading_application_data for ref_loading_application_data;

insert into ref_loading_application_data values(53,sysdate);

insert into hdb_ext_data_source
values (
 9, 'HDB WAPA/GCMRC Update', 7,null,
7,null,null,null,
'Report definition for querying data from r_hour for WAPA and GCMRC, data codes use datatype_common_names from hdb_datatype',null
);

select 'exec create_site_data_map ('||
a.ext_data_source_id||','||b.primary_site_code||','||
d.datatype_common_name||',0,'||e.site_datatype_id||
',''hour'',null,null,7,''Y'',null,null);'
from hdb_ext_data_source a, hdb_ext_site_code b,
hdb_ext_site_code_sys c, hdb_datatype d, hdb_site_datatype e
where 
a.ext_data_source_name = 'HDB WAPA/GCMRC Update' and
b.ext_site_code_sys_id = c.ext_site_code_sys_id and
c.ext_site_code_sys_name = 'XA21 SCADA' and
e.site_id = b.hdb_site_id and
e.datatype_id = d.datatype_id and
d.datatype_id in (39,49,46,1197,73,14);

--Other datatypes?

	exec create_site_data_map (9,BLUE,power release,0,1857,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,spill,0,4147,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,pool elevation,0,1923,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,ave bypass release,0,4149,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,ave head,0,1970,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,BLUE,power,0,1976,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,power release,0,1859,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,spill,0,4155,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,pool elevation,0,1925,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,ave bypass release,0,4157,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,ave head,0,1972,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,CRYSTAL,power,0,1978,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,power release,0,1861,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,spill,0,2416,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,pool elevation,0,1927,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,ave bypass release,0,4164,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,ave head,0,1974,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FLAMING,power,0,1980,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,power release,0,1860,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,spill,0,4159,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,pool elevation,0,1926,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,ave bypass release,0,4161,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,ave head,0,1973,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,FONTENELLE,power,0,1979,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,power release,0,1862,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,spill,0,4166,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,pool elevation,0,1928,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,ave bypass release,0,4167,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,ave head,0,1975,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,GLEN,power,0,1981,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,power release,0,1858,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,spill,0,4151,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,pool elevation,0,1924,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,ave bypass release,0,4153,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,ave head,0,1971,'hour',null,null,7,'Y',null,null);
	exec create_site_data_map (9,MORROW,power,0,1977,'hour',null,null,7,'Y',null,null);


*/



-- command line argument for date to retrieve data for
define day = &1;

define app_name = 'HDB WAPA/GCMRC Update';

-- setup output
set feedback off
set pagesize 0
set linesize 100
set verify off

set heading off;
set trimspool on;

set termout off;


/* This query is for all data in the specified day. */

spool wapa_&&day..dat
 -- extra period here because using &&day chomps one

select to_char(a.start_date_time,'YYYY-MM-DD,HH24:MI:SS')||','||
b.primary_site_code||','||b.primary_data_code||','||
round(a.value,2) as data
from r_hour a, ref_ext_site_data_map b, hdb_ext_data_source c where
trunc(a.start_date_time,'DD')
between to_date(&&day,'YYYYMMDD') and to_date(&&day,'YYYYMMDD')+1 and
b.hdb_site_datatype_id = a.site_datatype_id and
b.ext_data_source_id = c.ext_data_source_id and
c.ext_data_source_name = '&&app_name' 
order by b.primary_site_code, b.primary_data_code, a.start_date_time;

spool off;
