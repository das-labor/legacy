<?
//var_dump($_GET);

require_once "system_conf.php";
require_once "container.php";

if($_GET["doit"])
  {
    global $localstate;
    $tmpobj = new c_lightvortragmeta();
    $tmpobj2 = new c_lightvortragdetail();
    $command = $tmpobj->isvalidmode($_GET["function"]);
    $jsstr = "";
    $value="00";
    $value_dec=0;
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
	  exec("powercommander.lapcontrol powercommander VIRT VORTRAG ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
	}
	$localstate["VIRT_VORTRAG"]=1;
	$localstate["SW_LAMP_TAFEL"]=1;
	$localstate["SW_LAMP_BEAMER"]=1;
	$localstate["SW_LAMP_SCHRANK"]=1;
	$localstate["SW_LAMP_FLIPPER"]=1;
	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_on();\n";
	  }
	foreach($sortedobjects[$tmpobj2->getObjName()] as $myobj2)
	  {
	    $jsstr .= $myobj2->getid()."button_show_on_t();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_b();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_s();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
	  }


      }
      break;
    case "off":
      {
	//	echo "powercommander.lapcontrol powercommander VIRT VORTRAG OFF 0x00";
	if ($control == "enabled"){
	  exec("powercommander.lapcontrol powercommander VIRT VORTRAG OFF 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL OFF 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER OFF 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK OFF 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER OFF 0x00");
	}
	$localstate["VIRT_VORTRAG"]=0;
	$localstate["SW_LAMP_TAFEL"]=0;
	$localstate["SW_LAMP_BEAMER"]=0;
	$localstate["SW_LAMP_SCHRANK"]=0;
	$localstate["SW_LAMP_FLIPPER"]=0;
	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_off();\n";
	  }
	foreach($sortedobjects[$tmpobj2->getObjName()] as $myobj2)
	  {
	    $jsstr .= $myobj2->getid()."button_show_off_t();\n";
	    $jsstr .= $myobj2->getid()."button_show_off_b();\n";
	    $jsstr .= $myobj2->getid()."button_show_off_s();\n";
	    $jsstr .= $myobj2->getid()."button_show_off_f();\n";
	  }
      }
      break;
    case "dimm":
      {
	if ($localstate["VORTRAG_PWM"]!=$value_dec)
	  {
	    if ($control == "enabled"){
	      exec("powercommander.lapcontrol powercommander VIRT VORTRAG_PWM SET 0x".$value);
	    }
	    $localstate["VORTRAG_PWM"]=$value_dec;
	    $localstate["TAFEL_PWM"]=$value_dec;
	    $localstate["BEAMER_PWM"]=$value_dec;
	    $localstate["SCHRANK_PWM"]=$value_dec;
	    $localstate["FLIPPER_PWM"]=$value_dec;
	}

      }
      break;
    case "setdark":
      {
	if ($control == "enabled"){
	  exec("powercommander.lapcontrol powercommander VIRT VORTRAG ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
	  exec("powercommander.lapcontrol powercommander VIRT VORTRAG_PWM SET 0x00");
	}

	$localstate["VIRT_VORTRAG"]=1;
	$localstate["SW_LAMP_TAFEL"]=1;
	$localstate["SW_LAMP_BEAMER"]=1;
	$localstate["SW_LAMP_SCHRANK"]=1;
	$localstate["SW_LAMP_FLIPPER"]=1;
	$localstate["VORTRAG_PWM"]=0;
	$localstate["TAFEL_PWM"]=0;
	$localstate["BEAMER_PWM"]=0;
	$localstate["SCHRANK_PWM"]=0;
	$localstate["FLIPPER_PWM"]=0;
	$localstate["VORTRAG_PWM"]=0;
	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_on();\n";
	  }
	foreach($sortedobjects[$tmpobj2->getObjName()] as $myobj2)
	  {
	    $jsstr .= $myobj2->getid()."button_show_on_t();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_b();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_s();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
	  }

      }
      break;
    case "setlight":
      {
	if ($control == "enabled"){
	  exec("powercommander.lapcontrol powercommander VIRT VORTRAG ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
	  exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
	  exec("powercommander.lapcontrol powercommander VIRT VORTRAG_PWM SET 0x80");
	}

	$localstate["VIRT_VORTRAG"]=1;
	$localstate["SW_LAMP_TAFEL"]=1;
	$localstate["SW_LAMP_BEAMER"]=1;
	$localstate["SW_LAMP_SCHRANK"]=1;
	$localstate["SW_LAMP_FLIPPER"]=1;
	$localstate["VORTRAG_PWM"]=128;
	$localstate["TAFEL_PWM"]=128;
	$localstate["BEAMER_PWM"]=128;
	$localstate["SCHRANK_PWM"]=128;
	$localstate["FLIPPER_PWM"]=128;
	$localstate["VORTRAG_PWM"]=128;
	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_on();\n";
	  }
	foreach($sortedobjects[$tmpobj2->getObjName()] as $myobj2)
	  {
	    $jsstr .= $myobj2->getid()."button_show_on_t();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_b();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_s();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
	  }
      }
      break;
    case "frontdark":
      {
	if ($control == "enabled"){
	   exec("powercommander.lapcontrol powercommander VIRT VORTRAG ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL OFF 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
	   exec("powercommander.lapcontrol powercommander PWM BEAMER SET 0x00");
	   exec("powercommander.lapcontrol powercommander PWM SCHRANK SET 0x40");
	   exec("powercommander.lapcontrol powercommander PWM FLIPPER SET 0x80");
	}

	$localstate["VIRT_VORTRAG"]=1;
	$localstate["SW_LAMP_TAFEL"]=0;
	$localstate["SW_LAMP_BEAMER"]=1;
	$localstate["SW_LAMP_SCHRANK"]=1;
	$localstate["SW_LAMP_FLIPPER"]=1;
	$localstate["BEAMER_PWM"]=0;
	$localstate["SCHRANK_PWM"]=64;
	$localstate["FLIPPER_PWM"]=128;
	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_on();\n";
	  }
	foreach($sortedobjects[$tmpobj2->getObjName()] as $myobj2)
	  {
	    $jsstr .= $myobj2->getid()."button_show_off_t();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_b();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_s();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
	  }

      }
      break;
    case "frontlight":
      {
	if ($control == "enabled"){
	   exec("powercommander.lapcontrol powercommander VIRT VORTRAG ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER OFF 0x00");
	   exec("powercommander.lapcontrol powercommander PWM SCHRANK SET 0x00");
	   exec("powercommander.lapcontrol powercommander PWM BEAMER SET 0x40");
	   exec("powercommander.lapcontrol powercommander PWM TAFEL SET 0x80");
	}

	$localstate["VIRT_VORTRAG"]=1;
	$localstate["SW_LAMP_TAFEL"]=1;
	$localstate["SW_LAMP_BEAMER"]=1;
	$localstate["SW_LAMP_SCHRANK"]=1;
	$localstate["SW_LAMP_FLIPPER"]=0;
	$localstate["TAFEL_PWM"]=128;
	$localstate["BEAMER_PWM"]=64;
	$localstate["SCHRANK_PWM"]=0;

	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_on();\n";
	  }
	foreach($sortedobjects[$tmpobj2->getObjName()] as $myobj2)
	  {
	    $jsstr .= $myobj2->getid()."button_show_on_t();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_b();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_s();\n";
	    $jsstr .= $myobj2->getid()."button_show_off_f();\n";
	  }


      }
      break;
    case "default":
      {
	if ($control == "enabled"){
	   exec("powercommander.lapcontrol powercommander VIRT VORTRAG ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
	   exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
	   exec("powercommander.lapcontrol powercommander VIRT VORTRAG_PWM SET 0x40");
	   exec("powercommander.lapcontrol powercommander PWM SCHRANK SET 0x40");
	   exec("powercommander.lapcontrol powercommander PWM BEAMER SET 0x40");
	   exec("powercommander.lapcontrol powercommander PWM TAFEL SET 0x40");
	   exec("powercommander.lapcontrol powercommander PWM FLIPPER SET 0x40");
	}

	$localstate["TAFEL_PWM"]=64;
	$localstate["BEAMER_PWM"]=64;
	$localstate["SCHRANK_PWM"]=64;
	$localstate["FLIPPER_PWM"]=64;
	$localstate["VORTRAG_PWM"]=64;
	$localstate["VIRT_VORTRAG"]=1;
	$localstate["SW_LAMP_TAFEL"]=1;
	$localstate["SW_LAMP_BEAMER"]=1;
	$localstate["SW_LAMP_SCHRANK"]=1;
	$localstate["SW_LAMP_FLIPPER"]=1;
	foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
	  {
	    $jsstr .= $myobj->getid()."button_show_on();\n";
	  }
	foreach($sortedobjects[$tmpobj2->getObjName()] as $myobj2)
	  {
	    $jsstr .= $myobj2->getid()."button_show_on_t();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_b();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_s();\n";
	    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
	  }

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