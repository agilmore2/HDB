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

Look up ids from database:

 $hdb->get_app_ids(\%nameid);

 Argument is a hash of hashes, with values for the key 'name' for the keys:
 agen, collect, load_app, method, computation;

To get at the DBI handle, 
 my $dbh = Hdb->dbh;

=head1 DESCRIPTION

The Hdb module encapsulates some of the common functionality needed by perl
progams which interact with the database.

=cut

package Hdb;
use Exporter ();
@ISA = qw(Exporter);
@EXPORT = qw(hdbdie connect_to_db set_approle get_app_ids get_SDI);

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

sub get_app_ids
{
# Get ids to describe where data came from
  my $self = shift;

  my $nameid = $_[0];

  my $max_len = 11;

  my $sth;

  $sth = $self->dbh->prepare(q{
     BEGIN
           lookup_application(?,?,?,?,?,  /* agen, collect, load_app, method, computation names*/
                              ?,?,?,?,?); /* agen, collect, load_app, method, computation ids */
     END;
   });

  $sth->bind_param(1,$nameid->{agen}->{name});
  $sth->bind_param(2,$nameid->{collect}->{name});
  $sth->bind_param(3,$nameid->{load_app}->{name});
  $sth->bind_param(4,$nameid->{method}->{name});
  $sth->bind_param(5,$nameid->{computation}->{name});
  $sth->bind_param_inout(6, \$nameid->{agen}->{id}, $max_len);
  $sth->bind_param_inout(7, \$nameid->{collect}->{id}, $max_len);
  $sth->bind_param_inout(8, \$nameid->{load_app}->{id}, $max_len);
  $sth->bind_param_inout(9, \$nameid->{method}->{id}, $max_len);
  $sth->bind_param_inout(10, \$nameid->{computation}->{id}, $max_len);

  eval {
    $sth->execute;
  };

  if ($@ or
      !defined($nameid->{agen}->{id}) or
      !defined($nameid->{collect}->{id}) or
      !defined($nameid->{load_app}->{id}) or
      !defined($nameid->{method}->{id}) or
      !defined($nameid->{computation}->{id})) { # something screwed up
    print $self->dbh->errstr, " $@\n";
    hdbdie "Errors occurred during selection of ids for application.\n";
  }

  return $nameid;
}

sub get_SDI
{
  my ($cur_site, $datatype) = $_[0,1];
  my ($sth, $site_datatype);

  my $get_site_datatype_statement = "select site_datatype_id from hdb_site_datatype where site_id = ? and datatype_id = ? ";

  $sth = $hdb->dbh->prepare($get_site_datatype_statement) || die $sth->errstr;
  $sth->bind_param(1,$cur_site);
  $sth->bind_param(2,$datatype);
  $sth->execute || hdbdie $sth->errstr;
  $sth->bind_col(1,\$site_datatype);
#should check here if there are any rows. if not, no SDI exists for this combo.
  $sth->fetch;

  if (!defined($site_datatype)) {
    hdbdie "site datatype id not found for site: $cur_site datatype: $datatype");
  }

  $sth->finish;

  return $site_datatype;

}

1;

