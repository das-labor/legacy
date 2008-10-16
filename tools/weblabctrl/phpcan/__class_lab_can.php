<?php
require_once 'can_config.php';
class lab_can {
	// 'ping' the daemon - open a socket connection and close it immediately.
	// return values: true, false
	function ping_cand () {
		if (!($fp = @fsockopen(CAND_HOST, CAND_PORT)))
		{
			fclose ($fp);
			return TRUE;
		}
		return FALSE;

	}
	function sendto ($in_device_id, $in_nessage)
	{
		$fp = fsockopen (CAND_HOST, CAND_PORT);
	}
}
?>
