alter table  hdb_river_reach
add 
   constraint hdb_river_reach_fk1
   foreign key (river_id)
   references  hdb_river(river_id);
