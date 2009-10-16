<?
if($_GET[update]=="doit") {system("svn update"); exit; }
include "config.php";
echo "debug";
if($_GET[cmd]=="PWM")$cmd="PWM";
elseif($_GET[cmd]=="SW")$cmd="SW";
elseif($_GET[cmd]=="beamer_on")$cmd="beamer_on";
else unset($_GET[cmd]);

if(in_array($_GET[id],$pwm_ids)) $id=$_GET[id];
elseif(in_array($_GET[id],$sw_ids)) $id=$_GET[id];
else unset($_GET[id]);

if($cmd=="PWM")
{
	if(preg_match("/[!0-9]/",$_GET[value]) && $_GET[value]<=255 && $_GET[value]>=0) $value=dechex($_GET[value]);
	else unset($_GET[value]);
	$_SESSION[$id]=hexdec($value);
	exec("powercommander.lapcontrol powercommander $cmd $id SET 0x$value");
	echo "powercommander.lapcontrol powercommander $cmd $id SET 0x$value";
	
}
elseif($cmd=="SW")
{
	if($_GET[value]=="ON" || $_GET[value]=="OFF") $value=$_GET[value];
	if($value=="ON")$_SESSION[$id]=1;
	if($value=="OFF")$_SESSION[$id]=0;
	echo "powercommander.lapcontrol powercommander $cmd $id $value 0x00";
	exec("powercommander.lapcontrol powercommander $cmd $id $value 0x00");
}
elseif($cmd=="beamer_on")
{
	exec("powercommander.lapcontrol powercommander SW PROJEKTOR ON 0x00");
	echo "powercommander.lapcontrol powercommander SW PROJEKTOR ON 0x00";
	$script .= "document.getElementById('beamer_button').disabled=true;\n";;
	$_SESSION['beamer_on']=1;
}

echo "<script>$script</script>";
?>
