<?

require_once "container.php";


if($_GET["doit"])
  {
    $tmpobj = new c_stylebg();
    $command = $tmpobj->isvalidmode($_GET["value"]);

    if($command != "hacker")
      {
	setcookie("backgroundimage",$command,time()+60*60*24*30);
	$jsstr = "document.getElementById(\"bg\").src=\"bg/".$command.".jpg\"";
	echo "<script>".$jsstr."</script>";	
      }
    file_put_contents($file, serialize( $localstate )); 
  }
?>