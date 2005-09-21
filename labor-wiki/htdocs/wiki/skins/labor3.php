<?php
/**
 * See skin.txt
 *
 * @todo document
 * @package MediaWiki
 * @subpackage Skins
 */

if( !defined( 'MEDIAWIKI' ) )
	die();

/**
 * @todo document
 * @package MediaWiki
 * @subpackage Skins
 */
class SkinLabor3 extends Skin {

	function initPage() {
		# ...
	}

	function getStylesheet() {
		return 'labor3.css';
	}
	function getSkinName() {
		return "labor3";
	}

	function doBeforeContent() {
		global $wgUser, $wgOut, $wgTitle;
    $sep = "<br />\n";

		$s = "\n<div id='content'>\n";
    /*
     *    KOPF 
     */
    $s .= "<div id=\"kopf\">\n";
    // embedde 3 bilder vom pblog als header
    $s .= "<div id=\"kopf-bg\">";
    for($i = 0; $i < 3; $i++) {
      $s .= "<img src=\"http://www.das-labor.org/pblog/index.php?showthumb=latest_few&thumbtag=image&lim=".rand(0,10)."\">";
    }
    $s .= "</div>";
    $s .= "<p class=\"logo\"><a href=\"http://www.das-labor.org\"><img src=\"http://www.das-labor.org/weblog/wp-content/themes/labor/images/logo.gif\"></a></p>\n</div>\n\n";
    
    $s .= "<div id='leveleins'>\n<p><a href='http://www.das-labor.org'>weblog</a> &nbsp; <a href='http://wiki.das-labor.org'>wiki</a> &nbsp;  &nbsp; <a href='http://www.das-labor.org/weblog/index.php/labor/'>&uuml;ber uns</a></p>\n</div>\n\n";

    /*
     *     LEVELZWO
     */
    $s .= "\n<div id='levelzwo'>";
    $s .= "\n<div id='boxa'><h3>artikel</h3>";

    if ( $wgUser->getNewtalk() ) {
    # do not show "You have new messages" text when we are viewing our
    # own talk page

      if( !$wgTitle->equals( $wgUser->getTalkPage() ) ) {
        $tl = $this->makeKnownLinkObj( $wgUser->getTalkPage(), wfMsg('newmessageslink') );
        $s .= $sep . '<strong>'. wfMsg( 'newmessages', $tl ) . '</strong>';
        # disable caching
        $wgOut->setSquidMaxage(0);
        $wgOut->enableClientCache(false);
      }
    }

    $undelete = $this->getUndeleteLink();
    if( !empty( $undelete ) ) {
      $s .= $sep . $undelete;
    }


		$ol = $this->otherLanguages();
		if($ol) $s .= "<br />" . $ol;
		
		$cat = $this->getCategoryLinks();
		if($cat) $s .= "<br />" . $cat;

    if ( $wgOut->isArticleRelated() ) {
      $s .= '<strong>' . $this->editThisPage() . '</strong>';
      if ( $wgUser->isLoggedIn() ) {
        $s .= $sep . $this->watchThisPage();
      }
      $s .= $sep . $this->talkLink()
        . $sep . $this->historyLink()
        . $sep . $this->specialLink( 'recentchanges' ) 
        . $sep . $this->whatLinksHere()
        . $sep . $this->watchPageLinksLink();

      if ($wgUseTrackbacks)
        $s .= $sep . $this->trackbackLink();

      if ( $wgTitle->getNamespace() == NS_USER
          || $wgTitle->getNamespace() == NS_USER_TALK )

      {
        $id=User::idFromName($wgTitle->getText());
        $ip=User::isIP($wgTitle->getText());

        if($id || $ip) { # both anons and non-anons have contri list
          $s .= $sep . $this->userContribsLink();
        }
        if( $this->showEmailUser( $id ) ) {
          $s .= $sep . $this->emailUserLink();
        }
      }
      if ( $wgTitle->getArticleId() ) {
        if($wgUser->isAllowed('delete')) { $s .= $sep . $this->deleteThisPage(); }
        if($wgUser->isAllowed('protect')) { $s .= $sep . $this->protectThisPage(); }
        if($wgUser->isAllowed('move')) { $s .= $sep . $this->moveThisPage(); }
      }
      $s .= $sep . $this->otherLanguages();
    }

    //$s .= $this->printableLink();

    $s .= "\n</div>";

    /*
     *     BOXB
     */
    $s .= "\n<div id='boxb'><h3>benutzer</h3>";

    if ( $wgUser->isLoggedIn() ) {
      $name = $wgUser->getName();
      $tl = $this->makeKnownLinkObj( $wgUser->getTalkPage(),
        wfMsg( 'mytalk' ) );
      if ( $wgUser->getNewtalk() ) {
        $tl .= " *";
      }

      $s .= $this->makeKnownLinkObj( $wgUser->getUserPage(),
        wfMsg( "mypage" ) )
        . $sep . $tl
        . $sep . $this->specialLink( "watchlist" )
        . $sep . $this->makeKnownLinkObj( Title::makeTitle( NS_SPECIAL, "Contributions" ),
          wfMsg( "mycontris" ), "target=" . wfUrlencode($wgUser->getName() ) )
          . $sep . $this->specialLink( "preferences" )
          . $sep . $this->specialLink( "userlogout" );
    } else {
      $s .= $this->specialLink( "userlogin" );
    }



    $s .= "\n</div>";

    /*
     *     BOXC
     */
    $s .= "\n<div id='boxc'><h3>mehr</h3>";
		$s .= $sep . $this->aboutLink();
		$s .= $sep . $this->pageStats();
		$s .= $sep . $this->searchForm();
		$s .= "\n</div>\n";
		$s .= "\n</div>\n";
		$s .= "\n<div id='article'>";

		return $s;
	}

	function doAfterContent() {
		$s = "\n</div>\n";
		$s .= "\n<div id='footer'>";
		$s .= $this->specialPagesList();
		$s .= "\n</div>\n</div>\n";

		return $s;
	}
}

?>
