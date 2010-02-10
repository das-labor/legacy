<?
require_once "container.php";

if($_GET["doit"])
  {
    $myobj = new c_irbeamer();
    $command = $myobj->isvalidmode($_GET["value"]);
    if($command == "0xFF")
      {
	exec("powercommander.lapcontrol powercommander SW PROJEKTOR ON 0x00");
	echo "powercommander.lapcontrol powercommander SW PROJEKTOR ON 0x00";
      }
    else 
      {
	echo "powercommander.lapcontrol canir beamer ".$command;
	exec("powercommander.lapcontrol canir beamer ".$command);
      }
    file_put_contents($file, serialize( $localstate ));     
  }
?>