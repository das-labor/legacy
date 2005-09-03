<?php
require_once ('config.inc.php');
function show_rand_rss ($source = 'http://tilmanfrosch.de/pblog/index.php?x=rss')
{
	if (is_array($source))
		$source = $source[rand(0, count($source)-1)];
	$lines = file_get_contents($source);
	preg_match_all('#
		<item>.*?
			<link>([^<]+)</link>
			.*?
		</item>
		|
		<rdf\:li\ rdf\:resource="
			([^"]+)"
	#imsx', $lines, $m);
	// wer das liest ist doof.
	if ($m[2][0] != '')
		return $m[2][rand(0, count($m[2])-1)];
	return $m[1][rand(0, count($m[2])-1)];
}
// echo "URL: " . show_rand_rss($urls); //debugging: uncomment so see URLs. Attention: is NOT the corresponding URL!
?>
