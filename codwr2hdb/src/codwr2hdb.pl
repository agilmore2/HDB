#!/usr/bin/perl -w

#insert HDB library

use lib "$ENV{HDB_ENV}/perlLib/lib";
# don't need this?
#use lib "$ENV{HDB_ENV}/perlLib/lib/sun4-solaris";

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

my ($readfile, $runagg, $printurl, $debug, $accountfile);
my ($hdbuser, $hdbpass, @site_num_list);


parseargs(@ARGV);


#======================================================================
#parse arguments


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

#Set date format to the format used by their website
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/YYYY HH:MI:SS AM'")
  or die "Unable to set NLS_DATE_FORMAT to US \n";

my $datasource = "Colorado Division of Water Resources (CODWR)";

get_app_ids();

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

my $i = 0;

if (defined($readfile))
{
  $i = 0;

#note the DISCHRG code, some sites use numbers on the end, but this matches ok
  my $datate = new HTML::TableExtract(headers =>
				    ["Date/Time", "DISCHRG"]);

  $datate->parse_file($readfile) or die "HTML parse of $readfile failed: $!\n";
  foreach my $row ($datate->rows) {
    $data[$i][0] = $$row[0]; #note headers above, so this is date/time
    $data[$i++][1] = $$row[1]; # and this is discharge
  }
# we get the site_id from the html, looking for the following data:
#<p align='left'><big>Current Conditions For UCANALCO -- U LATERAL CANAL BELOW GREAT CUT DIKE NEAR DOLORES</big></p>

#first Link in the document is the sitename! Not any more, now it is the third
# table cell. Grr.
  my $p = HTML::TokeParser->new($readfile);
  my $token = $p->get_tag("td");
     $token = $p->get_tag("td");
     $token = $p->get_tag("td");
  my $text = $p->get_trimmed_text("/td");

  my ($codwr_no) = $text =~ /^([A-Z]{8}) --/
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
    my $datate = new HTML::TableExtract(headers =>
				    ["Date/Time",
                                     $codwr_sites->{$codwr_no}->{data_code}]);

    $datate->parse($response->content)
       or die "HTML parse of response failed: $!\n"; ;

    foreach my $row ($datate->rows) {
      $data[$i][0] = $$row[0]; #note headers above, so this is date/time
      $data[$i++][1] = $$row[1]; # and this is discharge
    }

    print Dumper(@data) if defined($debug);
    process_data(\@data, $codwr_no);
  } # end of foreach site loop
}

# print error messages for all sites that no data was returned for
foreach my $codwr_no (keys %$codwr_sites) {
  if (!defined($codwr_sites->{$codwr_no}->{found_data})) {
    print STDERR "No realtime data found for site CODWR ID: $codwr_no site_id: $codwr_sites->{$codwr_no}->{site_id}\n";
  }
}

exit 0;

#End main program, subroutines below

sub parseargs {
  while (@_) {
    my $arg = shift;
    if ($arg =~ /-h/) {	        # print help
      &usage;
    } elsif ($arg =~ /-v/) {	# print version
      &version;
    } elsif ($arg =~ /-t/) {	# get test flag
      $insertflag=undef;
    } elsif ($arg =~ /-f/) {	# get file to read from
      $readfile = shift;
      if (! -e $readfile) {
        print "file not found: $readfile";
        exit 1;
      }
    } elsif ($arg =~ /-a/) {	# get database login file
      $accountfile = shift;
      if (! -r $accountfile) {
        print "file not found or unreadable: $accountfile\n";
        exit 1;
      }
    } elsif ($arg =~ /-w/) {	# get print URL flag
      $printurl=1;
    } elsif ($arg =~ /-d/) {	# get debug flag
      $debug=1;
    } elsif ($arg =~ /-i/) {	# get codwr id
      push @site_num_list, shift;
    } elsif ($arg =~ /-u/) {	# get hdb user
      $hdbuser=shift;
    } elsif ($arg =~ /-p/) {	# get hdb passwd
      $hdbpass=shift;
    } elsif ($arg =~ /-.*/) {	# Unrecognized flag, print help.
      print STDERR "Unrecognized flag: $arg\n";
      usage();
    } else {
      print STDERR "Unrecognized command line argument: $arg\n";
      usage();
    }
  }

  # if user specified codwr gage ids, chop off last comma
  if (@site_num_list) {
    if (grep (/[^A-Z]/, @site_num_list)) {
      die "ERROR: @site_num_list\ndoes not look like CODWR id.\n";
    }
  }

  if (!defined($accountfile) and (!defined($hdbuser) || !defined($hdbpass))) {
    print "ERROR: Required: accountfile (-a) or HDB username and password(-u -p)\n";
    usage();
  }
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

  $nameid->{agen}->{name} = 'Colorado Division of Water Resources';
  $nameid->{collect}->{name} = '(see agency)';
  $nameid->{load_app}->{name} = 'codwr2hdb.pl';
  $nameid->{method}->{name} = 'unknown';
  $nameid->{computation}->{name} = 'unknown';

  $hdb->get_app_ids($nameid);

  $agen_id = $nameid->{agen}->{id};
  $collect_id = $nameid->{collect}->{id};
  $load_app_id = $nameid->{load_app}->{id};
  $method_id = $nameid->{method}->{id};
  $computation_id = $nameid->{computation}->{id};
}

sub process_data
{
  my $data = $_[0];
  my $codwr_no = $_[1];

  $codwr_sites->{$codwr_no}->{found_data} = 1;
  my $cur_sdi = $codwr_sites->{$codwr_no}->{sdi};
  my $cur_site = $codwr_sites->{$codwr_no}->{site_id};

  # put data into database, handing site id
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
}

sub build_url
{
#this url is generated from http://www.dwr.state.co.us/Hydrology/flow_search.asp
# parts of the url:
# program generating the result  : http://www.dwr.state.co.us/Hydrology/flow_data.asp
# specify site                            : ID=$_[0]
# specify discharge(these codes change!)  : MTYPE=$_[1]
# no way to specify how much data is returned

# retrieval from database included data_codes for the various discharge
# column ids

  die "Site id $_[0] not recognized, no datacode known!\n" 
      unless (defined $codwr_sites->{$_[0]}->{data_code});

  my $url = "http://www.dwr.state.co.us/Hydrology/flow_data.asp?";
  $url .= "ID=$_[0]&MTYPE=$codwr_sites->{$_[0]}->{data_code}";

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

# we appropriate the SCS id field for the CO DWR site ids.
sub get_codwr_sites
{
  my $id_limit_clause = '';

# join arguments with "','" to make quoted string comma delimited list
  if (@_) {
    my $commalist = join ('\',\'',@_);
    $id_limit_clause = "b.primary_site_code in ( '$commalist' ) and";
  }


  my $get_codwr_no_statement = "select b.primary_site_code codwr_id, b.primary_data_code data_code,
 b.hdb_interval_name interval, b.hdb_method_id meth_id,
 b.hdb_computation_id comp_id, b.hdb_site_datatype_id sdi,
 d.site_id, d.site_name
from hdb_site_datatype a, ref_ext_site_data_map b,
hdb_site d, hdb_ext_data_source e
where a.site_datatype_id = b.hdb_site_datatype_id and
b.is_active_y_n = 'Y' and $id_limit_clause 
a.site_id = d.site_id and
b.ext_data_source_id = e.ext_data_source_id and
e.ext_data_source_name = '$datasource'
order by codwr_id";

  $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

  my $hashref;

  print $get_codwr_no_statement."\n" if defined($debug);

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
    print "$hdb->dbh->errstr, $@\n";
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
  my $column = 1;

  my $i = 0;
  my $first_date = undef;
  my ($value, $value_date, $updated_date);
  my ($line);
  my ($old_val, $old_source);

# SQL statements

  my $modify_data_statement = "
    BEGIN
        modify_r_base(?,'instant',/*sdi, interval, */
                      ?,null,?, /*start_date_time,null end_date_time, value*/
                      $agen_id,null,'Z', /* overwrite, validation */
                      $collect_id,$load_app_id,$method_id,$computation_id,
                      'Y');                 /*do update? */
    END;";

  eval
  {
    my ($modsth);
    $modsth = $hdb->dbh->prepare($modify_data_statement);

    # insert or update or do nothing for each datapoint;
    foreach $line (@data) {
      #fix the non-reporting of time for their website for midnight,
      # ie, the date times are all in the form MM/DD/YYYY HH:MI:SS AM
      # except for midnight, which is reported as MM/DD/YYYY
      if ($line->[0] =~ /^\d\d?\/\d\d?\/\d{4}$/) {
	$line->[0] .= " 12:00:00 AM";
      }

      $value_date = $line->[0];
      $value = $line->[$column];

      # find the previous value, if any for this point
      undef $old_val;
      $old_val = check_value($value_date, $cur_sdi);
      # check if value is known
      if (!defined $value or $value eq '') {
	print "data missing: $cur_sdi, date $value_date\n" if defined($debug);
	next;
      } elsif ($value =~ m/[^0-9\.]/) { # check for other text, complain
	print "data corrupted: $cur_sdi, date $value_date: $value\n";
	next;
      } elsif (defined($old_val) and $old_val == $value) {
	next;			# source and database are same, do nothing
      } elsif (!defined($old_val) or $old_val != $value) {
	# update or insert, source and database differ (or database value does not exist)
	if (defined($debug)) {
          if (!defined($old_val)) {
            print "modifying for $cur_sdi, date $value_date, value $value, old_val = undef\n";
          } else {
            print "modifying for $cur_sdi, date $value_date, value $value, old_val = $old_val\n";
          }
        }
	#modify
	$modsth->bind_param(1,$cur_sdi);
	$modsth->bind_param(2,$value_date);
	$modsth->bind_param(3,$value);
	$modsth->execute;

	if (!defined($first_date)) { # mark that data has changed
	  $first_date = $value_date;
	}
	$updated_date = $value_date;
      }
    }
    $modsth->finish;

  };				# semicolon here because of use of eval

  if ($@) { # something screwed up in insert/update
    print "$hdb->dbh->errstr, $@\n";
    die "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.\n";
  } elsif ($first_date) {	# commit the data
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
Retrieves CODWR flow data and inserts into HDB.
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
