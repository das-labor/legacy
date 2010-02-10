<?
require_once "cl_base.php";

class c_irteufel extends c_content{
  protected $dname;
  protected $actorfile="act_teufel.php";
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
    $this->validirmodes=array("leiser" => "0x00", "lauter" => "0x01", "mute" => "0x02", 
			      "main" => "0x03", "front" => "0x04", 
			      "rear" => "0x05", "side" => "0x06",
			      "sub" => "0x07", "center" => "0x08");
    $this->initENV();
    # nothing
  }
  public function getObjName()
  {
    return $this->objname;
  }
  
  public function isvalidmode($mode)
  {
    $real_value = "0x00";
    foreach ($this->validirmodes as $key => $value)
      if( $mode==$key || $mode==$value) $real_value=$value;
    return $real_value;
  }

  protected function initENV()
  {
    $this->setobjname("teufel");
    $this->setdisplayname("Teufel");
    $this->setsetFile("act_teufel.php");
    $this->setsetfilecmd("teufel");
    $this->setid("id_teufel");
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
    $this->content = "<table><tr>";

    $this->content .= "<td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/sound_up.png\" ";
    $this->content .= "name=\"".$this->myid."_su_img\"";
    $this->content .= "id=\"".$this->myid."_su_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_su(); ".$this->myid."_cmd('".$this->validirmodes["lauter"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_su();\" >";
    $this->content .= "</td> ";

    $this->content .= "<td>";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/sound_down.png\" ";
    $this->content .= "name=\"".$this->myid."_sd_img\"";
    $this->content .= "id=\"".$this->myid."_sd_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_sd(); ".$this->myid."_cmd('".$this->validirmodes["leiser"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_sd();\" >";
    $this->content .= "</td> ";

    $this->content .= "<td rowspan=\"2\" align=\"center\">";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/mute.png\" ";
    $this->content .= "name=\"".$this->myid."_mute_img\"";
    $this->content .= "id=\"".$this->myid."_mute_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_mute(); ".$this->myid."_cmd('".$this->validirmodes["mute"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_mute();\" >";
    $this->content .= "</td> ";

    $this->content .= "</tr></table>";

    $this->content .= "<table width=\"100%\">";
    $this->content .= "<tr>";
    $this->content .= "<td align=\"center\">";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/main.png\" ";
    $this->content .= "name=\"".$this->myid."_main_img\"";
    $this->content .= "id=\"".$this->myid."_main_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_main(); ".$this->myid."_cmd('".$this->validirmodes["main"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_main();\" >";
    $this->content .= "</td>";

    $this->content .= "<td align=\"center\">";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/front.png\" ";
    $this->content .= "name=\"".$this->myid."_front_img\"";
    $this->content .= "id=\"".$this->myid."_front_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_front(); ".$this->myid."_cmd('".$this->validirmodes["front"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_front();\" >";
    $this->content .= "</td>";


    $this->content .= "<td align=\"center\">";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/rear.png\" ";
    $this->content .= "name=\"".$this->myid."_rear_img\"";
    $this->content .= "id=\"".$this->myid."_rear_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_rear(); ".$this->myid."_cmd('".$this->validirmodes["rear"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_rear();\" >";
    $this->content .= "</td>";

    $this->content .= "</tr><tr>";
    $this->content .= "<td align=\"center\">";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/side.png\" ";
    $this->content .= "name=\"".$this->myid."_side_img\"";
    $this->content .= "id=\"".$this->myid."_side_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_side(); ".$this->myid."_cmd('".$this->validirmodes["side"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_side();\" >";
    $this->content .= "</td>";


    $this->content .= "<td align=\"center\">";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/sub.png\" ";
    $this->content .= "name=\"".$this->myid."_sub_img\"";
    $this->content .= "id=\"".$this->myid."_sub_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_sub(); ".$this->myid."_cmd('".$this->validirmodes["sub"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_sub();\" >";
    $this->content .= "</td>";


    $this->content .= "<td align=\"center\">";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/center.png\" ";
    $this->content .= "name=\"".$this->myid."_center_img\"";
    $this->content .= "id=\"".$this->myid."_center_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_center(); ".$this->myid."_cmd('".$this->validirmodes["center"]."');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_center();\" >";
    $this->content .= "</td>";
    $this->content .= "</tr>";
    $this->content .= "</table>";
  }

  protected function initCSS()
  {
    #
  }

  protected function initJS()
  {
    $this->jsstr = "";
    $this->jsstr .= "function ".$this->myid."switch_su() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_su_img\").src='icons/sound_up_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_su_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_su() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_su_img\").src='icons/sound_up_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_su_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_su_img\").src='icons/sound_up.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_sd() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_sd_img\").src='icons/sound_down_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_sd_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_sd() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_sd_img\").src='icons/sound_down_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_sd_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_sd_img\").src='icons/sound_down.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_mute() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_mute_img\").src='icons/mute_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_mute_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_mute() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_mute_img\").src='icons/mute_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_mute_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_mute_img\").src='icons/mute.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_main() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_main_img\").src='icons/main_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_main_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_main() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_main_img\").src='icons/mute_main.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_main_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_main_img\").src='icons/main.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_front() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_front_img\").src='icons/front_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_front_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_front() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_front_img\").src='icons/front_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_front_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_front_img\").src='icons/front.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_rear() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_rear_img\").src='icons/rear_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_rear_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_rear() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_rear_img\").src='icons/rear_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_rear_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_rear_img\").src='icons/rear.png'\n";
    $this->jsstr .= "}\n";


    $this->jsstr .= "function ".$this->myid."switch_side() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_side_img\").src='icons/side_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_side_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_side() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_side_img\").src='icons/side_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_side_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_side_img\").src='icons/side.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_sub() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_sub_img\").src='icons/sub_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_sub_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_sub() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_sub_img\").src='icons/sub_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_sub_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_sub_img\").src='icons/sub.png'\n";
    $this->jsstr .= "}\n";


    $this->jsstr .= "function ".$this->myid."switch_center() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_center_img\").src='icons/center_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_center_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_center() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_center_img\").src='icons/center_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_center_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_center_img\").src='icons/center.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."_cmd(value)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '".$this->actorfile."?doit=1&value='+value,{method:'get', onComplete:function() {done=true;}} );\n";
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