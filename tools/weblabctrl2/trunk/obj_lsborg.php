<?
require_once "cl_base.php";

class c_lsborg extends c_content{
  protected $dname;
  protected $actorfile="act_laufschriftborg.php";
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
    $this->validmodes=array("coin","dice","w20","text");
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
    $this->setobjname("laufschriftborg");
    $this->setdisplayname("Laufschriftborg");
    $this->setsetFile("act_laufschriftborg.php");
    $this->setsetfilecmd("laufschriftborg");
    $this->setid("id_laufschriftborg");
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

    $this->content .= "<td colspan=\"3\">";
    $this->content .= "<input type=\"text\"  onchange=\"".$this->myid."_cmd(this.value,'text');this.value='';\" value=\"\">";
    $this->content .= "</td>";
    $this->content .= "</tr><tr>";
    $this->content .= "<td><input type=\"button\" id=\"coinflip\" onclick=\"".$this->myid."_cmd(this.value,'coin')\";\" value=\"coinflip\"></td>";
    $this->content .= "<td><input type=\"button\" id=\"dice\" onclick=\"".$this->myid."_cmd(this.value,'dice');\" value=\"dice\"></td>";
    $this->content .= "<td><input type=\"button\" id=\"w20\" onclick=\"".$this->myid."_cmd(this.value,'w20');\" value=\"w20\"></td>";
    $this->content .= "</tr></table>";
  }

  protected function initCSS()
  {
    #
  }

  protected function initJS()
  {
    $this->jsstr = "";

    $this->jsstr .= "\nfunction ".$this->myid."_cmd(value,funct)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '".$this->actorfile."?doit=1&function='+funct+'&value='+value,{method:'get', onComplete:function() {done=true;}} );\n";
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