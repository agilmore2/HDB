#!/usr/bin/perl

use strict;
use warnings;

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{HDB_ENV}/perlLib/lib";

use LWP::UserAgent;
use Date::Calc qw(Delta_DHMS Add_Delta_Days Month_to_Text Decode_Date_EU Today);
use Compress::Zlib;
use File::Basename;
use Data::Dumper;
use Spreadsheet::ParseExcel;
use IO::File;

#insert HDB library
use Hdb;

#Version Information
my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);
chomp $progname;

my $insertflag = 1;
my $overwrite  = 'null';

my ( $readfile, $accountfile, $printurl, $debug, @site_num_list );
my ( $hdbuser, $hdbpass );

my ($datestr);

#global variables read from database in get_app_ids
my ($model_run_id);

my $agen_abbrev = "WAPA";

my $title = 'LAP Preschedule';

#======================================================================
#parse arguments
process_args(@ARGV);    # uses globals, bad!

=head2 FORMAT

 The data is expected in the canonical Excel spreadsheet from WAPA scheduling.

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

#Set date format to ISO, USGS data also in this format
$hdb->dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'MONTH DD, YYYY HH24:MI'")
  or $hdb->hdbdie("Unable to set NLS_DATE_FORMAT to ISO standard\n");

get_app_ids();

my ($sites);

$sites = get_sites();

my (@data);
read_from_file( \@data );

$datestr =~ s/\"//g;    #cut out quotes

#skip rows until row[34] matches a known site.

ROW:
until ( !defined( $data[0] ) ) {

	#read row
	my $row = shift @data;
	my ($site) = read_fields($row);

	next ROW if $site eq 0;    #failed to find a site, next station!

	#make a slice of the input data for this site
	my $start = 4;
	my $end   = 27;
	my (@cur_values) = @$row[ $start .. $end ];

	# put data into database, unless testflag is set
	if ( defined($insertflag) ) {
		process_data( $site, \@cur_values );
	}
	if ( defined($debug) ) {
		print($site);
		print(@cur_values);
	}
}

# print error messages for all sites that no or bad data was returned
my @data_errors;
foreach my $site ( keys %$sites ) {
	my $site_data = $sites->{$site};
	if ( !defined( $site_data->{found_data} ) ) {
		push @data_errors,
		  "No data found for site: $site_data->{site_name}, $site\n";
	} elsif ( defined( $site_data->{error_code} ) ) {
		push @data_errors,
"Bad data seen for site: $site_data->{site_name}, $site: $site_data->{error_code}\n";
	}
}

print STDERR sort @data_errors;

exit 0;

#End main program, subroutines below

sub get_app_ids {

	# Get model id
	my $sth;

	my $get_other_info_statement = "select model_run_id
 from hdb_ext_data_source e, ref_model_run run
 where e.ext_data_source_name = '$title' and
 run.model_id = e.model_id and
 run.run_date = (select max(run_date) from ref_model_run 
 where model_id = e.model_id)";

	eval {
		$sth = $hdb->dbh->prepare($get_other_info_statement);
		$sth->execute;
		$sth->bind_col( 1, \$model_run_id );
		my $stuff = $sth->fetch;

		#		Dumper($stuff);
		unless ($stuff) {
			$hdb->hdbdie(
				  "Data source definition not found, $title, $model_run_id!\n");
		}
		$sth->finish();
	};

	if ($@) {    # something screwed up
		print $hdb->dbh->errstr, " $@\n";
		$hdb->hdbdie(
			"Errors occurred during selection of data source info for $title.\n"
		);
	}
}

sub get_sites {
	my $id_limit_clause = '';

	# join arguments with "','" to make quoted string comma delimited list
	if (@_) {
		my $commalist = join( "','", @_ );
		$id_limit_clause = "map.primary_site_code in ( '$commalist' ) and";
	}

	# outcome should be 'load_usgs_realtime' or official or provisional
	# the mapping list has already been loaded with just the site that
	# are appropriate for this installation
	# limited by any site number list passed in
	my $get_sites_statement = "select map.primary_site_code site,
 map.hdb_interval_name interval, map.hdb_method_id meth_id,
 map.hdb_computation_id comp_id, map.hdb_site_datatype_id sdi,
 sites.site_id, sites.site_name
from hdb_site_datatype sdis, ref_ext_site_data_map map,
hdb_site sites, hdb_ext_data_source source
where sdis.site_datatype_id = map.hdb_site_datatype_id and $id_limit_clause
map.is_active_y_n = 'Y' and
sdis.site_id = sites.site_id and
map.ext_data_source_id = source.ext_data_source_id and
source.ext_data_source_name = '$title'
order by site";

	$hdb->dbh->{FetchHashKeyName} = 'NAME_lc';

	my $hashref;

	#this DBI function returns a hash indexed by column 1 (one-based), which is
	# the usgs id (the first column returned by the query above)
	# if no data returned, checked later
	eval { $hashref = $hdb->dbh->selectall_hashref( $get_sites_statement, 1 ); };

	if ($@) {    # something screwed up
		print $hdb->dbh->errstr, " $@\n";
		$hdb->hdbdie(
"Errors occurred during automatic selection of $agen_abbrev gage ids.\n" );
	}

	return $hashref;
}

sub insert_values {
	my @data      = @{ $_[0] };  #first arg is a reference to an array of values
	my $site_data = $_[1];       #reference to a hash

	my $numrows = 0;
	my ( $value, $value_date, $end_date );

	# SQL statements

   # this statement takes arguments of
   # start_date_time, end_date_time, value, validation, and collection system id
   # the rest of the arguments are predetermined by command line arguments and
   # the generic mapping for this site

	if ( !defined( $site_data->{interval} ) ) {
		$hdb->hdbdie(
"Unable to write data to database for $site_data->{site_name}\nRequired information missing in insert_values()!\n"
		);
	}

	my $modify_data_statement = "
  BEGIN
    modify_m_table_raw($model_run_id, $site_data->{sdi},
                      ?,?,/* start_date_time, end_date_time (unused) */
                      ?,/* value */
                      '$site_data->{interval}',
                      'Y');/*do update? */
  END;";

	eval {
		my ($modsth);
		$modsth = $hdb->dbh->prepare($modify_data_statement);

		# loop through array of data read for single site
		# insert or update or do nothing for each datapoint (row in file);
		foreach $value (@data) {

			$numrows++;

			#value date is datestring from spreadsheet cell plus hour
			$value_date = $datestr . sprintf( " %02d:00", $numrows - 1 );

			if ($value) {    # get rid of ',' in display
				$value =~ s/,//g;
			}

			#need all three of these here. Also checking on value next
			if ( !defined($value_date) ) {
				$hdb->hdbdie("Undefined date insert_values()!\n");
			}

			# check if value is known, if not, move to next row
			if ( !defined($value) or $value eq '' ) {
				print "data missing: $site_data->{sdi}, date $value_date\n"
				  if defined($debug);
				next;
			} elsif ( $value =~ m/[^0-9\.]/ ) { # check for other text, complain
				print
"value field not recognized: $site_data->{sdi}, date $value_date: $value\n"
				  if defined($debug);
				$site_data->{error_code} = $value;
				next;
			} else {

				# update or insert
				if ( defined($debug) ) {
					print
"modifying for $site_data->{sdi}, date $value_date, value $value, update status unknown\n";
				}
				$modsth->bind_param( 1, $value_date );
				$modsth->bind_param_inout( 2, \$end_date, 30 )
				  ;    #unused, but necessary for  IN OUT oracle parameter
				$modsth->bind_param( 3, $value );
				$modsth->execute;
			}
		}
		$modsth->finish;

	};    # semicolon here because of use of eval

	if ($@) {    # something screwed up in insert/update
		print $hdb->dbh->errstr, " $@\n";
		$hdb->hdbdie(
"Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.\n"
		);
	} else {     # commit the data
		         #$hdb->dbh->rollback; #for testing
		$hdb->dbh->commit or $hdb->hdbdie( $hdb->dbh->errstr );
	}
	return $numrows;
}

sub usage {
	print STDERR <<"ENDHELP";
$progname $verstring [ -h | -v ] | [ options ]
Retrieves $agen_abbrev flow data and inserts into HDB
Example: $progname -a <accountfile> -n 2 -i 09260000 -l u

  -h               : This help
  -v               : Version
  -a <accountfile> : HDB account access file (REQUIRED or both below)
  -u <hdbusername> : HDB application user name (account file or REQUIRED)
  -p <hdbpassword> : HDB password (account file or REQUIRED)
  -f <filename>    : Filename (REQUIRED)

  -i <wapa_id>     : $agen_abbrev gage id to look for. Must be in HDB. More than one
                     may be specified with -i id1,id2 or -i id1 -i id2
  -t               : Test retrieval, but do not insert data into DB
  -d               : Debugging output
ENDHELP

	exit(1);
}

sub version {
	print "$progname version $verstring\n";
	exit 1;
}

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
		} elsif ( $arg =~ /-i/ ) {    # get id
			push @site_num_list, split /,/, shift;
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

	# if user specified ids, check that they match pattern
	if (@site_num_list) {
		if ( grep ( /[\W\d]/, @site_num_list ) ) {
			die
			  "ERROR: @site_num_list\ndoes not look like $agen_abbrev id(s).\n";
		}
	}

	if ( !defined($readfile) ) {
		print STDERR "ERROR: file to process is required (-f)\n";
	}

	#check given login info
	if ( !defined($accountfile)
		 and ( !defined($hdbuser) || !defined($hdbpass) ) )
	{
		print STDERR
"ERROR: Required: accountfile (-a) or HDB username and password(-u -p)\n";
		usage();
	}
	return;
}

sub read_from_file {
	my $data = shift;
	my $XLS  = new IO::File;

	$XLS->open("< $readfile") || $hdb->hdbdie("Cannot open spreadsheet: $!");
	my $Book = Spreadsheet::ParseExcel::Workbook->Parse($XLS)
	  || $hdb->hdbdie("Can't read spreadsheet!");

	#refuse to read spreadsheets with more than one worksheet! LAP does not

	if ( $Book->{SheetCount} > 1 ) {
		$hdb->hdbdie("Won't read spreadsheets with more than one worksheet!");
	}
	my ($Sheet) = @{ $Book->{Worksheet} };    #first worksheet

	my $DateCell = $Sheet->{Cells}[1][0];

	$datestr = readcell ($DateCell);
	for ( my $Rownum = $Sheet->{MinRow} ; defined $Sheet->{MaxRow} && $Rownum <= $Sheet->{MaxRow} ; $Rownum++ )
	{
        my @Row;
        for ( my $Col = $Sheet->{MinCol} ; defined $Sheet->{MaxCol} && $Col <= $
Sheet->{MaxCol} ; $Col++ )
        {
                my $Cell = $Sheet->{Cells}[$Rownum][$Col];
                
                my $Value = readcell ($Cell);
	                # We assume the line is blank if there is nothing in the first column.
                last if $Col == $Sheet->{MinCol} and !$Value;
                
                push(@Row, $Value);
        }
        
        next unless @Row;
		push(@$data, \@Row);
	}
	
	return ();
}

sub read_fields {

	my $row = shift;
	if ( !defined($row) ) {
		return 0;
	}

	# column number for site abbreviation
	my $SITEPOS = 34;

	#check to see if no data at all, blank line
	if ( !defined( $row->[$SITEPOS] ) or $row->[$SITEPOS] eq '' ) {
		return 0;    #no data, next station please!
	}

	my $site = $row->[$SITEPOS];

	# check that site is sane, i.e. contains only alphabetic:
	if ( $site =~ /[\W\d]/ ) {
		print STDERR
"Warning! '$site' does not appear to be a valid $agen_abbrev site name!\n";

		#		$hdb->hdbdie("Assuming data retrieved is garbage and giving up!\n");
	}

	if ( !( exists( $sites->{$site} ) ) ) {
		return 0;
	}    #move along, nothing to see here

	#check on found data for this site
	if ( exists( $sites->{$site}->{found_data} ) ) {
		return 0;
	}    #skip if already found;
	$sites->{$site}->{found_data} = 1;

	if (    !defined( $sites->{$site}->{sdi} )
		 or !defined( $sites->{$site}->{site_id} ) )
	{
		$hdb->hdbdie("site or sdi not defined for id: $site!\n");
	}

	return $site;
}

sub process_data {
	my $site       = shift;
	my $cur_values = shift;

	my ( $rows_processed, $updated_date, $first_date );

	#tell user something, so they know it is working
	print "Working on $agen_abbrev site: $site\n";

	#get data needed to do insert
	my $site_data = $sites->{$site};

	#pass in possibly huge array of data for specific usgs id
	#function returns date of first value and last value updated
	($rows_processed) = insert_values( \@$cur_values, $site_data );

	#report results of insertion, and report error codes to STDERR
	if ( $rows_processed == 0 ) {
		print "No data processed for $site_data->{site_name}, $site\n";
	} else {
		print
		  "Data processed for $datestr for $site_data->{site_name}, $site\n";
		print "Number of rows from $agen_abbrev processed: $rows_processed\n";
	}
	return;
}

#Spreadsheet manipulation, etc

sub readcell {
	my $Cell    = shift;
	my $value;

	if ($Cell) {
		$value = $Cell->Value;
		if (
			$value eq 'GENERAL' ) {

			# Sometimes numbers are read incorrectly as "GENERAL".
			# In this case, the correct value should be in ->{Val}.
			$value = $Cell->{Val};
		}
	} 
    return $value;
}
