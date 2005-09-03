<?php
/*
global config file
Set $title to a title you like to see on your browser (like 'Free Sex and Beer!' or something)
Set $sec to a time (in seconds :) the script should wait befor switching to the next page

For each rss stream you want to shuffle through, add a line as the ones given below:

$url[] = 'http://mystream.com/news.rss';

*/


$title = "Weltherrschaft a.k.a. RSS-Shuffle for the lazy masses";
$sec = 80;

$urls[] = 'http://tilmanfrosch.de/pblog/index.php?x=rss';
$urls[] = 'http://slashdot.org/index.rss';
$urls[] = 'http://www.das-labor.org/weblog/index.php/feed/';

?>
