#!/usr/bin/perl

use warnings;
use strict;

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{HDB_ENV}/perlLib/lib";

#this next is for solaris only, but won't hurt Linux
use lib "$ENV{HDB_ENV}/perlLib/lib/sun4-solaris";

use File::Basename;

use Hdb;

#Version Information
my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my ( $accountfile, $hdbuser, $hdbpass, $debug );

my $decbin = "$ENV{HDB_ENV}/decodes/bin";

main();

#never reached
exit 0;

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
  startup_rs($rs_to_start);

  my $cp_to_start = read_cp($hdb);
  startup_cp($cp_to_start);

  exit 0;
}

sub startup_rs($) {
  my $rs_to_start = shift;

  foreach my $rs (@$rs_to_start) {
    system( "echo", "$decbin/rs","-e","-o","$rs.stat","-k","$rs.lock","-l","$rs.log","$rs" );

  }
}

sub startup_cp($) {
  my $cp_to_start = shift;

  foreach my $cp (@$cp_to_start) {
    system( "echo", "$decbin/compproc", "-d", "1", "-a", "$cp" );
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
