#!/usr/bin/perl

use warnings;
use strict;

use POSIX 'setsid';
use File::Basename;

#use libraries from HDB environment (Solaris only except for HDB.pm)
# Line below changed by M. Bogner 01-APRIL-2011 to account for ISIS move and 64 bit Perl Libraries
#use lib "$ENV{PERL_ENV}/lib";
use lib "$ENV{HDB_ENV}/perlLib/lib";

#this next is for solaris only, but won't hurt Linux
# following line removed for ISIS move by M. Bogner since this line not needed for linux and
# this program failed compile on Linux
#use lib "$ENV{HDB_ENV}/perlLib/lib/sun4-solaris";

use Hdb;

#Version Information
my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my ( $accountfile, $hdbuser, $hdbpass, $debug );

#store decodes dir of HDB environment for use
my $decdir = "$ENV{DECODES_INSTALL_DIR}";
my $lockdir = "$decdir/lockdir";
my $statdir = "$decdir/routstat";
#OPENDCS 6: user properties and writable data moved out of distribution
if ( -e "$ENV{HDB_ENV}/opendcs_user/lockdir" ) {
    $lockdir = "$ENV{HDB_ENV}/opendcs_user/lockdir";
    $statdir = "$ENV{HDB_ENV}/opendcs_user/routstat";
}

main();

sub main {

  #parse arguments
  my $mode = process_args(@ARGV);    # uses globals, bad!

  if ( ! -x "$decdir/bin/compproc" ) {
    die "OPENDCS Installation not found, compproc not at $decdir/bin/compproc\n";
  }

  #HDB database interface
  my $hdb = Hdb->new;

  #connect to database
  # Be tricky here, check if errors occur. If they do, sleep for 60 seconds and try again.
  my $failcount = 0;
  $|=1; # set autoflushing of output, allows user to see output sooner.
  for (;$failcount<10; $failcount++) {
    eval {
      if ( defined($accountfile) )
      {
        $hdb->connect_from_file($accountfile);
      } else {
        my $dbname;
        if ( !defined( $dbname = $ENV{HDB_LOCAL} ) ) {
          $hdb->hdbdie("Environment variable HDB_LOCAL not set...\n");
        }
        #create connection to database
        $hdb->connect_to_db( $dbname, $hdbuser, $hdbpass );
      }
    };
    if ($@) {print "Attempt to connect failed:\n$@\n";sleep 60}
    else {last;}
  }
  if ($failcount == 10) {
    die "Unable to connect to database, giving up!\n";
  }
  
  # read information about routing specs and computation processes from db
  my $rss = read_rs($hdb);
  my $cps = read_cp($hdb);

  # mode is command line arg, one of start, status, stop
  if ( $mode eq 'start' ) {

    #disconnect from db
    undef $hdb;

    startup_rs($rss);
    startup_cp($cps);
  } elsif ( $mode eq 'status' ) {
    check_rs( $hdb, $rss );
    check_cp( $hdb, $cps );
  } elsif ( $mode eq 'stop' ) {
    stop_rs( $hdb, $rss );
    stop_cp( $hdb, $cps );

    #sleep to make sure that routing specs finish
    sleep 10;
  } else {
    $hdb->hdbdie("Unrecognized mode $mode!\n");
  }

  exit 0;
}

sub startup_rs($) {
  my $rss = shift;

  foreach my $rs (@$rss) {

    my $app = lc($rs);
    $app =~ s/\W//g;
    my $date = `date +%Y%m%d`;
    chomp $date;
 # write lock file. Log and stat files go to default location ($decdir/routstat or $userdir/routstat)

    my @args =
      ( "$decdir/bin/rs", "-e", 
		          #"-d", "3",
                          "-l", "$ENV{HDB_ENV}/log/" . $app . "$date.log",
	                  "-o", "$statdir/$rs.status",
                          "-k", "$lockdir/$rs.lock", "\"$rs\"" );

    print "Starting up $rs\n";
    daemonize( @args, $app );
  }
}

sub startup_cp($) {
  my $cps = shift;
  my @args;

  foreach my $cp (@$cps) {
    my $app = lc($cp);
    my $date = `date +%Y%m%d`;
    $app =~ s/\W//g;
    chomp $date;
    my $logfile = "$ENV{HDB_ENV}/log/" . $app . "$date.log";

    #If this is the compdepends proc, start it differently.
    if ($app eq "compdepends") {
	@args = ( "$decdir/bin/compdepends",
#                  "-d","1",
                  "-Y","America/Denver",
                  "-l", $logfile );
    } elsif ($app eq "dcpmon") {
        @args = ( "$ENV{HDB_ENV}/OPENDCS/dcpmon/dcpmon_start.sh",
#                  "-d","1",
                  "-Y","America/Denver",
                  "-l", $logfile );
#    } elsif ($app eq "rap") {
    # debug level 3, to logfile as specified, need more data on FTRW-Inflow-Volume, HDB ticket #1440
#        @args = ( "$decdir/bin/compproc", "-a","\"$cp\"",
#                      "-d","3",
#                      "-Y","America/Denver",
#                      "-l", $logfile );
    } else {
	@args = ( "$decdir/bin/compproc", "-a","\"$cp\"",
#                  "-d","1",
                  "-Y","America/Denver",
                  "-l", $logfile );
    }

    print "Starting up $cp\n";

    daemonize( @args, $app );
  }
}

#this function is from perldoc perlipc, with a few additions for real use
sub daemonize {
  local $SIG{CHLD} = 'IGNORE';
  my $appfile = "$ENV{HDB_ENV}/log/" . pop();

  defined( my $pid = fork() ) or die "Can't fork: $!";
  return if $pid;    # if we are parent, finished with launch
  setsid() or die "Can't start a new session: $!";
  open STDIN, '/dev/null' or die "Can't read /dev/null: $!";
  open STDOUT, ">$appfile$$.out"
    or die "Can't write stdout to $appfile$$.out: $!";
  open STDERR, ">$appfile$$.err"
    or die "Can't write stderr to $appfile$$.err: $!";

  #set environment variable TZ to handle Java default timezone issues
  $ENV{TZ}="America/Phoenix";

  print "in child $appfile\n";
  exec(@_);
}

sub stop_rs ($$) {
  my $hdb = shift;
  my $rss = shift;

# method for stopping Routing Specs is removing optional lock file created at startup

  foreach my $rs (@$rss) {
    if ( !-e "$lockdir/$rs.lock" ) {
      print "Cannot stop $rs, $lockdir/$rs.lock file not found!\n";
    } else {
      print "Stopping Routing Spec $rs\n";
      unlink("$lockdir/$rs.lock");
      unlink("$lockdir/$rs.lock");
    }
  }
}

sub stop_cp ($$) {
  my $hdb = shift;
  my $cps = shift;

  print "Checking which computations to stop:\n";
  my $pids = check_cp($hdb,$cps);
  # use provided stopcomp script to remove lock/heartbeat from db
  # while compproc is actually computing, will not check for heartbeat
  # otherwise will be checking every second
  foreach my $cp (@$cps) {
    $SIG{CHLD} = 'DEFAULT';
    system( "$decdir/bin/stopcomp", "-a", "\"$cp\"" );
  }

  # wait for processes to detect and exit.
  sleep 5;
  my @runningpids;
  foreach my $pid (@$pids) {
    #check which pids are still running
    $SIG{CHLD} = 'DEFAULT';
    system("ps -p $pid &>/dev/null");
    if ( $? == -1 ) {
      print "failed to execute ps: $!\n";
    } elsif ( ( $? >> 8 ) == 0 ) {    # this gives exit value for system call
      push @runningpids, $pid;
      print "Found running pid $pid!\n";
    }
  }
  if (@runningpids) {
    sleep 30; #is 30 seconds long enough to wait for really long running computations?
    foreach my $pid (@runningpids) {
	print "Last resort! Killing pid $pid and child processes\n";
	system("kill $pid &>/dev/null"); #kill shell process
	system("pkill -f java.*-DPPID=$pid &>/dev/null"); #kill java processes with right command line
    }
    sleep 5; #Give some time to allow processes to gracefully exit, then show no mercy!
    foreach my $pid (@runningpids) {
      system("kill -9 $pid &>/dev/null"); #kill shell process
      system("pkill -9 -f java.*-DPPID=$pid &>/dev/null"); #kill java processes with right command line
    }
  }
}

sub check_rs ($$) {
  my $hdb = shift;
  my $rss = shift;
  my $down;

  #find lock and stat files
  foreach my $rs (@$rss) {
    if ( !-e "$lockdir/$rs.lock" and !-e "$statdir/$rs.stat" ) {
      print "Routing Spec $rs is down, no lock or status file\n";
    } else {
      my $rs_stat = read_rs_stat($rs);

      #returns much information, but we care about StartTime,
      #which is in milliseconds since epoch, and Status

      my $timediff = time() - $rs_stat->{StartTime} / 1000;

# time() returns current time since epoch, so this is difference in seconds between
# last rs run time and current time
# according to docs:
#"While running, the process will ‘touch’ the lock file every 10 seconds. If the file was
#deleted, the process will terminate. So allow about 10 seconds after deleting a lock file
#before starting a new instance.
#A lock file is “busy” if it exists and has been touched within the last 20 seconds."

      if ($timediff < 21
          and (    $rs_stat->{Status} eq 'Running'
               or $rs_stat->{Status} eq 'Waiting' )
         )
      {
        printf "Routing Spec $rs is up, last run %.2f seconds ago.\n",
          $timediff;
      } elsif ( $rs_stat->{Status} eq 'Stopped' ) {
        print "Routing Spec $rs is down, status is stopped.\n";
      } else {
        printf
"Routing Spec $rs is down, status: '$rs_stat->{Status}' file last updated %.2f seconds ago\n",
          $timediff;
      }
    }
  }
}

sub check_cp ($$) {
  #check heartbeat existence and age
  my $hdb = shift;
  my $cps = shift;
  my @pids;
  #read status from db
  foreach my $cp (@$cps) {
    my $heartbeats = $hdb->dbh->selectall_arrayref(
      "select sysdate-heartbeat days, pid
  from cp_comp_proc_lock a, hdb_loading_application b
  where a.loading_application_id = b.loading_application_id and
  b.loading_application_name = '$cp'"
    );

    # last touch of heartbeat, in fractional days
    my $heartbeat = $heartbeats->[0][0];

    # pid of controlling script for CP
    my $pid = $heartbeats->[0][1];

    if ( !defined($heartbeat) ) {
      print "Comp Proc $cp is down: No heartbeat in database\n";
    } elsif ( $heartbeat < 2 / 60 / 60 / 24 ) {

      #heartbeat is less than two seconds old, CP is looking for work
      print "Comp Proc $cp is up, looking for work.\n";
    } else {

      #heartbeat is older, see if pid is running
      # we use the ps command here and check its exit value
      # if it is 0, the command exited sucessfully
 
     $SIG{CHLD} = 'DEFAULT';
      system("ps -p $pid >/dev/null");
      if ( $? == -1 ) {
        print "failed to execute ps: $!\n";
      } elsif ( ( $? >> 8 ) > 0 ) {    # this gives exit value for system call
        printf
          "Comp Proc $cp is down, %.2f days old, no process id $pid found.\n",
          $heartbeat;
      } else {
        printf "Comp Proc $cp is up, process id $pid found, %.2f days old.\n",
          $heartbeat;
      }
      #store pid for use with stop
      push @pids, $pid;
    }
  }
  return \@pids;
}

sub read_rs($) {
  my $hdb = shift;

  my $rs_to_start = $hdb->dbh->selectcol_arrayref(
    "select a.name from
routingspec a, routingspecproperty b
where 
a.id = b.routingspecid and
b.prop_name = 'autostart' and
b.prop_value = 'Y'"
  );

  return $rs_to_start;
}

sub read_cp($) {
  my $hdb = shift;

  my $cp_to_start = $hdb->dbh->selectcol_arrayref(
    "select loading_application_name name
  from hdb_loading_application a, ref_loading_application_prop b 
  where a.loading_application_id = b.loading_application_id and
  b.prop_name = 'autostart' and
  b.prop_value = 'Y'"
  );

  return $cp_to_start;
}

sub read_rs_stat ($) {
  my $rs = shift;

  open STATFILE, "$statdir/$rs.status";
  my @lines = <STATFILE>;
  close STATFILE;
  my %rs_stat;
  foreach my $line (@lines) {
    chomp $line;
    chop $line;    # stupid \r\n!
    my @fields = split( /=/, $line, 2 );
    $rs_stat{ $fields[0] } = $fields[1];
  }

  return \%rs_stat;

}

sub process_args {
  my $mode;

  #scalar and array versions, the scalars are a string of the arrays

  while (@_) {
    my $arg = shift;
    if ( $arg =~ /-h/ ) {    # print help
      usage();
    } elsif ( $arg =~ /-v/ ) {    # print version
      version();
    } elsif ( $arg =~ /-a/ ) {    # get database login file
      $accountfile = shift;
      if ( !-r $accountfile ) {
        print "file not found or unreadable: $accountfile\n";
        exit 1;
      }
    } elsif ( $arg =~ /-d/ ) {    # get debug flag
      $debug = 1;
    } elsif ( $arg =~ /-u/ ) {    # get hdb user
      $hdbuser = shift;
    } elsif ( $arg =~ /-p/ ) {    # get hdb passwd
      $hdbpass = shift;
    } elsif ( $arg =~ /-.*/ ) {    # Unrecognized flag, print help.
      print STDERR "Unrecognized flag: $arg\n";
      &usage;
    } elsif (    $arg eq 'start'
              or $arg eq 'stop'
              or $arg eq 'status' )
    {
      $mode = $arg;
    } else {
      print STDERR "Unrecognized command line argument: $arg\n";
      &usage;
    }
  }

  if ( !defined($accountfile)
       and ( !defined($hdbuser) || !defined($hdbpass) ) )
  {
    print STDERR
      "ERROR: Required: accountfile (-a) or HDB username and password(-u -p)\n";
    usage();
  }

  if ( !defined($mode) ) {
    print STDERR
      "Error, program mode unspecified, one of start,status,stop required.\n";
    usage();
  }

  return $mode;
}

sub usage {
  print STDERR <<"ENDHELP";
$progname $verstring [ -h | -v ] | [ options ] [start|status|stop]
Script to start, stop or check status of all computation processors and
routing specs defined to automatically start.
Example: $progname -a <accountfile> 

  -h               : This help
  -v               : Version
  -a <accountfile> : HDB account access file (REQUIRED or both below)
  -u <hdbusername> : HDB application user name (account file or REQUIRED)
  -p <hdbpassword> : HDB password (account file or REQUIRED)
  -d               : Debugging output
ENDHELP

  exit(1);
}

sub version {
  print "$progname version $verstring\n";
  exit 1;
}
