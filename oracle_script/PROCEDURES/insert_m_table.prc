PROMPT CREATE OR REPLACE PROCEDURE insert_m_table;

CREATE OR REPLACE PROCEDURE insert_m_table (
        INTERVAL VARCHAR2,
        MODEL_RUN_ID NUMBER,
        SITE_DATATYPE_ID NUMBER,
        START_DATE_TIME DATE,
        END_DATE_TIME  DATE,
        VALUE FLOAT)
   IS

   BEGIN
/* Blecherous Nasty Duplicated Code 
   We could make this into native dynamic sql with a execute immediate ('insert into m_'||interval||' (model_run_id...')
   But I understand that would be slower. Slower than all these string comparisons, I expect? */
     IF INTERVAL = 'hour' THEN
             INSERT INTO m_hour (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE)
             VALUES (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE);
     ELSIF INTERVAL = 'day' THEN 
             INSERT INTO m_day (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE)
             VALUES (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE);
     ELSIF INTERVAL = 'month' THEN 
             INSERT INTO m_month (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE)
             VALUES (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE);
     ELSIF INTERVAL = 'wy' THEN 
             INSERT INTO m_wy (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE)
             VALUES (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE);
     ELSIF INTERVAL = 'year' THEN 
             INSERT INTO m_year (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE)
             VALUES (MODEL_RUN_ID, SITE_DATATYPE_ID, START_DATE_TIME, END_DATE_TIME, VALUE);
     END IF;
   END;
/
 show errors;
/

create public synonym insert_m_table for insert_m_table;
grant execute on insert_m_table to app_role;
grant execute on insert_m_table to savoir_faire;
grant execute on insert_m_table to model_role;

/
