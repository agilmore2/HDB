#!/usr/bin/perl -w

#insert HDB library

use lib "$ENV{HDB_ENV}/perlLib/lib";
use lib "$ENV{HDB_ENV}/perlLib/lib/sun4-solaris";

use LWP::UserAgent;
use Date::Calc qw(Delta_DHMS);
use Compress::Zlib;
use File::Basename;
use Data::Dumper;
use Hdb;

use strict;


#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my $insertflag = 1;

my ($readfile, $runagg, $printurl, $debug);
my ($numdays, $hdbuser, $hdbpass, $site_num_list);

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
  } elsif ($arg =~ /-f/) {	# get file to read from
    $readfile = shift(@ARGV);
    if (! -e $readfile) {
      print "file not found: $readfile";
      exit 1;
    }
  } elsif ($arg =~ /-a/) {	# get run aggDisagg flag
    $runagg=1;
  } elsif ($arg =~ /-w/) {	# get print URL flag
    $printurl=1;
  } elsif ($arg =~ /-d/) {	# get debug flag
    $debug=1;
  } elsif ($arg =~ /-i/) {	# get usgs id
    $site_num_list .= shift(@ARGV) . ',';
  } elsif ($arg =~ /-n/) {	# get number of days
    $numdays=shift(@ARGV);
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

# if user specified usgs gage ids, chop off last comma
if (defined($site_num_list)) {
  chop $site_num_list;
  if ($site_num_list =~ /[^0-9,]/ ) {
    die "ERROR: $site_num_list\ndoes not look like USGS id.\n";
  }
}

if (!defined($numdays)) {
  $numdays = 1;
}

if (!defined($hdbuser) || !defined($hdbpass)) {
  usage();
}

if ($numdays =~ m/\D/) {
  print "Number of days (argument to -n) must be numeric.\n";
  usage();
}

if ($numdays >31 ) {
  print "Number of days (argument to -n) must be 31 or less.\n";
  usage();
}


if (defined($runagg)) {
  my $aggdisagg_loc = `which aggDisagg 2>/dev/null`;
  chomp $aggdisagg_loc;
  if (! -x $aggdisagg_loc ) {
    print STDERR "Cannot find aggDisagg in your path, will not aggregate.
To fix this, include the HDB bin directory in your path.\n";
    undef $runagg;
  }
}

#HDB database interface
# global because used in several sub routines
my $hdb = Hdb->new;

my $dbname;
if (!defined($dbname = $ENV{HDB_LOCAL})) {
  die "Environment variable HDB_LOCAL not set...\n";
}
#create connection to database
$hdb->connect_to_db($dbname, $hdbuser, $hdbpass);
$hdb->set_approle();

#Set date format to ISO, USGS data also in this format
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI'")
  or die "Unable to set NLS_DATE_FORMAT to ISO standard\n";

my ($usgs_sites);

if (defined ($site_num_list)) {
  $usgs_sites = get_usgs_sites($site_num_list);
} else {
  $usgs_sites = get_usgs_sites();
  $site_num_list = build_site_num_list($usgs_sites);
}

=FORMAT

The data is expected in the following format:
# large number of comments delimited by pound signs (#)
# including a disclaimer, a retrieved: timestamp, a description of the format
# a list of sites contained in the file
# the first per site header is appended to the site header
# as a list of data available for each site, and which data is included in the
# file
# Then
#
agency_cd	site_no	datetime	02_00060
USGS	09261000	2003-04-29 00:00	7,250
USGS	09261000	2003-04-29 00:15	7,220
...
# next site header
agency_cd	site_no	datetime	02_00060
USGS	09261000	2003-04-29 00:00	7,250
USGS	09261000	2003-04-29 00:15	7,220
...

The last character of the file is a newline, after the last datapoint

This program relies upon the presence of the agency_cd in the first column
of the header, and upon the site_no, datetime and values to be in the
columns as in the example.

The USGS web site allows for multiple parameters to be given in the same file.
The insert values subroutine uses the $column variable to signify
which column to expect the value to be inserted.

=cut

my (@data);

if (defined($readfile))
{
  open INFILE, "$readfile" or die "Cannot open $readfile: $!\n";
  @data = <INFILE>;
  print @data if defined($debug);
  print Dumper(@data) if defined($debug);
  chomp @data;
} else { # attempt to get data from USGS web page

  my $url = build_url($site_num_list);
  if (defined($printurl) or defined($debug)) {
    print "$url\n";
  }

  my ($ua, $request) = build_web_request();
  $request->uri($url);

  # this next function actually gets the data
  my $response = $ua->simple_request($request);

  # check result
  if ($response->is_success) {
    my $status = $response->status_line;
    print "Data read from USGS: $status\n";

    # assume that response is compressed, and expand it
    my ($inflated) = Compress::Zlib::memGunzip($response->content);
    if (!$inflated) {
      die "no data returned from USGS, exiting.\n";
    }
    print $inflated if defined($debug);
    @data = split /\n/, $inflated;
  } else { # ($response->is_error)
    printf STDERR $response->error_as_HTML;
    die "Error reading from USGS web page, cannot continue\n";
  }
}

#this next section runs through the data destructively,
# shortening the @data array after a insert
# the insert is passed a slice of @data

my $i = 0;
until (!defined($data[0])) {
  # remove leading comments and comments between sites
  while (substr ($data[0], 0, 1) eq '#') {
    shift @data;
  }

  #get rid of actual header on data
  my @header = split /\t/, shift @data;
  if ($header[0] ne "agency_cd") {
    print "@header\n";
    die "Data is not USGS website tab delimited format!\n";
  }
  # count number of rows that are not comments after header
  while ($i <= $#data and substr ($data[$i], 0, 1) ne '#') {
    $i++;
  }

  #make a slice of the input data for this site
  my (@cur_values)= @data[0..$i-1];

  # now we have data, mark data as found in USGS ID
  # assume that the format holds, and the id is in second column
  my (@firstrow) = split /\t/, $data[0];
  my $usgs_no = $firstrow[1];

  $usgs_sites->{$usgs_no}->{found_data} = 1;
  my $cur_sdi = $usgs_sites->{$usgs_no}->{site_datatype_id};
  my $cur_site = $usgs_sites->{$usgs_no}->{site_id};


  # put data into database, handing site id and agg id for use
  # when aggDisagg is called, unless testflag is set
  # function returns date of first value and last value updated
  my ($first_date, $updated_date);

  if (defined($insertflag)) {
  #tell user something, so they know it is working
    print "Working on USGS gage number: $usgs_no\n";
    ($first_date, $updated_date) = insert_values(\@cur_values, $cur_sdi);
    if (!defined($first_date)) {
      print "No data updated for $cur_site\n";
    }
  }

  # now call aggDisagg (an HDB application) to move data up to
  # r_hour table, uses dates returned from insert from previous function
  # 
  if (defined($runagg) and (defined($first_date) or !defined($insertflag))) {
    if (!defined($insertflag)) { # no insert, need dates
      $first_date = $firstrow[2];
      $updated_date = (split /\t/, $data[$i-1])[2];
    }

    # find number of hours and properly formatted date and time
    my ($num_hours, $agg_date, $aggtime) = agg_hours($first_date,
						     $updated_date,
						     $cur_sdi);

    my ($houragg_id, $dayagg_id) = find_agg_id()
    or die "No aggregation id for moving flows from r_instant or r_hour exist, one must be created.\n";

    my $cmd = qq{aggDisagg $hdbuser $hdbpass $houragg_id 1 n n n r d $num_hours '$agg_date $aggtime' $cur_site >logs/aggDisagg_usgshour$usgs_no.out 2>logs/aggDisagg_usgshour$usgs_no.err};
    print "$cmd\n" if defined($debug);
    system $cmd unless defined($debug);

    my $num_days =  sprintf("%d", $num_hours/24);
    $num_days++;

    $cmd = qq{aggDisagg $hdbuser $hdbpass $dayagg_id 1 n n n r d $num_days '$agg_date' $cur_site >logs/aggDisagg_usgsday$usgs_no.out 2>logs/aggDisagg_usgsday$usgs_no.err};
    print "$cmd\n" if defined($debug);
    system $cmd unless defined($debug);
  }

  @data = @data[$i..$#data];
  $i=0;
}

# print error messages for all sites that no data was returned for
foreach my $usgs_no (keys %$usgs_sites) {
  if (!defined($usgs_sites->{$usgs_no}->{found_data})) {
    print STDERR "No realtime data found for site USGS ID: $usgs_no site_id: $usgs_sites->{$usgs_no}->{site_id}\n";
  }
}

#End main program, subroutines below

sub build_url
{
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

  my $url = "http://waterdata.usgs.gov/nwis/uv?parameter_cd=00060&format=rdb&rdb_compression=gz&period=$numdays&search_site_no_match_type=exact&multiple_site_no=";

  $url .= $_[0];

  return $url;
}

sub build_site_num_list
{
  my $usgs_sites = $_[0];
  my ($usgs_num, $site_num_list);

  #build list of sites to grab data for
  foreach $usgs_num (keys %$usgs_sites) {
    $site_num_list .= $usgs_num . ','
  }
  chop $site_num_list;
  return $site_num_list;
}

sub build_web_request
{
  my $ua = LWP::UserAgent->new;
  $ua->agent('USGS Streamflow -> US Bureau of Reclamation HDB dataloader: ' . $ua->agent);
  $ua->from('agilmore@uc.usbr.gov');
  my $request = HTTP::Request->new();
  $request->method('GET');
  return ($ua, $request);
}


sub find_agg_id
{
  my $sth;
  my $hourvalue=undef;
  my $dayvalue =undef;

# for this program to successfully call the hdb program aggDisagg
# an agg disagg operation
# must be defined in the database with the parameters
# as referenced in this select statement

  my $find_hour_agg_id_statement = "select agg_disagg_id from ref_agg_disagg
where source_datatype_id = 18 and
source_observation_prd = 'instant' and
dest_datatype_unit_ind = 'D' and
dest_datatype_or_unit_id = 19 and
dest_observation_prd = 'hour' and
agg_disagg_operator = 'ave'";

  my $find_day_agg_id_statement = "select agg_disagg_id from ref_agg_disagg
where source_datatype_id = 19 and
source_observation_prd = 'hour' and
dest_datatype_unit_ind = 'D' and
dest_datatype_or_unit_id = 19 and
dest_observation_prd = 'day' and
agg_disagg_operator = 'ave'";

  eval {
    $sth = $hdb->dbh->prepare($find_hour_agg_id_statement);
    $sth->execute;
    $sth->bind_col(1,\$hourvalue);
    $sth->fetch;
    $sth->finish();

    $sth = $hdb->dbh->prepare($find_day_agg_id_statement);
    $sth->execute;
    $sth->bind_col(1,\$dayvalue);
    $sth->fetch;
    $sth->finish();
  };

  if ($@) { # something screwed up
    print "$hdb->dbh->errstr, $@\n";
    die "Errors occurred during selection of aggDisagg id for aggregation.\n";
  }

  return $hourvalue, $dayvalue;
}

sub get_usgs_sites
{
  my $site_num_list;
  my $id_limit_clause = '';

  if ($site_num_list = shift) {
    $id_limit_clause = "a.usgs_id in ( $site_num_list ) and";
  }

  my $get_usgs_no_statement = "select a.usgs_id, a.site_id, b.site_datatype_id
from hdb_site a, hdb_site_datatype b, hdb_datatype c, ref_hm_site_datatype d
where a.site_id = b.site_id and
b.datatype_id = c.datatype_id and
b.datatype_id = 18 and
a.usgs_id is not null and $id_limit_clause
b.site_datatype_id = d.site_datatype_id and
d.hourly = 'Y'";

  $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

  my $hashref;

  eval {
    $hashref = $hdb->dbh->selectall_hashref($get_usgs_no_statement,1);
  };

  if ($@) { # something screwed up
    print "$hdb->dbh->errstr, $@\n";
    die "Errors occurred during automatic selection of USGS gage ids.\n";
  }

  return $hashref;
}

sub check_value
{
  my($check_date)=$_[0];
  my($site_datatype_id)=$_[1];
  my($sth, $value, $source);

  my $check_data_statement = "select value, source_id from r_instant
where date_time = ? and site_datatype_id = ? ";

  eval {
    $sth = $hdb->dbh->prepare($check_data_statement);
    $sth->bind_param(1,$check_date);
    $sth->bind_param(2,$site_datatype_id);

    $sth->execute;
    $sth->bind_col(1,\$value);
    $sth->bind_col(2,\$source);
    $sth->fetch;
    $sth->finish();
  };

  if ($@) { # something screwed up
    print "$hdb->dbh->errstr, $@\n";
    die "Errors occurred during selection of data already in database.\n";
  }
  return ($value, $source);
}

#Find what the maximum hourly date is for this sdi, and update hm 
#table if necessary
#hmHourly should have updated the ref_hm_site_datatype table if it put
# new data
# we want to run aggDisagg to get all new data into r_hour,
# even if source is hydromet
sub get_update_max_date
{
  my($cur_sdi)=$_[0];
  my($usgs_max_date)=$_[1];
  my($sth, $max_date);

  my $check_date_statement = "select max_hourly_date
from ref_hm_site_datatype
where site_datatype_id = ?
and max_hourly_date >= ? ";

  eval {
    $sth = $hdb->dbh->prepare($check_date_statement);
    $sth->bind_param(1,$cur_sdi);
    $sth->bind_param(2,$usgs_max_date);

    $sth->execute;
    $sth->bind_col(1,\$max_date);

    if (!$sth->fetch) { #no value returned, USGS data newer
      $sth->finish();
      $max_date = $usgs_max_date;
      $hdb->dbh->do("update ref_hm_site_datatype
set (max_hourly_date) = ('$max_date') where (site_datatype_id = $cur_sdi) ");

      $hdb->dbh->commit;

      print "Updated ref_hm_site_datatype: max_hourly_date $max_date, site_datatype_id $cur_sdi" if defined ($debug);
    } else { # hydromet data newer
      print "No new data inserted, usgs_date $usgs_max_date, max_hourly_date $max_date, site_datatype_id $cur_sdi" if defined ($debug);
    }

    $sth->finish();
  };

  if ($@) { # something screwed up
    print "$hdb->dbh->errstr, $@\n";
    die "Errors occurred during update of ref_hm_site_datatype.\n";
  }

  return $max_date;
}


# find number of hours to run aggDisagg given two timestamps
# format of timestamps is 'YYYY-MM-DD HH:MI'
# also return date and times formatted for the begin timestamp for aggDisagg
sub agg_hours
{
  my $start_ts = $_[0];
  my $end_ts = $_[1];
  my $cur_sdi = $_[2];

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
	     $endhour,$endminute,0) or die "Date Calculation failed.\n";

  #return number of hours, and properly formatted date and time
  return $Dd*24 + $Dh + 1, "$startmonth/$startday/$startyear", $starttime;
}


sub insert_values
{
  my @data = @{$_[0]};
  my $cur_sdi = $_[1];

  #fixed the url encoding to only retrieve one
  # parameter, so no longer need this, but could need it again
  my $column = 3;

  my $i = 0;
  my $first_date = undef;
  my ($value, $value_date, $updated_date);
  my ($line, @row);
  my ($old_val, $old_source);

# SQL statements
  my $insert_data_statement = "insert into r_instant values(?,?,?,11,'Z')";

  my $update_data_statement = "update r_instant
SET (value) = ( ?), source_id = 11
where (site_datatype_id = ? and date_time = ?) ";

  eval
  {
    my ($insth,$upsth);
    $insth = $hdb->dbh->prepare($insert_data_statement);
    $upsth = $hdb->dbh->prepare($update_data_statement);

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
      ($old_val, $old_source) = check_value($value_date, $cur_sdi);
# check if value from source is known
      if (!defined $value or $value eq '') {
	print "data missing: $cur_sdi, date $value_date\n" if defined($debug);
	next;
      } elsif ($value =~ m/Ice/) { # check for Ice
	print "Iced up: $cur_sdi, date $value_date: $value\n"
	     if defined($debug);
	next;
      } elsif ($value =~ m/[^0-9\.]/) { # check for other text, complain
	print "data corrupted: $cur_sdi, date $value_date: $value\n";
	next;
      } elsif (!defined $old_val) { # insert, source value is good
	print "inserting for $cur_sdi, date $value_date, value $value, old_val = undefined\n" if defined($debug);

	$insth->bind_param(1,$cur_sdi);
	$insth->bind_param(2,$value_date);
	$insth->bind_param(3,$value);
	$insth->execute;
	if (!defined($first_date)) { # mark that data has been changed
	  $first_date = $value_date;
	}
	$updated_date = $value_date;
      } elsif ($old_val == $value and $old_source == 11) {
	next; # source and database are same, do nothing
      } elsif ($old_val != $value or $old_source != 11 ) {
	# update, source and database differ (or source id is different!)
	print "updating for $cur_sdi, date $value_date, value $value, old_val = $old_val old_source = $old_source\n"  if defined($debug);
	#update instead of insert
	$upsth->bind_param(1,$value);
	$upsth->bind_param(2,$cur_sdi);
	$upsth->bind_param(3,$value_date);
	$upsth->execute;
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
    print "$hdb->dbh->errstr, $@\n";
    die "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.\n";
  } elsif ($first_date) {	# commit the data
    $hdb->dbh->commit or die $hdb->dbh->errstr;

    # update hm driver table, and check for newer hydromet data
    $updated_date = get_update_max_date($cur_sdi, $updated_date);
  }
  return $first_date, $updated_date;
}

sub usage
{
  print STDERR <<"ENDHELP";
$progname [ -h | -v ] | [ options ]
Retrieves USGS flow data and inserts into HDB, runs aggDisagg to r_hour.
Example: $progname -u app_user -p <hdbpassword> -n 2 -i 09260000 -a

  -h               : This help
  -v               : Version
  -u <hdbusername> : HDB application user name (REQUIRED)
  -p <hdbpassword> : HDB password (REQUIRED)
  -i <usgs_id>     : USGS gage id to look for. Must be in HDB. More than one
                     may be specified with -i id1,id2 or -i id1 -i id2
                     If none specified, will load all gages set up in HDB.
  -t               : Test retrieval, but do not insert data into DB
  -f <filename>    : Filename instead of live web
  -n <1..31>       : Days ending now for retrieval (default 1, max 31)
  -a               : Run aggDisagg after loading instantaneous data
  -w               : Web address (URL developed to get USGS data)
  -d               : Debugging output
ENDHELP

  exit (1);
}

sub version
{
  my $verstring = '$Revision$';
  $verstring =~ s/^\$Revision: //;
  $verstring =~ s/\$//;

  print "$progname version $verstring\n";
  exit 1;
}
