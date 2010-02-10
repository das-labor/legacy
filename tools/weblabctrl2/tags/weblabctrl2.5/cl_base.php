<?

abstract class c_content {

# returns string with css-definition
  abstract public function getcss();

#returns string with javascript definitions
  abstract public function getjs();

#returns string with content
  abstract public function getcontent();

#returns string with id
  abstract public function getid();

#get displayname
  abstract public function getdname();

#get objectname - all objects of one class does have the same name
  abstract public function getObjName();

#set setfile and updates code
  abstract public function setsetFile($newsetfile);
#set objname and updates code
  abstract public function setobjname($newobjname);
#set setfilecmd and updates code
  abstract public function setsetfilecmd($newsetfilecmd);
#set an id
  abstract public function setid($newid);
#set a displayname
  abstract public function setdisplayname($newdisplayname);

  abstract public function getSysConf();

  abstract public function setSysConf($sysconf);

  abstract public function getClientConf();

  abstract public function setClientConf($clientconf);

  public function drawcontent(){
    echo $this->getcontent();
  }
  public function drawcss(){
    echo $this->getcss();
  }
  public function drawjs(){
    echo $this->getjs();
  }

}


class c_identifier 
{
  protected $contentobj;
  protected $content;
  protected $jsstr;
  protected $cssstr;
  
  function __construct($contentobj)
  {
    $this->contentobj=$contentobj;
    $this->initDiv();
    $this->initCSS();
    $this->initJS();
  }

  function initDiv()
  {
    $this->content = $this->contentobj->getdname();
    $this->content .= "<div id=\"".$this->contentobj->getid()."_cont\" name=\"".$this->contentobj->getid()."_cont\">\n";
    $this->content .= $this->contentobj->getcontent();
    $this->content .= "</div>\n";
  }

  function initJS()
  {
    $this->jsstr = "";
  }

  function initCSS()
  {
    $this->cssstr = "div.".$this->contentobj->getid."_cont ";
    $this->cssstr .= "{background-color:#ccccff; width: 256px; margin-bottom:5px;}\n";
    $this->cssstr .= $this->contentobj->getcss();
  }

  function getcontent()
  {
    return $this->content;
  }

  function getjs()
  {
    return $this->contentobj->getjs();
  }

  function getcss()
  {
    return $this->cssstr;
  }

}




class c_pannel {
  protected $contentobjs=array();
  protected $jsstr;
  protected $cssstr;
  protected $content;
  protected $name;
  protected $dname;
  protected $bgimage;
  protected $xpos;
  protected $ypos;
  protected $closexpos;
  protected $closeypos;
  protected $width;
  protected $height;
  protected $closed;
  
  function __construct($dname,$name,$closexpos,$closeypos,$xpos,$ypos,$closed)
  {
    $this->dname=$dname;
    $this->name=$name;
    $this->bgimage="divbg256.png";
    $this->xpos = $xpos;
    $this->ypos = $ypos;
    $this->width = "276px";
    $this->padding = "0px";
    $this->closed = $closed;
    $this->closexpos = $closexpos;
    $this->closeypos = $closeypos;
    $this->initCSS();
    $this->initJS();
    $this->initDIV();
  }

  function setDname($newdname)
  {
    $this->dname=$newdname;
    $this->initCSS();
    $this->initJS();
    $this->initDIV();
  }

  function setName($newName)
  {
    $this->name=$newName;
    $this->initCSS();
    $this->initJS();
    $this->initDIV();
  }

  function getDname()
  {
    return $this->dname;
  }

  function getName()
  {
    return $this->name;
  }
  
  function initCSS()
  {
    $this->cssstr  = "  div.".$this->name." { \n";
    $this->cssstr .= "    position:absolute; \n";
    $this->cssstr .= "    left:".$this->xpos.";\n";
    $this->cssstr .= "    top: ".$this->ypos.";\n";
    $this->cssstr .= "    background-image:url(".$this->bgimage."); \n";
    $this->cssstr .= "    background-repeat:repeat-y; \n";
    $this->cssstr .= "    background-color: transparent;\n";
    $this->cssstr .= "    width: ".$this->width."; \n";
    $this->cssstr .= "    padding: ".$this->padding.";\n";
    $this->cssstr .= "  }";
    $this->cssstr .= "  div.".$this->name."_all { \n";
    $this->cssstr .= "    background-color: transparent;\n";
    $this->cssstr .= "    overflow: hidden;\n";
    $this->cssstr .= "  }";
    $this->cssstr .= "  div.".$this->name."_top { \n";
    $this->cssstr .= "    background-color: #aaaadd;\n";
    $this->cssstr .= "    overflow: hidden;\n";
    $this->cssstr .= "  }";
    foreach($this->contentobjs as $obj)
      {
	$this->cssstr .= $obj->getcss();
      }

  }
  
  function initJS()
  {

    $this->jsstr = "";
    $this->jsstr .= "var ".$this->name."_xpos = '".$this->xpos."';\n";
    $this->jsstr .= "var ".$this->name."_ypos = '".$this->ypos."';\n";
    $this->jsstr .= "var ".$this->name."_xposclosed = '".$this->closexpos."';\n";
    $this->jsstr .= "var ".$this->name."_yposclosed = '".$this->closeypos."';\n";
    $this->jsstr .= "var ".$this->name."_closed = '".$this->closed."';\n";

    // restore from cookie
    $this->jsstr .= "if(".$this->name."_closed == 'closed') {\n";
    $this->jsstr .= "document.getElementById('".$this->name."').style.left=".$this->name."_xposclosed; \n";
    $this->jsstr .= "document.getElementById('".$this->name."').style.top=".$this->name."_yposclosed; \n";
    $this->jsstr .= "document.getElementById('".$this->name."_all').style.height='0px';\n}";
    $this->jsstr .= "else {\n";
    $this->jsstr .= "document.getElementById('".$this->name."').style.left=".$this->name."_xpos; \n";
    $this->jsstr .= "document.getElementById('".$this->name."').style.top=".$this->name."_ypos; \n";
    $this->jsstr .= "document.getElementById('".$this->name."_all').style.height='';\n}";

    $this->jsstr .= "new Draggable('".$this->name."',{";

    $this->jsstr .= "onEnd:function(element){";
    // set js-vars according to state open or closed on move
    $this->jsstr .= "if(".$this->name."_closed == 'closed') {\n";
    $this->jsstr .= $this->name."_xposclosed = document.getElementById('".$this->name."').style.left; \n";
    $this->jsstr .= $this->name."_yposclosed = document.getElementById('".$this->name."').style.top; \n";
    $this->jsstr .= "} else {\n";
    $this->jsstr .= $this->name."_xpos = document.getElementById('".$this->name."').style.left; \n";
    $this->jsstr .= $this->name."_ypos = document.getElementById('".$this->name."').style.top; \n";
    $this->jsstr .= "\n}";

    // save in cookie
    $this->jsstr .= "new Ajax.Updater('ajax',";
    $this->jsstr .= "'save_user.php?";
    $this->jsstr .= "name=".$this->name."&";
    $this->jsstr .= "xpos='+".$this->name."_xpos+'&";
    $this->jsstr .= "ypos='+".$this->name."_ypos+'&";
    $this->jsstr .= "xpos_small='+".$this->name."_xposclosed+'&";
    $this->jsstr .= "ypos_small='+".$this->name."_yposclosed+'&";
    $this->jsstr .= "height='+document.getElementById('".$this->name."_all').style.height,";
    $this->jsstr .= "{method:'get', onComplete:function() {done=true;}} );}});";

    // functions for jumping
    $this->jsstr .= "function ".$this->name."_mov1(){ ";
    $this->jsstr .= "  document.getElementById('".$this->name."').style.left=".$this->name."_xposclosed;}\n";
    $this->jsstr .= "function ".$this->name."_mov2(){ ";
    $this->jsstr .= "  document.getElementById('".$this->name."').style.top=".$this->name."_yposclosed;}\n";
    $this->jsstr .= "function ".$this->name."_mov3(){ ";
    $this->jsstr .= "  document.getElementById('".$this->name."').style.left=".$this->name."_xpos;}\n";
    $this->jsstr .= "function ".$this->name."_mov4(){ ";
    $this->jsstr .= "  document.getElementById('".$this->name."').style.top=".$this->name."_ypos; ";
    $this->jsstr .= "  document.getElementById('".$this->name."_all').style.height='';";
    $this->jsstr .=    $this->name."_moveupdater();}\n";



    $this->jsstr .= "function ".$this->name."_roll()\n{\n";
    $this->jsstr .= "if(document.getElementById('".$this->name."_all').style.height=='0px') \n";
    $this->jsstr .= "{\n";
    $this->jsstr .=   "window.setTimeout(\"".$this->name."_mov3()\", 200);";
    $this->jsstr .=   "window.setTimeout(\"".$this->name."_mov4()\", 400);";
    $this->jsstr .=   $this->name."_closed = 'open';\n";
    $this->jsstr .= "";
    $this->jsstr .= "} else  { \n";
    $this->jsstr .=   "document.getElementById('".$this->name."_all').style.height='0px';\n";
    $this->jsstr .=   "window.setTimeout(\"".$this->name."_mov1()\", 200);";
    $this->jsstr .=   "window.setTimeout(\"".$this->name."_mov2()\", 400);";
    $this->jsstr .=   "window.setTimeout(\"".$this->name."_moveupdater()\", 600);";
    $this->jsstr .=   $this->name."_closed = 'closed';\n";
    $this->jsstr .= "}";
    $this->jsstr .= "}";



    $this->jsstr .= "\nfunction ".$this->name."_moveupdater()\n{";
    $this->jsstr .= "new Ajax.Updater('ajax',";
    $this->jsstr .= "'save_user.php?";
    $this->jsstr .= "name=".$this->name."&";
    $this->jsstr .= "xpos='+".$this->name."_xpos+'&";
    $this->jsstr .= "ypos='+".$this->name."_ypos+'&";
    $this->jsstr .= "xpos_small='+".$this->name."_xposclosed+'&";
    $this->jsstr .= "ypos_small='+".$this->name."_yposclosed+'&";
    $this->jsstr .= "height='+document.getElementById('".$this->name."_all').style.height,";
    $this->jsstr .= "{method:'get', onComplete:function() {done=true;}} )\n};";

    foreach($this->contentobjs as $obj)
      {
	$this->jsstr .= $obj->getjs();
      }
    

  }

  function initDIV()
  {
    $this->content = "<div id=\"".$this->name."\" class=\"".$this->name."\">\n";
    $this->content .= "<div ondblclick=\"".$this->name."_roll();\" id=\"".$this->name."_top\" class=\"".$this->name."_top\">\n";
 $this->content .= "<table border=0 cellpadding=0 cellspacing=0 width=\"100%\"><tr><td width=\"100%\">".$this->dname."</td><td align=\"right\">";

 $this->content .= "<input type=\"image\" ";
 $this->content .= "src=\"icons/buttonx.png\" ";
 $this->content .= "onclick=\"".$this->name."_roll();\"> ";
 $this->content .= "</td></tr></table> ";


 $this->content .= "</div>\n";
 $this->content .= "<div style=\"height:0px\" id=\"".$this->name."_all\" class=\"".$this->name."_all\">\n";
 foreach($this->contentobjs as $obj)
   {
 	$this->content .= $obj->getcontent();
   }
    $this->content .= "</div>\n";
    $this->content .= "</div>\n";
  }

  function addcontentOBJ($contentobj)
  {
    array_push($this->contentobjs,$contentobj);
    $this->initJS();
    $this->initDIV();
    $this->initCSS();
  }

  function getctx()
  {
    return $this->content;
  }
  function getjs()
  {
    return $this->jsstr;
  }
  function getcss()
  {
    return $this->cssstr; 
  }
  
}
?>