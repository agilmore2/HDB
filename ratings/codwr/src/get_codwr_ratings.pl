#!/usr/bin/perl

# This is a loader for CODWR ratings using their REST web service mandated on October 1, 2018.
# Written by Andrew Gilmore

#Developed under contract with the Bureau of Reclamation, 2018

#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.

use warnings;
use strict;

#use libraries from HDB environment (Solaris only except for HDB.pm and Ratings.pm)
if (-d "$ENV{HDB_ENV}/perlLib64/lib") {
    use lib "$ENV{HDB_ENV}/perlLib64/lib";
} else {
    use lib "$ENV{HDB_ENV}/perlLib/lib";
}

use Hdb;
use Ratings;

use LWP::UserAgent;
use File::Basename;
use JSON qw(from_json);

#Version Information
my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);    
chomp $progname;

my ( $accountfile, $hdbuser, $hdbpass, $debug, $sitearg, $dataarg );

#Rest URLs
my $resturl = 'http://dnrweb.state.co.us/DWR/DwrApiService/api/v2/telemetrystations/';
my $stat    = 'telemetrydecodesettings?format=json&apiKey=PQWK6ExfREAbSivycMJbLztmNPlUJWby';
my $rt      = 'telemetryratingtable?format=json&apiKey=PQWK6ExfREAbSivycMJbLztmNPlUJWby';
my $sc      = 'telemetryshiftcurve?format=json&apiKey=PQWK6ExfREAbSivycMJbLztmNPlUJWby';

#the ugly globals...
my $agen_abbrev      = 'CDWR';
my $data_source      = 'CODWR Rating Retrieval';

main();  #at end of file to allow all subroutines to be prototyped.

# Data source indicates which sites to retrieve ratings for 
sub read_cdwr_sites ($) {
    my $hdb = shift;
    $hdb->dbh->{FetchHashKeyName} = 'NAME_lc';
    
    my $sites = $hdb->dbh->selectall_hashref(
	"select distinct b.primary_site_code siteno, b.primary_data_code datacode from
hdb_ext_data_source a, ref_ext_site_data_map b
where 
a.ext_data_source_id = b.ext_data_source_id and
a.ext_data_source_name = '$data_source' and
b.is_active_y_n = 'Y' 
order by siteno",
	'siteno'
	);

    return $sites;
}

# Loop over the list of sites and get current conditons and process current shift curve
sub process_current_conditions ($$) {
    my $hdb   = shift;
    my $sites = shift;

    my ( $ua, $req ) = build_rest_request();
    
    foreach my $site (keys %$sites) {
	my ($res,$cs,$rtname,$scname) = retrieve_current_conditions( $ua, $req, $site, $sites->{$site}->{datacode});
	$sites->{$site}->{rtname} = $rtname;
	print STDERR "$agen_abbrev SITE: $site ";
	if (!defined($res)) {
	    print STDERR "no data returned\n";
	    next;
	} elsif (!defined($cs)){ #returns undef when no current shift
	    print STDERR "PROCESSING SITE WITH SHIFT CURVE\n";

	    my $sth = $hdb->dbh->prepare( # update variable/single shift setting
					  "BEGIN
      MERGE INTO cp_comp_property CCP 
      USING (SELECT  CC.COMPUTATION_ID from CP_COMPUTATION CC, CP_COMP_TS_PARM CCTP,
      HDB_SITE_DATATYPE HSD, HDB_SITE HS
      where HS.SITE_COMMON_NAME = '$site'
                  and HSD.site_id = HS.SITE_ID
                  and HSD.datatype_id = 18
                  and HSD.site_datatype_id = CCTP.site_datatype_id
                  and CCTP.ALGO_ROLE_NAME = 'dep'
                  and CCTP.INTERVAL = 'instant'
                  and CCTP.TABLE_SELECTOR = 'R_'
                  and CC.ALGORITHM_ID = 29
                  and CC.LOADING_APPLICATION_ID = 47
                  and CC.computation_id = CCTP.computation_id
      ) CV
      ON (ccp.computation_id  = CV.computation_id and 
      ccp.prop_name = 'variableShift')
      WHEN MATCHED THEN UPDATE SET CCP.PROP_VALUE = 'true' 
           WHERE CCP.PROP_VALUE = 'false'
      WHEN NOT MATCHED THEN INSERT
      (CCP.COMPUTATION_ID,CCP.PROP_NAME,CCP.PROP_VALUE)
      VALUES (CV.computation_id,'variableShift','true');
      END;");
	    $sth->execute;
	    $hdb->dbh->commit;
	    
	    my $rating_table = retrieve_shift_curve( $ua, $req, $scname);
	    if ($rating_table) {
		process_rating( $hdb, $site, $rating_table, 'Stage Shift' );
	    }
        } elsif ($cs =~ m/\d+/){ # Single shift!
	    print STDERR "CURRENT SHIFT VALUE: $cs\n";
	    my $sth = $hdb->dbh->prepare( # update single shift value
					  "BEGIN
    MERGE INTO cp_comp_property CCP 
    USING (SELECT  CC.COMPUTATION_ID from CP_COMPUTATION CC, CP_COMP_TS_PARM CCTP,
    HDB_SITE_DATATYPE HSD, HDB_SITE HS
    where HS.SITE_COMMON_NAME = '$site'
                and HSD.site_id = HS.SITE_ID
                and HSD.datatype_id = 18
                and HSD.site_datatype_id = CCTP.site_datatype_id
                and CCTP.ALGO_ROLE_NAME = 'dep'
                and CCTP.INTERVAL = 'instant'
                and CCTP.TABLE_SELECTOR = 'R_'
                and CC.ALGORITHM_ID = 29
                and CC.LOADING_APPLICATION_ID = 47
                and CC.computation_id = CCTP.computation_id
    ) CV
    ON (ccp.computation_id  = CV.computation_id and 
    ccp.prop_name = 'singleShift')
    WHEN MATCHED THEN UPDATE SET CCP.PROP_VALUE = '$cs' 
         WHERE CCP.PROP_VALUE != '$cs'
    WHEN NOT MATCHED THEN INSERT
    (CCP.COMPUTATION_ID,CCP.PROP_NAME,CCP.PROP_VALUE)
    VALUES (CV.computation_id,'singleShift','$cs');
    END;");
	    $sth->execute;
	    $hdb->dbh->commit;
	    $sth = $hdb->dbh->prepare( # update variable/single shift setting
				       "BEGIN
      MERGE INTO cp_comp_property CCP 
      USING (SELECT  CC.COMPUTATION_ID from CP_COMPUTATION CC, CP_COMP_TS_PARM CCTP,
      HDB_SITE_DATATYPE HSD, HDB_SITE HS
      where HS.SITE_COMMON_NAME = '$site'
                  and HSD.site_id = HS.SITE_ID
                  and HSD.datatype_id = 18
                  and HSD.site_datatype_id = CCTP.site_datatype_id
                  and CCTP.ALGO_ROLE_NAME = 'dep'
                  and CCTP.INTERVAL = 'instant'
                  and CCTP.TABLE_SELECTOR = 'R_'
                  and CC.ALGORITHM_ID = 29
                  and CC.LOADING_APPLICATION_ID = 47
                  and CC.computation_id = CCTP.computation_id
      ) CV
      ON (ccp.computation_id  = CV.computation_id and 
      ccp.prop_name = 'variableShift')
      WHEN MATCHED THEN UPDATE SET CCP.PROP_VALUE = 'false' 
           WHERE CCP.PROP_VALUE = 'true'
      WHEN NOT MATCHED THEN INSERT
      (CCP.COMPUTATION_ID,CCP.PROP_NAME,CCP.PROP_VALUE)
      VALUES (CV.computation_id,'variableShift','false');
      END;");
	    $sth->execute;
	    $hdb->dbh->commit;
	} else {
	    print STDERR "Unrecogized status: $cs returned, skipping!\n"
	}
    }
}
# Loop over the list of sites and get the rating table and process it.
sub process_cdwr_sites ($$) {
    my $hdb   = shift;
    my $sites = shift;

    my ( $ua, $req ) = build_rest_request();

    foreach my $site (keys %$sites) {
	print STDERR "PROCESSING SITE WITH RATING TABLES:  $site\n";
        #rtname set by get current conditions
	if (defined($sites->{$site}->{rtname})) { #don't ask for rating table when have no name, will gladly send you 50K random rows
	    my $rating_table = retrieve_rating_table( $ua, $req, $sites->{$site}->{rtname});
	    if ($rating_table) {
		process_rating( $hdb, $site, $rating_table, 'Stage Flow' );
	    }
	}
    }
}
#Retrive current conditions
sub retrieve_current_conditions ($$$$) {
    my $ua       = shift;
    my $req      = shift;
    my $site     = shift;
    my $datacode = shift;
    
    my $params   = $stat . "&abbrev=$site&parameter=$datacode";
    my $url = $resturl . $params;
    $req->uri($url);
    my $response = $ua->simple_request($req);

    if ($debug) {
	print $url."\n";
    }
    # check result
    my ($status,$cs,$rtname,$scname);
    if ( $response->is_success ) {
	$status = $response->status_line;
	print "Data read from $agen_abbrev for site $site: $status\n";
	my $content = from_json($response->content);
	my @results = @{$content->{'ResultList'}};
	
	$cs = $results[0]{'current_shift'};
	$rtname = $results[0]{'rating_table_name'};
	$scname = $results[0]{'shift_curve_name'};
    } else {    # ($response->is_error)
	printf STDERR $response->error_as_HTML;
    }
#returns undef $status if error
    return $status,$cs,$rtname,$scname;
}

#download the shift curve
sub retrieve_shift_curve ($$$) {
    my $ua       = shift;
    my $req      = shift;
    my $scname   = shift;
    
    my $params   = $sc . "&shiftcurvename=$scname";
    my $url = $resturl . $params;
    return retrieve_table($ua, $req, $url, $scname);
}

#download the rating table
sub retrieve_rating_table ($$$) {
    my $ua       = shift;
    my $req      = shift;
    my $rtname   = shift;
    
    my $params   = $rt . "&ratingtablename=$rtname";
    my $url = $resturl . $params;
    return retrieve_table($ua, $req, $url, $rtname);
}

sub retrieve_table ($$$$) {
    my $ua   = shift;
    my $req  = shift;
    my $url  = shift;
    my $name = shift;

    $req->uri($url);
    my $response = $ua->simple_request($req);

    if ($debug) {
	print $url."\n";
    }
    # check result
    my ($status,@results);
    if ( $response->is_success ) {
	$status = $response->status_line;
	print "Data read from $agen_abbrev for curve $name: $status\n";
	my $content = from_json($response->content);
	@results = @{$content->{'ResultList'}};	# $content is a hash pointer. ResultList value is an array
        return undef unless grep { /$name/ } values %{$results[0]}; #check for table name in values of first element, table name is part of json result
    } else {    # ($response->is_error)
	printf STDERR $response->error_as_HTML;
	return undef;
    }
    my @splitrat2 = map { [ $_->{X}, $_->{Y} ] } @results;
    return \@splitrat2;
}

# setup the request.
sub build_rest_request () {
    my $ua = LWP::UserAgent->new;
    $ua->agent("CODWR Rating Table Retrieval for Bureau of Reclamation HDB: ");
    $ua->from("$ENV{HDB_XFER_EMAIL}");
    $ua->timeout(600);
    my $request = HTTP::Request->new();
    $request->method('GET');
    return ( $ua, $request );
}
#check the returned rating against filesystem stored version
# and update the database and write a new file if different.
# have to compare the actual table rather than header on the
# table because it changes (at least the shifted date) every day
# FIXME: compare new rating against database stored version instead
sub process_rating ($$$$) {
    my $hdb          = shift;
    my $site         = shift;
    my $rating_table = shift;
    my $rating_type  = shift;
    
    my $rating = find_rating( $hdb, $site, $rating_type, $data_source);
    my @comp_ary = @$rating_table;
    if ($rating) {
	if (compare_rating($hdb, $rating, \@comp_ary )) {
	    #actual ratings are the same, no update required, we're done;
	    print "No change in rating for $site\n";
	    return;
	} else {
	    #clean up old rating
	    delete_rating_points($hdb,$rating);
	}
    } else {
	$rating = create_site_rating( $hdb, $site, $rating_type, $data_source);
    }
    #update database, pass in already split rating with comments
    update_rating( $hdb, $rating, $rating_table );
}

# rating table is different, so update the database
# run through the headers, saving certain lines
# skip the header and column defs
# create a new rating record if required, 
# update description if rating record already exists
# then run through table and write to database.
sub update_rating ($$$) {
    my $hdb  = shift;
    my $rating = shift;
    my $rating_table  = shift;

    foreach (@$rating_table) {
        modify_rating_point( $hdb, $rating,($_->[0],$_->[1]));
    }
}

sub process_args (@) {
    my $mode;

    #scalar and array versions, the scalars are a string of the arrays

    while (@_) {
	my $arg = shift;
	if ( $arg =~ /-h/ ) {    # print help
	    usage();
	} elsif ( $arg =~ /-v/ ) {    # print version
	    version();
	} elsif ( $arg =~ /-a/ ) {    # get database login file
	    $accountfile = shift;
	    if ( !-r $accountfile ) {
		print "file not found or unreadable: $accountfile\n";
		exit 1;
	    }
	} elsif ( $arg =~ /-i/ ) {    # get debug flag
 	    if ( defined($sitearg) ) {
		print STDERR "only one -i site allowed!\n";
		&usage;
	    }
	    $sitearg = shift;
	} elsif ( $arg =~ /-d/ ) {    # get debug flag
	    $debug = 1;
	} elsif ( $arg =~ /-l/ ) {    # get parameter code
 	    if ( defined($dataarg) ) {
		print STDERR "only one -l data code allowed!\n";
		&usage;
	    }
	    $dataarg = shift;
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

    if ( defined($sitearg)
	 and ( !defined($dataarg)) )
    {
	print STDERR
	    "ERROR: Required: datacode (-l) if -i sitecode is used\n";
	usage();
    }

    
    if ( !defined($accountfile)
	 and ( !defined($hdbuser) || !defined($hdbpass) ) )
    {
	print STDERR
	    "ERROR: Required: accountfile (-a) or HDB username and password(-u -p)\n";
	usage();
    }

    return;
}

sub usage {
    print STDERR <<"ENDHELP";
    $progname $verstring [ -h | -v ] | [ options ]
	Script to retrieve USGS ratings files from USGS website
Example: $progname -a <accountfile> 

-h               : This help
-v               : Version
-a <accountfile> : HDB account access file (REQUIRED or both below)
-u <hdbusername> : HDB application user name (account file or REQUIRED)
-p <hdbpassword> : HDB password (account file or REQUIRED)
-l <datacode>    : Parameter for site
-i <codwrid>     : Get rating for one site
-d               : Debugging output
ENDHELP

exit(1);
}

sub version {
    print "$progname version $verstring\n";
    exit 1;
}

sub main {

    #parse arguments
    my $mode = process_args(@ARGV);    # uses globals, bad!

    #HDB database interface
    my $hdb = Hdb->new;

    #connect to database
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
    my $sites;
    if ($sitearg) {
	$sites->{$sitearg}->{datacode}=$dataarg;
    } else {
	$sites = read_cdwr_sites ($hdb);
    }
    #conncet to codwr and download current conditions and shift curves
    process_current_conditions ($hdb, $sites) ;
    #connect to codwr and download rating tables
    process_cdwr_sites ($hdb, $sites) ;
}
