<?
require_once "cl_base.php";

class c_lightvortragdetail extends c_content{
  protected $dname;
  protected $actorfile="act_lightvortragdetail.php";
  protected $updatefile="updt_lightvortragdetail.php";
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
    $this->validobjs=array("tafel","beamer","schrank","flipper");

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

  public function isvalidobj($obj)
  {
    if(in_array($obj,$this->validobjs)) return $obj;
    return "hacker";
  }

  protected function initENV()
  {
    $this->setobjname("lightvortragdetail");
    $this->setdisplayname("Licht Vortragsraum Detail");
    $this->setsetFile("act_lightvortragdetail.php");
    $this->setsetfilecmd("lightvortragdetail");
    $this->setid("id_lightvortragdetail");
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
# tafel    
    $this->content  = "<div>\n";
    $this->content .= "<div id=\"".$this->myid."_slider_status\" name=\"".$this->myid."_slider_t_status\"></div>";

    $this->content .= "<table>";
    $this->content .= "  <tr>";
    $this->content .= "    <td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/dotOFF.png\" ";
    $this->content .= "name=\"".$this->myid."_to_img\"";
    $this->content .= "id=\"".$this->myid."_to_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_to(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    
    $this->content .= "	 <div id=\"".$this->myid."_slider_t\" style=\"width:235px; height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\">";
    $this->content .= "   <div class=\"handle\" style=\"width:15px; background-image:url(icons/1up.png); background-repeat:no-repeat; background-color: transparent; \"></div><div style=\"position:relative; top:-15px; left:170px;\">Tafel</div>";
    $this->content .= "  </div>";
    
    $this->content .= "  </tr>";
    $this->content .= "</table>";

# beamer


    $this->content .= "<table>";
    $this->content .= "  <tr>";
    $this->content .= "    <td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/dotOFF.png\" ";
    $this->content .= "name=\"".$this->myid."_bo_img\"";
    $this->content .= "id=\"".$this->myid."_bo_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_bo(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    
    $this->content .= "	 <div id=\"".$this->myid."_slider_b\" style=\"width:235px; height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\">";
    $this->content .= "   <div class=\"handle\" style=\"width:15px; background-image:url(icons/1up.png); background-repeat:no-repeat; background-color: transparent; \"></div><div style=\"position:relative; top:-15px; left:170px;\">Beamer</div>";
    $this->content .= "  </div>";
    
    $this->content .= "  </tr>";
    $this->content .= "</table>";

# schrank

    $this->content .= "<table>";
    $this->content .= "  <tr>";
    $this->content .= "    <td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/dotOFF.png\" ";
    $this->content .= "name=\"".$this->myid."_so_img\"";
    $this->content .= "id=\"".$this->myid."_so_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_so(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    
    $this->content .= "	 <div id=\"".$this->myid."_slider_s\" style=\"width:235px; height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\">";
    $this->content .= "   <div class=\"handle\" style=\"width:15px; background-image:url(icons/1up.png); background-repeat:no-repeat; background-color: transparent; \"></div><div style=\"position:relative; top:-15px; left:170px;\">Schrank</div>";
    $this->content .= "  </div>";
    
    $this->content .= "  </tr>";
    $this->content .= "</table>";

# flipper

    $this->content .= "<table>";
    $this->content .= "  <tr>";
    $this->content .= "    <td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/dotOFF.png\" ";
    $this->content .= "name=\"".$this->myid."_fo_img\"";
    $this->content .= "id=\"".$this->myid."_fo_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_fo(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    
    $this->content .= "	 <div id=\"".$this->myid."_slider_f\" style=\"width:235px; height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\">";
    $this->content .= "   <div class=\"handle\" style=\"width:15px; background-image:url(icons/1up.png); background-repeat:no-repeat; background-color: transparent; \"></div><div style=\"position:relative; top:-15px; left:170px;\">Flipper</div>";
    $this->content .= "  </div>";
    
    $this->content .= "  </tr>";
    $this->content .= "</table>";


    $this->content .= "</div>\n";

  }

  protected function initCSS()
  {
    $this->cssstr = "";
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

    $this->jsstr .= "\nfunction ".$this->myid."switch_to()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_to_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on_t();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off_t();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on_t()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('tafel','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_to_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_t()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_to_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_t()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('tafel','off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_to_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off_t()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_to_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";



    $this->jsstr .= "var ".$this->myid."_tvalue=100;\n";

    $this->jsstr .= "    var ".$this->myid."_slider_t = \$('".$this->myid."_slider_t');\n";

    $this->jsstr .= "\n";
    $this->jsstr .= "    var ".$this->myid."_obj_slider_t = new Control.Slider(".$this->myid."_slider_t.select('.handle'), ".$this->myid."_slider_t, \n";
    $this->jsstr .= "		       {\n";
    $this->jsstr .= "			   range: \$R(0, 255),\n";
    $this->jsstr .= "			   increment: 1,\n";
    $this->jsstr .= "			   sliderValue: [100],\n";
    $this->jsstr .= "			   onSlide: function(values) \n";
    $this->jsstr .= "			       {\n";
    $this->jsstr .= "				   ".$this->myid."_tvalue=Math.round(values);\n";
    $this->jsstr .= "				   if(done)".$this->myid."_cmd('tafel','dimm',".$this->myid."_tvalue);\n";
    $this->jsstr .= "                              done=false;\n";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";


#beamer


    $this->jsstr .= "\nfunction ".$this->myid."switch_bo()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_bo_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on_b();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off_b();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on_b()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('beamer','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_bo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_b()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_bo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_b()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('beamer','off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_bo_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off_b()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_bo_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";



    $this->jsstr .= "var ".$this->myid."_bvalue=100;\n";

    $this->jsstr .= "    var ".$this->myid."_slider_b = \$('".$this->myid."_slider_b');\n";

    $this->jsstr .= "\n";
    $this->jsstr .= "    var ".$this->myid."_obj_slider_b = new Control.Slider(".$this->myid."_slider_b.select('.handle'), ".$this->myid."_slider_b, \n";
    $this->jsstr .= "		       {\n";
    $this->jsstr .= "			   range: \$R(0, 255),\n";
    $this->jsstr .= "			   increment: 1,\n";
    $this->jsstr .= "			   sliderValue: [100],\n";
    $this->jsstr .= "			   onSlide: function(values) \n";
    $this->jsstr .= "			       {\n";
    $this->jsstr .= "				   ".$this->myid."_bvalue=Math.round(values);\n";
    $this->jsstr .= "				   if(done)".$this->myid."_cmd('beamer','dimm',".$this->myid."_bvalue);\n";
    $this->jsstr .= "                              done=false;";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";


# Schrank


    $this->jsstr .= "\nfunction ".$this->myid."switch_so()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_so_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on_s();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off_s();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on_s()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('schrank','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_so_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_s()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_so_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_s()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('schrank','off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_so_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off_s()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_so_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";



    $this->jsstr .= "var ".$this->myid."_svalue=100;\n";

    $this->jsstr .= "    var ".$this->myid."_slider_s = \$('".$this->myid."_slider_s');\n";

    $this->jsstr .= "\n";
    $this->jsstr .= "    var ".$this->myid."_obj_slider_s = new Control.Slider(".$this->myid."_slider_s.select('.handle'), ".$this->myid."_slider_s, \n";
    $this->jsstr .= "		       {\n";
    $this->jsstr .= "			   range: \$R(0, 255),\n";
    $this->jsstr .= "			   increment: 1,\n";
    $this->jsstr .= "			   sliderValue: [100],\n";
    $this->jsstr .= "			   onSlide: function(values) \n";
    $this->jsstr .= "			       {\n";
    $this->jsstr .= "				   ".$this->myid."_svalue=Math.round(values);\n";
    $this->jsstr .= "				   if(done)".$this->myid."_cmd('schrank','dimm',".$this->myid."_svalue);\n";
    $this->jsstr .= "                              done=false;";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";



# flipper
    $this->jsstr .= "\nfunction ".$this->myid."switch_fo()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_fo_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on_f();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off_f();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on_f()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('flipper','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_fo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_f()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_fo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_f()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('flipper','off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_fo_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off_f()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_fo_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";



    $this->jsstr .= "var ".$this->myid."_fvalue=100;\n";

    $this->jsstr .= "    var ".$this->myid."_slider_f = \$('".$this->myid."_slider_f');\n";

    $this->jsstr .= "\n";
    $this->jsstr .= "    var ".$this->myid."_obj_slider_f = new Control.Slider(".$this->myid."_slider_f.select('.handle'), ".$this->myid."_slider_f, \n";
    $this->jsstr .= "		       {\n";
    $this->jsstr .= "			   range: \$R(0, 255),\n";
    $this->jsstr .= "			   increment: 1,\n";
    $this->jsstr .= "			   sliderValue: [100],\n";
    $this->jsstr .= "			   onSlide: function(values) \n";
    $this->jsstr .= "			       {\n";
    $this->jsstr .= "				   ".$this->myid."_fvalue=Math.round(values);\n";
    $this->jsstr .= "				   if(done)".$this->myid."_cmd('flipper','dimm',".$this->myid."_fvalue);\n";
    $this->jsstr .= "                              done=false;";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";

#

    $this->jsstr .= "\nfunction ".$this->myid."_cmd(object,funct,value)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '".$this->actorfile."?doit=1&object='+object+'&function='+funct+'&".$this->myid."value='+value,{method:'get', evalScripts: true, onComplete:function() {done=true;}} );\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\n";
    

  }

  public function getupdaterjssec()
  {
    $updaterjs  = "";
    return $updaterjs;

  }


  public function getupdaterjs()
  {
    global $localstate;
    $updaterjs = "";
    $updaterjs .= $this->myid."_obj_slider_t.setValue('".$localstate["TAFEL_PWM"]."'); \n";
    if( $localstate["SW_LAMP_TAFEL"]==1 ) 
      {
	$updaterjs .= $this->myid."button_show_on_t();";
      } 
    else 
      {
	    $updaterjs .= $this->myid."button_show_off_t();";
      }

    $updaterjs .= $this->myid."_obj_slider_b.setValue('".$localstate["BEAMER_PWM"]."'); \n";
    if($localstate["SW_LAMP_BEAMER"]==1)
      {
	$updaterjs .= $this->myid."button_show_on_b();";
      } 
    else 
      {
	$updaterjs .= $this->myid."button_show_off_b();";
      }

    $updaterjs .= $this->myid."_obj_slider_s.setValue('".$localstate["SCHRANK_PWM"]."'); \n";
    if($localstate["SW_LAMP_SCHRANK"]==1)
      {
	$updaterjs .= $this->myid."button_show_on_s();";
      } 
    else 
      {
	$updaterjs .= $this->myid."button_show_off_s();";
      }

    $updaterjs .= $this->myid."_obj_slider_f.setValue('".$localstate["FLIPPER_PWM"]."'); \n";
    if($localstate["SW_LAMP_FLIPPER"]==1)
      {
	$updaterjs .= $this->myid."button_show_on_f();";
      } 
    else 
      {
	$updaterjs .= $this->myid."button_show_off_f();";
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