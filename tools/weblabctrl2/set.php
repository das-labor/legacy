<?
include "config.php";
echo "debug";
if($_GET[cmd]=="PWM")$cmd="PWM";
elseif($_GET[cmd]=="SW")$cmd="SW";
else unset($_GET[cmd]);

if(in_array($_GET[id],$pwm_ids)) $id=$_GET[id];
elseif(in_array($_GET[id],$sw_ids)) $id=$_GET[id];
else unset($_GET[id]);

if($cmd=="PWM")
{
	if(preg_match("/[!0-9]/",$_GET[value]) && $_GET[value]<=255 && $_GET[value]>=0) $value=dechex($_GET[value]);
	else unset($_GET[value]);
	if($id=="VORTRAG_ALL")
		foreach(array("TAFEL","BEAMER","SCHRANK","FLIPPER") as $id)
		{
			exec("powercommander.lapcontrol powercommander $cmd $id SET 0x$value");
		}
	else exec("powercommander.lapcontrol powercommander $cmd $id SET 0x$value");
	echo "powercommander.lapcontrol powercommander $cmd $id SET 0x$value";

	include "status.txt";
	$first=false;
	$file_content = "<? \$status = array( ";
	foreach($status as $status_key => $status_value)
	{
		if($first==true) $file_content .= ", ";
		if($status_key==$id)
		{
			$file_content .= "\"$status_key\" => \"".hexdec($value)."\""; 
			$script .= "slider_".$status_key."_control.setValue('".hexdec($value)."');\n";
		}
		else
			$file_content .= "\"$status_key\" => \"$status_value\""; 
		$first=true;
	}
	$file_content .= "); ?>";
	$file=fopen("status.txt","w");
	fwrite($file,$file_content);
	fclose($file);
		
}
elseif($cmd=="SW")
{
	if($_GET[value]=="ON" || $_GET[value]=="OFF") $value=$_GET[value];
	echo "powercommander.lapcontrol powercommander $cmd $id $value 0x00";
	exec("powercommander.lapcontrol powercommander $cmd $id $value 0x00");
}

echo "<script>$script</script>";
?>
