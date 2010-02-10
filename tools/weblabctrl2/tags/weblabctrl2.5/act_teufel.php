<?

require_once "container.php";

if($_GET["doit"])
  {
    $myobj = new c_irteufel();
    $command = $myobj->isvalidmode($_GET["value"]);
    echo "powercommander.lapcontrol canir teufel ".$command;
    exec("powercommander.lapcontrol canir teufel ".$command);
    file_put_contents($file, serialize( $localstate )); 
  }
?>