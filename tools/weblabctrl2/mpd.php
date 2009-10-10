<?
include "config.php";
if(array_key_exists($_GET[room],$rooms)) $room=$rooms[$_GET[room]];
else unset($_GET[room]);
if(in_array($_GET[cmd],$mpd_cmd)) $cmd=$_GET[cmd];
else unset($_GET[cmd]);

if($cmd=="volume")
	if(preg_match("/[!0-9]/",$_GET[value]) && $_GET[value]<=100 && $_GET[value]>=0) $value=$_GET[value];
else unset($value);

echo "export MPD_PORT=$room; mpc $cmd $value";
exec ("export MPD_PORT=$room; mpc $cmd $value");

?>
