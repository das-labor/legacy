<?php
require_once ('config.inc.php');
header('Refresh: '.$sec);
require_once ('functions.php');
$link_url = show_rand_rss($urls);
$base_url = preg_replace('#(http://.*?/)[^/]+$#i', '\1', $link_url);
#$page_contents = preg_replace('#^.*?(<link .*?rel="stylesheet"[^>]+>)+?.*?(<body>.*?</body>).*$#ims', '\1</head>\2', file_get_contents($link_url));
$page_contents = preg_replace('#<title>.*?</title>#i', '<title>' . $title . '</title>', file_get_contents($link_url));
?>
<html>
<head>
<title><?php $title; ?></title>
<?php
echo preg_replace('#((?:src|href)=")([^"]+)"#i', '\1' . $base_url . '\2"', $page_contents);
?>
</html>
