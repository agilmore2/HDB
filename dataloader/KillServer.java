package dataloader;

import java.io.*;
import java.net.*;
import dbutils.*;
import java.lang.Integer;

public class KillServer {

    private static String version = "1.00.2";

    public static void main(String[] args) throws IOException {


     if (args.length == 1 && args[0].equalsIgnoreCase ("-V"))
     {
      System.out.println("KillServer Executeable: " + version);
      System.exit(0);
     }

        Socket kkSocket = null;
        PrintWriter out = null;
        DataObject dobj = new DataObject();

        dobj.put("socket","4322");  // default incase the user forgets to add in property file

        try {
            dobj.addPropertyFile(System.getProperty("start.property"));
            kkSocket = new Socket(args[0],new Integer((String) dobj.get("socket")).intValue());
            out = new PrintWriter(kkSocket.getOutputStream(), true);
        } catch (UnknownHostException e) {
            System.err.println("Don't know about host: "+ args[0]);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to: "+ args[0]);
            System.exit(1);
        }
          catch (Exception e) {
            System.out.println(e.toString());
            System.exit(-1);
        }


        out.println("KILLPROCESS");

        out.close();
        kkSocket.close();
    }
}
