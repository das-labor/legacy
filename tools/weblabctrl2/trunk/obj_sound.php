<?
require_once "cl_base.php";

class c_sound extends c_content{
  protected $dname;
  protected $actorfile="act_sound.php";
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

  public function isvalidvoice($mode)
  {
    if(in_array($mode,$this->validvoices)) return $mode;
    return "us1_mbrola";
  }

  protected function initENV()
  {
    $this->setobjname("sound");
    $this->setdisplayname("Sound");
    $this->setsetFile("act_sound.php");
    $this->setsetfilecmd("sound");
    $this->setid("id_sound");
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
    $this->content = "";
    $this->content .= "<form enctype=\"multipart/form-data\" action=\"act_sound.php?doit=1\" method=\"POST\">";
    $this->content .= "<table width=\"100%\"><tr>";

    $this->content .= "<tr><td><input name=\"userfile\" type=\"file\"><br></td></tr><tr>";
    $this->content .= "<td><input type=\"submit\" value=\"Send File\"></td></tr>";

    $this->content .= "</table>";
    $this->content .= "</form>";
  }

  protected function initCSS()
  {
    #
  }

  protected function initJS()
  {
    $this->jsstr = "";
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