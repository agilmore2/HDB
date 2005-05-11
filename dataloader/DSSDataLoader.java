package dataloader;

import java.io.*;
import java.net.*;
import java.text.*;
import java.util.Date;
import java.util.*;
import java.lang.Float;
import java.text.DecimalFormat;
import java.util.*;
import dbutils.*;
import java.sql.*;
import com.bitmechanic.sql.*;
import java.lang.Integer;
import java.util.Vector;
import hec.heclib.dss.*;
import hec.heclib.util.doubleArrayContainer;
import hec.heclib.util.intArrayContainer;
import hec.heclib.util.HecTime;

import dataloader.*;

public class DSSDataLoader extends hec.heclib.dss.HecDSSFileAccess {

  private Hashtable hash = null;
  private String file_name = null;
  private Logger log = null;
  private String location = "OTHER";
  private DataObject dobj = new DataObject();

  public DSSDataLoader( String _file_name)
  {
    file_name = _file_name; 
    log = Logger.getInstance();
  }


 private void poolsetup ()
 {

        try {
            //   add some connection defaults incase the property file doesn't have them
            dobj.put("connections","5");
            dobj.put("reuse","2000");
            dobj.put("days_back","2000"); // defaults to 2000 days back if not specified in the property file
            dobj.put("data_source_name",file_name); // add the filename to the data object
            dobj.addPropertyFile(System.getProperty("start.property"));
            dobj.put("def_noninstant","hour");  // allows for any noninstant data to default to daily data
            dobj.put("file_type","D"); // set filetype to  dss file


            // get the password for enabling app_role and then set the user and password back to what it was
            String temp_user = (String) dobj.get("username");
            String temp_password = (String) dobj.get("password");
            String url = (String) dobj.get("url");
            dobj.put("password",url.substring(url.lastIndexOf(":")+1));
            dobj.put("username","PSSWD_USER");
            DBAccess db = new DBAccess(null);
            Connection conn = db.getConnection(dobj);
            RBASEUtils rb = new RBASEUtils(dobj,conn);
            rb.get_role_password("APP_ROLE");
            // get the r_base default foreign keys  for this process
            rb.get_all_ids();
            dobj.put("username",temp_user);
            dobj.put("password",temp_password);

//          go get a date loading limit as specified  in the property file  or the default
            String select_clause = "(trunc(sysdate-" + dobj.get("days_back") + ") - to_date('31-DEC-1899')) * 60 * 24 minutes_limit";
            db.get_col(dobj,"dual",select_clause,"1=1");
            db.closeConnection();

            ConnectionPoolManager mgr = new ConnectionPoolManager(300);
            Class.forName((String) dobj.get("driver")).newInstance();
            mgr.addAlias("myalias", (String) dobj.get("driver"),
                         (String) dobj.get("url"),
                         (String) dobj.get("username"),
                         (String) dobj.get("password"),
                         new Integer((String) dobj.get("connections")).intValue(),  // max connections to open
                         600, // seconds a connection can be idle before it is closed
                         120, // seconds a connection can be checked out by a thread
                              // before it is returned back to the pool
                         new Integer((String) dobj.get("reuse")).intValue(), // number of times a connection can be re-used before
                              // connection to database is closed and re-opened
                              // (optional parameter)
                      false); // specifies whether to cache statements
                              //   (optional parameter.  set to 'true' by default.)

            Connection cnn = DriverManager.getConnection(ConnectionPoolManager.URL_PREFIX + "myalias", null, null);
            RBASEUtils rb2 = new RBASEUtils(dobj,cnn);
            rb2.get_all_ids();
            cnn.close();

        } 
        catch (Exception e) 
        {
          log.debug(this,e.getMessage()); 
          System.exit(-1);
        }


   } // end of poolsetup method



public void process()

 {
   this.poolsetup();
   DecimalFormat df = new DecimalFormat("###########0.000000");
   String pathName = null;
   Vector pathnameList = new Vector();
   HecTimeSeries hts = null;
   DSSPathname dsp = null;
   doubleArrayContainer datarows = new doubleArrayContainer();
   intArrayContainer datatimes = new intArrayContainer();
   String inputLine = null;
   double dvalue = -9999.0;
   int reads = 0;
   Integer model_id = new Integer((String) dobj.get("model_id"));
   int minutes_limit =  Integer.parseInt((String) dobj.get("minutes_limit"));

   try 
   {

     hts = new HecTimeSeries();
     hts.setDSSFileName(file_name); //set class to dss file
     this.setDSSFileName(file_name);
     this.getPathnameList("",pathnameList);  // gets all pathnames in dss file
     //System.out.println( "Path size: " + pathnameList.size());

     for ( int i=0; i < pathnameList.size(); i++)  //process all pathnames
     //for ( int i=0; i < 1; i++)  // for debug purposes
     {
          boolean process = false;  //change to false for production
          pathName = (String) pathnameList.get(i);
          log.debug(this,"Processing DSS pathname : " + pathName);
          dsp = new DSSPathname(pathName);

          String sdi_key  = "|apart=" + dsp.getAPart() + "|"
             + "bpart=" + dsp.getBPart() + "|"
             + "cpart=" + dsp.getCPart() + "|"
             + "epart=" + dsp.getEPart() + "|"
             + "fpart=" + dsp.getFPart() + "|";

          sdi_key = sdi_key.toUpperCase();  // key for ref_dmi_data_map

          String temp_str = "insert into ref_dmi_data_map values ("  
          + model_id.toString()
          + ",'" + sdi_key
          + "','" + file_name.toUpperCase() + "',-9999)";

          //  go get the sdi for this particular pathname
          Connection conn1 = DriverManager.getConnection(ConnectionPoolManager.URL_PREFIX + "myalias", null, null);
          DBAccess db1 = new DBAccess(conn1);
          RBASEUtils rb1 = new RBASEUtils(dobj,conn1);
          Integer sdi = rb1.get_DMI_SDI(model_id.intValue(),sdi_key,file_name.toUpperCase());
          db1.closeConnection();

          if (sdi.intValue() < 0) log.debug(this,temp_str);
          if (sdi.intValue() > 0)  //  valid sdi so process the pathname data 
          {
             //  now time to get all the data and times from the DSS data file for this path name
             hts.clear();
             hts.setPathname(pathName);
             hts.read();
             hts.getData(datarows);
             hts.getTimes(datatimes);

           //System.out.println("Length: " + datarows.length);  // finds out how many datarows we have
          // for every row go get the value and the timestamp for each record
          //  only loop through the rows if there is a valid sdi
          int row_count = datarows.length;
          for ( int j=0; j < row_count; j++)
          {
            Double doubleValue = new Double (datarows.array[j]);
            dvalue = doubleValue.doubleValue();

            // if the value is not a null default and the minutes since 1899 >== time limit then process this record
            if (dvalue > -3.4028234663852886E+38 && datatimes.array[j] >= minutes_limit)
            {
             Connection conn = DriverManager.getConnection(ConnectionPoolManager.URL_PREFIX + "myalias", null, null);

             // this could be a new connection so make sure role is set
             String query,status = null;
             DBAccess db = new DBAccess(conn);
             query = "set role app_role identified by " + dobj.get("role_password");
             status = db.performDML(query,dobj);
             RBASEUtils rb = new RBASEUtils(dobj,conn);
             String time = rb.get_DSS_Date(datatimes.array[j]); // get the time in correct format
             //System.out.println("value : " + dvalue  + "  time : " + hect.date() + " " + hect.time() + "   " + datatimes.array[j]);

             inputLine = time  + "                          " + df.format(doubleValue);
             //System.out.println(inputLine);
             hash = new Hashtable(dobj.getTable());
             // now go put this row of data into the DB
             new ObservationThread(inputLine,hash,conn).start();
             reads ++;
            } // end of block for a valid value

          }   // end of for to get all the rows values

        }  // end of process this sdi block


     }  // loop to handle each distinct pathname in the dss file
     hts.done();  // done with the file so close it
     log.debug(this,"DSS RECORDS Processed: " + reads);

     } // end of big try
     catch (Exception e) 
     {
       log.debug(this,e.getMessage()); 
       System.exit(-1);
     }

    }  // end of process method


}
