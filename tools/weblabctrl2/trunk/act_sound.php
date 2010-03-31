<?
require_once "container.php";

$narf="1234567890";

if($_GET["doit"])
  {
    $myobj = new c_speek();
    $cmd = $myobj->isvalidmode($_GET["function"]);
    $voice=$myobj->isvalidvoice($_GET["voice"]);
    $tmpname = tempnam( "/tmp/", "weblab_sound");
    $filesize = $_FILES['userfile']['size'];
    var_dump($_FILES);
    if($filesize < (256*1024*1024)){
      if (move_uploaded_file($_FILES['userfile']['tmp_name'], $tmpname)) {
#	exec("echo ".escapeshellcmd(substr($_GET['value'],0,120))." | text2wave -eval voice_init/festvial.scm -o ".$tmpname.".wav &&  );
	exec("(./exec_timeout.sh 15 paplay --device vortrag ".$tmpname." && rm ".$tmpname.")&");
      }
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