<?
require_once "system_conf.php";
require_once "container.php";


if($_GET["doit"])
  {
    $tmpobj = new c_lightbasteldetail();
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
	    case "banner":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_BANNER 1");
		$localstate["SW_LAMP_BANNER"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_t();\n";
		  }

	      }
	      break;
	    case "fenster":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_FENSTER 1");
		$localstate["SW_LAMP_FENSTER"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_b();\n";
		  }

	      }
	      break;
	    case "orgatisch":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_ORGATISCH 1");
		$localstate["SW_LAMP_ORGATISCH"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_s();\n";
		  }

	      }
	      break;
	    case "drucker1":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_DRUCKER_1 1");
		$localstate["SW_LAMP_DRUCKER_1"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
		  }

	      }
	      break;
	    case "drucker2":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_DRUCKER_2 1");
		$localstate["SW_LAMP_DRUCKER_2"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
		  }

	      }
	      break;
	    case "helmer1":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_HELMER_1 1");
		$localstate["SW_LAMP_HELMER_1"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
		  }

	      }
	      break;
	    case "helmer2":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_HELMER_2 1");
		$localstate["SW_LAMP_HELMER_2"]=1;
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
	    case "banner":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_BANNER 0");
		$localstate["SW_LAMP_BANNER"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_t();\n";
		  }

	      }
	      break;
	    case "fenster":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_FENSTER 0");
		$localstate["SW_LAMP_FENSTER"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_b();\n";
		  }

	      }
	      break;
	    case "orgatisch":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_ORGATISCH 0");
		$localstate["SW_LAMP_ORGATISCH"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_s();\n";
		  }

	      }
	      break;
	    case "drucker1":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_DRUCKER_1 0");
		$localstate["SW_LAMP_DRUCKER_1"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
		  }

	      }
	      break;
	    case "drucker2":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_DRUCKER_2 0");
		$localstate["SW_LAMP_DRUCKER_2"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
		  }

	      }
	      break;
	    case "helmer1":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_HELMER_1 0");
		$localstate["SW_LAMP_HELMER_1"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
		    $jsstr .= $myobj2->getid()."button_show_on_f();\n";
		  }

	      }
	      break;
	    case "helmer2":
	      {
		if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd SW LAMP_HELMER_2 0");
		$localstate["SW_LAMP_HELMER_2"]=1;
		foreach($sortedobjects[$tmpobj->getObjName()] as $myobj2)
		  {
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
      }
      break;
    case "dimm":
      {
	  switch($object)
	    {
	    case "fenster":
	      {
		if ($localstate["FENSTER_PWM"] != $value_dec)
		  {
		    if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd PWM FENSTER ".$value);
		    $localstate["FENSTER_PWM"]=$value_dec;
		  }
	      }
	      break;
	    case "banner":
	      {
		if($localstate["BANNER_PWM"]!=$value_dec)
		  {
		    if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd PWM BANNER ".$value);
		    $localstate["BANNER_PWM"]=$value_dec;
		  }
	      }
	      break;
	    case "orgatisch":
	      {
		if ( $localstate["ORGATISCH_PWM"]!=$value_dec)
		  {
		    if ($control == "enabled") exec("powercommander.lapcontrol bastelcmd PWM ORGATISCH ".$value);
		    $localstate["ORGATISCH_PWM"]=$value_dec;
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