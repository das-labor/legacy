<?
require_once "cl_base.php";

class c_moodbar extends c_content{
  protected $dname;
  protected $actorfile="act_moodbar.php";
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
    $this->validmodes=array("color","fade");
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
    $this->setobjname("moodbar");
    $this->setdisplayname("Moodbar");
    $this->setsetFile("act_moodbar.php");
    $this->setsetfilecmd("moodbar");
    $this->setid("id_moodbar");
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
    $this->content  = "<div>";
    $this->content .= "	 <div id=\"".$this->myid."r_slider\" style=\"height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\">";
    $this->content .= "   <div class=\"handle\" style=\"background-color: #f00;\"></div>";
    $this->content .= "  </div>";
    $this->content .= "  <div id=\"".$this->myid."g_slider\" style=\"height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\">";
    $this->content .= "    <div class=\"handle\" style=\"background-color: #0f0;\"></div>";
    $this->content .= "  </div>";
    $this->content .= "  <div id=\"".$this->myid."b_slider\" style=\"height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\">";
    $this->content .= "    <div class=\"handle\" style=\"background-color: #00f;\"></div>";
    $this->content .= "  </div>";

    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/fadeon.png\" ";
    $this->content .= "name=\"".$this->myid."_fo_img\"";
    $this->content .= "id=\"".$this->myid."_fo_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_fo(); ".$this->myid."_cmd('fade',0,0,0);\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_fo();\" >";

    $this->content .= "</div>";

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
    #
  }

  protected function initJS()
  {

    $this->jsstr = "";

    $this->jsstr .= "function ".$this->myid."switch_fo() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_fo_img\").src='icons/fadeon_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_fo_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_fo() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_fo_img\").src='icons/fadeon_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_fo_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_fo_img\").src='icons/fadeon.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "var ".$this->myid."r=100;\n";
    $this->jsstr .= "var ".$this->myid."g=100;\n";
    $this->jsstr .= "var ".$this->myid."b=100;\n";

    //    $this->jsstr .= "(function() {\n";
    $this->jsstr .= "    var ".$this->myid."r_slider = \$('".$this->myid."r_slider');\n";
    $this->jsstr .= "    var ".$this->myid."g_slider = \$('".$this->myid."g_slider');\n";
    $this->jsstr .= "    var ".$this->myid."b_slider = \$('".$this->myid."b_slider');\n";

    $this->jsstr .= "\n";
    $this->jsstr .= "    new Control.Slider(".$this->myid."r_slider.select('.handle'), ".$this->myid."r_slider, \n";
    $this->jsstr .= "		       {\n";
    $this->jsstr .= "			   range: \$R(0, 255),\n";
    $this->jsstr .= "			   increment: 16,\n";
    $this->jsstr .= "			   sliderValue: [100],\n";
    $this->jsstr .= "			   onSlide: function(values) \n";
    $this->jsstr .= "			       {\n";
    $this->jsstr .= "				   ".$this->myid."r=Math.round(values);\n";
    $this->jsstr .= "				   ".$this->myid."_cmd('color',".$this->myid."r,".$this->myid."g,".$this->myid."b);\n";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";
    $this->jsstr .= "    new Control.Slider(".$this->myid."g_slider.select('.handle'), ".$this->myid."g_slider, {\n";
    $this->jsstr .= "                      range: \$R(0, 255),\n";
    $this->jsstr .= "		           increment: 16,\n";
    $this->jsstr .= "                      sliderValue: [100],\n";
    $this->jsstr .= "                      onSlide: function(values) {\n";
    $this->jsstr .= "			           ".$this->myid."g=Math.round(values);\n";
    $this->jsstr .= "			           ".$this->myid."_cmd('color',".$this->myid."r,".$this->myid."g,".$this->myid."b);\n";
    $this->jsstr .= "                          }\n";
    $this->jsstr .= "                  });\n";
    $this->jsstr .= "\n";
    $this->jsstr .= "    new Control.Slider(".$this->myid."b_slider.select('.handle'), ".$this->myid."b_slider, {\n";
    $this->jsstr .= "                      range: \$R(0, 255),\n";
    $this->jsstr .= "		           increment: 16,\n";
    $this->jsstr .= "                      sliderValue: [100],\n";
    $this->jsstr .= "                      onSlide: function(values) {\n";
    $this->jsstr .= "			           ".$this->myid."b=Math.round(values);\n";
    $this->jsstr .= "			           ".$this->myid."_cmd('color',".$this->myid."r,".$this->myid."g,".$this->myid."b);\n";
    $this->jsstr .= "                          }\n";
    $this->jsstr .= "                  });\n";
    //    $this->jsstr .= "  })();\n";

    $this->jsstr .= "\nfunction ".$this->myid."_cmd(funct,r,g,b)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '".$this->actorfile."?doit=1&function='+funct+'&".$this->myid."r='+r+'&".$this->myid."g='+g+'&".$this->myid."b='+b,{method:'get', onComplete:function() {done=true;}} );\n";
    $this->jsstr .= "}\n";

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