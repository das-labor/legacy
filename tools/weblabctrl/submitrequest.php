<?php
require_once ('conf/lapd.php');
require_once ('ipcheck.php');

function parsempccommand ($in_string, $in_fp)
{
	switch ($in_string)
	{
		case 'last':
			return 'previous';

		case 'next':
			return 'next';

		case 'stop':
			return 'stop';

		case 'play':
			return 'play';

		case 'volminus':
		case 'volplus':
			fwrite ($in_fp, "status\r\n");
			while ($tmp = fgets ($in_fp))
			{
				echo 'received: ' . $tmp . '<br />';
				if (preg_match('#^volume *:? *([0-9]+)#i', $tmp, $m))
				{
					print_r ($m);
					if ($in_string == 'volplus' && (int) $m[1] <= 95)
						$strout = 'setvol ' . ((int)$m[1] + 5);
					
					if ($in_string == 'volminus' && (int) $m[1] >= 5)
						$strout = 'setvol ' . ((int)$m[1] - 5);
				}
				if (preg_match ('#^ok#i', $tmp)) break;
			}
			echo 'sending: ' . $strout;
			return $strout;

		case 'mute':
			return 'setvol 0';

		case 'pause':
			return 'pause';	
	}
	return FALSE;
}
function waitforok ($in_fp)
{
	$starttime = time();
	while ($starttime + 5 > time() && !preg_match('#^ok#i', fgets($in_fp)));
}

$fp = pfsockopen(CONF_LAPD_ADDR, CONF_LAPD_PORT, $errno, $errstr, 30);

switch ($_GET['action'])
{
	case 'togglelight':
		if (ipcheck())
			fwrite ($fp, 'toggle ' . strtoupper($_GET['light']) . "\r\n");
	break;
	case 'triggergarage':
		system ('toroeffner');	
	break;
	case 'mpc':
		if (!ipcheck()) break;

		$fpmpc = pfsockopen ('127.0.0.1', 6600, $errnompc, $errstrmpc, 30);
		waitforok ($fpmpc);
		$tmp = parsempccommand ($_GET['mpccmd'], $fpmpc);
		if ($tmp) fwrite ($fpmpc, $tmp . "\r\n");
		waitforok ($fpmpc);
	break;
	case 'dimspot':
		$myspot =  dechex(intval($_GET['spotid']));
		$mylevel = dechex(intval($_GET['level']));

		system ('lapcontrol packet 0x00:0x20 0x35:0x20 0x00,0x' . $myspot . ',0x' . $mylevel . ' > /dev/null'); 
	break;
}
?>
