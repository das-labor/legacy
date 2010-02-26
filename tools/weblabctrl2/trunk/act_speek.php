<?
require_once "container.php";

$narf="1234567890";

if($_GET["doit"])
  {
    $myobj = new c_speek();
    $cmd = $myobj->isvalidmode($_GET["function"]);
    if($cmd=="text")
      {
	$tmpname = tempnam( "/tmp/", "weblab_speek");
	exec("echo ".escapeshellcmd(substr($_GET['value'],0,120))." | text2wave -o ".$tmpname.".wav &&  paplay -p -d lounge ".$tmpname.".wav && rm ".$tmpname." ".$tmpname.".wav");
      }

    file_put_contents($file, serialize( $localstate )); 
    
  }
?>