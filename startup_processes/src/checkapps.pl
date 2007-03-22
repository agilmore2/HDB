#!/usr/bin/perl

use warnings;
use strict;

use File::Basename;
use Date::Calc qw(Decode_Date_US Today_and_Now);

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
  # global because used in several sub routines
  my $hdb = Hdb->new;

  if ( defined($accountfile) ) {
    $hdb->connect_from_file($accountfile);
  } else {
    my $dbname;
    if ( !defined( $dbname = $ENV{HDB_LOCAL} ) ) {
      $hdb->hdbdie("Environment variable HDB_LOCAL not set...\n");
    }

    #create connection to database
    $hdb->connect_to_db( $dbname, $hdbuser, $hdbpass );
  }

  my $rss = read_rs($hdb);
  my $cps = read_cp($hdb);

  if ( $mode eq 'start' ) {
    startup_rs($rss);
    startup_cp($cps);
  } elsif ( $mode eq 'status' ) {
    check_rs( $hdb, $rss );
    check_cp( $hdb, $cps );
  } elsif ( $mode eq 'stop' ) {
    stop_rs( $hdb, $rss );
    stop_cp( $hdb, $cps );
  } else {
    $hdb->hdbdie("Unrecognized mode $mode!\n");
  }

  exit 0;
}

sub startup_rs($) {
  my $rss = shift;

  foreach my $rs (@$rss) {
    system( "echo", "$decdir/bin/rs", "-e", "-k", "$decdir/lockdir/$rs.lock",
            "-l", "$rs.log", "$rs" );

  }
}

sub startup_cp($) {
  my $cps = shift;

  foreach my $cp (@$cps) {
    system( "echo", "$decdir/bin/compproc", "-d", "1", "-a", "$cp" );
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
      if ( $rs_stat->{StartTime}/1000 > time()-3600 ) {
        print
          "Routing Spec $rs is down, status file not updated in 60 minutes\n";
      } else {
        print "Routing Spec $rs is up\n";
      }
    }
  }
}

sub check_cp ($$) {

  #check heartbeat existence and age
  my $hdb = shift;
  my $cps = shift;

  #find lock and stat files
  foreach my $cp (@$cps) {
    my $heartbeats = $hdb->dbh->selectall_arrayref(
      "select sysdate-heartbeat days, pid
  from cp_comp_proc_lock a, hdb_loading_application b
  where a.loading_application_id = b.loading_application_id and
  b.loading_application_name = '$cp'"
    );

    my @heartbeat = @$heartbeats;

    if ( !defined( $heartbeat[0][0] ) ) {
      print "Comp Proc $cp is down: No heartbeat in database\n";
    } elsif ( $heartbeat[0][0] < 2 / 24 / 60 / 60 ) {

      #heartbeat is less than two seconds old, CP is looking for work
      print "Comp Proc $cp is up, looking for work.\n";
    } else {

      #heartbeat is older, see if pid is running
      system("ps -p $heartbeat[0][1]");
      if ( $? == -1 ) {
        print "failed to execute ps: $!\n";
      } elsif ( ( $? >> 8 ) > 0 ) {    # this gives exit value for system call
        print "Comp Proc $cp is down, no process id $heartbeat[0][1] found\n";
      } else {
        print
"Comp Proc $cp is up, process id $heartbeat[0][1] found, $heartbeat[0][0] days old.\n";
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
