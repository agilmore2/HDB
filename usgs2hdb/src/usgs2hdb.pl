#!/usr/bin/perl

use strict;
use warnings;

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{HDB_ENV}/perlLib/lib";

#this next is for solaris only, but won't hurt Linux
use lib "$ENV{HDB_ENV}/perlLib/lib/sun4-solaris";

use LWP::UserAgent;
use Date::Calc qw(Delta_DHMS Add_Delta_Days Month_to_Text Decode_Date_EU Today);
use Compress::Zlib;
use File::Basename;
use Data::Dumper;

#insert HDB library
use Hdb;

#Version Information
my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my $insertflag = 1;
my $overwrite = 'null';

my ($readfile, $accountfile, $runagg, $printurl, $debug, $flowtype);
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
    if (! -r $readfile) {
      print "file not found or unreadable: $readfile\n";
      exit 1;
    }
  } elsif ($arg =~ /-a/) {	# get database login file
    $accountfile = shift(@ARGV);
    if (! -r $accountfile) {
      print "file not found or unreadable: $accountfile\n";
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
  } elsif ($arg =~ /-s/) {	# get database site to load, IGNORED
    shift(@ARGV);
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

#handle flowtype, unit or daily
#realtime, provisional, or official used to be these,
#but the USGS merged the sources for Official and Provisional
#check if flowtype is defined, if not, we assume realtime
if (!defined($flowtype)) {
  $flowtype = 'u';
}

if (! ($flowtype =~ /^[ud]$/)) {
  print "Error, invalid flow type defined: '$flowtype'\n";
  print "Specify one of u or d (unit (instantaneous) or daily)\n";
  usage();
}

# If we are running from a terminal, check if user really wants to load all
# this data! If it is not a terminal, we can assume the user knows what they
# are doing!
if (-t STDIN) {
  if (($flowtype eq "d") and !defined($site_num_list)) {
    print "Lookout! You have specified loading all daily sites in HDB!\n";
    print "Are you sure you want to do this? (y/n) \n";
    my $yn;
    read STDIN, $yn,2;
    chomp $yn;
    if ($yn eq 'n') {
      exit 0;
    } elsif ( $yn eq 'y') {
      ;                         # do nothing
    } else {
      print "I'm sorry, I can't do that, Dave.\n";
      exit 1;
    }
  }
}


#flowtype defines the title of the datasource, and we retrieve all other
# data corresponding to that source from the database

my %title;
$title{u}='USGS Unit Values (Realtime)';
$title{d}='USGS Daily Values (Provisional/Official)';

#Truth table and results for numdays, begindate and enddate
# errors are all defined, only enddate defined.
# everything else gets fixed up.
# Num Days | Beg Date | End Date | Result
# nothing defined                   error
#  def                              end date = now, beg date = enddate-numdays
#  def        def                   end date = beg date + numdays
#             def                   end date = now, check max num days?
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

  if ($numdays >31 and $flowtype eq 'u') {
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

# we may want to rethink the above to handle new capabilities of
# NWIS to return only a few hours of data, and only data that has changed in 
# the last x minutes.


if ((!defined($hdbuser) || !defined($hdbpass)) && !defined($accountfile)) {
  print "Error! No database login information! No user or password!\n";
  usage();
}

=head2 FORMAT

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

 agency_cd       site_no datetime        01_00060_00003  01_00060_00003_cd
 5s      15s     16s     14s     14s
 USGS    09380000        2006-06-20      14400  P

So will have to distinguish data sources, collection systems etc by this extra
column, also need to search for which column to look for data in by ignoring
the _cd columns.

=head2 Design Notes

Current limitations include the lack of ability to collect more than one
parameter per site. They might even be in the file received from USGS, but
this application will only read the first. This would be fixable with
some work to insert_values() and the header parsing code.

Getting a different parameter at a different site is currently supported, but
unused.

This program really, really needs some modularization.

=cut

#HDB database interface
# global because used in several sub routines
my $hdb = Hdb->new;

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

#set write ability on connection (app_role)
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
  if (($data[0] eq '') or (substr ($data[0], 0, 1) eq '#')) {
    shift @data;
    next STATION;
  }

  # now we have data, mark data as found in USGS ID
  # assume that the format holds, and the id is in second column
  my (@firstrow) = split /\t/, $data[0];
  my $usgs_no = $firstrow[1];

  # check that usgs_no is sane, i.e. contains only digits:
  if ($usgs_no =~ /\D/) {
    print STDERR "Warning! '$usgs_no' does not appear to be a valid USGS site number!\n";
    $hdb->hdbdie("Assuming data retrieved is garbage and giving up!\n");
  }

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
      print STDERR "Not found: '$usgs_sites->{$usgs_no}->{data_code}' in:\n";
      print STDERR "@headers\n";
      print STDERR "Cannot find value column code from header!\n";
      $hdb->hdbdie("Data is not USGS website tab delimited format!\n");
    }
  }
  #variable defining which column to read values from
  $usgs_sites->{$usgs_no}->{column} = $col-1;

  #look for data quality column
  # column header should be something like 01_00060_00003_cd
  # and should be the column immediately after the value
  # does the realtime data have this?
  unless ($headers[$col] =~ /$usgs_sites->{$usgs_no}->{data_code}_cd$/) {
    print STDERR "Not found: '$usgs_sites->{$usgs_no}->{data_code}_cd' in:\n";
    print STDERR "@headers\n";
    print STDERR "Cannot find quality column code from header!\n";
    $hdb->hdbdie("Data is not USGS website tab delimited format!\n");
  }
  $usgs_sites->{$usgs_no}->{qualitycolumn} = $col;

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

  # put data into database, unless testflag is set
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
      print "No data processed for $usgs_sites->{$usgs_no}->{site_name}, $usgs_no\n";
    } else {
      print "Data processed from $first_date to $updated_date for $usgs_sites->{$usgs_no}->{site_name}, $usgs_no\n";
      print "Number of rows from USGS processed: $rows_processed\n";
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
my ($load_app_id, $agen_id, $validation, $url);

my ($collect_id, $official_collect_id);

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
    }
    $sth->finish();
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie("Errors occurred during selection of data source info for $title{$flowtype}.\n");
  }

  if (!defined($validation)) {
 #needs to be empty string for dynamic sql to recognize as null, not 'null'!
    $validation = '';
  }

  if ($flowtype eq "d") {
    my $get_official_collect_statement = 
    "select collection_system_id
     from hdb_collection_system
     where collection_system_name = 'USGS Official'";

    eval {
      $sth = $hdb->dbh->prepare($get_official_collect_statement);
      $sth->execute;
      $sth->bind_col(1,\$official_collect_id);
      my $stuff = $sth->fetch;
      Dumper($stuff);
      unless ( $stuff ) {
        warn("Data source definition for USGS Official not found!\n");
        print("Official data will not be labeled with USGS Official collection system\.n");
        $official_collect_id= $collect_id;
      }
      $sth->finish();
    };

    if ($@) {                   # something screwed up
      print $hdb->dbh->errstr, " $@\n";
      $hdb->hdbdie("Errors occurred during selection of collection id info for USGS Official.\n");
    }
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

  #append data code list, complete url syntax done in get_usgs_codes
  $parameters .= "$_[1]";

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
hdb_site d, hdb_ext_data_source e
where a.site_datatype_id = b.hdb_site_datatype_id and $id_limit_clause
b.is_active_y_n = 'Y' and
a.site_id = d.site_id and
b.ext_data_source_id = e.ext_data_source_id and
e.ext_data_source_name = '$title{$flowtype}'
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
    #create list for use with retrieval. 
    # the parameter list should be like "&cb_00060=on&cb_00065=on" etc
    # one cb_ setting for each data code. The value codes in the resulting
    # output and in the generic mapping are 00060_00003, so
    # we have to subtract all that extra at the end for this use.
    # realtime codes are 00060 in the db, so this will not affect them
    $value =~ s/_.*//;
    $data_code_list = '&cb_' . $value. '=on';
    # now build list if there is more than one data code returned
    # if more than one data code per site, will fail
    while ($sth->fetch) {
      $value =~ s/_.*//;
      $data_code_list .= '&cb_' . $value . '=on';
    }
    $sth->finish();
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie("Errors occurred during automatic selection of $title{$flowtype} data codes.\n");
  }

  return $data_code_list;
}

sub insert_values
{
  my @data = @{$_[0]};
  my $usgs_site = $_[1];

  my $i = 0;
  my $first_date = undef;
  my ($value, $value_date, $updated_date, $qual_code);
  my ($line, @row);
  my $valid_code = $validation;
  my $coll_id = $collect_id;

# SQL statements

# this statement takes arguments of
# start_date_time, end_date_time, value, validation, and collection system id
# the rest of the arguments are predetermined by command line arguments and
# the generic mapping for this site

  if (!defined($usgs_site->{interval}) or
      !defined($agen_id) or
      !defined($overwrite) or
      !defined($load_app_id) or
      !defined($usgs_site->{meth_id}) or
      !defined($usgs_site->{comp_id})) {
    $hdb->hdbdie("Unable to write data to database for $usgs_site->{site_name}\nRequired information missing in insert_values()!\n");
  }

  my $modify_data_statement = "
  BEGIN
    modify_r_base($usgs_site->{sdi},'$usgs_site->{interval}',
                      ?,null,/* start_date_time, end_date_time */
                      ?,/* value */
                      $agen_id,$overwrite,
                      ?, ?, /* validation, collection system id */
                      $load_app_id,
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

      #value date is already in required format
      $value_date = $row[2];

      $value = $row[$usgs_site->{column}];

      if ($value) { # get rid of ',' in display
	$value =~ s/,//g;
      }

# For daily values:
# validation is defaulted to the value of the $validation variable
# but may be overwritten by the qual_code from the row of data
# Currently, the codes seen from the USGS are: none, P and A
# 'A' is approved data, which is the "Official" data. Rows with 
# this quality code are given a different collection system, and this
# quality code is passed on as the validation.

      if ($flowtype eq 'd') {
        $qual_code = $row[$usgs_site->{qualitycolumn}];
        if (defined($qual_code) and $qual_code eq 'A') {
          $valid_code = $qual_code;
          $coll_id = $official_collect_id; #from USGS official in collect table
        } else {
          $valid_code = $validation; #from data source table
          $coll_id = $collect_id; #from data source table
        }
      }

      #need all three of these here. Also checking on value next
      if (!defined($value_date) or
          !defined($valid_code) or
          !defined($coll_id)) {
        $hdb->hdbdie("Undefined date, valid code or collection id in insert_values()!\n");
      }
# check if value is known, if not, move to next row
      if (!defined ($value) or $value eq '') {
	print "data missing: $usgs_site->{sdi}, date $value_date\n" if defined($debug);
	next;
      } elsif ($value =~ m/Ice/) { # check for Ice
	print "Iced up: $usgs_site->{sdi}, date $value_date: $value\n"
	     if defined($debug);
	next;
      } elsif ($value =~ m/[^0-9\.]/) { # check for other text, complain
	print "value field not recognized: $usgs_site->{sdi}, date $value_date: $value\n";
	next;
      } else {
	# update or insert
        if (defined($debug)) {
          print "modifying for $usgs_site->{sdi}, date $value_date, value $value, update status unknown\n";
        }
	$modsth->bind_param(1,$value_date);
	$modsth->bind_param(2,$value);
	$modsth->bind_param(3,$valid_code);
	$modsth->bind_param(4,$coll_id);
	$modsth->execute;
	
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
$progname $verstring [ -h | -v ] | [ options ]
Retrieves USGS flow data and inserts into HDB
Example: $progname -a <accountfile> -n 2 -i 09260000 -l u

  -h               : This help
  -v               : Version
  -a <accountfile> : HDB account access file (REQUIRED or both below)
  -u <hdbusername> : HDB application user name (account file or REQUIRED)
  -p <hdbpassword> : HDB password (account file or REQUIRED)
  -i <usgs_id>     : USGS gage id to look for. Must be in HDB. More than one
                     may be specified with -i id1,id2 or -i id1 -i id2
                     IF NONE SPECIFIED, WILL LOAD ALL GAGES ENABLED IN HDB.
  -t               : Test retrieval, but do not insert data into DB
  -f <filename>    : Filename instead of live web
  -n <numdays>     : Days for retrieval (max 31 for realtime)
  -b <DD-MON-YYYY> : Begin date for data retrieval
  -e <DD-MON-YYYY> : End date for data retrieval
  -o               : Use overwrite flag, otherwise null
  -w               : Web address (URL developed to get USGS data)
  -d               : Debugging output
  -l <u,d>         : Specify flow type: unitvalues (realtime default),or daily
ENDHELP

  exit (1);
}

sub version
{
  print "$progname version $verstring\n";
  exit 1;
}
