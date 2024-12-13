package dataloader;

import dbutils.Logger;
import java.io.IOException;

public class ProcessUSBRHMfile {
   private static String version = "1.00.0";

   public static void main(String[] args) throws IOException {
      if (args.length == 1 && args[0].equalsIgnoreCase("-V")) {
         System.out.println("ProcessUSBRHMfile Executeable: " + version);
         System.exit(0);
      }

      String log_file = null;
      if (args.length < 5) {
         log_file = args[0] + ".log";
      } else {
         log_file = args[4];
      }

      Logger log = Logger.createInstance(log_file);
      USACEfileReader afp = new USACEfileReader(args[0], args[1], args[2], args[3]);
      afp.process();
   }
}
