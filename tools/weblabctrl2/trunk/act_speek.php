<?
require_once "container.php";

$narf="1234567890";

if($_GET["doit"])
  {
    $myobj = new c_speek();
    $cmd = $myobj->isvalidmode($_GET["function"]);
    $voice=$myobj->isvalidvoice($_GET["voice"]);
    if($cmd=="text")
      {
	$tmpname = tempnam( "/tmp/", "weblab_speek");
	$input = "\"
(Parameter.set 'Audio_Command \\\"cp \\\$FILE ".$tmpname.".wav\\\")
(Parameter.set 'Audio_Method 'Audio_Command)
(Parameter.set 'Audio_Required_Format 'snd)
(voice_".$voice.")
(SayText \\\"".escapeshellcmd(substr($_GET['value'],0,120))."\\\")\"";


#	exec("echo ".escapeshellcmd(substr($_GET['value'],0,120))." | text2wave -eval voice_init/festvial.scm -o ".$tmpname.".wav &&  );
	exec("echo ".$input." | festival &&  paplay -p -d lounge ".$tmpname.".wav && rm ".$tmpname." ".$tmpname.".wav");
      }

    file_put_contents($file, serialize( $localstate )); 
    
  }
?>