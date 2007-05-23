ALTER TABLE ref_source_priority ADD CONSTRAINT ref_source_priority_fk1 FOREIGN KEY
(site_datatype_id)
 REFERENCES hdb_site_datatype(site_datatype_id);

ALTER TABLE ref_source_priority ADD CONSTRAINT ref_source_priority_fk2 FOREIGN KEY
(agen_id)
 REFERENCES hdb_agen(agen_id);

ALTER TABLE ref_source_priority ADD CONSTRAINT positive_priority check (priority_rank >= 1);



