<?
require_once "cl_base.php";

class c_speek extends c_content{
  protected $dname;
  protected $actorfile="act_speek.php";
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
    $this->validmodes=array("text");
    $this->validvoices=array("cmu_us_bdl_arctic_clunits","cmu_us_clb_arctic_clunits",
			     "cmu_us_jmk_arctic_clunits","cmu_us_slt_arctic_clunits",
			     "cmu_us_rms_arctic_clunits","us1_mbrola","don_diphone",
			     "kal_diphone","ked_diphone","rab_diphone","us2_mbrola",
			     "us3_mbrola","el_diphone","nitech_us_awb_arctic_hts",
			     "nitech_us_bdl_arctic_hts","nitech_us_clb_arctic_hts",
			     "nitech_us_jmk_arctic_hts","nitech_us_rms_arctic_hts",
			     "nitech_us_slt_arctic_hts");
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
    $this->setobjname("texttospeach");
    $this->setdisplayname("Text To Speach");
    $this->setsetFile("act_speek.php");
    $this->setsetfilecmd("texttospeach");
    $this->setid("id_texttospeach");
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
    $this->content .= "<table width=\"100%\"><tr>";

    $this->content .= "<td colspan=\"4\">";
    $this->content .= "<input type=\"text\"  onchange=\"".$this->myid."_cmd(this.value,'text',".$this->myid."_voiceselected);this.value='';\" value=\"\">";
    $this->content .= "</td>";
    $this->content .= "</tr>";

    $this->content .= "<tr colspan=4>";
    $this->content .= "<td>";
    foreach($this->validvoices as $voice){
      $this->content .= "<input type=\"radio\" name=\"".$this->myid."_voice_radio\" onclick=\"".$this->myid."_voiceselected='".$voice."'\">";
    }

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
    
    $this->jsstr .= $this->myid."_voiceselected='".$this->validvoices[0]."'";
    $this->jsstr .= "\nfunction ".$this->myid."_cmd(value,funct,myvoice)\n";
    $this->jsstr .= "{\n";
    $this->jsstr .= "    new Ajax.Updater('ajax', '".$this->actorfile."?doit=1&voice='+myvoice+'&function='+funct+'&value='+value,{method:'get', onComplete:function() {done=true;}} );\n";
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