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

# site names (shef codes) and site id's for Cimmaron (Cimmaron near silver 
# jack res or some such) and Ouray Hwy
%siteid_hash = ("SJAC2", 23, "OMSC2", 57); 

#we don't get total wy precip or ave temp from nws, so this list is shorter
%datatype_hash = ("max temp", 9, "min temp", 11,
		  "precip total", 5);

%site_datatype_hash = ("max temp", undef, "min temp", undef,
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

#create connection to database
connect_to_db($ARGV[1], $ARGV[2]);

# various SQL statement strings, with placeholders for variables.

$get_site_datatype_statement = "select site_datatype_id from hdb_site_datatype where site_id = ? and datatype_id = ? ";

#the 8 stands for NWS, and the Z means no validation
$insert_data_statement = "insert into r_day values(?,?,?,8,'Z') ";

$update_data_statement = "update r_day SET (value) = ( ?) where (site_datatype_id = ? and date_day = ?) ";

$check_data_statement = "select value from r_day where date_day = ? and site_datatype_id = ? ";

open (INFILE, "$ARGV[0]") || mydie "Error: couldn't open input file $ARGV[0]";

# No junk before data in this file format!

# get date of data is interesting, because file contains no standalone date.
# just have to use date of file creation. 

@fileinfo = stat INFILE;

print "$fileinfo[10]\n";

$cur_site_name = "none";

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

  $year = $fields[8];
  $month = $fields[9];
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
  $daysinmonth = Days_in_Month($year,$month);

  for ($day = 1; $day < $daysinmonth; $day++) {
    @value_date = ($year,$month,$day);
			   
    # fix the fact that the data is reported for the previous day!
    # for the NWS data, the data is for the time period 1200 ZULU the
    # previous day to 1200 ZULU the date of the data, this is approximately
    # 4 am the date of the data MDT, so we place the data for the period of the
    # previous day.
    @value_date = Add_Delta_Days(@value_date, -1);

    $value = $fields[$day+9];

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

$dbh->disconnect();

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
    my($sdi) = $_[3];    
    my($value) = $_[4];

    my($insth, $upsth);
#    print @_;

    $datestr = sprintf("%02d-%.3s-%4d",$value_date[2],
			   Month_to_Text($value_date[1]), $value_date[0]); 

    eval {
#only use these once per prepare/finish, could do better
      $insth = $dbh->prepare($insert_data_statement) || mydie $insth->errstr;
      $upsth = $dbh->prepare($update_data_statement) || mydie $upsth->errstr;
      
# insert data;
      undef $old_val;
      $old_val = check_value($datestr, $sdi);
      if (!defined $value) {
	#delete data if data is -99.9, otherwise do nothing.
	if ($old_val && $old_val == -99.9) {
	  print "deleting bad data for $sdi, date $datestr, value undefined, old_val = $old_val\n";
	  $dbh->do("delete from r_day where site_datatype_id = $sdi and date_day = '$datestr'")|| mydie DBI::errstr;
	}
      }
      elsif (!defined($old_val)) {
	print "inserting for $sdi, date $datestr, value $value, old_val = undefined\n";
	    
	$insth->bind_param(1,$sdi);
	$insth->bind_param(2,$datestr);
	$insth->bind_param(3,$value);
	$insth->execute|| mydie $insth->errstr;
      }
      elsif ($old_val == $value) {
      }
      elsif ($old_val != $value) {
	print "updating for $sdi, date $datestr, value $value, old_val = $old_val\n";
	#update instead of insert
	$upsth->bind_param(1,$value);
	$upsth->bind_param(2,$sdi);
	$upsth->bind_param(3,$datestr);
	$upsth->execute|| mydie $upsth->errstr;
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
