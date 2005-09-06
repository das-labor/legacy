#!/usr/bin/perl -w
# Filename:	ogg2mp3
# Author:	David Ljung Madison <DaveSource.com>
# See License:	http://MarginalHacks.com/License
# Description:	Converts ogg to mp3, with artist/title/info
# Ideas from:  Joseph E. O'Doherty <odoherty.net>


#### Needed Stuff ####
## Applications: music123/music321 for oggdec, ogginfo. id3tool. lame (or similar. better lame).


use strict;
use File::Spec;

##################################################
# Setup the variables
##################################################
my $PROGNAME = $0;
$PROGNAME =~ s|.*/||;

my $TMPFILE = File::Spec->catfile( File::Spec->tmpdir(), "$PROGNAME.$$.wav" );

#########################
# Ogg decoder
# Different versions of ogg123 handle files differently!  Argh!
# If the first doesn't work, try the second..
#########################
#my $OGGDEC = "ogg123 -d wav -o file:$TMPFILE";
my $OGGDEC = "ogg123 -d wav -f $TMPFILE";

#########################
# MP3 encoder
# Choose your favorite..
#########################
my $MP3ENC = "lame";
#my $MP3ENC = "notlame";

# Choose your options. The given ones are quite failsave for stupid mp3 hardware decoder
my $LAMEOPTS ="-m s -h -b 128";

my $OGGINFO = "ogginfo";
my $MP3INFO = "id3tool";

##################################################
# Usage
##################################################
sub usage {
  foreach my $msg (@_) { print STDERR "ERROR:  $msg\n"; }
print <<USAGE;

Usage:\t$PROGNAME [-d] [-o <dir>] <ogg> ..
\tConverts ogg to mp3
\t-d\tSet debug mode
\t-o\tSpecify output directory

USAGE
  exit -1;
}

sub parse_args {
  my $outdir;
  my @oggs;
  while (my $arg=shift(@ARGV)) {
    if ($arg =~ /^-h$/) { usage(); }
    if ($arg =~ /^-d$/) { $MAIN::DEBUG=1; next; }
    if ($arg =~ /^-o$/) { $outdir=shift @ARGV; next; }
    if ($arg =~ /^-/) { usage("Unknown option: $arg"); }
    push(@oggs, $arg);
  }
  usage("No oggs specified") unless @oggs;

  ($outdir,@oggs);
}

sub debug {
  return unless $MAIN::DEBUG;
  foreach my $msg (@_) { print STDERR "[$PROGNAME] $msg\n"; }
}

##################################################
# Genre code
##################################################
my @GENRES;
open(G,"$MP3INFO -l|") || die("[$PROGNAME] Can't get genre info: [$MP3INFO -l]\n");
while (<G>) {
  push(@GENRES,$1) if (/^(\S.*\S)\s+\|/);
}
close G;

sub get_genre {
  my ($genre) = @_;
  return undef unless $genre;
  my @g = grep(/^$genre$/i, @GENRES);
  return $g[0] if @g;
  @g = grep(/$genre/i, @GENRES);
  print STDERR "[$PROGNAME] Multiple genre matches [$genre -> @g]\n" if $#g>0;
  return $g[0] if @g;
  # No match
  print STDERR "[$PROGNAME] Unknown genre [$genre]\n";
  return undef;
}

##################################################
# Main code
##################################################
sub ogginfo {
  my ($ogg) = @_;

  # Get ogg tags
  open(TAGS,"$OGGINFO \Q$ogg\E |") || die("[$PROGNAME] Couldn't run ogginfo [$ogg]\n");
  my %i;
  while(<TAGS>) {
    chomp;
    next if /^\s*$/;

## The old way:
#    die("[$PROGNAME] Bad ogginfo: [$_]") unless /^\s*(\S+)=(.*)$/;
#    $i{$1} .= $2;

## Old ogginfo:
## "key=value"
##
## New ogginfo:
## "Some comment stuff..."
## "	key=value"
##
## I suppose I could parse the entire output to see if it's new or
## old, but it looks like the comments never have '=' in them, and who
## cares if it does anyways?  So, we'll just use a loose regexp:
    $i{$1} .= $2 if /^\s*(\S+)=(.*)$/;
  }
  close(TAGS);
  \%i;
}

sub set_mp3info {
  my ($mp3,$info) = @_;
  my $set;
  $set .= " -t \Q$info->{title}\E" if $info->{title};
  $set .= " -a \Q$info->{album}\E" if $info->{album};
  $set .= " -r \Q$info->{artist}\E" if $info->{artist};
  $set .= " -n \Q$info->{comment}\E" if $info->{comment};
  my $genre = get_genre($info->{genre});
  $set .= " -G \Q$genre\E" if $genre;
  $set .= " -y \Q$info->{date}\E" if $info->{date} =~ /^\d+$/;
  return print STDERR "[$PROGNAME] No tag info for [$mp3]\n" unless $set;
  system("$MP3INFO $set \Q$mp3\E");
  print STDERR "[$PROGNAME] Errors from:\n  $MP3INFO $set $mp3\n  $!\n" if $?;
}

sub ogg2mp3 {
  my ($outdir,$ogg) = @_;
  my $mp3 = $ogg;
  $mp3 =~ s/(\.ogg)?$/.mp3/i;
  return print STDERR "[$PROGNAME] mp3 already exists: [$mp3]\n" if -f $mp3;

  # Handle outdir if specified
  if ($outdir) {
    $mp3 =~ s|.*/||g;
    $mp3 = "$outdir/$mp3";
  }

  print "$ogg -> $mp3\n";

  # Decode
#  system("$OGGDEC \Q$ogg\E | $MP3ENC - \Q$mp3\E");
#  print STDERR "[$PROGNAME] Errors from:\n  $OGGDEC \Q$ogg\E | $MP3ENC - \Q$mp3\E\n  $!\n"
#    if $?;
  system("$OGGDEC \Q$ogg\E");
  print STDERR "[$PROGNAME] Errors from:\n  $OGGDEC \Q$ogg\E\n  $!\n" if $?;

  # Encode
  system("$MP3ENC $LAMEOPTS $TMPFILE \Q$mp3\E");
  print STDERR "[$PROGNAME] Errors from:\n  $MP3ENC $TMPFILE \Q$mp3\E\n  $!\n" if $?;
  unlink $TMPFILE;


  my $info = ogginfo($ogg);
  set_mp3info($mp3,$info);
}

sub main {
  my ($outdir,@oggs) = parse_args();

  map(ogg2mp3($outdir,$_), @oggs);
}
main();


