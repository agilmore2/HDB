#!/usr/local/bin/perl -w

use lib "$ENV{HDB_ENV}/perlLib/lib";
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

my %siteid_hash = ('ELEPHANT BUTTE RES.', 1119, "CABALLO RESERVOIR", 1094);

my %datatype_hash = ("ELEVATION", 49, "RELEASE", 42, "STORAGE", 17,
		  "COMPUTED INFLOW", 29);

my %site_datatype_hash = ("ELEVATION", undef, "RELEASE", undef, "STORAGE", undef,
		       "COMPUTED INFLOW", undef);

my %value_hash = ("ELEVATION", undef, "RELEASE", undef, "STORAGE", undef,
	       "COMPUTED INFLOW", undef);

#check to see command line usage.
if (scalar(@ARGV) <3)
  {
    my $progname = `basename $0`;
    chop $progname;
    print "$progname -- program to put elpaso data into HDB.\n";
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

open (INFILE, "$ARGV[0]") || die "Error: couldn't open input file $ARGV[0]";

my ($i, $line);
# skip past opening junk
for ($i = 0; $i < 52; $i++)
  { $line = <INFILE>; }

# get date of data
$line = <INFILE>;
my $datestr = substr $line, 0, 9;

(@data_date = Decode_Date_EU($datestr)) ||
  die "There was an error parsing the date in the datafile!.\n";

# fix the fact that the data is reported for the next day!
@value_date = Add_Delta_Days(@data_date, -1);

# reuse string to hold Oracle ready date string
  $datestr = sprintf("%02d-%.3s-%4d",$value_date[2],
			   Month_to_Text($value_date[1]), $value_date[0]);
print "$datestr\n";

#skip past header
SKIP: while ($line = <INFILE>) 
{
  if ($line =~ /RESERVOIR DATA/) {
    #skip 12 more lines
    for ($i = 0; $i < 12; $i++) {
      $line = <INFILE>;
    }
    last SKIP;
  }
}

my ($cur_site_name, $site_id);
READ: while ($line = <INFILE>)
{
  chomp($line);
  #end data if have already read data for Caballo
  last READ if defined($cur_site_name) && $cur_site_name eq "CABALLO RESERVOIR";

  #check to see if the site changed
  $cur_site_name = substr $line, 0, index $line, '<';
  if ($cur_site_name =~ /\S/g)
    { # if so, go get new site datatype ids
      $site_id = $siteid_hash{$cur_site_name};
      get_SDI($site_id) if $site_id;
    }
  #read one line of html junk
  $line = <INFILE>;
  #read one datapoint
  $line = <INFILE>;
  $value_hash{"ELEVATION"} = substr $line, 0, index $line, '<';

  #read one line of html junk
  $line = <INFILE>;
  #read one datapoint
  $line = <INFILE>;
  $value_hash{"RELEASE"} = substr $line, 0, index $line, '<';

  #read one line of html junk
  $line = <INFILE>;
  #read one datapoint
  $line = <INFILE>;
  $value_hash{"STORAGE"} = substr $line, 0, index $line, '<';

  #read one line of html junk
  $line = <INFILE>;
  #read one datapoint
  $line = <INFILE>;
  $value_hash{"COMPUTED INFLOW"} = substr $line, 0, index $line, '<';

  $value_hash{"ELEVATION"} =~ s/\s//g;
  $value_hash{"ELEVATION"} =~ s/\,//g;
  $value_hash{"RELEASE"} =~ s/\s//g ;
  $value_hash{"RELEASE"} =~ s/\,//g;
  $value_hash{"STORAGE"} =~ s/\s//g;
  $value_hash{"STORAGE"} =~ s/\,//g;
  $value_hash{"COMPUTED INFLOW"} =~ s/\s//g;
  $value_hash{"COMPUTED INFLOW"} =~ s/\,//g;

# Handle negative values reported as (200)
  $value_hash{"COMPUTED INFLOW"} =~ s/\)//g;
  $value_hash{"COMPUTED INFLOW"} =~ s/\(/-/g;

# NO longer have to convert AF to cfs
#  if ($value_hash{"COMPUTED INFLOW"}) {
#    $value_hash{"COMPUTED INFLOW"} = sprintf("%.1f",$value_hash{"COMPUTED INFLOW"}/1.98347);
#  }

#values are in a global hash, so just need to pass in date, and could omit that

  if ($site_id) {
    insert_values($datestr);
  }
  for ($i = 0; $i < 6; $i++)
  { $line = <INFILE>; }
}

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

my $agen_id;
my $collect_id;
my $load_app_id;
my $method_id;
my $computation_id;

sub get_app_ids
{
# Get ids to describe where data came from
  my $nameid;

  $nameid->{agen}->{name} = 'Bureau of Reclamation';
  $nameid->{collect}->{name} = '(see agency)';
  $nameid->{load_app}->{name} = 'elpaso2hdb.pl';
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
    $datestr = $_[0];

    my($modsth);
    my($end_date_time);

#    print @_;

    my $modify_data_statement = "
    BEGIN
        modify_r_base_raw(?,'day',?,?,?, /* sdi, interval, start_date_time, end_date_time (in/out, not used), value */
                          $agen_id,null,'Z',              /* overwrite, validation */
                          $collect_id,$load_app_id,$method_id,$computation_id,
                          'Y');                 /*do update? */
    END;";


    eval {
      $modsth = $hdb->dbh->prepare($modify_data_statement) || die $modsth->errstr;

# insert data;
      my ($datatype, $old_val, $cur_sdi);
      foreach $datatype (keys %datatype_hash)
	{
	  undef $old_val;
	  $old_val = check_value($datestr, $site_datatype_hash{$datatype});

	  if (!defined($old_val)) {
	    print "modifying for $site_datatype_hash{$datatype}, date $datestr, value $value_hash{$datatype}, old_val = undefined\n";
	  }
	  elsif ($old_val == $value_hash{$datatype}) {
	    next;
	  }
	  elsif ($old_val != $value_hash{$datatype}) {
	    print "modifying for $site_datatype_hash{$datatype}, date $datestr, value $value_hash{$datatype}, old_val = $old_val\n";
	    #update instead of insert
	  }
	  $modsth->bind_param(1,$site_datatype_hash{$datatype});
	  $modsth->bind_param(2,$datestr);
	  $modsth->bind_param_inout(3,\$end_date_time,50);
	  $modsth->bind_param(4,$value_hash{$datatype});
	  $modsth->execute|| die modsth->errstr;
	  
	  $end_date_time = undef;
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
#should check here if there are any rows. if not, no SDI exists for this combo.
      $sth->fetch;
    }

    $sth->finish;
  }
