ALTER TABLE hdb_dimension         
      ADD ( CONSTRAINT hdb_dimension_pk
            PRIMARY KEY (dimension_id) using index tablespace hdb_idx storage (initial 50k next 50k pctincrease 0));   

ALTER TABLE hdb_physical_quantity
      ADD ( CONSTRAINT hdb_physical_quantity_pk
            PRIMARY KEY (physical_quantity_name) using index tablespace hdb_idx storage (initial 50k next 50k pctincrease 0));

ALTER TABLE hdb_unit  
      ADD ( CONSTRAINT hdb_unit_pk
            PRIMARY KEY (unit_id) using index tablespace hdb_idx storage (initial 50k next 50k pctincrease 0));

ALTER TABLE ref_hdb_installation
      ADD ( CONSTRAINT ref_hdb_installation_pk
            PRIMARY KEY (db_site_db_name) using index tablespace hdb_idx storage (initial 50k next 50k pctincrease 0));  

ALTER TABLE ref_phys_quan_refresh_monitor
      ADD ( CONSTRAINT ref_phys_quan_refresh_mon_pk
            PRIMARY KEY (db_site_db_name) using index tablespace hdb_idx storage (initial 50k next 50k pctincrease 0));  

