<?
$localstate=array();
include_once "container.php";

echo "<html>
<head>
<script src=\"scriptaculous/prototype.js\" type=\"text/javascript\"></script>
<script src=\"scriptaculous/scriptaculous.js\" type=\"text/javascript\"></script>


<style type=\"text/css\">\n";

echo "img#bg {\n";
echo " position:fixed;\n";
echo " top:0;\n";
echo " left:0;\n";
echo " width:100%;\n";
echo " height:100%;\n";
echo "}\n";

echo $vortrag->getcss();


echo $lounge->getcss();

echo $bastel->getcss();

echo $keller->getcss();

echo $musik->getcss();

echo $licht->getcss();

echo $spielzeug->getcss();

echo $weblabinterface->getcss();

echo $labstatus->getcss();


echo "</style></head><body style=\"margin: 0px; padding: 0px; \">";

echo "<img src=\"bg/".$defaults['backgroundimage'].".jpg\" alt=\"background image\" id=\"bg\" />";


echo $musik->getctx();

echo $vortrag->getctx();

echo $lounge->getctx();

echo $bastel->getctx();

echo $keller->getctx();

echo $licht->getctx();

echo $spielzeug->getctx();

echo $weblabinterface->getctx();

echo $labstatus->getctx();


echo "</div>";
echo "<script>\n var done=true;\n</script>";
echo "<script>\n".$lounge->getjs()."\n</script>";

echo "<script>\n".$vortrag->getjs()."\n</script>";

echo "<script>\n".$bastel->getjs()."\n</script>";

echo "<script>\n".$keller->getjs()."\n</script>";

echo "<script>\n".$musik->getjs()."\n</script>";

echo "<script>\n".$licht->getjs()."\n</script>";

echo "<script>\n".$spielzeug->getjs()."\n</script>";

echo "<script>\n".$weblabinterface->getjs()."\n</script>";

echo "<script>\n".$labstatus->getjs()."\n</script>";


// global updates - dont stress apache
echo "<script>";
echo "function global_updater(){\n";
echo "new Ajax.Updater(\"global_updater_status\",\"general_update.php?";
echo "update=1\",";
echo "{method:'get', evalScripts: true});\n";
echo "}\n";
echo "var global_updater_interval = window.setInterval(\"global_updater()\", 10000);";
    //initial beim laden der seite
echo "global_updater()\n";
echo "</script>";

echo "<div id=\"ajax\">#debug</div>";
echo "<div id=\"global_updater_status\"></div>";

echo "</body></html>\n";

#finaly store state
echo $file;

var_dump($localstate);
file_put_contents($file, serialize( $localstate )); 
?>