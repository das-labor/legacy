<?
//var_dump($_GET);
require_once "system_conf.php";
require_once "container.php";



if($_GET["doit"])
  {
    $tmpobj = new c_lightkueche();
    $jsstr = "";
    $command = $tmpobj->isvalidmode($_GET["function"]);
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
	if ($control == "enabled"){
	  exec("powercommander.lapcontrol powercommander SW LAMP_KUECHE ON 0x00");
	}
	$localstate["SW_LAMP_KUECHE"]=1;
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
	  exec("powercommander.lapcontrol powercommander SW LAMP_KUECHE OFF 0x00");
	}
	$localstate["SW_LAMP_KUECHE"]=0;
	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_off();\n";
	  }

      }
      break;
    case "dimm":
      {
	if ($control == "enabled"){
	   exec("powercommander.lapcontrol powercommander PWM KUECHE SET 0x".$value);
	}
	$localstate["KUECHE_PWM"]=$value_dec;
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