<?
require_once "system_conf.php";
require_once "container.php";


if($_GET["doit"])
  {
    $tmpobj = new c_lightvortragdetail();
    $command = $tmpobj->isvalidmode($_GET["function"]);
    $object = $tmpobj->isvalidobj($_GET["object"]);
    $value="00";
    $value_dec=0;
    $jsstr = "";
    global $localstate;
    foreach($sortedobjects[$tmpobj->getObjName()] as $myobj)
      {
	if( ( preg_match("/[!0-9]/",$_GET[$myobj->getid()."value"]) ) && 
	    ( $_GET[$myobj->getid()."value"]<=255 ) && 
	    ( $_GET[$myobj->getid()."value"]>=0 ) ) 
	  {
	    $value_dec=(int) $_GET[$myobj->getid()."value"];
	    $value=dechex($value_dec);
	  }
      }

    switch ($command){
    case "on":
      {
	//	echo "powercommander.lapcontrol powercommander VIRT VORTRAG ON 0x00";
	  switch($object)
	    {
	    case "tafel":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL ON 0x00");
		$localstate["SW_LAMP_TAFEL"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_t();\n";
		  }

	      }
	      break;
	    case "beamer":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER ON 0x00");
		$localstate["SW_LAMP_BEAMER"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_b();\n";
		  }

	      }
	      break;
	    case "schrank":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK ON 0x00");
		$localstate["SW_LAMP_SCHRANK"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_s();\n";
		  }

	      }
	      break;
	    case "flipper":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER ON 0x00");
		$localstate["SW_LAMP_FLIPPER"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
		  }

	      }
	      break;
	    case "hacker":
	      {
		//		var_dump($_GET);
		echo "I think it's a hacker";
	      }
	      break;
	    default:
	      {
		echo "Idiot!";
	      }
	    }
      }
      break;
    case "off":
      {
	  switch($object)
	    {
	    case "tafel":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol powercommander SW LAMP_TAFEL OFF 0x00");
		$localstate["SW_LAMP_TAFEL"]=0;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_off_t();\n";
		  }

	      }
	      break;
	    case "beamer":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol powercommander SW LAMP_BEAMER OFF 0x00");
		$localstate["SW_LAMP_BEAMER"]=0;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_off_b();\n";
		  }

	      }
	      break;
	    case "schrank":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol powercommander SW LAMP_SCHRANK OFF 0x00");
		$localstate["SW_LAMP_SCHRANK"]=0;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_off_s();\n";
		  }

	      }
	      break;
	    case "flipper":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol powercommander SW LAMP_FLIPPER OFF 0x00");
		$localstate["SW_LAMP_FLIPPER"]=0;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_off_f();\n";
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
      }
      break;
    case "dimm":
      {
	  switch($object)
	    {
	    case "tafel":
	      {
		if ($localstate["TAFEL_PWM"] != $value_dec)
		  {
		    if ($control == "enabled") exec("powercommander.lapcontrol powercommander PWM TAFEL SET ".$value);
		    $localstate["TAFEL_PWM"]=$value_dec;
		  }
	      }
	      break;
	    case "beamer":
	      {
		if($localstate["BEAMER_PWM"]!=$value_dec)
		  {
		    if ($control == "enabled") exec("powercommander.lapcontrol powercommander PWM BEAMER SET ".$value);
		    $localstate["BEAMER_PWM"]=$value_dec;
		  }
	      }
	      break;
	    case "schrank":
	      {
		if ( $localstate["SCHRANK_PWM"]!=$value_dec)
		  {
		    if ($control == "enabled") exec("powercommander.lapcontrol powercommander PWM SCHRANK SET ".$value);
		    $localstate["SCHRANK_PWM"]=$value_dec;
		  }
	      }
	      break;
	    case "flipper":
	      {
		if ( $localstate["FLIPPER_PWM"]!=$value_dec)
		  {
		    if ($control == "enabled") exec("powercommander.lapcontrol powercommander PWM FLIPPER SET ".$value);
		    $localstate["FLIPPER_PWM"]=$value_dec;
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