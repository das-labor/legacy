<?

$defaults = array( "Lounge" => array("xpos" => "300px",
				     "ypos" => "60px",
				     "xpos_small" => "0px",
				     "ypos_small" => "0px",
				     'closed' => "closed",
				     "ctrl" => array()
				     ),
		   "Vortragsraum" => array("xpos" => "400px",
					   "ypos" => "90px",
					   "xpos_small" => "0px",
					   "ypos_small" => "25px",
					   'closed' => "closed",
					   "ctrl" => array()
					   ),
		   "Bastelraum" => array("xpos" => "500px",
					 "ypos" => "120px",
					 "xpos_small" => "0px",
					 "ypos_small" => "50px",
					 'closed' => "closed",
					 "ctrl" => array()
					 ),
		   "Keller" => array("xpos" => "600px",
				     "ypos" => "145px",
				     "xpos_small" => "0px",
				     "ypos_small" => "75px",
				     'closed' => "closed",
				     "ctrl" => array()
				     ),
		   "Musik" => array("xpos" => "700px",
					 "ypos" => "170px",
					 "xpos_small" => "0px",
					 "ypos_small" => "100px",
					 'closed' => "closed",
					 "ctrl" => array()
					 ),
		   "Licht" => array("xpos" => "800px",
					 "ypos" => "195px",
					 "xpos_small" => "0px",
					 "ypos_small" => "125px",
					 'closed' => "closed",
					 "ctrl" => array()
					 ),
		   "Spielzeug" => array("xpos" => "900px",
					 "ypos" => "220px",
					 "xpos_small" => "0px",
					 "ypos_small" => "150px",
					 'closed' => "closed",
					 "ctrl" => array()
					 ),
		   "LabStatus" => array("xpos" => "900px",
					 "ypos" => "220px",
					 "xpos_small" => "0px",
					 "ypos_small" => "200px",
					 'closed' => "open",
					 "ctrl" => array()
					 ),
		   "WebLabInterface" => array("xpos" => "900px",
					 "ypos" => "220px",
					 "xpos_small" => "0px",
					 "ypos_small" => "175px",
					 'closed' => "closed",
					 "ctrl" => array()
					 ),
		   "backgroundimage" => "1",
  		   );


if($_COOKIE['Lounge_xpos']) $defaults["Lounge"]["xpos"]=$_COOKIE['Lounge_xpos'];
if($_COOKIE['Lounge_ypos']) $defaults["Lounge"]["ypos"]=$_COOKIE['Lounge_ypos'];
if($_COOKIE['Lounge_xpos_small']) $defaults["Lounge"]["xpos_small"]=$_COOKIE['Lounge_xpos_small'];
if($_COOKIE['Lounge_ypos_small']) $defaults["Lounge"]["ypos_small"]=$_COOKIE['Lounge_ypos_small'];
if($_COOKIE['Lounge_closed']) $defaults["Lounge"]["closed"]=$_COOKIE['Lounge_closed'];

if($_COOKIE['Vortragsraum_xpos']) $defaults["Vortragsraum"]["xpos"]=$_COOKIE['Vortragsraum_xpos'];
if($_COOKIE['Vortragsraum_ypos']) $defaults["Vortragsraum"]["ypos"]=$_COOKIE['Vortragsraum_ypos'];
if($_COOKIE['Vortragsraum_xpos_small']) $defaults["Vortragsraum"]["xpos_small"]=$_COOKIE['Vortragsraum_xpos_small'];
if($_COOKIE['Vortragsraum_ypos_small']) $defaults["Vortragsraum"]["ypos_small"]=$_COOKIE['Vortragsraum_ypos_small'];
if($_COOKIE['Vortragsraum_closed']) $defaults["Vortragsraum"]["closed"]=$_COOKIE['Vortragsraum_closed'];

if($_COOKIE['Bastelraum_xpos']) $defaults["Bastelraum"]["xpos"]=$_COOKIE['Bastelraum_xpos'];
if($_COOKIE['Bastelraum_ypos']) $defaults["Bastelraum"]["ypos"]=$_COOKIE['Bastelraum_ypos'];
if($_COOKIE['Bastelraum_xpos_small']) $defaults["Bastelraum"]["xpos_small"]=$_COOKIE['Bastelraum_xpos_small'];
if($_COOKIE['Bastelraum_ypos_small']) $defaults["Bastelraum"]["ypos_small"]=$_COOKIE['Bastelraum_ypos_small'];
if($_COOKIE['Bastelraum_closed']) $defaults["Bastelraum"]["closed"]=$_COOKIE['Bastelraum_closed'];

if($_COOKIE['Keller_xpos']) $defaults["Keller"]["xpos"]=$_COOKIE['Keller_xpos'];
if($_COOKIE['Keller_ypos']) $defaults["Keller"]["ypos"]=$_COOKIE['Keller_ypos'];
if($_COOKIE['Keller_xpos_small']) $defaults["Keller"]["xpos_small"]=$_COOKIE['Keller_xpos_small'];
if($_COOKIE['Keller_ypos_small']) $defaults["Keller"]["ypos_small"]=$_COOKIE['Keller_ypos_small'];
if($_COOKIE['Keller_closed']) $defaults["Keller"]["closed"]=$_COOKIE['Keller_closed'];

if($_COOKIE['Licht_xpos']) $defaults["Licht"]["xpos"]=$_COOKIE['Licht_xpos'];
if($_COOKIE['Licht_ypos']) $defaults["Licht"]["ypos"]=$_COOKIE['Licht_ypos'];
if($_COOKIE['Licht_xpos_small']) $defaults["Licht"]["xpos_small"]=$_COOKIE['Licht_xpos_small'];
if($_COOKIE['Licht_ypos_small']) $defaults["Licht"]["ypos_small"]=$_COOKIE['Licht_ypos_small'];
if($_COOKIE['Licht_closed']) $defaults["Licht"]["closed"]=$_COOKIE['Licht_closed'];

if($_COOKIE['Musik_xpos']) $defaults["Musik"]["xpos"]=$_COOKIE['Musik_xpos'];
if($_COOKIE['Musik_ypos']) $defaults["Musik"]["ypos"]=$_COOKIE['Musik_ypos'];
if($_COOKIE['Musik_xpos_small']) $defaults["Musik"]["xpos_small"]=$_COOKIE['Musik_xpos_small'];
if($_COOKIE['Musik_ypos_small']) $defaults["Musik"]["ypos_small"]=$_COOKIE['Musik_ypos_small'];
if($_COOKIE['Musik_closed']) $defaults["Musik"]["closed"]=$_COOKIE['Musik_closed'];

if($_COOKIE['Spielzeug_xpos']) $defaults["Spielzeug"]["xpos"]=$_COOKIE['Spielzeug_xpos'];
if($_COOKIE['Spielzeug_ypos']) $defaults["Spielzeug"]["ypos"]=$_COOKIE['Spielzeug_ypos'];
if($_COOKIE['Spielzeug_xpos_small']) $defaults["Spielzeug"]["xpos_small"]=$_COOKIE['Spielzeug_xpos_small'];
if($_COOKIE['Spielzeug_ypos_small']) $defaults["Spielzeug"]["ypos_small"]=$_COOKIE['Spielzeug_ypos_small'];
if($_COOKIE['Spielzeug_closed']) $defaults["Spielzeug"]["closed"]=$_COOKIE['Spielzeug_closed'];

if($_COOKIE['WebLabInterface_xpos']) $defaults["WebLabInterface"]["xpos"]=$_COOKIE['WebLabInterface_xpos'];
if($_COOKIE['WebLabInterface_ypos']) $defaults["WebLabInterface"]["ypos"]=$_COOKIE['WebLabInterface_ypos'];
if($_COOKIE['WebLabInterface_xpos_small']) $defaults["WebLabInterface"]["xpos_small"]=$_COOKIE['WebLabInterface_xpos_small'];
if($_COOKIE['WebLabInterface_ypos_small']) $defaults["WebLabInterface"]["ypos_small"]=$_COOKIE['WebLabInterface_ypos_small'];
if($_COOKIE['WebLabInterface_closed']) $defaults["WebLabInterface"]["closed"]=$_COOKIE['WebLabInterface_closed'];

if($_COOKIE['backgroundimage']) $defaults['backgroundimage']=$_COOKIE['backgroundimage'];


if($_COOKIE['LabStatus_xpos']) $defaults["LabStatus"]["xpos"]=$_COOKIE['LabStatus_xpos'];
if($_COOKIE['LabStatus_ypos']) $defaults["LabStatus"]["ypos"]=$_COOKIE['LabStatus_ypos'];
if($_COOKIE['LabStatus_xpos_small']) $defaults["LabStatus"]["xpos_small"]=$_COOKIE['LabStatus_xpos_small'];
if($_COOKIE['LabStatus_ypos_small']) $defaults["LabStatus"]["ypos_small"]=$_COOKIE['LabStatus_ypos_small'];
if($_COOKIE['LabStatus_closed']) $defaults["LabStatus"]["closed"]=$_COOKIE['LabStatus_closed'];


// $bg='dwarf';
// $toiletlamp=0;
// $pwmx=10;
// $pwmy=60;
// $mpdx=310;
// $mpdy=60;
// $miscx=570;
// $miscy=180;
// $vortragsdevsx=570;
// $vortragsdevsy=60;
// $treppenblinkx=830;
// $treppenblinky=60;
// $mybackgroundx=830;
// $mybackgroundy=120;


// if($_COOKIE['bg']) $bg=$_COOKIE['bg'];
// if($_COOKIE['toiletlamp']) $toiletlamp=$_COOKIE['toiletlamp'];
// if($_COOKIE['pwmx']) $pwmx=$_COOKIE['pwmx'];
// if($_COOKIE['pwmy']) $pwmy=$_COOKIE['pwmy'];
// if($_COOKIE['mpdx']) $mpdx=$_COOKIE['mpdx'];
// if($_COOKIE['mpdy']) $mpdy=$_COOKIE['mpdy'];
// if($_COOKIE['miscx']) $miscx=$_COOKIE['miscx'];
// if($_COOKIE['miscy']) $miscy=$_COOKIE['miscy'];
// if($_COOKIE['vortragsdevsx']) $vortragsdevsx=$_COOKIE['vortragsdevsx'];
// if($_COOKIE['vortragsdevsy']) $vortragsdevsy=$_COOKIE['vortragsdevsy'];
// if($_COOKIE['mybackgroundx']) $mybackgroundx=$_COOKIE['mybackgroundx'];
// if($_COOKIE['mybackgroundy']) $mybackgroundy=$_COOKIE['mybackgroundy'];

?>