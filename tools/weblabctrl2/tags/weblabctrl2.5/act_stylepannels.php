<?

require_once "container.php";

if($_GET["doit"])
  {
    $tmpobj = new c_stylepannels();
    $command = $tmpobj->isvalidmode($_GET["value"]);
    $js="";
    switch($command)
      {
      case "top":
	{
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
	      setcookie($key."_ypos_small", ($foo),time()+60*60*24*30);
	      setcookie($key."_xpos_small", ($foo2),time()+60*60*24*30);
	      $i++;
	    }
	}
	break;
      case "left":
	{
	  $i=0;

	  foreach($defaults as $key => $value)
	    {
	      $i++;
	      setcookie($key."_ypos_small", $i*25);
	      setcookie($key."_xpos_small",0);
	    }
	}
	break;
      }
    echo "<script>location.reload()</script>";
    file_put_contents($file, serialize( $localstate )); 
  }
?>