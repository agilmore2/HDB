--------------------------------------------------------
--  DDL for Type NUMBER_ARRAY
--------------------------------------------------------

  CREATE OR REPLACE TYPE "ECODBA"."NUMBER_ARRAY" as table of number;

/

  GRANT EXECUTE ON "ECODBA"."NUMBER_ARRAY" TO PUBLIC;
  CREATE OR REPLACE PUBLIC SYNONYM "NUMBER_ARRAY" FOR "ECODBA"."NUMBER_ARRAY";

--------------------------------------------------------
--  DDL for Type DATEARRAY
--------------------------------------------------------

  CREATE OR REPLACE TYPE "ECODBA"."DATEARRAY" as table of date;

/

  GRANT EXECUTE ON "ECODBA"."DATEARRAY" TO PUBLIC;
  CREATE OR REPLACE PUBLIC SYNONYM "DATEARRAY" FOR "ECODBA"."DATEARRAY";
 


CREATE OR REPLACE PACKAGE          "TS_XFER" as
/*  PACKAGE TS_XFER is designed to contain fast methods for timeseries
    writing and retrieval

    Created by A. Gilmore October 2015
*/
	/* declare the associative array table types for this package   */

--TYPE number_array is table of NUMBER;
--TYPE date_array is table of DATE; -- overriding main date_array date object array


procedure GET_REAL_DATA 
(
  sdi IN NUMBER 
, start_date IN DATE 
, end_date IN DATE
, interval IN HDB_INTERVAL.INTERVAL_NAME%type 
, dates OUT datearray
, ts_values OUT number_array
, inst_interval IN NUMBER DEFAULT 15 --interval for r_instant data
);

procedure GET_MODEL_DATA
(
  sdi IN NUMBER 
, start_date IN DATE 
, end_date IN DATE
, interval in HDB_INTERVAL.INTERVAL_NAME%type
, dates OUT datearray
, ts_values OUT number_array
, mri in REF_MODEL_RUN.MODEL_RUN_ID%type
);

-- the above two do little or no validation, as it would repeat what is in get_data
-- calling the above directly will be a bit faster as they avoid sdi. interval.
-- and mri table lookups.

-- get_date
-- given site_datatype_id, begin and end dates, an interval,
--       optional real or modeled indicator, and timeseries interval in minutes
-- return a correlated array of dates and values containing all the dates in 
-- the range, and any missing values as null
--
-- checks for valid site_datatype_id, end date after begin date,
-- interval, and model_run_id if modeled data


procedure GET_DATA
(
  sdi IN NUMBER 
, start_date IN DATE 
, end_date IN DATE
, interval in HDB_INTERVAL.INTERVAL_NAME%type
, dates OUT datearray
, ts_values OUT number_array
, real_or_model IN VARCHAR2 default 'R_'
, mri_or_interval in NUMBER default 15
);

-- Write data procedures for 2016 HDB Support Task
-- initially written by Andrew Gilmore Dec 2016

procedure WRITE_REAL_DATA
(
  sdi IN NUMBER
, INTERVAL IN hdb_interval.interval_name%TYPE
, dates IN datearray
, ts_values IN number_array
, agen_id NUMBER
, overwrite_flag VARCHAR2
, VALIDATION CHAR
, COLLECTION_SYSTEM_ID NUMBER
, LOADING_APPLICATION_ID NUMBER
, METHOD_ID NUMBER
, computation_id NUMBER
, do_update_y_n VARCHAR2
, data_flags IN VARCHAR2 DEFAULT NULL
, TIME_ZONE IN VARCHAR2 DEFAULT NULL
);

procedure WRITE_MODEL_DATA
(
  sdi IN NUMBER
, INTERVAL IN hdb_interval.interval_name%TYPE
, dates IN datearray
, ts_values IN number_array
, model_run_id IN NUMBER
, do_update_y_n IN VARCHAR2
);


END TS_XFER;
/

  GRANT EXECUTE ON "TS_XFER" TO "SAVOIR_FAIRE";
  GRANT EXECUTE ON "TS_XFER" TO "APP_ROLE";
  CREATE OR REPLACE PUBLIC SYNONYM TS_XFER FOR TX_XFER;
--------------------------------------------------------
--  File created - Friday-October-09-2015   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for Package Body TS_XFER
--------------------------------------------------------

CREATE OR REPLACE PACKAGE BODY          "TS_XFER" as

procedure GET_REAL_DATA
(
  sdi IN NUMBER 
, start_date IN DATE 
, end_date IN DATE
, interval in HDB_INTERVAL.INTERVAL_NAME%type 
, dates OUT DATEARRAY
, ts_values OUT number_array
, inst_interval IN NUMBER DEFAULT 15 --interval for r_instant, r_other data
) is

  CURSOR instant (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select instants.date_time, r_instant.value from r_instant,
    table (instants_between(start_date,end_date, inst_interval)) instants
    where r_instant.site_datatype_id(+) = sdi and
    r_instant.start_date_time(+) = instants.date_time
    order by instants.date_time;

  CURSOR hour (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select hours.date_time, r_hour.value from r_hour,
    table (dates_between(start_date, end_date, 'hour')) hours
    where r_hour.site_datatype_id(+) = sdi and
    r_hour.start_date_time(+) = hours.date_time
   order by hours.date_time;

  CURSOR day (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select days.date_time, r_day.value from r_day,
    table (dates_between(start_date, end_date, 'day')) days
    where r_day.site_datatype_id(+) = sdi and
    r_day.start_date_time(+) = days.date_time
    order by days.date_time;

  CURSOR month (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select months.date_time, r_month.value from r_month,
    table (dates_between(start_date, end_date, 'month')) months
    where r_month.site_datatype_id(+) = sdi and
    r_month.start_date_time(+) = months.date_time
    order by months.date_time;

  CURSOR year (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select years.date_time, r_year.value from r_year,
    table (dates_between(start_date, end_date, 'year')) years
    where r_year.site_datatype_id(+) = sdi and
    r_year.start_date_time(+) = years.date_time
    order by years.date_time;

  CURSOR wy (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select wys.date_time, r_wy.value from r_wy,
    table (dates_between(start_date, end_date, 'wy')) wys
    where r_wy.site_datatype_id(+) = sdi and
    r_wy.start_date_time(+) = wys.date_time
    order by wys.date_time;

  CURSOR other (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select others.date_time, r_other.value from r_other,
    table (instants_between(start_date, end_date, inst_interval)) others
    where r_other.site_datatype_id(+) = sdi and
    r_other.start_date_time(+) = others.date_time
    order by others.date_time;    

--  val NUMBER;

  BEGIN

    CASE interval
    WHEN 'instant' THEN
      OPEN instant (sdi,start_date,end_date);
      FETCH instant BULK COLLECT into dates,ts_values;
      CLOSE instant;

    WHEN 'hour' THEN
      OPEN HOUR (sdi,start_date,end_date);
      FETCH HOUR BULK COLLECT into dates,ts_values;
      CLOSE HOUR;

    WHEN 'day' THEN
      OPEN day (sdi,start_date,end_date);
      FETCH day BULK COLLECT into dates,ts_values;
      CLOSE day;

    WHEN 'month' THEN
      OPEN month (sdi,start_date,end_date);
      FETCH month BULK COLLECT into dates,ts_values;
      CLOSE month;

    WHEN 'year' THEN
      OPEN year (sdi,start_date,end_date);
      FETCH year BULK COLLECT into dates,ts_values;
      CLOSE year;

    WHEN 'wy' THEN
      OPEN wy (sdi,start_date,end_date);
      FETCH wy BULK COLLECT into dates,ts_values;
      CLOSE wy;

    WHEN 'other' THEN
      OPEN other (sdi,start_date,end_date);
      FETCH other BULK COLLECT into dates,ts_values;
      CLOSE other;

    END CASE;

--testing   
--      val:=0;
--      for temp_num in 1..dates.count() loop
--        val:= val +1;
--      end loop;
--      deny_action('Numbers: '|| val);


  END GET_REAL_DATA;

procedure GET_MODEL_DATA
(
  sdi IN NUMBER 
, start_date IN DATE 
, end_date IN DATE
, interval IN HDB_INTERVAL.INTERVAL_NAME%type
, dates OUT DATEARRAY
, ts_values OUT number_array
, mri IN REF_MODEL_RUN.MODEL_RUN_ID%type
) is

  CURSOR hour (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select hours.date_time, m_hour.value from m_hour,
    table (dates_between(start_date, end_date, 'hour')) hours
    where m_hour.site_datatype_id(+) = sdi and
    m_hour.start_date_time(+) = hours.date_time and
    m_hour.model_run_id(+) = mri
    order by hours.date_time;

  CURSOR day (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select days.date_time, m_day.value from m_day,
    table (dates_between(start_date, end_date, 'day')) days
    where m_day.site_datatype_id(+) = sdi and
    m_day.start_date_time(+) = days.date_time and
    m_day.model_run_id(+) = mri
    order by days.date_time;

  CURSOR month (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select months.date_time, m_month.value from m_month,
    table (dates_between(start_date, end_date, 'month')) months
    where m_month.site_datatype_id(+) = sdi and
    m_month.start_date_time(+) = months.date_time and
    m_month.model_run_id(+) = mri
    order by months.date_time;

  CURSOR year (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select years.date_time, m_year.value from m_year,
    table (dates_between(start_date, end_date, 'year')) years
    where m_year.site_datatype_id(+) = sdi and
    m_year.start_date_time(+) = years.date_time and
    m_year.model_run_id(+) = mri
    order by years.date_time;

  CURSOR wy (sdi in NUMBER, start_date in DATE, end_date in DATE) is
    select wys.date_time, m_wy.value from m_wy,
    table (dates_between(start_date, end_date, 'wy')) wys
    where m_wy.site_datatype_id(+) = sdi and
    m_wy.start_date_time(+) = wys.date_time and
    m_wy.model_run_id(+) = mri
    order by wys.date_time; 

  BEGIN

    CASE interval
    WHEN 'instant' THEN
      deny_action('TS_XFER.GET_MODEL_DATA invalid INTERVAL, no m_ table for: ' || interval);
    WHEN 'other' THEN
      deny_action('TS_XFER.GET_MODEL_DATA invalid INTERVAL, no m_ table for: ' || interval);

    WHEN 'hour' THEN
      OPEN HOUR (sdi,start_date,end_date);
      FETCH HOUR BULK COLLECT into dates,ts_values;
      CLOSE HOUR;

    WHEN 'day' THEN
      OPEN day (sdi,start_date,end_date);
      FETCH day BULK COLLECT into dates,ts_values;
      CLOSE day;

    WHEN 'month' THEN
      OPEN month (sdi,start_date,end_date);
      FETCH month BULK COLLECT into dates,ts_values;
      CLOSE month;

    WHEN 'year' THEN
      OPEN year (sdi,start_date,end_date);
      FETCH year BULK COLLECT into dates,ts_values;
      CLOSE year;

    WHEN 'wy' THEN
      OPEN wy (sdi,start_date,end_date);
      FETCH wy BULK COLLECT into dates,ts_values;
      CLOSE wy;


    END CASE;

  END GET_MODEL_DATA;


procedure GET_DATA
(
  sdi IN NUMBER 
, start_date IN DATE 
, end_date IN DATE
, interval IN HDB_INTERVAL.INTERVAL_NAME%type
, dates OUT DATEARRAY
, ts_values OUT number_array
, real_or_model IN VARCHAR2 default 'R_'
, mri_or_interval IN NUMBER default 15 --
) is

  temp_num NUMBER;
  temp_inter HDB_INTERVAL.INTERVAL_NAME%type;

BEGIN
  -- validate inputs
  BEGIN
    SELECT site_datatype_id
    INTO temp_num
    FROM hdb_site_datatype
    WHERE site_datatype_id = sdi;
  EXCEPTION WHEN others THEN
    deny_action('TS_XFER.GET_DATA invalid SITE_DATATYPE_ID: ' || sdi);
  END;  

  if end_date < start_date then
    deny_action('TS_XFER.GET_DATA end_date must be before start_date: ' || start_date || ' ' || end_date);
  end if;

  BEGIN
    SELECT interval_name
    INTO temp_inter
    FROM hdb_interval
    WHERE interval_name = interval;

  EXCEPTION WHEN others THEN
    deny_action('TS_XFER.GET_DATA invalid INTERVAL: ' || interval);
  END;

  CASE real_or_model 
  WHEN 'R_' THEN
    GET_REAL_DATA (sdi, start_date, end_date, interval, dates, ts_values, mri_or_interval);
  WHEN 'M_' THEN
  -- validate inputs
    BEGIN
      SELECT model_run_id
      INTO temp_num
      FROM ref_model_run
      WHERE model_run_id = mri_or_interval; --default 15 might trip folks up here
    EXCEPTION WHEN others THEN
      deny_action('TS_XFER.GET_DATA invalid MODEL_RUN_ID: ' || mri_or_interval);
    END;

    GET_MODEL_DATA (sdi, start_date, end_date, interval, dates, ts_values, mri_or_interval);
  ELSE
    deny_action('Invalid real_or_model selector: ' || real_or_model);
  END CASE;

END GET_DATA;

PROCEDURE write_real_data
(
  sdi IN NUMBER
, INTERVAL IN hdb_interval.interval_name%TYPE
, dates IN DATEARRAY
, ts_values IN number_array
, agen_id NUMBER
, overwrite_flag VARCHAR2
, VALIDATION CHAR
, COLLECTION_SYSTEM_ID NUMBER
, LOADING_APPLICATION_ID NUMBER
, METHOD_ID NUMBER
, computation_id NUMBER
, do_update_y_n VARCHAR2
, data_flags IN VARCHAR2 DEFAULT NULL
, TIME_ZONE IN VARCHAR2 DEFAULT NULL
) IS
   items       NUMBER;
  -- validate inputs
BEGIN
  IF dates.count() != ts_values.count() THEN
     deny_action('TS_XFER.WRITE_REAL_DATA arrays must contain equal number of items!');
  END IF;

  begin  /* begin block for HDB stored Procedures exceptions */
			items := dates.count();
			FOR i IN 1..items loop	    
					modify_r_base (sdi,INTERVAL,dates(i),NULL,ts_values(i),agen_id,
          overwrite_flag,validation,collection_system_id,loading_application_id,
					method_id,computation_id,do_update_y_n, data_flags, time_zone);
			END loop;
  end;
END write_real_data;       

PROCEDURE write_model_data
(
  sdi IN NUMBER
, INTERVAL IN hdb_interval.interval_name%TYPE
, dates IN DATEARRAY
, ts_values IN number_array
, model_run_id IN NUMBER
, do_update_y_n IN VARCHAR2
) IS
   items       NUMBER;
BEGIN
  -- validate inputs, most validations are performed by modify_ procedures
  -- potential improvement to validate once instead for every row?
  IF dates.count() != ts_values.count() THEN
     deny_action('TS_XFER.WRITE_MODEL_DATA arrays must contain equal number of items!');
  END IF;

  begin  /* begin block for HDB stored Procedures exceptions */
			items := dates.count();
			FOR i IN 1..items loop	    
					modify_m_table(model_run_id,sdi,dates(i),null,ts_values(i),INTERVAL,do_update_y_n);
			END loop;
  end;  
END write_model_data;

END ts_xfer;
/

DROP TYPE t_tf_tab;
DROP TYPE t_tf_row;


CREATE TYPE t_tf_row AS OBJECT (
  row_dates DATE,
  row_values NUMBER
);
/

CREATE TYPE t_tf_tab IS TABLE OF t_tf_row;
/



-- Build a pipelined table function.
CREATE OR REPLACE FUNCTION get_ts_xfer_data (p_sdi IN NUMBER,p_sdate IN date,p_edate IN date,p_interval IN VARCHAR2,p_REAL_OR_MODEL IN VARCHAR2,p_MRI_OR_INTERVAL IN number) RETURN t_tf_tab PIPELINED AS

  SDI NUMBER;
  START_DATE DATE;
  END_DATE DATE;
  INTERVAL VARCHAR2(16);
  DATES DATEARRAY;
  TS_VALUES NUMBER_ARRAY;
  REAL_OR_MODEL VARCHAR2(200);
  MRI_OR_INTERVAL NUMBER;
BEGIN
  SDI := p_sdi;
  START_DATE := p_sdate; 
  END_DATE := p_edate;
  INTERVAL := p_interval;
  REAL_OR_MODEL := p_REAL_OR_MODEL;
  MRI_OR_INTERVAL := p_MRI_OR_INTERVAL;

  TS_XFER.GET_DATA(
    SDI => SDI,
    START_DATE => START_DATE,
    END_DATE => END_DATE,
    INTERVAL => INTERVAL,
    DATES => DATES,
    TS_VALUES => TS_VALUES,
    REAL_OR_MODEL => REAL_OR_MODEL,
    MRI_OR_INTERVAL => MRI_OR_INTERVAL
  );
 
FOR indx IN 1 .. DATES.COUNT 
        LOOP
    --DBMS_OUTPUT.PUT_LINE(DATES(indx) || ' ' || TS_VALUES(indx));
    PIPE ROW(t_tf_row(DATES(indx), TS_VALUES(indx)));  
        END LOOP;
  RETURN;
END;
/

  GRANT EXECUTE ON "GET_TS_XFER_DATA" TO "SAVOIR_FAIRE";
  GRANT EXECUTE ON "GET_TS_XFER_DATA" TO "APP_ROLE";
  CREATE OR REPLACE PUBLIC SYNONYM GET_TS_XFER_DATA FOR GET_TS_XFER_DATA;

