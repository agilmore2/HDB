
CREATE OR REPLACE PROCEDURE is_derivation_source ( SITE_DATATYPE_ID_IN NUMBER, INTERVAL_IN VARCHAR2, YES_OR_NO OUT VARCHAR2)
IS
BEGIN

	validate_sdi_interval(SITE_DATATYPE_ID_IN, INTERVAL_IN);

	YES_OR_NO := 'N';

	begin
	        select 'Y'
	       	into YES_OR_NO
	        from ref_derivation_source a
	        where
	        a.site_datatype_id = SITE_DATATYPE_ID_IN and
	        a.interval = INTERVAL_IN;
        exception when others THEN null; 
        end;
END;
.
/

grant execute on is_derivation_source to public;
create public synonym is_derivation_source for is_derivation_source;
