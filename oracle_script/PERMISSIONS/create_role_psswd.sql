/* Run as psswd_user */
drop table role_psswd;

create table role_psswd (
role varchar2 (30) NOT NULL,
psswd varchar2 (30) NOT NULL
)
pctfree 10
pctused 40
tablespace HDB_data
storage (initial 500k
         next     50k
         pctincrease 25);

ALTER TABLE role_psswd
      ADD ( CONSTRAINT role_psswd_pk
            PRIMARY KEY (role));


/* Insert needed rows */
insert into role_psswd values ('app_role', 'hdb_app_');
insert into role_psswd values ('ref_meta_role', 'ref_meta_');
insert into role_psswd values ('hdb_meta_role', 'hdb_meta_');
insert into role_psswd values ('model_role', 'hdb_model_');
insert into role_psswd values ('derivation_role', 'hdb_derivation_');
