package dbutils;

import java.sql.*;
import java.util.*;
import dbutils.*;

public class Baseloader 
{
   private String error_message;
   private Logger log = null;
   private DataObject do2 = null;
   private Connection conn = null;
   private boolean debug = false;
   private boolean log_all = true;
   private boolean fatal_error = true;
   private DBAccess db = null;

   public Baseloader(DataObject dobj, Connection _conn) 
   {
      log = Logger.getInstance();
      do2 = dobj;
      conn = _conn;
      db = new DBAccess(conn);
      if ((String)this.do2.get("debug") != null && ((String)this.do2.get("debug")).equals("YES")) 
      {
         debug = true;
      }
   }

   public void process()
   {
  
      fatal_error = true;

      try
      {
         if (debug) {
            log.debug(this, "Passed in Database Object to Follow:");
            log.debug(this, do2.toString());
         }

         String proc = "{ call modify_r_base_raw(?,?,to_date(?,'" + do2.get("sample_date_format") + "'),?,to_number(?),?,?,?,?,?,?,?,?,?,?)}";
         CallableStatement stmt = db.getConnection(do2).prepareCall(proc);
         // set all the called procedures input variables from the DataObject
         stmt.setLong(1,Long.parseLong(do2.get("site_datatype_id").toString()));
         stmt.setString(2,(String)do2.get("sample_interval"));
         stmt.setString(3,(String)do2.get("sample_date"));
         //stmt.setString(4,(String) do2.get("end_date_time"));  let procedure determine end_date_time
         stmt.setFloat(5,Float.parseFloat(do2.get("sample_value").toString()));
         stmt.setLong(6,Long.parseLong(do2.get("agen_id").toString()));
         stmt.setString(7,(String)do2.get("overwrite_flag"));
         stmt.setString(8,(String)do2.get("validation"));
         stmt.setLong(9,Long.parseLong(do2.get("collection_system_id").toString()));
         stmt.setLong(10,Long.parseLong(do2.get("loading_application_id").toString()));
         stmt.setLong(11,Long.parseLong(do2.get("method_id").toString()));
         stmt.setLong(12,Long.parseLong(do2.get("computation_id").toString()));
         stmt.setString(13,"Y");
         stmt.setString(14,(String)do2.get("sample_data_flags"));
         stmt.setString(15,(String)do2.get("sample_tz"));

         stmt.registerOutParameter(4, java.sql.Types.DATE);

         // execute the stored procedure call
         stmt.execute();

          // we are done with the procedure call so close the statement
         stmt.close();
         fatal_error = false;
      }
      catch (Exception e) {if (log_all) log.debug(this, e.getMessage());}
   }
}
