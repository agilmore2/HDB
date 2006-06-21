#!/usr/bin/perl -w

#insert HDB library

use lib "$ENV{HDB_ENV}/perlLib/lib";
# Don't need
#use lib "$ENV{HDB_ENV}/perlLib/lib/i386-linux";

use LWP::UserAgent;
use Date::Calc qw(Delta_DHMS Add_Delta_Days Month_to_Text Decode_Date_EU Today);
use Compress::Zlib;
use File::Basename;
use Data::Dumper;
use Hdb;

use strict;

#Version Information
my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my $insertflag = 1;
my $overwrite = 'null';

my ($readfile, $runagg, $printurl, $debug, $flowtype, $db_site);
my ($numdays, $hdbuser, $hdbpass, $site_num_list);
#scalar and array versions, the scalars are a string of the arrays
my ($begindate, $enddate, @begindate, @enddate);

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
  } elsif ($arg =~ /-l/) {	# get flow type
    $flowtype = shift(@ARGV);
  } elsif ($arg =~ /-w/) {	# get print URL flag
    $printurl=1;
  } elsif ($arg =~ /-d/) {	# get debug flag
    $debug=1;
  } elsif ($arg =~ /-o/) {	# get overwrite flag
    $overwrite="'O'";
  } elsif ($arg =~ /-i/) {	# get usgs id
    $site_num_list .= shift(@ARGV) . ','
  } elsif ($arg =~ /-n/) {	# get number of days
    $numdays=shift(@ARGV);
  } elsif ($arg =~ /-b/) {	# get begin date
    $begindate=shift(@ARGV);
    if (not @begindate = Decode_Date_EU($begindate)) {
      print "Error in begin date specified: $begindate\n";
      usage();
    }
  } elsif ($arg =~ /-e/) {	# get end date
    $enddate=shift(@ARGV);
    if (not @enddate = Decode_Date_EU($enddate)) {
      print "Error in end date specified: $enddate\n";
      usage();
    }
  } elsif ($arg =~ /-u/) {	# get hdb user
    $hdbuser=shift(@ARGV);
  } elsif ($arg =~ /-p/) {	# get hdb passwd
    $hdbpass=shift(@ARGV);
  } elsif ($arg =~ /-s/) {	# get database site to load
    $db_site=shift(@ARGV);
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
    die("ERROR: $site_num_list\ndoes not look like USGS id(s).\n");
  }
}

# check db_site_code
if (!defined($db_site)) {
  die("ERROR: db site code must be specified! (use -s <DB> on command line)");
}

#handle flowtype, realtime, provisional, or official
#check if flowtype is defined, if not, we assume realtime
if (!defined($flowtype)) {
  $flowtype = 'r';
}

if (! ($flowtype =~ /^[rop]$/)) {
  print "Error, invalid flow type defined: $flowtype.";
  print "Specify one of r, p, or o (realtime, provisional, or official)";
  usage();
}

#do not have table set up to specify which official data to automatically get
if (($flowtype eq "o") and ! defined($site_num_list)) {
  print "Lookout! You have specified loading all sites in HDB!\n";
  print "Are you sure you want to do this? (y/n) \n";
  my $yn;
  read STDIN, $yn,2;
  chomp $yn;
  if ($yn eq 'n') {
    exit 0;
  } elsif ( $yn eq 'y') {
    ; # do nothing
  } else {
    print "I'm sorry, I can't do that, Dave.\n";
    exit 1;
  }
}


#flowtype defines the title of the datasource, and we retrieve all other
# data corresponding to that source from the database

my %title;
$title{r}='USGS Realtime';
$title{p}='USGS Provisional';
$title{o}='USGS Official';

#Truth table and results for numdays, begindate and enddate
# errors are all defined, only enddate defined.
# everything else gets fixed up.
# Num Days | Beg Date | End Date | Result
# nothing defined                   error
#  def                              end date = now, beg date = enddate-numdays
#  def        def                   enddate = beg date + numdays
#             def                   endate = now, check max num days?
#             def         def       ok
#                         def       error, what is beg date?
#  def                    def       beg date = end date - numdays
#  def        def         def       error, not checking to see if consistent
#
#  We end up with a begin date and an end date
if (defined($numdays)) {
  if ($numdays =~ m/\D/) {
    print "Number of days (argument to -n) must be numeric.\n";
    usage();
  }

  if ($numdays >31 and $flowtype eq 'i') {
    print "Number of days (argument to -n) must be 31 or less for realtime data.\n";
    usage();
  }

  if (@begindate and @enddate) {
    print "Error, overspecified dates, all of -n, -b, -e specified!\n";
    exit 1;
  } elsif (@begindate) {
    @enddate = Add_Delta_Days(@begindate, $numdays);
  } elsif (@enddate) {
    @begindate = Add_Delta_Days(@enddate, -$numdays);
  } else {
    @enddate = Today();
    @begindate = Add_Delta_Days(@enddate, -$numdays);
  }
}
else {
  if (@begindate and @enddate) {
    #do nothing, we're good
  } elsif (@begindate) {
    @enddate = Today();
  } elsif (@enddate) {
    print "Error, cannot specify only end date!\n";
    exit 1;
  } else {
    print "Error, dates are unspecified!\n";
    usage();
  }
}

if (!@begindate or !@enddate) {
  print "Error, dates not specifed or error parsing dates!\n";
  exit(1);
}

$begindate = sprintf ("%4d-%02d-%02d",@begindate);
$enddate = sprintf  ("%4d-%02d-%02d",@enddate);

if (!defined($hdbuser) || !defined($hdbpass)) {
  print "Error! No user or password!\n";
  usage();
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

This program relies upon the presence of the agency_cd and the datatype code
in the header, and upon the site_no, datetime and values to be in the
columns as in the example.

The USGS web site allows for multiple parameters to be given in the same file.
The generic mapping tables (hdb_ext_data_code and ref_ext_site_data_map)
contain the usgs ids and datatype codes.
The insert values subroutine uses the usgs data code to find the appropriate
column to get the values from. This is a problem for the
provisional website, which gives daily max, mean, and average flows, even when
they are not wanted.

Multiple usgs sites from the official url are not separated by headers (official site
now gone, so who cares???)

As of 6/21/2006, the official and provisional sites have merged.
Data qualification codes are added to output, eg:

agency_cd       site_no datetime        01_00060_00001  01_00060_00001_cd      01_00060_00002   01_00060_00002_cd       01_00060_00003  01_00060_00003_cd
5s      15s     16s     14s     14s     14s     14s     14s     14s
USGS    09380000        2006-06-20      17700   P       9430    P       14400  P

So will have to distinguish data sources, collection systems etc by this extra column,
also need to search for which column to look for data in by ignoring the _cd columns.

=cut

#HDB database interface
# global because used in several sub routines
my $hdb = Hdb->new;

my $dbname;
if (!defined($dbname = $ENV{HDB_LOCAL})) {
  $hdb->hdbdie("Environment variable HDB_LOCAL not set...\n");
}
#create connection to database
$hdb->connect_to_db($dbname, $hdbuser, $hdbpass);
$hdb->set_approle();

#Set date format to ISO, USGS data also in this format
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI'")
  or $hdb->hdbdie("Unable to set NLS_DATE_FORMAT to ISO standard\n");

get_app_ids();

my ($usgs_sites, $usgs_codes);

if (defined ($site_num_list)) {
  $usgs_sites = get_usgs_sites($site_num_list);
} else {
  $usgs_sites = get_usgs_sites();
  $site_num_list = build_site_num_list($usgs_sites);
}

#build data code list
# right now, we assume that we want any data codes defined
# for the sites specified.
$usgs_codes=get_usgs_codes($site_num_list);

my (@data);

#if we are just reading in data from a previously downloaded file
if (defined($readfile))
{
  open INFILE, "$readfile" or $hdb->hdbdie("Cannot open $readfile: $!\n");
  @data = <INFILE>;
  print @data if defined($debug);
  print Dumper(@data) if defined($debug);
  chomp @data;
} else { # attempt to get data from USGS web page

  my $url = build_url($site_num_list, $usgs_codes);
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
      $hdb->hdbdie("no data returned from USGS, exiting.\n");
    }
    print $inflated if defined($debug);
    @data = split /\n/, $inflated;
  } else { # ($response->is_error)
    printf STDERR $response->error_as_HTML;
    $hdb->hdbdie("Error reading from USGS web page, cannot continue\n");
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

  #find value column and get rid of actual header on data
  my $header = shift @data;
  my @headers = split /\t/, $header;
  if ($headers[0] ne "agency_cd" and
      $headers[0] ne "USGS") {
    print "@headers\n";
    $hdb->hdbdie("Data is not USGS website tab delimited format!\n");
  }
  #save header data for finding column, need usgs no first, see below

  # shift away extra header line (has field sizes and definitions, ignored)
  my $field_defs = shift @data;

  #check to see if no data at all
  if (substr ($data[0], 0, 1) eq '#') {
    next STATION;
  }

  # now we have data, mark data as found in USGS ID
  # assume that the format holds, and the id is in second column
  my (@firstrow) = split /\t/, $data[0];
  my $usgs_no = $firstrow[1];

  #check on found data for this usgs no
  $usgs_sites->{$usgs_no}->{found_data} = 1;
  if (!defined($usgs_sites->{$usgs_no}->{sdi}) or
      !defined($usgs_sites->{$usgs_no}->{site_id}) or
      !defined($usgs_sites->{$usgs_no}->{data_code})) {
    $hdb->hdbdie("site or sdi or data code not defined for usgs id: $usgs_no!\n");
  }

#find the column in the header that contains the right data code, making
# sure not to match column with data qualification code (ending regex with $)
  my $col = 1;
  until ($headers[$col++] =~ /$usgs_sites->{$usgs_no}->{data_code}$/) {
    if (!defined($headers[$col])) {
      print "Looking for: '$usgs_sites->{$usgs_no}->{data_code}' in:\n";
      print "@headers\n";
      print "Cannot find value column code for header of values column!\n";
      $hdb->hdbdie("Data is not USGS website tab delimited format!\n");
    }
  }
  #variable defining which column to read values from
  $usgs_sites->{$usgs_no}->{column} = $col-1;

  # count number of rows that are not comments after header
  #and are still the same usgs site
  while ($i <= $#data and
         substr ($data[$i], 0, 1) ne '#' and
         (split /\t/, $data[$i])[1] eq $usgs_no) {
    $i++;
  }

  if ($i eq 0) { # USGS has started putting completely empty headers in for stations not in realtime system?
    next STATION;
  }

  #make a slice of the input data for this site
  my (@cur_values)= @data[0..$i-1];

  # put data into database, handing site id and agg id for use
  # when aggDisagg is called, unless testflag is set
  # function returns date of first value and last value updated
  my ($first_date, $updated_date, $rows_processed);

  if (defined($insertflag)) {
  #tell user something, so they know it is working
    print "Working on USGS gage number: $usgs_no\n";
    #pass in possibly huge array of data for specific usgs id
    ($first_date, $updated_date, $rows_processed) =
             insert_values(\@cur_values,
                           $usgs_sites->{$usgs_no});
    if (!defined($first_date)) {
      print "No data updated for $usgs_no\n";
    } else {
      print "Data updated from $first_date to $updated_date for $usgs_no\n";
      print "Number of data values from USGS processed: $rows_processed\n";
    }
  }

  #now shorten the file array by the number of lines used up by this usgs id
  @data = @data[$i..$#data];

  #if there is data left, but no header at all, recycle old one
  #assume agency code is USGS
  if (@data and (split /\t/, $data[0])[0] eq 'USGS') {
    unshift @data, $field_defs;
    unshift @data, $header;
  }

  #reset row count
  $i=0;
}

# print error messages for all sites that no data was returned for
foreach my $usgs_no (keys %$usgs_sites) {
  if (!defined($usgs_sites->{$usgs_no}->{found_data})) {
    print STDERR "No $title{$flowtype} data found for site: $usgs_sites->{$usgs_no}->{site_name}   USGS ID: $usgs_no\n";
  }
}

#End main program, subroutines below

#global variables read from database
my ($load_app_id, $agen_id, $collect_id, $validation, $url);

sub get_app_ids
{
# Get application wide ids to describe where data came from
# only need to lookup loading application separately,
# the data source generic mapping table contain everything but method
# and computation.

# defined early in the program
  my $load_app_name = $progname;
  my $sth;

  my $get_load_app_statement = "select loading_application_id from
hdb_loading_application where loading_application_name = '$load_app_name'";

  eval {
    $sth = $hdb->dbh->prepare($get_load_app_statement);
    $sth->execute;
    $sth->bind_col(1,\$load_app_id);
    unless ($sth->fetch) {
      $hdb->hdbdie("Loading application id for $load_app_name not found!\n")
    };
    $sth->fetch;
    $sth->finish();
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie("Errors occurred during selection of loading application id for $load_app_name.\n");
  }

  my $get_other_info_statement = "select e.agen_id,
 e.collection_system_id collect_id, e.data_quality validation,
 e.description url
 from hdb_ext_data_source e
 where e.ext_data_source_name = '$title{$flowtype}'";

  eval {
    $sth = $hdb->dbh->prepare($get_other_info_statement);
    $sth->execute;
    $sth->bind_col(1,\$agen_id);
    $sth->bind_col(2,\$collect_id);
    $sth->bind_col(3,\$validation);
    $sth->bind_col(4,\$url);
    my $stuff = $sth->fetch;
    Dumper($stuff);
    unless ( $stuff ) {
      $hdb->hdbdie("Data source definition not found, $agen_id, $collect_id, $validation, $url!\n")
    };
    $sth->finish();
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie("Errors occurred during selection of data source info for $title{$flowtype}.\n");
  }

  if (!defined($validation)) {
    $validation = 'null';
  }
  else {
    $validation = "'" . $validation . "'";
  }
}

sub build_url
{

  #locations of USGS programs generating our data are contained in database
  # and looked up by get_app_ids subroutine above.
  #this url could be generated from http://waterdata.usgs.gov/nwis/uv
  # or dv, or the discharge url
  # but the url generated that way has lots of extra junk
  # parts of the url used here:
  # program generating the result  : stored in $url, see definition
  # specify flow                   : parameter_cd= (from database)
  # specify date format            : date_format=YYYY-MM-DD
  # specify tab delimited format   : format=rdb
  # specify gzip compression       : rdb_compression=gz
  # specify begin date             : begin_date=$begindate
  # specify end date               : end_date=$enddate
  # specify multiple site numbers  : multiple_site_no=

# can use numdays=<number> instead of begin and end date
# for official, it is always at least 300? days behind,
# not sure how to handle dates for official data.
# you can specify period also in hours by appending h to the end of numdays

  my $parameters = "?date_format=YYYY-MM-DD&format=rdb&rdb_compression=gz&begin_date=$begindate&end_date=$enddate";

  #apparently, the official site barfs if you use "parameter_cd" but
  # not having any parameter code at all doesn't make any difference
  if ($flowtype ne 'o') {
    $parameters .= "&parameter_cd=$_[1]";
  }

  $parameters .="&multiple_site_no=$_[0]";


# url already has http:// etc. part,
# argument is multiple site id list generated earlier
  $url .= $parameters;

  return $url;
}

sub build_site_num_list
{
  my $usgs_sites = $_[0];
  my ($usgs_num, $site_num_list);

  #build list of sites to grab data for
  foreach $usgs_num (keys %$usgs_sites) {
    $site_num_list .= $usgs_num . ',';
  }
  chop $site_num_list;
  return $site_num_list;
}

sub build_web_request
{
  my $ua = LWP::UserAgent->new;
  $ua->agent('USGS Streamflow -> US Bureau of Reclamation HDB dataloader: ' . $ua->agent);
  $ua->from('agilmore@uc.usbr.gov');
  $ua->timeout(600);
  my $request = HTTP::Request->new();
  $request->method('GET');
  return ($ua, $request);
}

sub get_usgs_sites
{
  my $site_num_list;
  my $id_limit_clause = '';

  if ($site_num_list = shift) {
    $site_num_list = "'" . $site_num_list;
    $site_num_list =~ s/,/','/g;
    $site_num_list .= "'";
    $id_limit_clause = "b.primary_site_code in ( $site_num_list ) and";
  }

#use title of data source, lowercase, with '_' instead of spaces,
# preceded by 'load_' to find the right key name for the datamapping table
  my $load_title = "load_";
  $load_title .= lc($title{$flowtype});
  $load_title =~ s/ /_/g;

# outcome should be 'load_usgs_realtime' or official or provisional
# the mapping list has already been loaded with just the site that
# are appropriate for this installation
# limited by any site number list passed in
  my $get_usgs_no_statement =
  "select b.primary_site_code usgs_id, b.primary_data_code data_code,
 b.hdb_interval_name interval, b.hdb_method_id meth_id,
 b.hdb_computation_id comp_id, b.hdb_site_datatype_id sdi,
 d.site_id, d.site_name
from hdb_site_datatype a, ref_ext_site_data_map b,
ref_ext_site_data_map_keyval c, hdb_site d
where a.site_datatype_id = b.hdb_site_datatype_id and
b.mapping_id = c.mapping_id and $id_limit_clause 
c.key_name = '$load_title' and 
c.key_value ='Y' and
a.site_id = d.site_id and
d.db_site_code = '$db_site'
order by usgs_id";

  $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

  my $hashref;

#this DBI function returns a hash indexed by column 1 (one-based), which is
# the usgs id (the first column returned by the query above)
# if no data returned, checked later
  eval {
    $hashref = $hdb->dbh->selectall_hashref($get_usgs_no_statement,1);
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie("Errors occurred during automatic selection of USGS gage ids.\n");
  }

  return $hashref;
}

sub get_usgs_codes
{
  my ($code_num_list, $value, $sth);
  my $id_limit_clause = '';
  my $site_num_list;

  if ($site_num_list = shift) {
    $site_num_list = "'" . $site_num_list;
    $site_num_list =~ s/,/','/g;
    $site_num_list .= "'";
    $id_limit_clause = "b.primary_site_code in ( $site_num_list ) and";
  } else {
    $hdb->hdbdie("No sites specified for usgs codes!\n");
  }


# outcome should be 'load_usgs_realtime' or official or provisional
# the mapping list has already been loaded with just the site that
# are appropriate for this installation
# we could limit this also by the data source name, but that is already 
# done with the keyname
# limited by any site number list passed in
  my $get_data_code_statement =
  "select distinct b.primary_data_code data_id
from hdb_ext_data_source a, ref_ext_site_data_map b 
where $id_limit_clause
a.ext_data_source_id = b.ext_data_source_id and
a.ext_data_source_name = '$title{$flowtype}'";

  my $data_code_list;

  eval {
    $sth = $hdb->dbh->prepare($get_data_code_statement);
    $sth->execute;
    $sth->bind_col(1,\$value);
    unless ($sth->fetch) {
      $hdb->hdbdie("No data codes found!\n")
    };
    $data_code_list = $value;
    # now build list if there is more than one data code returned
    # if more than one data code per site, will fail
    while ($sth->fetch) {
      $data_code_list .= ',' . $value;
    }
    $sth->finish();
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie("Errors occurred during automatic selection of $title{$flowtype} data codes.\n");
  }

  return $data_code_list;
}

sub check_value
{
  my $check_date=$_[0];
  my $site_datatype_id=$_[1];
  my $interval=$_[2];
  my ($sth, $value);

  my $check_data_statement = "select value from r_base
where start_date_time = '$check_date' and site_datatype_id = $site_datatype_id and interval = '$interval'";

  eval {
    $sth = $hdb->dbh->prepare($check_data_statement);

    $sth->execute;
    $sth->bind_col(1,\$value);
    $sth->fetch;
    $sth->finish();
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie("Errors occurred during selection of data already in database.\n");
  }
  return ($value);
}

sub insert_values
{
  my @data = @{$_[0]};
  my $usgs_site = $_[1];

  my $i = 0;
  my ($first_date, $end_date_time) = undef;
  my ($value, $value_date, $updated_date);
  my ($line, @row);
  my $old_val;

# SQL statements

  my $modify_data_statement = "
  BEGIN
    modify_r_base_raw($usgs_site->{sdi},'$usgs_site->{interval}',
                      ?,/* start_date_time */
                      ?,/* end_date_time (in/out, not used) */
                      ?,/* value */
                      $agen_id,$overwrite,$validation,
                      $collect_id,$load_app_id,
                      $usgs_site->{meth_id},$usgs_site->{comp_id},
                      'Y');/*do update? */
  END;";

  eval
  {
    my ($modsth);
    $modsth = $hdb->dbh->prepare($modify_data_statement);

    # loop through array of data read for single site
    # insert or update or do nothing for each datapoint (row in file);
    foreach $line (@data)
    {

      $i++;
      @row = split /\t/, $line;
      $value_date = $row[2];
      $value = $row[$usgs_site->{column}];
      if ($value) { # get rid of ',' in display
	$value =~ s/,//g;
      }

# check if value from source is known
      if (!defined ($value) or $value eq '') {
	print "data missing: $usgs_site->{sdi}, date $value_date\n" if defined($debug);
	next;
      } elsif ($value =~ m/Ice/) { # check for Ice
	print "Iced up: $usgs_site->{sdi}, date $value_date: $value\n"
	     if defined($debug);
	next;
      } elsif ($value =~ m/[^0-9\.]/) { # check for other text, complain
	print "data corrupted: $usgs_site->{sdi}, date $value_date: $value\n";
	next;
      } else {
	# update or insert
        if (defined($debug)) {
          if (!defined($old_val)) {
            print "modifying for $usgs_site->{sdi}, date $value_date, value $value, old_val = undef\n";
          } else {
            print "modifying for $usgs_site->{sdi}, date $value_date, value $value, old_val = $old_val\n";
          }
        }
	$modsth->bind_param(1,$value_date);
	$modsth->bind_param_inout(2,\$end_date_time,50);
	$modsth->bind_param(3,$value);
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
    $hdb->hdbdie("Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.\n");
  } elsif ($first_date) {	# commit the data
    $hdb->dbh->commit or $hdb->hdbdie($hdb->dbh->errstr);
  }
  return $first_date, $updated_date, $i;
}

sub usage
{
  print STDERR <<"ENDHELP";
$progname [ -h | -v ] | [ options ]
Retrieves USGS flow data and inserts into HDB
Example: $progname -u app_user -p <hdbpassword> -n 2 -i 09260000 -a -r i

  -h               : This help
  -v               : Version
  -u <hdbusername> : HDB application user name (REQUIRED)
  -p <hdbpassword> : HDB password (REQUIRED)
  -i <usgs_id>     : USGS gage id to look for. Must be in HDB. More than one
                     may be specified with -i id1,id2 or -i id1 -i id2
                     IF NONE SPECIFIED, WILL LOAD ALL GAGES SET UP IN HDB.
  -t               : Test retrieval, but do not insert data into DB
  -f <filename>    : Filename instead of live web
  -n <numdays>     : Days for retrieval (max 31 for realtime)
  -b <DD-MON-YYYY>  : Begin date for data retrieval
  -e <DD-MON-YYYY>  : End date for data retrieval
  -o               : Use overwrite flag, otherwise null
  -w               : Web address (URL developed to get USGS data)
  -d               : Debugging output
  -l <r,o,p>       : Specify flow type realtime(default), provis., or official
  -s <UC,LC,...>   : Specify db_site_code (from hdb_site) for sites to load (REQUIRED)
ENDHELP

  exit (1);
}

sub version
{
  print "$progname version $verstring\n";
  exit 1;
}
