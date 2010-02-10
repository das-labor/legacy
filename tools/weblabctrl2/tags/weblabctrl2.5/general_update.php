<?
require_once "system_conf.php";
require_once "container.php";

if($_GET["update"])
  {

    global $localstate;
    $jsstr = "";

    foreach($objlist as $obj)
      {
	if(method_exists( $obj ,'getupdaterjs'))
	  $jsstr .= $obj->getupdaterjs();

      }



    echo "<script>".$jsstr."</script>";
    file_put_contents($file, serialize( $localstate )); 
#    var_dump($localstate);


  }
?>