/* new user agilmore*/
create user agilmore identified by newagilmore
default tablespace HDB_user quota unlimited on HDB_user
temporary tablespace HDB_temp quota unlimited on HDB_temp;
grant connect to agilmore;
grant hdb_meta_role to agilmore;
grant ref_meta_role to agilmore;
grant savoir_faire to agilmore;
grant model_role to agilmore;
alter user agilmore default role connect, savoir_faire;

/* new user dking*/
create user dking identified by dking_new
default tablespace HDB_user quota unlimited on HDB_user
temporary tablespace HDB_temp quota unlimited on HDB_temp;
grant connect to dking;
alter user dking default role connect;

/* new user pdavidson*/
create user pdavidson identified by pdavidson_new
default tablespace HDB_user quota unlimited on HDB_user
temporary tablespace HDB_temp quota unlimited on HDB_temp;
grant connect to pdavidson;
grant hdb_meta_role to pdavidson;
grant ref_meta_role to pdavidson;
grant savoir_faire to pdavidson;
grant model_role to pdavidson;
alter user pdavidson default role connect, savoir_faire;

/* new user rclayton*/
create user rclayton identified by rclayton_new
default tablespace HDB_user quota unlimited on HDB_user
temporary tablespace HDB_temp quota unlimited on HDB_temp;
grant connect to rclayton;
grant hdb_meta_role to rclayton;
grant ref_meta_role to rclayton;
grant savoir_faire to rclayton;
grant model_role to rclayton;
alter user rclayton default role connect, savoir_faire;

/* new user cmarra*/
create user cmarra identified by cmarra_new
default tablespace HDB_user quota unlimited on HDB_user
temporary tablespace HDB_temp quota unlimited on HDB_temp;
grant connect to cmarra;
grant hdb_meta_role to cmarra;
grant ref_meta_role to cmarra;
grant savoir_faire to cmarra;
grant model_role to cmarra;
alter user cmarra default role connect, savoir_faire;


