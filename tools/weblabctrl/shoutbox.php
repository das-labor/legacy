<?php
	header('Refresh: 1');
?>
<html>
<head>
<style type="text/css">
p.odd
{
	background-color:#E0E0E0;
}
p
{
	padding:0px;
	margin:0px;
}
</style>
</head>
<body>
<?php
function sanitize_input ($in_string)
{
	$tmp = htmlentities ( $in_string);
	$tmp = str_replace ("\r", '', $tmp);
	$tmp = str_replace ("\n", '', $tmp);

	return stripslashes($tmp);
}

$tmp = @file_get_contents ('tmp/shoutbox.txt');

$tmparr = explode ("\n", $tmp);


if ($_POST['nick'] && $_POST['msg'])
{
	if (empty($_POST['nick'])) $_POST['nick'] = 'Nameless Idiot';
	if (empty($_POST['msg'])) die('nope.');

	$nick = sanitize_input (substr($_POST['nick'], 0, 20));
	$msg = sanitize_input (substr($_POST['msg'], 0, 256));

	$tmpstr = '<small>' . date('(H:i:s) ') . '</small><strong>' .
		$nick . '</strong>: ' .
		$msg . "\n";

	for ($i = 0; $i < min (count($tmparr), 20); $i++)
	{
		if ($i % 2)
			echo '<p class="odd">' . $tmparr[$i] . '</p>';
		else
			echo '<p>' . $tmparr[$i] . '</p>';

		$tmpstr .= $tmparr[$i] . "\n";
	}
	
	file_put_contents ('tmp/shoutbox.txt', $tmpstr);
} else
{
	for ($i = 0; $i < min (count($tmparr), 20); $i++)
	{
		if ($i % 2)
			echo '<p class="odd">' . $tmparr[$i] . '</p>';
		else
			echo '<p>' . $tmparr[$i] . '</p>';

	}
}
?>
</body>
</html>
