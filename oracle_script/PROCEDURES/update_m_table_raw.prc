PROMPT CREATE OR REPLACE PROCEDURE update_m_table_raw;

CREATE OR REPLACE PROCEDURE update_m_table_raw (
        INTERVAL VARCHAR2,
        MODEL_RUN_ID_IN NUMBER,
        SITE_DATATYPE_ID_IN NUMBER,
        START_DATE_TIME_IN DATE,
        VALUE_IN FLOAT)
   IS

   BEGIN

/*    DENY_ACTION(to_char(MODEL_RUN_ID_IN)||to_char(SITE_DATATYPE_ID_IN)||INTERVAL||TO_CHAR(START_DATE_TIME_IN)||'  REC COUNT: assumed >0'); */

/* These are duplicated for speed, see comments in insert_m_table and modify_m_table */
     IF INTERVAL = 'hour' THEN
        UPDATE m_hour 
           set value = VALUE_IN
         where model_run_id = model_run_id_in and site_datatype_id = site_datatype_id_in and start_date_time = start_date_time_in;

     ELSIF INTERVAL = 'day' THEN 
        UPDATE m_day 
           set value = VALUE_IN
         where model_run_id = model_run_id_in and site_datatype_id = site_datatype_id_in and start_date_time = start_date_time_in;

     ELSIF INTERVAL = 'month' THEN 
        UPDATE m_month
           set value = VALUE_IN
         where model_run_id = model_run_id_in and site_datatype_id = site_datatype_id_in and start_date_time = start_date_time_in;

     ELSIF INTERVAL = 'wy' THEN 
        UPDATE m_wy
           set value = VALUE_IN
         where model_run_id = model_run_id_in and site_datatype_id = site_datatype_id_in and start_date_time = start_date_time_in;

     ELSIF INTERVAL = 'year' THEN 
        UPDATE m_year
           set value = VALUE_IN
         where model_run_id = model_run_id_in and site_datatype_id = site_datatype_id_in and start_date_time = start_date_time_in;

     END IF;
   END;
/

/* Permission not granted to anything on purpose. Only modify_m_table_raw should call this */
 show errors;
/


