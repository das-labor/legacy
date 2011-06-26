<?
require_once "cl_base.php";

class c_jingels extends c_content{
  protected $dname;
  protected $actorfile="act_jingels.php";
  protected $content;
  protected $divstr;
  protected $cssstr;
  protected $setfile;
  protected $myid;
  protected $setfilecmd;
  protected $objname;
  protected $valimodes;
  protected $validstages;
  function __construct() {
    $this->divstr = "";
    $this->cssstr = "";
    $this->jsstr = "";
    $this->content = "";
    $this->setfile = "";
    $this->setfilecmd = "";
    $this->basedir="/srv/sounds";
    $this->validmodes=array();
    $this->validstages=array('bastel','lounge','vortrag','unten');
    $this->initENV();
    # nothing
  }

  public function getBasedir()
  {
    return $this->basedir;
  }

  public function getObjName()
  {
    return $this->objname;
  }

  public function getValidModes()
  {
    return $this->validmodes;
  }
  
  public function isvalidmode($mode)
  {
    if(in_array($mode,$this->validmodes)) return $mode;
    return "hacker";
  }

  public function isvalidstage($stage)
  {
    if(in_array($stage,$this->validstages)) return $stage;
    return "lounge";
  }

  protected function initENV()
  {
    $this->setobjname("jingels");
    $this->setdisplayname("Jingels");
    $this->setsetFile("act_jingels.php");
    $this->setsetfilecmd("jingels");
    $this->setid("id_jingels");
    if ($dh = opendir($this->basedir))
      if ($subdir !== "." && $subdir !== "..")
	while (($subdir = readdir($dh)) !== false)
	  if ($dhsubdir = opendir($this->basedir."/".$subdir))
	    while(($file = readdir($dhsubdir)) !== false)
	      if (filetype($this->basedir."/".$subdir."/". $file) && $file !== "." && $file !== "..")
		array_push($this->validmodes,$this->basedir."/".$subdir."/".$file);
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
    $this->content .= "<input type=\"image\" src=\"icons/stop_h.png\" onclick=\"".$this->myid."_cmd('stopall','','');\">";
    $this->content  = "<table width=\"100\"><tr>";
    if ($dh = opendir($this->basedir)) {
      while (($subdir = readdir($dh)) !== false) {
	if ($subdir !== "." && $subdir !== ".."){
	  $this->content .= "<td>".$subdir."<br>";
	  $this->content .= "<select name='".$subdir."' size=1>";
	  if ($dhsubdir = opendir($this->basedir."/".$subdir)){
	    while(($file = readdir($dhsubdir)) !== false) {
	      if (filetype($this->basedir."/".$subdir."/". $file) && $file !== "." && $file !== ".."){
		$this->content .= "<option value=\"".$file."\">".substr(substr($file,0,-4),0,15)."</option>";
	      }
	      
	    }
	    closedir($dhsubdir);
	  }
	  $this->content .= "</select></td><td>";
	  for($i=0;$i < sizeof($this->validstages); $i++){
	      $this->content .= "<input type=\"image\" src=\"icons/".$this->validstages[$i]."_room.png\" onclick=\"".$this->myid."_cmd('".$subdir."',document.getElementsByName('".$subdir."')[0].options[document.getElementsByName('".$subdir."')[0].options.selectedIndex].value,'".$this->validstages[$i]."');\">";
	  }
	  $this->content .= "</td></td></tr>";
	}
      }

    }
    closedir($dh);
    $this->content .= "</table>";
    $this->content .= "</div>";

  }

  protected function initCSS()
  {

  }

  protected function initJS()
  {
    $this->jsstr = "\nfunction ".$this->myid."_cmd(subdir,file,stage)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '".$this->actorfile."?doit=1&stage='+stage+'&file='+file+'&subdir='+subdir,{method:'get', onComplete:function() {done=true;}} );\n";
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