#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <utils.h>
#include <dbutils.h>

#define N_OF_SIGNALS 19

typedef struct 
{
  int signal_id; 
  char *signal_symbol;
  char *signal_text;
} SIGNAL;

SIGNAL signals[N_OF_SIGNALS] =
{
  {SIGHUP,   "SIGHUP",  "Hangup"},
  {SIGINT,   "SIGINT",  "Interrupt"},
  {SIGQUIT,  "SIGQUIT", "Quit"},
  {SIGABRT,  "SIGABRT", "Abort"},
  {SIGFPE,   "SIGFPE",  "Floating Point Exception"},
  {SIGKILL,  "SIGKILL", "Killed"},
  {SIGBUS,   "SIGBUS",  "Bus Error"},
  {SIGSEGV,  "SIGSEGV", "Segmentation Fault"},
  {SIGSYS,   "SIGSYS",  "Bad System Call"},
  {SIGPIPE,  "SIGPIPE", "Broken Pipe"},
  {SIGALRM,  "SIGALRM", "Alarm Clock"},
  {SIGTERM,  "SIGTERM", "Terminated"},
  {SIGUSR1,  "SIGUSR1", "User Signal 1"},
  {SIGUSR2,  "SIGUSR2", "User Signal 2"},
  {SIGCHLD,  "SIGCHLD", "Child Status Changed"},
  {SIGTSTP,  "SIGTSTP", "Stopped - User"},
  {SIGCONT,  "SIGCONT", "Continued"},
  {SIGXCPU,  "SIGXCPU", "CPU Time Limit Exceeded"},
  {SIGXFSZ,  "SIGXFSZ", "File Size Limit Exceeded"}
};

#define FUNC_NAME "RegisterSignalHandler"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> RegisterSignalHandler

<P> <STRONG>Function signature:</STRONG> int RegisterSignalHandler (void)

<P> <STRONG>Function description:</STRONG> Registers the signal handler
SignalHandler. This function is to be called only once per application.

<P><DL> <STRONG>Function arguments:</STRONG>
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

int RegisterSignalHandler()
{
  int i;

/* register the signal handler for each of the entries in the
   signals table */

  for (i = 0; i < N_OF_SIGNALS; i++)
    signal(signals[i].signal_id, SignalHandler);

  return(OK);
}
/*=========================================================*/

#define FUNC_NAME "SignalHandler"
/*
<HTML>

<P> <STRONG>Function name:</STRONG> SignalHandler

<P> <STRONG>Function signature:</STRONG> void SignalHandler (int the_signal)

<P> <STRONG>Function description:</STRONG> Signal handler for HDB
applications. The handler catches N_OF_SIGNALS (19) UNIX signals
and closes all open database connections before returning control to
UNIX signal handling.<p>
Do <b>never!!!</b> call this function in your application. It is called by
the UNIX signal function.

<P><DL> <STRONG>Function arguments:</STRONG>
<DD> int signal: the UNIX signal function passes the argument the_signal
to SignalHandler. SignalHandler then searches a global array of SIGNAL
structures to find which signal it got past so it can print out the
associated message.</DD>
</DL>

<P> <STRONG>Function return value:</STRONG> OK or ERROR

</HTML>
*/

void SignalHandler(int the_signal)
{
  int i,result;

/* Look up the signal received in the signals table and printf
   the diagnostic. Then disconnect from all open database 
   connections. Note: Whether or not the disconnect succeeds,
   exit the signal handler. */

  for (i = 0; i < N_OF_SIGNALS; i++)
   if (the_signal == signals[i].signal_id)
   {
      printf("Signal %s (%s) received...\n",
                  signals[i].signal_symbol,
                  signals[i].signal_text);
      printf("Closing any open database connections...\n");

      result = SqlDisconnectAll();
      if (result != OK)
        PrintError("Problem disconnecting from database session(s) after receiving signal...\n");

      signal(SIGABRT,SIG_DFL); 
      abort();
   }

  printf("Unrecognized signal %d received...\n",the_signal);
  printf("Closing any open database connections...\n");
  result = SqlDisconnectAll();
  if (result != OK)
    PrintError("Problem disconnecting from database session(s) after receiving signal...\n");

  signal(SIGABRT,SIG_DFL);
  abort();
}
