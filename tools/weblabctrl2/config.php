<?
session_id("nakka");
session_start();
#light
$pwm_ids=array("VORTRAG_PWM","TAFEL","BEAMER","SCHRANK","FLIPPER","LOUNGE","KUECHE");
$sw_ids=array("LAMP_VORTRAG_PWM","LAMP_TAFEL","LAMP_BEAMER","LAMP_SCHRANK","LAMP_FLIPPER","LAMP_LOUNGE","LAMP_KUECHE");
$treppenblink_modes=array("blau" => "0x00", "stop" => "0x01", "weiss" => "0x02", "grau" => "0x03" , "aus" => "0x04", "gruen" => "0x05", "rot" => "0x06", "epi" => "0x07");
$canir_beamer_a=array("suspend" => "0x00", "scan" => "0x01");
$canir_beamer_a_channel=array("vga" => "0x02", "dvi" => "0x03" , "svideo" => "0x04", "component" => "0x05", "blank" => "0x06");
$canir_teufel_a=array("leiser" => "0x00", "lauter" => "0x01", "mute" => "0x02");
$canir_teufel_a_channel=array("main" => "0x03" , "front" => "0x04", "rear" => "0x05" , "side" => "0x06", "sub" => "0x07", "center" => "0x08");

#mpd
$rooms=array("VORTRAG" => "6603","LOUNGE" => "6600","BASTEL" => "6602","KELLER" => 6601);
$mpd_cmd=array("pause","play","volume","next","prev");
$room_color=array("#0000dd","#eeee00","#dd0000");
?>
