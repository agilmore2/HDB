#!/usr/bin/perl -w

use LWP::UserAgent;
use Date::Calc qw(Delta_DHMS);
use Compress::Zlib;
use DBI;
use strict;

#check to see command line usage.
my $progname = `basename $0`;
chop $progname;

if (scalar(@ARGV) <3)
  {
    print "$progname -- program to put USGS flow data into HDB.\n";
    print "Usage: $progname <num days> <hdbusername> <hdbpassword>\n";
    exit (1);
  }

if ($ARGV[0] =~ m/\D/) {
  print "First argument must be numeric.\n";
  print "Usage: $progname <num days> <hdbusername> <hdbpassword>\n";
  exit (1);
}

if ($ARGV[0] >31 ) {
  print "Number of days must be 31 or less.\n";
  print "Usage: $progname <num days> <hdbusername> <hdbpassword>\n";
  exit (1);
}

my $aggdisagg_loc = `which aggDisagg 2>/dev/null`;
chomp $aggdisagg_loc;
if (! -x $aggdisagg_loc ) {
  print "aggDisagg is needed to run this program.
Include the HDB bin directory in your path.\n";
  exit(1);
}

#database handle
# global because used in several sub routines
my $dbh;

#create connection to database
$dbh = connect_to_db($ARGV[1], $ARGV[2]);
#Set date format to ISO, USGS data also in this format
$dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI'");

#this url could be generated from http://waterdata.usgs.gov/nwis/uv
# but the url generated that way has lots of extra junk
# parts of the url:
# program generating the result  : http://waterdata.usgs.gov/nwis/uv
# specify discharge(code 00060)  : parameter_cd=00060
# specify tab delimited format   : format=rdb
# specify gzip compression       : rdb_compression=gz
# specify amount of data (days)  : period=$ARGV[0] (note command line arg)
# specify exact site number match: search_site_no_match_type=exact
# specify multiple site numbers  : multiple_site_no=

#later on, we build a list of usgs site numbers from hdb and append to this url
# you can specify period also in hours by appending h to the end of the arg

my $url = "http://waterdata.usgs.gov/nwis/uv?parameter_cd=00060&format=rdb&rdb_compression=gz&period=$ARGV[0]&search_site_no_match_type=exact&multiple_site_no=";

# can limit this to certain sites by uncommenting and filling out
# the usgs_id like line in the query
my $get_usgs_no_statement = "select a.usgs_id, a.site_id, b.site_datatype_id
from hdb_site a, hdb_site_datatype b, hdb_datatype c, ref_hm_site_datatype d
where a.site_id = b.site_id and
b.datatype_id = c.datatype_id and
b.datatype_id = 18 and
a.usgs_id is not null and
/*a.usgs_id like '0926%' and*/
b.site_datatype_id = d.site_datatype_id and
d.hourly = 'Y'";

# SQL statements
my $insert_data_statement = "insert into r_instant values(?,?,?,11,'Z')";

my $update_data_statement = "update r_instant SET (value) = ( ?), source_id = 11  where (site_datatype_id = ? and date_time = ?) ";

my $check_data_statement = "select value from r_instant where date_time = ? and site_datatype_id = ? ";

# for this program to successfully call the hdb program aggDisagg
# it must be in the path, and a agg disagg operation
# must be defined in the database with the parameters
# as referenced in this select statement

my $find_agg_id_statement = "select agg_disagg_id from ref_agg_disagg
where source_datatype_id = 18 and
source_observation_prd = 'instant' and
dest_datatype_unit_ind = 'D' and
dest_datatype_or_unit_id = 19 and
dest_observation_prd = 'hour' and
agg_disagg_operator = 'ave'";

my $ua = LWP::UserAgent->new;
$ua->agent('USGS Streamflow -> US Bureau of Reclamation HDB dataloader: ' . $ua->agent);
$ua->from('agilmore@uc.usbr.gov');
my $request = HTTP::Request->new();
$request->method('GET');


my (@data,@headers);
my ($usgs_no,$column,$site_no_list);

my $agg_id = find_agg_id() 
or die "No aggregation id for moving flows to r_hour from r_instant exists,
 one must be created.";

my $usgs_sites = get_usgs_sites();

#build list of sites to grab data for
foreach $usgs_no (keys %$usgs_sites) {
  $site_no_list .= $usgs_no . ','
}
# chop off last comma
chop $site_no_list;
$url .= $site_no_list;
$request->uri($url);

# this next function actually gets the data
my $response = $ua->simple_request($request);

# check result
if ($response->is_success) {
  my $status = $response->status_line;
  print "Data read from USGS: $status\n";

  my ($inflated) = Compress::Zlib::memGunzip($response->content);
  @data = split /\n/,$inflated;

#use this file as testing, will have \n on end of lines, regular does not due
# to use in split above
#open INFILE, "/tmp/test.dat";
#@data = <INFILE>;

  my $i = 0;
  until (!defined($data[0])) {
    while (substr ($data[0], 0, 1) eq '#') {
      shift @data;
    }

    #get rid of actual header on data
    shift @data;
    # count number of rows that are not comments after header
    while ($i <= $#data and substr ($data[$i], 0, 1) ne '#') {
      $i++;
    }

    #make a slice of the input data for this site
    my (@cur_values)= @data[0..$i-1];

    # now we have data, mark data as found, find USGS ID
    my (@firstrow) = split /\t/, $data[0];
    $usgs_sites->{$firstrow[1]}->{found_data} = 1;

    # put data into database, handing site id and agg id for use
    # when aggDisagg is called
    insert_values(\@cur_values,
		  $usgs_sites->{$firstrow[1]}->{site_datatype_id},
		  $usgs_sites->{$firstrow[1]}->{site_id},
		  $agg_id);

    @data = @data[$i..$#data];
    $i=0;
  }
} else {
  printf STDERR $response->error_as_HTML;
}

# print error messages for all sites that no data was returned for
foreach $usgs_no (keys %$usgs_sites) {
  if (!defined($usgs_sites->{$usgs_no}->{found_data})) {
    print "No realtime data found for site USGS ID: $usgs_no site_id: $usgs_sites->{$usgs_no}->{site_id}\n";
  }
}

$dbh->disconnect();

#End main program, subroutines below

sub mydie {
  $dbh->rollback or die $dbh->errstr;;
  $dbh->disconnect;
  die @_;
}

sub find_agg_id
{
  my($sth);
  my $value=undef;

  eval {
    $sth = $dbh->prepare($find_agg_id_statement) or mydie $sth->errstr;
    $sth->execute or mydie $sth->errstr;
    $sth->bind_col(1,\$value);
    $sth->fetch;
    $sth->finish();
  };

  if ($@) { # something screwed up
    print "$DBI::errstr, $@\n";
    mydie "Errors occurred during selection of data.";
  }

  return $value;
}


sub get_usgs_sites
{
  $dbh->{FetchHashKeyName} = 'NAME_lc';

  eval {
    my $hashref = $dbh->selectall_hashref($get_usgs_no_statement,1);
  };

  if ($@) { # something screwed up
    print "$DBI::errstr, $@\n";
    mydie "Errors occurred during selection of data.";
  }

  return $dbh->selectall_hashref($get_usgs_no_statement,1);
}

sub check_value
{
  my($check_date)=$_[0];
  my($site_datatype_id)=$_[1];
  my($sth, $value);

  eval {
    $sth = $dbh->prepare($check_data_statement) or mydie $sth->errstr;
    $sth->bind_param(1,$check_date);
    $sth->bind_param(2,$site_datatype_id);

    $sth->execute or mydie $sth->errstr;
    $sth->bind_col(1,\$value);
    $sth->fetch;
    $sth->finish();
  };

  if ($@) { # something screwed up
    print "$DBI::errstr, $@\n";
    mydie "Errors occurred during selection of data.";
  }
  return $value;
}

# find number of hours to run aggDisagg given two timestamps
# format of timestamps is 'YYYY-MM-DD HH:MI'
sub agg_hours
{
  my $start_ts = $_[0];
  my $end_ts = $_[1];

  my ($startdate,$starttime) = split / /, $start_ts;
  my ($startyear,$startmonth,$startday) = split /-/, $startdate;
  my ($starthour,$startminute) = split /:/, $starttime;

  my ($enddate,$endtime) = split / /, $end_ts;
  my ($endyear,$endmonth,$endday) = split /-/, $enddate;
  my ($endhour,$endminute) = split /:/, $endtime;

  my ($Dd,$Dh,$Dm,$Ds) =
  Delta_DHMS($startyear,$startmonth,$startday,
	     $starthour,$startminute,0,
	     $endyear,$endmonth,$endday,
	     $endhour,$endminute,0);

  return $Dd*24 + $Dh + 1;
}


sub insert_values
{
  my @data = @{$_[0]};
  my $cur_sdi = $_[1];
  my $cur_site = $_[2];
  my $agg_id = $_[3];

  #fixed the url encoding to only retrieve one
  # parameter, so no longer need this, but could need it again
  my $column = 3;

  my $i = 0;
  my $first_date = undef;
  my ($value, $value_date, $updated_date, @row);
  my ($line, $date, $datestr, $old_val);

#  print @data;
#  return;

  eval
  {
    my ($insth,$upsth);
    $insth = $dbh->prepare($insert_data_statement) or mydie $insth->errstr;
    $upsth = $dbh->prepare($update_data_statement) or mydie $upsth->errstr;

    # insert or update or do nothing for each datapoint;
    foreach $line (@data)
    {
      @row = split /\t/, $line;
      $value_date = $row[2];
      $value = $row[$column];
      if ($value) {
	$value =~ s/,//g;
      }

      # find the previous value, if any for this point
      undef $old_val;
      $old_val = check_value($value_date, $cur_sdi);
# check if value from source is known
      if (!defined $value or $value eq '') {
#	print "data missing: $cur_sdi, date $value_date\n";
	next;
      } elsif ($value =~ m/Ice/) { # check for Ice
#	print "Iced up: $cur_sdi, date $value_date: $value\n";
	next;
      } elsif ($value =~ m/[^0-9\.]/) { # check for other text, complain
	print "data corrupted: $cur_sdi, date $value_date: $value\n";
	next;
      } elsif (!defined $old_val) { # insert, source value is good
#	print "inserting for $cur_sdi, date $value_date, value $value, old_val = undefined\n";

	$insth->bind_param(1,$cur_sdi);
	$insth->bind_param(2,$value_date);
	$insth->bind_param(3,$value);
	$insth->execute or mydie $insth->errstr;
	if (!defined($first_date)) { # mark that data has been changed
	  $first_date = $value_date;
	}
	$updated_date = $value_date;
      } elsif ($old_val == $value) { # source and database are same, do nothing
	next;
      } elsif ($old_val != $value) { # update, source and database differ
#	print "updating for $cur_sdi, date $value_date, value $value, old_val = $old_val\n";
	#update instead of insert
	$upsth->bind_param(1,$value);
	$upsth->bind_param(2,$cur_sdi);
	$upsth->bind_param(3,$value_date);
	$upsth->execute or mydie $upsth->errstr;
	if (!defined($first_date)) { # mark that data has changed
	  $first_date = $value_date;
	}
	$updated_date = $value_date;
      }
    }
    $upsth->finish;
    $insth->finish;
  }; # semicolon here because of use of eval

  if ($@) { # something screwed up in insert/update
    print "$DBI::errstr, $@\n";
    mydie "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.";
  } elsif ($first_date) {	# commit the data
    $dbh->commit or mydie $dbh->errstr;

    eval {
      $dbh->do("update ref_hm_site_datatype set (max_hourly_date) = ('$updated_date') where (site_datatype_id = $cur_sdi) ");
    };

    if ($@) { # something screwed up
      print "$DBI::errstr, $@\n";
      mydie "Errors occurred during selection of data.";
    }

    $dbh->commit or mydie $dbh->errstr;


    # now call aggDisagg (an HDB application) to move data up to
    # r_hour table

    # find begin and end times
    my $num_hours = agg_hours($first_date,$updated_date);

    # redo some of the work done in agg_hours to get args for command line
    my ($startdate,$starttime) = split / /, $first_date;
    my ($startyear,$startmonth,$startday) = split /-/, $startdate;

    system qq{aggDisagg $ARGV[1] $ARGV[2] $agg_id 1 n n n r d $num_hours '$startmonth/$startday/$startyear $starttime' $cur_site};
#    print qq{aggDisagg $ARGV[1] $ARGV[2] $agg_id 1 n n n r d $num_hours '$startmonth/$startday/$startyear $starttime' $cur_site\n};
  } else {
    print "No data updated for $cur_site\n";
  }
}

#global symbol for use in local die subroutine
my $psswd_dbh;

sub passdie {
    $psswd_dbh->disconnect;
    die @_;
}

# HDB does something interesting with the app_user 
# app_user has the permission to insert into the database, but only
# if the app_role is enabled, which is not by default
# the applications get around this by connecting to the database as 
# psswd_user (from the environment) and getting the password to 
# turn on this role, and then connecting as app_user and 
# enabling this role.
# This code does that, while also taking the name of the database
# from the environment, and the app_user name and password from the
# command line
sub connect_to_db
{
  my $user=$_[0];
  my $pass=$_[1];
  my ($approlepass, $approle, $dbname, $dbh);
  my ($psswduser, $psswd_statement, $psswdsth);

  if (!defined($psswduser = $ENV{PSSWD_USER})) {
    die "Environment variable PSSWD_USER not set...\n";
  }
  if (!defined($dbname = $ENV{HDB_LOCAL})) {
    die "Environment variable HDB_LOCAL not set...\n";
  }
  if (!defined($approle = $ENV{APP_ROLE})) {
    die "Environment variable APP_ROLE not set...\n";
  }

  $psswd_statement =
  "select psswd from role_psswd where UPPER(ROLE) = UPPER('$approle')";

  # create connections, exceptions on error, no autocommit
  $psswd_dbh = DBI->connect("dbi:Oracle:$dbname", $psswduser, $dbname,
			    {RaiseError => 1, AutoCommit => 0 })
			    or mydie $DBI::errstr;

  # find the password for the app role that allows writing
  $psswdsth = $psswd_dbh->prepare($psswd_statement)
              or passdie $psswdsth->errstr;
  $psswdsth->execute or passdie $psswdsth->errstr;

  $psswdsth->bind_col(1,\$approlepass);
  $psswdsth->fetch;
  $psswdsth->finish;
  $psswd_dbh->disconnect;

  $dbh = DBI->connect("dbi:Oracle:$dbname", $user, $pass,
		      { RaiseError => 1, AutoCommit => 0 })
                      or die $DBI::errstr;

  # set the app role which allows for writing to database
  $dbh->do("set role $approle identified by $approlepass")
          or die $DBI::errstr;

  return $dbh;
}

