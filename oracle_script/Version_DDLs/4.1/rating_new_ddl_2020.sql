
--drop below PK triggers, if exist as we will use sequences
drop trigger REF_SITE_RATING_PK_TRIG;
drop trigger REF_RATING_TABLE_PK_TRIG;


--create sequences to replace PK triggers
--Change START WITH value  for dbs that has existing RATINGS with Max(Rating_id)+1
-- Select max(rating_id) from ref_site_rating;

CREATE SEQUENCE REF_SITE_RATING_SEQ  MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 NOCACHE ORDER NOCYCLE;
CREATE SEQUENCE REF_RATING_TABLE_SEQ MINVALUE 1 MAXVALUE 999999999999999999999999999 INCREMENT BY 1 START WITH 1 NOCACHE ORDER NOCYCLE;


--create new ref_rating_table Table 
create table ref_rating_table (                                  
RATING_ID NUMBER NOT NULL ENABLE, 
DESCRIPTION VARCHAR2(1000 BYTE),
CONSTRAINT "REF_RATING_TABLE_PK" PRIMARY KEY ("RATING_ID")
)                                                             
pctfree 10
pctused 40
tablespace HDB_data
storage (initial 100k
         next 100k
         pctincrease 0);


--create new ref_rating_table_archive Table 
create table ref_rating_table_archive (                                  
RATING_ID NUMBER NOT NULL ENABLE,            
DESCRIPTION VARCHAR2(1000),           
ARCHIVE_REASON		       VARCHAR2(10)     NOT NULL ENABLE,  
DATE_TIME_ARCHIVED	       DATE             NOT NULL ENABLE,        
ARCHIVE_CMMNT		       VARCHAR2(1000)           
)                                                             
pctfree 10
pctused 40
tablespace HDB_data
storage (initial 100k
         next 100k
         pctincrease 0);



-- Populate ref_rating_table with ref_site_rating existing rating_id & descriptions values, if needed
INSERT INTO ref_rating_table (rating_id, description)
SELECT rating_id,description FROM ref_site_rating;



--main rating tables modifications
ALTER TABLE REF_SITE_RATING ADD ("SITE_RATING_ID" NUMBER(38,0));
ALTER TABLE REF_SITE_RATING DROP ("DESCRIPTION");
ALTER TABLE REF_SITE_RATING MODIFY ("RATING_ID" NUMBER(38,0));
ALTER TABLE REF_SITE_RATING MODIFY ("RATING_ID" NULL);

alter trigger REF_SITE_RATING_ARCH_UPD disable;
alter trigger REF_SITE_RATING_ARCH_DEL disable;
update REF_SITE_RATING set site_rating_id=rating_id;
commit;
alter trigger REF_SITE_RATING_ARCH_UPD enable;
alter trigger REF_SITE_RATING_ARCH_DEL enable;

alter table REF_RATING disable  constraint REF_RATING_REF_SDI_RATING_FK1;
alter table REF_RATING drop constraint REF_RATING_REF_SDI_RATING_FK1;
ALTER TABLE REF_SITE_RATING DROP CONSTRAINT REF_SITE_RATING_PK;
drop index REF_SITE_RATING_PK;
ALTER TABLE REF_SITE_RATING ADD CONSTRAINT REF_SITE_RATING_PK PRIMARY KEY ("SITE_RATING_ID") ENABLE;
ALTER TABLE REF_SITE_RATING ADD CONSTRAINT "REF_SITE_RATING_ID_FK1" FOREIGN KEY ("RATING_ID") REFERENCES REF_RATING_TABLE("RATING_ID") ENABLE;
ALTER TABLE REF_SITE_RATING ADD CONSTRAINT "REF_SITE_RATING_ID_CHECK" CHECK ("RATING_ID" IS NOT NULL) ENABLE;
alter table REF_RATING  add constraint REF_RATING_REF_SDI_RATING_FK1 foreign key("RATING_ID") references REF_RATING_TABLE("RATING_ID");
alter table REF_RATING enable   constraint REF_RATING_REF_SDI_RATING_FK1;

--ref_site_rating_archive table modifications
ALTER TABLE REF_SITE_RATING_ARCHIVE ADD ("SITE_RATING_ID" NUMBER NOT NULL ENABLE);
ALTER TABLE REF_SITE_RATING_ARCHIVE DROP ("DESCRIPTION");
ALTER TABLE REF_SITE_RATING_ARCHIVE MODIFY ("RATING_ID" NULL);


--update ref_site_rating_arch triggers
create or replace TRIGGER ref_site_rating_arch_upd
after update on ref_site_rating for each row begin insert into ref_site_rating_archive (
SITE_RATING_ID,
INDEP_SITE_DATATYPE_ID,
RATING_TYPE_COMMON_NAME,
EFFECTIVE_START_DATE_TIME,
EFFECTIVE_END_DATE_TIME,
DATE_TIME_LOADED,
AGEN_ID,
RATING_ID,
ARCHIVE_REASON, DATE_TIME_ARCHIVED, ARCHIVE_CMMNT) values (
:old.SITE_RATING_ID,
:old.INDEP_SITE_DATATYPE_ID,
:old.RATING_TYPE_COMMON_NAME,
:old.EFFECTIVE_START_DATE_TIME,
:old.EFFECTIVE_END_DATE_TIME,
:old.DATE_TIME_LOADED,
:old.AGEN_ID,
:old.RATING_ID,
'UPDATE', sysdate, NULL); end;


create or replace TRIGGER ref_site_rating_arch_del
after delete on ref_site_rating for each row begin insert into ref_site_rating_archive (
SITE_RATING_ID,
INDEP_SITE_DATATYPE_ID,
RATING_TYPE_COMMON_NAME,
EFFECTIVE_START_DATE_TIME,
EFFECTIVE_END_DATE_TIME,
DATE_TIME_LOADED,
AGEN_ID,
RATING_ID,
ARCHIVE_REASON, DATE_TIME_ARCHIVED, ARCHIVE_CMMNT) values (
:old.SITE_RATING_ID,
:old.INDEP_SITE_DATATYPE_ID,
:old.RATING_TYPE_COMMON_NAME,
:old.EFFECTIVE_START_DATE_TIME,
:old.EFFECTIVE_END_DATE_TIME,
:old.DATE_TIME_LOADED,
:old.AGEN_ID,
:old.RATING_ID,
'DELETE', sysdate, NULL); end;




--Public synonyms
CREATE OR REPLACE PUBLIC SYNONYM ref_rating_table_archive FOR ref_rating_table_archive;
CREATE OR REPLACE PUBLIC SYNONYM ref_rating_table FOR ref_rating_table;
CREATE OR REPLACE PUBLIC SYNONYM REF_SITE_RATING_SEQ FOR REF_SITE_RATING_SEQ;
CREATE OR REPLACE PUBLIC SYNONYM REF_RATING_TABLE_SEQ FOR ref_rating_table;


--Grants to Public
GRANT SELECT ON  ref_rating_table_archive to PUBLIC;
GRANT SELECT ON  ref_rating_table to PUBLIC;
GRANT SELECT ON  REF_SITE_RATING_SEQ to PUBLIC;
GRANT SELECT ON  REF_RATING_TABLE_SEQ to PUBLIC;

--Grants to REF_META_ROLE
GRANT SELECT,UPDATE,INSERT,DELETE ON ref_rating_table TO REF_META_ROLE;

commit;


--Update ref_rating_table_arch TRIGGERS
create or replace trigger ref_rating_table_arch_update                                                                    
after update on ref_rating_table 
for each row 
begin 
insert into ref_rating_table_archive (  
RATING_ID,
DESCRIPTION,
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) 
values (                                           
:old.RATING_ID,                                                                                              
:old.DESCRIPTION,                                                                                                                                                                                                                                                                                             
'UPDATE', 
sysdate, 
coalesce(
          sys_context('APEX$SESSION','app_user')
          ,sys_context('userenv','session_user')
         )
); 
end;                                                                    
                                                                                                                      
show errors trigger ref_rating_table_arch_update;                                                                         

                                                                                                                        
create or replace trigger ref_rating_table_arch_delete                                                                    
after delete on ref_rating_table 
for each row 
begin 
insert into ref_rating_table_archive (                     
RATING_ID,
DESCRIPTION,
ARCHIVE_REASON, 
DATE_TIME_ARCHIVED, 
ARCHIVE_CMMNT) values (                                           
:old.RATING_ID,                                                                                              
:old.DESCRIPTION,                                                                                                 
'DELETE', 
sysdate, 
coalesce(
          sys_context('APEX$SESSION','app_user')
          ,sys_context('userenv','session_user')
         )
); 
end;                                                                    
                                                                                                                      
show errors trigger ref_rating_table_arch_delete;      



--Update FIND_RATING FUNCTION
CREATE OR REPLACE FUNCTION FIND_RATING 
( rating_type in varchar2
, indep_sdi in number
, value_date_time in date default null
) return number is
rating number;
begin

  select rating_id into rating
  from ref_site_rating
  where
  indep_site_datatype_id = indep_sdi and
  rating_type_common_name = rating_type and
  value_date_time between effective_start_date_time and effective_end_date_time;

  if rating is null
  then rating := -1;
  end if;
  
  return rating;
  
end find_rating;


---------------------------
--Changed PACKAGE
--RATINGS
---------------------------
create or replace PACKAGE ratings AS

/**
 * Ratings package
 * connects to four tables: hdb_rating_algorithm, hdb_rating_type,
 * ref_site_rating and ref_rating
 * Currently only supports 2-dimension x to y ratings
 * 3 dimension (x,y to z) ratings should
 * use two more tables, ref_site_3d_rating, and ref_3d_rating
 * also would require do_3d_rating, find_site_3d_rating, rating_3d_linear,
 * rating_3d_logarithmic algorithms and so forth.
*/


/** do_rating: actually does a rating. Must have previously looked up a rating_id
 * this procedure will use the rating algorithm defined in the hdb_rating_type
 * for the specified rating_id
 * Inputs:  rating_in - rating id from ref_site_rating
          indep_value - the independent value being rated
  Outputs: indep_base - the lower bound on indep_value found in the table
            dep_value - the y value resulting from the rating
          match_check - status of rating. Null, E for exact, A for extrapolation
          above maximum value, or B for extrapolation below min value.
*/

  PROCEDURE do_rating(rating_in in number,
  indep_value in number,
  indep_date in date,
  indep_base out number,
  dep_value out number,
  match_check out nocopy varchar2);

/* deletes all rows in ref_rating for a specific rating_id */

  procedure delete_rating_points(rating_in number);

/* creates a ref_site_rating entry */

  procedure create_site_rating(indep_site_datatype_id in number,
  rating_type_common_name in varchar2,
  effective_start_date_time in date,
  effective_end_date_time in date,
  agen_id in number,
  description in varchar2, RATING_ID IN NUMBER DEFAULT NULL);

/* finds the minimum and maximum independent values for a specific rating_id */

  procedure find_rating_limits (rating_in in number,
  min_value out number,
  max_value out number);

/* finds the bounding independent and dependent values for a specified value and
  rating_id, as well as the same codes as do_rating for match_check */

  procedure find_rating_points (rating_in in number,
  indep_value in number,
  x1 out number,
  x2 out number,
  y1 out number,
  y2 out number,
  match_check out nocopy varchar2);

/*
Function to return a rating id if one exists that matches the input parameters
indep_sdi and rating_type are required
value_date_time is optional, and defaults to null
if it is null, ratings will only match if both start and end are null
if eff start or end are not null, they must be before or after value_date
respectively.
There is nothing in ref_site_rating prohibiting overlapping ratings.
Use of this function for time interpolated ratings is problematic because in that
case, we want two ratings, one from before, and one from after the date.

That case is currently left for applications to handle.
The most likely way to implement that is to put all ratings for time interpolation
in the database with  start and end effective dates as instants in time, then
query for the two ratings that span the value date of interest. Then do separate ratings
using those two rating ids, and then do time interpolation between them.
*/

  function find_site_rating( rating_type in varchar2,
  indep_sdi in number,
  value_date_time in date)
  return ref_site_rating.rating_id%type;

/* alters a single row in ref_rating, good candiate for a merge statement, but
have not used that yet.*/

  procedure modify_rating_point (rating_in in number,
  indep_value in number,
  dep_value in number);

/*
does a linear interpolation:
if an exact match is found, returns.
assumes the same equation for extrapolation as for interpolation
*/
  procedure rating_linear (rating_in in number,
      indep_value in number,
      indep_date in date,
      indep_base out number,
      dep_value out number,
      match_check out nocopy varchar2);

/*
LOG-LOG algorithm after Hydromet loglog and GSLOGLOG algorithms
Shift and offset are not applied here.
This function does no rounding on output, so it is likely to have many significant
figures (15+)

mathematic exceptions like log of a negative number cause exceptions just like other errors
*/
  procedure rating_logarithm (rating_in in number,
      indep_value in number,
      indep_date in date,
      indep_base out number,
      dep_value out number,
      match_check out nocopy varchar2);

/* want the row matching or immediately below the input
 * makes no sense to handle table bounds here,
 * so we just return B if the input x value is below the lowest number.
 * this should checked by the calling code
 */

  procedure rating_lookup (rating_in in number,
      indep_value in number,
      indep_date in date,
      indep_base out number,
      dep_value out number,
      match_check out nocopy varchar2);

/*
After Hydromet semilogx algorithm
Shift and offset are not applied here.
This function does no rounding on output, so it is likely to have many significant
figures (15+)
*/

  procedure rating_semilogx (rating_in in number,
      indep_value in number,
      indep_date in date,
      indep_base out number,
      dep_value out number,
      match_check out nocopy varchar2);

  PROCEDURE rating_time_interp_lookup(rating_in IN NUMBER,
      indep_value IN NUMBER,
      indep_date IN date,
      indep_base out number,
      dep_value OUT NUMBER,
      match_check OUT nocopy VARCHAR2);

   PROCEDURE rating_time_interp_linear(rating_in IN NUMBER,
      indep_value IN NUMBER,
      indep_date IN date,
      indep_base out number,
      dep_value OUT NUMBER,
      match_check OUT nocopy VARCHAR2);

/* alters the description field in ref_site_rating for a specified rating_id*/

  procedure update_rating_desc ( rating_in in number,
  description_in in varchar2);
  
/* Find rating from ref_rating_table  */
  function find_rating_table( rating_in in number)
  return ref_rating_table.rating_id%type;


/* Find rating from ref_rating  */
  function find_rating( rating_in in number)
  return ref_rating.rating_id%type;


END ratings;


---------------------------
--Changed PACKAGE BODY
--RATINGS
---------------------------
create or replace PACKAGE BODY ratings
AS

  PROCEDURE 
  create_site_rating(indep_site_datatype_id IN NUMBER,
  rating_type_common_name IN VARCHAR2,
  effective_start_date_time IN DATE,
  effective_end_date_time IN DATE,
  agen_id IN NUMBER,
  description IN VARCHAR2,
  RATING_ID IN NUMBER DEFAULT NULL)
  AS
  max_rating_id NUMBER;
  find_rating_v ref_rating.rating_id%type;
  find_rating_table_v ref_rating_table.rating_id%type;
  v_ref_rating_table_seq NUMBER;
  v_ref_site_rating_seq NUMBER;
  BEGIN

    IF(indep_site_datatype_id IS NULL) THEN
      deny_action('Invalid <NULL> indep_site_datatype_id');
    ELSIF(rating_type_common_name IS NULL) THEN
      deny_action('Invalid <NULL> rating_type_common_name');
    ELSIF(agen_id IS NULL) THEN
      deny_action('Invalid <NULL> agen_id');
    END IF;


IF(RATING_ID IS NULL) THEN

 v_ref_rating_table_seq  := ref_rating_table_seq.nextval;
 v_ref_site_rating_seq := REF_SITE_RATING_SEQ.nextval;
 
   Insert into ref_rating_table ( rating_id,description ) values ( v_ref_rating_table_seq ,description);

    INSERT
    INTO ref_site_rating(site_rating_id, indep_site_datatype_id,
      rating_type_common_name, effective_start_date_time,
      effective_end_date_time, date_time_loaded,
      agen_id, rating_id)
    VALUES(v_ref_site_rating_seq, indep_site_datatype_id,
      rating_type_common_name, effective_start_date_time,
      effective_end_date_time, sysdate,
      agen_id, v_ref_rating_table_seq);
ELSE

 v_ref_site_rating_seq := REF_SITE_RATING_SEQ.nextval;

 select find_rating(rating_id)
    into find_rating_v
    from dual;
    
 select find_rating_table(rating_id)
    into find_rating_table_v
    from dual;

    if (find_rating_v  is not null AND find_rating_table_v is not null) then
    
    INSERT
    INTO ref_site_rating(site_rating_id, indep_site_datatype_id,
      rating_type_common_name, effective_start_date_time,
      effective_end_date_time, date_time_loaded,
      agen_id, rating_id)
    VALUES(v_ref_site_rating_seq, indep_site_datatype_id,
      rating_type_common_name, effective_start_date_time,
      effective_end_date_time, sysdate,
      agen_id, rating_id);
      
      ELSE      
      deny_action('Invalid non-existing rating_id ' || rating_id );      
      END IF;
END IF;

  END create_site_rating;

  PROCEDURE delete_rating_points(rating_in NUMBER)
  AS
  v_count NUMBER;
  BEGIN

    IF(rating_in IS NULL) THEN
      deny_action('Invalid <NULL> rating_id');
    END IF;

    SELECT COUNT(*)
    INTO v_count
    FROM ref_site_rating
    WHERE rating_id = rating_in;

    IF v_count = 0 THEN
      deny_action('Invalid non-existing rating_id ' ||
      rating_in || ' attempted for deletion of table');
    END IF;

    DELETE FROM ref_rating
    WHERE rating_id = rating_in;

  END delete_rating_points;

  PROCEDURE do_rating(rating_in IN NUMBER,
  indep_value IN NUMBER,
  indep_date IN DATE,
  indep_base OUT NUMBER,
  dep_value OUT NUMBER,
  match_check OUT nocopy VARCHAR2)
  AS
  algorithm hdb_rating_type.rating_algorithm%TYPE;
  sqlstmt VARCHAR2(100);
  BEGIN

    IF(rating_in IS NULL) THEN
      deny_action('Invalid <NULL> rating_id');
    END IF;

    IF(indep_value IS NULL) THEN
      deny_action('Invalid <NULL> independent value');
    END IF;

    BEGIN
      SELECT procedure_name
      INTO algorithm
      FROM hdb_rating_algorithm a,
        hdb_rating_type b,
        ref_site_rating c
      WHERE a.rating_algorithm = b.rating_algorithm
       AND b.rating_type_common_name = c.rating_type_common_name
       AND c.rating_id = rating_in;

    EXCEPTION
    WHEN others THEN
      deny_action('Unable to find algorithm for rating ' || rating_in);
    END;

    /* native dynamic SQL call to the procedure named in the hdb_rating_algorithm table */
    sqlstmt := 'begin ratings.' || algorithm || '(:2, :3, :4, :5, :6, :7); end;';

    EXECUTE IMMEDIATE(sqlstmt) USING IN rating_in,
      IN indep_value,
      IN indep_date,
      OUT indep_base,
      OUT dep_value,
      OUT match_check;

  END do_rating;

  PROCEDURE find_rating_limits(rating_in IN NUMBER,
  min_value OUT NUMBER,
  max_value OUT NUMBER)
  AS
  v_count NUMBER;
  BEGIN
    SELECT COUNT(*),
      MIN(independent_value),
      MAX(independent_value)
    INTO v_count,
      min_value,
      max_value
    FROM ref_rating
    WHERE rating_id = rating_in;

    IF(v_count = 0) THEN
      deny_action('Rating_id ' ||
      rating_in || ' does not have any rows in rating table');
    END IF;

  END find_rating_limits;

  PROCEDURE find_rating_points(rating_in IN NUMBER,
  indep_value IN NUMBER,
  x1 OUT NUMBER,
  x2 OUT NUMBER,
  y1 OUT NUMBER,
  y2 OUT NUMBER,
  match_check OUT nocopy VARCHAR2)
  AS
  v_count NUMBER;
  min_value NUMBER;
  max_value NUMBER;
  x_search_above NUMBER;
  x_search_below NUMBER;
  BEGIN

    /* check input */

    IF(rating_in IS NULL) THEN
      deny_action('Invalid <NULL> rating_id');
    END IF;

    IF(indep_value IS NULL) THEN
      deny_action('Invalid <NULL> independent value');
    END IF;

    /* search points */
    x_search_above := indep_value;
    x_search_below := indep_value;

    /* check for exact match */
    SELECT COUNT(*)
    INTO v_count
    FROM ref_rating
    WHERE rating_id = rating_in
     AND independent_value = indep_value;

    IF(v_count > 0) THEN

      /*exact, return two copies of same data */
      match_check := 'E';
      x1 := indep_value;
      x2 := indep_value;

      SELECT dependent_value,
        dependent_value
      INTO y1,
        y2
      FROM ref_rating
      WHERE rating_id = rating_in
       AND independent_value = indep_value;

      RETURN;
      -- unreachable?
    END IF;

    /* non exact, find outer limits of table */
    find_rating_limits(rating_in, min_value, max_value);

    IF(indep_value < max_value) THEN  /* must search for x2 */
      IF(indep_value < min_value) THEN

        /* x1 is min_value, search above for x2 */
        match_check := 'B';
        x1 := min_value;
        x_search_above := min_value;
      END IF;

      SELECT MIN(independent_value)
      INTO x2
      FROM ref_rating
      WHERE rating_id = rating_in
       AND independent_value > x_search_above;
    END IF;

    IF(indep_value > min_value) THEN /* must search for x1 */
      IF(indep_value > max_value) THEN

        /* x2 is max value, search below for x1 */
        match_check := 'A';
        x2 := max_value;
        x_search_below := max_value;
      END IF;

      SELECT MAX(independent_value)
      INTO x1
      FROM ref_rating
      WHERE rating_id = rating_in
       AND independent_value < x_search_below;
    END IF;

    /* get y values for resulting x */
    SELECT dependent_value
    INTO y1
    FROM ref_rating
    WHERE rating_id = rating_in
     AND independent_value = x1;

    SELECT dependent_value
    INTO y2
    FROM ref_rating
    WHERE rating_id = rating_in
     AND independent_value = x2;

  END find_rating_points;

  FUNCTION find_site_rating(rating_type IN VARCHAR2,
  indep_sdi IN NUMBER,
  value_date_time IN DATE)
  RETURN ref_site_rating.rating_id%TYPE
  AS
  rating ref_site_rating.rating_id%TYPE;
  duprating ref_site_rating.rating_id%TYPE;
  v_count NUMBER;

  CURSOR c1 IS
  SELECT rating_id
  INTO rating
  FROM ref_site_rating
  WHERE indep_site_datatype_id = indep_sdi
   AND rating_type_common_name = rating_type
   AND((effective_start_date_time IS NULL
        AND effective_end_date_time IS NULL)
      OR (value_date_time >= effective_start_date_time
          AND effective_end_date_time IS NULL)
      OR (value_date_time <= effective_end_date_time
          AND effective_start_date_time IS NULL)
      OR (value_date_time >= effective_start_date_time
          AND value_date_time < effective_end_date_time));

  BEGIN

  /*procedure from O'Reilly PL/SQL programming
  determine if more than one rating matches
  if so, we have a problem
  */

    OPEN c1;
    FETCH c1
    INTO rating;

    IF c1 % NOTFOUND THEN
      CLOSE c1;
      RETURN NULL;

      /*no rating at all matched*/
    ELSE
      FETCH c1
      INTO duprating;

      IF c1 % NOTFOUND THEN
        CLOSE c1;
        RETURN rating;
      ELSE
        /* more than one match!*/
        CLOSE c1;
        deny_action('More than one rating matched input!');
        RETURN NULL;
      END IF;
    END IF;

  END find_site_rating;

  PROCEDURE modify_rating_point(rating_in IN NUMBER,
  indep_value IN NUMBER,
  dep_value IN NUMBER)
  AS
  v_count NUMBER;
  BEGIN
    SELECT COUNT(*)
    INTO v_count
    FROM ref_rating
    WHERE rating_id = rating_in
     AND independent_value = indep_value;

    IF v_count = 0 THEN
      INSERT
      INTO ref_rating(rating_id, independent_value, dependent_value)
      VALUES(rating_in, indep_value, dep_value);
    ELSE
      UPDATE ref_rating
      SET dependent_value = dep_value
      WHERE rating_id = rating_in
       AND independent_value = indep_value;
    END IF;

  END modify_rating_point;

  PROCEDURE rating_linear(rating_in IN NUMBER,
  indep_value IN NUMBER,
  indep_date IN DATE,
  indep_base OUT NUMBER,
  dep_value OUT NUMBER,
  match_check OUT nocopy VARCHAR2)
  AS
  x1 NUMBER;
  x2 NUMBER;
  y1 NUMBER;
  y2 NUMBER;
  BEGIN

    IF(rating_in IS NULL) THEN
      deny_action('Invalid <NULL> rating_id');
    END IF;

    BEGIN
      find_rating_points(rating_in,
        indep_value, x1, x2, y1, y2, match_check);

      indep_base := x1;

      IF(match_check = 'E') THEN
        dep_value := y1;
      ELSE
        dep_value := y1 + (y2 -y1)/(x2 -x1)*(indep_value -x1);
      END IF;

    EXCEPTION
    WHEN others THEN
      deny_action('Unable to complete linear rating for rating ' ||
        rating_in || ' value ' || indep_value);
    END;

  END rating_linear;

  PROCEDURE rating_logarithm(rating_in IN NUMBER,
  indep_value IN NUMBER,
  indep_date IN DATE,
  indep_base OUT NUMBER,
  dep_value OUT NUMBER,
  match_check OUT nocopy VARCHAR2)
  AS
  x1 NUMBER;
  x2 NUMBER;
  y1 NUMBER;
  y2 NUMBER;
  dy NUMBER;
  dx NUMBER;
  x NUMBER;
  BEGIN

    IF(rating_in IS NULL) THEN
      deny_action('Invalid <NULL> rating_id');
    END IF;

    BEGIN
    
  -- This first outer IF statement added by Ismail Ozdemir on 08/20/2010
  -- to check if the shiftedGH <=0 , the flow is 0.  
  -- Another if statement to check unshifted GH=0 then flow=0 is 
  -- added in uc_algorithms.jar file
  
 IF(indep_value <= 0) THEN
 dep_value :=0;
 ELSE
      find_rating_points(rating_in,
      indep_value, x1, x2, y1, y2, match_check);

      indep_base := x1;

      IF (y1 = 0 OR y2 = 0 or x1 = 0 or x2 = 0 OR indep_value = 0) THEN
        deny_action('Unable to complete logarithmic rating for rating ' ||
      rating_in || ', rating points = 0 are not usable in logarithmic rating, value ' || indep_value);
      END IF;

      IF(match_check = 'E') THEN
        dep_value := y1;
      ELSE
        y1 := LOG(10,   y1);
        y2 := LOG(10,   y2);
        x1 := LOG(10,   x1);
        x2 := LOG(10,   x2);
        x := LOG(10,   indep_value);
        dy :=(y2 -y1);
        dx :=(x2 -x1);

        dep_value := POWER(10,   y1 + dy / dx *(x -x1));
      END IF;
 END IF;
 
    EXCEPTION
    WHEN others THEN
      deny_action('Unable to complete logarithmic rating for rating ' ||
      rating_in || ' value ' || indep_value);
    END;

  END rating_logarithm;

  PROCEDURE rating_lookup(rating_in IN NUMBER,
  indep_value IN NUMBER,
  indep_date IN DATE,
  indep_base OUT NUMBER,
  dep_value OUT NUMBER,
  match_check OUT nocopy VARCHAR2)
  AS
  BEGIN

    IF(rating_in IS NULL) THEN
      deny_action('Invalid <NULL> rating_id');
    END IF;

    BEGIN
      SELECT independent_value,
        dependent_value
      INTO indep_base,
        dep_value
      FROM ref_rating
      WHERE rating_id = rating_in
       AND independent_value =
        (SELECT MAX(independent_value)
         FROM ref_rating
         WHERE rating_id = rating_in
         AND independent_value <= indep_value);

    EXCEPTION
    WHEN no_data_found THEN
      match_check := 'B';
    WHEN others THEN
      deny_action('Unable to complete lookup rating for rating ' ||
      rating_in || ' value ' || indep_value);
    END;

  END rating_lookup;

  PROCEDURE rating_semilogx(rating_in IN NUMBER,
  indep_value IN NUMBER,
  indep_date IN DATE,
  indep_base OUT NUMBER,
  dep_value OUT NUMBER,
  match_check OUT nocopy VARCHAR2)
  AS
  x1 NUMBER;
  x2 NUMBER;
  y1 NUMBER;
  y2 NUMBER;
  dy NUMBER;
  dx NUMBER;
  x NUMBER;
  BEGIN

    IF(rating_in IS NULL) THEN
      deny_action('Invalid <NULL> rating_id');
    END IF;

    BEGIN
      find_rating_points(rating_in,
      indep_value, x1, x2, y1, y2, match_check);

      IF (x1 = 0 or x2 = 0 or indep_value = 0) THEN
        deny_action('Unable to complete logarithmic rating for rating ' ||
      rating_in || ', rating points = 0 are not usable in logarithmic rating, value ' || indep_value);
      END IF;
      
      indep_base := x1;

      IF(match_check = 'E') THEN
        dep_value := y1;
      ELSE
        x1 := LOG(10,   x1);
        x2 := LOG(10,   x2);
        x := LOG(10,   indep_value);
        dy :=(y2 -y1);
        dx :=(x2 -x1);

        dep_value := y1 + dy / dx *(x -x1);
      END IF;

    EXCEPTION
    WHEN others THEN

      deny_action('Unable to complete logarithmic rating for rating ' ||
      rating_in || ' value ' || indep_value);
    END;

  END rating_semilogx;

  PROCEDURE rating_time_interp_lookup(rating_in IN NUMBER,
  indep_value IN NUMBER,
  indep_date IN DATE,
  indep_base out NUMBER,
  dep_value OUT NUMBER,
  match_check OUT nocopy VARCHAR2)
  AS
/** Function to do a time interpolated rating. The algorithm:
   Do a lookup rating with this rating table.
   See if a rating exists for the time period after this rating id.
   If it does exist, then perform a lookup rating with that table.
   Interpolate between the two results in time, and return that value
  */
  sdi ref_site_rating.indep_site_datatype_id%type;
  rating ref_site_rating.rating_type_common_name%type;
  sdate ref_site_rating.effective_start_date_time%type;
  edate ref_site_rating.effective_end_date_time%type;
  after_rating ref_site_rating.rating_id%type;
  after_dep_value NUMBER;
  x1 date;
  x2 date;
  y1 NUMBER;
  y2 NUMBER;
  BEGIN

    rating_lookup(rating_in,indep_value,indep_date,indep_base,dep_value,match_check);

    /*find information about this rating */
    select indep_site_datatype_id, rating_type_common_name,
      effective_start_date_time,effective_end_date_time
    into sdi, rating, sdate, edate
    from ref_site_rating
    where rating_id = rating_in;

    if indep_date <sdate or indep_date >= edate then
      deny_action ('Date for time interpolated rating is not within effective range
        for rating id '|| rating_in);
    end if;

    /* find rating id for values after we end (our end date or later) */
    select find_site_rating(rating,sdi,edate)
    into after_rating
    from dual;

    if after_rating is not null and after_rating != rating_in then
    /* sanity check, after rating id must have a effective_start_date_time
      equal to our end time */
      begin
        select rating_id
        into after_rating
        from ref_site_rating where
        rating_id = after_rating and
        effective_start_date_time = edate;
        exception when no_data_found then
          deny_action('Rating '|| after_rating ||' does not have expected start_time, cannot complete time interpolation!');
      end;

      /* do a rating in the rating table after us */
      rating_lookup(after_rating,indep_value,indep_date,indep_base,after_dep_value,match_check);

      /* now interpolate */
      y1:=dep_value;
      y2:=after_dep_value;
      x1:=sdate;
      x2:=edate;

      IF (indep_date = sdate) THEN
        dep_value := y1;
      ELSE
        dep_value := y1 + (y2 -y1)/(x2 -x1)*(indep_date -x1);
      END IF;
    /* else no rating found after us, we just use the first result. */
    end if;

  END rating_time_interp_lookup;

  PROCEDURE rating_time_interp_linear(rating_in IN NUMBER,
  indep_value IN NUMBER,
  indep_date IN DATE,
  indep_base out NUMBER,
  dep_value OUT NUMBER,
  match_check OUT nocopy VARCHAR2)
  AS
/** Function to do a time interpolated rating. The algorithm:
   Do a linear rating with this table.
   See if a rating exists for the time period after this rating id.
   If it does exist, then perform a linear rating with that table.
   Interpolate between the two results in time, and return that value
  */
  sdi ref_site_rating.indep_site_datatype_id%type;
  rating ref_site_rating.rating_type_common_name%type;
  sdate ref_site_rating.effective_start_date_time%type;
  edate ref_site_rating.effective_end_date_time%type;
  after_rating ref_site_rating.rating_id%type;
  after_dep_value NUMBER;
  x1 date;
  x2 date;
  y1 NUMBER;
  y2 NUMBER;
  BEGIN

    rating_linear(rating_in,indep_value,indep_date,indep_base,dep_value,match_check);

    /*find information about this rating */
    select indep_site_datatype_id, rating_type_common_name,
      effective_start_date_time,effective_end_date_time
    into sdi, rating, sdate, edate
    from ref_site_rating
    where rating_id = rating_in;

    if indep_date <sdate or indep_date >= edate then
      deny_action ('Date for time interpolated rating is not within effective range
        for rating id '|| rating_in);
    end if;

    /* find rating id for values after we end (our end date or later) */
    select find_site_rating(rating,sdi,edate)
    into after_rating
    from dual;

    /* see if there is an after rating, and it is a different table than first */
    if after_rating is not null and after_rating != rating_in then
    /* sanity check, after rating id must have a effective_start_date_time
      equal to our end time */
      begin
        select rating_id
        into after_rating
        from ref_site_rating where
        rating_id = after_rating and
        effective_start_date_time = edate;
        exception when no_data_found then
          deny_action('Rating '|| after_rating ||' does not have expected start_time, cannot complete time interpolation!');
      end;

      /* do a rating in the rating table after us */
      rating_linear(after_rating,indep_value,indep_date,indep_base,after_dep_value,match_check);

      /* now interpolate */
      y1:=dep_value;
      y2:=after_dep_value;
      x1:=sdate;
      x2:=edate;

      IF (indep_date = sdate) THEN
        dep_value := y1;
      ELSE
        dep_value := y1 + (y2 -y1)/(x2 -x1)*(indep_date -x1);
      END IF;
    /* else no rating found after us, we just use the first result. */
    end if;

  END rating_time_interp_linear;


  PROCEDURE update_rating_desc(rating_in IN NUMBER,
  description_in IN VARCHAR2)
  AS
  v_count NUMBER;
  BEGIN

    IF(rating_in IS NULL) THEN
      deny_action('Invalid <NULL> rating_id');
    END IF;

    SELECT COUNT(*)
    INTO v_count
    FROM ref_rating_table
    WHERE rating_id = rating_in;

    IF v_count = 0 THEN
      deny_action('Invalid non-existing rating_id ' ||
      rating_in || ' attempted for description update');
    END IF;

    UPDATE ref_rating_table
    SET description = description_in
    WHERE rating_id = rating_in;

  END update_rating_desc;



FUNCTION find_rating_table(rating_in IN NUMBER)
  RETURN ref_rating_table.rating_id%TYPE
  AS
  rating ref_rating_table.rating_id%TYPE;
  v_count NUMBER;

  CURSOR c1 IS
  SELECT rating_id
  INTO rating
  FROM ref_rating_table
  WHERE rating_id = rating_in
  ;

  BEGIN

    OPEN c1;
    FETCH c1
    INTO rating;

    IF c1 % NOTFOUND THEN
      CLOSE c1;
      RETURN NULL;

    ELSE
        CLOSE c1;
        RETURN rating;
        
    END IF;

  END find_rating_table;


FUNCTION find_rating(rating_in IN NUMBER)
  RETURN ref_rating.rating_id%TYPE
  AS
  rating ref_rating.rating_id%TYPE;
  v_count NUMBER;

  CURSOR c1 IS
  SELECT rating_id
  INTO rating
  FROM ref_rating
  WHERE rating_id = rating_in
  ;

  BEGIN

    OPEN c1;
    FETCH c1
    INTO rating;

    IF c1 % NOTFOUND THEN
      CLOSE c1;
      RETURN NULL;

    ELSE
        CLOSE c1;
        RETURN rating;
        
    END IF;

  END find_rating;




END ratings;
