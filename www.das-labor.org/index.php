<?php
//language switcher
if ( $_GET['m'] != 1 ){ // if manual flag set, do not check for language!
  $foo = explode(",", $_SERVER['HTTP_ACCEPT_LANGUAGE']);
  if ( !ereg("^de", $foo[0]) ){ // if ACCEPTED_LANGUAGES does not begin with 'de', redirect to english page
    header("Location: http://www.das-labor.org/index.en.php");
  }
}
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="de" lang="de"> 
<head> 
  <title>Das Labor - Bochum</title> 
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
  <meta http-equiv="Content-Style-Type" content="text/css" />
  <META NAME="ROBOTS" CONTENT="NOARCHIVE">
  <meta name="keywords" content="Das LABOR, Bochum, Verein, Hackerspace" />
  <meta name="description" content="LABOR e.V. Bochum. Weblog, Wiki, Termine, Projekte" />
  <meta name="publisher" content="Das Labor e.V." />
  <meta name="verify-v1" content="IjAb+nfZ0+AExGgM0czSCi0AWq6PLS0VPwjPlMsb7GA=" >
  <link rel="shortcut icon" href="http://www.das-labor.org/favicon.ico" />
  <link rel="stylesheet" type="text/css" href="main.css" />
  <link rel="copyright" href="/impressum.html" title="Impressum" />
  <link rel="P3Pv1" href="http://www.das-labor.org/p3p/p3p.xml" />
  <link rel="alternate" type="application/rss+xml"  title="Labor Blog (RSS 2.0)" href="http://www.das-labor.org/blog/index.php/feed/" />
  <link rel="alternate" type="application/atom+xml" title="Labor Blog (Atom 0.3)" href="http://www.das-labor.org/blog/index.php/feed/atom/" />
  <link rel="alternate" type="application/rss+xml"  title="Labor Termine (RSS 2.0)" href="http://www.das-labor.org/termine.rss" />

</head>

<body>
<div id="container">
<table>
<tr>
<td width="340" align="left">
  <a href="http://www.das-labor.org/"><img width="320" height="325" src="images/logo.png" alt="LABOR-Logo" style="border:none" /></a>
</td>
<td width="340" bgcolor="#ffffff">
&nbsp;
</td>
<td align="left" width="340" valign="bottom">
&nbsp; <img src="images/de_bleached.png" alt="german" style="border:none"> <a href="index.en.php"><img src="images/en.png" alt="switch to english" style="border:none"></a><br /><br />
&nbsp; <a href="http://www.das-labor.org/wiki/Status-Bot">LABOR-Status: <img src="status.png" alt="gruen = offen, rot = geschlossen, orange = kein Status" width="30" height="30" align="middle" style="border:none" /></a>
<br />
<br />
<br />
<br />
<ul>
<li><a href="http://www.das-labor.org/wiki/Kontakt">Kontakt</a></li>
<li><a href="http://www.das-labor.org/wiki/Projekte">Projekte</a></li>
<li><a href="http://www.das-labor.org/wiki/Wegbeschreibung">Wegbeschreibung</a></li>
<li><a href="http://www.das-labor.org/ueber.html">&Uuml;ber uns</a> (<em><a href="files/LABOR-Flyer.pdf">pdf</a></em>) (<em><a href="files/LABOR-Flyer.png">png</a></em>)</li>
<li><a href="impressum.html">Impressum</a></li>
<br />
<li><a href="http://www.das-labor.org/blog/">weblog</a></li>
<li><a href="http://planet.das-labor.org/">planet</a></li>
<li><a href="http://www.das-labor.org/wiki/">wiki</a></li>
</ul>
</td>
</tr>
</table>
<table>
<tr>
<td align="left" width="340" bgcolor="#ffffff">&nbsp;</td>
<td>&nbsp;</td>
</tr>
<tr>
<td align="left" width="515" bgcolor="#ffffff">
<a href="http://www.das-labor.org/wiki/Termine"><b>Termine</b></a>&nbsp; &nbsp; &nbsp;<a href="http://www.das-labor.org/termine.rss"><img src="images/rss.png" alt="rss 2.0" width="15" height="15" style="border:none" /></a> <a href="webcal://www.das-labor.org/termine.ics"><img src="images/ical.png" alt="iCal" width="20" height="20" style="border:none" /></a>
<!-- <a href="http://www.das-labor.org/files/Monatspr_small.pdf"><img src="pdf.png" alt="pdf" width="17" height="17" style="border:none" /></a> -->
</td>
<td align="center" width="165" bgcolor="#ffffff" nowrap>
&nbsp;</td>
</tr>
</table>
<table>
<tr>
<td align="left" width="455" bgcolor="#ffffff" nowrap>
<?php include(termine_html); ?>
</td>
<td align="center" width="300" bgcolor="#ffffff" valign="bottom">
<div id="blog_labor">
<div id="alert">Neues vom Blog:</div>
<a href="http://www.das-labor.org/blog/"><?php include "http://www.das-labor.org/blog/?microsummary=1"; ?></a>
<br />
<br />
</div>
</td>
</tr>
</table>
</div>
<div id="footer">
Das LABOR e.V., Bochum, Rottstr. 31, <a href="http://www.das-labor.org">www.das-labor.org</a>
</div>
</body>
</html>
