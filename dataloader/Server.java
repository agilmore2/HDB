package dataloader;

import java.net.*;
import java.io.*;
import java.util.*;
import dbutils.*;
import java.sql.*;
import com.bitmechanic.sql.*;
import java.lang.Integer;

	
public class Server {

    private static boolean killprocess = false;
    private static String reason = null;
    private static String email_addr = null;
    private static Server server = null;
    private static String version = "1.00.2";

    public static void main(String[] args) throws IOException 
    {

     if (args.length == 1 && args[0].equalsIgnoreCase ("-V"))
     {
      System.out.println("Server Executeable: " + version);
      System.exit(0);
     }

       server = new Server();
       server.process();
    } // end of main
   
    public static void process()
    {
        ServerSocket serverSocket = null;
        boolean listening = true;
        Hashtable hash = null;
        Logger log = Logger.createInstance("rtdl.log");
        DataObject dobj = new DataObject();

        try {
            //   add some connection defaults incase the property file doesn't have them
            dobj.put("connections","5");
            dobj.put("reuse","100");
            dobj.put("def_noninstant","hour");  // allows for any noninstant data to default to hourly data
            dobj.put("socket","4322");  // default incase the user forgets to add in property file
            dobj.addPropertyFile(System.getProperty("start.property"));

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

            email_addr = (String) dobj.get("emailaddr");

            hash = new Hashtable(dobj.getTable());
            serverSocket = new ServerSocket(new Integer((String) dobj.get("socket")).intValue());//should be 4322 by default

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

        } catch (IOException e) {
            System.err.println("Could not listen on port: " + dobj.get("socket"));
            System.exit(-1);
           }
          catch (Exception e) {
            System.out.println(e.toString());
            System.exit(-1);
          }

        while (listening)
        {
           try
           {
              if (killprocess) break;
              Connection conn = DriverManager.getConnection(ConnectionPoolManager.URL_PREFIX + "myalias", null, null);
              if (killprocess) break;

              // this could be a new connection so make sure role is set
              String query,status = null;
              DBAccess db = new DBAccess(conn);
              query = "set role app_role identified by " + dobj.get("role_password");
              status = db.performDML(query,dobj);

              new ServerThread(serverSocket.accept(),hash,conn,server).start();
           }
             catch (Exception e) {System.out.println(e.toString());}

        }
        try
        {	
          String msg = "The Real time data loader has been shut down for the following Reason: \n" + reason;
          String frm = "REAL TIME DATA LOADER";
          String subject = "Listener SHUTDOWN!!!!";
          StringOps so = new StringOps();
          String rmsg = so.sendEmail(email_addr,frm,subject,msg);

          Thread.sleep(5000); // allow some time for threads to finish up
          serverSocket.close();
        }
        catch (Exception e) {System.out.println(e.toString());}

    }

    public static void setkillprocess(String _reason)
    {
  
     killprocess = true;
     reason = _reason;
    }  // end of set killprocess  method
}
