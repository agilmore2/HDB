set pagesize 500;
set linesize 100;

select object_type, substr (object_name, 1, 50) OBJECT_NAME from dba_objects 
where (owner like '%DBA' or owner like 'PSS%') 
and object_type in ('PROCEDURE', 'FUNCTION', 'VIEW', 'PACKAGE', 'PACKAGE BODY', 'TRIGGER',
'SEQUENCE', 'INDEX', 'TABLE', 'DATABASE LINK') order by 1, 2;

select constraint_name from dba_constraints where (owner like '%DBA' or owner like 'PSS%')
and constraint_name not like 'SYS%'
order by 1;

select synonym_name from dba_synonyms 
where (table_owner like '%DBA' or table_owner like 'PSS%') order by 1;

select role from dba_roles order by 1;

select substr (grantee, 1, 20) GRANTEE, table_name, substr (privilege, 1, 20) PRIVILEGE from dba_tab_privs 
where (
grantee not like 'AQ%' and
grantee not like 'DBA%' and
grantee not like 'DEL%' and
grantee not like 'EXECUTE%' and
grantee not like 'EXP%' and
grantee not like 'IMP%' and
grantee not like 'OUT%' and
grantee not like 'SELECT%' and
grantee not like 'SNM%' and
grantee not like 'SYS%')
order by 1, 2, 3;
quit;
