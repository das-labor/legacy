<html>
<head><title>GetCoords</title></head>
<body>
<?php
// Read data from GET request
$id=htmlentities($_GET["deviceID"]);
$pos=htmlentities($_GET["position"]);

$input=$id.",".$pos;

$data = explode(",", $input, 1000);
		$id = $data[0];	// deviceID
        $ts = $data[2];
        $lat = $data[3];
        $lon = $data[5];
        $ns = $data[4];
        $ew = $data[6];
		$alt = $data[10];
		$altunit = $data[11];

function nmea2deg($nmea, $hem){
    $deg = (int)($nmea/100) + ($nmea - (int)($nmea / 100) * 100) / 60;
    if ( $hem == "S" || $hem == "W" )
        $deg = 0-$deg;
    return $deg;
}

$time = (int)$ts;
$lat = nmea2deg($lat, $ns);
$lon = nmea2deg($lon, $ew);
$alt = $alt.strtolower($altunit);
$hea="none";
$spd="none";



// write to file
	$posFile = "view/position.csv";
	$fh = fopen($posFile, 'w+') or die("can't open file");
	$stringData = $id.",".$time.",".$lat.",".$lon.",".$alt.",".$hea.",".$spd;
	fwrite($fh, $stringData);
	fclose($fh);
?>
