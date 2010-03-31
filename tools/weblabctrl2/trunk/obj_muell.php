<?
require_once "cl_base.php";
require_once 'ICSReader.php';



class c_muell extends c_content{
  protected $dname;
  protected $actorfile="act_muell.php";
  protected $updaterfile="updt_muell.php";
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
    $this->setobjname("muell");
    $this->setdisplayname("Muelldienst");
    $this->setsetFile("act_muell.php");
    $this->setsetfilecmd("muell");
    $this->setid("id_muell");
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
    $ical = new ICSReader();
    $ical->readfromfile('muell.ics');
    $events = $ical->getEvents();
    $ctime = time()-(60*60*24);
    $showevents = 2;
    $i=0;
    
    $this->content ="";
    $this->content .= "<div id=\"".$this->myid."_status\" name=\"".$this->myid."_status\"></div>";
    $this->content .= "<table width=\"100%\">";

    foreach($events as $event)
      {
 	if($event->get_start_date() > $ctime)
	  {
	    $i++;
	    $this->content .= "<tr><td  style=\"Color: #CC0000; font-weight:bold;\">";
	    $this->content .= date("D,d-M-Y",$event->get_start_date());
	    $this->content .= "</td>";
	    $this->content .= "<td>";
	    $this->content .= "<div id='".$this->myid.$event->get_start_date()."' name='".$this->myid.$event->get_start_date()."'>";
	    $this->content .= $event->get_summary();
	    $this->content .= "</div></td>";
	    $this->content .= "</tr>";
	  }
	if($i >= $showevents) break;
      }
    $this->content .= "</table>";
  }

  protected function initCSS()
  {
    #
  }

  protected function initJS()
  {
//     $this->jsstr = "";
    
//     $this->jsstr .= "function ".$this->myid."setcputemp1(value){";
//     $this->jsstr .= "document.getElementById (\"".$this->myid."cpu_temp1\").firstChild.data = value;";
//     $this->jsstr .= "}";

//     $this->jsstr .= "function ".$this->myid."setcputemp2(value){";
//     $this->jsstr .= "document.getElementById (\"".$this->myid."cpu_temp2\").firstChild.data = value;";
//     $this->jsstr .= "}";

//     $this->jsstr .= "function ".$this->myid."setcputemp3(value){";
//     $this->jsstr .= "document.getElementById (\"".$this->myid."cpu_temp3\").firstChild.data = value;";
//     $this->jsstr .= "}";

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

//     $value1 = 0;
//     $value2 = 0;
//     $value3 = 0;

//     if((!isset($localstate["status_last"])) ||
//        (((int)$localstate["status_last"] + 100) < time()))
//       {

// 	 $timeoutscript="./exec_timeout.sh 1 ";
// 	 $syscommand = $timeoutscript." powercommander.lapcontrol cantemp 0x25 0x01";
// 	 exec($syscommand,$status1);

// 	 $syscommand = $timeoutscript." powercommander.lapcontrol cantemp 0x04 0x00";
// 	 exec($syscommand,$status2);

// 	 $syscommand = $timeoutscript." powercommander.lapcontrol cantemp 0x23 0x01";
// 	 exec($syscommand,$status3);

// 	 $indata = sscanf  ( $status1[1] , "Temp is %f" );
// 	 if ( !isset($indata[0]) || $indate[0] > 127 || $indata[0] < -128 ) $value1 = 'Fail';
// 	 else $value1= $indata[0];

// 	 $indata = sscanf  ( $status2[1] , "Temp is %f" );
// 	 if ( !isset($indata[0]) || $indate[0] > 127 || $indata[0] < -128) $value2 = 'Fail';
// 	 else $value2= $indata[0];

	 
// 	 $indata = sscanf  ( $status3[1] , "Temp is %f" );
// 	 if ( !isset($indata[0]) || $indate[0] > 127 || $indata[0] < -128 ) $value3 = 'Fail';
// 	 else $value3= $indata[0];
	
// 	 $localstate["status_temp1"] = $value1;
// 	 $localstate["status_temp2"] = $value2;
// 	 $localstate["status_temp3"] = $value3;
// 	 $localstate["status_last"]= time();
//       }

//      if( !isset($localstate["status_temp1"]) || ($localstate["status_temp1"] < -128) || ($localstate["status_temp1"] > 127)) $value1='Fail';
//      else  $value1 = $localstate["status_temp1"];

//      if((!isset($localstate["status_temp2"]) || $localstate["status_temp2"] < -128) || ($localstate["status_temp2"] > 127)) $value2='Fail';
//      else  $value2 = $localstate["status_temp2"];

//      if((!isset($localstate["status_temp3"]) || $localstate["status_temp3"] < -128) || ($localstate["status_temp3"] > 127)) $value3='Fail';
//      else  $value3 = $localstate["status_temp3"];

    
//      $updatejs .= $this->myid."setcputemp1(\"".$value1."\");\n";
//      $updatejs .= $this->myid."setcputemp2(\"".$value2."\");\n";
//      $updatejs .= $this->myid."setcputemp3(\"".$value3."\");\n";

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
