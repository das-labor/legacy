<?php
# RSS-Feed Mediawiki extension
# 
# original by mutante 25.03.2005
# extended by Duesentrieb 30.04.2005
# extended by Rdb78 07.07.2005
# extended by Mafs  10.07.2005, 24.07.2005
#
# Requires: 
#  * magpie rss parser <http://magpierss.sourceforge.net/>
#  * iconv <http://www.gnu.org/software/libiconv/>, see also <http://www.php.net/iconv>
#
# Installation:
#  * put this file (rss.php) into the extension directory of your mediawiki installation 
#  * add the following to the end of LocalSettings.php: include("extensions/rss.php");
#  * make sure magpie can be found by PHP.
#
# Usage:
#  Use one section between <rss>-tags for each feed. The rss section may contain parameters
#  separated by a pipe ("|"), just like links and templates. These parameters are supported:
#
#    * charset=...             The charset used by the feed. iconv is used to convert this.
#    * short                   Do not show the description text for each news item.
#    * max=x                   Shows x most recent headlines.
#    * highlight= term1 term2  The terms separated by a space are highlighted.
#    * filter= term1 term2     Show only rss items containing at least one of the terms.
#    * reverse                 display the rss items in reverse order
#
# Example: 
#    <rss>http://slashdot.org/slashdot.rss|charset=UTF-8|short|max=5</rss>
#

if( !defined( 'MEDIAWIKI' ) ) {
        die();
}

$wgExtensionCredits['other'][] = array(
        'name' => 'rss feed extension',
        'author' => 'mutante, Duesentrieb, Rdb, Mafs',
        'version' => '09/2005',
        'url' => 'http://meta.wikimedia.org/wiki/User:Mafs/RSS',
        'description' => 'displays an rss feed on a wiki page'
); 

#change this according to your magpie installation!
require_once('../../stuff/magpierss/rss_fetch.inc'); 

#install extension hook
$wgExtensionFunctions[] = "wfRssExtension"; 

#extension hook callback function
function wfRssExtension() { 
   global $wgParser;
   
   #install parser hook for <rss> tags
   $wgParser->setHook( "rss", "renderRss" );
}
 
#parser hook callback function
function renderRss( $input ) {
    global $wgOutputEncoding;
    $DefaultEncoding = "ISO-8859-1";
   $DisableCache = true;
   
   # $input = mysql_escape_string($input);
   
   if (!$input) return ""; #if <rss>-section is empty, return nothing
   
   #parse fields in rss-section
   $fields= explode("|",$input);
   $url= @$fields[0];
   
   $args= array();
   for ($i=1; $i<sizeof($fields); $i++) {
       $f= $fields[$i];
       
       if (strpos($f,"=")===False) $args[strtolower(trim($f))]= False;
       else {
               list($k,$v)= explode("=",$f,2);
        if (trim($v)==False) $args[strtolower(trim($k))] = False; 
        else $args[strtolower(trim($k))]= trim($v);
       }
   }

   #get charset from argument-array    
   $charset= @$args["charset"];
   if (!$charset) $charset= $DefaultEncoding;
    #get max number of headlines from argument-array
   $maxheads = @$args["max"];
   $headcnt = 0;
   
   #get short-flag from argument-array
   #if short is set, no description text is printed
   if (isset($args["short"])) $short = True; else $short = False;
    #get reverse-flag from argument-array
   if (isset($args["reverse"])) $reverse = True; else $reverse = False;
    #get highlight terms from argument-array    
   $rssHighlight= @$args["highlight"];
   $rssHighlight= str_replace("  "," ", $rssHighlight);
   $rssHighlight= explode(" ", trim($rssHighlight));
   
   #get filter terms from argument-array    
   $rssFilter= @$args["filter"];
   $rssFilter= str_replace("  "," ", $rssFilter);
   $rssFilter= explode(" ", trim($rssFilter));    
   
   #fetch rss. may be cached locally.
   #Refer to the documentation of magpie for details.
   $rss = @fetch_rss($url);
   
   
   #check for errors.
   if ($rss->ERROR) {
       return "<div>Failed to load RSS feed from $url: ".$rss->ERROR."</div>"; #localize...
   }
   
   if (!is_array($rss->items)) {
       return "<div>Failed to load RSS feed from $url!</div>"; #localize...
   }

   #Bild title line    
   $title= iconv($charset,$wgOutputEncoding,$rss->channel['title']);
   if ($rss->channel['link']) $title= "<a href='".$rss->channel['link']."'>$title</a>";
   
   $output="<h3>$title</h3>";
    if ($reverse) $rss->items = array_reverse($rss->items);
    $description = False; 
   foreach ($rss->items as $item) {
if ($item['description']) {$description = True; break;}
   }
    #Bild items
   if (!$short and $description) { #full item list
        $output.="<dl>";
       foreach ($rss->items as $item) {

        $d_text = true;
        $d_title = true;

               $href = trim(iconv($charset,$wgOutputEncoding,$item['link']));
               $title = trim(iconv($charset,$wgOutputEncoding,$item['title']));
        
        $d_title = wfRssFilter ($title, $rssFilter);
        $title= wfRssHighlight($title, $rssHighlight);
        
        #bild description text if desired
               if ($item["description"]) {
                       $text= trim(iconv($charset,$wgOutputEncoding,$item['description']));
                        #avoid pre-tags
                       $text= str_replace("\r"," ",$text);
                       $text= str_replace("\n"," ",$text);
                       $text= str_replace("\t"," ",$text);
                
                                        $d_text = wfRssFilter ($text, $rssFilter);
                                        $text= wfRssHighlight($text, $rssHighlight);
                
                                        $display = $d_text or $d_title;
                
               }
                        else   {
                                        $text = "";
                                        $display = $d_title;
                        }       
                if ($display) {
                $output.="<dt><a href='$href'>$title</a></dt>";
                if ($text) $output.="<dd>$text</dd>\n";
        }
        #Cut off output when maxheads is reached:
       if (++$headcnt == $maxheads)  break;
       
       }
       $output.="</dl>";
   }
   else { #short item list
       $output.="<ul>";
       foreach ($rss->items as $item) {
               $href = trim(iconv($charset,$wgOutputEncoding,$item['link']));
               $title = trim(iconv($charset,$wgOutputEncoding,$item['title']));
        
                        $d_title = wfRssFilter ($title, $rssFilter);
                        $title= wfRssHighlight($title, $rssHighlight);
                                if ($d_title ) $output.="<li><a href='$href'>$title</a></li>";
                #Cut off output when maxheads is reached:
               if (++$headcnt == $maxheads)  break;
       }
       $output.="</ul>";
   }

   if ($DisableCache) {
   
        global $wgVersion;
   
        # Do not cache this wiki page.
        # for details see http://public.kitware.com/Wiki/User:Barre/MediaWiki/Extensions
        global $wgTitle, $wgDBprefix;
        $ts = mktime();
        $now = gmdate("YmdHis", $ts + 120);
        $ns = $wgTitle->getNamespace();
        $ti = wfStrencode($wgTitle->getDBkey());

        $version = preg_replace("/^([1-9]).([1-9]).*/", "\\1\\2", $wgVersion);
        if ($version>14) $sql = "UPDATE $wgDBprefix"."page SET page_touched='$now' WHERE page_namespace=$ns AND page_title='$ti'";
        else             $sql = "UPDATE $wgDBprefix"."cur SET cur_touched='$now' WHERE cur_namespace=$ns AND cur_title='$ti'";

        wfQuery($sql, DB_WRITE, "");
   }
    return $output;
 }

 function wfRssFilter ($text, $rssFilter) {

        $display = true;
        if (is_array($rssFilter)) {
        foreach($rssFilter as $term) {
                
                if ($term) {
                        $display = false;
                        if (preg_match("|$term|i", $text, $a))  {  $display = true; return $display; }
                }
        if ($display) break;
        }
}
return $display;
}
 
function wfRssHighlight($text, $rssHighlight) {

$i=0;
$starttag = "v8x5u3t3u8h";
$endtag   = "q8n4f6n4n4x";

$color[]="coral";
       $color[]="greenyellow";
       $color[]="lightskyblue";
       $color[]="gold";
       $color[]="violet";
        $count_color = count($color);

if (is_array($rssHighlight)) {
        foreach($rssHighlight as $term) {
                if ($term) {
                        $text = preg_replace("|\b(\w*?".$term."\w*?)\b|i", "$starttag"."_".$i."\\1$endtag", $text);
                        $i++;
                        if ($i == $count_color) $i=0;
                        }
                }
        }
# to avoid trouble should someone wants to highlight the terms "span", "style", ...
for ($i=0; $i<5; $i++) {
        $text = preg_replace("|$starttag"."_".$i."|", "<span style=\"background-color:".$color[$i]."; font-weight: bold;\">", $text);
        $text = preg_replace("|$endtag|", "</span>", $text);
        }

return $text;
}

?>

