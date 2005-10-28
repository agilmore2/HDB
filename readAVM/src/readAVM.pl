#!/usr/bin/perl -w

#insert HDB library

use lib "$ENV{HDB_ENV}/perlLib/lib";
# Don't need
#use lib "$ENV{HDB_ENV}/perlLib/lib/i386-linux";

use Hdb;

use Date::Calc qw(:all);
use DBI;
use English qw( -no_match_vars );
#use DBD::Oracle;

use strict;

my ($hdb, @cur_sdi, @value_date, @data_date);

#check to see command line usage.
if (scalar(@ARGV) <3)
  {
    my $progname = `basename $0`;
    chop $progname;
    print "$progname -- program to put Glen Canyon AVM data into HDB.\n";
    print "Usage: $progname <filename> <hdbusername> <hdbpassword>\n";
    exit (1);

  }

#HDB database interface
# global because used in several sub routines
$hdb = Hdb->new;

my $dbname;
if (!defined($dbname = $ENV{HDB_LOCAL})) {
  die "Environment variable HDB_LOCAL not set...\n";
}
#create connection to database
$hdb->connect_to_db($dbname, $ARGV[1], $ARGV[2]);

#set date format to handle hourly data
set_date_format('DD-MON-YYYY HH24:MI:SS');

#set role so that application may write to DB
$hdb->set_approle();

#retrieve application ids for later updates and inserts
get_app_ids();

#prepare the handle for the modify statement
prepare_sth();

open (INFILE, "$ARGV[0]") || die "Error: couldn't open input file $ARGV[0]";

# File format looks like:

#*** END OPERATE ***
#
#                           GLEN CANYON PENSTOCK FLOWS
#                              HOURLY FLOW RECORDS

#                           FLOW      P-VOL     T-VOL   
#                         ========  ========  ========  
#Tue Sep 28 2004 14.00.30     539 measurements
#   SECT-1  (  0% good)   --------     55813     55813  
#   SECT-2  (100% good)   2580.754     77357     77357  
#   SECT-3  (100% good)     27.458     13828     13828  
#   SECT-4  (100% good)   2502.785    143873    143873  
#   SECT-5  (100% good)     32.414      8140      8140  
#   SECT-6  (100% good)   2495.330    141360    141360  
#   SECT-7  (100% good)     33.725     13631     13631  
#   SECT-8  (100% good)   2472.694     41797     41797  
#                         ========  ========  ========  
#        totals:          10145.16    495799    495803  

#Tue Sep 28 2004 15.00.30     628 measurements
#   SECT-1  (  0% good)   --------     55813     55813  
#   SECT-2  (100% good)   2557.394     77568     77568  
#   SECT-3  (100% good)     27.491     13831     13831  
#   SECT-4  (100% good)   2477.485    144078    144078  
#   SECT-5  (100% good)     32.877      8142      8142  
#   SECT-6  (100% good)   2484.597    141565    141565  
#   SECT-7  (100% good)     33.707     13634     13634  
#   SECT-8  (100% good)   2466.842     42000     42000  
#                         ========  ========  ========  
#        totals:          10080.39    496631    496635  


# where the END OPERATE line may occur after the end of an hour printout,
# but may not occur for months.
# A single  and the GLEN CANYON etc. header occur every four hours,
# and is reset by the END_OPERATE line
# the original file has DOS line ending format

# sites and sdis: the site names here are the first field of each line
# containing the data, even for the total: line. (for Glen Canyon Power Plant
# units and whole plant)

my %siteid_hash = ("SECT-1", 2020, "SECT-2", 2021, "SECT-3", 2022,
                   "SECT-4", 2023, "SECT-5", 2024, "SECT-6", 2025,
                   "SECT-7", 2026, "SECT-8", 2027,"totals:", 970); 
my %site_datatype_hash = ("SECT-1", undef, "SECT-2", undef, "SECT-3", undef,
                          "SECT-4", undef, "SECT-5", undef, "SECT-6", undef,
                          "SECT-7", undef, "SECT-8", undef,"totals:", undef); 

# avm release
my $datatype = 1235;

my $cur_site_name = "none";
my ($line, @fields, $date, $time, $value, $site_datatype_id, $datestr);

#handle DOS end lines
$INPUT_RECORD_SEPARATOR="\r\n";

READ: while ($line = <INFILE>)
{
  chomp($line);

#next line if is short (END OPERATE, page and line feeds),
# or unneeded (header and separator lines)
  next READ if (length($line) < 22 or $line =~ /FLOW/
                or $line =~ /={8}/ or $line =~ /END OPERATE/);

#collapse whitespace
  $line =~ s/  +/ /g;

  #process date if this line contains one
  if (($date, $time) =
      $line =~ /^(\w{3} \w{3} \d{2} \d{4}) (\d\d\.\d\d\.\d\d)/ ) {
    if (not @value_date = Parse_Date($date)) {
      $hdb->hdbdie ("Invalid date: $date");
    }
    #add the time to the date, and subtract an hour to get start_date_time
    @value_date[3..5] = split /\./, $time;
    @value_date = Add_Delta_DHMS(@value_date, 0,-1,0,0);
    #create date string for use in writing to db, format set as above
    $datestr = sprintf("%02d-%.3s-%4d %02d:%02d:%02d",$value_date[2],
                       Month_to_Text($value_date[1]), $value_date[0],
                       $value_date[3], $value_date[4], $value_date[5]); 

    next READ;
  }

  @fields = split  ' ', $line;
  $cur_site_name = $fields[0];

  if (defined $siteid_hash{$cur_site_name}) {
    get_SDI($cur_site_name)
  } else {
    $hdb->hdbdie ("unknown site name: $cur_site_name");
  }

  #File has one datapoint per line, site is indicated by first part of line.
  $site_datatype_id = $site_datatype_hash{$cur_site_name};

  # fields look different for total line
  if ($cur_site_name eq 'totals:') {
    $value = $fields[1];
  } else {
    if ($fields[1] eq '(') {#data quality is < 100%, so '( 'instead of '(100%'
      $value = undef; # do not put data in db, if data is bad
    } elsif ($fields[1] eq '(100%') {
      $value = $fields[3];
    } else { #quality unrecognized
      $hdb->hdbdie ("quality field $fields[1] unrecognized, file format corrupted");
    }
  }

#one of these calls per value, which is inefficent, but quick to code.
  if (defined($site_datatype_id) && defined($value)) {
    insert_values($datestr, $site_datatype_id, $value);
  }
}

finish_sth();

$hdb->dbh->disconnect();

exit 0;

#End main program, subroutines below

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
  $nameid->{collect}->{name} = '(see agency)';
  $nameid->{load_app}->{name} = 'readAVM.pl';
  $nameid->{method}->{name} = 'unknown';
  $nameid->{computation}->{name} = 'unknown';

  $hdb->get_app_ids($nameid);

  $agen_id = $nameid->{agen}->{id};
  $collect_id = $nameid->{collect}->{id};
  $load_app_id = $nameid->{load_app}->{id};
  $method_id = $nameid->{method}->{id};
  $computation_id = $nameid->{computation}->{id};
}

my $modsth;

sub prepare_sth
{
  my $modify_data_statement = "
    BEGIN
        modify_r_base_raw(?,'hour',?,?,?, /* sdi, interval, start_date_time, end_date_time (in/out, not used), value */
                          $agen_id,null,'Z', /* overwrite, validation */
                          $collect_id,$load_app_id,$method_id,$computation_id,
                          'Y');                 /*do update? */
    END;";

    $modsth = $hdb->dbh->prepare($modify_data_statement) || $hdb->hdbdie ($DBI::errstr . $modsth->errstr);
}

sub finish_sth
{
  $modsth->finish;
}


sub get_SDI
{
  my $cur_site_name = $_[0];
  my $cur_site_id = $siteid_hash{$_[0]};

  if (!defined $site_datatype_hash{$cur_site_name}) {
    $site_datatype_hash{$cur_site_name} = $hdb->get_SDI($cur_site_id,$datatype);
  }
}

sub set_date_format
{
  my $date_format = shift;
  eval {
    $hdb->dbh->do("alter session set NLS_DATE_FORMAT = '$date_format'");
  };
  if ($@) {
    print "$DBI::errstr, $@\n";
    $hdb->hdbdie ("Errors occurred setting of date format.");
    }

}

sub insert_values
{
  my($datestr) = shift;
  my($sdi) = shift;
  my($value) = shift;

  my $end_date_time = undef;
  #    print @_;


  eval {
    # insert data;
    my  $old_val = undef;
    $old_val = check_value($datestr, $sdi);
    if (!defined $value) {
      #delete data if data is -99.9, otherwise do nothing.
    } elsif (defined($old_val) and $old_val == $value) {
      print "data is identical, skipping!\n";
    } elsif (!defined($old_val) or $old_val != $value) {
      if (!defined($old_val)) {
#        print "modifying for $sdi, date $datestr, value $value, old_val = undef\n";
      } else {
        print "modifying for $sdi, date $datestr, value $value, old_val = $old_val\n";
      }
      $modsth->bind_param(1,$sdi);
      $modsth->bind_param(2,$datestr);
      $modsth->bind_param_inout(3,\$end_date_time, 50);
      $modsth->bind_param(4,$value);
#      $modsth->execute|| die $modsth->errstr;
    }
  };
  if ($@) {
    print "$DBI::errstr, $@\n";
    $hdb->hdbdie ("Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.");
  } else {
    $hdb->dbh->commit;
  }
}

sub check_value
  {
    my $check_data_statement = "select value from r_base where start_date_time = ? and site_datatype_id = ? and interval = 'hour'";

    my($check_date)=$_[0];
    my($site_datatype_id)=$_[1];
    my($sth);
    my($myval);

    $sth = $hdb->dbh->prepare($check_data_statement) || die $sth->errstr;
    $sth->bind_param(1,$check_date);
    $sth->bind_param(2,$site_datatype_id);
    $sth->execute || die $sth->errstr;
    $sth->bind_col(1,\$myval);
    $sth->fetch;
#    DBI::dump_results($sth)|| die $DBI::errstr;;
    $sth->finish();
#    print $myval;
    return $myval;
  }

