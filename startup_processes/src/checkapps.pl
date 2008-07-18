#!/usr/bin/perl

use warnings;
use strict;

use POSIX 'setsid';
use File::Basename;

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{HDB_ENV}/perlLib/lib";

#this next is for solaris only, but won't hurt Linux
use lib "$ENV{HDB_ENV}/perlLib/lib/sun4-solaris";

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
my $decdir = "$ENV{HDB_ENV}/decodes";

main();

sub main {

  #parse arguments
  my $mode = process_args(@ARGV);    # uses globals, bad!

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

 # write lock file. Log and stat files go to default location ($decdir/routstat)

    my @args =
      ( "$decdir/bin/rs", "-e", "-k", "$decdir/lockdir/$rs.lock", "\"$rs\"" );

    print "Starting up $rs\n";
    daemonize( @args, $app );
  }
}

sub startup_cp($) {
  my $cps = shift;

  foreach my $cp (@$cps) {
    my $app = lc($cp);
    $app =~ s/\W//g;
    my $logfile = "$ENV{HDB_ENV}/log/" . $app . "$$.log";

    # debug level 1, to logfile as specified
    my @args = ( "$decdir/bin/compproc", "-a","\"$cp\"",
                 "-d","1", "-Y","America/Denver",
                 "-l", $logfile );

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
    if ( !-e "$decdir/lockdir/$rs.lock" ) {
      print "Cannot stop $rs, $decdir/lockdir/$rs.lock file not found!\n";
    } else {
      print "Stopping Routing Spec $rs\n";
      unlink("$decdir/lockdir/$rs.lock");
      unlink("$decdir/lockdir/$rs.lock");
    }
  }
}

sub stop_cp ($$) {
  my $hdb = shift;
  my $cps = shift;

  # use provided stopcomp script to remove lock/heartbeat from db
  # while compproc is actually computing, will not check for heartbeat
  # otherwise will be checking every second
  foreach my $cp (@$cps) {
    system( "$decdir/bin/stopcomp", "-a", "\"$cp\"" );
  }
}

sub check_rs ($$) {
  my $hdb = shift;
  my $rss = shift;
  my $down;

  #find lock and stat files
  foreach my $rs (@$rss) {
    if ( !-e "$decdir/lockdir/$rs.lock" and !-e "$decdir/routstat/$rs.stat" ) {
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
    }
  }
}

sub read_rs($) {
  my $hdb = shift;

  my $rs_to_start = $hdb->dbh->selectcol_arrayref(
    "select a.name from
routingspec a, routingspecproperty b
where 
a.id = b.routingspecid and
b.name = 'autostart' and
b.value = 'Y'"
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

  open STATFILE, "$decdir/routstat/$rs.stat";
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
      "Error, program mode unspecified, one of start,status,stop required.";
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
