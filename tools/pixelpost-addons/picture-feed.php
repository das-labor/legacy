<?php

/*

Pixelpost version 1.4.1
RSS picture feed addon version 0.3
Addon developer: Tilman Frosch

Pixelpost www: http://www.pixelpost.org/

// usage
1) copy the addon to addons folder of Pixelpost

2) Use this URL to access the rss-feed containing the direct links to your images
	http://www.your_photoblog.com/index.php?x=picrss

This addon is meant for easy scripting things like rss-based picture screensavers, slideshows and so on


*/

$addon_name = "RSS Picture Feed";
$addon_description = "Pure images as rss feed.";
$addon_version = "0.3";


if($_GET['x'] == "picrss") {

   pullout($cfgrow['sitetitle']);
    $output = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
    <rss version=\"2.0\">
    <channel>
    <title>".$cfgrow['sitetitle']."</title>
    <link>".$cfgrow['siteurl']."</link>
    <description>".$cfgrow['sitetitle']." photoblog</description>
    <docs>http://blogs.law.harvard.edu/tech/rss</docs>
    <generator>pixelpost</generator>
    ";
   $tzoner = $cfgrow['timezone'];
   $tprefix = '+';
   $tzoner = sprintf ("%01.2f", $tzoner);
   if (substr($tzoner,0,1)=='-') {
      $tzoner = (substr($tzoner,1));
      $tprefix = '-';
      }
   if ($tzoner < 10){$tzoner = "0".$tzoner;}
   $hh = substr($tzoner,0,2);
   $mm = substr($tzoner,-2);
   $tzoner = $tprefix.$hh.$mm;
   $query = mysql_query("select id,datetime,headline,body,image from ".$pixelpost_db_prefix."pixelpost where (datetime<='$cdate') order by datetime desc limit 10");
   while(list($id,$datetime,$headline,$body,$image) = mysql_fetch_row($query)) {
        $headline = pullout($headline);
        $body = pullout($body);
        $image_name = $image;
   	// filename
   	$filename = 'images/'.$image_name;	
	
        $datetime = strtotime($datetime);
        $datetime =  date("D, d M Y H:i",$datetime);
        $datetime .= ' ' .$tzoner;
        $body = stripslashes($body);
        $body = str_replace("\n","&lt;br /&gt;",$body);
        // added to remove HTML tags
        $body = strip_tags( $body );
        $output .= "
        <item>
        <title>$headline</title>
        <link>".$cfgrow['siteurl']."$filename</link>
        
        <pubDate>$datetime</pubDate>
        </item>
        ";
        }
    $output .= "
        </channel>
        </rss>";
    header("Content-type:application/xml");
    echo $output;
    exit;
    }
?>
