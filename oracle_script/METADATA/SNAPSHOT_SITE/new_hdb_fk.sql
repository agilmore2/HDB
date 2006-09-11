set echo off
set heading off
set feedback off
set pagesize 0
set linesize 200
set verify off

spool new_hdb_fk_script.sql

-- First drop all the constraints that will be recreated to point to the snapshots.
select 'ALTER TABLE '||a.table_name||' DROP CONSTRAINT '||a.constraint_name||';'
from all_constraints a where a.r_constraint_name like 'HDB_%_PK'
       OR (a.table_name like 'HDB_%' and a.constraint_type = 'R'
           and a.table_name not in ('HDB_UNIT','HDB_DIMENSION','HDB_PHYSICAL_QUANTITY'))
order by a.table_name;

select ' ' from dual;
-- Then recreate the constraints to point to the snapshots.
select distinct 'ALTER TABLE '||replace (a.table_name, '_OLD', '') ||' ADD CONSTRAINT '||a.constraint_name||
          ' FOREIGN KEY ('||c.column_name||') REFERENCES '||replace (b.table_name, '_OLD', '')||'('||b.column_name||');'
from all_constraints a, all_cons_columns b, all_cons_columns c
where (a.r_constraint_name like 'HDB_%_PK' 
       OR (a.table_name like 'HDB_%' and a.constraint_type = 'R'
           and a.table_name not in ('HDB_UNIT','HDB_DIMENSION','HDB_PHYSICAL_QUANTITY')))
and   a.r_constraint_name = b.constraint_name
and   a.constraint_name = c.constraint_name;

select 'quit' from dual;

spool off;
set feedback on
quit




