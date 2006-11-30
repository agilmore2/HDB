# package to generically interface with HDB

=head1 NAME

HDB Generic PERL interface

=head1 VERSION

Documentation for Hdb.pm $Id$

=head1 SYNOPSIS

To create a DBI handle to HDB:

 use Hdb;

 my $hdb = Hdb->new;

 $hdb->connect_to_db($dbname, $user, $pass);

Different version of the above, using connect info from a file.

 $hdb->connect_from_file($filename);

To allow editting of values, must have appropriate environment variables:

 $hdb->set_approle();

Look up ids from database by name. Argument is a hash of hashes, with values
for the key 'name' for the keys: agen, collect, load_app, method, computation:

  my $nameid;

  $nameid->{agen}->{name} = 'Colorado Division of Water Resources';
  $nameid->{collect}->{name} = '(see agency)';
  $nameid->{load_app}->{name} = 'codwr2hdb.pl';
  $nameid->{method}->{name} = 'unknown';
  $nameid->{computation}->{name} = 'unknown';

  $hdb->get_app_ids($nameid);

  $agen_id = $nameid->{agen}->{id};
  $collect_id = $nameid->{collect}->{id};
  $load_app_id = $nameid->{load_app}->{id};
  $method_id = $nameid->{method}->{id};
  $computation_id = $nameid->{computation}->{id};

To get at the DBI handle:

 my $dbh = Hdb->dbh;

=head1 DESCRIPTION

The Hdb module encapsulates some of the common functionality needed by perl
progams which interact with the database.

=cut

package Hdb;
use Exporter ();
@ISA = qw(Exporter);
@EXPORT = qw();

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

=item hdbdie()

Call hdbdie to rollback uncommitted database changes and disconnect from the database:

 $hdb->hdbdie("Error occurred, rolling back changes\n");

=cut

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
                 or $self->hdbdie($DBI::errstr);

  return $self->{dbh};
}

#this function connects to the database as the specified user
sub connect_to_db {
  my $self = shift;
  if (defined($self->{dbh})) {
    $self->hdbdie("Attempted to connect while already connected to $self->{dbname}!\n");
  }

  $self->{dbname} = $_[0];
  my $user = $_[1];
  my $pass=$_[2];

  $self->{dbh} = DBI->connect("dbi:Oracle:$self->{dbname}", $user, $pass,
			     {RaiseError => 1, AutoCommit => 0 })
                     or $self->hdbdie($DBI::errstr);

  return $self->{dbh};
}

sub connect_from_file {
  use Fcntl ':mode';

  my $self = shift;
  if (defined($self->{dbh})) {
    $self->hdbdie("Attempted to connect while already connected to $self->{dbname}!\n");
  }

#check that user can read the file
  $filename = $_[0];
  if (! -r $filename) {
    die ("$filename is not readable!\n")
  }

 open ACCESS, $filename;
# check that the file is not readable from anyone but user
  my $mode = (stat(ACCESS))[2];
  if ($mode & (S_IRGRP | S_IROTH) ) {
    die ("$filename has incorrect permissions! Should not be readable by group or others\n");
  }

=item ACCOUNT FILE FORMAT


 should have three lines:

 username <username>
 password <password>
 database <databasename>

Will fail if file contains anything else.

=cut

  while (<ACCESS>) {
    if (s/^username //) {
      chomp;
      $user = $_;
    } elsif (s/^password //) {
      chomp;
      $pass = $_;
    } elsif (s/^database //) {
      chomp;
      $database = $_;
    } else {
      die ("Unrecognized line in access file: $_");
    }
  }
  close ACCESS;

  if (!defined($user) || !defined($pass) || !defined($database)) {
    die ("database account file has incorrect format!\nexpecting user pass database\n");
  }

  return $self->connect_to_db($database,$user,$pass);
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
    $self->hdbdie("Errors occurred during selection of ids for application.\n");
  }

  return $nameid;
}


=item get_SDI

Get a site_datatype_id from the database, given site_id and datatype_id.
Meant as an example, not really all that useful.


 my $sdi = $hdb->get_SDI($site_id,$datatype_id);

=cut

sub get_SDI
{
  my $self = shift;
  my ($cur_site, $datatype) = @_[0,1];
  my ($sth, $site_datatype);

  my $get_site_datatype_statement = "select site_datatype_id from hdb_site_datatype where site_id = ? and datatype_id = ? ";

  $sth = $self->dbh->prepare($get_site_datatype_statement) || die $sth->errstr;
  $sth->bind_param(1,$cur_site);
  $sth->bind_param(2,$datatype);
  $sth->execute || $self->hdbdie($sth->errstr);
  $sth->bind_col(1,\$site_datatype);
#should check here if there are any rows. if not, no SDI exists for this combo.
  $sth->fetch;

  if (!defined($site_datatype)) {
   $self->hdbdie("site datatype id not found for site: $cur_site datatype: $datatype");
  }

  $sth->finish;

  return $site_datatype;

}

1;

