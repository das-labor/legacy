<?php

/*

Pixelpost version 1.4.1
RSS picture feed addon version .1
Addon developer: Tilman Frosch

Pixelpost www: http://www.pixelpost.org/

// usage
1) copy the addon to addons folder of Pixelpost

2) Use this tag to show the latest thumbnail of Pixelpost photblog on other pages on the web!
	<img src='http://www.your_photoblog.com/index.php?showthumb=latest&thumbtag=image' />

	you can make a link to it as
	<a href='http://www.your_photoblog.com/' ><img src='http://www.your_photoblog.com/index.php?showthumb=latest&thumbtag=image' /> </a>

	don't forget to replace the http://www.your_photoblog.com/ with you photblog address!


// TODO for future versions:
	Showing title of the latest post as a tool tip on the image


*/

$addon_name = "Pure images as rss feed";
$addon_description = "rss picture feed.";
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
