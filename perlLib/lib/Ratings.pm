

# update the description and delete the old rating points
sub update_rating_info ($$$) {
  my $hdb    = shift;
  my $rating = shift;
  my $desc   = shift;

  eval {
    my $sth = $hdb->dbh->prepare(
    "begin
       ratings.update_rating_desc (?,?);
     end;");
    $sth->bind_param(1,$rating);
    $sth->bind_param(2,substr($desc,0,1000)); #description in DB has 1000 char limit
    $sth->execute;
  };    # semicolon here because of use of eval

  if (@$)    #error occurred
  {
    $hdb->dbh->rollback;
    $hdb->hdbdie("Update of rating id $rating failed!\n");
  } else {
    $hdb->dbh->commit or $hdb->hdbdie( $hdb->dbh->errstr );
  }
}

# make a new site rating record
sub create_site_rating ($$$$) {
  my $hdb  = shift;
  my $site = shift;
  my $rating_type = shift;
  my $data_source = shift;

  my $rating_id;

  # get data for creation
  my ( $agen, $sdi );
  ($agen,$sdi) = find_agen_sdi( $hdb, $site, $rating_type, $data_source);
  
  eval {
    my $sth = $hdb->dbh->do( "
      begin ratings.create_site_rating($sdi,
            '$rating_type',null,null,$agen,null);
      end;" );

    $rating_id =
      $hdb->dbh->selectcol_arrayref(
            "select ratings.find_site_rating('$rating_type',$sdi,null) from dual")->[0];
  };    # semicolon here because of use of eval

  if ( @$ or !defined($rating_id) )    #error occurred
  {
    $hdb->dbh->rollback;
    $hdb->hdbdie("Creation of rating failed for site $site! $@\n");
  } else {
    $hdb->dbh->commit or $hdb->hdbdie( $hdb->dbh->errstr );
  }

  return $rating_id;
}


# attempt to find rating id for site
sub find_rating ($$$$) {
  my $hdb  = shift;
  my $site = shift;
  my $rating_type = shift;
  my $data_source = shift;
    
  my $rating_id;

  my $sdi; 
  ( undef, $sdi ) = find_agen_sdi( $hdb, $site, $rating_type, $data_source);

  eval {
    $rating_id =
      $hdb->dbh->selectcol_arrayref( 
            "select ratings.find_site_rating('$rating_type',$sdi,null) from dual")->[0];
  };

  if (@$)    #error occurred
  {
    $hdb->hdbdie("Query of site $site for ratings failed! $!\n");
  }

  return $rating_id;
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
    $sth->execute;
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

# find agency and sdi from ext_data_map
# FIXME: decoded telemetry site list instead
sub find_agen_sdi ($$$$) {
  my $hdb  = shift;
  my $site = shift;
  my $rating_type = shift;
  my $data_source = shift;

  my $querysql = "select data.agen_id, sdisb.site_datatype_id
  from ref_ext_site_data_map map, hdb_ext_data_source data,
  hdb_site_datatype sdisa, hdb_site_datatype sdisb,
  hdb_rating_type rat_typ
  where map.primary_site_code = '$site' and
  data.ext_data_source_id = map.ext_data_source_id and
  data.ext_data_source_name = '$data_source' and
  map.hdb_site_datatype_id = sdisa.site_datatype_id and
  sdisb.site_id = sdisa.site_id and
  sdisb.datatype_id = rat_typ.indep_datatype_id and
  rat_typ.rating_type_common_name = '$rating_type'"; 

  my ( $agen_id, $sdi);

  eval {
    my $result = $hdb->dbh->selectall_arrayref($querysql);
# a bit hairy here, see DBI docs for return value
    ( $agen_id, $sdi) = @{$result->[0]};
  };

  if (@$)    #error occurred
  {
    $hdb->hdbdie("Query of site $site for ext_data_map failed!\n");
  }

  return ( $agen_id, $sdi );
}

# compare rating from web with db
# more expensive than comparison with filesystem, but best to make sure
# database is in sync
# format of newrat is:
# ref to array of references to arrays of (x,y)
sub compare_rating ($$$) {    #returns 1 if arrays are equal, 0 if not
  my $hdb    = shift;
  my $rating    = shift;
  my @newrat = @{shift()};
  
  
  my $dbrat;
  #retrieve entire rating from db
  
  eval {
    $dbrat = $hdb->dbh->selectall_arrayref("
    select * from ref_rating where rating_id = $rating
     order by independent_value");
  };
  
  if (@$) {
    $hdb->dbh->rollback;
    $hdb->hdbdie("Failed to select rating table $rating from db! $!\n")  
  }
  
  # after perldoc -f splice
  return 0 unless @newrat == @$dbrat;    # same len?
  while (@newrat) {
    my ($db_indep, $db_dep) = @{pop(@$dbrat)}[1,2]; # hoorah for perl ref and array syntax!
    my ($web_indep, $web_dep) = @{pop(@newrat)}; # same!
    return 0 if $web_indep != $db_indep and $web_dep != $db_dep; #possible floating point issues
  }
  return 1;
}


1;