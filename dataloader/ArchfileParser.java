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

public class ArchfileParser {

  private Hashtable hash = null;
  private String file_name = null;
  private Logger log = null;
  private String location = "OTHER";
  private DataObject dobj = new DataObject();
  private DataObject site_codes = new DataObject();
  private boolean specified_codes = false;
  private boolean parameter_override = false;

  public ArchfileParser( String _file_name)
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
            dobj.put("data_source_name",file_name); // add the filename to the data object
            if (System.getProperty("start.property") == null)
            {
              System.out.println("ERROR: Unable to identify property file.  Please identify the Property File");
              System.exit(-1);
            }
            dobj.addPropertyFile(System.getProperty("start.property"));
            dobj.put("def_noninstant","day");  // allows for any noninstant data to default to daily data
            dobj.put("file_type","A"); // set filetype to  archive file

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
            db.closeConnection();

            //  now set up the dataobject for a user specified site_codes
            //  if there are specific site_codes and it already hasn't been
            //  overwritten by a run time parameter and  there are some specified 
            if (!parameter_override && dobj.get("site_codes") != null) this.site_codes_setup((String) dobj.get("site_codes"));

            if (((String)dobj.get("URL")).indexOf(".yak.") > -1||((String)dobj.get("LOCATION")).equals("YAKIMA")) location = "YAKIMA"; 
            else location = "OTHER";

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
   FileInputStream input = null;
   DecimalFormat df = new DecimalFormat("###########0.00");

   try 
   {
     input = new FileInputStream(file_name);
   } 
   catch (FileNotFoundException e) 
   {
     log.debug(this,e.getMessage()); 
     System.exit(-1);
   }


   int b_in =0;  // used to read in the file byte by byte
   boolean process;

   try 
   {

   // read the file int by int
   int i = 0;
   int reads = 0; 
   int recs_processed = 0; 
   char[] buffer = new char[20]; 
   String old_site_code = "xxxxxx";
   String site_code = null;
   String time = null;	
   String old_pcode = "xxxxxxx";
   String pcode = null;
   String [] value = new String[400];
   int group_num = 0;
   boolean flag = false;
   int z_code = 0;
   char edit_flag = ' ';
   int npairs = 366;
   String [] dt_str = new String[npairs];
   byte[] buffer2 = new byte[4];
   byte[] buffer3 = new byte[4];
   DataInputStream dis = null;

   int year = Integer.parseInt(file_name.substring(file_name.indexOf(".seq")-4,file_name.indexOf(".seq"))) -1;
   SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMMdd");
   Calendar cal = new GregorianCalendar(year,8,30);

   for (i=0;i<npairs;i++)
   {
     cal.add(Calendar.DAY_OF_YEAR,1);
     Date dt = cal.getTime();
     dt_str[i] = sdf.format(dt).toUpperCase();
   }

   while (b_in != -1)  // read the file until an End of File EOF is returned
   {
       process = true;

/*
       input.read(buffer2,0,2);
       buffer3[0] = buffer2[1];
       buffer3[1] = buffer2[0];
       dis = new DataInputStream(new ByteArrayInputStream(buffer3));
       group_num = dis.readInt();
*/

       for (i = 0; i< 2; i++)  input.read();
       for (i = 0; i< 12; i++) buffer[i] = (char) input.read();
       site_code  = new String(buffer,0,8);

       for (int j  = 0; j< 9; j++) buffer[j] = (char) input.read();
       pcode = new String(buffer,0,7);

       for (i = 0; i< 4; i++) buffer[i] = (char) input.read(); // read in  time
       time  = new String(buffer,0,2) + ":" + new String(buffer,2,2);

/*
       input.read(buffer3,0,1);
       dis = new DataInputStream(new ByteArrayInputStream(buffer3));
       flag = dis.readBoolean(); // read in the  boolean field
       z_code =  input.read(); //  read in z_code
       edit_flag =  (char) input.read(); //  read in edit_flag
*/

       for (i = 0; i< 5; i++) b_in = input.read();  //temporarily read and dissmiss

       for (i = 0; i< npairs; i++)
       {
         input.read(buffer2,0,4);

         buffer3[0] = buffer2[1];
         buffer3[1] = buffer2[0];
         buffer3[2] = buffer2[3];
         buffer3[3] = buffer2[2];

         dis = new DataInputStream(new ByteArrayInputStream(buffer3));

         Float temp2 = new Float(dis.readFloat() / (float) 4);
         value[i] = df.format(temp2.doubleValue()).toString(); 

        }


      // we are done with the record so read to the beginning of the next record
      for (i = 0; i< 4; i++) b_in = input.read();  //temporarily read and dissmiss
      reads ++;

      //  now see if the site_code is a site that the user may have specified to process 
      //  if there are values in the site_codes dataObject (hash table) then the program should process only those
      //  otherwise process them all

     
if (!specified_codes || (specified_codes && (site_codes.get(site_code.trim()) != null || site_codes.get(site_code.trim()+"/"+pcode.trim()) != null)))
      {
         String inputLine;	
         for (i=0; i<npairs; i++) 
         {
  
           inputLine = dt_str[i] + " " + time + " " + site_code  + " " + pcode + "         " + value[i];
           // the following lines are for online debugging only it should be commented out for production
           //System.out.println(reads + "   " + inputLine);
           //System.out.println("S=" +inputLine.substring(16,24).trim() + " P="+ inputLine.substring(25,33).trim()+
           //" V=" + inputLine.substring(36).trim());

       
             try
             {
              if (!value[i].equals("998877.00"))   //  null value assigned to unfilled values
              {
                //System.out.println(inputLine);
                Connection conn = DriverManager.getConnection(ConnectionPoolManager.URL_PREFIX + "myalias", null, null);

                // this could be a new connection so make sure role is set
                String query,status = null;
                DBAccess db = new DBAccess(conn);
                query = "set role app_role identified by " + dobj.get("role_password");
                status = db.performDML(query,dobj);
                if (!(site_code.equals(old_site_code)) || !(pcode.equals(old_pcode)))
                {
                  dobj.put("STATION",site_code.trim());
                  dobj.put("PCODE",pcode.trim());
                  RBASEUtils rb = new RBASEUtils(dobj,conn);
                  process = rb.get_sdi();
                }
 
                old_site_code = site_code;
                old_pcode = pcode;
                hash = new Hashtable(dobj.getTable());
                if (process) new ObservationThread(inputLine,hash,conn).start();
                if (process) recs_processed ++;
                if (!process) db.closeConnection();
              }
             }
             catch (SQLException e) 
             {
               log.debug(this,e.getMessage()); 
               System.exit(-1);
             }

         }  // end of for loop

      } // end of if to see if we will process this sitecode



       //      if (reads> 4) b_in = -1; // used to stop parsing prematurely
      //System.out.println("RECORD: " + reads);
   

     }  /// end of big loop to read the whole file	

        input.close();
        log.debug(this,"RECORDS Processed: " + recs_processed);

     } // end of big try
     catch (IOException e) 
     {
       log.debug(this,e.getMessage()); 
       System.exit(-1);
     }

    }  // end of process method


 public void site_codes_setup (String codes_in)
 {
     //  just make sure the starting status of this Process indicating boolean
     //  System.out.println("codes in =  " + codes_in);
     specified_codes = false;
     parameter_override = true;  //  indicates tha this method has now been invoked
     // if lenght of codes_in > 0 then we have some so  parse the site_codes
     if (!(codes_in == null) && codes_in.length() > 0 && !codes_in.equals("*") )
     {
        specified_codes = true;
        String field = null;
        int tcount = 0;

        StringTokenizer st = new StringTokenizer(codes_in,",");
        while (st.hasMoreTokens()) {
          field = st.nextToken();
          tcount ++;
          //  put the site_codes in the dataobject for the list of site_codes we want to extract
          site_codes.put(field,new Integer(tcount));
          // following line for debugging only..  comment out for production
          //System.out.println("code=  " + field + "  token= " + tcount);

        } //  end tokenizer while loop
     }


 }  //   end of method site_codes_setup

}
