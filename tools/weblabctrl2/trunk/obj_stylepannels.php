<?
require_once "cl_base.php";

class c_stylepannels extends c_content{
  protected $dname;
  protected $actorfile="act_stylepannels.php";
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
    $this->validmodes=array("top","left");
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
    $this->setobjname("stylepannels");
    $this->setdisplayname("Stylepannels");
    $this->setsetFile("act_stylepannels.php");
    $this->setsetfilecmd("stylepannels");
    $this->setid("id_stylepannels");
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
    $this->content = "<table width=\"100%\"><tr>";


    $this->content .= "<td align=\"center\">";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/left.png\" ";
    $this->content .= "name=\"".$this->myid."_left_img\"";
    $this->content .= "id=\"".$this->myid."_left_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_left(); ".$this->myid."_cmd('left');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_left();\" >";
    $this->content .= "</td> ";

    $this->content .= "<td align=\"center\">";
    $this->content .= "<input type=\"image\" ";
    $this->content .= "src=\"icons/top.png\" ";
    $this->content .= "name=\"".$this->myid."_top_img\"";
    $this->content .= "id=\"".$this->myid."_top_img\"";
    $this->content .= "onclick=\"".$this->myid."switch_top(); ".$this->myid."_cmd('top');\" ";
    $this->content .= "onmousedown=\"".$this->myid."down_top();\" >";
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
    $this->jsstr .= "function ".$this->myid."switch_left() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_left_img\").src='icons/left_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_left_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_left() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_left_img\").src='icons/left_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_left_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_left_img\").src='icons/left.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "function ".$this->myid."switch_top() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_top_img\").src='icons/top_down.png';\n";
    $this->jsstr .= "window.setTimeout('".$this->myid."switch_top_off()',50);\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."down_top() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_top_img\").src='icons/top_down.png';\n";
    $this->jsstr .= "}\n";
    $this->jsstr .= "function ".$this->myid."switch_top_off() {\n";
    $this->jsstr .= "document.getElementById(\"".$this->myid."_top_img\").src='icons/top.png'\n";
    $this->jsstr .= "}\n";

    $this->jsstr .= "\nfunction ".$this->myid."_cmd(value)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '".$this->actorfile."?doit=1&value='+value,{method:'get', evalScripts: true, onComplete:function() {done=true;}} );\n";
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