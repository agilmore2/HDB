alter table hdb_site
add
  constraint  hdb_site_fk1
  foreign key (objecttype_id)
  references  hdb_objecttype(objecttype_id);

alter table hdb_site
add
  constraint  hdb_site_fk2
  foreign key (parent_site_id)
  references  hdb_site(site_id);

alter table hdb_site
add
  constraint  hdb_site_fk3
  foreign key (parent_objecttype_id)
  references  hdb_objecttype(objecttype_id);

alter table hdb_site
add
  constraint  hdb_site_fk4
  foreign key (hydrologic_unit,segment_no)
  references  hdb_river_reach(hydrologic_unit,segment_no);

alter table hdb_site
add
  constraint  hdb_site_fk5
  foreign key (state_id)
  references  hdb_state(state_id);

alter table hdb_site
add
  constraint  hdb_site_fk6
  foreign key (basin_id)
  references  hdb_site(site_id);

alter table hdb_site
add
    constraint   hdb_site_fk7
    foreign key  (db_site_code)
    references   ref_db_list(db_site_code);
