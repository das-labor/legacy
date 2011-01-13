<?
require_once "cl_base.php";

class c_lightbasteldetail extends c_content{
  protected $dname;
  protected $actorfile="act_lightbasteldetail.php";
  protected $updatefile="updt_lightbasteldetail.php";
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
    $this->validobjs=array("orgatisch","banner","fenster","drucker1","drucker2","helmer1","helmer2");
    $this->dimmbar=array("orgatisch","banner","fenster");

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

  public function isdimmbar($obj)
  {
    if(in_array($obj,$this->dimmbar)) return $obj;
    return "hacker";
  }

  protected function initENV()
  {
    $this->setobjname("lightbasteldetail");
    $this->setdisplayname("Licht Bastelraum Detail");
    $this->setsetFile("act_lightbasteldetail.php");
    $this->setsetfilecmd("lightbasteldetail");
    $this->setid("id_lightbasteldetail");
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
    $this->content .= "name=\"".$this->myid."_oo_img\"";
    $this->content .= "id=\"".$this->myid."_oo_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_oo(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    
    $this->content .= "	 <div id=\"".$this->myid."_slider_o\" style=\"width:235px; height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\">";
    $this->content .= "   <div class=\"handle\" style=\"width:15px; background-image:url(icons/1up.png); background-repeat:no-repeat; background-color: transparent; \"></div><div style=\"position:relative; top:-15px; left:170px;\">Orgatisch</div>";
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
    $this->content .= "   <div class=\"handle\" style=\"width:15px; background-image:url(icons/1up.png); background-repeat:no-repeat; background-color: transparent; \"></div><div style=\"position:relative; top:-15px; left:170px;\">Banner</div>";
    $this->content .= "  </div>";
    
    $this->content .= "  </tr>";
    $this->content .= "</table>";

# schrank

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
    $this->content .= "   <div class=\"handle\" style=\"width:15px; background-image:url(icons/1up.png); background-repeat:no-repeat; background-color: transparent; \"></div><div style=\"position:relative; top:-15px; left:170px;\">Fenster</div>";
    $this->content .= "  </div>";
    
    $this->content .= "  </tr>";
    $this->content .= "</table>";

# flipper

    $this->content .= "<table>";
    $this->content .= "  <tr>";
    $this->content .= "    <td>Drucker";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/dotOFF.png\" ";
    $this->content .= "name=\"".$this->myid."_d1o_img\"";
    $this->content .= "id=\"".$this->myid."_d1o_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_d1o(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/dotOFF.png\" ";
    $this->content .= "name=\"".$this->myid."_d2o_img\"";
    $this->content .= "id=\"".$this->myid."_d2o_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_d2o(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
    $this->content .= "  </tr>";

    $this->content .= "  <tr>";
    $this->content .= "    <td>Helmer";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/dotOFF.png\" ";
    $this->content .= "name=\"".$this->myid."_h1o_img\"";
    $this->content .= "id=\"".$this->myid."_h1o_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_h1o(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
    $this->content .= "    <td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/dotOFF.png\" ";
    $this->content .= "name=\"".$this->myid."_h2o_img\"";
    $this->content .= "id=\"".$this->myid."_h2o_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_h2o(); \" ";
    $this->content .= "style=\"background-color: transparent;\">";
    $this->content .= "    </td>";
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

# orgatisch
    $this->jsstr .= "\nfunction ".$this->myid."switch_oo()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_oo_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on_o();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off_o();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on_o()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('orgatisch','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_oo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_o()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_oo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_o()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('orgatisch','off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_oo_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off_o()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_oo_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";



    $this->jsstr .= "var ".$this->myid."_tvalue=100;\n";

    $this->jsstr .= "    var ".$this->myid."_slider_o = \$('".$this->myid."_slider_o');\n";

    $this->jsstr .= "\n";
    $this->jsstr .= "    var ".$this->myid."_obj_slider_o = new Control.Slider(".$this->myid."_slider_o.select('.handle'), ".$this->myid."_slider_o, \n";
    $this->jsstr .= "		       {\n";
    $this->jsstr .= "			   range: \$R(0, 255),\n";
    $this->jsstr .= "			   increment: 1,\n";
    $this->jsstr .= "			   sliderValue: [100],\n";
    $this->jsstr .= "			   onSlide: function(values) \n";
    $this->jsstr .= "			       {\n";
    $this->jsstr .= "				   ".$this->myid."_ovalue=Math.round(values);\n";
    $this->jsstr .= "				   if(done)".$this->myid."_cmd('orgatisch','dimm',".$this->myid."_ovalue);\n";
    $this->jsstr .= "                              done=false;\n";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";


#banner


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
    $this->jsstr .=      $this->myid."_cmd('banner','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_bo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_b()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_bo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_b()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('banner','off',0);\n";
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
    $this->jsstr .= "				   if(done)".$this->myid."_cmd('banner','dimm',".$this->myid."_bvalue);\n";
    $this->jsstr .= "                              done=false;";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";


# fenster


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
    $this->jsstr .=      $this->myid."_cmd('fenster','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_fo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_f()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_fo_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_f()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('fenster','off',0);\n";
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
    $this->jsstr .= "				   if(done)".$this->myid."_cmd('fenster','dimm',".$this->myid."_fvalue);\n";
    $this->jsstr .= "                              done=false;";
    $this->jsstr .= "			       }\n";
    $this->jsstr .= "		       });\n";
    $this->jsstr .= "\n";

# drucker1


    $this->jsstr .= "\nfunction ".$this->myid."switch_d1o()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_d1o_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on_d1();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off_d1();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on_d1()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('drucker1','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_d1o_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_d1()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_d1o_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_d1()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('drucker1','off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_d1o_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off_d1()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_d1o_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";


# drucker2


    $this->jsstr .= "\nfunction ".$this->myid."switch_d2o()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_d2o_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on_d2();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off_d2();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on_d2()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('drucker2','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_d2o_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_d2()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_d2o_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_d2()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('drucker2','off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_d2o_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off_d2()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_d2o_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";





# helmer1


    $this->jsstr .= "\nfunction ".$this->myid."switch_h1o()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_h1o_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on_h1();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off_h1();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on_h1()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('helmer1','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_h1o_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_h1()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_h1o_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_h1()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('helmer1','off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_h1o_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off_h1()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_h1o_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";


# helmer2


    $this->jsstr .= "\nfunction ".$this->myid."switch_h2o()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "  if(document.getElementById(\"".$this->myid."_h2o_img\").src.substr(-10,10)==\"dotOFF.png\")";
    $this->jsstr .= "  {\n";
    $this->jsstr .= $this->myid."button_on_h2();\n";
    $this->jsstr .= "  } \n";
    $this->jsstr .= "else \n";
    $this->jsstr .= "  { \n";
    $this->jsstr .= $this->myid."button_off_h2();\n";
    $this->jsstr .= "  }\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_on_h2()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('helmer2','on',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_h2o_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_on_h2()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_h2o_img\").src=\"icons/dotON.png\"; \n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_off_h2()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .=      $this->myid."_cmd('helmer2','off',0);\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_h2o_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."button_show_off_h2()\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    document.getElementById(\"".$this->myid."_h2o_img\").src=\"icons/dotOFF.png\";\n";
    $this->jsstr .= "}\n";



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
    $updaterjs .= $this->myid."_obj_slider_o.setValue('".$localstate["FENSTER_PWM"]."'); \n";
    if( $localstate["SW_LAMP_FENSTER"]==1 ) 
      {
	$updaterjs .= $this->myid."button_show_on_o();";
      } 
    else 
      {
	    $updaterjs .= $this->myid."button_show_off_o();";
      }

    $updaterjs .= $this->myid."_obj_slider_b.setValue('".$localstate["BANNER_PWM"]."'); \n";
    if($localstate["SW_LAMP_BEAMER"]==1)
      {
	$updaterjs .= $this->myid."button_show_on_b();";
      } 
    else 
      {
	$updaterjs .= $this->myid."button_show_off_b();";
      }

    $updaterjs .= $this->myid."_obj_slider_f.setValue('".$localstate["ORGATISCH_PWM"]."'); \n";
    if($localstate["SW_LAMP_ORGATISCH"]==1)
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