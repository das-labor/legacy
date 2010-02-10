<?

$file = "weblab2.5.status.txt";

$localstate = array();
global $localstate;
$indata = file_get_contents($file);

$localstate = unserialize( $indata );


$control="enabled";
#$control="aus";

?>