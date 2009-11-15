<?
session_id("nakka");
session_start();
#light
$pwm_ids=array("VORTRAG_PWM","TAFEL","BEAMER","SCHRANK","FLIPPER","LOUNGE","KUECHE");
$sw_ids=array("LAMP_VORTRAG_PWM","LAMP_TAFEL","LAMP_BEAMER","LAMP_SCHRANK","LAMP_FLIPPER","LAMP_LOUNGE","LAMP_KUECHE");
#mpd
$rooms=array("VORTRAG" => "6603","LOUNGE" => "6600","BASTEL" => "6602","KELLER" => 6601);
$mpd_cmd=array("pause","play","volume","next","prev");
$room_color=array("#0000dd","#eeee00","#dd0000");
?>
