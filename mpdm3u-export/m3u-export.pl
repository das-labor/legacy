#!/usr/bin/perl -w

#####
## eventuell auch mal doku schreiben ... wofuer das hier so ist ...
# dann kannman gaanz viel schrieben -- jetzt aber keine lust

use File::Copy;


$musicpath="/data/music1/";
$m3upath="/data/.mpd/playlists/";
$destination="/tmp/";

system("mpc save tmp");

open( $f, $m3upath."tmp.m3u" );

while ( <$f> ) {
	chomp;
	print "Copying ".$_."\n";

	/.+\/([^\/]+)$/;

	$basename = $1;
	$basename =~ tr/ /_/;

#	print $basename."\n";

	$srcfile = $musicpath.$_;
	$dstfile = $destination.$basename;

	copy( $srcfile,  $dstfile );
}

system("mpc rm tmp");
