
CREATE OR REPLACE PROCEDURE validate_sdi_interval ( 
SITE_DATATYPE_ID_IN NUMBER, INTERVAL_IN VARCHAR2)
IS
interval_order_in number;
sdi_holder number;
BEGIN
	IF SITE_DATATYPE_ID_IN IS NULL THEN DENY_ACTION ( 'INVALID <NULL> SITE_DATATYPE_ID' );
	ELSIF INTERVAL_IN IS NULL THEN DENY_ACTION ( 'INVALID <NULL> INTERVAL' );
	end if;
	
	begin
		Select c.interval_order
        	into interval_order_in
        	from hdb_interval c
		where
        	c.interval_name = INTERVAL_IN;
	exception when others THEN
       		DENY_ACTION ( 'INVALID INTERVAL: ' || INTERVAL_IN );
       		return;
    	end;
    	
	begin  	
        	select e.site_datatype_id
        	into sdi_holder /*not used for anything*/
        	from hdb_site_datatype e
        	where e.site_datatype_id = SITE_DATATYPE_ID_IN;
        exception when others THEN
		DENY_ACTION ( 'INVALID SDI: ' || SITE_DATATYPE_ID_IN );
      		return;
	end;
END;
.
/

create public synonym validate_sdi_interval for validate_sdi_interval;
grant execute on validate_sdi_interval to public;


