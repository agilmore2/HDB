#!/usr/bin/perl -w
#RFC code, $model_id, $ext_data_source_id, $fcname, name, $site_datatype_id_in, site_common_name, notes 
#MBRFC (4) (5) ESSC2S.ESSC2N.SQIN.06.CS  estes lake inflow (ESSC2) 100252 BTABESCO -- NOT LOADING because of errors in data transfer from MBRFC
#MBRFC (4) (5) DKKC2S.DKKC2N.SQIN.06.CS  drake river gage (DKKC2) 100441 BTNFDRCO -- NOT LOADING because of errors in data transfer from MBRFC
#CBRFC (5) (5) RURC2L_F.RURC2L_F.SQME.24.CS ruedi inflow (RURC2L_F) 100840 RUERESCO
#CBRFC (5) (5) WCRC2H_F.WCRC2H_F.SQME.24.CS willow creek inflow (WCRC2H_F) 100524 WILRESCO
#CBRFC (5) (5) GMRC2L_F.GMRC2L_F.SQME.24.CS green Mountain inflow (GMRC2L_F) 100683 GRERESCO
#CBRFC (5) (5) GCOC2L_F.GCOC2L_F.SQME.24.CS colorado river shoshone (GCOC2L_F) 100350 COLDOTCO
#CBRFC (5) (5) GBYC2H_F.GBYC2H_F.SQME.24.CS GL + SM + Granby inflow (CBYC2H_F) 100608 GRARESCO
#ABRFC (8) (5) CCAC2X.CCAC2.QINE.06.CS  Granite 2 SW, CLEAR LK ABOVE CLEAR CRK RSV (CCAC2) -- NOT LOADING reg inflow to clear crk res, not loading because site not set up hdb 
#ABRFC (8) (5) CHKC2X.CHKC2.QINE.06.CS  Nathrop 4w, CHALK CRK near nathrop (CHKC2) CHCRNACO -- NOT LOADING because site not set up on hdb
#ABRFC (8) (5) TINC2.TINC2N.QINE.06.CS Leadville 12 SW, TWIN LAKES INFLOW (model segment above Twin Lakes Res.) (TINC2) 101030 TWIRESCO -- unreg inf
#ABRFC (8) (5) TLRC2.TLRC2.QINE.06.CS Balltown 2W, Twin Lakes Reservoir, LAKE CREEK (TLRC2) 100416 LAKBTLCO -- reg outflow
#ABRFC (8) (5) HMCC2.HMCC2.QINE.06.CS  Malta 4 SW, HALFMOON CRK at Malta, USGS (HMCC2) 100407 HALMALCO
#ABRFC (8) (5) LEDC2.LEDC2N.QINE.06.CS Leadville 3NW, ARKANSAS RVR At Leadville, USGS (LEDC2) 101028 ARKLEACO
#ABRFC (8) (5) TQLC2.TQLC2.QINE.06.CS Leadville 4WNW, Sugarloaf Dam on Lake Fork (TQLC2) 100418 LFCBSLCO -- res outflow, has regulation
#ABRFC (8) (5) PBAC2.PBAC2N.QINE.06.CS Pueblo 6W,  Pueblo Reservior on the Arkansas River (PBAC2) 101029 PUERESCO -- unreg inflow
#ABRFC (8) (5) WSVC2.WSVC2N.QINE.06.CS Salida 4SE, ARKANSAS RVR near Wellsville (WSVC2) 100357 ARKWELCO
#NCWCD (6) (6) WCRC2.NCWCD.SQIN.06.CS  willow creek inflow (WCRC2) 100524 WILRESCO
#NCWCD (6) (6) BTABE.NCWCD.SQIN.06.CS  lake estes inflow   (BTABE) 100252 BTABESCO
#NCWCD (6) (6) BTNFD.NCWCD.SQIN.06.CS  north fork big thompson at drake (BTNFD) 100441 BTNFDRCO
#NCWCD (6) (6) GBYC2.NCWCD.SQIN.06.CS  inflow to lake granby (GBYC2) -- NOT LOADING
#NCWCD (6) (6) SDMCO.NCWCD.SQIN.06.CS  inflow to shadow mountain and grand lake (SDMCO) -- NOT LOADING

use strict;
use lib "$ENV{HDB_ENV}/perlLib/lib";
use NWSRFS;
use GetOpt;
use LWP::Simple;
use DBI;
use Date::Calc qw(Add_Delta_DHMS);
use Net::FTP;

#Create connection to database
my $name = 'dbi:Oracle:ecohdb';
my $user = 'ecodba';
my $pass = '<PASSWORD>';
my $dbh = DBI->connect($name,$user,$pass,{AutoCommit=>0}) || die "Cannot connect to $name\n";
$dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24'")
    or $dbh->hdbdie("Unable to set NLS_DATE_FORMAT to ISO standard\n");

#Assign constants & variables
my $pwd = "$ENV{HDB_ENV}/nws2hdb/ESP";
my $type = "ESP";
my ($ftp,@files,$model_id,$start_date_time_in,$end_date_time,$load,$model_run_date,@x,$x);
my $getlog = "$ENV{HDB_ENV}/nws2hdb/CSretrieval.out";

#delete retrieval output
system("rm -f $getlog");

#Get files from Northern's ftp site.
chdir $pwd;
my $host="ftp.ncwcd.org";
my $directory="Reclamation";
my $ftpuser="District";
my $paswd="NCWCDGeneral";
$ftp=Net::FTP->new($host, Timeout=>240);
$ftp->login("$ftpuser","$paswd");
$ftp->cwd($directory);
$ftp->binary();
@files=$ftp->dir;
foreach(@files) {
  $_=substr($_,39);  
  chomp $_; 
  print "Now downloading: $_ ESP trace file from NCWCD\n";  
  $ftp->get($_);
  }
$ftp->quit;

#Get constants from hdb and start the ball rolling
foreach my $ext_data_source_id (5..6){
    my $get_ref_ext_site_data_map = "SELECT HDB_SITE_DATATYPE_ID, CMMNT FROM REF_EXT_SITE_DATA_MAP WHERE EXT_DATA_SOURCE_ID = $ext_data_source_id";
    my $arrayref = $dbh->selectall_arrayref($get_ref_ext_site_data_map);   
    my @all_array = @$arrayref;
    my $j = @all_array; 
    foreach my $i (0..$j-1){
	my @row_array = @{$all_array[$i]};
	my $site_datatype_id_in = $row_array[0];
	my $fcname = $row_array[1];
	if ($fcname eq 'DKKC2S.DKKC2N.SQIN.06.CS' or $fcname eq 'ESSC2S.ESSC2N.SQIN.06.CS'){
	    $model_id = "4";
	}
	elsif ($fcname eq 'RURC2L_F.RURC2L_F.SQME.24.CS' or $fcname eq 'WCRC2H_F.WCRC2H_F.SQME.24.CS' or $fcname eq 'GMRC2L_F.GMRC2L_F.SQME.24.CS' or $fcname eq 'GCOC2L_F.GCOC2L_F.SQME.24.CS' or $fcname eq 'GBYC2H_F.GBYC2H_F.SQME.24.CS'){
	    $model_id = "5";
	}
	elsif ($fcname eq 'WCDC2.NCWCD.SQIN.06.CS' or $fcname eq 'BTABE.NCWCD.SQIN.06.CS' or $fcname eq 'BTNFD.NCWCD.SQIN.06.CS' or $fcname eq 'GBYC2.NCWCD.SQIN.06.CS' or $fcname eq 'SDMCO.NCWCD.SQIN.06.CS'){
	    $model_id = "6";
	}
	elsif ($fcname eq 'TINC2.TINC2N.QINE.06.CS' or $fcname eq 'TLRC2.TLRC2.QINE.06.CS' or $fcname eq 'HMCC2.HMCC2.QINE.06.CS' or $fcname eq 'LEDC2.LEDC2N.QINE.06.CS' or $fcname eq 'TQLC2.TQLC2.QINE.06.CS' or $fcname eq 'PBAC2.PBAC2N.QINE.06.CS' or $fcname eq 'WSVC2.WSVC2N.QINE.06.CS'){
	    $model_id = "8";
	}

#Get ensembles from RFC's ftp site
#if ($ext_data_source_id eq '5'){
	if ($model_id eq '4' or $model_id eq '5'){
	    system("rm -f $fcname");
	    system("wget -a $getlog http://www.cbrfc.noaa.gov/outgoing/ecao/$fcname");
	}
	elsif ($model_id eq '8'){
	    system("rm -f $fcname");
	    system("wget -a $getlog http://www.srh.noaa.gov/anonymous/abrfc/bor_esp/$fcname");
	}
	print "$fcname, $model_id, $ext_data_source_id\n";

#  Conversion factors to CFS and feet
	my %unit = (CFS=>1,CMS=>35.314667,FT=>1,M=>1/0.3048,CFSD=>1,CMSD=>35.314667);
	my %avg  = (CFS=>1,CMS=>1,FT=>0,M=>0);
	my %type = (HIGH=>'High',EXPD=>'Expected',LOW=>'Low',CRITICAL=>'Critical',ESP=>'ESP');

#  Open CS file
	my $cs = NWSRFS->open($fcname);

#  Get Model Run Date
	my @x = $cs->raw();  
	my $date = $x[16];
	$date =~ s/\.\d+$//;
	my $select = "SELECT to_char(to_date(2415021+$date,'J'),'YYYY-MM-DD HH24') from dual";
	my @row_array1 = $dbh->selectrow_array($select);
	my $model_run_date = $row_array1[0];

#  Get units and reference year
	my ($unit,undef,$refyr) = $cs->unit();

#  Set conversion factor
	exists($unit{$unit}) || die "$fcname contains unknown units $unit\n";
	my $conv = $unit{$unit};

#  Loop over all traces
	my $N = ($type eq 'ESP') ? $cs->size() : 1;
        YEAR: for (my $k=0;$k<$N;$k++)

        {
#  Write raw data to hdb.
	    my $refyear = $refyr+$k;
	    $refyear =~ s/\.\d+$//;
	    print "$model_id, $refyear ";
	    my $get_model_run_id = "SELECT MODEL_RUN_ID, RUN_DATE, TIME_STEP_DESCRIPTOR FROM REF_MODEL_RUN WHERE MODEL_ID = $model_id AND HYDROLOGIC_INDICATOR = $refyear";
	    my @row_array = $dbh->selectrow_array($get_model_run_id);   
	    if (!$row_array[0]) {
		print "Warning, no model_run_id returned for model_id $model_id, reference year $refyear\n";
		next;
	    }; #skip if no model_run_id returned.
	    my $model_run_id = $row_array[0];
	    my $db_run_date = $row_array[1];
	    my $interval = $row_array[2];

	    my %q = $cs->get00($k);
	    foreach my $time (sort keys %q){
		my $value = $conv*$q{$time};
		$end_date_time = $time;
		$end_date_time =~ s/\:\d+$//;

		if ($value <1e-10 or $value >1e10) {
		    print "ERROR, corrupt data found for: $fcname $model_run_date $value, $refyear, $model_run_id, $interval, $site_datatype_id_in, $end_date_time\n";
		    next YEAR;
		}
		$load = $dbh->prepare("BEGIN   
              MODIFY_M_TABLE_RAW(?,?,?,?,?,?,?);  
              END;");
		$load->bind_param(1,"$model_run_id");
		$load->bind_param(2,$site_datatype_id_in);
		$load->bind_param(3,$end_date_time);
		$load->bind_param(4,$end_date_time);
		$load->bind_param(5,$value);
		$load->bind_param(6,"$interval");
		$load->bind_param(7,"Y");
		$load->execute;
	    }

#Update model_run_id only once
	    if ($model_run_date ne $db_run_date) {
		print "$model_run_date, $db_run_date $model_run_id\n";
		my $load1 = $dbh->prepare("BEGIN
                 UPDATE_MODEL_RUN_ID(?,?,?,?,?,?,?,?,?,?,?,?);  
                 END;");
		$load1->bind_param(1,"$model_run_id");
		$load1->bind_param(2,"");
		$load1->bind_param(3,"$model_run_date");
		$load1->bind_param(4,"");
		$load1->bind_param(5,"");
		$load1->bind_param(6,"");
		$load1->bind_param(7,"");
		$load1->bind_param(8,"");
		$load1->bind_param(9,"");
		$load1->bind_param(10,"");
		$load1->bind_param(11,"");
		$load1->bind_param(12,"Y");
		$load1->execute;
	    }
	    $dbh->commit;
	}
    }
}
