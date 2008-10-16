<?php
/* reloads parent site upon status change
 */

require_once ('conf/lapd.php');
require_once ('conf/lights.php');

$fp = pfsockopen(CONF_LAPD_ADDR, CONF_LAPD_PORT, $errno, $errstr, 30);

if (!$fp) die( '<h1>errr!!!</h1>');

// first initialization, send WATCH commands to server
if (!isset($_GET['status']))
{
	for ($i = 0;$i < count($conf_light);$i++)
	{
		fwrite ($fp, 'watch ' . $conf_light[$i]['command'] . "\n");
	}
}

stream_set_blocking ($fp, 0);

$lastchuckpoll = 0;

while (!feof($fp))
{
	if ($lastchuckpoll < (time()-300))
	{
		fwrite ($fp, "get CHUCK humidity\n");
		$lastchuckpoll = time();
	}

	echo "\n";
	$tmp = fgets ($fp, 256);

	if (preg_match('#^info ([^ ]+) (1|on|off|0)#i', $tmp, $m))
	{
		echo 'status change<br />';
		flush();
		$status[$m[1]] = $m[2];
		if ($m[2] == '0' || strtolower($m[2]) == 'off')
		{
			echo '<script language="javascript">document.getElementById' .
				'("' . strtoid($m[1]) . 
				'").style.backgroundColor=\'#F1F1F1\';</script>';
		} else
		{
			echo '<script language="javascript">document.getElementById' .
				'("' . strtoid($m[1]) .
				'").style.backgroundColor=\'#00FF00\';</script>';
		}
		flush();
	} else if (preg_match('#CHUCK humidity ([0-9]+).*#i', $tmp, $m))
	{
		echo '<script language="javascript">document.getElementById("chuck_humidity").' .
			'value = "' . $m[1] . '";</script>';
	}

	usleep (100000);
	flush();
}
?>
