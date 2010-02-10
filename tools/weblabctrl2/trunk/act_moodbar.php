<?
require_once "container.php";

//var_dump($_GET);

if($_GET["doit"])
  {

    $mymood = new c_moodbar();
  
    $myfunction = $mymood->isvalidmode($_GET["function"]); 

    $mood_green=0;
    $mood_blue=0;
    $mood_red=0;
    if( ( $_GET[$mymood->getid()."b"] >=0 ) && 
	( $_GET[$mymood->getid()."b"] <=255 ) &&
	( $_GET[$mymood->getid()."r"] >=0 ) && 
	( $_GET[$mymood->getid()."r"] <=255 ) &&
	( $_GET[$mymood->getid()."g"] >=0 ) && 
	( $_GET[$mymood->getid()."g"] <=255) )
      {
	$mood_blue=255-$_GET[$mymood->getid()."b"];
	$mood_red=255-$_GET[$mymood->getid()."r"];
	$mood_green=255-$_GET[$mymood->getid()."g"];
      }
    else 
      {
	$mymode = "hacker";
      }

    switch ( $myfunction ) {
    case "color":
      {
	//	echo "powercommander.lapcontrol packet 0x00:0x00 0x31:0x17 0x03,0x00 && powercommander.lapcontrol packet 0x00:0x00 0x31:0x17 0x02,0x00,".$mood_blue.",".$mood_green.",".$mood_red; // stop mood
	exec("powercommander.lapcontrol packet 0x00:0x00 0x31:0x17 0x03,0x00 && powercommander.lapcontrol packet 0x00:0x00 0x31:0x17 0x02,0x00,".$mood_blue.",".$mood_green.",".$mood_red); // stop mood
      }
      break;
    case "fade":
      {
	//echo "powercommander.lapcontrol packet 0x00:0x00 0x31:0x17 0x03,0x01"; // enable mood
	exec("powercommander.lapcontrol packet 0x00:0x00 0x31:0x17 0x03,0x01"); // enable mood
      }
      break;
    case "hacker" :
      {
	echo "I think it's a hacker";
      }
    default:
      {
	echo "Idiot!";
      }
    }
    file_put_contents($file, serialize( $localstate )); 
  }

?>