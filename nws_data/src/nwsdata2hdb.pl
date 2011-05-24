#!/usr/local/bin/perl -w
# the following line was changed by M. Bogner for the AThena to ISIS move
#!/usr/bin/perl -w
##!/usr/local/bin/perl -w

#insert HDB library

# the following line was changed by M. Bogner March 2011 for the AThena to ISIS move
# uses different PERL Libraries for ISIS compatibility
use lib "$ENV{PERL_ENV}/lib";
# Don't need for ISIS
#use lib "$ENV{HDB_ENV}/perlLib/lib/i386-linux";

use Hdb;

use Date::Calc qw(:all);
use DBI;
#use DBD::Oracle;

use strict;

my ($hdb, @cur_sdi, @value_date, @data_date);
    
#sites and datatypes desired. Not all site_ids and site_datatype_ids
# have been created, yet.

# site names (shef codes) and site id's for Cimmaron (Cimmaron near silver 
# jack res or some such) and Ouray Hwy
my %siteid_hash = ("SJAC2", 23, "OMSC2", 57); 

#we don't get total wy precip or ave temp from nws, so this list is shorter
my %datatype_hash = ("max temp", 9, "min temp", 11,
		  "precip total", 5);

my %site_datatype_hash = ("max temp", undef, "min temp", undef,
		       "precip total", undef );

#check to see command line usage.
if (scalar(@ARGV) <3)
  {
    my $progname = `basename $0`;
    chop $progname;
    print "$progname -- program to put gunnison climate data from NWS into HDB.\n";
    print "Usage: $progname <filename> <hdbusername> <hdbpassword>\n";
    exit (1);

  }

#HDB database interface
# global because used in several sub routines
$hdb = Hdb->new;

my $dbname;
if (!defined($dbname = $ENV{HDB_LOCAL})) {
  die "Environment variable HDB_LOCAL not set...\n";
}
#create connection to database
$hdb->connect_to_db($dbname, $ARGV[1], $ARGV[2]);
$hdb->set_approle();

#retrieve application ids for later updates and inserts
get_app_ids();

# various SQL statement strings, with placeholders for variables.

open (INFILE, "$ARGV[0]") || die "Error: couldn't open input file $ARGV[0]";

# No junk before data in this file format!

# get date of data is interesting, because file contains no standalone date.
# just have to use date of file creation. 

my @fileinfo = stat INFILE;

my $filemdate = localtime($fileinfo[9]);
print "File created locally at $filemdate\n";

my $cur_site_name = "none";
my ($line, @fields, $new_site_name, $site_id, $site_datatype_id);

READ: while ($line = <INFILE>)
{
  chomp($line);
  #end data if line is really short, or contains 78 dashes ('-')
  last READ if (length($line) < 5) ;

  @fields = split (/\|/,$line);
  #check to see if the site changed
  $new_site_name = $fields[0];

  if ($new_site_name ne $cur_site_name)
    { # if so, go get new site datatype ids
      $cur_site_name = $new_site_name;
      $site_id = $siteid_hash{$cur_site_name};
      get_SDI($site_id) if $site_id;
    }

  #File has one month per line, datatype is also indicated by a shef code.

  #get the data year and month

  my $year = $fields[8];
  my $month = $fields[9];
  $year =~ s/\s//g;
  $month =~ s/\s//g;

# here we use a one character piece of the shef data code to tell 
# what value is on this line
  if ($fields[6] eq 'X') {
    $site_datatype_id = $site_datatype_hash{"max temp"};
  }
  elsif ($fields[6] eq 'N') {
    $site_datatype_id = $site_datatype_hash{"min temp"};
  }
  elsif ($fields[6] eq 'Z') {
    $site_datatype_id = $site_datatype_hash{"precip total"};
  }

#with one line per month, just loop through the days in this month
  my $daysinmonth = Days_in_Month($year,$month);
  my $day;

  for ($day = 1; $day < $daysinmonth; $day++) {
    @value_date = ($year,$month,$day);
			   
    # fix the fact that the data is reported for the previous day!
    # for the NWS data, the data is for the time period 1200 ZULU the
    # previous day to 1200 ZULU the date of the data, this is approximately
    # 4 am the date of the data MDT, so we place the data for the period of the
    # previous day.
    @value_date = Add_Delta_Days(@value_date, -1);

    my $value = $fields[$day+9];

    if ($value == -9999 ) {
      $value = undef;
    }
    else {
      $value = sprintf("%.1f",$value);
    }  
    
#one of these calls per value, which is inefficent, but quick to code. 
    if ($site_datatype_id) {
      insert_values(@value_date, $site_datatype_id, $value);
    }
  }
}

$hdb->dbh->disconnect();

#End main program, subroutines below

sub check_value
  {
    my $check_data_statement = "select value from r_base where start_date_time = ? and site_datatype_id = ? and interval = 'day'";

    my($check_date)=$_[0];
    my($site_datatype_id)=$_[1];
    my($sth);
    my($myval);

    $sth = $hdb->dbh->prepare($check_data_statement) || die $sth->errstr;
    $sth->bind_param(1,$check_date);
    $sth->bind_param(2,$site_datatype_id);
    $sth->execute || die $sth->errstr;
    $sth->bind_col(1,\$myval);
    $sth->fetch;
#    DBI::dump_results($sth)|| die $DBI::errstr;;
    $sth->finish();
#    print $myval;
    return $myval;
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

  $nameid->{agen}->{name} = 'National Weather Service';
  $nameid->{collect}->{name} = '(see agency)';
  $nameid->{load_app}->{name} = 'nwstemps2hdb.pl';
  $nameid->{method}->{name} = 'unknown';
  $nameid->{computation}->{name} = 'unknown';

  $hdb->get_app_ids($nameid);

  $agen_id = $nameid->{agen}->{id};
  $collect_id = $nameid->{collect}->{id};
  $load_app_id = $nameid->{load_app}->{id};
  $method_id = $nameid->{method}->{id};
  $computation_id = $nameid->{computation}->{id};
}

sub insert_values
  {
    my(@value_date) = @_[0,1,2];
    my($sdi) = $_[3];    
    my($value) = $_[4];

    my $end_date_time = undef;
    my($modsth, $sth);
#    print @_;

    my $modify_data_statement = "
    BEGIN
        modify_r_base_raw(?,'day',?,?,?, /* sdi, interval, start_date_time, end_date_time (in/out, not used), value */
                          $agen_id,null,'Z', /* overwrite, validation */
                          $collect_id,$load_app_id,$method_id,$computation_id,
                          'Y');                 /*do update? */
    END;";

    my $datestr = sprintf("%02d-%.3s-%4d",$value_date[2],
			   Month_to_Text($value_date[1]), $value_date[0]); 

    eval {
#only use these once per prepare/finish, could do better
      $modsth = $hdb->dbh->prepare($modify_data_statement) || die $modsth->errstr;
      
# insert data;
      my  $old_val = undef;
      $old_val = check_value($datestr, $sdi);
      if (!defined $value) {
	#delete data if data is -99.9, otherwise do nothing.
      }
      elsif (defined($old_val) and $old_val == $value) {
      }
      elsif (!defined($old_val) or $old_val != $value) {
	if (!defined($old_val)) {
	  print "modifying for $sdi, date $datestr, value $value, old_val = undef\n";
	} else {
	  print "modifying for $sdi, date $datestr, value $value, old_val = $old_val\n";
	}
	$modsth->bind_param(1,$sdi);
	$modsth->bind_param(2,$datestr);
	$modsth->bind_param_inout(3,\$end_date_time, 50);
	$modsth->bind_param(4,$value);
	$modsth->execute|| die $modsth->errstr;
      }
      $modsth->finish;
    };
    if ($@) {
      $hdb->dbh->rollback;
      print "$DBI::errstr, $@\n";
      die "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.";
    }
    else {
      $hdb->dbh->commit;
    }
}


sub get_SDI
{
  my $cur_site= $_[0];
  my $datatype;

  foreach $datatype (keys %datatype_hash)
  {
    $site_datatype_hash{$datatype} = $hdb->get_SDI($cur_site,$datatype_hash{$datatype});
  }
}
