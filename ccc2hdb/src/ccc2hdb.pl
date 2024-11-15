#!/usr/bin/perl

use strict;
use warnings;

#use libraries from HDB environment (HDB.pm here)
# the following line was changed by M. Bogner for the AThena to ISIS move
# below line changed by M. Bogner March 2011 to use 64 Bit Perl Libraries
use lib ( defined $ENV{PERL_ENV} ? "$ENV{PERL_ENV}/lib" : "$ENV{HDB_ENV}/perlLib/lib" );

use LWP::UserAgent;
use Date::Calc qw(Add_N_Delta_YMDHMS Moving_Window);
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

#status hash
my %siterows;

my $agen_abbrev = "CCC";

#flowtype defines the title of the datasource, and we retrieve all other
# data corresponding to that hdb_ext_data_source row from the database

my $title = 'BOR-CBTBILLINGSDATA';

#======================================================================
#parse arguments
process_args(@ARGV); # uses globals, ick.

=head1 FORMAT

    Currently, we get data from CCC SCADA via email from Hydromet.
    The data is pulled off SCADA via some unknown method and bounced to us.

    There are 7 powerplants with 2-9 values depending on units and schedules.

    The email contains a csv text file with the format:

16-Sep-14,Plant Name,U1 mwh,U1 - Pump mwh,U1-MW Inst.,U2 mwh,U2 - Pump mwh,U2-MW Inst.,U3 mwh - gen,U3 mwh - pump,U3-MW Inst.
14091600,Flatiron,21.0,,16.9,0.0,,0.0,0.0,8.0,-8.6
14091600,Mt Elbert,0.0,0.0,0.3,0.0,94.0,-122.6,,,
14091600,Green Mtn,6.2,,6.3,6.0,,6.0,,,
14091600,Marys Lake,7.0,,6.4,,,,,,
14091600,Estes,14.0,,14.8,0.0,,0.0,0.0,,0.0
14091600,Polehill,31.0,,29.3,,,,,,
14091600,Big Thompson,0.0,,0.0,,,,,,
<next hour>

Every hour at :15, a new email is sent with another hour of data appended.
The new day has only one row in it. DST effects?

=head2 Design Notes

    Mapped powerplant names and csv header column name for SDI and interval

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
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'RRMMDDHH24'")
  or $hdb->hdbdie("Unable to set NLS_DATE_FORMAT to format CCC csv\n");

get_app_ids();

my (@data);

#we are reading data from a previously downloaded file
read_from_file (\@data); 

# $sites is the return from the external mapping query from HDB,
# and is the critical data structure.
# $sites is a reference to a hash of site names to a hash of column names.
# The column names hash to the sdis, validation codes, etc that are used to 
# do the actual insert.
my $sites = get_ccc_sites();

#find site in file, used as key into siterows for counting rows
#used to check that the row defined is desired data into %$sites hashref
#skips data until a site with requested data is found.
my $colnames = read_header (\@data); 

while (defined ($data[0])) {  
#allow negatives, leading periods and "T"race precip
    my $row = $data[0];
    
#    print $row."\n" if defined $debug;

    my ($date,$site,@values) = split /,/, $row;

#skip empty rows of data or empty date defined
    if (!defined($date) or
	!defined($site) or
	($date =~ m/^\s*$/)) {
	shift @data; #remove processed line from @data
	next;
    }

    # make a destructable copy of the column names
    my @cols =@$colnames;

    #loop through column names
    while (defined $cols[0]) {
	# put data into database, if all metadata good and value present
	if (defined $values[0] and
	    defined ($sites->{$site}) and
	    defined ($sites->{$site}->{$cols[0]}->{sdi}) ) {
	    insert_value ( $date, $sites->{$site}->{$cols[0]}, $values[0]);
	}
	#move onto next set
	shift @cols;
	shift @values;
    }

    $siterows{$site}++; #count rows seen for this site.
    shift @data; #remove processed line from @data
}


# print error messages for all sites that no or bad data was returned ?
foreach my $site (keys %siterows) {
    print "$siterows{$site} row(s) found for $site.\n";
}

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
	print $hdb->dbh->errstr, "$@\n";
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

# returns a hashref of sites, but the hash key is the row number. Need to figure out the key to get to all
# sitenames. 
sub get_ccc_sites {

    # the mapping list has already been loaded with just the site that
    # are appropriate for this installation
    my $get_ccc_sites_statement =
	"select b.primary_site_code ccc_id, b.primary_data_code data_code,
 b.hdb_interval_name interval, b.hdb_method_id meth_id,
 b.hdb_computation_id comp_id, b.hdb_site_datatype_id sdi,
 d.site_id, d.site_name
from hdb_site_datatype a, ref_ext_site_data_map b,
hdb_site d, hdb_ext_data_source e
where a.site_datatype_id = b.hdb_site_datatype_id and
b.is_active_y_n = 'Y' and
a.site_id = d.site_id and
b.ext_data_source_id = e.ext_data_source_id and
e.ext_data_source_name = '$title'
order by ccc_id, data_code";

    $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

    my $hashref;

    #this DBI function returns a hash indexed by column 1 and a sub-hash
    # indexed by 2 (one-based), which is
    # the site name count (the first column returned by the query above)
    # and the column name
    # if no data returned, checked later
    eval { $hashref = $hdb->dbh->selectall_hashref( $get_ccc_sites_statement, [1,2] ); };

    if ($@) {    # something screwed up
	print $hdb->dbh->errstr, " $@\n";
	$hdb->hdbdie(
		     "Errors occurred during automatic selection of $agen_abbrev ids.\n");
    }

    return $hashref;
}


sub insert_value {
    my $date      = $_[0];
    my $site      = $_[1];
    my $value     = $_[2];

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
		     "Unable to write data to database for $site->{ccc_id}\nRequired information missing in insert_values()!\n"
		     );
    }

    #fix date for end of period v. begin of period for hourly data
    # confirmed via comparison with flowmeter data for Flatiron for
    # March 5-6, 2014
    if ($site->{interval} eq 'hour') {
	#date format is YYMMDDHH
	my ($yr,$mm,$dd,$hr) = $date =~ /(\d\d)(\d\d)(\d\d)(\d\d)/;
	# makes assumption that date passed in is within 50 years of
	# current time
	my $year = Moving_Window($yr);
	#subtract one hour to get to start_date_time
	($year,$mm,$dd,$hr) = Add_N_Delta_YMDHMS($year,$mm,$dd,$hr,0,0,0,0,0,-1,0,0);
	#get year number without century
	$yr = substr($year,2,2);
	#make the date back with zero padding
	$date=sprintf("%02s%02s%02s%02s",$yr,$mm,$dd,$hr);
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
                      'Y', null, HDB_UTILITIES.IS_DATE_IN_DST(?,'MDT','MST')); /*do update?, dataflags, timezone */
                      
  END;";

    eval {
	my ($modsth);
	$modsth = $hdb->dbh->prepare($modify_data_statement);

	if ($value) {    # get rid of ',' in display
	    $value =~ s/\s//g;
	}

    #need date and collection id here. Also checking on value next
	if (    !defined($date) or
		!defined($coll_id) )
	{
	    $hdb->hdbdie(
			 "Undefined date, collection id in insert_values()!\n");
	}

    # check if value is known, if not, move to next row
	if ( !defined($value) or $value eq '' ) {
	    print STDERR "data missing: $site->{site_name}, date $date\n"
		    if defined($debug);
	} elsif ( $value =~ m/[^-.\d]/ ) {    # check for other text, complain
	    print STDERR
		"value field not recognized: $site->{site_name}, date $date: $value\n"
		if defined($debug);
	} else {
    # update or insert
	    if ( defined($debug) ) {
		print
		    "modifying for $site->{site_name}, $site->{data_code}, date $date, value $value\n";
	    }
	    $modsth->bind_param( 1, $date );
	    $modsth->bind_param( 2, $value );
	    $modsth->bind_param( 3, $date );
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
-f <filename>    : Filename (REQUIRED)
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
    
    if (!defined($readfile)) {
	print STDERR "ERROR: Required argument missing: filename (-f)\n";
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

sub read_header {

    my $data = shift;
    my $header;
    #pick top row
    $header = shift @$data;
    #ignore date and site name header on top row,
    #split rest of header into column names
    my ($date, $plantheader, @names) = split /,/, $header if defined ($header);

    # fix unfortunate missing field separator for data 2012-nov-28 to 2012-dec-26
    if (scalar(@names) == 8) {
	unshift @names, $plantheader;
    } elsif (scalar(@names) != 9) {
	$hdb->hdbdie("ERROR! Unrecognizable header $header\n");
    }

    # check last column name for trailing /r
    $names[8] =~ s/\r//;

 #   print Dumper(@names) if defined($debug);
    return \@names;
}
