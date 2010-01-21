<?
include "config.php";
?>
<html>
<head>
<script src="scriptaculous/prototype.js" type="text/javascript"></script>
<script src="scriptaculous/scriptaculous.js" type="text/javascript"></script>
<script src="weblabctrl.js" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="weblabcss.php">
</head>
<body class="" id="body" name="body">
<script>
<?
  echo "set_bg('".$bg."');";
?>
function rollelement(myelement,maxsize)
{
  if(document.getElementById(myelement).style.height=='0px') 
    {
      document.getElementById(myelement).style.height=maxsize;
    }
  else 
    {
      document.getElementById(myelement).style.height='0px';
    }

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
        echo "<input id=\"LAMP_$id\" 
                         name=\"LAMP_$id\" 
                         src=\"dotOFF.png\" 
                         type=\"image\" 
                         onclick='if(this.src.substr(-10,10)==\"dotOFF.png\")
                                   { 
                                     set_value(\"SW\",\"LAMP_$id\",\"ON\"); 
                                     this.src=\"dotON.png\"; 
                                   } 
                                   else 
                                   { 
                                     set_value(\"SW\",\"LAMP_$id\",\"OFF\"); 
                                     this.src=\"dotOFF.png\";
                                   }' style=\"background-color: transparent;\">&nbsp;";
	echo $GLN["$id"] ;
	echo "
	<div id=\"slider_$id\" style=\"height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" class=\"slider\"> 
            <div class=\"handle\" style=\"width:15px; background-image:url(1up.png); background-repeat:no-repeat; background-color: transparent; \"></div>
        </div>\n";
        if($id == 'VORTRAG_PWM'){
echo "
<table width=\"100%\"><tr><td align=center>
<input style=\"margin: 0px; padding: 0px; background-color: transparent;\" type=\"image\" src=\"blue_mode_off.png\" onclick=\"blue_mode('off');\">
<input style=\"margin: 0px; padding: 0px; background-color: transparent;\" type=\"image\" src=\"blue_mode_on.png\" onclick=\"blue_mode('on');\">
<input style=\"margin: 0px; padding: 0px; background-color: transparent;\" type=\"image\" src=\"blue_mode_video_conference.png\" onclick=\"blue_mode('video_conference');\">
<input style=\"margin: 0px; padding: 0px; background-color: transparent;\" type=\"image\" src=\"blue_mode_speaker.png\" onclick=\"blue_mode('speaker');\">
<input style=\"margin: 0px; padding: 0px; background-color: transparent;\" type=\"image\" src=\"blue_mode_dimm.png\" onclick=\"blue_mode('dimm');\"></td></tr></table>";
	}

}
?>
Moodbar
	<div id="r_slider" style="height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;" class="slider">
	    <div class="handle" style="background-color: #f00;"></div>
	</div>
	<div id="g_slider" style="height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;" class="slider">
	    <div class="handle" style="background-color: #0f0;"></div>
	</div>
	<div id="b_slider" style="height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;" class="slider">
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
	echo "<td><input src=\"pause_h.png\" type=\"image\" name=\"".$room."_pause\" id=\"".$room."_pause\" class=\"".$room."_button\" style=\"background-color: transparent;\" onclick=\"mpd_cmd('$room','pause'); \" value=\"pause\"></td>";
	echo "<td><input src=\"play_h.png\" type=\"image\" name=\"".$room."_play\" id=\"".$room."_play\" class=\"".$room."_button\" style=\"background-color: transparent;\" onclick=\"mpd_cmd('$room','play'); \" value=\"play\"></td>";
	echo "<td><input src=\"rev_h.png\" type=\"image\" name=\"".$room."_prev\" id=\"".$room."_prev\" class=\"".$room."_button\" style=\"background-color: transparent;\" onclick=\"mpd_cmd('$room','prev'); \" value=\"prev\"></td>";
	echo "<td><input src=\"ff_h.png\" type=\"image\" name=\"".$room."_next\" id=\"".$room."_next\" class=\"".$room."_button\" style=\"background-color: transparent;\" onclick=\"mpd_cmd('$room','next'); \" value=\"next\"></td>";
	echo "</tr></table>";
	echo "<div id=\"".$room."_status\"></div>";
	echo "<div style=\"height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\" id=\"slider_mpd_volume_$room\" class=\"slider\"> 
            <div class=\"handle\" style=\"width:15px; background-image:url(1up.png); background-repeat:no-repeat; background-color: transparent; \"></div>
        </div>\n";
	$color=sprintf('#%02X%02X%02X',rand(0,255),rand(0,255),rand(0,255));
	echo "<div style=\"height:15px; background-image:url(sliderbg.png); background-repeat:repeat-x; background-color: transparent;\"  id=\"slider_mpd_process_$room\" class=\"slider\"> 
            <div class=\"handle\" style=\"width:15px; background-image:url(1up.png); background-repeat:no-repeat; background-color: transparent; \"></div>
        </div>\n";
}
?>
</div>
</div>

<div id="treppenblink" class="treppenblink">
    <div class="treppenblink_top" ondblclick="rollelement('treppenblink_cont','80px')">Treppenblink</div>
    <div id="treppenblink_cont" style="overflow: hidden;" name="treppenblink_cont">
<?
# button oben, text drunter
  $foo=0;
    echo "<table width=100%>";
  echo "<tr>";
  foreach ($treppenblink_modes as $tbm_key => $tbm_value){
    $foo++;
    echo "<td align=\"center\"><input type=\"radio\" onclick=\"treppenblink_cmd('".$tbm_value."');\" name=\"treppenblink\" value=\"".$tbm_value."\"><br>".$tbm_key."</td>";
    if($foo == 4){
	$foo=0;
	echo "</tr><tr>";
    }

  }
  if($foo != 0)  echo "</tr><tr>";
  foreach ($treppenblink_modes as $tbm_key => $tbm_value)
#   echo "<td>".$tbm_key."</td>";
 echo "</tr></table>";
# twitter
# curl --basic --user username:password --data status="Having fun with cURL" http://twitter.com/statuses/update.xml
?>
</div>
</div>


<div id="mybackground" class="mybackground">
<div class="mybackground_top" ondblclick="rollelement('mybackground_cont','150px')">Background</div>
    <div id="mybackground_cont" style="overflow: hidden;" name="mybackground_cont">
<input type="image" src="bg/1_small.jpg" onclick="set_bg('eins')">
<input type="image" src="bg/2_small.jpg" onclick="set_bg('zwei');">
<input type="image" src="bg/3_small.jpg" onclick="set_bg('drei');">
<input type="image" src="bg/4_small.jpg" onclick="set_bg('vier');">
<input type="image" src="bg/5_small.jpg" onclick="set_bg('fuenf');">
<input type="image" src="bg/6_small.jpg" onclick="set_bg('sechs');">
<input type="image" src="bg/7_small.jpg" onclick="set_bg('sieben');">
<input type="image" src="bg/8_small.jpg" onclick="set_bg('acht');">
<input type="image" src="bg/9_small.jpg" onclick="set_bg('neun');">
<input type="image" src="bg/10_small.jpg" onclick="set_bg('zehn');">
<input type="image" src="bg/11_small.jpg" onclick="set_bg('dwarf');">
</div>
</div>

<div id="vortragsdevs" class="vortragsdevs">
<div class="vortragsdevs_top" ondblclick="rollelement('vortragsdevs_cont','170px')">Hardware Vortragsraum</div>
    <div id="vortragsdevs_cont" style="overflow: hidden;" name="vortragsdevs_cont">
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
Teufel Amp<br>
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
</div>
</div>

<div id="misc" class="misc">
<div class="misc_top">Misc</div>
Laufschriftborg<br>
<input type="text"  onchange="text_the_borg(this.value);this.value='';" value="">
<br>ToiletLamp<br>
<table>
  <tr>
    <td><input id="TOILET_0" type="checkbox" onclick="if(this.checked)toilet_cmd('TOILET_0','ON');else toilet_cmd('TOILET_0','OFF')">
    <td colspan=3>enable random</td>
  </tr>
  <tr>
    <td>
      <input id="TOILET_1" type="checkbox" onclick="if(this.checked)toilet_cmd('TOILET_1','ON');else toilet_cmd('TOILET_1','OFF')">
    </td>
    <td>
      <input id="TOILET_8" type="checkbox" onclick="if(this.checked)toilet_cmd('TOILET_8','ON');else toilet_cmd('TOILET_8','OFF')">
    </td>
    <td>
      <input id="TOILET_4" type="checkbox" onclick="if(this.checked)toilet_cmd('TOILET_4','ON');else toilet_cmd('TOILET_4','OFF')">
    </td>
    <td>
      <input id="TOILET_2" type="checkbox" onclick="if(this.checked)toilet_cmd('TOILET_2','ON');else toilet_cmd('TOILET_2','OFF')">
    </td>
  </tr>
</table>
<br>Random<br>
<table>
   <tr>
     <td><input type="button" id="coinflip" onclick="coinflip();" value="coinflip"></td>
     <td><input type="button" id="dice" onclick="dice();" value="dice"></td>
     <td><input type="button" id="w20" onclick="w20();" value="w20"></td>
   </tr>
</table>

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
new Draggable('treppenblink',{onEnd:function(element){new Ajax.Updater('ajax', 'set.php?cmd=save_pos&div=treppenblink&x='+document.getElementById('treppenblink').style.left+'&y='+document.getElementById('treppenblink').style.top,{method:'get', onComplete:function() {done=true;}} );}});
new Draggable('mybackground',{onEnd:function(element){new Ajax.Updater('ajax', 'set.php?cmd=save_pos&div=mybackground&x='+document.getElementById('mybackground').style.left+'&y='+document.getElementById('mybackground').style.top,{method:'get', onComplete:function() {done=true;}} );}});
new Draggable('vortragsdevs',{onEnd:function(element){new Ajax.Updater('ajax', 'set.php?cmd=save_pos&div=vortragsdevs&x='+document.getElementById('vortragsdevs').style.left+'&y='+document.getElementById('vortragsdevs').style.top,{method:'get', onComplete:function() {done=true;}} );}});

var done=true;
</script>
</body>
</html>

