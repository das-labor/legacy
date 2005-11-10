#!/usr/bin/perl -w

#####
## m3u-export is inteded to be a little helper script for mpd, the Music Player Daemon (http://www.musicpd.org/).
## I wanted to listen to the same music on my mobile mp3 player that I was just listening to on my mpd at home.
## This scipt is written in perl because of its better regexp handling and is released under the GNU GENERAL PUBLIC LICENSE (http://www.gnu.org/copyleft/gpl.html).
## IMPORTANT: This script can produce *a lot* of data! Configure it properly and use it at your own risk!
## 
## Needed Files:
## Perl module File::Copy, Getopt::Long
## mpc, the scriptable commandline client for mpd (http://www.musicpd.org/mpc.shtml)
##
## For ogg to mp3 conversion you also need the ogg2mp3 script by David Ljung Madison <DaveSource.com> 
## which can also be found here (https://roulette.das-labor.org/svn/).
## I did a little modification to let it pass some options to lame, so my mp3 player is able to play the resulting mp3s. Also added some documentation
## The original script can be found here: http://marginalhacks.com/bin/ogg2mp3
##
##
## How to configure:
## 1. Set $musicpath as the path to the folder where the media files mpd accesses are stored.
## 2. Set $m3upath as the path to the folder where mpd stores its m3u playlists
## 3. Set $destination as the path to your mounted mp3-player (like /media/usb-storage/ or something)
## 4. chmod 755 this file (m3u-export.pl).
## 5. Execute this scipt as a user allowed to use mpc, to read the playlists and to write to the destination folder

use File::Copy;
use Getopt::Long;

my $help = '';   # option variable with default value (false)
my $short = '';       # option variable with default value (false)
my $oc = '';	# option variable with default value (false)
GetOptions ('help' => \$help, 'short' => \$short, 'oc' => \$oc );

if ($help == 1){
	print "Usage: m3u-export.pl for normal mode, m3u-export.pl --short for exporting files to the solid old 8.3 format and m3u-export --oc for ogg conversion to mp3\n\n";
}
else {

$musicpath="/data/Music/"; #path to mp3 files
$m3upath="/data/Music/.mpd/playlists/"; #path to mpd's m3u playlists
$destination="/data/tmp/"; # destination directory of the media files


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

### 8.3 format
	if ($short == 1){
		$basename =~ s/(.mp3|.MP3|.Mp3|.mP3)+$//;
		$basename = substr($basename,0 , 3);
		$basename = $basename.".mp3";

	}
### end 8.3 format  

#	print $basename."\n";
	
	$srcfile = $musicpath.$_;
	$dstfile = $destination.$num."_".$basename;



### ogg convert choice
### FIX ESCAPING!! Characters like ' go *boang* with the bash!

	if ($oc == 1) {
		if ($basename =~ m/.ogg+$/i) {
			$oggtmpfile = "/tmp/".$num."_".$basename;
			print "Conversion of ".$oggtmpfile."\n";
			
			copy( $srcfile,  $oggtmpfile );
			system("ogg2mp3.pl -o $destination $oggtmpfile");
			print "\n\n\n".$oggtmpfile."\n\n\n";
			unlink $oggtmpfile;
		}
		else {
			copy( $srcfile,  $dstfile );
		}
	}
	else {

### end ogg convert

#	print $basename."\n";
#	
#	$srcfile = $musicpath.$_;
#	$dstfile = $destination.$num."_".$basename;

		copy( $srcfile,  $dstfile );
	}
### end ogg convert choice
}

system("mpc rm tmp");

}
