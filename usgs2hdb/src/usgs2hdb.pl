#!/usr/bin/perl -w

use LWP::UserAgent;
use DBI;
use strict;

my ($dbh,$psswd_dbh);

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
#check to see command line usage.
my $progname = `basename $0`;
chop $progname;

if (scalar(@ARGV) <3)
  {
    print "$progname -- program to put USGS flow data into HDB.\n";
    print "Usage: $progname <num days> <hdbusername> <hdbpassword>\n";
    exit (1);
  }

if ($ARGV[0] =~ m/\D/) {
  print "First argument must be numeric.\n";
  print "Usage: $progname <num days> <hdbusername> <hdbpassword>\n";
  exit (1);
}

if ($ARGV[0] >31 ) {
  print "Number of days must be less than 31.\n";
  print "Usage: $progname <num days> <hdbusername> <hdbpassword>\n";
  exit (1);
}

#create connection to database
connect_to_db($ARGV[1], $ARGV[2]);

my $url = "http://waterdata.usgs.gov/nwis/uv?format=rdb&period=$ARGV[0]&site_no=";

my $get_usgs_no_statement = "select a.usgs_id, b.site_datatype_id from
hdb_site a, hdb_site_datatype b, hdb_datatype c, ref_hm_site_datatype d
where a.site_id = b.site_id and
b.datatype_id = c.datatype_id and
b.datatype_id = 18 and
a.usgs_id is not null and
/*a.usgs_id like '09165000%' and*/
b.site_datatype_id = d.site_datatype_id and
d.hourly = 'Y'";

my $insert_data_statement = "insert into r_instant values(?,?,?,11,'Z')";

my $update_data_statement = "update r_instant SET (value) = ( ?) where (site_datatype_id = ? and date_time = ?) ";

my $check_data_statement = "select value from r_instant where date_time = ? and site_datatype_id = ? ";


my $ua = LWP::UserAgent->new;
$ua->agent('USGS Streamflow -> US Bureau of Reclamation HDB dataloader: ' . $ua->agent);
$ua->from('agilmore@uc.usbr.gov');
my $request = HTTP::Request->new();
$request->method('GET');
my $usgs_sites = get_usgs_sites();

my (@data,@headers);
my ($usgs_no,$column);

USGSNO: foreach $usgs_no (keys %$usgs_sites) {
  $request->uri($url . $usgs_no);
  my $response = $ua->simple_request($request);
  if ($response->is_success) {
    @data = split /\n/,$response->content;
    while (substr ($data[0], 0, 1) eq '#') {
      shift @data;
    }
    #figure out which column has data
    @headers = split /\t/, $data[0];
    if (@headers <4) {
        print STDERR "unable to find any data for this site: $usgs_no\n";
	next USGSNO;
    }
# 00060 is discharge. Lameness abounds.
    my $i=0;
    until (defined($headers[$i]) && ($headers[$i] =~ m/_00060/)) {
      $i++;
      if ($i>20) { #don't expect more than 16 realtime parameters per site
        print STDERR "unable to find any data for this site: $usgs_no\n";
	next USGSNO;
      }
    }
    $column = $i;
    #get rid of actual header on data
    shift @data;
    # now we have data
    insert_values(\@data, $usgs_sites->{$usgs_no}->{site_datatype_id},
		  $column);
  } else {
    print $response->error_as_HTML;
  }
  sleep 5;
}

$dbh->disconnect();

#End main program, subroutines below

sub get_usgs_sites
{
  $dbh->{FetchHashKeyName} = 'NAME_lc';
  return $dbh->selectall_hashref($get_usgs_no_statement,1);
}

sub check_value
{
  my($check_date)=$_[0];
  my($site_datatype_id)=$_[1];
  my($sth, $value);

  $sth = $dbh->prepare($check_data_statement) || mydie $sth->errstr;
  $sth->bind_param(1,$check_date);
  $sth->bind_param(2,$site_datatype_id);

  $sth->execute || mydie $sth->errstr;
  $sth->bind_col(1,\$value);
  $sth->fetch;
  $sth->finish();
  return $value;
}

sub insert_values
{
  my @data = @{$_[0]};
  my $cur_sdi = $_[1];
  my $column = $_[2];



  my $i=0;
  my $cur_date = undef;
  my ($value, $value_date, $updated_date, @row);
  my ($line, $date, $datestr, $old_val);

  eval {
    my ($insth,$upsth);
    $insth = $dbh->prepare($insert_data_statement) || mydie $insth->errstr;
    $upsth = $dbh->prepare($update_data_statement) || mydie $upsth->errstr;

    # insert data;
    foreach $line (@data)
    {
      @row = split /\t/, $line;
      $value_date = $row[2];
      $value = $row[$column];
      if ($value) {
	$value =~ s/,//g;
      }

      undef $old_val;
      $old_val = check_value($value_date, $cur_sdi);
      if (!defined $value || $value eq '' || $value =~ m/Ice/) {
	print "data missing: $cur_sdi, date $value_date\n";
      } elsif (!defined $old_val) {
	print "inserting for $cur_sdi, date $value_date, value $value, old_val = undefined\n";

	$insth->bind_param(1,$cur_sdi);
	$insth->bind_param(2,$value_date);
	$insth->bind_param(3,$value);
	$insth->execute|| mydie $insth->errstr;
	$updated_date = $value_date;
      } elsif ($old_val == $value) {
	next;
      } elsif ($old_val != $value) {
	print "updating for $cur_sdi, date $value_date, value $value, old_val = $old_val\n";
	next;
	#update instead of insert
	$upsth->bind_param(1,$value);
	$upsth->bind_param(2,$cur_sdi);
	$upsth->bind_param(3,$value_date);
	$upsth->execute|| mydie $upsth->errstr;
	$updated_date = $value_date;
      }
    }
    $upsth->finish;
    $insth->finish;
  };

  if ($@) {
    $dbh->rollback;
    print "$DBI::errstr, $@\n";
    mydie "Errors occurred during insert/update/deletion of data. Rolled back any database manipulation.";
  } else {
    $dbh->commit;
    if ($updated_date) {
      $dbh->do("update ref_hm_site_datatype set (max_hourly_date) = ('$updated_date') where (site_datatype_id = $cur_sdi) ");
    }
    $dbh->commit;
  }
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
    my ($approlepass, $psswduser, $dbname, $approle);

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

    my $psswd_dbh = DBI->connect("dbi:Oracle:$dbname", $psswduser, $dbname,
			{ RaiseError => 1, AutoCommit => 0 })|| mydie $DBI::errstr;
    my $psswd_statement = "select psswd from role_psswd where UPPER(ROLE) = UPPER('$approle')";

    my $psswdsth;
    $psswdsth = $psswd_dbh->prepare($psswd_statement) || passdie $psswdsth->errstr;
    $psswdsth->execute || passdie $psswdsth->errstr;
    $psswdsth->bind_col(1,\$approlepass);
    $psswdsth->fetch;
    $psswdsth->finish;
    $psswd_dbh->disconnect;

    $dbh = DBI->connect("dbi:Oracle:$dbname", $user, $pass,
			{ RaiseError => 1, AutoCommit => 0 })|| mydie $DBI::errstr;

    $dbh->do("set role $approle identified by $approlepass")|| mydie $DBI::errstr;
    #Set date format to ISO, USGS data also in this format
    $dbh->do("ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD HH24:MI'");
  }
