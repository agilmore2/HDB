#!/usr/bin/perl -w

#insert HDB library

use lib ( defined $ENV{PERL_ENV} ? "$ENV{PERL_ENV}/lib" : "$ENV{HDB_ENV}/perlLib/lib" );

#use LWP::UserAgent;
#use LWP::Debug qw(+conns); 
use WWW::Mechanize;
use Date::Calc qw(Delta_DHMS Decode_Date_EU Today Add_Delta_Days Add_Delta_DHMS);
use File::Basename;
use Data::Dumper;

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

#global variables read from database in get_app_ids
my ( $load_app_id, $agen_id, $validation, $url, $collect_id );
my ( $agen_abbrev ); #initialized by command line argument

#global hash reference containing meta data about sites
# initialized by get_sites below, and referenced throughout
my ($_sites);

#global date string representations for use in retrieving data
my ( $begindatestr, $enddatestr ) = process_args(@ARGV);

#global datasource name, used in get_app_ids
my $datasource = "$agen_abbrev OneRain Website CSV Loader";

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
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI:SS'")
  or $hdb->hdbdie( "Unable to set NLS_DATE_FORMAT to YYYY-MM-DD HH24:MI:SS\n");

get_app_ids();

# get the list of sites to process. Must specify on command line for reading a file
if (@site_num_list) {
  $_sites = get_sites(@site_num_list);
} else {
  $_sites = get_sites();
  (@site_num_list) = build_site_num_list($_sites);
}

=head1 FORMAT

The data is expected in CVS format. Example:

Reading,Receive,Value,Unit,Data Quality
2016-07-22 23:40:00,2016-07-22 23:40:03,5.35,ft,A
2016-07-22 23:10:03,2016-07-22 23:10:07,5.34,ft,A
2016-07-22 22:39:58,2016-07-22 22:40:01,5.34,ft,A
2016-07-22 22:10:18,2016-07-22 22:10:21,5.33,ft,A

=cut


if ( defined($readfile) ) {
  my ($data);
  read_from_file( \$data );

# must be only one entry here
  process_data( \$data, $site_num_list[0] );

} else {    # attempt to get data from web page
  foreach my $a_id ( keys %$_sites ) {
    my ($data);
    if (read_from_web( $a_id, $_sites->{$a_id}->{dev_code}, \$data )) {
      process_data( \$data, $a_id );
    }

    undef($data);    #needed to reinitialize @data in every loop?
  }    # end of foreach site loop
}

# print error messages for all sites that no or bad data was returned 
my @data_errors;
foreach my $a_id ( keys %$_sites ) {
  my $_site = $_sites->{$a_id};
  if ( !defined( $_site->{found_data} ) ) {
    push @data_errors,
"No data found for site: $_site->{site_name}, $a_id\n";
  } elsif (defined($_site->{error_code})) {
    push @data_errors,
"Bad data seen for site: $_site->{site_name}, $a_id: $_site->{error_code}\n";
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
    } elsif ( $arg =~ /-l/ ) {    # get agency abbreviation
      $agen_abbrev = shift;
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

  # if user specified ids, check for good id. Can have spaces!!
  if (@site_num_list) {
    if ( grep ( /[^a-zA-Z0-9 ]/, @site_num_list ) ) {
      die "ERROR: @site_num_list\ndoes not look like $agen_abbrev id.\n";
    }
  }
  # if defined readfile, must also specify single id because file format does not specify!
  if (defined($readfile) and @site_num_list != 1) {
      die "ERROR: must specify one and only one site id for reading from a file\n";
  }
  if ( !defined($agen_abbrev)) {
    print "ERROR: Required Agency Abbreviation not set ('-l <abbrev>')\n";
    usage();
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

sub parse_data {
  my $csv = shift;
 
# make array out of string
  my (@array) = split /^/m,$$csv;

#just check that CSV file is in right format
  my $row = shift @array;
  chomp $row;
  
  if (!($row =~ /Reading,Receive,Value,Unit,Data Quality.*/))
  {
      $hdb->hdbdie(
             "File not in expected format: $row\n"); 
  }
  
  print Dumper($csv) if $debug;

  return \@array;
}

sub process_data {
  my $csv  = $_[0];
  my $a_id = $_[1];

  return if (!defined ($csv)) or ($csv eq 0);    # no data?

  my $dataref = parse_data($csv);

  return if ($dataref == 0); # no data found

  #check that this site is defined completely from database
  if (    !defined( $_sites->{$a_id}->{sdi} )
       or !defined( $_sites->{$a_id}->{site_id} )
       or !defined( $_sites->{$a_id}->{dev_code} ) )
  {
    $hdb->hdbdie(
             "site or sdi or data code not defined for id: $a_id!\n");
  }

  # now we have data, mark data as found 
  $_sites->{$a_id}->{found_data} = 1;

  my $_site = $_sites->{$a_id};

  # put data into database, handing site id
  # function returns date of first value and last value updated
  my ( $first_date, $updated_date, $rows_processed );

  if ( defined($insertflag) ) {

    #tell user something, so they know it is working
    print "Working on $agen_abbrev gage number: $a_id\n";
    ( $first_date, $updated_date, $rows_processed ) = 
       insert_values( $dataref, $_site );

#report results of insertion, and report error codes to STDERR
    if ( !defined($first_date) ) {
      print "No data updated for $_site->{site_name}, $a_id\n";
    } else {
      print
"Data processed from $first_date to $updated_date for $_sites->{$a_id}->{site_name}, $a_id\n";
      print "Number of rows from $agen_abbrev processed: $rows_processed\n";
    }
  }
}

sub build_url ($$$$) {
  my $site_code  = $_[0];
  my $dev_code   = $_[1];
  my $begin_date = $_[2];
  my $end_date   = $_[3];

  # this url is generated from the EBID website, when the CSV button is clicked
  # Exact URL is started with data stored in description field in hdb_ext_data_source table, but
  # it looks something like:
  # http://ebid.onerain.com/export/csv.php?site_id=757&site=6d49bf58-adfe-493d-a7c6-cdb7a5d53a18&device_id=5&device=c967a5c8-fd33-477e-aaff-aa09b0cec6cf&mode=&hours=&data_start=2016-06-25%2013:50:56&data_end=2016-07-25%2013:50:56&tz=America%2FDenver&format_datetime=%25Y-%25m-%25d+%25H%3A%25i%3A%25S&mime=txt
  # We remove the site and device UUIDs, since they don't seem to be necessary, and the URL
  # stored in the database is:
  # http://ebid.onerain.com/export/csv.php?tz=America/Denver&format_datetime=%Y-%m-%d+%H:%i:%S&mime=txt
  # parts of the url:
  # program generating the result (from HDB)	: http://ebid.onerain.com/export/csv.php
  # specify site				: site_id=757
  # specify device (datatype)			: device_id=5
  # specify report format, txt			: mime=txt
  # specify start and end dates: data_start=YYYY-MM-DD+HH:MI:SS&data_end=YYYY-MM-DD+HH:MI:SS
  # retrieval from database included site ids and datatypes 
  # need to work out if URL needs to be html entity encoded to handle spaces in timestamps, or can use ISO 8601. No.
  # The timezone parameter in the URL is a lie, no matter what you put in, you'll get America/Denver out.

  # https://onerain.ebid-nm.org/export/csv.php?tz=America%252FDenver&format_datetime=%25Y-%25m-%25d+%25H:%25i:%25S&mime=txt&data_start=2017-05-20%252000:00:00&data_end=2017-05-21%252023:59:59&site_id=757&device_id=5
  
  my $parameters = "&data_start=$begin_date&data_end=$end_date";

  $parameters .= "&site_id=$site_code";
  $parameters .= "&device_id=$dev_code";
  
  # url described by database, retrieved in get_app_ids()

  return $url . $parameters;
}

sub build_site_num_list {
  my $_sites = $_[0];
  my ( $_no, @site_num_list );

  #build list of sites to grab data for
  foreach $_no ( keys %$_sites ) {
    push @site_num_list, $_no;
  }

  return @site_num_list;
}

sub build_web_request {
  my $ua = WWW::Mechanize->new(autocheck => 0);
  $ua->agent(
         "$agen_abbrev Streamflow -> US Bureau of Reclamation HDB dataloader: "
           . $ua->agent );
  $ua->from("$ENV{HDB_XFER_EMAIL}");
  $ua->quiet(1);
  return ( $ua );
}

# we use the generic mapping to get the site and datacode ids
sub get_sites {
  my $id_limit_clause = '';

  # join arguments with "','" to make quoted string comma delimited list
  if (@_) {
    my $commalist = join( '\',\'', @_ );
    $id_limit_clause = "b.primary_site_code in ( '$commalist' ) and";
  }

  #retrieve ids. 
  my $get_id_statement =
    "select b.primary_site_code a_id, b.primary_data_code dev_code,
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
order by a_id";

  $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

  my $hashref;

  print $get_id_statement. "\n" if defined($debug);

  eval {
    $hashref = $hdb->dbh->selectall_hashref( $get_id_statement, 1 );
  };

  if ($@) {    # something screwed up
    print $hdb->dbh->errstr, ": $@\n";
    die "Errors occurred during automatic selection of $agen_abbrev gage ids.\n";
  }

  return $hashref;
}

sub insert_values {
  my @data       = @{ $_[0] };
  my $_site = $_[1];
  my $cur_sdi    = $_site->{sdi};

  #fixed the url encoding to only retrieve one
  # parameter, so no longer need this, but could need it again
  my $column = 1;

  my $numrows    = 0;
  my $first_date = undef;
  my ( $value, $value_date, $tz, $updated_date );
  my ( $row);
  my ( $old_val, $old_source, $data_flags );

  if (    !defined( $_site->{interval} )
       or !defined($agen_id)
       or !defined($load_app_id)
       or !defined($collect_id)
       or !defined( $_site->{meth_id} )
       or !defined( $_site->{comp_id} ) )
  {
    $hdb->hdbdie(
"Unable to write data to database for $_site->{site_name}
Required information missing in insert_values()!\n"
    );
  }

  # SQL statements

  my $modify_data_statement = "
    BEGIN
        modify_r_base(?,'$_site->{interval}',/*sdi, interval, */
                      ?,null,?,  /*start_date_time,null end_date_time, value*/
                      $agen_id,null,'$validation', /*agen, overwrite, validation*/
                      $collect_id,$load_app_id,
                      $_site->{meth_id},$_site->{comp_id},
                      'Y',?,                  /*do update?, data flags */
                      hdb_utilities.is_date_in_dst(?,'MDT','MST'));  /* timezone info. TODO: rethink is_date_in_dst for the November case and this data*/
    END;";

  eval {
    my ($modsth);
    $modsth = $hdb->dbh->prepare($modify_data_statement);

    # insert or update or do nothing for each datapoint;
    foreach $row (@data) {
      chomp $row;
      $numrows++;

#split CSV, want the first, third, fifth fields for date, value, and data flags
      ($value_date,$value,$data_flags) = (split /,/,$row) [0,2,4];

# update or insert, source and database differ (or database value does not exist)
       if ( defined($debug) ) {
         print "modifying for $cur_sdi, date $value_date, value $value\n";
       }

       # check if value is known
       if ( !defined $value or $value eq '' ) {
         print "data missing: $cur_sdi, date $value_date\n" if defined($debug);
         next;
       } elsif ( $value =~ m/[^0-9\.]/ ) {    # check for other text, complain
         print "data corrupted: $cur_sdi, date $value_date: $value\n";
         $_site->{error_code} = $value;
       } elsif ( $value_date =~ m/[^-0-9\/: ]/ ) {    # check for bad chars in date, complain
         print "date corrupted: $cur_sdi, date $value_date: $value\n";
         $_site->{error_code} = "Bad date";
       } else {       #modify
	       #round date minutes to nearest 15, per https://hdbsupport.precisionwre.com/helpdesk/tickets/382
         $value_date = round_15($value_date);
         if ( defined($debug) ) {
           print "rounded date: $value_date\n";
         }
         $modsth->bind_param( 1, $cur_sdi );
         $modsth->bind_param( 2, $value_date );
         $modsth->bind_param( 3, $value );
         $modsth->bind_param( 4, $data_flags );
         $modsth->bind_param( 5, $value_date );#Repeated date for dst computation
	
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
Example: $progname -u app_user -p <hdbpassword> -i 189

  -h               : This help
  -v               : Version
  -u <hdbusername> : HDB application user name (REQUIRED)
  -p <hdbpassword> : HDB password (REQUIRED)
  -i <id>          : $agen_abbrev gage id to look for. Must be in HDB. More than one
                     may be specified with -i id1,id2 or -i id1 -i id2
                     If none specified, will load all gages set up in HDB.
  -l <ABBREV>      : Agency abbreviation, used to determine source of data
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
  my $verstring = '$Revision$';
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
  my $a_id     = shift;
  my $dev_code = shift; 
  my $data     = shift;

  my $url = build_url( $a_id, $dev_code, $begindatestr, $enddatestr );

  if ( defined($printurl) or defined($debug) ) {
    print "$url\n";
  }

  my ( $ua ) = build_web_request();

  # this next function actually gets the data, twice. The first time gets the right cookies, and
  # gets redirected to the home page. The second time actually gets the data we want. There's got to be a better way?
  my $response = $ua->get($url);
  $response = $ua->get($url);

  print Dumper($response) if $debug;

  # check result
  if ( $response->is_success ) {
    my $status = $response->status_line;
    print "Data read from $agen_abbrev: $status\n";

    if ( !$response->content ) {
      $hdb->hdbdie("no data returned from $agen_abbrev, exiting.\n");
    }

    $response->decode;#handle uncompress if needed
    $$data = $response->content;

    print "data: ".$$data if defined($debug);

  } else {    # ($response->is_error)
    printf STDERR $response->error_as_HTML;
    print ("Error reading from $agen_abbrev web page, skipping site $a_id\n");
    return 0;
  }
  return 1;
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

  my $begindatestr = sprintf( "%4d-%02d-%02d%%2000:00:00", @$begindate );
  my $enddatestr   = sprintf( "%4d-%02d-%02d%%2023:59:59", @$enddate );

  return ( $begindatestr, $enddatestr );
}

sub round_15($) {
    my $value_date = shift;
    my ($date,$time) = split(/ /,$value_date);
    my ($year,$month,$day) = split(/-/,$date);
    my ($hour,$minute,$sec) = split(/:/,$time);

    #add 7.5 minutes, then truncate to nearest 15
    my @newdate = Add_Delta_DHMS($year,$month,$day,$hour,$minute,$sec, 0,0,7,30);
    $newdate[4]=15*int($newdate[4]/15);

    return sprintf( "%4d-%02d-%02d %02d:%02d:00",@newdate[0..4]); #zero out seconds    
}
   

