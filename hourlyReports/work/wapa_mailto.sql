/* 
This sqlscript does the following:
gets email addresses defined by database in
table hdb_ext_data_source for this report

*/

define app_name = 'HDB WAPA/GCMRC Update';

-- setup output
set feedback off
set pagesize 0
set linesize 100
set verify off

set heading off;
set trimspool on;

select description from hdb_ext_data_source where
ext_data_source_name = '&&app_name';

quit;
