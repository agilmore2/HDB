#!/usr/bin/perl -w

#insert HDB library

use lib "$ENV{HDB_ENV}/perlLib/lib";
# Don't need
#use lib "$ENV{HDB_ENV}/perlLib/lib/i386-linux";

use LWP::UserAgent;
use Date::Calc qw(Delta_DHMS);
use Compress::Zlib;
use File::Basename;
use Data::Dumper;
use Hdb;

use strict;

#Version Information
my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/\$//;

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

get_app_ids();

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
# Sigh, they added another line to make things more complicated 9/2003
#
agency_cd	site_no	datetime	02_00060
5s      15s     16s     14s
USGS	09261000	2003-04-29 00:00	7,250
USGS	09261000	2003-04-29 00:15	7,220
...
# next site header
agency_cd	site_no	datetime	02_00060
5s      15s     16s     14s
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
STATION:
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
  # shift away extra header line
  shift @data;

  # count number of rows that are not comments after header
  while ($i <= $#data and substr ($data[$i], 0, 1) ne '#') {
    $i++;
  }

  if ($i eq 0) { # USGS has started putting completely empty headers in for stations not in realtime system?
    next STATION;
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
      print "No data updated for $usgs_no\n";
    } else {
      print "Data updated from $first_date to $updated_date for $usgs_no\n";
    }
  }

  # now call aggDisagg (an HDB application) to move data up to
  # r_hour table, uses dates returned from insert from previous function
  # 
  # FIXME: Check to see if specification exists for hourly and daily derivations

  @data = @data[$i..$#data];
  $i=0;
}

# print error messages for all sites that no data was returned for
foreach my $usgs_no (keys %$usgs_sites) {
  if (!defined($usgs_sites->{$usgs_no}->{found_data})) {
    print STDERR "No realtime data found for site: $usgs_sites->{$usgs_no}->{site_name}   USGS ID: $usgs_no\n";
  }
}

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

  $nameid->{agen}->{name} = 'United States Geological Survey';
  $nameid->{collect}->{name} = '(see agency)';
  $nameid->{load_app}->{name} = 'usgs2hdb.pl';
  $nameid->{method}->{name} = 'unknown';
  $nameid->{computation}->{name} = 'unknown';

  $hdb->get_app_ids($nameid);

  $agen_id = $nameid->{agen}->{id};
  $collect_id = $nameid->{collect}->{id};
  $load_app_id = $nameid->{load_app}->{id};
  $method_id = $nameid->{method}->{id};
  $computation_id = $nameid->{computation}->{id};

}

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

sub get_usgs_sites
{
  my $site_num_list;
  my $id_limit_clause = '';

  if ($site_num_list = shift) {
    $id_limit_clause = "a.usgs_id in ( $site_num_list ) and";
  }

  my $get_usgs_no_statement = "select a.usgs_id, a.site_id, b.site_datatype_id, a.site_name
from hdb_site a, hdb_site_datatype b, hdb_datatype c, ref_hm_site_datatype d
where a.site_id = b.site_id and
b.datatype_id = c.datatype_id and
b.datatype_id = 18 and
a.usgs_id is not null and $id_limit_clause
b.site_datatype_id = d.site_datatype_id and
d.hourly = 'Y'
order by usgs_id";

  $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

  my $hashref;

  eval {
    $hashref = $hdb->dbh->selectall_hashref($get_usgs_no_statement,1);
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    die "Errors occurred during automatic selection of USGS gage ids.\n";
  }

  return $hashref;
}

sub check_value
{
  my($check_date)=$_[0];
  my($site_datatype_id)=$_[1];
  my($sth, $value);

  my $check_data_statement = "select value from r_base
where start_date_time = ? and site_datatype_id = ? and interval = 'instant'";

  eval {
    $sth = $hdb->dbh->prepare($check_data_statement);
    $sth->bind_param(1,$check_date);
    $sth->bind_param(2,$site_datatype_id);

    $sth->execute;
    $sth->bind_col(1,\$value);
    $sth->fetch;
    $sth->finish();
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    die "Errors occurred during selection of data already in database.\n";
  }
  return ($value);
}

sub insert_values
{
  my @data = @{$_[0]};
  my $cur_sdi = $_[1];

  #fixed the url encoding to only retrieve one
  # parameter, so no longer need this, but could need it again
  my $column = 3;

  my $i = 0;
  my ($first_date, $end_date_time) = undef;
  my ($value, $value_date, $updated_date);
  my ($line, @row);
  my $old_val;

# SQL statements

  my $modify_data_statement = "
    BEGIN
        modify_r_base_raw(?,'instant',?,?,?, /* sdi, interval, start_date_time, end_date_time (in/out, not used), value */
                          $agen_id,null,'Z', /* overwrite, validation */
                          $collect_id,$load_app_id,$method_id,$computation_id,
                          'Y');                 /*do update? */
    END;";

  eval
  {
    my ($modsth);
    $modsth = $hdb->dbh->prepare($modify_data_statement);

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
      if (!defined ($value) or $value eq '') {
	print "data missing: $cur_sdi, date $value_date\n" if defined($debug);
	next;
      } elsif ($value =~ m/Ice/) { # check for Ice
	print "Iced up: $cur_sdi, date $value_date: $value\n"
	     if defined($debug);
	next;
      } elsif ($value =~ m/[^0-9\.]/) { # check for other text, complain
	print "data corrupted: $cur_sdi, date $value_date: $value\n";
	next;
      } elsif (defined($old_val) and $old_val == $value) {
	next; # source and database are same, do nothing
      } elsif (!defined($old_val) or $old_val != $value) {
	# update or insert, source and database differ (or database value does not exist)
        if (defined($debug)) {
          if (!defined($old_val)) {
            print "modifying for $cur_sdi, date $value_date, value $value, old_val = undef\n";
          } else {
            print "modifying for $cur_sdi, date $value_date, value $value, old_val = $old_val\n";
          }
        }
	$modsth->bind_param(1,$cur_sdi);
	$modsth->bind_param(2,$value_date);
	$modsth->bind_param_inout(3,\$end_date_time,50);
	$modsth->bind_param(4,$value);
	$modsth->execute;
	
	$end_date_time = undef;
	if (!defined($first_date)) { # mark that data has changed
	  $first_date = $value_date;
	}
	$updated_date = $value_date;
      }
    }
    $modsth->finish;

  }; # semicolon here because of use of eval

  if ($@) { # something screwed up in insert/update
    print $hdb->dbh->errstr, " $@\n";
    die "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.\n";
  } elsif ($first_date) {	# commit the data
    $hdb->dbh->commit or die $hdb->dbh->errstr;
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
  print "$progname version $verstring\n";
  exit 1;
}
