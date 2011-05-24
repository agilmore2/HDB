#!/usr/bin/perl -w
#  changed perl lib by M.bogner 3.19.2011 to run on Linux box
#!/usr/local/bin/perl -w

#insert HDB library

#  changed to use New PERL_ENV environment variable by M.bogner 3.19.2011 to run on Linux box
use lib "$ENV{PERL_ENV}/lib";
#use lib "$ENV{HDB_ENV}/perlLib/lib";

# Don't need?
#use lib "$ENV{HDB_ENV}/perlLib/lib/i386-linux";
use Hdb;

use Date::Calc qw(:all);
use DBI;
#use DBD::Oracle;

use strict;

my ($hdb, @cur_sdi, @value_date, @data_date);

#sites and datatypes desired. Not all site_ids and site_datatype_ids
# have been created, yet.

my %siteid_hash = ('PARK CONE           ', 28, "IDARADO             ", 133, 
		"BUTTE               ", 22, "SCHOFIELD PASS      ", 32,
		"PORPHYRY CREEK      ", 30, "SLUMGULLION         ", 33,
		"BRUMLEY             ", 1064, "NORTH LOST TRAIL    ", 72);

my %datatype_hash = ("max temp", 9, "ave temp", 8, "min temp", 11,
		  "precip total", 5, "total wy precip", 72);

my %site_datatype_hash = ("max temp", undef, "ave temp", undef, "min temp", undef,
		       "precip total", undef, "total wy precip", undef);

my %value_hash = ("max temp", undef, "ave temp", undef, "min temp", undef,
		       "precip total", undef, "total wy precip", undef);

#check to see command line usage.
if (scalar(@ARGV) <3)
  {
    my $progname = `basename $0`;
    chop $progname;
    print "$progname -- program to put gunnison climate data into HDB.\n";
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

#input file is opened 4 different times.
# get date on input file
open (INFILE, "$ARGV[0]") || die "Error: couldn't open input file $ARGV[0]";
# skip past opening junk
my ($i, $line, $datestr, $cur_site_name, $site_id);
for ($i = 0; $i < 6; $i++)
  { $line = <INFILE>; }

# get date of data
$line = <INFILE>;
$datestr = substr $line, 33, 8;

(@data_date = Decode_Date_US($datestr)) ||
  die "There was an error parsing the date in the datafile!.\n";

print "@data_date\n";

#skip past header
for ($i = 0; $i < 6; $i++)
  { $line = <INFILE>; }

READ: while ($line = <INFILE>)
{
  chomp($line);
  #end data if line is really short, or contains 78 dashes ('-')
  last READ if (length($line) < 5 || $line =~ m/-{78}/) ;

  #check to see if the site changed
  $cur_site_name = substr $line, 0, 20;
  if ($cur_site_name =~ /\S/g)
    { # if so, go get new site datatype ids
      $site_id = $siteid_hash{$cur_site_name};
      get_SDI($site_id)      if $site_id;
    }
#chop out the pieces of the line that are interesting, date, precip,
# and 3 temperatures, max, min, and average
  my $monthdaystr = substr $line, 22, 5;
  $value_hash{"total wy precip"} = substr $line, 44, 6;
  $value_hash{"max temp"}        = substr $line, 61, 6;
  $value_hash{"min temp"}        = substr $line, 67, 6;
  $value_hash{"ave temp"}        = substr $line, 73, 6;

  $value_hash{"total wy precip"} =~ s/\s//g;
  $value_hash{"max temp"} =~ s/\s//g;
  $value_hash{"min temp"} =~ s/\s//g;
  $value_hash{"ave temp"} =~ s/\s//g;

  $value_date[1] = substr $monthdaystr, 0, 2;
  $value_date[2] = substr $monthdaystr, 3, 2;

#handle finding year value, if value is for a month after the date on the
#file, the year for the value is the previous year
  if ($value_date[1] > $data_date[1]) {
    $value_date[0] = $data_date[0] - 1;
  }
  else {
    $value_date[0] = $data_date[0];
  }

# fix the fact that the data is reported for the next day!
  @value_date = Add_Delta_Days(@value_date, -1);

  if ($value_hash{"total wy precip"} == -99.9 ) {
    $value_hash{"total wy precip"} = undef;
  }
  if ($value_hash{"max temp"} == -99.9 ) {
    $value_hash{"max temp"} = undef;
  }
  if ($value_hash{"min temp"} == -99.9 ) {
    $value_hash{"min temp"} = undef;
  }
  if ($value_hash{"ave temp"} == -99.9 ) {
    $value_hash{"ave temp"} = undef;
  }

  if ($site_id) {
    insert_values(@value_date);
  }
}

$hdb->dbh->disconnect();

#End main program, subroutines below

sub check_value
  {
    my($check_date)=$_[0];
    my($site_datatype_id)=$_[1];
    my($sth);
    my($myval);

    my $check_data_statement = "select value from r_base where start_date_time = ? and site_datatype_id = ? and interval = 'day'";

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

sub last_value
{
    my($prev_date) = $_[0];
    my($site_datatype_id)=$_[1];
    my($myval);

    my $find_data_statement = "select a.value from r_base a where
a.site_datatype_id = ? and interval = 'day' and
a.start_date_time = (select max(start_date_time) from r_base a where
a.site_datatype_id = ? and interval = 'day' and
a.start_date_time < ? )
";

    my($sth);
    $sth = $hdb->dbh->prepare($find_data_statement) || die $sth->errstr;

    $sth->bind_param(1,$site_datatype_id);
    $sth->bind_param(2,$site_datatype_id);
    $sth->bind_param(3,$prev_date);
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
my $unk_computation_id;
my $diff_computation_id;

sub get_app_ids
{
# Get ids to describe where data came from
  my $nameid;

  $nameid->{agen}->{name} = 'Natural Resources Conservation Service';
  $nameid->{collect}->{name} = 'Snotel';
  $nameid->{load_app}->{name} = 'guntemps2hdb.pl';
  $nameid->{method}->{name} = 'unknown';
  $nameid->{computation}->{name} = 'unknown';

  $hdb->get_app_ids($nameid);

  $agen_id = $nameid->{agen}->{id};
  $collect_id = $nameid->{collect}->{id};
  $load_app_id = $nameid->{load_app}->{id};
  $method_id = $nameid->{method}->{id};
  $unk_computation_id = $nameid->{computation}->{id};

  my $sth = $hdb->dbh->prepare("select computation_id from hdb_computed_datatype where computation_name = 'difference'");
  eval {
    $sth->execute;
    $sth->bind_col(1,\$diff_computation_id);
    $sth->fetch;
    $sth->finish;
  };

  if ($@ or !defined($diff_computation_id)) { # something screwed up
    print $hdb->dbh->errstr, " $@\n";
    $hdb->hdbdie("Errors occurred during selection of difference computation ids for r_base.\n");
  }
}

sub insert_values
  {
    my(@value_date) = @_[0,1,2];
    my($cum_precip) = $value_hash{"total wy precip"};

    my $end_date_time = undef;
    my($modsth, $computation_id);
    my($day_precip) = undef;
#    print @_;

    my $modify_data_statement = "
    BEGIN
        modify_r_base_raw(?,'day',?,?,?, /* sdi, interval, start_date_time, end_date_time (in/out, not used), value */
                          $agen_id,null,'Z', /* overwrite, validation */
                          $collect_id,$load_app_id,$method_id,?, /*unk_ or diff_computation_id*/
                          'Y');                 /*do update? */
    END;";


    $datestr = sprintf("%02d-%.3s-%4d",$value_date[2],
			   Month_to_Text($value_date[1]), $value_date[0]);

    eval {
      $modsth = $hdb->dbh->prepare($modify_data_statement) || die $modsth->errstr;
#      $modtotsth = $hdb->dbh->prepare($modify_tot_data_statement) || die $modtotsth->errstr;

# do precip, if the data are listed from 10/01 ( cumulative till midnight of 9/30), the cumulative counter resets.
      if ($value_date[1] == 9 && $value_date[2] == 30) {
	$day_precip = $cum_precip;
      }
      else {
	my @prev_date =  Add_Delta_Days(@value_date, -1);
	my $prevdatestr = sprintf("%02d-%.3s-%4d",$prev_date[2],
			   Month_to_Text($prev_date[1]), $prev_date[0]); 
	my $prev_cum_precip = check_value($prevdatestr,
				       $site_datatype_hash{"total wy precip"});
	#assume data is coming in order of date, earlier first.
	#if prev data is not there, set day's to undef, won't insert
	if (!defined($cum_precip)) {
	} elsif (!defined($prev_cum_precip)) {
	  $prev_cum_precip = last_value($prevdatestr,
				 $site_datatype_hash{"total wy precip"});
	  if (!defined($prev_cum_precip)) {
	    $day_precip = undef; #still can not find it.
	  } else {
	    $day_precip = sprintf("%.1f",$cum_precip - $prev_cum_precip);
	  }
	} else {
	  $day_precip = sprintf("%.1f",$cum_precip - $prev_cum_precip);
	  $day_precip = 0 if $day_precip < 0 ;
	}
      }
      # add one more value to the end of the values array to represent the
      # day's precip
      $value_hash{"precip total"} = $day_precip;

# insert data;
      my ($datatype, $old_val, $cur_sdi);
      foreach $datatype ( keys %site_datatype_hash )
	{
          if ($datatype eq 'precip total') {
            $computation_id  = $diff_computation_id;
          } else {
            $computation_id  = $unk_computation_id;
          }

	  undef $old_val;
	  $old_val = check_value($datestr, $site_datatype_hash{$datatype});
	  if (!defined $value_hash{$datatype}) {
	    # do nothing, could delete a value if it is in the db here?
	  }
	  elsif (defined($old_val) and $old_val == $value_hash{$datatype}) {
	    next;
	  }
	  elsif (!defined($old_val) or $old_val != $value_hash{$datatype}) {
            if (!defined($old_val)) {
              print "modifying for $site_datatype_hash{$datatype}, date $datestr, value $value_hash{$datatype}, old_val = undef\n";
            } else {
              print "modifying for $site_datatype_hash{$datatype}, date $datestr, value $value_hash{$datatype}, old_val = $old_val\n";
            }
	    $modsth->bind_param(1,$site_datatype_hash{$datatype});
	    $modsth->bind_param(2,$datestr);
	    $modsth->bind_param_inout(3,\$end_date_time,50);
	    $modsth->bind_param(4,$value_hash{$datatype});
	    $modsth->bind_param(5,$computation_id);
	    $modsth->execute|| die $modsth->errstr;
	  }
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
    my ($sth, $datatype);
    my $get_site_datatype_statement = "select site_datatype_id from hdb_site_datatype where site_id = ? and datatype_id = ? ";

    $sth = $hdb->dbh->prepare($get_site_datatype_statement) || die $sth->errstr;
    $sth->bind_param(1,$cur_site);

    foreach $datatype (keys %datatype_hash)
      {
	$sth->bind_param(2,$datatype_hash{$datatype});
	$sth->execute|| die $sth->errstr;
	$sth->bind_col(1,\$site_datatype_hash{$datatype});
        $sth->fetch;
      }
    $sth->finish;
  }
