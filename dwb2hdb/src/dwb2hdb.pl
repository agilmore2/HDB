#!/usr/bin/perl

use strict;
use warnings;

#use libraries from HDB environment (HDB.pm here)
# the following line was changed by M. Bogner for the AThena to ISIS move
# below line changed by M. Bogner March 2011 to use 64 Bit Perl Libraries
use lib ( defined $ENV{PERL_ENV} ? "$ENV{PERL_ENV}/lib" : "$ENV{HDB_ENV}/perlLib/lib" );

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

#Used in usage and hdb_loading_application lookup.
my $progname = basename($0);
chomp $progname;

#globals for command line options
my $insertflag = 1;
my $overwrite  = 'null';
my ( $accountfile, $debug, $readfile );
my ( $hdbuser, $hdbpass );

#global variables read from database in get_app_ids
my ( $load_app_id, $agen_id, $validation,
     $url, $collect_id, $official_collect_id );

my $agen_abbrev = "DWB";

#flowtype defines the title of the datasource, and we retrieve all other
# data corresponding to that hdb_ext_data_source row from the database

my $title = 'DWB Daily Data (8:00 am readings!)';

#======================================================================
#parse arguments
process_args(@ARGV); # uses globals, ick.

=head2 FORMAT

    Currently, Denver Water is delivering data to us in two ways. One, a PDF emailed daily. 
    Two, a spreadsheet/cvs file combination that is not updated as often available via FTP.

    We will build the loader to handle either, but for now only handles PDF output.
    There are only 6-8 values per day, so
    will not be as complex as usgs or codwr loaders.

    We use the poppler-utils command pdftotext to extract HTML from the PDF file.

    The output from the PDF converter looks like:
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"><HTML>
<HEAD>
<TITLE></TITLE>
</HEAD>
<BODY>
<A name=1></a><IMG src="Oct0213-1_1.png"><br>
<b>Day:&nbsp;&nbsp;Wednesday</b><br>
<b>Date:&nbsp;&nbsp;10/2/13</b><br>
<b>Denver Water</b><br>
<b>Raw&nbsp;Water Supply Daily Report</b><br>
    ...

Except when their symbol does NOT appear in the document, when it looks like:

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"><HTML>
<HEAD>
<TITLE></TITLE>
</HEAD>
<BODY>
<A name=1></a><b>Day:&nbsp;&nbsp;Monday</b><br>
<b>Date:&nbsp;&nbsp;11/4/13</b><br>
<b>Denver Water</b><br>
<b>Raw&nbsp;Water Supply Daily Report</b><br>

So, we need to find the Date: line in the first few rows.

    The data part we're interested in is:
lines 53+
Roberts Tunnel<br>
374<br>

etc. 
This looks to be fragile. I'll have to look at ways to improve.

    The cvs file looks like
    Dillon Reservoir,,,,,,,,,,,,,,,,,,,
    wsJun12.xls,,,,,,,,,,,,,,,,,,,
    12,,,,,,,,,,,,,,,,,,,
    May-12,"Elevation,ft","Storage,af","Storage change,af",Thru Power Plant,Thru By-Pass,Total to Blue,to Roberts Tunnel:,Total Outflow,"precip,in","new snow,in","total snow,in",hi temp,low temp,Computed Inflow,Gross Evap,Precip,Net Evap,Precip Credit,Req'd Evap Release
1,8013.6,246499,62,55,0,55,275,330,0,0,0,68,31,361.1044191,15.92,0,15.92,0,0
...

    Of course, there are two problems here. Dillon is at elevation 9013, not 8013.
    Second, this data is for June, not May. So still fragile. Grr.

    =head2 Design Notes

    Will map "Roberts Tunnel" "Williams Fork" and "Dillon"
    And depend on html ordering to get the data codes.'

=cut

#HDB database interface
# global because used in several sub routines
my $hdb = Hdb->new;

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

#Set date format, where RR is the two digit year in the current century
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/RR'")
  or $hdb->hdbdie("Unable to set NLS_DATE_FORMAT to format Denver Water\n");

get_app_ids();

my (@data);

#if we are just reading in data from a previously downloaded file
if ( defined($readfile) ) {
  read_from_file (\@data); 
} else {    # attempt to get data from web page?
  read_from_web (\@data); 
}

# $sites is the return from the external mapping query from HDB,
# and is the critical data structure.
# $sites is a reference to a hash of site names to a hash of row numbers.
# The row numbers hash to the sdis, validation codes, etc that are used to 
# do the actual insert.
my $sites = get_dwb_sites();

#find all sitenames
my %siterows = map { $_, 1} keys %$sites;

#%siterows is a hash of names to roughly column counts in the PDF file,
# but specifically row counts in the output html from pdftohtml.
#This is now per site, rather than over the entire pdf for all sites.

# de goopify data, change all &nbsp; to actual spaces
map s/&nbsp;/ /g , @data;

#get date from data, only one day in each file
# search 7th through 9th rows
my $i = 6;
my $date=$data[$i]; #check if 7th row has date in it
while (!($date =~ /Date:/)) {
  $date = $data[$i];  # if not, use row 8, or 9, or.....
  $i++;
  last if $i > 30; # give up if looked at 30th row. Error check happens below.
}

chomp $date;
$date =~ s/^<b>Date:  ?//;
$date =~ s|</b><br>||; #changed delimiter character since matching </b>
#should now be in the form 7/3/12
print $date."\n" if defined $debug;
#error check
if (!($date =~ /^\d{1,2}\/\d{1,2}\/\d{2}(\d\d)?/)) {
  $hdb->hdbdie("Failed to find date in inputfile. Date string is $date.\n");
}

while ( defined( $data[0] ) ) 
{
    #find site in file, used as key into siterows for counting rows
    #used to check that the row defined is desired data into %$sites hashref
    #skips data until a site with requested data is found.
    my $site = read_header (\@data); 

    while (defined ($data[0]) and 
	   $data[0] =~ /^[-\d.,T]/) {  
#allow negatives, leading periods and "T"race precip
	my $row = $data[0];
	
	print $row."\n" if defined $debug;

# cut newline and '<br>' from end of value;
	my $value = substr $row, 0, -4;

	# put data into database, unless testflag is set
	#check that name from site matches database mapping
	if ( defined ($sites->{$site}) and
             defined ($sites->{$site}->{$siterows{$site}}->{sdi}) ) {
	    insert_value ( $sites->{$site}->{$siterows{$site}}, $value);
	}

	$siterows{$site}++; #count rows seen for this site.
	shift @data; #remove processed line from @data
    }
}

# print error messages for all sites that no or bad data was returned 

exit 0;

#End main program, subroutines below

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
 where e.ext_data_source_name = '$title'";

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
		     "Errors occurred during selection of data source info for $title.\n"
		     );
    }

    if ( !defined($validation) ) {
	$validation = 'null';
    }

}



sub build_web_request {
    my $ua = LWP::UserAgent->new;
    $ua->agent( "$agen_abbrev Streamflow -> US Bureau of Reclamation HDB dataloader: "
		. $ua->agent );
    $ua->from('$ENV{HDB_XFER_EMAIL}');
    $ua->timeout(600);
    my $request = HTTP::Request->new();
    $request->method('GET');
    return ( $ua, $request );
}

# returns a hashref of sites, but the hash key is the row number. Need to figure out the key to get to all
# sitenames. 
sub get_dwb_sites {

    # the mapping list has already been loaded with just the site that
    # are appropriate for this installation
    my $get_dwb_sites_statement =
	"select b.primary_site_code dwb_id, b.primary_data_code data_code,
 b.hdb_interval_name interval, b.hdb_method_id meth_id,
 b.hdb_computation_id comp_id, b.hdb_site_datatype_id sdi, b.cmmnt cmmnt,
 d.site_id, d.site_name
from hdb_site_datatype a, ref_ext_site_data_map b,
hdb_site d, hdb_ext_data_source e
where a.site_datatype_id = b.hdb_site_datatype_id and
b.is_active_y_n = 'Y' and
a.site_id = d.site_id and
b.ext_data_source_id = e.ext_data_source_id and
e.ext_data_source_name = '$title'
order by dwb_id, data_code";

    $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

    my $hashref;

    #this DBI function returns a hash indexed by column 1 (one-based), which is
    # the site row count (the first column returned by the query above)
    # if no data returned, checked later
    eval { $hashref = $hdb->dbh->selectall_hashref( $get_dwb_sites_statement, [1,2] ); };

    if ($@) {    # something screwed up
	print $hdb->dbh->errstr, " $@\n";
	$hdb->hdbdie(
		     "Errors occurred during automatic selection of $agen_abbrev gage ids.\n");
    }

    return $hashref;
}


sub insert_value {
    my $site      = $_[0];
    my $value     = $_[1];

    my $coll_id    = $collect_id;

    # SQL statements

    # this statement takes arguments of
    # start_date_time, end_date_time, value, validation, collection system id
    # the rest of the arguments are predetermined by command line arguments and
    # the generic mapping for this site
    # existance of sdi already checked


    if (    !defined( $site->{interval})
	    or !defined($agen_id)
	    or !defined($overwrite)
	    or !defined($load_app_id)
	    or !defined($site->{meth_id})
	    or !defined($site->{comp_id})
	    or !defined($validation)
	    or !defined($coll_id) )
    {
	$hdb->hdbdie(
		     "Unable to write data to database for $site->{dwb_id}\nRequired information missing in insert_values()!\n"
		     );
    }

    my $modify_data_statement = "
  BEGIN


    modify_r_base($site->{sdi},'$site->{interval}',
                      ?,null,/* start_date_time, end_date_time */
                      ?,/* value */
                      $agen_id,$overwrite,
                      '$validation', $coll_id, 
                      $load_app_id,
                      $site->{meth_id},$site->{comp_id},
                      'Y'); /*do update?*/
                      
  END;";

    eval {
	my ($modsth);
	$modsth = $hdb->dbh->prepare($modify_data_statement);

	if ($value) {    # get rid of ',' in display
	    $value =~ s/,//g;
	}
	if ($value eq 'T') {
	    $value = 0;
	}

    #need all three of these here. Also checking on value next
	if (    !defined($date) or
		!defined($coll_id) )
	{
	    $hdb->hdbdie(
			 "Undefined date, collection id in insert_values()!\n");
	}

    # check if value is known, if not, move to next row
	if ( !defined($value) or $value eq '' ) {
	    print "data missing: $site->{sdi}, date $date\n"
		    if defined($debug);
	} elsif ( $value =~ m/[^-.\d]/ ) {    # check for other text, complain
	    print
		"value field not recognized: $site->{sdi}, date $date: $value\n"
		if defined($debug);
	} else {
    # update or insert
	    if ( defined($debug) ) {
		print
		    "modifying for $site->{sdi}, $site->{cmmnt}, date $date, value $value\n";
	    }
	    $modsth->bind_param( 1, $date );
	    $modsth->bind_param( 2, $value );
	    $modsth->execute;
	}
	$modsth->finish;

    };    # semicolon here because of use of eval

    if ($@) {    # something screwed up in insert/update
	print $hdb->dbh->errstr, " $@\n";
	$hdb->hdbdie(
		     "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.\n"
		     );
    } else {
	# commit the data
	if ($insertflag) {
	    $hdb->dbh->commit or $hdb->hdbdie( $hdb->dbh->errstr );
	} else
	{
	    $hdb->dbh->rollback;
	}
    }
    return;
}

sub usage {
    print STDERR <<"ENDHELP";
    $progname $verstring [ -h | -v ] | [ options ]
	Retrieves $agen_abbrev flow data and inserts into HDB
Example: $progname -a <accountfile> -f <localfile>

-h               : This help
-v               : Version
-a <accountfile> : HDB account access file (REQUIRED or both below)
-u <hdbusername> : HDB application user name (account file or REQUIRED)
-p <hdbpassword> : HDB password (account file or REQUIRED)
-t               : Test retrieval, but do not insert data into DB
-f <filename>    : Filename instead of live web
-o               : Use overwrite flag, otherwise null
-d               : Debugging output
ENDHELP

exit(1);
}

sub version {
    print "$progname version $verstring\n";
    exit 1;
}

sub process_args {    
#scalar and array versions, the scalars are a string of the arrays
    
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
	    if ( !-r $readfile ) {
		print "file not found or unreadable: $readfile\n";
		exit 1;
	    }
	} elsif ( $arg =~ /-a/ ) {    # get database login file
	    $accountfile = shift;
	    if ( !-r $accountfile ) {
		print "file not found or unreadable: $accountfile\n";
		exit 1;
	    }
	} elsif ( $arg =~ /-d/ ) {    # get debug flag
	    $debug = 1;
	} elsif ( $arg =~ /-o/ ) {    # get overwrite flag
	    $overwrite = "'O'";
	} elsif ( $arg =~ /-u/ ) {    # get hdb user
	    $hdbuser = shift;
	} elsif ( $arg =~ /-p/ ) {    # get hdb passwd
	    $hdbpass = shift;
	} elsif ( $arg =~ /-.*/ ) {    # Unrecognized flag, print help.
	    print STDERR "Unrecognized flag: $arg\n";
	    &usage;
	} else {
	    print STDERR "Unrecognized command line argument: $arg\n";
	    &usage;
	}
    }

    if (!defined($accountfile) and (!defined($hdbuser) || !defined($hdbpass))) {
	print STDERR "ERROR: Required: accountfile (-a) or HDB username and password(-u -p)\n";
	usage();
    }
    
    return;
}

sub read_from_file {
    my $data     = shift;

    open INFILE, "$readfile" or $hdb->hdbdie("Cannot open $readfile: $!\n");
    @$data = <INFILE>;
#    print @$data         if defined($debug);
#    print Dumper(@$data) if defined($debug);
    chomp @$data;
    return ();
}


# sub read_from_web {
#     my $data          = shift;

#     my $url = build_url( \@site_num_list, $usgs_codes );
#     if ( defined($printurl) or defined($debug) ) {
# 	print "$url\n";
#     }

#     my ( $ua, $request ) = build_web_request();
#     $request->uri($url);

#     # this next function actually gets the data
#     my $response = $ua->request($request);

#     # check result
#     if ( $response->is_success ) {
# 	my $status = $response->status_line;
# 	print "Data read from $agen_abbrev: $status\n";

# 	# if compression requested, expand the response content
# 	# not by default since USGS asked not to for CPU power limits
# 	if ($compression) {
# 	    my ($inflated) = Compress::Zlib::memGunzip( $response->content );
# 	    if ( !$inflated ) {
# 		$hdb->hdbdie("no data returned from $agen_abbrev, exiting.\n");
# 	    }
# 	    print $inflated if defined($debug);
# 	    @$data = split /\n/, $inflated;
# 	} else {
# 	    my $content = $response->content;
# 	    if ( !$content) {
# 		$hdb->hdbdie("no data returned from $agen_abbrev, exiting.\n");
# 	    }
# 	    print $content if defined($debug);
# 	    @$data = split /\n/, $content;
# 	}
#     } else {    # ($response->is_error)
# 	printf STDERR $response->error_as_HTML;
# 	$hdb->hdbdie("Error reading from $agen_abbrev web page, cannot continue\n");
#     }
#     return;
# }

sub read_header {

    my $data = shift;
    my $site = undef;
    my $header;
    do {
	$header = shift @$data;
	$site = substr $header, 0, -4 if defined ($header);
	$site =~ s/<b>Western Slope:<br><\/b>//; #handle messy text in front of Dillon on line 158
    }  until ( !defined $header  or  #out of data or
	       defined($siterows{$site})); #found a site that we want data for

    return $site;
}
