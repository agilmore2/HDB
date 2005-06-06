# package to generically interface with HDB

=head1 NAME

HDB Generic PERL interface

=head1 VERSION

Documentation for Hdb.pm $ID: $

=head1 SYNOPSIS

To create a DBI handle to HDB as app_user, must have the proper environmental
variables set:

 use Hdb;

 my $hdb = Hdb->new;

 $hdb->connect_to_db($dbname, $user, $pass);

To allow editting of values:

 $hdb->set_approle();

=head1 DESCRIPTION

The Hdb module encapsulates some of the common functionality needed by perl
progams which interact with the database.

=cut


package Hdb;
use Exporter ();
@ISA = qw(Exporter);
@EXPORT = qw(hdbdie connect_to_db set_approle);

use DBI;

#$passw_dbh, $dbh, %objslotsdi, $dbname;

sub new {
  my $proto = shift;
  my $class = ref($proto) || $proto;
  my $self  = {};
  $self->{dbh} = undef;
  $self->{dbname} = undef;
  bless ($self, $class);
  return $self;
}

sub DESTROY
{
  my $self = shift;
  if (defined($self->{dbh})) {
    $self->{dbh}->rollback;
    $self->{dbh}->disconnect;
  }
  undef $self;
}

sub dbh
{
  my $self = shift;
  return $self->{dbh};
}

sub hdbdie {
  my $self = shift;
  if (defined($self->{dbh}) and $self->{dbh}->ping) {
    $self->{dbh}->rollback;
    $self->{dbh}->disconnect;
  }
  die @_;
}

# HDB does something interesting with the app_user account
# app_user has the permission to insert into the database, but only
# if the app_role is enabled, which is not by default
# the applications get around this by connecting to the database as 
# psswd_user (from the environment) and getting the password to 
# turn on this role, and then enabling this role.

sub set_approle {
  my $self = shift;

  unless (defined($self->{dbh}) and $self->{dbh}->ping) {
    print "ERROR: attempted to set app role on unconnected DB handle.\n";
    die @_;
  }

  my $passd_dbh;
  my ($approlepass, $approle);
  my ($psswduser, $psswd_statement, $psswdsth);

  sub passdie {
    $psswd_dbh->disconnect;
    die @_;
  }

  if (!defined($psswduser = $ENV{PSSWD_USER})) {
    die "Environment variable PSSWD_USER not set...\n";
  }
  if (!defined($approle = $ENV{APP_ROLE})) {
    die "Environment variable APP_ROLE not set...\n";
  }

  # create connections, exceptions on error, no autocommit

  $psswd_dbh = DBI->connect("dbi:Oracle:$self->{dbname}", $psswduser,
			    $self->{dbname},
			    {RaiseError => 1, AutoCommit => 0 })
                           or passdie $DBI::errstr;

  $psswd_statement =
     "select psswd from role_psswd where UPPER(ROLE) = UPPER('$approle')";

  $psswdsth = $psswd_dbh->prepare($psswd_statement)
                          or passdie $psswdsth->errstr;
  $psswdsth->execute or passdie $psswdsth->errstr;

  $psswdsth->bind_col(1,\$approlepass);
  $psswdsth->fetch;
  $psswdsth->finish;
  $psswd_dbh->disconnect;

  $self->{dbh}->do("set role $approle identified by $approlepass")
                 or hdbdie $DBI::errstr;

  return $self->{dbh};
}


#this function connects to the database as the specified user
sub connect_to_db {
  my $self = shift;
  $self->{dbname} = $_[0];
  my $user = $_[1];
  my $pass=$_[2];

  $self->{dbh} = DBI->connect("dbi:Oracle:$self->{dbname}", $user, $pass,
			     {RaiseError => 1, AutoCommit => 0 })
                     or hdbdie $DBI::errstr;

  return $self->{dbh};
}

1;

