create or replace procedure
          update_new_modelrun_id
IS
          num_db_sites integer;
          sqMaxid       integer;
          ID_RANGE     integer;
          site_code    varchar(5);
BEGIN
      ID_RANGE := 100;
      SELECT db_site_code INTO site_code
       FROM  ref_db_list
       WHERE session_no = 1;
       if SQL%NOTFOUND then
          raise_application_error(-20001,'Error: Unable to retrieve current site code');
       end if;
       SELECT count(*) INTO num_db_sites
         FROM ref_db_list;
         if SQL%NOTFOUND then
            raise_application_error(-20002,' Error: Unable to determine current site number');
         end if;
       SELECT maxid INTO sqMaxid
         FROM ref_db_list
        WHERE db_site_code = site_code;
        if SQL%NOTFOUND then
           raise_application_error(-20003,' Error: Unable to retrieve current model run record');
        end if ;
        UPDATE ref_model_run
           SET model_run_id = sqMaxid
         WHERE model_run_id = 0;
         if SQL%NOTFOUND then
            raise_application_error(-20004,'Error: Unable to update model_run_id of ref_model_run');
         end if;
         if SQL%ROWCOUNT <> 1 then
            raise_application_error(-20005,'Error: model run is corrupted -- not one row in ref_model_run');
         end if;
	 if mod (sqMaxid, ID_RANGE) != 0 then
	    sqMaxid := sqMaxid + 1;
	 else
            sqMaxid := sqMaxid + (num_db_sites - 1) * ID_RANGE + 1;
         end if;
         UPDATE ref_db_list
            SET maxid = sqMaxid
         WHERE db_site_code = site_code;
         if SQL%NOTFOUND then
             raise_application_error(-20005,'Error: Unable to update maxid of ref_db_list');
          end if;
          if SQL%ROWCOUNT <> 1 then
             raise_application_error(-20006,'Error: record is corrupt -- not one row in ref_db_list');
 end if;
END;
/
show errors;
/
grant execute on update_new_modelrun_id to public;
/
