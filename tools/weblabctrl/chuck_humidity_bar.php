<?php
header ('Expires: Thu, 23 Nov 1971 23:42:31 GMT');

define ('CHUCK_MINHUMIDITY', 25);
define ('CHUCK_MAXHUMIDITY', 75);
define ('CHUCK_TRESHOLDLEV', 50);

define ('CHUCK_BARWIDTH', 20);
define ('CHUCK_BARHEIGHT', 100);

require_once ('conf/lapd.php');
$fp = pfsockopen(CONF_LAPD_ADDR, CONF_LAPD_PORT, $errno, $errstr, 30);
fwrite ($fp, "get CHUCK humidity\n");
$tmp = fgets($fp, 256);
preg_match ('#chuck +humidity +([0-9]+)#i', $tmp, $m);

$hum_level = $m[1];
($hum_level > CHUCK_MAXHUMIDITY) ?
	CHUCK_MAXHUMIDITY : $hum_level;

($hum_level < CHUCK_MINHUMIDITY) ?
	CHUCK_MINHUMIDITY : $hum_level;

fclose($fp);



$pxperstep = (CHUCK_BARHEIGHT / (CHUCK_MAXHUMIDITY - CHUCK_MINHUMIDITY));

header ("Content-type: image/png");
$im = imagecreatetruecolor(CHUCK_BARWIDTH, CHUCK_BARHEIGHT)
or die("Cannot Initialize new GD image stream");
$bgcolor = imagecolorallocate($im, 255, 255, 255);
imagefill ($im, 0, 0, $bgcolor);
$border_color = imagecolorallocate($im, 0, 0, 0);

// bar border
imagerectangle  ( $im  ,0 ,0 , CHUCK_BARWIDTH - 1, CHUCK_BARHEIGHT - 1, $border_color);


$fillcolor = imagecolorallocate($im, 30, 30, 230);
imagefilledrectangle  ( $im  ,1 ,1+ floor((CHUCK_MAXHUMIDITY-$hum_level) * $pxperstep),
	CHUCK_BARWIDTH - 2, CHUCK_BARHEIGHT - 2, $fillcolor);

// treshold bar
$tresholdcolor = imagecolorallocate($im, 255, 128, 0);
imagerectangle ( $im, 1, 1+((CHUCK_MAXHUMIDITY - CHUCK_TRESHOLDLEV) * $pxperstep),
	CHUCK_BARWIDTH - 2, 2+ ((CHUCK_MAXHUMIDITY - CHUCK_TRESHOLDLEV) * $pxperstep),
	$tresholdcolor);


//imagettftext($im, 20, 0, 10, 20, $bgcolor, "Arial", "humidity");

imagepng($im);
imagedestroy($im);

?>

