<?php
require_once ('conf/lights.php');
require_once ('statusstring.php');
require_once ('conf/lapd.php');

$status = statusstring_decode($_GET['status']);

?>
<html>
<head>
<title>WebLabCtrl</title>
<style type="text/css">
body
{
	font-size:12px;
	font-family:Verdana,Arial,Helvetica;
}
div fieldset
{
	background-color:#FFFFFF;
	display:table;
	border-style:solid;
	border-color:#000000;
	text-align:center;
	border-width:2px;
	-moz-border-radius:10px;
}
div#lightbox
{
	width:140px;	
}
legend
{
	font-size:14px;
	font-weight:bold;
	-moz-border-radius:10px;
	border-style:solid;
	border-width:2px;
	background-color:#FFFFFF;
	border-color:inherit;
	padding-left:10px;
	padding-right:10px;
}
div fieldset a
{
	border-width:1px;
	border-style:solid;
	width:110px;
	height:18px;
	display:table;
	text-decoration:none;
	font-weight:bold;
	font-size:12px;
	color:#000000;
	-moz-border-radius:5px;
}
div
{
	border-style:none;
	border-width:1px;
	padding:10px;
	display:table;
	float:right;
}
iframe#chuckframe
{
	width:320px;
	height:200px;
}
div#submitframe
{
	height:0px;
	width:0px;
}
/*
iframe#submitbox
{
	width:800px;
	height:100px;
}
*/
div#debugwindow
{
	bottom:0px;
	display:table;
	height:100px;
	background-color:#FF0000;
<?php
if (!$_GET['debug'])
	echo 'display:none; width:0px; height:0px;';
	echo '} iframe#submitbox { height:0px; width:0px; '
?>
}
iframe
{
	border-style:none;
	padding:0px;
	margin:0px;
}

iframe#shoutboxframe
{
	height:300px;
	width:280px;
}

div#shoutboxdiv input
{
	border-style:solid;
	border-width:1px;
	border-color:#999999;
	background-color:#F1F1F1;
	margin-top:4px;
	font-size:12px;
}
div#shoutboxdiv form
{
	text-align:right;
}
div#musicbox a
{
	width:45px;
	margin:1px;
	padding:10px;
	float:left;
	-moz-border-radius:0px;
	border-style:outset;
}
div#musicbox a:hover
{
	border-style:solid;
}
div#musicbox a:active
{
	border-style:inset;
}
div#bottombox 
{
	height:20px;
	font-size:8px;
	color:#666666;
	bottom:0px;
	left:0px;
	right:0px;
	border-style:none;
	position:absolute;
}
input.inpinvisible
{
	border-style:none;

}
iframe#lsframe {
	width:300px;
	height:120px;
}
a.spotbtnr, a.spotbtnl, a.spotbtnm {
	display:inline;
	float:left;
	width:10px;
	height:20px;
	-moz-border-radius:0;
	border-style:outset;
}
a.spotbtnr {
	border-left:none;
}
a.spotbtnl {
	border-right-style:dotted;
	border-right-width:1px;
	border-right-color:#CCCCCC;
}
a.spotbtnm {
	border-left:none;
	border-right-style:dotted;
	border-right-color:#CCC;
	border-right-width:1px;
}
</style>
</head>
<body>
<iframe name="submitframe" id="submitbox">blau</iframe>
<?php   /************************** LIGHTS *****/  ?>
<div id="lightbox">
<fieldset>
<legend>LIGHTS</legend>
&nbsp; <br />
<?php
reset ($status);
/*
while (list($key, $val) = each($status))
{
	if (stripos($key, 'light ') == (int) 0)
	{
		echo '<input type="submit" value="' . substr($key, 5) . '" class="';
		echo ($val != '0') ? 'lgreen' : 'lred';
		echo '" /><br />';
	}

}
*/

for ($i=0;$i<count($conf_light);$i++)
{
	echo '<a href="submitrequest.php?action=togglelight&amp;light=' .
		strtoid($conf_light[$i]['command']) . '" id="' .
		strtoid($conf_light[$i]['command'])  
		. '" target="submitframe">' . $conf_light[$i]['name'] .
		'</a><br />';
}
?>
</fieldset>
<fieldset>
<legend>SPOTS</legend>
<?php
for ($i=0;$i<4;$i++)
{
	echo '<a href="submitrequest.php?action=dimspot&amp;spotid=' . $i .
		'&amp;level=0" class="spotbtnl" target="submitframe">+</a>';
	for ($k=1;$k<8;$k++) echo '<a href="submitrequest.php?action=dimspot&amp;spotid=' . $i .
		'&amp;level=' . ($k*8) . '" class="spotbtnm" target="submitframe"> </a>';
	echo '<a href="submitrequest.php?action=dimspot&amp;spotid=' . $i .
		'&amp;level=64" class="spotbtnr" target="submitframe">-</a><strong>' . ($i + 1) .
		'</strong><br />&nbsp;<br />';
}
?>
</fieldset>
</div>

<?php   /************************** CHUCK *****/  ?>
<div id="chuck">
	<iframe src="chuck.php" id="chuckframe"></iframe>
</div>
<div id="shoutboxdiv">
<fieldset>
<legend>SHOUTBOX</legend>
&nbsp; <br />
<form action="shoutbox.php?action=submit" method="POST" target="shoutboxframe">
NICK: <input type="text" size="20" name="nick" />&nbsp;&nbsp;<input type="submit" value="shout!" />
<br />
<input type="text" size="40" name="msg"  onsubmit="this.value=''" onclick="this.value=''" />
</form>
<iframe src="shoutbox.php" name="shoutboxframe" id="shoutboxframe"></iframe>
</fieldset>
</div>
<br />
<div id="bottombox">
100% Web 2.0&copy;&trade;&reg; &amp; Ajax&copy;&trade;&reg; free!
</div>
<div id="misc">
<fieldset>
<legend>MISC</legend>
&nbsp; <br />
<a href="submitrequest.php?action=triggergarage" target="submitframe">Tor (toggle)</a>
</fieldset>
</div>
<div id="musicbox">
<fieldset>
<legend>MUSIC</legend>
&nbsp; <br />
<a href="submitrequest.php?action=mpc&amp;mpccmd=last" target="submitframe">&laquo;</a>
<a href="submitrequest.php?action=mpc&amp;mpccmd=next" target="submitframe">&raquo;</a>
<a href="submitrequest.php?action=mpc&amp;mpccmd=stop" target="submitframe">STOP</a>
<a href="submitrequest.php?action=mpc&amp;mpccmd=play" target="submitframe">PLAY</a>
<br />
<a href="submitrequest.php?action=mpc&amp;mpccmd=volminus" target="submitframe">-&nbsp;VOL</a>
<a href="submitrequest.php?action=mpc&amp;mpccmd=volplus" target="submitframe">VOL&nbsp;+</a>
<a href="submitrequest.php?action=mpc&amp;mpccmd=mute" target="submitframe">MUTE</a>
<a href="submitrequest.php?action=mpc&amp;mpccmd=pause" target="submitframe">PAUSE</a>

</fieldset>
</div>
<div id="reactorstatus">
<fieldset>
<legend>Reactor Status</legend>
<strong>Core Temp.:</strong> 1617.2&deg;C<br />
<strong>Outer hull Temp.:</strong>123.1&deg;C<br />
<strong>Cooling Pipe 1:</strong> 112.9&deg;C<br />
<strong>Cooling Pipe 2:</strong> 122.3&deg;C<br />
<strong>Cooling pressure:</strong> 20912mbar<br />
<strong>Nakka Level:</strong> 23nk<br />
</fieldset>
</div>

<div id="chuckstat">
<fieldset>
	<legend>Chuck status</legend>
	<strong>Chuck's humidity level:</strong> <input type="text" size="4" value="N/A"
		name="chuck_humidity" class="inpinvisible" />
</fieldset>
</div>
<div id="laufschrift">
<fieldset>
<legend>Laufschrift Borg</legend>
<iframe src="laufschrift.php"  name="lsframe" id="lsframe"></iframe>
</fieldset>
</div>

<div id="debugwindow">
<?php
require_once ('statusreload.php');
?>
</div>
</body>
</html>
