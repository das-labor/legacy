<?
include_once "user_conf.php";

$i=0;
$k=0;

foreach($defaults as $key => $value)
{
  if($i%4==0)
    { 
      $i=0; 
      $k++; 
    }
  $foo=$k*25-20;
  $foo2=$i*280;
  setcookie($key."_ypos_small", ($foo));
  setcookie($key."_xpos_small", ($foo2));
  $i++;
}

?>