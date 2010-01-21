<?
session_id("weblabctrl2");
session_start();
#light
$pwm_ids=array("VORTRAG_PWM","TAFEL","BEAMER","SCHRANK","FLIPPER","LOUNGE","KUECHE");
$toilet_ids=array("TOILET_0","TOILET_1","TOILET_2","TOILET_4","TOILET_8");
$sw_ids=array("LAMP_VORTRAG_PWM","LAMP_TAFEL","LAMP_BEAMER","LAMP_SCHRANK","LAMP_FLIPPER","LAMP_LOUNGE","LAMP_KUECHE");
$treppenblink_modes=array("blau" => "0x00", "stop" => "0x01", "weiss" => "0x02", "grau" => "0x03" , "aus" => "0x04", "gruen" => "0x05", "rot" => "0x06", "hurtz" => "0x07");
$canir_beamer_a=array("suspend" => "0x00", "scan" => "0x01");
$canir_beamer_a_channel=array("vga" => "0x02", "dvi" => "0x03" , "svideo" => "0x04", "component" => "0x05", "blank" => "0x06");
$canir_teufel_a=array("leiser" => "0x00", "lauter" => "0x01", "mute" => "0x02");
$canir_teufel_a_channel=array("main" => "0x03" , "front" => "0x04", "rear" => "0x05" , "side" => "0x06", "sub" => "0x07", "center" => "0x08");

$background_images=array("eins","zwei","drei","vier","fuenf","sechs","sieben","acht","neun","zehn","dwarf");
#mpd
$rooms=array("VORTRAG" => "6603","LOUNGE" => "6600","BASTEL" => "6602","KELLER" => 6601);
$mpd_cmd=array("pause","play","volume","next","prev");
$room_color=array("#0000dd","#eeee00","#dd0000");

$GLN=array("VORTRAG_PWM" => "gesamter Vortragsraum","TAFEL" => "Tafellampe", "BEAMER" => "Beamerlampe", "SCHRANK" => "Schranklampe","FLIPPER" => "Flipperlampe", "LOUNGE" => "gesamte Lounge", "KUECHE" => "K&uuml;chlampe");


# restroing sessionvariables and setting defaults if missing 

$bg='dwarf';
$toiletlamp=0;
$pwmx=10;
$pwmy=60;
$mpdx=310;
$mpdy=60;
$miscx=570;
$miscy=180;
$vortragsdevsx=570;
$vortragsdevsy=60;
$treppenblinkx=830;
$treppenblinky=60;
$mybackgroundx=830;
$mybackgroundy=120;

if($_COOKIE['bg']) $bg=$_COOKIE['bg'];
if($_COOKIE['toiletlamp']) $toiletlamp=$_COOKIE['toiletlamp'];
if($_COOKIE['pwmx']) $pwmx=$_COOKIE['pwmx'];
if($_COOKIE['pwmy']) $pwmy=$_COOKIE['pwmy'];
if($_COOKIE['mpdx']) $mpdx=$_COOKIE['mpdx'];
if($_COOKIE['mpdy']) $mpdy=$_COOKIE['mpdy'];
if($_COOKIE['miscx']) $miscx=$_COOKIE['miscx'];
if($_COOKIE['miscy']) $miscy=$_COOKIE['miscy'];
if($_COOKIE['vortragsdevsx']) $vortragsdevsx=$_COOKIE['vortragsdevsx'];
if($_COOKIE['vortragsdevsy']) $vortragsdevsy=$_COOKIE['vortragsdevsy'];
if($_COOKIE['treppenblinkx']) $treppenblinkx=$_COOKIE['treppenblinkx'];
if($_COOKIE['treppenblinky']) $treppenblinky=$_COOKIE['treppenblinky'];
if($_COOKIE['mybackgroundx']) $mybackgroundx=$_COOKIE['mybackgroundx'];
if($_COOKIE['mybackgroundy']) $mybackgroundy=$_COOKIE['mybackgroundy'];


?>
