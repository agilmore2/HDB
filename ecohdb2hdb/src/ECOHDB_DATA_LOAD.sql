create or replace FUNCTION                 "ECOHDB_DATA_LOAD" (start_date date,
end_date date,
overwrite_flag VARCHAR2 DEFAULT NULL
)
RETURN NUMBER AS 
-- Loading function for data from ECOHDB
-- Relatively straightforward set up
-- data to load is defined in ref_ext_site_data_map
-- site_code is source ECAO SDI
-- SDI in map is destination SDI

-- Will be doing commits/rollbacks ourselves here.
PRAGMA AUTONOMOUS_TRANSACTION;

rows_updated number;
loading_app  NUMBER;
valid        VARCHAR2(1);

CURSOR ecodata IS
  SELECT map.hdb_site_datatype_id sdi, dat.start_date_time datadate, dat.VALUE val
  FROM
  ref_ext_site_data_map map, hdb_ext_data_source src,
  r_day@ecohdb dat, TABLE(dates_between(start_date, end_date, 'day')) DAY
  WHERE
  src.ext_data_source_name = 'Load from ECOHDB' AND
  map.ext_data_source_id = src.ext_data_source_id and
  map.primary_site_code = dat.site_datatype_id AND
  dat.start_date_time = DAY.date_time
  order by sdi, datadate;
 
BEGIN
    BEGIN
    SELECT loading_application_id
    INTO loading_app
    FROM hdb_loading_application
    WHERE loading_application_name = 'Load from ECOHDB';
  EXCEPTION
  WHEN no_data_found THEN
    deny_action('cannot find loading application id');
    RETURN 0;
  END;

  rows_updated:=0;
  
  -- if we're using overwrite, assert validation as well
  IF overwrite_flag = 'O' then
    valid:= 'V';
  END IF;
  
  FOR e IN ecodata
  loop
  
    modify_r_base(e.sdi,'day',e.datadate,NULL,e.val,7,overwrite_flag,valid,13,loading_app,18,1,'Y');
    rows_updated:=rows_updated+1;
  
    IF trunc(e.datadate,'YYYY') = e.datadate THEN
      COMMIT; -- commit at the end of every year
      --dbms_output.put_line(to_char(e.datadate,'YYYY'));
    END IF;
  END loop;

  COMMIT;
  RETURN rows_updated;

END; -- function ECOHDB_DATA_LOAD