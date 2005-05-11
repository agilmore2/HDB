package dataloader; 

import java.io.*;
import dbutils.Logger;
	
public class ProcessWAMetadata {

    private static String version = "1.00.0";

    public static void main(String[] args) throws IOException 

    {

     if (args.length == 1 && args[0].equalsIgnoreCase ("-V"))
     {
      System.out.println("ProcessWAMetadata Executeable: " + version);
      System.exit(0);
     }

      String log_file = null;

      if (args.length == 1) log_file = args[0];
      else log_file = "ProcessMetadata.log";
 
      Logger log = Logger.createInstance(log_file);

      WAMetadata mdp = new WAMetadata(args[0]);
      mdp.process();

    }
}
