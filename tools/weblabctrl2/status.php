<?

include "config.php";
include "status.txt";

foreach($pwm_ids as $id)
{
	$script .= "slider_".$id."_control.setValue('".$status[$id]."');\n";
}
foreach($rooms as $room => $port)
{
	unset($status);
	$port=$rooms[$room];
	exec("export MPD_PORT=$port; mpc",$status);
	$script .= "for (var i = 0; i < document.getElementsByClassName('".$room."_button').length; i++) {document.getElementsByClassName('".$room."_button')[i].style.backgroundColor='';}
";
	if(strpos($status[0],"volume")===false) $script .= "document.getElementById('".$room."_status').innerHTML='".preg_replace("/'/","&#39;",substr(htmlspecialchars($status[0]),0,24))."';\n";;
	if(strpos($status[1],"playing")!==false) $script .= "document.getElementById('".$room."_play').style.backgroundColor='#aaaaaa';\n";;
	if(strpos($status[1],"paused")!==false) $script .= "document.getElementById('".$room."_pause').style.backgroundColor='#aaaaaa';\n";;
}
echo "<script>$script</script>";
?>
