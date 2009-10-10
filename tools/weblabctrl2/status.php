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
	if(strpos($status[0],"volume")===false) $script .= "document.getElementById('".$room."_status').innerHTML='".substr(htmlspecialchars($status[0]),0,24)."';\n";;
}
echo "<script>$script</script>";
?>
