<?
include "config.php";
?>
<html>
<head>
<script src="scriptaculous/prototype.js" type="text/javascript"></script>
<script src="scriptaculous/scriptaculous.js" type="text/javascript"></script>
<script src="weblabctrl.js" type="text/javascript"></script>
<?
if($_COOKIE['bg']) $bg=$_COOKIE['bg'];
 else $bg=1;
if($_COOKIE['pwmx']) $pwmx=$_COOKIE['pwmx'];
 else $pwmx=50;
if($_COOKIE['pwmy']) $pwmy=$_COOKIE['pwmy'];
 else $pwmy=60;
if($_COOKIE['mpdx']) $mpdx=$_COOKIE['mpdx'];
 else $mpdx=350;
if($_COOKIE['mpdy']) $mpdy=$_COOKIE['mpdy'];
 else $mpdy=60;
if($_COOKIE['miscx']) $miscx=$_COOKIE['miscx'];
 else $miscx=650;
if($_COOKIE['miscy']) $miscy=$_COOKIE['miscy'];
 else $miscy=60;
echo "<style type=\"text/css\">
  * {font-family:Arial;}
  div.slider { width:256px; margin:10px 0; background-color:#ccc; height:10px; position: relative; }
  div.slider div.handle { width:10px; height:15px; background-color:#f00; cursor:move; position: absolute; }
  div.pwm {position:absolute; left:".$pwmx."; top: ".$pwmy."; background-color:#ddddff; width: 256px; padding:5px;}
  div.pwm_top {background-color:#ccccff; width: 256px; margin-bottom:5px;}
  div.mpd {position:absolute; left:".$mpdx."; top: ".$mpdy."; background-color:#ddddff; width: 256px; padding:5px;}
  div.mpd_top {background-color:#ccccff; width: 256px; margin-bottom:5px;}
  div.misc {position:absolute; left:".$miscx."; top: ".$miscy."; background-color:#ddddff; width: 300px; padding:5px;}
  div.misc_top {background-color:#ccccff; width: 300px; margin-bottom:5px;}
  body {margin: 0px; padding: 0px; background-image:url(bg.jpg);background-repeat:no-repeat;background-color:#000000;}
  input {}
</style>";
?>

</head>

<script>

function set_value(cmd,id,value)
{
  new Ajax.Updater('ajax', 'set.php?cmd='+cmd+'&id='+id+'&value='+value,{method:'get', onComplete:function() {done=true;}, evalScripts: true} );
}
function treppenblink_cmd(value)
{
  new Ajax.Updater('ajax', 'set.php?cmd=treppenblink&value='+value,{method:'get', onComplete:function() {done=true;}} );
}
function canir_beamer_cmd(value)
{
  new Ajax.Updater('ajax', 'set.php?cmd=canir_beamer&value='+value,{method:'get', onComplete:function() {done=true;}} );
}
function canir_teufel_cmd(value)
{
  new Ajax.Updater('ajax', 'set.php?cmd=canir_teufel&value='+value,{method:'get', onComplete:function() {done=true;}} );
}
function mpd_cmd(room,cmd,value)
{
  new Ajax.Updater('ajax', 'mpd.php?cmd='+cmd+'&room='+room+'&value='+value,{method:'get', onComplete:function() {done=true;}} );
}
function beamer_on()
{
  new Ajax.Updater('ajax', 'set.php?cmd=beamer_on',{method:'get', onComplete:function() {done=true;}} );
}
function text_the_borg(text)
{
  new Ajax.Updater('ajax', 'set.php?cmd=text_the_borg&text='+text,{method:'get', onComplete:function() {done=true;}} );
}
function save_pos(element)
{
  new Ajax.Updater('ajax', 'set.php?cmd=save_pos&x='+document.getElementById(element.id).style.left+'&y='+document.getElementById(element.id).style.top,{method:'get', onComplete:function() {done=true;}} );
}
function save_bg()
{
  new Ajax.Updater('ajax', 'set.php?cmd=save_bg&bg='document.body.background,{method:'get', onComplete:function() {done=true;}} );
}
function set_bg(localimage)
{
  document.body.background='bg/'+localimage;
  save_bg();
}
function blue_mode(mode)
{
  new Ajax.Updater('ajax', 'set.php?cmd=blue_mode&mode='+mode,{method:'get', onComplete:function() {done=true;}} );
}
function moodbar(r,g,b)
{
  new Ajax.Updater('ajax', 'set.php?cmd=moodbar&r='+r+'&g='+g+'&b='+b,{method:'get', onComplete:function() {done=true;}} );
}
function moodbar_fade()
{
  new Ajax.Updater('ajax', 'set.php?cmd=moodbar_fade',{method:'get', onComplete:function() {done=true;}} );
}
</script>
<div id="ajax">
#debug
</div>
<input type="button" value="svn update" onclick="new Ajax.Updater('ajax', 'set.php?update=doit',{method:'get'});">
<div id="status"> </div>
<div id="pwm" class="pwm">
<div class="pwm_top" on_click="Effect.toggle('pwm_entries','slide');">Light</div>
<div id="pwm_entries">
<?
foreach($pwm_ids as $id)
{
	$color=sprintf('#%02X%02X%02X',rand(0,255),rand(0,255),rand(0,255));
	echo "$id <input id=\"LAMP_$id\" type=\"checkbox\" onclick=\"if(this.checked)set_value('SW','LAMP_$id','ON');else set_value('SW','LAMP_$id','OFF')\">
	<div id=\"slider_$id\" class=\"slider\"> 
            <div class=\"handle\" style=\"background-color: $color;\"></div>
        </div>\n";
}
?>
Moodbar
	<div id="r_slider" class="slider">
	    <div class="handle" style="background-color: #f00;"></div>
	</div>
	<div id="g_slider" class="slider">
	    <div class="handle" style="background-color: #0f0;"></div>
	</div>
	<div id="b_slider" class="slider">
	    <div class="handle" style="background-color: #00f;"></div>
	</div>

<input type="button" value="fade on" onclick="moodbar_fade();">

</div>
</div>
<div id="mpd" class="mpd">
<div class="mpd_top" on_click="Effect.toggle('mpd_entries','slide');">Music</div>
<div id="mpd_entries">
<?
foreach($rooms as $room => $port)
{
	#unset($status);
	#exec("export MPD_PORT=$port; mpc",$status);
	$color=sprintf('#%02X%02X%02X',rand(0,255),rand(0,255),rand(0,255));
	#$color="#".dechex(rand(0,255)).dechex(rand(0,255)).dechex(rand(0,255));
	echo "$room";
	echo "<table width=100%><tr>";
	echo "<td><input type=\"button\" id=\"".$room."_pause\" class=\"".$room."_button\" onclick=\"mpd_cmd('$room','pause');\" value=\"pause\"></td>";
	echo "<td><input type=\"button\" id=\"".$room."_play\" class=\"".$room."_button\" onclick=\"mpd_cmd('$room','play');\" value=\"play\"></td>";
	echo "<td><input type=\"button\" id=\"".$room."_prev\" class=\"".$room."_button\" onclick=\"mpd_cmd('$room','prev');\" value=\"prev\"></td>";
	echo "<td><input type=\"button\" id=\"".$room."_next\" class=\"".$room."_button\" onclick=\"mpd_cmd('$room','next');\" value=\"next\"></td><br>";
	echo "</tr></table>";
	echo "<div id=\"".$room."_status\"></div>";
	#if(strpos($status[0],"volume")===false) echo substr(htmlspecialchars($status[0]),0,24);
	echo "<div id=\"slider_mpd_volume_$room\" class=\"slider\"> 
            <div class=\"handle\" style=\"background-color: $color;\"></div>
        </div>\n";
	$color=sprintf('#%02X%02X%02X',rand(0,255),rand(0,255),rand(0,255));
	#$color="#".dechex(rand(0,255)).dechex(rand(0,255)).dechex(rand(0,255));
	echo "<div id=\"slider_mpd_process_$room\" class=\"slider\"> 
            <div class=\"handle\" style=\"background-color: $color;\"></div>
        </div>\n";
}
?>
</div>
</div>

<div id="misc" class="misc">
<div class="misc_top">Misc</div>
Beamer<br>
<?
  $beamer_counter=1;
  echo "<table width=100% border=0 cellpadding=0 cellspacing=0><tr>";
  echo "<td align=center><input type=\"button\" id=\"beamer_button\" onclick=\"beamer_on();\" value=\"MainPower on\"></td>";  
  foreach ($canir_beamer_a as $cirb_key => $cirb_value){
    if($beamer_counter==4){
      echo "</tr><tr>";
      $beamer_counter=1;
    } else { 
      $beamer_counter++;
    }
    echo "<td align=center><input type=\"button\" id=\"cirb_".$cirb_key."\" onclick=\"canir_beamer_cmd('".$cirb_value."');\" value=\"".$cirb_key."\"></td>";
  }
echo "</tr></table>";
$beamer_counter=0;
echo "<form name=\"beamerSourceForm\">";
echo "source select: <select onchange=\"CheckBeamerSource();\" name=\"beamerSource\" size=\"1\">";


  foreach ($canir_beamer_a_channel as $cirb_key => $cirb_value){
    echo "<option value=\"".$cirb_value."\">".$cirb_key."</option>";
  }
 echo "</select>";
echo "</form>";
?>

<br>Teufel Amp<br>
<?
  $teufel_counter=0;
  echo "<table width=100% border=0 cellpadding=0 cellspacing=0><tr>";
  foreach ($canir_teufel_a as $cirt_key => $cirt_value){
    if($teufel_counter==4){
      echo "</tr><tr>";
      $teufel_counter=1;
    } else { 
      $teufel_counter++;
    }
    echo "<td align=center><input type=\"button\" id=\"cirt_".$cirb_key."\" onclick=\"canir_teufel_cmd('".$cirt_value."');\" value=\"".$cirt_key."\"></td>";
  }
 echo "</tr></table>";
echo "<form name=\"teufelSourceForm\">";
echo "source select: <select onchange=\"CheckTeufelSource()\" name=\"teufelSource\" size=\"1\">" ;



  foreach ($canir_teufel_a_channel as $cirb_key => $cirb_value){
    echo "<option value=\"".$cirb_value."\">".$cirb_key."</option>";
  }
 echo "</select>";
echo "</form>";
?>

<br>Laufschriftborg<br>
<input type="text"  onchange="text_the_borg(this.value);this.value='';" value="">
<br>Treppenblink<br>
<?
# button oben, text drunter
    echo "<table width=100%>";
  echo "<tr>";
  foreach ($treppenblink_modes as $tbm_key => $tbm_value)
    echo "<td align=\"center\"><input type=\"radio\" onclick=\"treppenblink_cmd('".$tbm_value."');\" name=\"treppenblink\" value=\"".$tbm_value."\"></td>";
  echo "</tr><tr>";
  foreach ($treppenblink_modes as $tbm_key => $tbm_value)
   echo "<td>".$tbm_key."</td>";
 echo "</tr></table>";
?>
<br>Vortrag Mode<br>
<input type="image" src="blue_mode_off.png" onclick="blue_mode('off');">
<input type="image" src="blue_mode_on.png" onclick="blue_mode('on');">
<input type="image" src="blue_mode_video_conference.png" onclick="blue_mode('video_conference');">
<input type="image" src="blue_mode_speaker.png" onclick="blue_mode('speaker');">
<input type="image" src="blue_mode_dimm.png" onclick="blue_mode('dimm');">
<br>Background Image<br>
<input type="image" src="bg/1_small.jpg" onclick="document.body.background='bg/'+localimage;">
<input type="image" src="bg/2_small.jpg" onclick="set_bg('2.jpg');">
<input type="image" src="bg/3_small.jpg" onclick="set_bg('3.jpg');">
<input type="image" src="bg/4_small.jpg" onclick="set_bg('4.jpg');">
<input type="image" src="bg/5_small.jpg" onclick="set_bg('5.jpg');">
<input type="image" src="bg/6_small.jpg" onclick="set_bg('6.jpg');">
<input type="image" src="bg/7_small.jpg" onclick="set_bg('7.jpg');">
<input type="image" src="bg/8_small.jpg" onclick="set_bg('8.jpg');">
<input type="image" src="bg/9_small.jpg" onclick="set_bg('9.jpg');">
<input type="image" src="bg/10_small.jpg" onclick="set_bg('10.jpg');">

</div>
</form>
<script>

function CheckTeufelSource () {
  for (i = 0; i < document.teufelSourceForm.teufelSource.length; ++i)
    if (document.teufelSourceForm.teufelSource.options[i].selected == true)
      canir_teufel_cmd(document.teufelSourceForm.teufelSource.options[i].value);
}
function CheckBeamerSource () {
  for (i = 0; i < document.beamerSourceForm.beamerSource.length; ++i)
    if (document.beamerSourceForm.beamerSource.options[i].selected == true)
      canir_beamer_cmd(document.beamerSourceForm.beamerSource.options[i].value);
}

  (function() {
<?
foreach($pwm_ids as $id)
{

echo "
      var slider_$id = $('slider_$id');

      slider_".$id."_control = new Control.Slider(slider_$id.select('.handle'), slider_$id, {
      range: \$R(0, 255),
      increment: 16,
      sliderValue: [100],
      onSlide: function(values) {
                        value=Math.round(values);
			if(done)set_value('PWM','$id',value);
			done=false;
      }
    });";
}
?>
	var r=100,g=100,b=100, timestamp;
  (function() {
      var r_slider = $('r_slider'),
      	g_slider = $('g_slider'),
      	b_slider = $('b_slider'),
      	info = $('info');

      new Control.Slider(r_slider.select('.handle'), r_slider, {
      range: $R(0, 255),
		increment: 16,
      sliderValue: [100],
      onSlide: function(values) {
			r=Math.round(values);
			moodbar(r,g,b);
      }
    });

      new Control.Slider(g_slider.select('.handle'), g_slider, {
      range: $R(0, 255),
		increment: 16,
      sliderValue: [100],
      onSlide: function(values) {
			g=Math.round(values);
			moodbar(r,g,b);
      }
    });

      new Control.Slider(b_slider.select('.handle'), b_slider, {
      range: $R(0, 255),
		increment: 16,
      sliderValue: [100],
      onSlide: function(values) {
			b=Math.round(values);
			moodbar(r,g,b);
      }
    });
  })();

<?

foreach($rooms as $room => $port)
{
	unset($volume);
	exec("export MPD_PORT=$port; mpc volume",$volume);
	preg_match('/volume: (..)%/',$volume[0],$volume);
	echo "
      var slider_mpd_volume_$room = $('slider_mpd_volume_$room');


      new Control.Slider(slider_mpd_volume_$room.select('.handle'), slider_mpd_volume_$room, {
      range: \$R(0, 100),
      increment: 10,
      sliderValue: [".$volume[1]."],
      onSlide: function(values) {
                        value=Math.round(values);
			if(done)mpd_cmd('$room','volume',value);
			done=false;
      }
    });";
	echo "var slider_mpd_process_$room = $('slider_mpd_process_$room');
	slider_process_".$room."_control = new Control.Slider(slider_mpd_process_$room.select('.handle'), slider_mpd_process_$room, {
      range: \$R(0, 100),disabled:true});";
}
echo "  })();\n";
echo "function update_status(){\n";
echo "new Ajax.Updater('status', 'status.php',{method:'get', evalScripts: true} );";
echo "\nwindow.setTimeout('update_status()',1000);\n";
echo "}\nupdate_status();";
?>    
new Draggable('pwm',{onEnd:function(element){new Ajax.Updater('ajax', 'set.php?cmd=save_pos&div=pwm&x='+document.getElementById('pwm').style.left+'&y='+document.getElementById('pwm').style.top,{method:'get', onComplete:function() {done=true;}} );}});
new Draggable('mpd',{onEnd:function(element){new Ajax.Updater('ajax', 'set.php?cmd=save_pos&div=mpd&x='+document.getElementById('mpd').style.left+'&y='+document.getElementById('mpd').style.top,{method:'get', onComplete:function() {done=true;}} );}});
new Draggable('misc',{onEnd:function(element){new Ajax.Updater('ajax', 'set.php?cmd=save_pos&div=misc&x='+document.getElementById('misc').style.left+'&y='+document.getElementById('misc').style.top,{method:'get', onComplete:function() {done=true;}} );}});

var done=true;
</script>

</html>

