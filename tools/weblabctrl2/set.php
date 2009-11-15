<?
if($_GET[update]=="doit") {system("svn update"); exit; }
include "config.php";
if($_GET[cmd]=="PWM")$cmd="PWM";
elseif($_GET[cmd]=="SW")$cmd="SW";
elseif($_GET[cmd]=="beamer_on")$cmd="beamer_on";
elseif($_GET[cmd]=="text_the_borg")$cmd="text_the_borg";
elseif($_GET[cmd]=="save_pos")$cmd="save_pos";
else unset($_GET[cmd]);

if(in_array($_GET[id],$pwm_ids)) $id=$_GET[id];
elseif(in_array($_GET[id],$sw_ids)) $id=$_GET[id];
else unset($_GET[id]);

if($cmd=="PWM")
{
	if(preg_match("/[!0-9]/",$_GET[value]) && $_GET[value]<=255 && $_GET[value]>=0) $value=dechex($_GET[value]);
	else unset($_GET[value]);
	if($id=="VORTRAG_PWM")
	{
		$_SESSION[$id]=hexdec($value);
		$_SESSION["TAFEL"]=hexdec($value);
		$_SESSION["BEAMER"]=hexdec($value);
		$_SESSION["SCHRANK"]=hexdec($value);
		$_SESSION["FLIPPER"]=hexdec($value);
		exec("powercommander.lapcontrol powercommander VIRT $id SET 0x$value");
		echo "powercommander.lapcontrol powercommander VIRT $id SET 0x$value";
	}
	else
	{
		$_SESSION[$id]=hexdec($value);
		exec("powercommander.lapcontrol powercommander $cmd $id SET 0x$value");
		echo "powercommander.lapcontrol powercommander $cmd $id SET 0x$value";
	}
}
elseif($cmd=="SW")
{
	if($_GET[value]=="ON" || $_GET[value]=="OFF") $value=$_GET[value];
	if($id=="LAMP_VORTRAG_PWM")
	{
		if($value=="ON")
		{
			$_SESSION[$id]=1;
			$_SESSION["LAMP_TAFEL"]=1;
			$_SESSION["LAMP_BEAMER"]=1;
			$_SESSION["LAMP_SCHRANK"]=1;
			$_SESSION["LAMP_FLIPPER"]=1;
		}
		if($value=="OFF")
		{
			$_SESSION[$id]=0;
			$_SESSION["LAMP_TAFEL"]=0;
			$_SESSION["LAMP_BEAMER"]=0;
			$_SESSION["LAMP_SCHRANK"]=0;
			$_SESSION["LAMP_FLIPPER"]=0;
		}
		$id="VORTRAG";
		echo "powercommander.lapcontrol powercommander VIRT $id $value 0x00";
		exec("powercommander.lapcontrol powercommander VIRT $id $value 0x00");
	}
	else
	{
		if($value=="ON")$_SESSION[$id]=1;
		if($value=="OFF")$_SESSION[$id]=0;
		echo "powercommander.lapcontrol powercommander $cmd $id $value 0x00";
		exec("powercommander.lapcontrol powercommander $cmd $id $value 0x00");
	}
}
elseif($cmd=="beamer_on")
{
	exec("powercommander.lapcontrol powercommander SW PROJEKTOR ON 0x00");
	echo "powercommander.lapcontrol powercommander SW PROJEKTOR ON 0x00";
	$script .= "document.getElementById('beamer_button').disabled=true;\n";
	$_SESSION['beamer_on']=1;
}
elseif($cmd=="text_the_borg")
{
	echo "borg";
	exec("lapcontrol -s rl borg scroll 0x24 '2<5|+30/#".escapeshellcmd($_GET[text])."'");
	exec("lapcontrol -s rl borg mode 0x24 1");
}
elseif($cmd=="save_pos")
{
	setcookie($_GET[div]."x",$_GET[x]);
	setcookie($_GET[div]."y",$_GET[y]);
	echo "Element $_GET[div] $_GET[x] $_GET[y]";
}

echo "<script>$script</script>";
?>
