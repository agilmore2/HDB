#!/usr/local/bin/perl -w

use Date::Calc qw(:all);
use DBI;
#use DBD::Oracle;

my ($dbh, @cur_sdi, @value_date, @data_date);

my($psswd_dbh);

sub mydie {
  $dbh->disconnect;
  die @_;
}

sub passdie {
    $psswd_dbh->disconnect;
    die @_;
}
    
#sites and datatypes desired. Not all site_ids and site_datatype_ids
# have been created, yet.

%siteid_hash = ('ELEPHANT BUTTE RESERVOIR', 1119, "CABALLO RESERVOIR", 1094);

%datatype_hash = ("ELEVATION", 49, "RELEASE", 42, "STORAGE", 17,
		  "COMPUTED INFLOW", 30);

%site_datatype_hash = ("ELEVATION", undef, "RELEASE", undef, "STORAGE", undef,
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

#create connection to database
connect_to_db($ARGV[1], $ARGV[2]);

#$statement = "select * from hdb_data_source ";

$get_site_datatype_statement = "select site_datatype_id from hdb_site_datatype where site_id = ? and datatype_id = ? ";

$insert_data_statement = "insert into r_day values(?,?,?, 14,'Z') ";

$update_data_statement = "update r_day SET (value) = ( ?) where (site_datatype_id = ? and date_day = ?) ";

$check_data_statement = "select value from r_day where date_day = ? and site_datatype_id = ? ";

open (INFILE, "$ARGV[0]") || mydie "Error: couldn't open input file $ARGV[0]";
# skip past opening junk
for ($i = 0; $i < 49; $i++)
  { $line = <INFILE>; }

# get date of data
$line = <INFILE>;
$datestr = substr $line, 0, 9;

(@data_date = Decode_Date_EU($datestr)) ||
  die "There was an error parsing the date in the datafile!.\n";

print "@data_date\n";

# fix the fact that the data is reported for the next day!
  @value_date = Add_Delta_Days(@data_date, -1);

#skip past header
for ($i = 0; $i < 179-51; $i++)
  { $line = <INFILE>; }

READ: while ($line = <INFILE>)
{
  chomp($line);
  #end data if line is really short, or contains 78 dashes ('-')
#  last READ if (length($line) < 5 || $line =~ m/-{78}/) ;

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
  $elevation = substr $line, 0, index $line, '<';

  #read one line of html junk
  $line = <INFILE>;
  #read one datapoint
  $line = <INFILE>;
  $release = substr $line, 0, index $line, '<';

  #read one line of html junk
  $line = <INFILE>;
  #read one datapoint
  $line = <INFILE>;
  $storage = substr $line, 0, index $line, '<';

  #read one line of html junk
  $line = <INFILE>;
  #read one datapoint
  $line = <INFILE>;
  $inflow = substr $line, 0, index $line, '<';

  $elevation =~ s/\s//g;
  $elevation =~ s/\,//g;
  $release =~ s/\s//g ;
  $release =~ s/\,//g;
  $storage =~ s/\s//g;
  $storage =~ s/\,//g;
  $inflow =~ s/\s//g;
  $inflow =~ s/\,//g;

#  if ($elevation =~ /\D/ ) {
#    $elevation = undef;
#  }
#  if ($release == /\D/ ) {
#    $release = undef;
#  }
#  if ($storage == /\D/ ) {
#    $storage = undef;
#  }
#  if ($inflow == /\D/ ) {
#    $inflow = undef;
#  }

  if ($site_id) {
    insert_values(@value_date, $elevation, $release, $storage, $inflow);
  }
  for ($i = 0; $i < 8; $i++)
  { $line = <INFILE>; }
}

#$dbh->disconnect();

#End main program, subroutines below

sub check_value
  {
    my($check_date)=$_[0];
    my($site_datatype_id)=$_[1];
    my($sth);
    my($myval);

    $sth = $dbh->prepare($check_data_statement) || mydie $sth->errstr;
    $sth->bind_param(1,$check_date);
    $sth->bind_param(2,$site_datatype_id);
    $sth->execute || mydie $sth->errstr;
    $sth->bind_col(1,\$myval);
    $sth->fetch;
#    DBI::dump_results($sth)|| mydie $DBI::errstr;;
    $sth->finish();
#    print $myval;
    return $myval;
  }


sub insert_values
  {
    my(@value_date) = @_[0,1,2];
    my(@cur_values) = @_[3,4,5,6];

    my($insth, $upsth);
#    print @_;

    $datestr = sprintf("%02d-%.3s-%4d",$value_date[2],
			   Month_to_Text($value_date[1]), $value_date[0]); 

    eval {
      $insth = $dbh->prepare($insert_data_statement) || mydie $insth->errstr;
      $upsth = $dbh->prepare($update_data_statement) || mydie $upsth->errstr;

      $i = -1;
# insert data;
      foreach $datatype (keys %datatype_hash)
	{
	  $i++;
	  undef $old_val;
	  $old_val = check_value($datestr, $site_datatype_hash{$datatype});
#	  if (!defined $cur_values[$i]) {
#	    #delete data if data is -99.9, otherwise do nothing.
#	    if ($old_val && $old_val == -99.9) {
#	      print "deleting bad data for $site_datatype_hash{$datatype}, date $datestr, value undefined, old_val = $old_val\n";
#	      $dbh->do("delete from r_day where site_datatype_id = $site_datatype_hash{$datatype} and date_day = '$datestr'")|| mydie DBI::errstr;
#	    }
#	  }
#	  els
	  if (!defined($old_val)) {
	    print "inserting for $site_datatype_hash{$datatype}, date $datestr, value $cur_values[$i], old_val = undefined\n";

	    $insth->bind_param(1,$site_datatype_hash{$datatype});
	    $insth->bind_param(2,$datestr);
	    $insth->bind_param(3,$cur_values[$i]);
	    $insth->execute|| mydie $insth->errstr;
	  }
	  elsif ($old_val == $cur_values[$i]) {
	    next;
	  }
	  elsif ($old_val != $cur_values[$i]) {
	    print "updating for $site_datatype_hash{$datatype}, date $datestr, value $cur_values[$i], old_val = $old_val\n";
	    #update instead of insert
	    $upsth->bind_param(1,$cur_values[$i]);
	    $upsth->bind_param(2,$site_datatype_hash{$datatype});
	    $upsth->bind_param(3,$datestr);
	    $upsth->execute|| mydie $upsth->errstr;
	  }
	}

      $upsth->finish;
      $insth->finish;
    };
    if ($@) {
      $dbh->rollback;
      print "$DBI::errstr, $@\n";
      mydie "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.";
    }
    else {
      $dbh->commit;
    }
}


sub get_SDI
  {
    $cur_site= $_[0];
    $sth = $dbh->prepare($get_site_datatype_statement) || mydie $sth->errstr;
    $sth->bind_param(1,$cur_site);

    foreach $datatype (keys %datatype_hash)
    {
      $sth->bind_param(2,$datatype_hash{$datatype});
      $sth->execute|| mydie $sth->errstr;
      $sth->bind_col(1,\$site_datatype_hash{$datatype});
#should check here if there are any rows. if not, no SDI exists for this combo.
      $sth->fetch;
    }

    $sth->finish;
  }

# HDB does something interesting with the app_user 
# app_user has the permission to insert into the database, but only
# if the app_role is enabled, which is not by default
# the applications get around this by connecting to the database as 
# psswd_user (from the environment) and getting the password to 
# turn on this role, and then connecting as app_user and 
# enabling this role.
# This code does that, while also taking the name of the database
# from the environment, and the app_user name and password from the
# command line
sub connect_to_db
  {
    my $user=$_[0];
    my $pass=$_[1];
    my $approlepass;

    if (!defined($psswduser = $ENV{PSSWD_USER})) {
      die "Environment variable PSSWD_USER not set...\n";
    }
    if (!defined($dbname = $ENV{HDB_LOCAL})) {
      die "Environment variable HDB_LOCAL not set...\n";
    }
    if (!defined($approle = $ENV{APP_ROLE})) {
      die "Environment variable APP_ROLE not set...\n";
    }

# create connections, exceptions on error, no autocommit

    $psswd_dbh = DBI->connect("dbi:Oracle:$dbname", $psswduser, $dbname,
			{ RaiseError => 1, AutoCommit => 0 })|| mydie $DBI::errstr;
    $psswd_statement = "select psswd from role_psswd where UPPER(ROLE) = UPPER('$approle')";

    $psswdsth = $psswd_dbh->prepare($psswd_statement) || passdie $psswdsth->errstr;
    $psswdsth->execute || passdie $psswdsth->errstr;
    $psswdsth->bind_col(1,\$approlepass);
    $psswdsth->fetch;
    $psswdsth->finish;
    $psswd_dbh->disconnect;

    $dbh = DBI->connect("dbi:Oracle:$dbname", $user, $pass,
			{ RaiseError => 1, AutoCommit => 0 })|| mydie $DBI::errstr;

    $dbh->do("set role $approle identified by $approlepass")|| mydie $DBI::errstr;
  }
