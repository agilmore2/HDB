CREATE OR REPLACE TRIGGER REF_SPATIAL_RELATION_SITE_PERM
  AFTER INSERT OR DELETE OR UPDATE ON REF_SPATIAL_RELATION
  REFERENCING FOR EACH ROW
  begin
     if not (DBMS_SESSION.Is_Role_Enabled ('SAVOIR_FAIRE')
             OR DBMS_SESSION.Is_Role_Enabled ('REF_META_ROLE')) then
           check_site_id_auth (:new.a_site_id);
           check_site_id_auth (:new.b_site_id);
        end if;
end;
/
show errors trigger REF_SPATIAL_RELATION_SITE_PERM;
/

