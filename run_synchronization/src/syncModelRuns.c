#include <stdio.h>

#include <defines.h>
#include <dmi_utils.h>
#include <dmiGlobals.h>

#define VERSION "1.1"

int main(int argc, char *argv[])
{
  int result;

  if (argc==2)
   if (!(strcmp(argv[1],"-v")))
   {
     printf("\n%s version %s\n\n",argv[0],VERSION);
     exit (1);
   }
  if (argc >= 2)
  {
     printf("Illegal options; valid command syntax is:\n\n");
     printf("%s <-v>\n\n",argv[0]);
     printf("where: -v: print version number\n");
  }  

  result = SqlSyncModelRuns();
  if (result != OK)
  {
    printf("Syncing of modelruns not completed...Exiting\n");
    exit(1);
  }
  exit (OK); 
}
