<?
include_once "user_conf.php";

$i=0;

foreach($defaults as $key => $value)
{
  $i++;
  setcookie($key."_ypos_small", $i*25);
  setcookie($key."_xpos_small",0);
}

?>