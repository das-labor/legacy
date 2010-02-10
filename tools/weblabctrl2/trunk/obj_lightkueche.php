<?
require_once "system_conf.php";
require_once "cl_base.php";

class c_lightkueche extends c_content{
  protected $dname;
  protected $actorfile="act_lightkueche.php";
  protected $updaterfile="updt_lightkueche.php";
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
    $this->validmodes=array("on","off","dimm");
    $this->initENV();
    # nothing
  }
  
  public function getObjName()
  {
    return $this->objname;
  }
  public function isvalidmode($mode)
  {
    if(in_array($mode,$this->validmodes)) return $mode;
    return "hacker";
  }

  protected function initENV()
  {
    $this->setobjname("lightkueche");
    $this->setdisplayname("Licht Kueche");
    $this->setsetFile("act_lightkueche.php");
    $this->setsetfilecmd("lightkueche");
    $this->setid("id_lightkueche");
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
    
    $this->content  = "<div>\n";
    $this->content .= "<div id=\"".$this->myid."_slider_status\" name=\"".$this->myid."_slider_status\"></div>";

    $this->content .= "<table>";
    $this->content .= "  <tr>";
    $this->content .= "    <td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/dotOFF.png\" ";
    $this->content .= "name=\"".$this->myid."_ko_img\"";
    $this->content .= "id=\"".$this->myid."_ko_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_ko(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    

    $this->content .= "	 <div id=\"".$this->myid."_slider\" style=\"width:235px; height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\">";
    $this->content .= "   <div class=\"handle\" style=\"width:15px; background-image:url(icons/1up.png); background-repeat:no-repeat; background-color: transparent; \"></div>";
    $this->content .= "  </div>";
    
    $this->content .= "  </tr>";
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

    $this->jsstr = "";

    $this->jsstr .= "\nfunction ".$this->myid."switch_ko()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_ko_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_ko_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_ko_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_ko_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_ko_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";



    $this->jsstr .= "var ".$this->myid."value=100;\n";

    $this->jsstr .= "    var ".$this->myid."_slider = \$('".$this->myid."_slider');\n";

    $this->jsstr .= "\n";
    $this->jsstr .= "    var ".$this->myid."_obj_slider = new Control.Slider(".$this->myid."_slider.select('.handle'), ".$this->myid."_slider, \n";
    $this->jsstr .= "		       {\n";
    $this->jsstr .= "			   range: \$R(0, 255),\n";
    $this->jsstr .= "			   increment: 1,\n";
    $this->jsstr .= "			   sliderValue: [100],\n";
    $this->jsstr .= "			   onSlide: function(values) \n";
    $this->jsstr .= "			       {\n";
    $this->jsstr .= "				   ".$this->myid."value=Math.round(values);\n";
    $this->jsstr .= "				   if(done)".$this->myid."_cmd('dimm',".$this->myid."value);\n";
    $this->jsstr .= "                              done=false;";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";

    $this->jsstr .= "\nfunction ".$this->myid."_cmd(funct,value)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '".$this->actorfile."?";
    $this->jsstr .= "doit=1&";
    $this->jsstr .= "function='+funct+'&";
    $this->jsstr .= $this->objname."value='+value,";
    $this->jsstr .= "{method:'get', evalScripts: true, onComplete:function() {done=true;}} );\n";
    $this->jsstr .= "}\n";

    

  }

  public function getupdaterjssec()
  {
    $updaterjs  = "";
    return $updaterjs;

  }

  public function getupdaterjs()
  {
    global $localstate;
    $updaterjs  = "";
    $updaterjs .= $this->myid."_obj_slider.setValue('".$localstate["KUECHE_PWM"]."'); \n";
    if( $localstate["SW_LAMP_KUECHE"]==1 ) 
      {
	$updaterjs .= $this->myid."button_show_on();";
      } 
    else 
      {
	$updaterjs .= $this->myid."button_show_off();";
      }
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