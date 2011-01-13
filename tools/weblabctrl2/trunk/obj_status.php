<?
require_once "cl_base.php";

class c_status extends c_content{
  protected $dname;
  protected $actorfile="act_status.php";
  protected $updaterfile="updt_status.php";
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
    $this->initENV();
    # nothing
  }
  
  public function getObjName()
  {
    return $this->objname;
  }

  public function isvalidmode($mode)
  {
    return "ok";
  }

  protected function initENV()
  {
    $this->setobjname("status");
    $this->setdisplayname("Temperaturen");
    $this->setsetFile("act_status.php");
    $this->setsetfilecmd("status");
    $this->setid("id_status");
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
    $this->content ="";
    $this->content .= "<div id=\"".$this->myid."_status\" name=\"".$this->myid."_status\"></div>";
    $this->content .= "<table width=\"100%\"><tr>";


    $this->content .= "<td>";
    $this->content .= "Treppenblink";
    $this->content .= "</td>";
    $this->content .= "<td>";
    $this->content .= "<div id='".$this->myid."cpu_temp1' name='".$this->myid."cpu_temp1'>";
    $this->content .= "NaN";
    $this->content .= "</div></td>";

    $this->content .= "</tr><tr>";

    $this->content .= "<td>";
    $this->content .= "Draussen";
    $this->content .= "</td>";
    $this->content .= "<td>";
    $this->content .= "<div id='".$this->myid."cpu_temp2' name='".$this->myid."cpu_temp2'>";
    $this->content .= "NaN";
    $this->content .= "</div></td>";
    $this->content .= "</tr><tr>";
    $this->content .= "<td>";
    $this->content .= "Kueche";
    $this->content .= "</td>";
    $this->content .= "<td>";
    $this->content .= "<div id='".$this->myid."cpu_temp3' name='".$this->myid."cpu_temp3'>";
    $this->content .= "NaN";
    $this->content .= "</div></td>";

    $this->content .= "</tr></table>";
  }

  protected function initCSS()
  {
    #
  }

  protected function initJS()
  {
    $this->jsstr = "";
    
    $this->jsstr .= "function ".$this->myid."setcputemp1(value){";
    $this->jsstr .= "document.getElementById (\"".$this->myid."cpu_temp1\").firstChild.data = value;";
    $this->jsstr .= "}";

    $this->jsstr .= "function ".$this->myid."setcputemp2(value){";
    $this->jsstr .= "document.getElementById (\"".$this->myid."cpu_temp2\").firstChild.data = value;";
    $this->jsstr .= "}";

    $this->jsstr .= "function ".$this->myid."setcputemp3(value){";
    $this->jsstr .= "document.getElementById (\"".$this->myid."cpu_temp3\").firstChild.data = value;";
    $this->jsstr .= "}";

    // $this->jsstr .= "function ".$this->myid."_updater(){\n";
    // $this->jsstr .= "new Ajax.Updater(\"".$this->myid."_status\",'".$this->updaterfile."?";
    // $this->jsstr .= "myid=".$this->myid."&";
    // $this->jsstr .= "update=1',";
    // $this->jsstr .= "{method:'get', evalScripts: true});\n";
    // $this->jsstr .= "}\n";
    // // das ist nur ne temperatur - alle 100 sekunden reicht eigentlich
    // $this->jsstr .= "var ".$this->myid."obj_updater = window.setInterval(\"".$this->myid."_updater()\", 100000);";
    // $this->jsstr .= $this->myid."_updater();";

  }

  
  public function getupdaterjs()
  {
    global $localstate;
    $updatejs = "";

    $value1 = 0;
    $value2 = 0;
    $value3 = 0;

    if((!isset($localstate["status_last"])) ||
       (((int)$localstate["status_last"] + 100) < time()))
      {

	 $timeoutscript="/usr/src/weblabctrl/exec_timeout.sh 1 ";
	 $syscommand = $timeoutscript." powercommander.lapcontrol cantemp 0x25 0x01";
	 exec($syscommand,$status1);

	 $syscommand = $timeoutscript." powercommander.lapcontrol cantemp 0x04 0x00";
	 exec($syscommand,$status2);

	 $syscommand = $timeoutscript." powercommander.lapcontrol cantemp 0x23 0x01";
	 exec($syscommand,$status3);

	 $value1 = sscanf  ( $status1[1] , "Temp is %f" );
	 $value1=$value1[0];
//	 $indata = sscanf  ( $status1[1] , "Temp is %f" );
// 	 if ( !isset($indata[0]) || $indate[0] > float(127.0) || $indata[0] < float(-128.0) ) $value1 = 'Fail';
// 	 else $value1= $indata[0];

	 $value2 = sscanf  ( $status2[1] , "Temp is %f" );
	 $value2=$value2[0];
//	 $indata = sscanf  ( $status2[1] , "Temp is %f" );
// 	 if ( !isset($indata[0]) || $indate[0] > float(127.0) || $indata[0] < float(-128.0)) $value2 = 'Fail';
// 	 else $value2= $indata[0];

	 $value3 = sscanf  ( $status3[1] , "Temp is %f" );
	 $value3=$value3[0];
//	 $indata = sscanf  ( $status3[1] , "Temp is %f" );
// 	 if ( !isset($indata[0]) || $indate[0] > float(127.0) || $indata[0] < float(-128.0) ) $value3 = 'Fail';
// 	 else $value3= $indata[0];
	
	 $localstate["status_temp1"] = $value1;
	 $localstate["status_temp2"] = $value2;
	 $localstate["status_temp3"] = $value3;
	 //	 $localstate["status_last"]= time();
	 $localstate["status_last"]= 0;
      }

//      if( !isset($localstate["status_temp1"]) || ($localstate["status_temp1"] < -128) || ($localstate["status_temp1"] > 127)) $value1='Fail';
//      else  $value1 = $localstate["status_temp1"];

//      if((!isset($localstate["status_temp2"]) || $localstate["status_temp2"] < -128) || ($localstate["status_temp2"] > 127)) $value2='Fail';
//      else  $value2 = $localstate["status_temp2"];

//      if((!isset($localstate["status_temp3"]) || $localstate["status_temp3"] < -128) || ($localstate["status_temp3"] > 127)) $value3='Fail';
//      else  $value3 = $localstate["status_temp3"];

    
     $updatejs .= $this->myid."setcputemp1(\"".$value1."\");\n";
     $updatejs .= $this->myid."setcputemp2(\"".$value2."\");\n";
     $updatejs .= $this->myid."setcputemp3(\"".$value3."\");\n";

     //     var_dump($localstate);

     return $updatejs;
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
