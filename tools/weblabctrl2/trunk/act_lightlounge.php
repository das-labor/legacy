<?
//var_dump($_GET);
require_once "system_conf.php";
require_once "container.php";

if($_GET["doit"])
  {
    $tmpobj = new c_lightlounge();
    $command = $tmpobj->isvalidmode($_GET["function"]);
    $jsstr = "";
    global $localstate;
    foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
      {
	if( ( preg_match("/[!0-9]/",$_GET[$myobj->getObjName()."value"]) ) && 
	    ( $_GET[$myobj->getObjName()."value"]<=255 ) && 
	    ( $_GET[$myobj->getObjName()."value"]>=0 ) ) 
	  {
	    $value_dec=(int)$_GET[$myobj->getObjName()."value"];
	    $value=dechex($value_dec);
	  }
      }

    switch ($command){
    case "on":
      {
	//	echo "powercommander.lapcontrol powercommander VIRT VORTRAG ON 0x00";
	if ($control == "enabled"){
	  exec("powercommander.lapcontrol powercommander SW LAMP_LOUNGE ON 0x00");
	  exec("powercommander.lapcontrol packet 0x00:0x00 0x60:0x01 0x01,0x03,0xff");
	  exec("powercommander.lapcontrol packet 0x00:0x00 0x61:0x01 0x01,0x03,0xff");
	}
	$localstate["SW_LAMP_LOUNGE"]=1;
	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_on();\n";
	  }

      }
      break;
    case "off":
      {
	//	echo "powercommander.lapcontrol powercommander VIRT VORTRAG OFF 0x00";
	if ($control == "enabled"){
	  exec("powercommander.lapcontrol powercommander SW LAMP_LOUNGE OFF 0x00");
	  exec("powercommander.lapcontrol packet 0x00:0x00 0x60:0x01 0x01,0x03,0x00");
	  exec("powercommander.lapcontrol packet 0x00:0x00 0x61:0x01 0x01,0x03,0x00");
	}
	$localstate["SW_LAMP_LOUNGE"]=0;
	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_off();\n";
	  }

      }
      break;
    case "dimm":
      {
	if ($control == "enabled"){
	   exec("powercommander.lapcontrol powercommander PWM LOUNGE SET 0x".$value);
	}
	$localstate["LOUNGE_PWM"]=$value_dec;
      }
      break;
    case "dimm_s1":
      if ($control == "enabled"){
	exec("powercommander.lapcontrol packet 0x00:0x00 0x60:0x01 0x01,0x00,0x".$value);
      }
      break;
    case "dimm_s2":
      if ($control == "enabled"){
	exec("powercommander.lapcontrol packet 0x00:0x00 0x60:0x01 0x01,0x01,0x".$value);
      }
      break;
    case "dimm_s3":
      if ($control == "enabled"){
	exec("powercommander.lapcontrol packet 0x00:0x00 0x60:0x01 0x01,0x02,0x".$value);
      }
      break;
    case "dimm_s4":
      if ($control == "enabled"){
	exec("powercommander.lapcontrol packet 0x00:0x00 0x61:0x01 0x01,0x00,0x".$value);
      }
      break;
    case "dimm_s5":
      if ($control == "enabled"){
	exec("powercommander.lapcontrol packet 0x00:0x00 0x61:0x01 0x01,0x01,0x".$value);
      }
      break;
    case "dimm_s6":
      if ($control == "enabled"){
	exec("powercommander.lapcontrol packet 0x00:0x00 0x61:0x01 0x01,0x02,0x".$value);
      }
      break;
    case "hacker":
      {
	echo "I think it's a hacker";
      }
      break;
    default:
      {
	echo "Idiot!";
      }
    }
    echo "<script>".$jsstr."</script>";
    file_put_contents($file, serialize( $localstate )); 
  }

?>