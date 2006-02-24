package dataloader;

import java.io.*;
import java.net.*;
import java.lang.Float;
import java.text.DecimalFormat;
import java.util.*;
import dbutils.*;
import java.sql.*;
import com.bitmechanic.sql.*;
import java.lang.Integer;

public class DayfileParser {

  private Hashtable hash = null;
  private String file_name = null;
  private Logger log = null;
  private String location = "OTHER";
  private DataObject dobj = new DataObject();

  public DayfileParser( String _file_name)
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
            dobj.addPropertyFile(System.getProperty("start.property"));
            dobj.put("def_noninstant","hour");  // allows for any noninstant data to default to hourly data
            dobj.put("file_type","D"); // set filetype to dayfile

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


            hash = new Hashtable(dobj.getTable());


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
   char[] buffer = new char[20]; 
   char fmt = ' '; 
   String site_code = null;
   String time = null;	
   String [] pcode = new String[100];
   String [] q_code = new String[100];
   String [] value = new String[100];
   char rmode = ' ';
   int npairs = 0;

   b_in = input.read();  //temporarily read in the  first  and dissmiss
// System.out.println("read: "  + i + "  byte: " + b_in + "  CHAR: " + (char) b_in);

   while (b_in != -1)  // read the file until and End of File EOF is returned
   {
       process = true;

       for (i = 0; i< 1; i++) {
        fmt  = (char) input.read();  //temporarily read in the  first  and dissmiss
        if (fmt =='Q' || fmt == 'q') process = false;
        //System.out.println((char) b_in); 
       }

       for (i = 0; i< 9; i++) buffer[i] = (char) input.read();
       site_code  = new String(buffer,1,8);
       //System.out.println("SITE CODE: "  + site_code);
       for (i = 0; i< 4; i++) buffer[i] = (char) input.read(); // read in  time
       time  = new String(buffer,0,2) + ":" + new String(buffer,2,2);
        //System.out.println("Time: " + time);

          rmode = (char) input.read(); // convert this int to a char field
          //if (rmode=='R') process = false;  // per paul davidson, these are alarms that need to be allowed to pass through
          //System.out.println("RMODE : " + rmode);

         for (i = 0; i< 5; i++) b_in = input.read();  //temporarily read and dissmiss

         if (location.equals("YAKIMA") && (site_code.trim().equals("CLE")||site_code.trim().equals("KEE")||
             site_code.trim().equals("RIM")||site_code.trim().equals("QSPW")))
         { 
           b_in = input.read();
           if (b_in != 13) npairs = b_in;
           else npairs = input.read();  //read in the number of pairs
          }
          else npairs = input.read();  //read in the number of pairs 
          //System.out.println("NPAIRS: " + npairs);

         for (i = 0; i< npairs; i++)
         {
           for (int j  = 0; j< 9; j++) buffer[j] = (char) input.read();
           pcode[i] = new String(buffer,0,7);

           if ((buffer[7] != '\377') && (buffer[7] != '\375') &&
            (buffer[7] != '\373') && (buffer[7] != '\371') &&
            (buffer[7] != '\354') && (buffer[7] != '\352') &&
            (buffer[7] != '\350') && (buffer[7] != '\346') &&
            (buffer[7] != '\344'))
            q_code[i] = " ";
           else if (buffer[7] == '\377') q_code[i] = " ";
           else if ((buffer[7] == '\375') || (buffer[7] == '\373') || (buffer[7] == '\371')) q_code[i] = "e";
           else if (buffer[7] == '\354') q_code[i] = "-";
           else if (buffer[7] == '\352') q_code[i] = "+";
           else if (buffer[7] == '\350') q_code[i] = "^";
           else if (buffer[7] == '\346') q_code[i] = "~";
           else if (buffer[7] == '\344') q_code[i] = "|";

         }


         for (i = 0; i< npairs; i++)
         {
           byte[] buffer2 = new byte[4];
           byte[] buffer3 = new byte[4];
           input.read(buffer2,0,4);

           buffer3[0] = buffer2[1];
           buffer3[1] = buffer2[0];
           buffer3[2] = buffer2[3];
           buffer3[3] = buffer2[2];

           DataInputStream dis = new DataInputStream(new ByteArrayInputStream(buffer3));

           Float temp2 = new Float(dis.readFloat() / (float) 4);
           value[i] = df.format(temp2.doubleValue()).toString(); 
           //System.out.println("VALUE : " + temp2);

         }


      // we are done with the record so read to the beginning of the next record
      b_in = 0;
      while (b_in != 64 && b_in != -1) {
         b_in = input.read();
         //System.out.println("fmt: " + fmt + " " + site_code + time + " " + rmode + " "   + i + "  byte: " + b_in + "  CHAR: " + (char) b_in);
      }


      String inputLine;	
      for (i=0; i<npairs; i++) {
  
        inputLine = file_name.toUpperCase().substring(0,9) + " " + time + " " + site_code  + " " + pcode[i] + " " + q_code[i] + "       " + value[i];
        //System.out.println(reads + "   " + inputLine);
       
          try
          {
            Connection conn = DriverManager.getConnection(ConnectionPoolManager.URL_PREFIX + "myalias", null, null);

             // this could be a new connection so make sure role is set
             String query,status = null;
             DBAccess db = new DBAccess(conn);
             query = "set role app_role identified by " + dobj.get("role_password");
             status = db.performDML(query,dobj);


            new ObservationThread(inputLine,hash,conn).start();
          reads++; 
          }
          catch (SQLException e) 
          {
            log.debug(this,e.getMessage()); 
            System.exit(-1);
          }

      }

      //if (reads> 3249) b_in = -1; // used to stop parsing prematurely
      //System.out.println("RECORD: " + reads);
   

     }  /// end of big loop to read the whole file	

        input.close();
        log.debug(this,"RECORDS Processed: " + reads);

     } // end of big try
     catch (IOException e) 
     {
       log.debug(this,e.getMessage()); 
       System.exit(-1);
     }

    }  // end of process method


}
