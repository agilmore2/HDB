CREATE OR REPLACE PROCEDURE is_overwrite ( SITE_DATATYPE_ID_IN NUMBER, INTERVAL_IN VARCHAR2, OVERWRITE_INFO OUT VARCHAR2)
IS

/* output values for OVERWRITE_INFO are:
	null - when null or invalid sdi or interval are specified, application error is also raised.
	'NULL'    - use null overwrite flag when modifying r_base for this sdi and interval
	'O'       - use 'O' as overwrite flag
	'ERROR_%' - various errors as below:
	'NO_DERIV'	     - no derivation specification for this sdi exists, data will not move out of r_base unless 'O' is used
	'NO_SOURCE_INTERVAL' - could not find the interval for the source data, something is REALLY wrong
	'SAME_INTERVAL'      - input interval is the same as the base data for the sdi, derivation will not occur
	'TOO_SHORT'          - input interval is shorter than base data for sdi, derivation will not occur
	'INSTANT_OTHER'	     - source interval is instant or other, input interval is longer timeframe, can only edit source interval
	'UNKNOWN'	     - some other error occured, no idea what, but is probably serious
*/

interval_order_source number;
interval_order_in number;
sdi_holder number;
BEGIN

	validate_sdi_interval(SITE_DATATYPE_ID_IN,INTERVAL_IN);

	begin
		Select c.interval_order
        	into interval_order_in
        	from hdb_interval c
		where
        	c.interval_name = INTERVAL_IN;
	END;

	begin
        select 'NULL'
	       	into OVERWRITE_INFO
	        from ref_derivation_source a
	        where
	        a.site_datatype_id = SITE_DATATYPE_ID_IN and
	        a.interval = INTERVAL_IN;
        exception when others THEN null; /*not an error to not have entry for this sdi and interval*/
        end;

        if OVERWRITE_INFO is null then
	begin 
        	/* find interval order where derivation source is own datatype */
		begin
			select c.interval_order
	        	into interval_order_source
	        	from ref_derivation_source b, hdb_interval c 
		       	where
	        	c.interval_name = b.interval and
	        	b.site_datatype_id = SITE_DATATYPE_ID_IN;
	        exception when others THEN null;
	        end;
        	
        	/* case when derivation source is different datatype */
        	if (interval_order_source is NULL) then
		begin
			begin /* Check that a derivation even exists for this sdi, if not, return error*/
				select d.base_site_datatype_id
				into sdi_holder
				from ref_derivation_destination d
				where d.dest_site_datatype_id = SITE_DATATYPE_ID_IN;
		        exception when others THEN
		        	OVERWRITE_INFO := 'ERROR_NO_DERIV';
		        	return;
		        end;
	        		
	        	begin /*get interval for base sdi*/
	        		select c.interval_order 
				into interval_order_source
        			from ref_derivation_source b, hdb_interval c
        			where
        			b.site_datatype_id = sdi_holder and
        			c.interval_name = b.interval;
		        exception when others THEN /* one more level of indirection*/
			        begin
			        select d.base_site_datatype_id
				into sdi_holder
				from ref_derivation_destination d
				where d.dest_site_datatype_id = sdi_holder;
				
				select d.base_site_datatype_id
				into sdi_holder
				from ref_derivation_destination d
				where d.dest_site_datatype_id = SITE_DATATYPE_ID_IN;
				
		        	OVERWRITE_INFO := 'ERROR_NO_SOURCE_INTERVAL';
		        	return;
		        	END;
			END;
	        end;
	        end if;

        	if (interval_order_in > interval_order_source) then
		begin
       		        select 'O'
        		into OVERWRITE_INFO
        		from ref_derivation_destination d
		        where
        		d.dest_site_datatype_id = SITE_DATATYPE_ID_IN;
	        exception when others THEN 
	        	select 'ERROR_INSTANT_OTHER'
	        	into OVERWRITE_INFO
	        	FROM ref_derivation_source b
	        	where
	        	b.site_datatype_id = SITE_DATATYPE_ID_IN and
	        	b.interval in ( 'instant', 'other');
	        end;
	        elsif (interval_order_in = interval_order_source) then
	        	OVERWRITE_INFO := 'ERROR_SAME_INTERVAL';
	        elsif (interval_order_in < interval_order_source) then 
	        	OVERWRITE_INFO := 'ERROR_TOO_SHORT'; 
	        END IF;
        END;
        END IF;

        if OVERWRITE_INFO IS NULL then OVERWRITE_INFO := 'ERROR_UNKNOWN'; return;
        END IF;
END;
.
/

grant execute on is_overwrite to public;

create public synonym is_overwrite for is_overwrite;


