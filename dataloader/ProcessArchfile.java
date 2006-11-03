package dataloader;

import java.io.*;
import dbutils.*;
	
public class ProcessArchfile {

    private static String version = "1.01.2";
    // version 1.01.0 modified to version 1.01.2 to allow deletes in R_BASE for archive values of 998877
    // modified by M. Bogner 10 October 2006

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
      boolean do_deletes = false;

      for (int i=0;i<args.length;i++) 
      {
       if (args[i].toUpperCase().startsWith("FILENAME=")) file_name = args[i].substring(9);
       if (args[i].toUpperCase().startsWith("LOGNAME=")) log_file = args[i].substring(8);
       if (args[i].toUpperCase().startsWith("SITE_CODES=")) site_codes = args[i].substring(11);
       if (args[i].toUpperCase().equals("-D"))  do_deletes = true;
       if (args[i].toUpperCase().equals("-ND"))  do_deletes = false;
      }
 

     if (file_name == null || file_name.length() == 0 )
     {
      System.out.println("ERROR: Archive File Processing:  no file specified " );
      System.exit(-1);
     }

      if (log_file == null) log_file = file_name.substring(0,file_name.lastIndexOf(".")) + ".log";
      Logger log = Logger.createInstance(log_file);

      //System.out.println (log_file + "    " + site_codes);

      // set the do deletes to false if there are no site_codes listed
      // or if the * wild card has been used  Basically don't allow deletes to occur from processig
      // a full archive file
      if (site_codes == null || site_codes.equals("*")) do_deletes = false;
      //System.out.println(do_deletes);

      ArchfileParser afp = new ArchfileParser(file_name,do_deletes);
      if (site_codes != null) afp.site_codes_setup(site_codes);
      afp.process();

    }
}
