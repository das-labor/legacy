#!/usr/bin/perl

package MusicLinks;
use strict;
use Net::Amazon;
use CGI::Enurl;
use Net::Amazon::Request::Keyword;

my $ua = Net::Amazon->new( token => 'D2L55VST3FUMTH' );

my %amazon_cache; # we cache so we don't nail amazon as often
my %amazon_notfound;
my @cache_fields = ('url_smallcover', 'url_medcover',
                    'url_largecover','tracklist');

my $total_objs=0;
my $cache_size=0;
my $total_queries=0;
my $failed_queries=0;
my $notfound_queries=0;
my $cached_queries=0;
my $cache_inserts=0;

sub get_cache { return \%amazon_cache; }
sub cache_stats
{
    my($self, %options) = @_;
    my %result = (
      'Size'    => $cache_size,
      'Objects' => $total_objs,

      'Queries' => $total_queries,
      'Hits'    => $cached_queries,
      'Inserts' => $cache_inserts,
      'Failed'  => $failed_queries,
      'Notfound' => $notfound_queries,
    );
    return \%result;
}

sub new
{
    my($self, %options) = @_;
    set($self,%options);
    $self = { 'state' => 'new'};
    bless($self);
    return $self;
}

sub set
{
    my($self, %options) = @_;
    $total_objs++;
    my $baseurl =qq!<a href="http://www.allmusic.com/cg/amg.dll?p=amg&sql=!;
    $self->{state} = 'set';
    $self->{artist} = '';
    $self->{album}  = '';
    $self->{url_allmusicartist} = '';
    $self->{url_allmusicalbum} = '';
    $self->{url_smallcover} = '';
    $self->{url_medcover} =  '';
    $self->{url_largecover} =  '';
    $self->{tracklist} = '';

    if ( (exists $options{artist}) && ($options{artist}) ) {
      $self->{artist} = $options{artist};
      $self->{url_allmusicartist} = $baseurl. enURL($self->{artist});
      $self->{url_allmusicartist} .= qq!&opt1=1&x=1&y=1">!;
    }

    if ( (exists $options{album}) && ($options{album}) ) {
      $self->{album} = $options{album};
      $self->{url_allmusicalbum} = $baseurl . enURL($self->{album});
      $self->{url_allmusicalbum} .= qq!&opt1=2&x=1&y=1">!;
    }
    return $self;
}

sub get_amazoninfo {
  my ($self,%options) = @_;
  return 0 if (! defined $self->{artist});
  return 0 if (! defined $self->{album});
  return 0 if (! $self->{artist});
  return 0 if (! $self->{album});
  $total_queries++;
  return 2 if (is_cached($self));

  # feeble attempt at sanitizing artist and album
  #
  my $q_artist = lc($self->{artist});
  my $q_album = lc($self->{album});
  $q_album =~ s/\s+\-\s+/ /;
  $q_album =~ s/disc (one|two|three|four|five)//;
  map (s/^\s*//g, ($q_album, $q_artist));
  map (s/\s+/ /g, ($q_album, $q_artist));
  #map (s/the/ /g, ($q_artist));
  map (s/(,|\.|\;|:|\")//g, ($q_album, $q_artist));

  my $req = Net::Amazon::Request::Keyword->new( 
      keyword => "$q_artist $q_album",
      mode     => 'music'
  );

  my $resp = $ua->request($req);
  if(! $resp->is_success()) {
    $failed_queries++;
    $amazon_notfound{"$self->{artist}\t$self->{album}"} = 2;
    return 0;
  }
  my $bestmatch='';

  for my $prop ($resp->properties) {
    my $result_artists = join("/",$prop->artists());
    if ( ($prop->album =~ /$q_album/i) &&
         ($result_artists =~ /$q_artist/i) ) {
      $bestmatch=$prop;
      last;
    }
  }

  if ($bestmatch) {
      $self->{url_smallcover} = $bestmatch->ImageUrlSmall();
      $self->{url_medcover} =  $bestmatch->ImageUrlMedium();
      $self->{url_largecover} =  $bestmatch->ImageUrlLarge();
      $self->{tracklist} = join("\t",($bestmatch->tracks()));
      #my @trklist;
      # for my $track ($bestmatch->tracks()) { push(@trklist,$track); }
      #$self->{tracklist} = \@trklist;

      # add to cache
      for my $field (@cache_fields) {
        $amazon_cache{"$self->{artist}\t$self->{album}\t$field"} = 
          $self->{$field};
      }
      $cache_size++;
      $cache_inserts++;
      return 1;
  } else {
      $notfound_queries++;
      $amazon_notfound{"$self->{artist}\t$self->{album}"} = 1;
      return 0;
  }
}

sub load_amazoncache {
  my ($self,%options) = @_;

  my $filename='';
  $filename = $options{file} if(exists $options{file});
  return 0 if (! $filename);
  my $ok = open(INFILE,"$filename");
  if (!$ok) {
    warn "Unable to load_amazoncache! $!!\n";
    return 0;
  }
  while (<INFILE>) {
    chop;
    if (/^#/) {
      s/^#//;
      my @fields=split(/\t/,$_);
      $amazon_notfound{"$fields[0]\t$fields[1]"} = $fields[2];
      next;
    }
    my @fields=split(/\t/,$_);
    $amazon_cache{"$fields[0]\t$fields[1]\t$fields[2]"} = join("\t",@fields[3 .. $#fields]);
  }
  close INFILE;
  $cache_size=keys %amazon_cache;
  return 1;
}

sub save_amazoncache {
  my ($self,%options) = @_;
  my $filename='';
  $filename = $options{file} if(exists $options{file});
  return 0 if (! $filename);
  my $ok;
  if(exists $options{overwrite}) {
    $ok = open(OUTFILE,">$filename");
  } else {
    $ok = open(OUTFILE,">>$filename");
  }
  if (!$ok) {
    warn "Unable to save_amazoncache! $!!\n";
    return 0;
  }
  for my $item (keys %amazon_cache) {
    my $val=$amazon_cache{$item};
    print OUTFILE "$item\t$val\n";
  }
  for my $item (keys %amazon_notfound) {
    my $val=$amazon_notfound{$item};
    print OUTFILE "#$item\t$val\n";
  }
  close OUTFILE;
  return 1;
}

sub is_cached {
  my ($self,%options) = @_;
  my $bad=0;
  return 0 if (! defined $self->{artist});
  return 0 if (! defined $self->{album});
  return 0 if (! $self->{artist});
  return 0 if (! $self->{album});

  for my $field (@cache_fields) {
    my $cachevalue=undef;
    $cachevalue = 
       (exists($amazon_cache{"$self->{artist}\t$self->{album}\t$field"})) ?
         ($amazon_cache{"$self->{artist}\t$self->{album}\t$field"}) :
         undef;
    if (defined($cachevalue)) { $self->{$field} = $cachevalue; }
    else { $bad++; last; }
    #print STDERR "Cache OK for $field\n";
  }
  if ($bad) {
    for my $field (@cache_fields) { undef $self->{$field}; }
    return 0;
  }
  $cached_queries++;
  return 1;
}

1;
