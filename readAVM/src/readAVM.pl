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

#sites and datatypes desired. Not all site_ids and site_datatype_ids
# have been created, yet.

# site names (report codes) and site id's for Glen Canyon Power Plant
# units and total
my %siteid_hash = ("SECT-1", 2020, "SECT-2", 2021, "SECT-3", 2022,
                   "SECT-4", 2023, "SECT-5", 2024, "SECT-6", 2025,
                   "SECT-7", 2026, "SECT-8", 2027,"totals:", 970); 
my %site_datatype_hash = ("SECT-1", undef, "SECT-2", undef, "SECT-3", undef,
                          "SECT-4", undef, "SECT-5", undef, "SECT-6", undef,
                          "SECT-7", undef, "SECT-8", undef,"totals:", undef); 

# avm release
my $datatype = 1235;

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
set_date_format();

$hdb->set_approle();

#retrieve application ids for later updates and inserts
get_app_ids();

# various SQL statement strings, with placeholders for variables.

open (INFILE, "$ARGV[0]") || die "Error: couldn't open input file $ARGV[0]";

# No junk before data in this file format!

# get date of data is interesting, because file contains no standalone date.

my $cur_site_name = "none";
my ($line, @fields, $date, $time, $value, $site_datatype_id);

READ: while ($line = <INFILE>)
{
  $INPUT_RECORD_SEPARATOR="\r\n";
  chomp($line);

#next line if is really short, or is junk
  next READ if (length($line) < 20 or $line =~ /FLOW/ or $line =~ /====/);

  $line =~ s/  +/ /g;

  #process if this line contains a date
  if (($date, $time) =
      $line =~ /^(\w{3} \w{3} \d{2} \d{4}) (\d\d\.\d\d\.\d\d)/ ) {
    if (not @value_date = Parse_Date($date)) {
      hdbdie "Invalid date: $date";
    }
    #add the time to the date, and subtract an hour to get start_date_time
    @value_date[3..5] = split /\./, $time;
    @value_date = Add_Delta_DHMS(@value_date, 0,-1,0,0);
    next READ;
  }

  @fields = split  ' ', $line;
  $cur_site_name = $fields[0];

  if (defined $siteid_hash{$cur_site_name}) {
    get_SDI($cur_site_name)
  } else {
    hdbdie "unknown site name: $cur_site_name";
  }

  #File has one datapoint per line, site is indicated by first part of line.
  $site_datatype_id = $site_datatype_hash{$cur_site_name};

  if ($cur_site_name eq 'totals:') {
    $value = $fields[1];
    
  } else {
    $value = $fields[3];
  }
  if ($value == -9999 or $fields[2] eq 'bad') {
    $value = undef;
  }
    
#one of these calls per value, which is inefficent, but quick to code. 
  if ($site_datatype_id) {
    insert_values(@value_date, $site_datatype_id, $value);
#    print @value_date, $site_datatype_id, $value, "\n";

  }
}


$hdb->dbh->disconnect();

exit 0;

#End main program, subroutines below

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

sub insert_values
  {
    my(@value_date) = @_[0..5];
    my($sdi) = $_[6];    
    my($value) = $_[7];

    my $end_date_time = undef;
    my($modsth, $sth);
#    print @_;

    my $modify_data_statement = "
    BEGIN
        modify_r_base_raw(?,'hour',?,?,?, /* sdi, interval, start_date_time, end_date_time (in/out, not used), value */
                          $agen_id,null,'Z', /* overwrite, validation */
                          $collect_id,$load_app_id,$method_id,$computation_id,
                          'Y');                 /*do update? */
    END;";

    my $datestr = sprintf("%02d-%.3s-%4d %02d:%02d:%02d",$value_date[2],
			   Month_to_Text($value_date[1]), $value_date[0],
                          $value_date[3], $value_date[4], $value_date[5]); 

    eval {
#only use these once per prepare/finish, could do better
      $modsth = $hdb->dbh->prepare($modify_data_statement) || die $modsth->errstr;
      
# insert data;
      my  $old_val = undef;
      $old_val = check_value($datestr, $sdi);
      if (!defined $value) {
	#delete data if data is -99.9, otherwise do nothing.
      }
      elsif (defined($old_val) and $old_val == $value) {
        print "data is identical, skipping!\n";
      }
      elsif (!defined($old_val) or $old_val != $value) {
	if (!defined($old_val)) {
	  print "modifying for $sdi, date $datestr, value $value, old_val = undef\n";
	} else {
	  print "modifying for $sdi, date $datestr, value $value, old_val = $old_val\n";
	}
	$modsth->bind_param(1,$sdi);
	$modsth->bind_param(2,$datestr);
	$modsth->bind_param_inout(3,\$end_date_time, 50);
	$modsth->bind_param(4,$value);
	$modsth->execute|| die $modsth->errstr;
      }
      $modsth->finish;
    };
    if ($@) {
      print "$DBI::errstr, $@\n";
      hdbdie "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.";
    }
    else {
      $hdb->dbh->commit;
    }
}


sub get_SDI
{
  my $cur_site= $_[0];

  if (!defined$site_datatype_hash{$cur_site}) {
    $site_datatype_hash{$cur_site} = $hdb->get_SDI($siteid_hash{$cur_site},$datatype);
  }
}

sub set_date_format
{
  eval {
    $hdb->dbh->do("alter session set NLS_DATE_FORMAT = 'DD-MON-YYYY HH24:MI:SS'");
  };
  if ($@) {
    print "$DBI::errstr, $@\n";
    hdbdie "Errors occurred setting of date format.";
    }

}
