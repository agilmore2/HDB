#!/usr/bin/perl

use warnings;
use strict;

#use libraries from HDB environment (Solaris only except for HDB.pm)
use lib "$ENV{HDB_ENV}/perlLib/lib";

use Hdb;

use LWP::UserAgent;
use File::Basename;

#Version Information
my $verstring = '$Revision$';
$verstring =~ s/^\$Revision: //;
$verstring =~ s/ \$//;

#check to see command line usage.
my $progname = basename($0);    
chomp $progname;

my ( $accountfile, $hdbuser, $hdbpass, $test, $sitearg, $sdiarg );

#store decodes dir of HDB environment for use
my $decdir = "$ENV{HDB_ENV}/decodes";

main();  #at end of file to allow all subroutines to be prototyped.

# in a loop over the list of sites
# get the rating table and process it.
sub process_coef ($$$) {
  my $hdb   = shift;
  my $site = shift;
  my $sdi = shift;

  my @table = read_table( $site );
  process_rating( $hdb, $sdi, @table );
}

#read the rating file
sub read_table ($$$) {
  my $site    = shift;

  my @table;
  
  my $file = $site . "coeff.dat";
  open INFILE, $file or die "can't open $file! $!";
  my @data = <INFILE>;
  chomp @data;
  
  for (@data) {
    my @row = split /  */;
    push @table, \@row;
  }
  
  return @table;
}

#find rating ids for the three coefficients
# update the ratings from the file table
sub process_rating ($$$) {
  my $hdb      = shift;
  my $sdi     = shift;
  my @table = @_;

  my $rating = find_rating( $hdb, $sdi, 'A0' );    
  if ($rating) {
   update_rating( $hdb, $rating, 1,\@table );
  }
  $rating = find_rating( $hdb, $sdi, 'A1' );    
  if ($rating) {
   update_rating( $hdb, $rating, 2,\@table );
  }
  $rating = find_rating( $hdb, $sdi, 'A2' );    
  if ($rating) {
   update_rating( $hdb, $rating, 3,\@table );
  }
}

#delete old rating data
sub delete_rating_points ($$) {
  my $hdb=shift;
  my $rating=shift;
  
  eval {
    my $sth = $hdb->dbh->prepare(
    "begin
       ratings.delete_rating_points(?);
     end;");
    $sth->bind_param(1,$rating);
    $sth->execute;
  };
 
  if (@$) { #errors
    $hdb->dbh->rollback;
    $hdb->hdbdie("Deletion of rating_id $rating failed! $!\n");
  } else {
    $hdb->dbh->commit;
  }
}

# run through table and write to database.
sub update_rating ($$$) {
  my $hdb  = shift;
  my $rating = shift;
  my $col = shift;
  my $rat  = shift;

  foreach (@$rat) {
      modify_rating_point( $hdb, $rating, $_->[0],$_->[$col]);
  }
}

# attempt to find rating id for site
sub find_rating ($$) {
  my $hdb  = shift;
  my $sdi = shift;
  my $type = shift;
  
  my $rating_id;

  eval {
    $rating_id =
      $hdb->dbh->selectcol_arrayref(
            "select ratings.find_site_rating('ACAPS $type',$sdi,null) from dual")->[0];
  };

  if (@$)    #error occurred
  {
    $hdb->hdbdie("Cannot find rating for sdi $sdi! $!\n");
  }

  return $rating_id;
}

{
my $sth; #make static so prepare statement can be done once.
sub modify_rating_point ($$@) {
  my $hdb    = shift;
  my $rating = shift;

  #rest of data is INDEP, SHIFT, DEP, and STOR from the file. We store in
  # the database just INDEP and DEP, since SHIFT is meaningless with a
  # shift adjusted rating table

  eval {
    if (!defined ($sth)) {
      $sth = $hdb->dbh->prepare( "
         begin ratings.modify_rating_point(?,?,?);
         end;" );
    }
    $sth->bind_param(1,$rating);
    $sth->bind_param(2,$_[0]);    
    $sth->bind_param(3,$_[1]);
    $sth->execute if (!$test);
  };    # semicolon here because of use of eval

  if (@$)    #error occurred
  {
    $hdb->dbh->rollback;
    $hdb->hdbdie(
                "Modification for rating $rating, indep value $_[0] failed!\n");
  } else {
    $hdb->dbh->commit or $hdb->hdbdie( $hdb->dbh->errstr );
  }
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
    } elsif ( $arg =~ /-t/ ) {    # get testing flag
      $test = 1;
    } elsif ( $arg =~ /-i/ ) {    # get site name
      if ( defined($sitearg) ) {
        print STDERR "only one -i site allowed!\n";
        &usage;
      }
      $sitearg = shift;
    } elsif ( $arg =~ /-s/ ) {    # get sdi
      if ( defined($sdiarg) ) {
        print STDERR "only one -s sdi allowed!\n";
        &usage;
      }
      $sdiarg = shift;
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
Script to read in ACAPS files
Example: $progname -a <accountfile> -i <short res name> -s <sdi for Pool Elevation>
$progname -a ~/.hdb_user_file -i powe -s 1928


  -h               : This help
  -v               : Version
  -a <accountfile> : HDB account access file (REQUIRED or both below)
  -u <hdbusername> : HDB application user name (account file or REQUIRED)
  -p <hdbpassword> : HDB password (account file or REQUIRED)
  -t               : Test only, do not alter database
  -s               : SDI for specified site
  -i <fileabbrev>  : Filename abbrev to read
ENDHELP

  exit(1);
}

sub version {
  print "$progname version $verstring\n";
  exit 1;
}

sub main {

  #parse arguments
  process_args(@ARGV);    # uses globals, bad!

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

  process_coef ($hdb, $sitearg,$sdiarg);
}
