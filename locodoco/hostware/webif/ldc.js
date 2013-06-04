function ldc_send(in_cmd, in_target, in_val = null)
{
	var xr = new XMLHttpRequest();
	var rq = 'command.ldc?target=' + in_target + '&cmd=' + in_cmd +
		'&rand=' + Math.random();
	
	if (in_val != null)
		rq += 'u16val=' + in_val;
	
	xr.open ('GET', rq);
	xr.send(null);
}
