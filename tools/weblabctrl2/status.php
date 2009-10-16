<?

include "config.php";

foreach($pwm_ids as $id)
{
	unset($status);
	if(!isset($_SESSION[$id]))
	{
		exec("powercommander.lapcontrol powercommander PWM $id GET 0",$status);
		echo "new status $id";
		//var_dump($status);
		if(preg_match("/returned/",$status[2])) 
		$_SESSION[$id]=hexdec(substr($status[2],11,4));
	}
	$script .= "slider_".$id."_control.setValue('".$_SESSION[$id]."');\n";
}

foreach($rooms as $room => $port)
{
	unset($status);
	$port=$rooms[$room];
	exec("export MPD_PORT=$port; mpc",$status);
	$script .= "for (var i = 0; i < document.getElementsByClassName('".$room."_button').length; i++) {document.getElementsByClassName('".$room."_button')[i].style.backgroundColor='';}\n";
	if(strpos($status[0],"volume")===false) $script .= "document.getElementById('".$room."_status').innerHTML='".preg_replace("/'/","&#39;",substr(htmlspecialchars($status[0]),0,24))."';\n";;
	if(strpos($status[1],"playing")!==false) $script .= "document.getElementById('".$room."_play').style.backgroundColor='#aaaaaa';\n";;
	if(strpos($status[1],"paused")!==false) $script .= "document.getElementById('".$room."_pause').style.backgroundColor='#aaaaaa';\n";;
	if(preg_match("/\(([0-9]{1,2}%)\)/",$status[1],$volume)) $script .= "slider_process_".$room."_control.setValue('".substr($volume[1],0,-1)."');\n";
}
if(!isset($_SESSION['beamer_on']))
{
	unset($status);
	exec("powercommander.lapcontrol powercommander SW  PROJEKTOR STATUS 0",$status);
	if(preg_match("/returned/",$status[2]))
		$_SESSION['beamer_on']=hexdec(substr($status[2],11,4));
}

if($_SESSION['beamer_on']==1)
	$script .= "document.getElementById('beamer_button').disabled=true;\n";
else
	$script .= "document.getElementById('beamer_button').disabled=false;\n";

echo "<script>$script</script>";
?>
