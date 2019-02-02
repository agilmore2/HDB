#!/usr/bin/perl -w

use strict;
use warnings;

#use libraries from HDB environment (HDB.pm here)
# the following line was changed by M. Bogner for the AThena to ISIS move
# below line changed by M. Bogner March 2011 to use 64 Bit Perl Libraries
use lib ( defined $ENV{PERL_ENV} ? "$ENV{PERL_ENV}/lib" : "$ENV{HDB_ENV}/perlLib/lib" );

use LWP::UserAgent;
use Date::Calc qw(Delta_DHMS Decode_Date_EU Today Add_Delta_Days);
use File::Basename;
use Data::Dumper;

#insert HDB library
use Hdb;

use English qw( -no_match_vars );

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

#the following are global variables, these are set by command line arguments
my $insertflag = 1;
my ( $readfile, $runagg, $printurl, $debug, $accountfile, $flowtype);
my ( $hdbuser, $hdbpass, @site_num_list );

#HDB database interface
# global because used in several sub routines
my $hdb;

#global variables read from database in get_app_ids
my ( $load_app_id, $agen_id, $validation,
     $url, $collect_id );
     
my $agen_abbrev      = 'IBWC';
my $datasource      = 'IBWC Unit Values (Realtime)';

#global hash reference containing meta data about sites
# initialized by get_codwr_sites below, and referenced throughout
my ($ibwc_sites);

# No dates can be specified
process_args(@ARGV);

$hdb = Hdb->new();
if ( defined($accountfile) ) {
  $hdb->connect_from_file($accountfile);
} else {
  my $dbname;
  if ( !defined( $dbname = $ENV{HDB_LOCAL} ) ) {
    $hdb->hdbdie("Environment variable HDB_LOCAL not set...\n");
  }

  #create connection to database
  $hdb->connect_to_db( $dbname, $hdbuser, $hdbpass );
}

#set write ability on connection (app_role)
$hdb->set_approle();

#Set date format to the format used by their website
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/YYYY   HH24:MI'")
  or die "Unable to set NLS_DATE_FORMAT to IBWC Text format\n";

get_app_ids();

if (@site_num_list) {
  $ibwc_sites = get_ibwc_sites(@site_num_list);
} else {
  $ibwc_sites = get_ibwc_sites();
  (@site_num_list) = build_site_num_list($ibwc_sites);
}

=head1 FORMAT

The data is expected in fixed width format. Example:

ALL DATA ARE PROVISIONAL AND SUBJECT TO REVISION
08363300 Rio Grande at Hayner's Bridge
    Date     Time                                                  Precip
             MST       Stage (m)          Discharge (cms)        (Accum-mm)
07/07/2018   09:45       1.761                                          
07/07/2018   09:30       1.764                                          
07/07/2018   09:15       1.763                                          
07/07/2018   09:00       1.765                47.40                     
07/07/2018   08:45       1.764                47.30                     
07/07/2018   08:30       1.765                47.40                     
07/07/2018   08:15       1.761                47.10                     
07/07/2018   08:00       1.761                47.10                44.00

Problems:
Only one site at a time can be retrieved
Metric
Fixed width???
=cut


if ( defined($readfile) ) {
  my (@data);
  read_from_file( \@data );

  process_data( \@data );

} else {    # attempt to get data from IBWC web page
  foreach my $ibwc_no ( keys %$ibwc_sites ) {
    my (@data);
    read_from_web( $ibwc_no, \@data );

    process_data( \@data, $ibwc_no );

    undef(@data);    #needed to reinitialize @data in every loop?
  }    # end of foreach site loop
}

# print error messages for all sites that no or bad data was returned 
my @data_errors;
foreach my $ibwc_no ( keys %$ibwc_sites ) {
  my $ibwc_site = $ibwc_sites->{$ibwc_no};
  if ( !defined( $ibwc_site->{found_data} ) ) {
    push @data_errors,
"No data found for site: $ibwc_site->{site_name}, $ibwc_no\n";
  } elsif (defined($ibwc_site->{error_code})) {
    push @data_errors,
"Bad data seen for site: $ibwc_site->{site_name}, $ibwc_no: $ibwc_site->{error_code}\n";
  }
}

print STDERR sort @data_errors;

exit 0;

#End main program, subroutines below

sub process_args {
  while (@_) {
    my $arg = shift;
    if ( $arg =~ /-h/ ) {    # print help
      &usage;
    } elsif ( $arg =~ /-v/ ) {    # print version
      &version;
    } elsif ( $arg =~ /-t/ ) {    # get test flag
      $insertflag = undef;
    } elsif ( $arg =~ /-f/ ) {    # get file to read from
      $readfile = shift;
      if ( !-e $readfile ) {
        print "file not found: $readfile";
        exit 1;
      }
    } elsif ( $arg =~ /-a/ ) {    # get database login file
      $accountfile = shift;
      if ( !-r $accountfile ) {
        print "file not found or unreadable: $accountfile\n";
        exit 1;
      }
    } elsif ( $arg =~ /-w/ ) {    # get print URL flag
      $printurl = 1;
    } elsif ( $arg =~ /-d/ ) {    # get debug flag
      $debug = 1;
    } elsif ( $arg =~ /-i/ ) {    # get ibwc id, split possible id,id,id
      push @site_num_list, split /,/, shift;
    } elsif ( $arg =~ /-u/ ) {    # get hdb user
      $hdbuser = shift;
    } elsif ( $arg =~ /-p/ ) {    # get hdb passwd
      $hdbpass = shift;
    } elsif ( $arg =~ /-.*/ ) {    # Unrecognized flag, print help.
      print STDERR "Unrecognized flag: $arg\n";
      usage();
    } else {
      print STDERR "Unrecognized command line argument: $arg\n";
      usage();
    }
  }

  # if user specified ibwc gage ids, chop off last comma
  if (@site_num_list) {
    if ( grep ( /[^0-9]/, @site_num_list ) ) {
      die "ERROR: @site_num_list\ndoes not look like $agen_abbrev id.\n";
    }
  }

  if ( !defined($accountfile) and ( !defined($hdbuser) || !defined($hdbpass) ) )
  {
    print
      "ERROR: Required: accountfile (-a) or HDB username and password(-u -p)\n";
    usage();
  }

  return;
}

sub get_app_ids {

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
    $sth->bind_col( 1, \$load_app_id );
    unless ( $sth->fetch ) {
      $hdb->hdbdie("Loading application id for $load_app_name not found!\n");
    }
    $sth->fetch;
    $sth->finish();
  };

  if ($@) {    # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie(
"Errors occurred during selection of loading application id for $load_app_name.\n"
    );
  }

  my $get_other_info_statement = "select e.agen_id,
 e.collection_system_id collect_id, e.data_quality validation,
 e.description url
 from hdb_ext_data_source e
 where e.ext_data_source_name = '$datasource'";

  eval {
    $sth = $hdb->dbh->prepare($get_other_info_statement);
    $sth->execute;
    $sth->bind_col( 1, \$agen_id );
    $sth->bind_col( 2, \$collect_id );
    $sth->bind_col( 3, \$validation );
    $sth->bind_col( 4, \$url );
    my $stuff = $sth->fetch;
    Dumper($stuff);
    unless ($stuff) {
      $hdb->hdbdie(
"Data source definition not found, $agen_id, $collect_id, $validation, $url!\n"
      );
    }
    $sth->finish();
  };

  if ($@) {    # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie(
       "Errors occurred during selection of data source info for $datasource.\n"
    );
  }

  if ( !defined($validation) ) {

    #needs to be empty string for dynamic sql to recognize as null, not 'null'!
    $validation = '';
  }

}

sub skip_comments {
  my $data = shift;

  if ($data->[0] eq 'ALL DATA ARE PROVISIONAL AND SUBJECT TO REVISION') {
      shift @$data;
  } else {
    $hdb->hdbdie("Data is not $agen_abbrev website tab delimited format!\n");
  }

  return ();
}

sub read_header {

  my $data = shift;

  #save header record, for use in finding data column
  my $header = shift @$data;
  my @headers = split / /, $header;
  if (!($headers[0] =~ /[[:digit:]]{8,}/ )) {
    print "@headers\n";
  }
  my $ibwc_no = $headers[0];

  #DISABLE this because sometimes the retrieved file does not use the same id as it is retrieved from!
  # check that ibwc_no is sane, i.e. contains only digits:
  if ( $ibwc_no =~ /[^\d]/ ) {
    #may be returning alpha id that doesn't match, but is right site
#    print STDERR
#      "Warning! '$ibwc_no' does not appear to be a valid $agen_abbrev site number!\n";
#    return 0; #Go to next site retrieval
  } else { #only chop if starts with number
      #chop 2 digit HUC off of front of ID in file to match IWBC id
      $ibwc_no = substr $ibwc_no, 2;
  }
  
  shift @$data; #skip "    Date     Time " header
  my $timezone = unpack("A18", shift @$data); #pull timezone out of "     MST       Stage (m)          Discharge (cms)  ..." header
  $timezone =~ s/\s//g;
  # now we have data, mark data as found in IBWC ID
  # assume that the format holds, and the id is in second column
  #set found data for this ibwc no
  $ibwc_sites->{$ibwc_no}->{found_data} = 1;
  $ibwc_sites->{$ibwc_no}->{timezone} = $timezone;

  return ($ibwc_no);
}

sub process_data {
  my $data           = $_[0];
  my $input_ibwc_no = $_[1];

  chop(@$data); #sent in \r\n, so need to remove \r. \n handled by split in response or chomp in read_from_file.

  skip_comments($data); 

  my $ibwc_no = read_header($data);

  return if $ibwc_no eq 0;    # no station found

  if ( defined($input_ibwc_no)
       and $input_ibwc_no ne $ibwc_no )
  {                            #different id, so copy into right site
      foreach my $key (keys %{$ibwc_sites->{$ibwc_no}}) { $ibwc_sites->{$input_ibwc_no}->{$key} = $ibwc_sites->{$ibwc_no}->{$key}}
#    $hdb->hdbdie("$input_ibwc_no was not returned by web query! Exiting.");
  }

  my $ibwc_site = $ibwc_sites->{$input_ibwc_no};

  #check that this site is defined completely from database
  if (    !defined( $ibwc_site->{sdi} )
       or !defined( $ibwc_site->{site_id} )
       or !defined( $ibwc_site->{data_code} ) )
  {
    $hdb->hdbdie(
             "site or sdi or data code not defined for ibwc id: $input_ibwc_no!\n");
  }

  # put data into database, handing site id
  # function returns date of first value and last value updated
  my ( $first_date, $updated_date, $rows_processed );

  if ( defined($insertflag) ) {

    #tell user something, so they know it is working
    print "Working on $agen_abbrev gage number: $input_ibwc_no\n";
    ( $first_date, $updated_date, $rows_processed ) = 
       insert_values( $data, $ibwc_site );

#report results of insertion, and report error codes to STDERR
    if ( !defined($first_date) ) {
      print "No data updated for $ibwc_site->{site_name}, $input_ibwc_no\n";
    } else {
      print
"Data processed from $first_date to $updated_date for $ibwc_sites->{$input_ibwc_no}->{site_name}, $input_ibwc_no\n";
      print "Number of rows from $agen_abbrev processed: $rows_processed\n";
    }
  }
}

sub build_url ($) {
  my $site_code  = $_[0];

# parts of the url:
# Website generating the result           : https://ibwc.gov/wad
# specify site                            : $_[0]
# specify ASCII?                          : _a.txt
# No provision for data codes or time range

  my $parameters .= "/$_[0]_a.txt";

  # url described by database, retrieved in get_app_ids()

  return $url . $parameters;
}

sub build_site_num_list {
  my $ibwc_sites = $_[0];
  my ( $ibwc_num, @site_num_list );

  #build list of sites to grab data for
  foreach $ibwc_num ( keys %$ibwc_sites ) {
    push @site_num_list, $ibwc_num;
  }

  return @site_num_list;
}

sub build_web_request {
  my $ua = LWP::UserAgent->new;
  $ua->agent(
         "$agen_abbrev Streamflow -> US Bureau of Reclamation HDB dataloader: "
           . $ua->agent );
  $ua->from("$ENV{HDB_XFER_EMAIL}");
  my $request = HTTP::Request->new();
  $request->method('GET');
  return ( $ua, $request );
}

# we appropriate the SCS id field for the CO DWR site ids.
sub get_ibwc_sites {
  my $id_limit_clause = '';

  # join arguments with "','" to make quoted string comma delimited list
  if (@_) {
    my $commalist = join( '\',\'', @_ );
    $id_limit_clause = "b.primary_site_code in ( '$commalist' ) and";
  }

  my $get_ibwc_no_statement =
    "select b.primary_site_code ibwc_id, b.primary_data_code data_code,
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
order by ibwc_id";

  $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

  my $hashref;

  print $get_ibwc_no_statement. "\n" if defined($debug);

  eval {
    $hashref = $hdb->dbh->selectall_hashref( $get_ibwc_no_statement, 1 );
  };

  if ($@) {    # something screwed up
    print $hdb->dbh->errstr, ": $@\n";
    die "Errors occurred during automatic selection of $agen_abbrev gage ids.\n";
  }

  return $hashref;
}

sub check_value {
  my ($check_date)       = $_[0];
  my ($site_datatype_id) = $_[1];
  my ( $sth, $value );

  my $check_data_statement = "select value from r_base
where start_date_time = ? and site_datatype_id = ? and interval = 'instant'";

  eval {
    $sth = $hdb->dbh->prepare($check_data_statement);
    $sth->bind_param( 1, $check_date );
    $sth->bind_param( 2, $site_datatype_id );

    $sth->execute;
    $sth->bind_col( 1, \$value );
    $sth->fetch;
    $sth->finish();
  };

  if ($@) {    # something screwed up
    print "$hdb->dbh->errstr, $@\n";
    die "Errors occurred during selection of data already in database.\n";
  }
  return ($value);
}

sub insert_values {
  my @data       = @{ $_[0] };
  my $ibwc_site = $_[1];
  my $flow_sdi    = $ibwc_site->{sdi};
  my $stage_sdi   = $ibwc_site->{stage_sdi};
  
  my $numrows    = 0;
  my $first_date = undef;
  my ( $value, $value_date, $updated_date );
  my ( $stage, $row,        @fields );
  my ( $old_val, $old_source );

  if (    !defined( $ibwc_site->{interval} )
       or !defined($agen_id)
       or !defined($load_app_id)
       or !defined($collect_id)
       or !defined( $ibwc_site->{meth_id} )
       or !defined( $ibwc_site->{comp_id} ) )
  {
    $hdb->hdbdie(
"Unable to write data to database for $ibwc_site->{site_name}
Required information missing in insert_values()!\n"
    );
  }

  # SQL statements

  my $modify_data_statement = "
    BEGIN
        modify_r_base(?,'$ibwc_site->{interval}',/*sdi, interval, */
                      ?,null,?, /*start_date_time,null end_date_time, value*/
                      $agen_id,null,'$validation',/*agen, overwrite, validation*/
                      $collect_id,$load_app_id,
                      $ibwc_site->{meth_id},$ibwc_site->{comp_id},
                      'Y',null,                  /*do update?, data flags */
                      '$ibwc_site->{timezone}'); /* timezone for IBWC, they present in localtime */
    END;";

  eval {
    my ($modsth);
    $modsth = $hdb->dbh->prepare($modify_data_statement);

    # insert or update or do nothing for each datapoint;
    foreach $row (@data) {
      $numrows++;

      #07/07/2018   08:00       1.761                47.10                44.00
      # First floating point is stage, second is discharge if present, third could be precip or....
      ($value_date,$stage,$value) = unpack ("a18 A18 A18", $row); #need to keep spaces in date/time field. Rest of spaces can be dropped
      write_value($modsth,$value_date,$flow_sdi,$value,$ibwc_site);
      if (defined($stage_sdi)) {
        write_value($modsth,$value_date,$stage_sdi,$stage,$ibwc_site);
      }
	      
      if ( !defined($first_date) ) {    # mark that data has changed
          $first_date = $value_date;
      }
      $updated_date = $value_date;
    }
  
    $modsth->finish;

  };    # semicolon here because of use of eval

  
  if ($@) {    # something screwed up in insert/update
    print "$hdb->dbh->errstr, $@\n";
    $hdb->dbh->rollback;
    $hdb->hdbdie(
"Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.\n");
  } elsif ($first_date) {    # commit the data
    if ( defined($debug) ) {
      $hdb->dbh->rollback or $hdb->hdbdie($hdb->dbh->errstr);
    } else {
      $hdb->dbh->commit or $hdb->hdbdie($hdb->dbh->errstr);
    }
  }
  return ( $first_date, $updated_date, $numrows );
}

sub write_value() {
  my $modsth     = shift;
  my $value_date = shift;
  my $cur_sdi    = shift;
  my $value      = shift;
  my $ibwc_site  = shift;

  $value =~ s/^\s+|\s+$//g; #remove all other whitespace too

  # find the previous value, if any for this point
  my $old_val;
  $old_val = check_value( $value_date, $cur_sdi );

  # check if value is known
  if ( !defined $value or $value eq '' ) {
    print "data missing: $cur_sdi, date $value_date\n" if defined($debug);
    next;
  } elsif ( $value =~ m/[^0-9\.]/ ) {    # check for other text, complain
    print "data corrupted: $cur_sdi, date $value_date: $value\n";
    $ibwc_site->{error_code} = $value;
    next;
  } elsif ( defined($old_val) and $old_val == $value ) {
    next;    # source and database are same, do nothing
  } elsif ( !defined($old_val) or $old_val != $value ) {
  #update or insert, source and database differ (or database value does not exist)
    if ( defined($debug) ) {
      if ( !defined($old_val) ) {
          print "modifying for $cur_sdi, date $value_date, value $value, old_val = undef\n";
      } else {
        print "modifying for $cur_sdi, date $value_date, value $value, old_val = $old_val\n";
      }
    }

    #modify
    $modsth->bind_param( 1, $cur_sdi );
    $modsth->bind_param( 2, $value_date );
    $modsth->bind_param( 3, $value );
    $modsth->execute;
  }
  return;
}
  
sub usage {
  print STDERR <<"ENDHELP";
$progname [ -h | -v ] | [ options ]
Retrieves $agen_abbrev flow data and inserts into HDB.
Example: $progname -u app_user -p <hdbpassword> -i AZOTUNNM

  -h               : This help
  -v               : Version
  -u <hdbusername> : HDB application user name (REQUIRED)
  -p <hdbpassword> : HDB password (REQUIRED)
  -i <ibwc_id>    : $agen_abbrev gage id to look for. Must be in HDB. More than one
                     may be specified with -i id1,id2 or -i id1 -i id2
                     If none specified, will load all gages set up in HDB.
  -t               : Test retrieval, but do not insert data into DB
  -f <filename>    : Filename instead of live web
  -w               : Web address (URL developed to get $agen_abbrev data)
  -d               : Debugging output
  -l <u,d>         : Specify flow type: instanteous ("unit value", default),or daily
ENDHELP

  exit(1);
}

sub version {
  my $verstring = '$Revision$';
  $verstring =~ s/^\$Revision: //;
  $verstring =~ s/\$//;

  print "$progname version $verstring\n";
  exit 1;
}

sub read_from_file {
  my $data = shift;

  open INFILE, "$readfile" or $hdb->hdbdie("Cannot open $readfile: $!\n");
  @$data = <INFILE>;
  print @$data         if defined($debug);
  print Dumper(@$data) if defined($debug);
  chomp @$data;
  return ();
}

sub read_from_web {
  my $ibwc_no = shift;
  my $data     = shift;

  my $url = build_url( $ibwc_no);

  if ( defined($printurl) or defined($debug) ) {
    print "$url\n";
  }

  my ( $ua, $request ) = build_web_request();
  $request->uri($url);

  # this next function actually gets the data
  my $response = $ua->simple_request($request);

  # check result
  if ( $response->is_success ) {
    my $status = $response->status_line;
    print "Data read from $agen_abbrev: $status\n";

    if ( !$response->content ) {
      $hdb->hdbdie("no data returned from $agen_abbrev, exiting.\n");
    }
    print $response->content if defined($debug);

    @$data = split /$INPUT_RECORD_SEPARATOR/, $response->content;
  } else {    # ($response->is_error)
    printf STDERR $response->error_as_HTML;
    $hdb->hdbdie("Error reading from $agen_abbrev web page, cannot continue\n");
  }
  return;
}
