
CREATE OR REPLACE PROCEDURE find_derivation_source ( 
DEST_SITE_DATATYPE_ID_IN NUMBER, DEST_INTERVAL_IN VARCHAR2,
BASE_SITE_DATATYPE_ID_OUT OUT NUMBER, BASE_INTERVAL_OUT OUT VARCHAR2)
IS
BEGIN
	validate_sdi_interval(DEST_SITE_DATATYPE_ID_IN,DEST_INTERVAL_IN);
	
	begin /* Find derivation destination base*/
		select d.base_site_datatype_id
		into BASE_SITE_DATATYPE_ID_OUT
		from ref_derivation_destination d
		where d.dest_site_datatype_id = DEST_SITE_DATATYPE_ID_IN;
	exception when others THEN NULL;
        end;

	if BASE_SITE_DATATYPE_ID_OUT is null then
       	begin /*get interval for base sdi*/
       		select b.interval, b.site_datatype_id
		into BASE_INTERVAL_OUT, BASE_SITE_DATATYPE_ID_OUT
		from ref_derivation_source b
        	where
        	b.site_datatype_id = DEST_SITE_DATATYPE_ID_IN;
	exception when others THEN NULL;
	END;
	else 
       	begin /*get interval for base sdi*/
       		select b.interval, b.site_datatype_id
		into BASE_INTERVAL_OUT, BASE_SITE_DATATYPE_ID_OUT
		from ref_derivation_source b
        	where
        	b.site_datatype_id = BASE_SITE_DATATYPE_ID_OUT;
	exception when others THEN NULL;
	END;
	end if;
END;
.
/
 
grant execute on find_derivation_source to public;
create public synonym find_derivation_source for find_derivation_source;


