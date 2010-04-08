<?
require_once "cl_base.php";

class c_treppenblink extends c_content{
  protected $dname;
  protected $actorfile="act_treppenblink.php";
  protected $content;
  protected $divstr;
  protected $cssstr;
  protected $setfile;
  protected $myid;
  protected $setfilecmd;
  protected $objname;
  protected $treppenblink_modes=array("blau" => "0x00", "stop" => "0x01", "weiss" => "0x02", "grau" => "0x03" , "aus" => "0x04", "gruen" => "0x05", "rot" => "0x06", "hurtz" => "0x07", "borg" => "0x08");
  protected $treppenblink_order=array("blau","rot","gruen","weiss","grau","stop","aus","hurtz","borg");
  function __construct() {
    $this->divstr = "";
    $this->cssstr = "";
    $this->jsstr = "";
    $this->content = "";
    $this->setfile = "";
    $this->setfilecmd = "";
    $this->initENV();
    # nothing
  }
  
  public function getObjName()
  {
    return $this->objname;
  }

  public function isvalidmode($mode)
  {
    $real_tbm_value = "0x00";
    foreach ($this->treppenblink_modes as $tbm_key => $tbm_value)
      if($mode==$tbm_value || 
	 $mdoe==$tbm_key ) $real_tbm_value=$tbm_value;
    return $real_tbm_value;
  }

  protected function initENV()
  {
    $this->setobjname("treppenblink");
    $this->setdisplayname("Treppenblink");
    $this->setsetFile("obj_treppenblink.php");
    $this->setsetfilecmd("treppenblink");
    $this->setid("id_treppenblink");
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
    $this->content .= "<td><input type=\"image\" src=\"icons/blau.png\" onclick=\"".$this->myid."_cmd('".$this->treppenblink_modes["blau"]."');\"></td> ";
    $this->content .= "<td><input type=\"image\" src=\"icons/rot.png\" onclick=\"".$this->myid."_cmd('".$this->treppenblink_modes["rot"]."');\"></td> ";
    $this->content .= "<td><input type=\"image\" src=\"icons/gruen.png\" onclick=\"".$this->myid."_cmd('".$this->treppenblink_modes["gruen"]."');\"></td> ";
    $this->content .= "<td><input type=\"image\" src=\"icons/weiss.png\" onclick=\"".$this->myid."_cmd('".$this->treppenblink_modes["weiss"]."');\"></td></tr><tr> ";
    $this->content .= "<td><input type=\"image\" src=\"icons/grau.png\" onclick=\"".$this->myid."_cmd('".$this->treppenblink_modes["grau"]."');\"></td> ";
    $this->content .= "<td><input type=\"image\" src=\"icons/pause.png\" onclick=\"".$this->myid."_cmd('".$this->treppenblink_modes["stop"]."');\"></td> ";
    $this->content .= "<td><input type=\"image\" src=\"icons/aus.png\" onclick=\"".$this->myid."_cmd('".$this->treppenblink_modes["aus"]."');\"></td> ";
    $this->content .= "<td><input type=\"image\" src=\"icons/hurts.png\" onclick=\"".$this->myid."_cmd('".$this->treppenblink_modes["hurtz"]."');\"></td></tr><tr> ";
    $this->content .= "<td><input type=\"image\" src=\"icons/borg.png\" onclick=\"".$this->myid."_cmd('".$this->treppenblink_modes["borg"]."');\"></td> ";
    $this->content .= "</tr></table>";
  }

  protected function initCSS()
  {
    #
  }

  protected function initJS()
  {
    $this->jsstr = "\nfunction ".$this->myid."_cmd(value)\n";
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