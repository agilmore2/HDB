CREATE OR REPLACE PROCEDURE refresh_hdb_snap_wrap (which_table IN VARCHAR2, snapshot_num IN INTEGER) IS
   my_stmt VARCHAR2(1000);
   err_num NUMBER;
   BEGIN
     my_stmt := 'BEGIN run_refresh_hdb_snap' || snapshot_num || ' (:1); END;';
     err_num := SQLCODE;

     execute immediate my_stmt using which_table; 

     EXCEPTION
	   when others then
          err_num := SQLCODE;

          if err_num = -6550 then     
            RAISE_APPLICATION_ERROR (-20001, 'identifier not declared'); 
          else 
            RAISE_APPLICATION_ERROR (-20002, 'some other error');
          end if;

   END refresh_hdb_snap_wrap;
/

grant execute on refresh_hdb_snap_wrap to public;
drop public synonym refresh_hdb_snap_wrap;
create public synonym refresh_hdb_snap_wrap for refresh_hdb_snap_wrap;

