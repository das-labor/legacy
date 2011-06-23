<?
require_once "container.php";

$narf="1234567890";

if($_GET["doit"])
  {
    $myobj = new c_jingels();
#    var_dump($myobj->getValidModes());
    $cmd = $myobj->isvalidmode($myobj->getBasedir()."/".$_GET["subdir"]."/".$_GET["file"]);
    $stage = $myobj->isvalidstage($_GET["stage"]);
    if( $cmd !== "hacker") {
      exec("(./exec_timeout.sh 60 mplayer -ao pulse:kvm:".$stage." '".$myobj->getBasedir()."/".$_GET["subdir"]."/".$_GET["file"]."') &");
    }
    file_put_contents($file, serialize( $localstate )); 
   
  }
echo "<html>";
echo "<meta http-equiv=\"refresh\" content=\"0; URL=http://".$_SERVER['SERVER_NAME']."/weblabctrl/\">";
echo "<body>";
echo "<pre>";
echo "</pre>";
echo "</body>";
echo "</html>";

?>