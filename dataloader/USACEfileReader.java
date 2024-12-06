package dataloader;

import java.io.*;
import java.lang.Integer;
import java.util.*;
import dbutils.*;
import java.sql.*;

public class USACEfileReader {

  private Hashtable hash = null;
  private String file_name = null;
  private Logger log = null;
  private int errors = 0;
  private DBAccess db = null;
  private boolean debug = false;
  private Connection cnn = null;
  private String comma = ",";
  private String apos = "'";
  private String pipe = "|";
   DataObject dobj_orig = null;


   public USACEfileReader(String _file_name) {
    file_name = _file_name; 
    log = Logger.getInstance();
    db = new DBAccess(cnn);
    dobj_orig = new DataObject();

    try 
    {
      dobj_orig.addPropertyFile(System.getProperty("start.property"));
      RBASEUtils rbu = new RBASEUtils(dobj_orig, cnn);
      rbu.get_all_ids();
      cnn = db.getConnection(dobj_orig);
    }
    catch (Exception e) {System.out.println(e.toString());}

  }

   public USACEfileReader(String _file_name, String parm, String int) {
      file_name = _file_name;
      log = Logger.getInstance();
      db = new DBAccess(cnn);
      dobj_orig = new DataObject();

      try {
         dobj_orig.addPropertyFile(System.getProperty("start.property"));
         dobj_orig.put("SAMPLE_INTERVAL", int);
         dobj_orig.put("PARAMETER_CODE", parm);
         dobj_orig.put("SAMPLE_DATE_FORMAT", "yyyy-mm-dd HH24:MI");
         if (int.equalsIgnoreCase("day")) {
            dobj_orig.put("SAMPLE_DATE_FORMAT", "yyyy-mm-dd");
         }

         cnn = db.getConnection(dobj_orig);
         RBASEUtils rbu = new RBASEUtils(dobj_orig, cnn);
         rbu.get_all_ids();
      }
     catch (Exception e) {System.out.println(e.toString());}

   }

   public void process()

 {
   BufferedReader input = null;

   try 
   {
     input = new BufferedReader(new FileReader(file_name));
   } 
   catch (FileNotFoundException e) {
     log.debug(this,e.getMessage());
     System.exit(-1);
   }



   try
   {
     if (dobj_orig.get("debug") != null && dobj_orig.get("debug").toString().equalsIgnoreCase("YES")) {
         debug = true;
     }

   // read the file line by line
         String inputLine = null;
         int reads = 0;
         int skips = 0;
         DataObject dobj = null;

         for(inputLine = input.readLine(); inputLine != null; inputLine = var1.readLine()) {
            boolean process = true;
            if (inputLine != null) {
               ++reads;
               if (debug) {
                  log.debug(this, inputLine);
               }

               if (inputLine.substring(0, 1).equals("#")) {
                  ++skips;
               } else {
                  dobj = new DataObject(dobj_orig.getTable());
                  dobj.put("overwrite_flag", "");
                  dobj.put("validation", "");
                  dobj.put("sample_data_flags", "");
                  if (dobj.get("data_source") != null && dobj.get("file_type").toString().equals("CUWCD_PSV")) {
                     this.parseCentral(dobj, inputLine);
                  } else if (dobj.get("data_source") != null && dobj.get("file_type").toString().equals("USGS_TSV")) {
                     this.parseUSGSTSV(dobj, inputLine);
                  } else {
                     this.parse(dobj, inputLine);
                  }

                  RBASEUtils rbu = new RBASEUtils(dobj, this.cnn);
                  Integer sdi = rbu.get_external_data_sdi();
                  if (sdi > 0) {
                     if (this.debug) {
                        this.log.debug(this, dobj.toString());
                     }

                     Baseloader bl = new Baseloader(dobj, cnn);
                     bl.process();
                  } else {
                     this.log.debug("No mapping record found for site: " + dobj.get("site_code") + "  Parameter Code: " + dobj.get("parameter_code") + "  Source: " + dobj.get("data_source"));
                     ++skips;
                  }
               }
            }
         }

         input.close();
         cnn.close();
         log.debug(this, "RECORDS Processed: " + reads);
         log.debug(this, "SKIPS: " + skips);
      } catch (SQLException e) { // end of big try
         log.debug(this, e.getMessage());
         System.exit(-1);
      } catch (IOException e) {
         log.debug(this, e.getMessage());
         System.exit(-1);
      }

   }

    private void parse(DataObject dobj, String input)
    {

/* CSV file reader, Format:
Data Source,Date:HH:MI:TZ,site code,parameter code,interval,value,unit
*/

      int tcount = 0;

      dobj.put("INPUTLINE", input);
      String[] fields = input.split(comma);
      int l = fields.length;

      for(int i = 0; i < l; ++i) {
         String field = fields[i];
         switch(tcount) {
         case 0:
            dobj.put("DATA_SOURCE", field);
            break;
         case 1:
            field = field.replace("24:00", "00:00");
            String[] t = field.split(":");
            dobj.put("SAMPLE_DATE", t[0] + ":" + t[1]);
            dobj.put("SAMPLE_TZ", t[2]);
            break;
         case 2:
            dobj.put("SITE_CODE", field);
            break;
         case 3:
            dobj.put("PARAMETER_CODE", field);
            break;
         case 4:
            dobj.put("SAMPLE_INTERVAL", field);
            break;
         case 5:
            dobj.put("SAMPLE_VALUE", field);
            break;
         case 6:
            dobj.put("SAMPLE_UNIT", field);
         }

         ++tcount;
      }

   }

   private void parseCentral(DataObject dobj, String input)
    {

/* Central Utah Format (value may have thousands separators):
Site Code|Date|Value
*/

      int tcount = 0;

      dobj.put("INPUTLINE", input);
      String[] fields = input.split("[|]");
      int l = fields.length;

      for(int i = 0; i < l; ++i) {
         String field = fields[i];
         switch(tcount) {
         case 0:
            dobj.put("SITE_CODE", field);
            dobj.put("PARAMETER_CODE", "NO ENTRY");
            dobj.put("SAMPLE_INTERVAL", "hour");
            break;
         case 1:
            dobj.put("SAMPLE_DATE", field);
            dobj.put("SAMPLE_TZ", "MST");
            break;
         case 2:
            dobj.put("SAMPLE_VALUE", field.replaceAll(this.comma, ""));
         }

         ++tcount;
      }

   }

   private void parseUSGSTSVDataObject dobj, String input)
    {

/* USGS RDB Format (tab separated, two different format):
instant:
Source Code	Date	TZ	value	validation
non-instant assumes MST TZ
Source Code	Date	value	validation
*/

      int tcount = 0;

      dobj.put("INPUTLINE", input);
      String[] fields = input.split("\t");
      int l = fields.length;

      for(int i = 0; i < l; ++i) {
         String field = fields[i];
         switch(tcount) {
         case 0:
            dobj.put("EXTERNAL_SOURCE_CODE", field);
            break;
         case 1:
            dobj.put("SITE_CODE", field);
            break;
         case 2:
            dobj.put("SAMPLE_DATE", field);
            dobj.put("SAMPLE_TZ", "MST");
            break;
         case 3:
            if (dobj.get("sample_interval") != null && dobj.get("sample_interval").toString().equals("instant")) {
               dobj.put("SAMPLE_TZ", field);
               break;
            }

            dobj.put("SAMPLE_VALUE", field);
            break;
         case 4:
            if (dobj.get("sample_interval") != null && dobj.get("sample_interval").toString().equals("instant")) {
               dobj.put("SAMPLE_VALUE", field);
               break;
            }

            dobj.put("validation", var8);
            break;
         case 5:
            dobj.put("validation", var8);
         }

         ++tcount;
      }

   }
}
