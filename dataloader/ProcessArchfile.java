package dataloader;

import java.io.*;
import dbutils.*;
	
public class ProcessArchfile {

    private static String version = "1.00.0";

    public static void main(String[] args) throws IOException 
    {

     if (args.length == 1 && args[0].equalsIgnoreCase ("-V"))
     {
      System.out.println("ProcessArchfile Executeable: " + version);
      System.exit(0);
     }

      String log_file = null;

      if (args.length == 1) log_file = args[0].substring(0,args[0].lastIndexOf(".")) + ".log";
      else log_file = args[1];
 
      Logger log = Logger.createInstance(log_file);

      ArchfileParser afp = new ArchfileParser(args[0]);
      afp.process();

    }
}
