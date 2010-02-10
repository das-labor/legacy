<?
#was passiert wenn der updater greift?

require_once "container.php";

if($_GET["doit"]) 
  {
    $mytb = new c_treppenblink();
    echo "powercommander.lapcontrol treppenblink mode ".$mytb->isvalidmode($_GET["value"]);
    exec("powercommander.lapcontrol treppenblink mode ".$mytb->isvalidmode($_GET["value"]));
    file_put_contents($file, serialize( $localstate )); 
  }

?>