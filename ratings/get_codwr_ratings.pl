#!/usr/bin/perl

use warnings;
use strict;

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{HDB_ENV}/perlLib/lib";

use Hdb;
use Ratings;

use LWP::UserAgent;
use File::Basename;
use SOAP::Lite;

#Version Information
my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);    
chomp $progname;

my ( $accountfile, $hdbuser, $hdbpass, $debug, $sitearg, $dataarg );

#store decodes dir of HDB environment for use
my $decdir = "$ENV{HDB_ENV}/ratings/codwr/src";

#XML Source
my $NS =  'http://www.dwr.state.co.us/';
my $URI = 'http://www.dwr.state.co.us/SMS_WebService';
my $SERV= 'http://www.dwr.state.co.us/SMS_WebService/ColoradoWaterSMS.asmx';

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
sub get_current_conditions ($$) {
    my $hdb   = shift;
    my $sites = shift;

    my ( $soap ) = build_soap_request();
    
    foreach my $site (keys %$sites) {
	my ($cs,$rt,$sc,$wm) = retrieve_current_conditions( $soap, $site, $sites->{$site}->{datacode});
	print STDERR "$agen_abbrev SITE: $site ";
	if (!defined($cs)) {
	    print STDERR "no data returned\n";
	    next;
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

	} elsif ($cs eq 'SC'){ 
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
	    
	    my $rating_table = retrieve_shift_curve( $soap, $site, $sites->{$site}->{datacode});
	    process_rating( $hdb, $site, $rating_table, 'Stage Shift' );
	} else {
	    print STDERR "Unrecogized status: $cs returned, skipping!\n"
	}
    }
}
# Loop over the list of sites and get the rating table and process it.
sub process_cdwr_sites ($$) {
    my $hdb   = shift;
    my $sites = shift;

    my ( $soap ) = build_soap_request();

    foreach my $site (keys %$sites) {
	print STDERR "PROCESSING SITE WITH RATING TABLES:  $site\n";
	my $rating_table = retrieve_rating_table( $soap, $site, $sites->{$site}->{datacode});
	process_rating( $hdb, $site, $rating_table, 'Stage Flow' );
    }
}
#Retrive current conditions
sub retrieve_current_conditions ($$$) {
    my $soap     = shift;
    my $site     = shift;
    my $datacode = shift;
    
    my $methodSoap = SOAP::Data->name('GetSMSCurrentConditions')->attr({xmlns=>$NS});
    my @params;
    push(@params, SOAP::Data->name("Abbrev"=>$site));
    push(@params, SOAP::Data->name("Variable"=>$datacode));
    my $som = $soap->call($methodSoap=>@params);
    my @status = $som->valueof('//GetSMSCurrentConditionsResult/CurrentCondition');
    my($res);
    my($cs,$csd,$rt,$rtd,$sc,$scd,$wm);
    foreach $res (@status) {
	if (defined($res->{currentShift})) {
	    $cs = $res->{currentShift};
	} else { $cs = 'DNE'; }
	if ($res->{currentShiftEffectiveDate}){
	    $csd = "(" . $res->{currentShiftEffectiveDate} . ")";
	} else { $csd = 'DNE'; }
	if ($res->{ratingTable}) {
	    $rt = $res->{ratingTable};
	} else { $rt = 'DNE'; }
	chomp $rt;
	if ($res->{currentRatingTableEffectiveDate}) {
	    $rtd = "(" . $res->{currentRatingTableEffectiveDate} . ")";
	} else { $rtd = 'DNE'; }
	if ($res->{shiftCurve}) {
	    $sc = $res->{shiftCurve};
	} else { $sc = 'DNE'; }
	chomp $sc;
	if ($res->{currentShiftCurveEffectiveDate}) {
	    $scd = "(" . $res->{currentShiftCurveEffectiveDate} . ")";
	} else { $scd = 'DNE'; }
	if ($res->{webMessage}) {
	    $wm = $res->{webMessage};
	} else { $wm = 'DNE'; }
	return $cs,$rt,$sc,$wm;					}
				 }
#download the shift curve
sub retrieve_shift_curve ($$$) {
    my $soap     = shift;
    my $site     = shift;
    my $datacode = shift;
    
    my $methodSoap = SOAP::Data->name('GetSMSCurrentShiftCurve')->attr({xmlns=>$NS});
    my @params;
    push(@params, SOAP::Data->name("Abbrev"=>$site));
    push(@params, SOAP::Data->name("Variable"=>$datacode));
    my $som = $soap->call($methodSoap=>@params);
    my @results = $som->valueof('//GetSMSCurrentShiftCurveResult/curvePoints/ShiftCurvePoint');
    my @splitrat2 = map { [ $_->{x}, $_->{y} ]} @results;
    return \@splitrat2;
			  }
#download the rating table
sub retrieve_rating_table ($$$) {
    my $soap     = shift;
    my $site     = shift;
    my $datacode = shift;
    
    my $methodSoap = SOAP::Data->name('GetSMSCurrentRatingTable')->attr({xmlns=>$NS});
    my @params;
    push(@params, SOAP::Data->name("Abbrev"=>$site));
    push(@params, SOAP::Data->name("Variable"=>$datacode));
    my $som = $soap->call($methodSoap=>@params);
    my @results = $som->valueof('//GetSMSCurrentRatingTableResult/tablePoints/RatingTablePoint');
    my @splitrat = map { [ $_->{x}, $_->{y} ]} @results;
    return \@splitrat;
			   }
# setup the request.
sub build_soap_request () {
    my $soap = SOAP::Lite
	-> uri($NS)
	-> on_action( sub {sprintf '%s%s', @_ } )
	-> proxy($SERV);
    return ( $soap );
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
	    if ( defined($dataarg) ) {
		print STDERR "only one -i site allowed!\n";
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
-d               : Debugging output
-i <usgsno>      : Get rating for one site
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
    #conncet to codwr and download current conditons
    get_current_conditions ($hdb, $sites) ;
    #connect to codwr and download rating table and shift curves
    process_cdwr_sites ($hdb, $sites) ;
}
