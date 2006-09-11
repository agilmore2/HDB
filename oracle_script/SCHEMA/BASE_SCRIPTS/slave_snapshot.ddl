/* Create snapshots on slaves */
create snapshot hdb_physical_quantity
pctfree 10
pctused 40
tablespace HDB_data
storage (initial 50k
         next 50k
         pctincrease 0)
as select * from CZAR_DBA.hdb_physical_quantity@CZAR_DBNAME;

create snapshot hdb_unit
pctfree 10
pctused 40
tablespace HDB_data
storage (initial 50k
         next 50k
         pctincrease 0)
as select * from CZAR_DBA.hdb_unit@CZAR_DBNAME;

create snapshot hdb_dimension
pctfree 10
pctused 40
tablespace HDB_data
storage (initial 50k
         next 50k
         pctincrease 0)
as select * from CZAR_DBA.hdb_dimension@CZAR_DBNAME;

