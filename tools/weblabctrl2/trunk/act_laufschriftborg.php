<?
require_once "container.php";

if($_GET["doit"])
  {
    $myobj = new c_lsborg();
    $cmd = $myobj->isvalidmode($_GET["function"]);

    if($cmd=="coin")
      {
	$coin = 'number';
	if (rand(0,1) == '0') $coin = 'head';
	exec("powercommander.lapcontrol -s rl borg scroll 0x24 '2<5|+30/#I fliped a coin and got ".$coin."'");
	exec("powercommander.lapcontrol -s rl borg mode 0x24 1");
      }
    elseif($cmd=="dice")
      {
	srand (time() );
	$dicevalue=rand(1,6);
	exec("powercommander.lapcontrol -s rl borg scroll 0x24 '2<5|+30/#I rolled a dice and got ".$dicevalue."'");
	exec("powercommander.lapcontrol -s rl borg mode 0x24 1");
      }
    elseif($cmd=="w20")
      {
	srand (time() );
	$dicevalue=rand(1,20);
	if($dicevalue == 20) $dicevalue = 'FULL FAIL';
	if($dicevalue == 1) $dicevalue = 'Perfect';
	exec("powercommander.lapcontrol -s rl borg scroll 0x24 '2<5|+30/#I rolled a W20 and got ".$dicevalue."'");
	exec("powercommander.lapcontrol -s rl borg mode 0x24 1");
      }
    elseif($cmd=="text")
      {
	exec("powercommander.lapcontrol -s rl borg scroll 0x24 '2<5|+30/#".escapeshellcmd($_GET["value"])."'");
	exec("powercommander.lapcontrol -s rl borg mode 0x24 1");
      }
    else
      {
	exec("powercommander.lapcontrol -s rl borg scroll 0x24 '2<5|+30/#Ich glaub es ist ein Hacker!'");
	exec("powercommander.lapcontrol -s rl borg mode 0x24 1");
      }
    file_put_contents($file, serialize( $localstate )); 
    
  }
?>
