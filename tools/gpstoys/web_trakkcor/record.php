<html>
<head><title>GetCoords</title></head>
<body>
<?php
// Read data from GET request
$id=htmlentities($_GET["deviceID"]);
$pos=htmlentities($_GET["position"]);

echo $id.", ".$pos;

// write to file
	$posFile = "view/position.csv";
	$fh = fopen($posFile, 'w+') or die("can't open file");
	$stringData = $id.",".$pos;
	fwrite($fh, $stringData);
	fclose($fh);
?>
