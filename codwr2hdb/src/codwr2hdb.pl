#!/usr/bin/perl -w

#insert HDB library

use lib "$ENV{HDB_ENV}/perlLib/lib";
use lib "$ENV{HDB_ENV}/perlLib/lib/sun4-solaris";

use LWP::UserAgent;
use Date::Calc qw(Delta_DHMS);
use File::Basename;
use Data::Dumper;
use HTML::TableExtract;
use HTML::TokeParser;

use Hdb;

use strict;

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my $insertflag = 1;

my ($readfile, $runagg, $printurl, $debug);
my ($hdbuser, $hdbpass, @site_num_list);

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
  } elsif ($arg =~ /-i/) {	# get codwr id
    push @site_num_list, shift(@ARGV);
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

# if user specified codwr gage ids, chop off last comma
if (@site_num_list) {
  if (grep (/[^A-Z]/, @site_num_list)) {
    die "ERROR: @site_num_list\ndoes not look like CODWR id.\n";
  }
}

if (!defined($hdbuser) || !defined($hdbpass)) {
  print "ERROR: Arguments -u and -p (HDB username and password) required\n\n";
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
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/YYYY HH:MI:SS AM'")
  or die "Unable to set NLS_DATE_FORMAT to US \n";

my ($codwr_sites);

if (@site_num_list) {
  $codwr_sites = get_codwr_sites(@site_num_list);
} else {
  $codwr_sites = get_codwr_sites();
  (@site_num_list) = build_site_num_list($codwr_sites);
}

=FORMAT

The data is expected in HTML format with the following columns:
Date/Time	DISCHRG 	GAGE_HT
5/6/2003 12:15:00 PM	17.3 	0.54
etc...

Problems:
Name of the flow column changes from
DISCHRG to DISCHRG1 to DISCHRG2 to DISCHRG3
Only one site at a time can be retrieved
GOES randomly timed data is on a different page than
normal self-timed data
data is in REVERSE order!

=cut

my (@data);

#note the DISCHRG codee, some sites use numbers on the end, but this matches ok
my $datate = new HTML::TableExtract(headers =>
				    ["Date/Time", "DISCHRG"]);
my $i;

if (defined($readfile))
{
  $i = 0;

  $datate->parse_file($readfile) or die "HTML parse of $readfile failed: $!\n";
  foreach my $row ($datate->rows) {
    $data[$i][0] = $$row[0]; #note headers above, so this is date/time
    $data[$i++][1] = $$row[1]; # and this is discharge
  }
# we get the site_id from the html, looking for the following data:
#<p align='left'><big>Current Conditions For UCANALCO -- U LATERAL CANAL BELOW GREAT CUT DIKE NEAR DOLORES</big></p>

#first Link in the document is the sitename!
  my $p = HTML::TokeParser->new($readfile);
  my $token = $p->get_tag("p");
  my $text = $p->get_trimmed_text("/p");

  my ($codwr_no) = $text =~ /^Current Conditions For ([A-Z]{8})/
     or die "File does not appear to be a CO DWR DOMSAT data file\n";

  print Dumper(@data) if defined($debug);
  process_data(\@data, $codwr_no);
} else { # attempt to get data from CODWR web page

  foreach my $codwr_no (keys %$codwr_sites) {

    my $url = build_url($codwr_no);
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
      print "Data read from CODWR: $status\n";

      if (!$response->content) {
	die "no data returned from CODWR, exiting.\n";
      }
    } else {			# ($response->is_error)
      printf STDERR $response->error_as_HTML;
      die "Error reading from CODWR web page, cannot continue\n";
    }

    $i = 0;

    $datate->parse($response->content)
       or die "HTML parse of response failed: $!\n"; ;

    foreach my $row ($datate->rows) {
      $data[$i][0] = $$row[0]; #note headers above, so this is date/time
      $data[$i++][1] = $$row[1]; # and this is discharge
    }
    print Dumper(@data) if defined($debug);
    process_data(\@data, $codwr_no);
  }
}

# print error messages for all sites that no data was returned for
foreach my $codwr_no (keys %$codwr_sites) {
  if (!defined($codwr_sites->{$codwr_no}->{found_data})) {
    print STDERR "No realtime data found for site CODWR ID: $codwr_no site_id: $codwr_sites->{$codwr_no}->{site_id}\n";
  }
}

exit 0;

#End main program, subroutines below

sub process_data
{
  my $data = $_[0];
  my $codwr_no = $_[1];

  $codwr_sites->{$codwr_no}->{found_data} = 1;
  my $cur_sdi = $codwr_sites->{$codwr_no}->{site_datatype_id};
  my $cur_site = $codwr_sites->{$codwr_no}->{site_id};

  # put data into database, handing site id and agg id for use
  # when aggDisagg is called, unless testflag is set
  # function returns date of first value and last value updated
  my ($first_date, $updated_date);

  if (defined($insertflag)) {
    #tell user something, so they know it is working
    print "Working on CODWR gage number: $codwr_no\n";
    ($first_date, $updated_date) = insert_values($data, $cur_sdi);
    if (!defined($first_date)) {
      print "No data updated for $codwr_no\n";
    }
  }

  # now call aggDisagg (an HDB application) to move data up to
  # r_hour table, uses dates returned from insert from previous function

  if (defined($runagg) and (defined($first_date) or !defined($insertflag))) {
    if (!defined($insertflag)) { # no insert, need dates
      $first_date = $data->[scalar(@$data)-1][0];
      $updated_date = $data->[0][0];
    }

    # find number of hours and properly formatted date and time
    my ($num_hours, $agg_date, $aggtime) = agg_hours($first_date,
						     $updated_date);

    my ($houragg_id, $dayagg_id) = find_agg_id()
    or die "No aggregation id for moving flows from r_instant or r_hour exist, one must be created.\n";

    my $cmd = qq{aggDisagg $hdbuser $hdbpass $houragg_id 1 n n n r d $num_hours '$agg_date $aggtime' $cur_site >logs/aggDisagg_codwrhour$codwr_no.out 2>logs/aggDisagg_codwrhour$codwr_no.err};
    print "$cmd\n";
    system $cmd;

    my $num_days =  sprintf("%d", $num_hours/24);
    $num_days++;

    $cmd = qq{aggDisagg $hdbuser $hdbpass $dayagg_id 1 n n n r d $num_days '$agg_date' $cur_site >logs/aggDisagg_codwrday$codwr_no.out 2>logs/aggDisagg_codwrday$codwr_no.err};
    print "$cmd\n";
    system $cmd;
  }

}

sub build_url
{
#this url is generated from http://www.dwr.state.co.us/Hydrology/flow_search.asp
# parts of the url:
# program generating the result  : http://www.dwr.state.co.us/Hydrology/flow_data.asp
# specify site                            : ID=$_[0]
# specify discharge(these codes change!)  : MTYPE=$_[1]
# no way to specify how much data is returned

# this hack is to handle the different datacodes used for flow at the
# sites of interest. This is lame, but ...
  my %datacode = ("DOLBMCCO", "DISCHRG",
		  "DOLTUNCO", "DISCHRG",
		  "DOVCANCO", "DISCHRG1",
		  "MVIDIVCO", "DISCHRG3",
		  "UCANALCO", "DISCHRG2");

  die "Site id $_[0] not recognized, no datacode known!\n" 
      unless (defined ($datacode{$_[0]}));

  my $url = "http://www.dwr.state.co.us/Hydrology/flow_data.asp?";
  $url .= "ID=$_[0]&MTYPE=$datacode{$_[0]}";

  return $url;
}

sub build_site_num_list
{
  my $codwr_sites = $_[0];
  my ($codwr_num, @site_num_list);

  #build list of sites to grab data for
  foreach $codwr_num (keys %$codwr_sites) {
    push @site_num_list, $codwr_num;
  }

  return @site_num_list;
}

sub build_web_request
{
  my $ua = LWP::UserAgent->new;
  $ua->agent('CO DWR DOMSAT Streamflow -> US Bureau of Reclamation HDB dataloader: ' . $ua->agent);
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

# we appropriate the SCS id field for the CO DWR site ids.
sub get_codwr_sites
{
  my $id_limit_clause = '';

  if (@_) {
    my $commalist = join (',',@_);
    $id_limit_clause = "a.scs_id in ( $commalist ) and";
  }

  my $get_codwr_no_statement = "select a.scs_id, a.site_id, b.site_datatype_id
from hdb_site a, hdb_site_datatype b
where a.site_id = b.site_id and
b.datatype_id = 18 and
a.scs_id is not null and $id_limit_clause
a.scs_id like '________'
order by scs_id";

  $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

  my $hashref;

  print $get_codwr_no_statement, @_,"\n" if defined($debug);

  eval {
    $hashref = $hdb->dbh->selectall_hashref($get_codwr_no_statement,1);
  };

  if ($@) { # something screwed up
    print $hdb->dbh->errstr, ": $@\n";
    die "Errors occurred during automatic selection of CODWR gage ids.\n";
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
  my($codwr_max_date)=$_[1];
  my($sth, $max_date);

  my $check_date_statement = "select max_hourly_date
from ref_hm_site_datatype
where site_datatype_id = ?
and max_hourly_date >= ? ";

  eval {
    $sth = $hdb->dbh->prepare($check_date_statement);
    $sth->bind_param(1,$cur_sdi);
    $sth->bind_param(2,$codwr_max_date);

    $sth->execute;
    $sth->bind_col(1,\$max_date);

    if (!$sth->fetch) { #no value returned, CODWR data newer
      $sth->finish();
      $max_date = $codwr_max_date;
      $hdb->dbh->do("update ref_hm_site_datatype
set (max_hourly_date) = ('$max_date') where (site_datatype_id = $cur_sdi) ");

      $hdb->dbh->commit;

      print "Updated ref_hm_site_datatype: max_hourly_date $max_date, site_datatype_id $cur_sdi" if defined ($debug);
    } else { # hydromet data newer

      $sth->finish();
      print "No new data inserted, codwr_date $codwr_max_date, max_hourly_date $max_date, site_datatype_id $cur_sdi" if defined ($debug);
    }
  };

  if ($@) { # something screwed up
    print "$hdb->dbh->errstr, $@\n";
    die "Errors occurred during update of ref_hm_site_datatype.\n";
  }

  return $max_date;
}


# find number of hours to run aggDisagg given two timestamps
# format of timestamps is 'MM/DD/YYYY HH:MI:SS AM'
# also return date and times formatted for the begin timestamp for aggDisagg
sub agg_hours
{
  my $start_ts = $_[0];
  my $end_ts = $_[1];

  my ($startdate,$starttime,$startamorpm) = split / /, $start_ts;
  my ($startmonth,$startday,$startyear) = split /\//, $startdate;
  if (!defined ($starttime)) {
    $starttime = '12:00:00';
    $startamorpm = 'AM';
  }
  my ($starthour,$startminute,$startsec) = split /:/, $starttime;
  $starthour += 12 if ($startamorpm eq 'PM' and $starthour < 12);
  $starthour -= 12 if ($startamorpm eq 'AM' and $starthour == 12);

  my ($enddate,$endtime,$endamorpm) = split / /, $end_ts;
  my ($endmonth,$endday,$endyear) = split /\//, $enddate;
  if (!defined ($endtime)) {
    $endtime = '12:00:00';
    $endamorpm = 'AM';
  }
  my ($endhour,$endminute,$endsec) = split /:/, $endtime;
  $endhour += 12 if ($endamorpm eq 'PM' and $endhour < 12);
  $endhour -= 12 if ($endamorpm eq 'AM' and $endhour == 12);

  my ($Dd,$Dh,$Dm,$Ds) =
  Delta_DHMS($startyear,$startmonth,$startday,
	     $starthour,$startminute,$startsec,
	     $endyear,$endmonth,$endday,
	     $endhour,$endminute,$endsec);

  #return number of hours, and properly formatted date and time
  return $Dd*24 + $Dh + 1, "$startmonth/$startday/$startyear", $starttime;
}

sub insert_values
{
  my @data = @{$_[0]};
  my $cur_sdi = $_[1];

  #fixed the url encoding to only retrieve one
  # parameter, so no longer need this, but could need it again
  my $column = 1;

  my $i = 0;
  my $first_date = undef;
  my ($value, $value_date, $updated_date);
  my ($line);
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
    foreach $line (@data) {
      #fix the non-reporting of time for their website for midnight,
      # ie, the date times are all in the form MM/DD/YYYY HH:MI:SS AM
      # except for midnight, which is reported as MM/DD/YYYY
      if ($line->[0] =~ /^[0-9]{1,2}\/[0-9]{1,2}\/[0-9]{4}$/) {
	$line->[0] .= " 12:00:00 AM";
      }

      $value_date = $line->[0];
      $value = $line->[$column];

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
	if (!defined($updated_date)) { # mark that data has been changed
	  $updated_date = $value_date;
	}
	$first_date = $value_date;
      } elsif ($old_val == $value and $old_source == 11) {
	next;			# source and database are same, do nothing
      } elsif ($old_val != $value or $old_source != 11 ) {
	# update, source and database differ (or source id is different!)
	print "updating for $cur_sdi, date $value_date, value $value, old_val = $old_val old_source = $old_source\n"  if defined($debug);
	#update instead of insert
	$upsth->bind_param(1,$value);
	$upsth->bind_param(2,$cur_sdi);
	$upsth->bind_param(3,$value_date);
	$upsth->execute;
	if (!defined($updated_date)) { # mark that data has been changed
	  $updated_date = $value_date;
	}
	$first_date = $value_date;
      }
    }
    $upsth->finish;
    $insth->finish;
  };				# semicolon here because of use of eval

  if ($@) { # something screwed up in insert/update
    print "$hdb->dbh->errstr, $@\n";
    die "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.\n";
  } elsif ($updated_date) {	# commit the data
    if (defined($debug)) {
      $hdb->dbh->rollback or die $hdb->dbh->errstr;
    } else {
      $hdb->dbh->commit or die $hdb->dbh->errstr;
      # update hm driver table, and check for newer hydromet data
      # $updated_date = get_update_max_date($cur_sdi, $updated_date);
    }
  }
  return $first_date, $updated_date;
}

sub usage
{
  print STDERR <<"ENDHELP";
$progname [ -h | -v ] | [ options ]
Retrieves CODWR flow data and inserts into HDB, runs aggDisagg to r_hour.
Example: $progname -u app_user -p <hdbpassword> -n 2 -i 09260000 -a

  -h               : This help
  -v               : Version
  -u <hdbusername> : HDB application user name (REQUIRED)
  -p <hdbpassword> : HDB password (REQUIRED)
  -i <codwr_id>    : CODWR gage id to look for. Must be in HDB. More than one
                     may be specified with -i id1,id2 or -i id1 -i id2
                     If none specified, will load all gages set up in HDB.
  -t               : Test retrieval, but do not insert data into DB
  -f <filename>    : Filename instead of live web
  -n <1..31>       : Days ending now for retrieval (default 1, max 31)
  -a               : Run aggDisagg after loading instantaneous data
  -w               : Web address (URL developed to get CODWR data)
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
