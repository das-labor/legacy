<?
require_once "system_conf.php";
require_once "cl_base.php";

class c_irbeamer extends c_content{
  protected $dname;
  protected $actorfile="act_beamer.php";
  protected $content;
  protected $divstr;
  protected $cssstr;
  protected $setfile;
  protected $myid;
  protected $setfilecmd;
  protected $objname;
  protected $validirmodes;
  protected $validmainmodes;
  function __construct() {
    $this->divstr = "";
    $this->cssstr = "";
    $this->jsstr = "";
    $this->content = "";
    $this->setfile = "";
    $this->setfilecmd = "";
    $this->validirmodes=array("suspend" => "0x00", "scan" => "0x01", "vga" => "0x02", 
			      "dvi" => "0x03", "svideo" => "0x04", 
			      "component" => "0x05", "blank" => "0x06");
    $this->validmainmodes=array("MainPower" => "0xFF");
    $this->validmodesD=array("suspend" => "Suspend", "scan" => "Scan", "vga" => "VGA", 
			      "dvi" => "DVI", "svideo" => "sVideo", 
			     "component" => "Component", "blank" => "Blank","MainPower" => "MainPower");
    $this->initENV();
    # nothing
  }
  
  public function getObjName()
  {
    return $this->objname;
  }

  public function isvalidmode($mode)
  {
    $real_value = "0x01";
    foreach ($this->validirmodes as $key => $value)
      if( $mode==$key || $mode==$value) $real_value=$value;
    foreach ($this->validmainmodes as $key => $value)
      if($mode==$key || $mode==$value ) $real_value=$value;
    return $real_value;
  }

  protected function initENV()
  {
    $this->setobjname("beamer");
    $this->setdisplayname("Beamer");
    $this->setsetFile("act_beamer.php");
    $this->setsetfilecmd("beamer");
    $this->setid("id_beamer");
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
    $this->content = "<table class=\"".$this->myid."_table\" width=\"100%\"><tr class=\"".$this->myid."_tr\">";


    $this->content .= "<td class=\"".$this->myid."_td\">";
    $this->content .= "<input class=\"".$this->myid."_input_MainPower\" type=\"image\" ";
    $this->content .= "src=\"icons/mainpower.png\" ";
    $this->content .= "name=\"".$this->myid."_mp_img\"";
    $this->content .= "id=\"".$this->myid."_mp_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_mp(); ".$this->myid."_cmd('".$this->validmainmodes["MainPower"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_mp();\" >";
    $this->content .= "</td> ";


    $this->content .= "<td class=\"".$this->myid."_td\">";
    $this->content .= "<input class=\"".$this->myid."_input_suspend\" type=\"image\" ";
    $this->content .= "src=\"icons/suspend.png\" ";
    $this->content .= "name=\"".$this->myid."_suspend_img\"";
    $this->content .= "id=\"".$this->myid."_suspend_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_suspend(); ".$this->myid."_cmd('".$this->validirmodes["suspend"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_suspend();\" >";
    $this->content .= "</td> ";


    $this->content .= "<td class=\"".$this->myid."_td\">";
    $this->content .= "<input class=\"".$this->myid."_input_scan\" type=\"image\" ";
    $this->content .= "src=\"icons/scan.png\" ";
    $this->content .= "name=\"".$this->myid."_scan_img\"";
    $this->content .= "id=\"".$this->myid."_scan_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_scan(); ".$this->myid."_cmd('".$this->validirmodes["scan"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_scan();\" >";
    $this->content .= "</td> ";


    $this->content .= "<td class=\"".$this->myid."_td\">";
    $this->content .= "<input class=\"".$this->myid."_input_blank\" type=\"image\" ";
    $this->content .= "src=\"icons/blank.png\" ";
    $this->content .= "name=\"".$this->myid."_blank_img\"";
    $this->content .= "id=\"".$this->myid."_blank_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_blank(); ".$this->myid."_cmd('".$this->validirmodes["blank"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_blank();\" >";
    $this->content .= "</td></tr> ";


    $this->content .= "<tr class=\"".$this->myid."_tr\">";
    $this->content .= "<td class=\"".$this->myid."_td\">";
    $this->content .= "<input class=\"".$this->myid."_input_vga\" type=\"image\" ";
    $this->content .= "src=\"icons/vga.png\" ";
    $this->content .= "name=\"".$this->myid."_vga_img\"";
    $this->content .= "id=\"".$this->myid."_vga_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_vga(); ".$this->myid."_cmd('".$this->validirmodes["vga"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_vga();\" >";
    $this->content .= "</td> ";


    $this->content .= "<td class=\"".$this->myid."_td\">";
    $this->content .= "<input class=\"".$this->myid."_input_dvi\" type=\"image\" ";
    $this->content .= "src=\"icons/dvi.png\" ";
    $this->content .= "name=\"".$this->myid."_dvi_img\"";
    $this->content .= "id=\"".$this->myid."_dvi_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_dvi(); ".$this->myid."_cmd('".$this->validirmodes["dvi"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_dvi();\" >";
    $this->content .= "</td> ";

    $this->content .= "<td class=\"".$this->myid."_td\">";
    $this->content .= "<input class=\"".$this->myid."_input_component\" type=\"image\" ";
    $this->content .= "src=\"icons/component.png\" ";
    $this->content .= "name=\"".$this->myid."_comp_img\"";
    $this->content .= "id=\"".$this->myid."_comp_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_comp(); ".$this->myid."_cmd('".$this->validirmodes["component"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_comp();\" >";
    $this->content .= "</td> ";


    $this->content .= "<td class=\"".$this->myid."_td\">";
    $this->content .= "<input class=\"".$this->myid."_input_svideo\" type=\"image\" ";
    $this->content .= "src=\"icons/svideo.png\" ";
    $this->content .= "name=\"".$this->myid."_svideo_img\"";
    $this->content .= "id=\"".$this->myid."_svideo_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_svideo(); ".$this->myid."_cmd('".$this->validirmodes["svideo"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_svideo();\" >";
    $this->content .= "</td> ";


    $this->content .= "</tr></table>";
  }

  protected function initCSS()
  {
    #
  }

  protected function initJS()
  {
    $this->jsstr = "";
    $this->jsstr .= "function ".$this->myid."switch_dvi() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_dvi_img\").src='icons/dvi_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_dvi_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_dvi() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_dvi_img\").src='icons/dvi_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_dvi_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_dvi_img\").src='icons/dvi.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_mp() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_mp_img\").src='icons/mainpower_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_mp_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_mp() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_mp_img\").src='icons/mainpower_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_mp_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_mp_img\").src='icons/mainpower.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_suspend() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_suspend_img\").src='icons/suspend_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_suspend_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_suspend() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_suspend_img\").src='icons/suspend_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_suspend_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_suspend_img\").src='icons/suspend.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_scan() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_scan_img\").src='icons/scan_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_scan_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_scan() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_scan_img\").src='icons/scan_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_scan_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_scan_img\").src='icons/scan.png'\n";
    $this->jsstr .= "}\n";


    $this->jsstr .= "function ".$this->myid."switch_blank() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_blank_img\").src='icons/blank_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_blank_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_blank() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_blank_img\").src='icons/blank_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_blank_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_blank_img\").src='icons/blank.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_vga() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_vga_img\").src='icons/vga_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_vga_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_vga() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_vga_img\").src='icons/vga_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_vga_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_vga_img\").src='icons/vga.png'\n";
    $this->jsstr .= "}\n";


    $this->jsstr .= "function ".$this->myid."switch_comp() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_comp_img\").src='icons/component_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_comp_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_comp() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_comp_img\").src='icons/component_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_comp_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_comp_img\").src='icons/component.png'\n";
    $this->jsstr .= "}\n";


    $this->jsstr .= "function ".$this->myid."switch_svideo() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_svideo_img\").src='icons/svideo_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_svideo_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_svideo() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_svideo_img\").src='icons/svideo_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_svideo_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_svideo_img\").src='icons/svideo.png'\n";
    $this->jsstr .= "}\n";


    $this->jsstr .= "\nfunction ".$this->myid."_cmd(value)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '".$this->actorfile."?doit=1&value='+value,{method:'get', onComplete:function() {done=true;}} );\n";
    $this->jsstr .= "}\n";

  }

  public function getcss(){
    $this->cssstr = "";
    $this->cssstr .= ".".$this->myid."_table {}\n";
    $this->cssstr .= ".".$this->myid."_td {}\n";
    $this->cssstr .= ".".$this->myid."_input_MainPower {}\n";
    $this->cssstr .= ".".$this->myid."_input_suspend {}\n";
    $this->cssstr .= ".".$this->myid."_input_scan {}\n";
    $this->cssstr .= ".".$this->myid."_input_blank {}\n";
    $this->cssstr .= ".".$this->myid."_input_vga {}\n";
    $this->cssstr .= ".".$this->myid."_input_dvi {}\n";
    $this->cssstr .= ".".$this->myid."_input_component {}\n";
    $this->cssstr .= ".".$this->myid."_input_svideo {}\n";
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