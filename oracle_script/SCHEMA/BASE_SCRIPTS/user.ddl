create user DBA identified by PASSWD
default tablespace HDB_data quota unlimited on HDB_data
temporary tablespace HDB_temp quota unlimited on HDB_temp;
grant dba to DBA;
grant create session to DBA;
grant connect  to DBA;
