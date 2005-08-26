#!/usr/bin/perl -w

#####
## m3u-export is inteded to be a little helper script for mpd, the Music Player Daemon (http://www.musicpd.org/).
## I wanted to listen to the same music on my mobile mp3 player that I was just listening to on my mpd at home.
## This scipt is written in perl because of its better regexp handling and is released under the GNU GENERAL PUBLIC LICENSE (http://www.gnu.org/copyleft/gpl.html).
## IMPORTANT: This script can produce *a lot* of data! Configure it properly!
## 
## Needed Files:
## Perl module File::Copy
## mpc, the scriptable commandline client for mpd (http://www.musicpd.org/mpc.shtml)
##
## How to configure:
## 1. Set $musicpath as the path to the folder where the media files mpd accesses are stored.
## 2. Set $m3upath as the path to the folder where mpd stores its m3u playlists
## 3. Set $destination as the path to your mounted mp3-player (like /media/usb-storage/ or something)
## 4. chmod 755 this file (m3u-export.pl).
## 5. Execute this scipt as a user allowed to use mpc, to read the playlists and to write to the destination folder

use File::Copy;


$musicpath="/MusicPath/"; #path to mp3 files ending with a '/'
$m3upath="/MusicPath/.mpd/playlists/"; #path to mpd's m3u playlists ending with a '/'
$destination="/tmp/"; # destination directory of the media files ending with a '/'

system("mpc save tmp");

open( $f, $m3upath."tmp.m3u" );
$i = 0;
while ( <$f> ) {
	$i++;
	if($i < 1000){
	  $num = "0".$i;
	  if($i < 100){
	    $num = "00".$i;
	    if($i < 10){
	 	  $num = "000".$i;
	    }
	  }
	}
	else {
	  $num = $i;
	}
	chomp;
	print "Copying file number ".$num.": ".$_."\n";

	/.+\/([^\/]+)$/;

	$basename = $1;
	$basename =~ tr/ /_/;

#	print $basename."\n";
	
	$srcfile = $musicpath.$_;
	$dstfile = $destination.$num."_".$basename;

	copy( $srcfile,  $dstfile );
}

system("mpc rm tmp");
