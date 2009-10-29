<?php
// Read data from GET request
$id=htmlentities($_GET["id"]);
$time=htmlentities($_GET["ts"]);
$lat=htmlentities($_GET["lat"]);
$lon=htmlentities($_GET["lon"]);
$alt=htmlentities($_GET["alt"]);
$hea=htmlentities($_GET["hea"]);
$spd=htmlentities($_GET["spd"]);

if ($lat !== "" && $lon !== "")
	echo "OK, data received";

// write to file
if ( $lat != "" & $lon != "" ){
	$posFile = "view/position.csv";
	$fh = fopen($posFile, 'w+') or die("can't open file");
	$stringData = $id.",".$time.",".$lat.",".$lon.",".$alt.",".$hea.",".$spd;
	fwrite($fh, $stringData);
	fclose($fh);
}
?>
