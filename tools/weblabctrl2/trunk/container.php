<?

#reload clientconfig
require_once "system_conf.php";
include "user_conf.php";
include "obj_stylepannels.php";
include "obj_stylebg.php";
include "obj_treppenblink.php";
include "obj_irbeamer.php";
include "obj_irteufel.php";
include "obj_lsborg.php";
include "obj_moodbar.php";
include "obj_lightvortragmeta.php";
include "obj_lightvortragdetail.php";
include "obj_lightkueche.php";
include "obj_lightlounge.php";
include "obj_musikvortrag.php";
include "obj_musikbastel.php";
include "obj_musiklounge.php";
include "obj_musikkeller.php";
include "obj_status.php";
include "obj_speek.php";
include "obj_sound.php";
include "obj_muell.php";

$objlist = array();
$sortedobjects = array();

$ctrlelement_treppenblink = new c_treppenblink();
$ctrlelement_treppenblink->setdisplayname("Treppenblink");
array_push($objlist,$ctrlelement_treppenblink);
$encap_treppenblink = new c_identifier($ctrlelement_treppenblink);

$ctrlelement_irbeamer = new c_irbeamer();
$ctrlelement_irbeamer->setdisplayname("Beamer");
array_push($objlist,$ctrlelement_irbeamer);
$encap_irbeamer = new c_identifier($ctrlelement_irbeamer);

$ctrlelement_irteufel = new c_irteufel();
$ctrlelement_irteufel->setdisplayname("Teufel");
array_push($objlist,$ctrlelement_irteufel);
$encap_irteufel = new c_identifier($ctrlelement_irteufel);

$ctrlelement_lsborg = new c_lsborg();
$ctrlelement_lsborg->setdisplayname("Laufschriftborg");
array_push($objlist,$ctrlelement_lsborg);
$encap_lsborg = new c_identifier($ctrlelement_lsborg);

$ctrlelement_moodbar = new c_moodbar();
$ctrlelement_moodbar->setdisplayname("Moodbar");
array_push($objlist,$ctrlelement_moodbar);
$encap_moodbar = new c_identifier($ctrlelement_moodbar);

$ctrlelement_lightvortragmeta = new c_lightvortragmeta();
$ctrlelement_lightvortragmeta->setdisplayname("Vortragslicht");
array_push($objlist,$ctrlelement_lightvortragmeta);
$encap_lightvortragmeta = new c_identifier($ctrlelement_lightvortragmeta);

$ctrlelement_lightvortragdetail = new c_lightvortragdetail();
$ctrlelement_lightvortragdetail->setdisplayname("Vortragslicht Details");
array_push($objlist,$ctrlelement_lightvortragdetail);
$encap_lightvortragdetail = new c_identifier($ctrlelement_lightvortragdetail);

$ctrlelement_lightkueche = new c_lightkueche();
$ctrlelement_lightkueche->setdisplayname("Kuechenlicht");
array_push($objlist,$ctrlelement_lightkueche);
$encap_lightkueche = new c_identifier($ctrlelement_lightkueche);

$ctrlelement_lightlounge = new c_lightlounge();
$ctrlelement_lightlounge->setdisplayname("Loungelicht");
array_push($objlist,$ctrlelement_lightlounge);
$encap_lightlounge = new c_identifier($ctrlelement_lightlounge);

$ctrlelement_musikvortrag = new c_musikvortrag();
$ctrlelement_musikvortrag->setdisplayname("Vortrag Musik");
array_push($objlist,$ctrlelement_musikvortrag);
$encap_musikvortrag = new c_identifier($ctrlelement_musikvortrag);

$ctrlelement_musiklounge = new c_musiklounge();
$ctrlelement_musiklounge->setdisplayname("Lounge Musik");
array_push($objlist,$ctrlelement_musiklounge);
$encap_musiklounge = new c_identifier($ctrlelement_musiklounge);

$ctrlelement_musikbastel = new c_musikbastel();
$ctrlelement_musikbastel->setdisplayname("Bastel Musik");
array_push($objlist,$ctrlelement_musikbastel);
$encap_musikbastel = new c_identifier($ctrlelement_musikbastel);

$ctrlelement_musikkeller = new c_musikkeller();
$ctrlelement_musikkeller->setdisplayname("Keller Musik");
array_push($objlist,$ctrlelement_musikkeller);
$encap_musikkeller = new c_identifier($ctrlelement_musikkeller);

$ctrlelement_labstatus = new c_status();
$ctrlelement_labstatus->setdisplayname("Temperaturen");
array_push($objlist,$ctrlelement_labstatus);
$encap_labstatus = new c_identifier($ctrlelement_labstatus);

$ctrlelement_speach = new c_speek();
$ctrlelement_speach->setdisplayname("Text To Speach");
array_push($objlist,$ctrlelement_speach);
$encap_speach = new c_identifier($ctrlelement_speach);


$ctrlelement_musikvortrag2 = new c_musikvortrag();
$ctrlelement_musikvortrag2->setdisplayname("Vortrag Musik");
$ctrlelement_musikvortrag2->setid("id_musikvortrag2");
array_push($objlist,$ctrlelement_musikvortrag2);
$encap_musikvortrag2 = new c_identifier($ctrlelement_musikvortrag2);

$ctrlelement_musikbastel2 = new c_musikbastel();
$ctrlelement_musikbastel2->setdisplayname("Bastel Musik");
$ctrlelement_musikbastel2->setid("id_musikbastel2");
array_push($objlist,$ctrlelement_musikbastel2);
$encap_musikbastel2 = new c_identifier($ctrlelement_musikbastel2);

$ctrlelement_musikkeller2 = new c_musikkeller();
$ctrlelement_musikkeller2->setdisplayname("Keller Musik");
$ctrlelement_musikkeller2->setid("id_musikkeller2");
array_push($objlist,$ctrlelement_musikkeller2);
$encap_musikkeller2 = new c_identifier($ctrlelement_musikkeller2);


$ctrlelement_musiklounge2 = new c_musiklounge();
$ctrlelement_musiklounge2->setdisplayname("Lounge Musik");
$ctrlelement_musiklounge2->setid("id_musiklounge2");
array_push($objlist,$ctrlelement_musiklounge2);
$encap_musiklounge2 = new c_identifier($ctrlelement_musiklounge2);


$ctrlelement_lightvortragmeta2 = new c_lightvortragmeta();
$ctrlelement_lightvortragmeta2->setdisplayname("Vortragslicht");
$ctrlelement_lightvortragmeta2->setid("id_lightvortragsmeta2");
array_push($objlist,$ctrlelement_lightvortragmeta2);
$encap_lightvortragmeta2 = new c_identifier($ctrlelement_lightvortragmeta2);

$ctrlelement_lightvortragdetail2 = new c_lightvortragdetail();
$ctrlelement_lightvortragdetail2->setdisplayname("Vortragslicht Details");
$ctrlelement_lightvortragdetail2->setid("id_lightvortragsdetail2");
array_push($objlist,$ctrlelement_lightvortragdetail2);
$encap_lightvortragdetail2 = new c_identifier($ctrlelement_lightvortragdetail2);

$ctrlelement_lightkueche2 = new c_lightkueche();
$ctrlelement_lightkueche2->setdisplayname("Kuechenlicht");
$ctrlelement_lightkueche2->setid("id_lightkueche2");
array_push($objlist,$ctrlelement_lightkueche2);
$encap_lightkueche2 = new c_identifier($ctrlelement_lightkueche2);

$ctrlelement_lightlounge2 = new c_lightlounge();
$ctrlelement_lightlounge2->setdisplayname("Loungelicht");
$ctrlelement_lightlounge2->setid("id_lightlounge2");
array_push($objlist,$ctrlelement_lightlounge2);
$encap_lightlounge2 = new c_identifier($ctrlelement_lightlounge2);

$ctrlelement_stylepannel = new c_stylepannels();
$ctrlelement_stylepannel->setdisplayname("Style Pannel");
$ctrlelement_stylepannel->setid("id_stylepannel");
array_push($objlist,$ctrlelement_stylepannel);
$encap_stylepannel = new c_identifier($ctrlelement_stylepannel);

$ctrlelement_stylebg = new c_stylebg();
$ctrlelement_stylebg->setdisplayname("Style Pannel");
$ctrlelement_stylebg->setid("id_stylebg");
array_push($objlist,$ctrlelement_stylebg);
$encap_stylebg = new c_identifier($ctrlelement_stylebg);

$ctrlelement_sound = new c_sound();
$ctrlelement_sound->setdisplayname("Play Sound");
array_push($objlist,$ctrlelement_sound);
$encap_sound = new c_identifier($ctrlelement_sound);

$ctrlelement_muell = new c_muell();
$ctrlelement_muell->setdisplayname("Muell");
array_push($objlist,$ctrlelement_muell);
$encap_muell = new c_identifier($ctrlelement_muell);






$lounge = new c_pannel( "<center>Lounge</center>", "Lounge",
			$defaults["Lounge"]["xpos_small"],
			$defaults["Lounge"]["ypos_small"],
			$defaults["Lounge"]["xpos"],
			$defaults["Lounge"]["ypos"],
			$defaults["Lounge"]["closed"]);

$vortrag = new c_pannel( "<center>Vortragsraum</center>", "Vortragsraum",
			 $defaults["Vortragsraum"]["xpos_small"],
			 $defaults["Vortragsraum"]["ypos_small"],
			 $defaults["Vortragsraum"]["xpos"],
			 $defaults["Vortragsraum"]["ypos"],
			 $defaults["Vortragsraum"]["closed"]);

$bastel = new c_pannel( "<center>Bastelraum</center>", "Bastelraum",
			 $defaults["Bastelraum"]["xpos_small"],
			 $defaults["Bastelraum"]["ypos_small"],
			 $defaults["Bastelraum"]["xpos"],
			 $defaults["Bastelraum"]["ypos"],
			 $defaults["Bastelraum"]["closed"]);

$keller = new c_pannel( "<center>Keller</center>", "Keller",
			 $defaults["Keller"]["xpos_small"],
			 $defaults["Keller"]["ypos_small"],
			 $defaults["Keller"]["xpos"],
			 $defaults["Keller"]["ypos"],
			 $defaults["Keller"]["closed"]);

$musik = new c_pannel( "<center>Musik</center>", "Musik",
			 $defaults["Musik"]["xpos_small"],
			 $defaults["Musik"]["ypos_small"],
			 $defaults["Musik"]["xpos"],
			 $defaults["Musik"]["ypos"],
			 $defaults["Musik"]["closed"]);

$licht = new c_pannel( "<center>Licht</center>", "Licht",
			 $defaults["Licht"]["xpos_small"],
			 $defaults["Licht"]["ypos_small"],
			 $defaults["Licht"]["xpos"],
			 $defaults["Licht"]["ypos"],
			 $defaults["Licht"]["closed"]);

$spielzeug = new c_pannel( "<center>Spielzeug</center>", "Spielzeug",
			   $defaults["Spielzeug"]["xpos_small"],
			   $defaults["Spielzeug"]["ypos_small"],
			   $defaults["Spielzeug"]["xpos"],
			   $defaults["Spielzeug"]["ypos"],
			   $defaults["Spielzeug"]["closed"]);

$weblabinterface = new c_pannel( "<center>WebLabInterface</center>", "WebLabInterface",
				 $defaults["WebLabInterface"]["xpos_small"],
				 $defaults["WebLabInterface"]["ypos_small"],
				 $defaults["WebLabInterface"]["xpos"],
				 $defaults["WebLabInterface"]["ypos"],
				 $defaults["WebLabInterface"]["closed"]);

$labstatus = new c_pannel("<center>Lab Status</center>","LabStatus",
			  $defaults["LabStatus"]["xpos_small"],
			  $defaults["LabStatus"]["ypos_small"],
			  $defaults["LabStatus"]["xpos"],
			  $defaults["LabStatus"]["ypos"],
			  $defaults["LabStatus"]["closed"]);
			  
foreach($objlist as $obj)
{
  if(!isset($sortedobjects[$obj->getObjName()]))
    {
      $sortedobjects[$obj->getObjName()]=array();
    }
  array_push($sortedobjects[$obj->getObjName()],$obj);
}



$vortrag->addcontentOBJ($encap_irbeamer);
$vortrag->addcontentOBJ($encap_irteufel);
$vortrag->addcontentOBJ($encap_lightvortragmeta);
$vortrag->addcontentOBJ($encap_lightvortragdetail);
$vortrag->addcontentOBJ($encap_musikvortrag);

$labstatus->addcontentOBJ($encap_labstatus);
$labstatus->addcontentOBJ($encap_muell);

$lounge->addcontentOBJ($encap_lightkueche);
$lounge->addcontentOBJ($encap_lightlounge);
$lounge->addcontentOBJ($encap_musiklounge);
$lounge->addcontentOBJ($encap_sound);


$bastel->addcontentOBJ($encap_musikbastel);

$keller->addcontentOBJ($encap_musikkeller);

$musik->addcontentOBJ($encap_musiklounge2);
$musik->addcontentOBJ($encap_musikvortrag2);
$musik->addcontentOBJ($encap_musikbastel2);
$musik->addcontentOBJ($encap_musikkeller2);


$licht->addcontentOBJ($encap_lightkueche2);
$licht->addcontentOBJ($encap_lightlounge2);
$licht->addcontentOBJ($encap_lightvortragmeta2);
$licht->addcontentOBJ($encap_lightvortragdetail2);

$spielzeug->addcontentOBJ($encap_moodbar);
$spielzeug->addcontentOBJ($encap_lsborg);
$spielzeug->addcontentOBJ($encap_treppenblink);
$spielzeug->addcontentOBJ($encap_speach);

$weblabinterface->addcontentOBJ($encap_stylepannel);
$weblabinterface->addcontentOBJ($encap_stylebg);
?>