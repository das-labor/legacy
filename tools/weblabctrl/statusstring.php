<?php
/* functions for de- and encoding status information
 * in (HTTP GET-) strings
 */

function statusstring_encode ($in_array)
{
	reset ($in_array);
	while (list($key, $val) = each($in_array))
	{
		$str_out .= $key . '=' . $val . ';';
	}
	if (function_exists('bzcompress'))
	{
		return base64_encode(bzcompress($str_out, 9));
	} else
	{
		return base64_encode($str_out);
	}
}

function statusstring_decode ($in_string)
{
	if (function_exists('bzdecompress'))
	{
		$in_string = bzdecompress(base64_decode($in_string));
	} else
	{
		$in_string = base64_decode($in_string);
	}
	$a = explode (';', $in_string);
	while (list (, $val) = each ($a))
	{
		$b = explode ('=', $val);
		$out_arr[$b[0]] = $b[1];
	}
	return $out_arr;
}

function strtoid ( $in_str )
{
	return strtolower(str_replace (' ', '__', $in_str));
}

function idtostr ( $in_str )
{
	return str_replace ('__', ' ', $in_str);
}
?>
