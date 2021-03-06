#!/usr/bin/perl -w

#insert HDB library

use lib "$ENV{HDB_ENV}/perlLib/lib";

use LWP::UserAgent;
use Date::Calc qw(Delta_DHMS Decode_Date_EU Today Add_Delta_Days);
use File::Basename;
use Data::Dumper;
use XML::Simple;

use Hdb;

use strict;

use English qw( -no_match_vars );

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

#the following are global variables, these are set by command line arguments
my $insertflag = 1;
my ( $readfile, $runagg, $printurl, $debug, $accountfile );
my ( $hdbuser, $hdbpass, @site_num_list );

#HDB database interface
# global because used in several sub routines
my $hdb;

#global datasource name, used in get_app_ids
my $datasource = "NCWCD Webservice";

#global variables read from database in get_app_ids
my ( $load_app_id, $agen_id, $validation, $url, $collect_id );
my $agen_abbrev = "NCWCD";

#global hash reference containing meta data about sites
# initialized by get_codwr_sites below, and referenced throughout
my ($ncwcd_sites);

#global date string representations for use in retrieving data
my ( $begindatestr, $enddatestr ) = process_args(@ARGV);

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
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI'")
  or die "Unable to set NLS_DATE_FORMAT to ISO \n";

get_app_ids();

# get the list of sites to process. Must specify on command line for reading a file
if (@site_num_list) {
  $ncwcd_sites = get_ncwcd_sites(@site_num_list);
} else {
  $ncwcd_sites = get_ncwcd_sites();
  (@site_num_list) = build_site_num_list($ncwcd_sites);
}

=head1 FORMAT

The data is expected in XML format. Example:


Problems:
Name of the flow column changes from
DISCHRG to DISCHRG1 to DISCHRG2 to DISCHRG3
Only one site at a time can be retrieved

=cut


if ( defined($readfile) ) {
  my ($data);
  read_from_file( \$data );

# must be only one entry here
  process_data( \$data, $site_num_list[0] );

} else {    # attempt to get data from CODWR web page
  foreach my $ncwcd_id ( keys %$ncwcd_sites ) {
    my ($data);
    read_from_web( $ncwcd_id, \$data );

    process_data( \$data, $ncwcd_id );

    undef($data);    #needed to reinitialize @data in every loop?
  }    # end of foreach site loop
}

# print error messages for all sites that no or bad data was returned 
my @data_errors;
foreach my $ncwcd_id ( keys %$ncwcd_sites ) {
  my $ncwcd_site = $ncwcd_sites->{$ncwcd_id};
  if ( !defined( $ncwcd_site->{found_data} ) ) {
    push @data_errors,
"No data found for site: $ncwcd_site->{site_name}, $ncwcd_id\n";
  } elsif (defined($ncwcd_site->{error_code})) {
    push @data_errors,
"Bad data seen for site: $ncwcd_site->{site_name}, $ncwcd_id: $ncwcd_site->{error_code}\n";
  }
}

print STDERR sort @data_errors;

exit 0;

#End main program, subroutines below

sub process_args {
  my ( $numdays, $begindate, $enddate, @begindate, @enddate );

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
    } elsif ( $arg =~ /-i/ ) {    # get codwr id, split possible id,id,id
      push @site_num_list, split /,/, shift;
    } elsif ( $arg =~ /-u/ ) {    # get hdb user
      $hdbuser = shift;
    } elsif ( $arg =~ /-p/ ) {    # get hdb passwd
      $hdbpass = shift;
    } elsif ( $arg =~ /-n/ ) {    # get number of days
      $numdays = shift();
    } elsif ( $arg =~ /-b/ ) {    # get begin date
      $begindate = shift();
      if ( not @begindate = Decode_Date_EU($begindate) ) {
        print "Error in begin date specified: $begindate\n";
        usage();
      }
    } elsif ( $arg =~ /-e/ ) {    # get end date
      $enddate = shift();
      if ( not @enddate = Decode_Date_EU($enddate) ) {
        print "Error in end date specified: $enddate\n";
        usage();
      }
    } elsif ( $arg =~ /-.*/ ) {    # Unrecognized flag, print help.
      print STDERR "Unrecognized flag: $arg\n";
      usage();
    } else {
      print STDERR "Unrecognized command line argument: $arg\n";
      usage();
    }
  }

  # if user specified ncwcd ids, check for good id. Can have spaces!!
  if (@site_num_list) {
    if ( grep ( /[^a-zA-Z0-9 ]/, @site_num_list ) ) {
      die "ERROR: @site_num_list\ndoes not look like $agen_abbrev id.\n";
    }
  }
  # if defined readfile, must also specify single id because file format does not specify!
  if (defined($readfile) and @site_num_list != 1) {
      die "ERROR: must specify one and only one site id for reading from a file\n";
  }

  if ( !defined($accountfile) and ( !defined($hdbuser) || !defined($hdbpass) ) )
  {
    print
      "ERROR: Required: accountfile (-a) or HDB username and password(-u -p)\n";
    usage();
  }

  if (!defined($readfile)) { 
    return process_dates( \@enddate, \@begindate, $numdays );
  } else {return;}
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

my $xs;

sub parse_data {
  my $xml = shift;
  my @timestamps;

# initialize global xml::simple object with right options
  if (!defined ($xs)) {
    $xs = XML::Simple->new(ForceArray => 0, KeyAttr => 'Date_Time', GroupTags => { NewDataSet => 'Table' });
  }

  my $ref = eval {$xs->XMLin($$xml) };
  if ($@) {$hdb->hdbdie("Data caused XML parsing error!\n");}


  $ref = $ref->{'diffgr:diffgram'}->{NewDataSet};

#  check to see if data makes sense (improve?)
  if (!defined($ref))  
  {
    return 0;    #no data, next station please!
  }

  print Dumper($ref) if $debug;

# make timestamp, value 2D array
  my $data;

  if (exists($ref->{Val})) { # only one row returned
    push @$data, [ $ref->{Date_Time}, $ref->{Val}];
  } else {
    #sort data by row order ...
    @timestamps = sort { $ref->{$a}->{'msdata:rowOrder'} <=> $ref->{$b}->{'msdata:rowOrder'} } keys %$ref;
    foreach my $key ( @timestamps ) {
      push @$data, [ $key, $ref->{$key}->{'Val'} ];
    }
  }

  print Dumper($data) if $debug;

  return $data;
}

sub process_data {
  my $xml      = $_[0];
  my $ncwcd_id = $_[1];

  return if (!defined ($xml)) or ($xml eq 0);    # no data?

  my $dataref = parse_data($xml);

  return if ($dataref == 0); # no data found

  #check that this site is defined completely from database
  if (    !defined( $ncwcd_sites->{$ncwcd_id}->{sdi} )
       or !defined( $ncwcd_sites->{$ncwcd_id}->{site_id} )
       or !defined( $ncwcd_sites->{$ncwcd_id}->{data_code} ) )
  {
    $hdb->hdbdie(
             "site or sdi or data code not defined for id: $ncwcd_id!\n");
  }

  # now we have data, mark data as found 
  $ncwcd_sites->{$ncwcd_id}->{found_data} = 1;

  my $ncwcd_site = $ncwcd_sites->{$ncwcd_id};

  # put data into database, handing site id
  # function returns date of first value and last value updated
  my ( $first_date, $updated_date, $rows_processed );

  if ( defined($insertflag) ) {

    #tell user something, so they know it is working
    print "Working on $agen_abbrev gage number: $ncwcd_id\n";
    ( $first_date, $updated_date, $rows_processed ) = 
       insert_values( $dataref, $ncwcd_site );

#report results of insertion, and report error codes to STDERR
    if ( !defined($first_date) ) {
      print "No data updated for $ncwcd_site->{site_name}, $ncwcd_id\n";
    } else {
      print
"Data processed from $first_date to $updated_date for $ncwcd_sites->{$ncwcd_id}->{site_name}, $ncwcd_id\n";
      print "Number of rows from $agen_abbrev processed: $rows_processed\n";
    }
  }
}

sub build_url ($$$) {
  my $site_code  = $_[0];
  my $begin_date = $_[1];
  my $end_date   = $_[2];

#this url is generated from an Excel template from Northern. 
# Exact URL is started with data stored in comment field in hdb_ext_data_source table, but
# it looks something like:
# https://rest.ncwcd.org:3650/NWDATA/getData/username/password/fd/dg/4060/QME/DAY/YYYY-MM-DD/YYYY-MM-DD/RECLAMAT
# Old: https://rest.ncwcd.org:3650/NWDATA/getData/BOR_Gilmore/dd50a8e8-59c1-4314-935c-bcafe82da124/fd/dg/
# New: https://rest.ncwcd.org:3650/NWDATA/getData/TMiller/92d8d0ba-32cf-4a0a-a0ec-8a50bdd3a56b/fd/dg/
# parts of the url:
# program generating the result (from HDB):  https://rest.ncwcd.org:3650/NWDATA/getData/username/password/fd/dg/
# specify site                            : 4060/
# specify data (QME, QIN, PELV, RSTO      : QME/
# specify instant(MINUTE) or daily (DAY)  : DAY/
# specify start and end dates             : YYYY-MM-DD/YYYY-MM-DD/
# specify random 8 digit string (purpose?): RECLAMAT
  # retrieval from database included data_codes for the various discharge
  # column ids

  die "Site id $_[0] not recognized, no datacode known!\n"
    unless ( defined $ncwcd_sites->{ $_[0] }->{data_code} );

  my $parameters = "$site_code/$ncwcd_sites->{$_[0]}->{data_code}/";

  if ($ncwcd_sites->{ $_[0] }->{interval} eq "instant" ) {
    $parameters .= "MINUTE/";
  } elsif ($ncwcd_sites->{ $_[0] }->{interval} eq "day" ) {
    $parameters .= "DAY/";
  } else {
    $hdb->hdbdie("Unknown interval defined for $agen_abbrev web service");
  }

  $parameters .= "$begin_date/$end_date/RECLAMAT";

  # url described by database, retrieved in get_app_ids()

  return $url . $parameters;
}

sub build_site_num_list {
  my $ncwcd_sites = $_[0];
  my ( $ncwcd_no, @site_num_list );

  #build list of sites to grab data for
  foreach $ncwcd_no ( keys %$ncwcd_sites ) {
    push @site_num_list, $ncwcd_no;
  }

  return @site_num_list;
}

sub build_web_request {
  my $ua = LWP::UserAgent->new;
  $ua->agent(
         "$agen_abbrev Streamflow -> US Bureau of Reclamation HDB dataloader: "
           . $ua->agent );
  $ua->from('$ENV{HDB_XFER_EMAIL}');
  my $request = HTTP::Request->new();
  $request->method('GET');
  return ( $ua, $request );
}

# we appropriate the SCS id field for the CO DWR site ids.
sub get_ncwcd_sites {
  my $id_limit_clause = '';

  # join arguments with "','" to make quoted string comma delimited list
  if (@_) {
    my $commalist = join( '\',\'', @_ );
    $id_limit_clause = "b.primary_site_code in ( '$commalist' ) and";
  }

  my $get_ncwcd_id_statement =
    "select b.primary_site_code ncwcd_id, b.primary_data_code data_code,
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
order by ncwcd_id";

  $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

  my $hashref;

  print $get_ncwcd_id_statement. "\n" if defined($debug);

  eval {
    $hashref = $hdb->dbh->selectall_hashref( $get_ncwcd_id_statement, 1 );
  };

  if ($@) {    # something screwed up
    print $hdb->dbh->errstr, ": $@\n";
    die "Errors occurred during automatic selection of $agen_abbrev gage ids.\n";
  }

  return $hashref;
}

sub insert_values {
  my @data       = @{ $_[0] };
  my $ncwcd_site = $_[1];
  my $cur_sdi    = $ncwcd_site->{sdi};

  #fixed the url encoding to only retrieve one
  # parameter, so no longer need this, but could need it again
  my $column = 1;

  my $numrows    = 0;
  my $first_date = undef;
  my ( $value, $value_date, $tz, $updated_date );
  my ( $row);
  my ( $old_val, $old_source );

  if (    !defined( $ncwcd_site->{interval} )
       or !defined($agen_id)
       or !defined($load_app_id)
       or !defined($collect_id)
       or !defined( $ncwcd_site->{meth_id} )
       or !defined( $ncwcd_site->{comp_id} ) )
  {
    $hdb->hdbdie(
"Unable to write data to database for $ncwcd_site->{site_name}
Required information missing in insert_values()!\n"
    );
  }

  # SQL statements

  my $modify_data_statement = "
    BEGIN
        modify_r_base(?,'$ncwcd_site->{interval}',/*sdi, interval, */
                      ?,null,?,  /*start_date_time,null end_date_time, value*/
                      $agen_id,null,'$validation', /*agen, overwrite, validation*/
                      $collect_id,$load_app_id,
                      $ncwcd_site->{meth_id},$ncwcd_site->{comp_id},
                      'Y',null,                  /*do update?, data flags */
                      ?); /* timezone for NCWCD, they include timezone in data */
    END;";

  eval {
    my ($modsth);
    $modsth = $hdb->dbh->prepare($modify_data_statement);

    # insert or update or do nothing for each datapoint;
    foreach $row (@data) {
      $numrows++;

# timestamp is in format YYYY-MM-DDTHH:MI:SS -06:00
# truncate seconds, and transform timezone
      $value_date = substr($row->[0], 0, 16);
      $value_date =~ s/T/ /;
      $tz         = substr($row->[0], 19);
      $value      = $row->[1];

      $tz = 'MDT' if ($tz eq '-06:00');
      $tz = 'MST' if ($tz eq '-07:00');


# update or insert, source and database differ (or database value does not exist)
        if ( defined($debug) ) {
	    print "modifying for $cur_sdi, date $value_date, timezone $tz, value $value\n";
        }

       # check if value is known
       if ( !defined $value or $value eq '' ) {
         print "data missing: $cur_sdi, date $value_date\n" if defined($debug);
         next;
       } elsif ( $value =~ m/[^0-9\.]/ ) {    # check for other text, complain
         print "data corrupted: $cur_sdi, date $value_date: $value\n";
         $ncwcd_site->{error_code} = $value;
       } else {       #modify
        $modsth->bind_param( 1, $cur_sdi );
        $modsth->bind_param( 2, $value_date );
        $modsth->bind_param( 3, $value );
        $modsth->bind_param( 4, $tz);
        $modsth->execute;

        if ( !defined($first_date) ) {    # mark that data has changed
          $first_date = $value_date;
        }
        $updated_date = $value_date;
      } 
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

sub usage {
  print STDERR <<"ENDHELP";
$progname [ -h | -v ] | [ options ]
Retrieves $agen_abbrev flow data and inserts into HDB.
Example: $progname -u app_user -p <hdbpassword> -i AZOTUNNM

  -h               : This help
  -v               : Version
  -u <hdbusername> : HDB application user name (REQUIRED)
  -p <hdbpassword> : HDB password (REQUIRED)
  -i <ncwcd_id>    : $agen_abbrev gage id to look for. Must be in HDB. More than one
                     may be specified with -i id1,id2 or -i id1 -i id2
                     If none specified, will load all gages set up in HDB.
  -t               : Test retrieval, but do not insert data into DB
  -f <filename>    : Filename instead of live web, must specify -i 
  -w               : Web address (URL developed to get $agen_abbrev data)
  -d               : Debugging output
  -n               : number of days to load
  -b <DD-MON-YYYY> : Begin date for data retrieval
  -e <DD-MON-YYYY> : End date for data retrieval
ENDHELP

  exit(1);
}

sub version {
  my $verstring = '$Revision: 1.1 $';
  $verstring =~ s/^\$Revision: //;
  $verstring =~ s/\$//;

  print "$progname version $verstring\n";
  exit 1;
}

sub read_from_file {
  my $data = shift;

  open INFILE, "$readfile" or $hdb->hdbdie("Cannot open $readfile: $!\n");
  $$data = <INFILE>;
  print $$data         if defined($debug);
  print Dumper($$data) if defined($debug);
  return ();
}

sub read_from_web {
  my $ncwcd_id = shift;
  my $data     = shift;

  my $url = build_url( $ncwcd_id, $begindatestr, $enddatestr );

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

    $$data = $response->content;
  } else {    # ($response->is_error)
    printf STDERR $response->error_as_HTML;
    $hdb->hdbdie("Error reading from $agen_abbrev web page, cannot continue\n");
  }
  return;
}

sub process_dates {
  my $enddate   = shift;
  my $begindate = shift;
  my $numdays   = shift;

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
  if ( defined($numdays) ) {
    if ( $numdays =~ m/\D/ ) {
      print "Number of days (argument to -n) must be numeric.\n";
      usage();
    } elsif ($numdays <= 0) {
      print "Number of days (argument to -n) must be greater than 0.\n";
    }

    if ( @$begindate and @$enddate ) {
      print "Error, overspecified dates, all of -n, -b, -e specified!\n";
      exit 1;
    } elsif (@$begindate) {
      @$enddate = Add_Delta_Days( @$begindate, $numdays );
    } elsif (@$enddate) {
      @$begindate = Add_Delta_Days( @$enddate, -$numdays );
    } else {
      @$enddate = Add_Delta_Days(Today(), 1);
      @$begindate = Add_Delta_Days( @$enddate, -$numdays );
    }
  } else {
    if ( @$begindate and @$enddate ) {

      #do nothing, we're good
    } elsif (@$begindate) {
      @$enddate = Add_Delta_Days(Today(), 1);
    } elsif (@$enddate) {
      print "Error, cannot specify only end date!\n";
      exit 1;
    } else {
      print "Error, dates are unspecified!\n";
      usage();
    }
  }

  if ( !@$begindate or !@$enddate ) {
    print "Error, dates not specifed or error parsing dates!\n";
    exit(1);
  }

  my $begindatestr = sprintf( "%4d-%02d-%02d", @$begindate );
  my $enddatestr   = sprintf( "%4d-%02d-%02d", @$enddate );

  return ( $begindatestr, $enddatestr );
}
