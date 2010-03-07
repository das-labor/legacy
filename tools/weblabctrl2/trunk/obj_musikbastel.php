<?
require_once "cl_base.php";

class c_musikbastel extends c_content{
  protected $dname;
  protected $actorfile="act_musikbastel.php";
  protected $updaterfile="updt_musikbastel.php";
  protected $content;
  protected $divstr;
  protected $cssstr;
  protected $setfile;
  protected $myid;
  protected $setfilecmd;
  protected $objname;
  protected $valimodes;
  function __construct() {
    $this->divstr = "";
    $this->cssstr = "";
    $this->jsstr = "";
    $this->content = "";
    $this->setfile = "";
    $this->setfilecmd = "";
    $this->validmodes=array("current","play","stop","pause","prev","next","seek","playlist","volume");
    $this->initENV();
    # nothing
  }
  
  public function isvalidmode($mode)
  {
    if(in_array($mode,$this->validmodes)) return $mode;
    return "hacker";
  }

  protected function initENV()
  {
    $this->setobjname("musikbastel");
    $this->setdisplayname("Musik Bastel");
    $this->setsetFile("act_musikbastel.php");
    $this->setsetfilecmd("musikbastel");
    $this->setid("id_musikbastel");
  }
  public function setsetFile($newsetfile)
  {
    $this->setfile = $newsetfile;
    $this->initContent();
    $this->initCSS();
    $this->initJS();
  }

  public function setobjname($newobjname)
  {
    $this->objname = $newobjname;
    $this->initContent();
    $this->initCSS();
    $this->initJS();
  }

  public function setsetfilecmd($newsetfilecmd)
  {
    $this->setfilecmd = $newsetfilecmd;
    $this->initContent();
    $this->initCSS();
    $this->initJS();
  }

  public function setid($newid)
  {
    $this->myid = $newid;
    $this->initContent();
    $this->initCSS();
    $this->initJS();
  }

  public function getSysConf()
  {
    # system conf is which mode is selectet
    return $this->sysconf;
  }

  public function setSysConf($sysconf)
  {
    $this->sysconf=$sysconf;
  }

  public function getClientConf()
  {
    #currently no clientconf
    $this->clientconf="";
    return $this->clientconf;
  }

  public function setClientConf($clientconf)
  {
    # currently no clientconf
    #$this->clientconf=$clientconf;
  }

  protected function initContent()
  {
    
    $fkttmp=array("play","stop","pause","prev","next");

    $this->content  = "<div style=\"margin:5px; padding: 5px; background-image:url(icons/subpanelbg.png);\">\n";
    $this->content .= "<div id=\"".$this->myid."_slider_status\" name=\"".$this->myid."_slider_status\"></div>";
    $this->content .= "<div name=\"".$this->myid."title\" id=\"".$this->myid."title\" style=\"font-size:small\"> </div>";
    $this->content .= "<table cellpassing=0 cellspacing=0 border=0>";
    $this->content .= "<tr>";
    $this->content .= "    <td colspan=\"5\">";
    $this->content .= "<div name=\"".$this->myid."playpos\" id=\"".$this->myid."playpos\" style=\"font-size:small\"> </div>";
    $this->content .= "</td></tr>";
    $this->content .= "<tr>";
    $this->content .= "     <td colspan=\"5\">";
    

    $this->content .= "<div id=\"".$this->myid."_slider_seek\" style=\"height:10px; background-image:url(icons/sliderbg2.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider2\">";
    $this->content .= "<div class=\"handle\" style=\"width:35px; background-image:url(icons/seek.png); background-repeat:no-repeat; background-color: transparent; \"></div>";
    $this->content .= "</div>";
    $this->content .= "</td>";
    $this->content .= "  </tr>";
    $this->content .= "  <tr>";
    foreach($fkttmp as $fktkey)
      {
	$this->content .= "    <td>";
	$this->content .= "<input type=\"image\" ";
	$this->content .= "src=\"icons/".$fktkey."_h.png\" ";
	$this->content .= "name=\"".$this->myid."_".$fktkey."_img\"";
	$this->content .= "id=\"".$this->myid."_".$fktkey."_img\"";
	$this->content .= "onclick=\"".$this->myid."switch_".$fktkey."(); \" ";
	$this->content .= "style=\"background-color: transparent;\">";
	$this->content .= "    </td>";
      }
      
    $this->content .= "   </tr>";
    $this->content .= "<tr>";
    $this->content .= "     <td colspan=\"5\">";

    $this->content .= "<div id=\"".$this->myid."_slider_vol\" style=\"background-image:url(sliderbg_vol.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider2\">";
    $this->content .= "<div class=\"handle\" style=\"width:20px; background-image:url(icons/volume.png); background-repeat:no-repeat; background-color: transparent; \"></div>";
    $this->content .= "</div>";
    $this->content .= "</td>";
    $this->content .= "   </tr>";
    $this->content .= "</table>";


    $this->content .= "</div>\n";


  }

  protected function initCSS()
  {
    $this->cssstr = "";
    $this->cssstr .= "div.slider { width:256px; \n";
    $this->cssstr .= "  margin:10px 0; \n";
    $this->cssstr .= "  background-color:#ccc; \n";
    $this->cssstr .= "  height:10px; \n";
    $this->cssstr .= "  position: relative; }\n";
    $this->cssstr .= "div.slider div.handle { width:10px; \n";
    $this->cssstr .= "  height:15px; \n";
    $this->cssstr .= "  background-color:#f00; \n";
    $this->cssstr .= "  cursor:move; \n";
    $this->cssstr .= "  position: absolute; }\n";

  }

  protected function initJS()
  {
    $fkttmp=array("play","stop","pause","prev","next");

    $this->jsstr = "";

    foreach($fkttmp as $fktkey)
      {
	$this->jsstr .= "\nfunction ".$this->myid."switch_".$fktkey."()\n";
	$this->jsstr .= "{\n";
	$this->jsstr .= "  if(document.getElementById(\"".$this->myid."_play_img\").src.substr(".-1*strlen($fktkey.'_h.png').",">strlen($fktkey.'_h.png').")==\"".$fktkey."_h.png\")";
	$this->jsstr .= "  {\n";
	$this->jsstr .= $this->myid."button_".$fktkey."_on();\n";
	$this->jsstr .= "  } \n";
	$this->jsstr .= "}\n";
	$this->jsstr .= "\nfunction ".$this->myid."button_".$fktkey."_on()\n";
	$this->jsstr .= "{\n";
	$this->jsstr .=      $this->myid."_cmd('".$fktkey."',0);\n";
	$this->jsstr .=      $this->myid."button_show_".$fktkey."_on();\n";
	if( $fktkey == "prev" or $fktkey == "next")
	  {
	    $this->jsstr .=      "setTimeout(".$this->myid."button_show_".$fktkey."_off(),10000);\n";
	  }
	$this->jsstr .= "}\n";
	$this->jsstr .= "\nfunction ".$this->myid."button_show_".$fktkey."_on()\n";
	$this->jsstr .= "{\n";
	$this->jsstr .= "    document.getElementById(\"".$this->myid."_".$fktkey."_img\").src=\"icons/".$fktkey."_l.png\"; \n";
	$this->jsstr .= "}\n";
	$this->jsstr .= "\nfunction ".$this->myid."button_show_".$fktkey."_off()\n";
	$this->jsstr .= "{\n";
	$this->jsstr .= "    document.getElementById(\"".$this->myid."_".$fktkey."_img\").src=\"icons/".$fktkey."_h.png\";\n";
	$this->jsstr .= "}\n";
      }


    $this->jsstr .= "function ".$this->myid."settitle(value){";
    $this->jsstr .= "document.getElementById (\"".$this->myid."title\").firstChild.data = value;";
    $this->jsstr .= "}";

    $this->jsstr .= "function ".$this->myid."setplaypos(value){";
    $this->jsstr .= "document.getElementById (\"".$this->myid."playpos\").firstChild.data = value;";
    $this->jsstr .= "}";


    $this->jsstr .= "var ".$this->myid."value_vol=50;\n";

    $this->jsstr .= "    var ".$this->myid."_slider_vol = \$('".$this->myid."_slider_vol');\n";

    $this->jsstr .= "\n";
    $this->jsstr .= "    var ".$this->myid."_obj_slider_vol = new Control.Slider(".$this->myid."_slider_vol.select('.handle'), ".$this->myid."_slider_vol, \n";
    $this->jsstr .= "		       {\n";
    $this->jsstr .= "			   range: \$R(0, 100),\n";
    $this->jsstr .= "			   increment: 5,\n";
    $this->jsstr .= "			   sliderValue: [50],\n";
    $this->jsstr .= "                      values: new Array(0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100),\n";
    $this->jsstr .= "			   onSlide: function(values) \n";
    $this->jsstr .= "			       {\n";
    $this->jsstr .= "			           value=Math.round(values);\n";
    $this->jsstr .= "				   if(done)".$this->myid."_cmd('volume',value);\n";
    $this->jsstr .= "				   done=false;";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";


    $this->jsstr .= "var ".$this->myid."value_seek=0;\n";

    $this->jsstr .= "    var ".$this->myid."_slider_seek = \$('".$this->myid."_slider_seek');\n";

    $this->jsstr .= "\n";
    $this->jsstr .= "    var ".$this->myid."_obj_slider_seek = new Control.Slider(".$this->myid."_slider_seek.select('.handle'), ".$this->myid."_slider_seek, \n";
    $this->jsstr .= "		       {\n";
    $this->jsstr .= "			   range: \$R(0, 100),\n";
    $this->jsstr .= "			   increment: 5,\n";
    $this->jsstr .= "			   sliderValue: [0],\n";
    $this->jsstr .= "                      values: new Array(0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100),\n";
    $this->jsstr .= "			   onSlide: function(values) \n";
    $this->jsstr .= "			       {\n";
    $this->jsstr .= "				   ".$this->myid."_cmd('seek',values);\n";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";


    $this->jsstr .= "\nfunction ".$this->myid."_cmd(funct,value)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '";
    $this->jsstr .= $this->actorfile."?";
    $this->jsstr .= "myid=".$this->myid."&";
    $this->jsstr .= "doit=1&";
    $this->jsstr .= "function='+funct+'&";
    $this->jsstr .= $this->objname."value='+value,";
    $this->jsstr .= "{method:'get', evalScripts: true, onComplete:function() {done=true;}} );\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\n";

  }

  public function getupdaterjs()
  {
    global $localstate;
    $updaterjs  = "";

    if( (!isset($localstate["musik_bastel_last"])) ||
	($localstate["musik_bastel_last"]+10 < time()) ||
	(!isset($localstate["musik_bastel_status"])))
      {
	  unset($status);
  
	  exec("MPD_PORT=6602 MPD_HOST=10.0.1.3 mpc current",$status);
	
	  $titel = $status[0];
	  $localstate["musik_bastel_title"] = $status[0];

	  unset($status);

	  exec("MPD_PORT=6602 MPD_HOST=10.0.1.3 mpc volume",$status);
  
	  $volume = sscanf($status[0],"volume: %d");
	
	  unset($status);
    
	  exec("MPD_PORT=6602 MPD_HOST=10.0.1.3 mpc",$status);

	  if(strcmp($status[0],$titel)==0) 
	    {
	      $indata = sscanf  ( $status[1] , "%s\t%s\t%d:%d/%d:%d\t(%d" );
	      $localstate["musik_bastel_status"]=substr($indata[0],1,-1);
	      $localstate["musik_bastel_current"]=$indata[2].":".$indata[3];
	      $localstate["musik_bastel_end"]=$indata[4].":".$indata[5];
	      $localstate["musik_bastel_pos"]=$indata[6];
	    }
	  else 
	    {
	      $localstate["musik_bastel_current"]="0:0";
	      $localstate["musik_bastel_end"]="0:0";
	      $localstate["musik_bastel_pos"]=0;
	      $localstate["musik_bastel_status"]="stop";
	    }
	  $localstate["musik_bastel_volume"]=$volume[0];
	  $localstate["musik_bastel_last"]=time();
      }

    if($localstate["musik_bastel_status"]=="playing")
      { 
	$updaterjs .= $this->myid."button_show_play_on();\n";
	$updaterjs .= $this->myid."button_show_pause_off();\n";
	$updaterjs .= $this->myid."button_show_stop_off();\n";
      }    

    if($localstate["musik_bastel_status"]=="stop")
      { 
	$updaterjs .= $this->myid."button_show_play_off();\n";
	$updaterjs .= $this->myid."button_show_pause_off();\n";
	$updaterjs .= $this->myid."button_show_stop_on();\n";
      }	

    if($localstate["musik_bastel_status"]=="paused")
      { 
	$updaterjs .= $this->myid."button_show_play_off();\n";
	$updaterjs .= $this->myid."button_show_pause_on();\n";
	$updaterjs .= $this->myid."button_show_stop_off();\n";
      }	
    
    $updaterjs .= $this->myid."_obj_slider_vol.setValue('".$localstate["musik_bastel_volume"]."');\n";
    $updaterjs .= $this->myid."_obj_slider_seek.setValue('".$localstate["musik_bastel_pos"]."');\n";

    $updaterjs .= $this->myid."settitle(\"".htmlspecialchars($localstate["musik_bastel_title"])."\");\n";
    $updaterjs .= $this->myid."setplaypos(\"".htmlspecialchars($localstate["musik_bastel_current"])."/".htmlspecialchars($localstate["musik_lounge_end"])."\");\n";

  
    return $updaterjs;
  }

  public function getupdaterjssec()
  {
    $updaterjs = "";

    return $updaterjs;
  }

  
  public function getcss(){
    return $this->cssstr;
  }
  public function getjs(){
    return $this->jsstr;
  }

  public function getcontent()
  {
    return $this->content;
  }

  public function getid()
  {
    return $this->myid;
  }

  public function getObjName()
  {
    return $this->objname;
  }

  public function setdisplayname($newdisplayname)
  {
    $this->dname=$newdisplayname;
  }
  public function getdname()
  {
    return $this->dname;
  }
  

}

?>
