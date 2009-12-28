<?
if($_GET[update]=="doit") {system("svn update"); exit; }
include "config.php";
if($_GET[cmd]=="PWM")$cmd="PWM";
elseif($_GET[cmd]=="SW")$cmd="SW";
elseif($_GET[cmd]=="beamer_on")$cmd="beamer_on";
elseif($_GET[cmd]=="text_the_borg")$cmd="text_the_borg";
elseif($_GET[cmd]=="treppenblink")$cmd="treppenblink";
elseif($_GET[cmd]=="canir_teufel")$cmd="canir_teufel";
elseif($_GET[cmd]=="canir_beamer")$cmd="canir_beamer";
elseif($_GET[cmd]=="save_pos")$cmd="save_pos";
elseif($_GET[cmd]=="blue_mode")$cmd="blue_mode";
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
elseif($cmd=="treppenblink")
{
  $real_tbm_value = "0x00";
  foreach ($treppenblink_modes as $tbm_key => $tbm_value)
    if($_GET[value]==$tbm_value || 
       $_GET[value]==$tbm_key ) $real_tbm_value=$tbm_value;

  echo "powercommander.lapcontrol treppenblink mode $real_tbm_value";
  exec("powercommander.lapcontrol treppenblink mode $real_tbm_value");
}
elseif($cmd=="canir_teufel")
{
  $real_canirteufel_value = "0x00";
  foreach ($canir_teufel_a as $cirt_key => $cirt_value)
    if($_GET['value']==$cirt_value || 
       $_GET['value']==$cirt_key ) $real_canirteufel_value=$cirt_value;
  foreach ($canir_teufel_a_channel as $cirt_key => $cirt_value)
    if($_GET['value']==$cirt_value || 
       $_GET['value']==$cirt_key ) $real_canirteufel_value=$cirt_value;

  echo "powercommander.lapcontrol canir teufel $real_canirteufel_value";
  exec("powercommander.lapcontrol canir teufel $real_canirteufel_value");
}
elseif($cmd=="canir_beamer")
{
  $real_canirbeamer_value = "0x00";
  foreach ($canir_beamer_a as $cirb_key => $cirb_value)
    if($_GET[value]==$cirb_value || 
       $_GET[value]==$cirb_key ) $real_canirbeamer_value=$cirb_value;
  foreach ($canir_beamer_a_channel as $cirb_key => $cirb_value)
    if($_GET[value]==$cirb_value || 
       $_GET[value]==$cirb_key ) $real_canirbeamer_value=$cirb_value;

  echo "powercommander.lapcontrol canir beamer $real_canirbeamer_value";
  exec("powercommander.lapcontrol canir beamer $real_canirbeamer_value");
}
elseif($cmd=="save_pos")
{
	setcookie($_GET[div]."x",$_GET[x]);
	setcookie($_GET[div]."y",$_GET[y]);
	echo "Element $_GET[div] $_GET[x] $_GET[y]";
}
elseif($cmd=="blue_mode")
{
	if($_GET[mode]=="on")
	{
		$_SESSION["LAMP_VORTRAG_PWM"]=1;
		$_SESSION["VORTRAG_PWM"]=255;
		$_SESSION["LAMP_TAFEL"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL ON 0x00");
		$_SESSION["TAFEL"]="255";
		exec("powercommander.lapcontrol powercommander PWM TAFEL SET 0xff");
	
		$_SESSION["LAMP_BEAMER"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
		$_SESSION["BEAMER"]="255";
		exec("powercommander.lapcontrol powercommander PWM BEAMER SET 0xff");

		$_SESSION["LAMP_SCHRANK"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
		$_SESSION["SCHRANK"]="255";
		exec("powercommander.lapcontrol powercommander PWM SCHRANK SET 0xff");
		
		$_SESSION["LAMP_FLIPPER"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
		$_SESSION["FLIPPER"]="255";
		exec("powercommander.lapcontrol powercommander PWM FLIPPER SET 0xff");
	}
	elseif($_GET[mode]=="off")
	{
		$_SESSION["LAMP_VORTRAG_PWM"]=0;
		$_SESSION["VORTRAG_PWM"]=0;
		$_SESSION["LAMP_TAFEL"]=0;
		exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL OFF 0x00");
		$_SESSION["LAMP_BEAMER"]=0;
		exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER OFF 0x00");
		$_SESSION["LAMP_SCHRANK"]=0;
		exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK OFF 0x00");
		$_SESSION["LAMP_FLIPPER"]=0;
		exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER OFF 0x00");
	}
	elseif($_GET[mode]=="video_conference")
	{
		$_SESSION["VORTRAG_PWM"]=0;
		$_SESSION["LAMP_TAFEL"]=0;
		exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL OFF 0x00");

		$_SESSION["LAMP_BEAMER"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
		$_SESSION["BEAMER"]="0";
		exec("powercommander.lapcontrol powercommander PWM BEAMER SET 0x00");

		$_SESSION["LAMP_SCHRANK"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
		$_SESSION["SCHRANK"]="119";
		exec("powercommander.lapcontrol powercommander PWM SCHRANK SET 0x77");

		$_SESSION["LAMP_FLIPPER"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
		$_SESSION["FLIPPER"]="255";
		exec("powercommander.lapcontrol powercommander PWM FLIPPER SET 0xff");
	}
	elseif($_GET[mode]=="speaker")
	{
		$_SESSION["VORTRAG_PWM"]=0;
		$_SESSION["LAMP_TAFEL"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL ON 0x00");
		$_SESSION["TAFEL"]="255";
		exec("powercommander.lapcontrol powercommander PWM TAFEL SET 0xff");

		$_SESSION["LAMP_BEAMER"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
		$_SESSION["BEAMER"]="0";
		exec("powercommander.lapcontrol powercommander PWM BEAMER SET 0x00");

		$_SESSION["LAMP_SCHRANK"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
		$_SESSION["SCHRANK"]="0";
		exec("powercommander.lapcontrol powercommander PWM SCHRANK SET 0x00");

		$_SESSION["LAMP_FLIPPER"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
		$_SESSION["FLIPPER"]="0";
		exec("powercommander.lapcontrol powercommander PWM FLIPPER SET 0x00");
	}
	elseif($_GET[mode]=="dimm")
	{
		$_SESSION["LAMP_VORTRAG_PWM"]=1;
		$_SESSION["VORTRAG_PWM"]=51;
		$_SESSION["LAMP_TAFEL"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL ON 0x00");
		$_SESSION["TAFEL"]="51";
		exec("powercommander.lapcontrol powercommander PWM TAFEL SET 0x33");

		$_SESSION["LAMP_BEAMER"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
		$_SESSION["BEAMER"]="51";
		exec("powercommander.lapcontrol powercommander PWM BEAMER SET 0x33");

		$_SESSION["LAMP_SCHRANK"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
		$_SESSION["SCHRANK"]="51";
		exec("powercommander.lapcontrol powercommander PWM SCHRANK SET 0x33");

		$_SESSION["LAMP_FLIPPER"]=1;
		exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
		$_SESSION["FLIPPER"]="51";
		exec("powercommander.lapcontrol powercommander PWM FLIPPER SET 0x33");
	}
}

echo "<script>$script</script>";
?>
