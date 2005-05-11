package dataloader;

import java.io.*;
import dbutils.*;
	
public class ProcessArchfile {

    private static String version = "1.01.0";

    public static void main(String[] args) throws IOException 
    {

     if (args.length == 1 && args[0].equalsIgnoreCase ("-V"))
     {
      System.out.println("ProcessArchfile Executeable: " + version);
      System.exit(0);
     }

      String log_file = null;
      String site_codes = null;
      String file_name = null;

      for (int i=0;i<args.length;i++) 
      {
       if (args[i].toUpperCase().startsWith("FILENAME=")) file_name = args[i].substring(9);
       if (args[i].toUpperCase().startsWith("LOGNAME=")) log_file = args[i].substring(8);
       if (args[i].toUpperCase().startsWith("SITE_CODES=")) site_codes = args[i].substring(11);
      }
 

     if (file_name == null || file_name.length() == 0 )
     {
      System.out.println("ERROR: Archive File Processing:  no file specified " );
      System.exit(-1);
     }

      if (log_file == null) log_file = file_name.substring(0,file_name.lastIndexOf(".")) + ".log";
      Logger log = Logger.createInstance(log_file);

      //System.out.println (log_file + "    " + site_codes);

      ArchfileParser afp = new ArchfileParser(file_name);
      if (site_codes != null) afp.site_codes_setup(site_codes);
      afp.process();

    }
}
