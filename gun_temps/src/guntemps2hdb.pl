#!/usr/local/bin/perl -w

use Date::Calc qw(:all);
use DBI;
#use DBD::Oracle;

my ($dbh, @cur_sdi, @value_date, @data_date);

sub mydie {
  $dbh->disconnect();
  die @_;
}

#sites and datatypes desired. Not all site_ids and site_datatype_ids
# have been created, yet.

%siteid_hash = ('PARK_CONE___________', 28, "IDARADO_____________", 133, 
		"BUTTE_______________", 22, "SCHOFIELD_PASS______", 32,
		"PORPHYRY_CREEK______", 30, "SLUMGULLION_________", 33,
		"BRUMLEY_____________", undef, "NORTH_LOST_TRAIL____", undef);

%datatype_hash = ("max temp", 9, "ave temp", 8, "min temp", 11,
		  "precip total", 5, "total wy precip", 72);

%site_datatype_hash = ("max temp", undef, "ave temp", undef, "min temp", undef,
		       "precip total", undef, "total wy precip", undef);

#create connection to database, exceptions on error, no autocommit

$dbh = DBI->connect("dbi:Oracle:uchdb", $ARGV[0], $ARGV[1],
		    { RaiseError => 1, AutoCommit => 0 })|| mydie $DBI::errstr;

#$statement = "select * from hdb_data_source ";

$get_site_datatype_statement = "select site_datatype_id from hdb_site_datatype where site_id = ? and datatype_id = ? ";

$insert_data_statement = "insert into r_day values(?,?,?,2,'Z') ";

$update_data_statement = "update r_day SET (value) = ( ?) where (site_datatype_id = ? and date_day = ?) ";

$check_data_statement = "select value from r_day where date_day = ? and site_datatype_id = ? ";


# skip past opening junk
for ($i = 0; $i < 6; $i++)
  { $line = <STDIN>; }

# get date of data
$line = <STDIN>;
$datestr = substr $line, 33, 8;

(@data_date = Decode_Date_US($datestr)) ||
  die "There was an error parsing the end date.\n";

print "@data_date\n";

#skip past header
for ($i = 0; $i < 6; $i++)
  { $line = <STDIN>; }


READ: while ($line = <STDIN>)
{
  chomp($line);
  last READ if (length($line) < 5 || $line =~ m/-{78}/) ;

  #check to see if the site changed
  $cur_site_name = substr $line, 0, 20;
  if ($cur_site_name =~ /\S/g)
    { # if so, go get new site datatype ids
#     print "$cur_site_name\n";
#     print "$siteid_hash{$cur_site_name}\n";
      $site_id = $siteid_hash{$cur_site_name};
      get_SDI($site_id)      if $site_id;
    }
#chop out the pieces of the line that are interesting, date, precip,
# and 3 temperatures, max, min, and average
  $monthdaystr = substr $line, 22, 5;
  $cum_precip = substr $line, 45, 5;
  $max_temp = substr $line, 61, 6;
  $min_temp = substr $line, 67, 6;
  $ave_temp = substr $line, 73, 6;

  $cum_precip =~ s/\s//g;
  $max_temp =~ s/\s//g;
  $min_temp =~ s/\s//g;
  $ave_temp =~ s/\s//g;

  $value_date[1] = substr $monthdaystr, 0, 2;
  $value_date[2] = substr $monthdaystr, 3, 2;

  if ($value_date[1] > $data_date[1]) {
    $value_date[0] = $data_date[0] - 1;
  }
  else {
    $value_date[0] = $data_date[0]; 
  }

# fix the fact that the data is reported for the next day!
  @value_date = Add_Delta_Days(@value_date, -1);

  if ($max_temp == -99.9 ) {
    $max_temp = undef;
  }
  if ($min_temp == -99.9 ) {
    $min_temp = undef;
  }
  if ($ave_temp == -99.9 ) {
    $ave_temp = undef;
  }
#  print "@value_date\t";
#  print "$site_id\t$cum_precip\t$max_temp\t$min_temp\t$ave_temp\n";

  if ($site_id) {
    insert_values(@value_date, $cum_precip, $max_temp, $min_temp, $ave_temp);
  }
}

#print check_value('29-mar-2000', 2369);

$dbh->disconnect();

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
    my($cum_precip) = $_[3];    
    my($max_precip) = $_[4];    
    my($min_precip) = $_[5];    
    my($ave_precip) = $_[6];    
    my($insth, $upsth);

#    print @_;

    $datestr = sprintf("%02d-%.3s-%4d",$value_date[2],
			   Month_to_Text($value_date[1]), $value_date[0]); 

    $insth = $dbh->prepare($insert_data_statement) || mydie $insth->errstr;
    $upsth = $dbh->prepare($update_data_statement) || mydie $upsth->errstr;

    $i = 3;
    foreach $datatype ("max temp", "min temp", "ave temp") 
      {
	$i++; undef $cur_val;
	$cur_val = check_value($datestr, $site_datatype_hash{$datatype});
	if (!defined($cur_val)) {
	  print "inserting for $site_datatype_hash{$datatype}, date $datestr, value $_[$i], cur_val = undefined\n";
	
	  $insth->bind_param(1,$site_datatype_hash{$datatype});
	  $insth->bind_param(2,$datestr);
	  $insth->bind_param(3,$_[$i]);
	  $insth->execute|| mydie $insth->errstr;
	}
	elsif (!defined $_[$i]) {
	  #delete data if data is -99.9, otherwise do nothing.
	  if ($cur_val == -99.9) {
	    print "deleting bad data for $site_datatype_hash{$datatype}, date $datestr, value undefined, cur_val = $cur_val\n";
	    $dbh->do("delete from r_day where site_datatype_id = $site_datatype_hash{$datatype} and date_day = '$datestr'");
	  }
	}
	elsif ($cur_val == $_[$i]) {
	  next;
	}
	elsif ($cur_val != $_[$i]) {
	  print "updating for $site_datatype_hash{$datatype}, date $datestr, value $_[$i], cur_val = $cur_val\n";
	  #update instead of insert
	  $upsth->bind_param(1,$_[$i]);
	  $upsth->bind_param(2,$site_datatype_hash{$datatype});
	  $upsth->bind_param(3,$datestr);
	  $upsth->execute|| mydie $upsth->errstr;
	}
      }
    
    $upsth->finish;
    $insth->finish();
    

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
	$sth->fetch;
      }
    
    $sth->finish();
  }

