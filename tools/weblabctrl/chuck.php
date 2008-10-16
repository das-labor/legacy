<?php
define ('CONF_CHUCKSTATFILE', '/etc/chuck/fed');
define ('CONF_CHUCKFEEDFILE', '/etc/chuck/feed');
?>
<html>
<head>
	<title>Chuck Control</title>
	<style type="text/css">
	input.btn
	{
		border-width:1px;
		border-style:outset;
		color:#FFFFFF;
		font-weight:bold;
		background-color:#009900;
	}
	body {
		font-family:Verdana,Arial;
		background-color:#FFFFFF;
		padding:0px;
		margin:0px;
	}
	form {
		text-align:center;
		vertical-align:middle;
		padding:0px;
		margin:0px;
	}
	fieldset {
		background-color:#FFFFFF;
		position:absolute;
		display:table;
		border-style:solid;
		border-color:#000000;
		text-align:center;
		border-width:2px;
		vertical-align:middle;
		-moz-border-radius:10px;
		width:280px;
	}
	legend {
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
	img {
		float:left;

	}
	img#humbar {
		float:right;
		padding:10px;
	}	
	</style>
</head>
<body>
<form method="POST" action="<?php echo $_SERVER['SCRIPT_NAME']; ?>">
<fieldset>
<legend>CHUCK CONTROL</legend>
<img src="img/chuck.png" alt="chuck" />
<img src="chuck_humidity_bar.php" id="humbar" />
<br />
<?php
$pingresult = shell_exec ('ping -w 1 -c 1 10.0.1.3');

if (strstr ($pingresult, '100% packet loss') != "")
{
	echo 'sorry, the labor is currently offline.';
} else if (($_POST['chuck_enable'] && !@stat(CONF_CHUCKSTATFILE)) ||
	@stat(CONF_CHUCKFEEDFILE))
{
	touch (CONF_CHUCKFEEDFILE);
	touch (CONF_CHUCKSTATFILE);
?>
*slurp*
<?php
} else if (!@stat(CONF_CHUCKSTATFILE))
{
?>

<input type="submit" name="chuck_enable" value="Feed it!" class="btn" />
<?php
} else
{
?>
Chuck is fed for today, but thanks for caring!
<?php
}
?>
</fieldset>
</form>
</body>
</html>
