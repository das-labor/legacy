<?

if($_GET[height] == '')
  {
    setcookie($_GET[name]."_closed","open",time()+60*60*24*30);
  }
else
  {
    setcookie($_GET[name]."_closed","closed");
  }

setcookie($_GET[name]."_xpos_small",$_GET[xpos_small],time()+60*60*24*30);
    setcookie($_GET[name]."_ypos_small",$_GET[ypos_small],time()+60*60*24*30);
    setcookie($_GET[name]."_xpos",$_GET[xpos],time()+60*60*24*30);
    setcookie($_GET[name]."_ypos",$_GET[ypos],time()+60*60*24*30);

?>