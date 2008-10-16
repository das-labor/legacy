<?php
function ipcheck()
{
	if (substr(getenv('REMOTE_ADDR'), 0, 3) != '10.'
		&& !preg_match('#^2001:0?6f8:13cb:#', getenv('REMOTE_ADDR'))
		&& !preg_match('#^2001:0?7b8:0?379:#', getenv('REMOTE_ADDR')))
			return 0;
	return 1;
}
?>
