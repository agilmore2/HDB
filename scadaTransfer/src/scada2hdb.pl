#!/usr/local/bin/perl

#insert HDB library

use lib "$ENV{HDB_ENV}/perlLib/lib";
# Don't need?
#use lib "$ENV{HDB_ENV}/perlLib/lib/i386-linux";
use Hdb;

use Date::Calc qw(:all);
use File::Basename;
use Data::Dumper;
use DBI;
#use DBD::Oracle;

use strict;
use warnings;

my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my $insertflag = 1;
my $overwrite = 'null';

my ($hdb, @value_date);
my ($debug,$readfile);
my ($accountfile, $hdbuser, $hdbpass);

#======================================================================
#parse arguments
while (@ARGV)
{
  my $arg = shift(@ARGV);
  if ($arg =~ /-h/) {	        # print help
    &usage;
  } elsif ($arg =~ /-v/) {	# print version
    &version;
  } elsif ($arg =~ /-t/) {	# get test flag
    $insertflag=undef;
  } elsif ($arg =~ /-d/) {	# get debug flag
    $debug=1;
  } elsif ($arg =~ /-o/) {	# get overwrite flag
    $overwrite="'O'";
  } elsif ($arg =~ /-f/) {	# get file to read from
    $readfile = shift(@ARGV);
    if (! -e $readfile) {
      print "file not found: $readfile";
      exit 1;
    }
  } elsif ($arg =~ /-a/) {	# get file to read from
    $accountfile = shift(@ARGV);
    if (! -e $accountfile) {
      print "file not found: $accountfile";
      exit 1;
    }
  } elsif ($arg =~ /-u/) {	# get hdb user
    $hdbuser=shift(@ARGV);
  } elsif ($arg =~ /-p/) {	# get hdb passwd
    $hdbpass=shift(@ARGV);
  } elsif ($arg =~ /-.*/) {	# Unrecognized flag, print help.
    print STDERR "Unrecognized flag: $arg\n";
    &usage;
  } else {
    print STDERR "Unrecognized command line argument: $arg\n";
    &usage;
  }
}

if (!defined($accountfile) and (!defined($hdbuser) || !defined($hdbpass))) {
  print "Error! No database login information!\n";
  usage();
}

if (!defined($readfile)) {
  print "Error! No file was specified!\n";
  usage();
}

#HDB database interface
# global because used in several sub routines
$hdb = Hdb->new;

#create connection to database
if (defined($accountfile)) {
  $hdb->connect_from_file($accountfile);
} else {
  my $dbname;
  if (!defined($dbname = $ENV{HDB_LOCAL})) {
    $hdb->hdbdie("Environment variable HDB_LOCAL not set...\n");
  }
  #create connection to database
  $hdb->connect_to_db($dbname, $hdbuser, $hdbpass);
}

$hdb->set_approle();

#Set date format to HDB Standard, with hours and minutes
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI'")
  or $hdb->hdbdie("Unable to set NLS_DATE_FORMAT to HDB standard\n");

#retrieve application ids for later updates and inserts
get_app_ids();

# get date on input file
open (INFILE, "$readfile") || die "Error: couldn't open input file $ARGV[0]";


=FORMAT

The data is expected in the following format, one value per line, ~864 lines:

YYYY,MM,DD,HH,MI,SITE,datatatype_name,value,somevalidationcode

which is 9 fields.

Other processes in this pipeline cut this file down from the original
6000 lines per day.

=cut

my ($line, $datestr, @fields, @prevdate, $sitecode);
my ($value, $head, $rel);
my $seenspill =0;


# get date in filename, assume date starts after last '_'
# and ends at beginning of extension (.dat for example)
$datestr = substr $readfile, rindex($readfile,"_")+1;
$datestr = substr $datestr, 0,index($datestr,".");

print "$datestr\n";

my $scada_map = get_scada_map();

READ: while ($line = <INFILE>)
{
#  print $line if defined($debug);
  
  chomp($line);
  @fields=split(/,/,$line);

  if (@fields >9) { # do not read individual unit data
     last READ;
  }


#value_date is a Date::Calc date array, year, month,day,hour, minute, second
  @value_date=@fields[0..4]; #get first 5 fields from line and
  $value_date[5]=0; #zero seconds field

# check the date in the file
  eval {@value_date = Add_Delta_DHMS(@value_date,0,0,0,0);};
  if ($@) {
    die "Invalid Date detected in input file, exiting!\n$line\n";
  }

  my $sitecode=$fields[5].",".$fields[6];
  my $value=$fields[7];

# define this flag on command line to test reading, but not inserting.
  next READ unless defined($insertflag);

# check apparent validation field.
 # zero seems to be ok validation, 8 and 128 seem also ok. 32 may be bad.
  next READ if not ($fields[8]==0 or $fields[8]==8 or $fields[8]==128 or $fields[8]==32);

#use the string "sitecode,datacode" as the lookup key for the sdi etc.
#pretty lame, but don't want to take time to make 2D hash
  if (defined($scada_map->{$sitecode}->{sdi}))  { #do we have an sdi
    insert_values($scada_map->{$sitecode},
                  \@value_date, $value);
  }

  #process additional data
  #calculate head from reservoir elevation - tailbay
  # and total release as turbine release +spillway + hollow_jet(bypass)
  # check that dates match.
  # use two digits of accuracy, since that is all the SCADA reports for
  # elevations
  if ($fields[6] eq "reservoir_elevation") {
    $head=$value;
    @prevdate=@value_date;

  } elsif ($fields[6] eq "tailbay_elevation") {
    if (@value_date != @prevdate) {
      warn "data not in expected order, head not calculated!\n";
    } else {
      $head-=$value;
      $head=sprintf("%.2f",$head);
      if (defined($scada_map->{$fields[5].",head"}->{sdi})) {
        insert_values($scada_map->{$fields[5].",head"},
                      \@value_date, $head);
      }
    }
# expect the fields turbine, spillway, and hollow_jet releases in that order,
# warn if not
  } elsif ($fields[6] eq "turbine_release") {
    @prevdate=@value_date;
    $rel=$value;

  } elsif ($fields[6] eq "spillway_release") {
    if (@value_date != @prevdate or $seenspill==1) {
      warn "data not in expected order, release not calculated!\n";
    } else {
      $rel+=$value;
      $seenspill=1;
    }

  } elsif ($fields[6] eq "hollow_jet_release") {
    if (@value_date != @prevdate or $seenspill==0) {
      die "data not in expected order, release not calculated! Exiting!\n";
    } else {
      $rel+=$value;
      $seenspill=0;
      $rel=sprintf("%.2f",$rel);
      if (defined($scada_map->{$fields[5].",total_release"}->{sdi})) {
        insert_values($scada_map->{$fields[5].",total_release"},
                      \@value_date, $rel);
      }
    }
  }
}


$hdb->dbh->disconnect();

#End main program, subroutines below

sub check_value
  {
    my($check_date)=$_[0];
    my($site_datatype_id)=$_[1];
    my($interval)=$_[2];

    my($sth);
    my($myval);

    my $check_data_statement = "select value from r_base where start_date_time = ? and site_datatype_id = ? and interval = ?";

    $sth = $hdb->dbh->prepare($check_data_statement) || die $sth->errstr;
    $sth->bind_param(1,$check_date);
    $sth->bind_param(2,$site_datatype_id);
    $sth->bind_param(3,$interval);
    $sth->execute || die $sth->errstr;
    $sth->bind_col(1,\$myval);
    $sth->fetch;
#    DBI::dump_results($sth)|| die $DBI::errstr;;
    $sth->finish();
#    print $myval;
    return $myval;
  }

#globals, yuck!
my $agen_id;
my $collect_id;
my $load_app_id;
my $method_id;
my $computation_id;

sub get_app_ids
{
# Get ids to describe where data came from
  my $nameid;

  $nameid->{agen}->{name} = 'Bureau of Reclamation';
  $nameid->{collect}->{name} = 'SCADA';
  $nameid->{load_app}->{name} = $progname;
  $nameid->{method}->{name} = 'unknown';
  $nameid->{computation}->{name} = 'unknown';

  $hdb->get_app_ids($nameid);

  $agen_id = $nameid->{agen}->{id};
  $collect_id = $nameid->{collect}->{id};
  $load_app_id = $nameid->{load_app}->{id};
  $method_id = $nameid->{method}->{id};
  $computation_id = $nameid->{computation}->{id};

}

sub get_scada_map
{

# only get enabled maps, and do not get any maps that have extra keys
# only look at 'UC' sites!!
# mapping list key is sitecode,datacode, which is lame
# don't want to try to figure out better way.
  my $get_scada_map_statement =
  "select b.primary_site_code ||','|| 
 b.primary_data_code scada_code, b.primary_data_code datacode,
 b.hdb_interval_name interval, b.hdb_method_id meth_id,
 b.hdb_computation_id comp_id, b.hdb_site_datatype_id sdi,
 c.site_id, c.site_name
from hdb_site_datatype a, ref_ext_site_data_map b, hdb_site c
where a.site_datatype_id = b.hdb_site_datatype_id and
b.is_active_y_n ='Y' and
b.extra_keys_y_n = 'N' and
a.site_id = c.site_id and
c.db_site_code = 'UC'
order by scada_code";

  $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

  my $hashref;

#this DBI function returns a hash indexed by column 1 (one-based), which is
# the scada code "site,datacode" (the first column returned by the query above)
# if no data returned, checked later
  eval {
    $hashref = $hdb->dbh->selectall_hashref($get_scada_map_statement,1);
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie("Errors occurred during automatic selection of SCADA ids.\n");
  }

  return $hashref;
}


sub insert_values
  {
    my($scada) = $_[0];
    my(@value_date) = @{$_[1]};
    my $value=$_[2];

    my($modsth);
    my($head) = undef;
    #    print @_;

    my $modify_data_statement = "
  BEGIN
    modify_r_base(?,?, /*sdi, interval */
                      ?,null,/* start_date_time, end_date_time(not used) */
                      ?,/* value */
                      $agen_id,$overwrite,null, /*no validation code*/
                      $collect_id,$load_app_id,
                      $method_id,$computation_id,
                      'Y');/*do update? */
  END;";


    $datestr = sprintf("%4d-%02d-%02d %02d:%02d",$value_date[0],
                      $value_date[1], $value_date[2],
                      $value_date[3], $value_date[4]);

    eval {
      $modsth = $hdb->dbh->prepare($modify_data_statement) || die $modsth->errstr;

      # insert data;
      my ($old_val, $cur_sdi);

      undef $old_val;
      $old_val = check_value($datestr, $scada->{sdi}, $scada->{interval});
      if (!defined $value) {
        print "data missing: $scada->{sdi}, date $datestr\n" if defined($debug);
      } elsif (defined($old_val) and $old_val == $value) {
        print "data identical: $scada->{sdi}, date $datestr, value $value\n" if defined($debug);
      } elsif ((!defined($old_val) or $old_val != $value)) {
        if (defined($debug) and !defined($old_val)) {
          print "modifying for $scada->{sdi}, date $datestr, value $value, old_val = undef\n";
        } elsif (defined($debug)) {
          print "modifying for $scada->{scada_code}, date $datestr, value $value, old_val = $old_val\n";
        }
        $modsth->bind_param(1,$scada->{sdi});
        $modsth->bind_param(2,$scada->{interval});
        $modsth->bind_param(3,$datestr);
        $modsth->bind_param(4,$value);
        if (!defined($debug)) {
          $modsth->execute || die $modsth->errstr;
        }
      }
      $modsth->finish;
    };

    if ($@) {
      $hdb->dbh->rollback;
      print "$DBI::errstr, $@\n";
      die "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.";
    } else {
      $hdb->dbh->commit;
    }
  }


sub usage
{
  print STDERR <<"ENDHELP";
$progname [ -h | -v ] | [ options ]
Retrieves SCADA data and inserts into HDB
Example: $progname -u app_user -p <hdbpassword> -f <crspfile>

  -h               : This help
  -v               : Version
  -a <loginfile>   : HDB account login file (REQUIRED or both of the following)
  -u <hdbusername> : HDB application user name (see above)
  -p <hdbpassword> : HDB password (see above)
  -f <filename>    : file to read from (REQUIRED)
  -t               : Test retrieval, but do not insert data into DB
  -o               : Use overwrite flag, otherwise null
  -d               : Debugging output
ENDHELP

  exit (1);
}

sub version
{
  print "$progname version $verstring\n";
  exit 1;
}
