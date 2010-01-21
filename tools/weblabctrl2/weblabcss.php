<?
include "config.php";
?>

<style type=\"text/css\">
  * {font-family:Arial;}
  div.slider { width:256px; margin:10px 0; background-color:#ccc; height:10px; position: relative; }
  div.slider div.handle { width:10px; height:15px; background-color:#f00; cursor:move; position: absolute; }
  div.mybackground_top {background-color:#ccccff; width: 256px; margin-bottom:5px;}
  body {margin: 0px; padding: 0px; background-image:url(bg/11.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.eins {background-size: auto; width: 100%; height: 100%; margin: 0px; padding: 0px; background-image:url(bg/1.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.zwei {margin: 0px; padding: 0px; background-image:url(bg/2.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.drei {margin: 0px; padding: 0px; background-image:url(bg/3.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.vier {margin: 0px; padding: 0px; background-image:url(bg/4.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.fuenf {margin: 0px; padding: 0px; background-image:url(bg/5.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.sechs {margin: 0px; padding: 0px; background-image:url(bg/6.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.sieben {margin: 0px; padding: 0px; background-image:url(bg/7.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.acht {margin: 0px; padding: 0px; background-image:url(bg/8.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.neun {margin: 0px; padding: 0px; background-image:url(bg/9.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.zehn {margin: 0px; padding: 0px; background-image:url(bg/10.jpg);background-repeat:no-repeat;background-color:#000000;}
  body.dwarf {margin: 0px; padding: 0px; background-image:url(bg/11.jpg);background-repeat:no-repeat;background-color:#000000;}
  input.VORTRAG_button { border-size: 0px; border-style: none; background: transparent; cursor: hand; padding: 0px;}
  div.pwm_top {background-color:#ccccff; width: 256px; margin-bottom:5px;}
  div.mpd_top {background-color:#ccccff; width: 256px; margin-bottom:5px;}
  div.misc_top {background-color:#ccccff; width: 256px; margin-bottom:5px;}
  div.vortragsdevs_top {background-color:#ccccff; width: 256px; margin-bottom:5px;}
  div.treppenblink_top {background-color:#ccccff; width: 256px; margin-bottom:5px;}
  div.treppenblink_cont {background-color:#ccccff; overflow: hidden; width: 256px; margin-bottom:5px; height: 0px}
  input {}

<?
echo " div.pwm {position:absolute; left:".$pwmx."; top: ".$pwmy.";    background-image:url(divbg256.png); background-repeat:repeat-y; background-color: transparent; width: 256px; padding:10px;}
  div.mpd {position:absolute; left:".$mpdx."; top: ".$mpdy.";    background-image:url(divbg256.png); background-repeat:repeat-y; background-color: transparent; width: 256px; padding:10px;}
  div.misc {position:absolute; left:".$miscx."; top: ".$miscy."; background-image:url(divbg256.png); background-repeat:repeat-y; background-color: transparent; width: 256px; padding:10px;}
  div.vortragsdevs {position:absolute; left:".$vortragsdevsx."; top: ".$vortragsdevsy."; background-image:url(divbg256.png); background-repeat:repeat-y; background-color: transparent; width: 256px; padding:10px;}
  div.treppenblink {position:absolute; left:".$treppenblinkx."; top: ".$treppenblinky."; background-image:url(divbg256.png); background-repeat:repeat-y; background-color: transparent; width: 256px; padding:10px;}
  div.mybackground {position:absolute; left:".$mybackgroundx."; top: ".$mybackgroundy."; background-image:url(divbg256.png); background-repeat:repeat-y; background-color: transparent; width: 256px; padding:10px;}
";
?>

</style>
