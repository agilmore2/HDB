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

my $decdir = "$ENV{HDB_ENV}/decodes";

main ();

sub main {

  #parse arguments
  process_args(@ARGV);    # uses globals, bad!

  #HDB database interface
  # global because used in several sub routines
  my $hdb = Hdb->new;

  if ( defined($accountfile) ) {
    $hdb->connect_from_file($accountfile);
  }
  else {
    my $dbname;
    if ( !defined( $dbname = $ENV{HDB_LOCAL} ) ) {
      $hdb->hdbdie("Environment variable HDB_LOCAL not set...\n");
    }

    #create connection to database
    $hdb->connect_to_db( $dbname, $hdbuser, $hdbpass );
  }

  my $rs_to_start = read_rs($hdb);
  check_rs($rs_to_start);

  my $cp_to_start = read_cp($hdb);
  check_cp($cp_to_start);

  exit 0;
}

sub startup_rs($) {
  my $rs_to_start = shift;

  foreach my $rs (@$rs_to_start) {
    system( "echo", "$decdir/bin/rs","-e","-k","$rs.lock","-l","$rs.log","$rs" );

  }
}

sub startup_cp($) {
  my $cp_to_start = shift;

  foreach my $cp (@$cp_to_start) {
    system( "echo", "$decdir/bin/compproc", "-d", "1", "-a", "$cp" );
  }
}

sub check_rs ($) {
  my $rs_to_start = shift;
  my $down;
  
  #find lock and stat files
  foreach my $rs (@$rs_to_start) {
    if (! -e "$rs.lock" or ! -e "$rs.stat") {
      #rs is down because lock or status files are missing
      $down = 1;
    }
    my $rs_stat = read_rs_stat($rs);
    if ($rs_stat->{readtime} > localtime()-3600) {
    #last readtime is more than an hour ago, rs must be down
      $down = 1;
    } 
    if ($down) {
      start_rs($rs);
    }
  }
}

sub check_cp ($$) {
  #check heartbeat existence and age
  my $hdb=shift;
  my $cp =shift;
  my $heartbeats = $hdb->dbh->selectcol_arrayref("select heartbeat 
  from cp_comp_proc_lock a, hdb_loading_application b
  where a.loading_application_id = b.loading_application_id and
  b.loading_application_name = '$cp'");

  my @heartdate = Decode_Date_US ($heartbeats->[0]);
  my @now = Today_and_Now();
  
  my @diff = Delta_DHMS(@heartdate,@now);
  
  if ($diff[0] >0 or
      $diff[1] >0) {
    #heartbeat is more than an hour old!
    
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
  from hdb_loading_application a, cp_comp_proc_property b 
  where a.loading_application_id = b.loading_application_id and
  b.prop_name = 'autostart' and
  b.prop_value = 'Y'"
  );

  return $cp_to_start;
}

sub read_rs_stat ($) {
  my $rs=shift;

  open STATFILE, "$decdir/routstat/$rs.stat";
  my @lines = <STATFILE>;
  close STATFILE;
  
  my %rs_stat;
  foreach my $line (@lines) {
    my @fields = split(/=/,$line, 2);
    $rs_stat{$fields[0]} = $fields[1];  
  }
  
  return \%rs_stat;

}

sub process_args {    

  #scalar and array versions, the scalars are a string of the arrays

  while (@_) {
    my $arg = shift;
    if ( $arg =~ /-h/ ) {    # print help
      usage();
    }
    elsif ( $arg =~ /-v/ ) {    # print version
      version();
    }
    elsif ( $arg =~ /-a/ ) {    # get database login file
      $accountfile = shift;
      if ( !-r $accountfile ) {
        print "file not found or unreadable: $accountfile\n";
        exit 1;
      }
    }
    elsif ( $arg =~ /-d/ ) {    # get debug flag
      $debug = 1;
    }
    elsif ( $arg =~ /-u/ ) {    # get hdb user
      $hdbuser = shift;
    }
    elsif ( $arg =~ /-p/ ) {    # get hdb passwd
      $hdbpass = shift;
    }
    elsif ( $arg =~ /-.*/ ) {    # Unrecognized flag, print help.
      print STDERR "Unrecognized flag: $arg\n";
      &usage;
    }
    else {
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

  return;
}

sub usage {
  print STDERR <<"ENDHELP";
$progname $verstring [ -h | -v ] | [ options ]
Starts all computation processors and routing specs defined to automatically start.
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
