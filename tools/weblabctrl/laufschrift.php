<?php

if (isset($_POST['borg_text'])) header('Refresh: 3');
?>
<html>
<head>
	<title>laufschrift interface</title>
	<style type="text/css">
	input {
		border-style:solid;
		border-width:2px;
		margin-left:auto;
		margin-right:auto;
		display:block;	
	}
	body {
		font-family:Verdana,Arial,Helvetica;
		font-size:12px;
		text-align:center;
	}
	</style>
</head>
<body>
<?php
if (isset($_POST['borg_text']))
{
	$_POST['borg_text'] = preg_replace('#"#', '\"', $_POST['borg_text']);
	$_POST['borg_text'] = preg_replace('$#$', '\\#', $_POST['borg_text']);
	$_POST['borg_text'] = substr($_POST['borg_text'], 0, 123);
	exec ('lapcontrol borg scroll 0x24 "</#' . $_POST['borg_text'] . '"'); 
	usleep (500000);
	exec ('lapcontrol borg mode 0x24 1');
	echo 'your text was sent to the borg.<br />thank you for using our hardware and have a nice day!';
} else
{
?>
<form action="<?php echo $PHP_SELF; ?>" method="post">
ENTER YOUR TEXT BELOW AND PRESS SEND
<input type="text" size="35" name="borg_text" /> <br />
<input type="submit" value="send." />
</form>
<?php
}
?>
</body>
</html>
