create user brio_user identified by h1nterface
default tablespace HDB_user quota unlimited on HDB_user
temporary tablespace HDB_temp quota unlimited on HDB_temp;
grant create session to brio_user;
grant connect  to brio_user;
