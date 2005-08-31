-- MySQL dump 9.11
--
-- Host: localhost    Database: laborwiki
-- ------------------------------------------------------
-- Server version	4.0.23_Debian-3ubuntu2-log

--
-- Table structure for table `archive`
--

CREATE TABLE `archive` (
  `ar_namespace` int(11) NOT NULL default '0',
  `ar_title` varchar(255) binary NOT NULL default '',
  `ar_text` mediumblob NOT NULL,
  `ar_comment` tinyblob NOT NULL,
  `ar_user` int(5) unsigned NOT NULL default '0',
  `ar_user_text` varchar(255) binary NOT NULL default '',
  `ar_timestamp` varchar(14) binary NOT NULL default '',
  `ar_minor_edit` tinyint(1) NOT NULL default '0',
  `ar_flags` tinyblob NOT NULL,
  `ar_rev_id` int(8) unsigned default NULL,
  `ar_text_id` int(8) unsigned default NULL,
  KEY `name_title_timestamp` (`ar_namespace`,`ar_title`,`ar_timestamp`)
) TYPE=MyISAM;

--
-- Dumping data for table `archive`
--


--
-- Table structure for table `categorylinks`
--

CREATE TABLE `categorylinks` (
  `cl_from` int(8) unsigned NOT NULL default '0',
  `cl_to` varchar(255) binary NOT NULL default '',
  `cl_sortkey` varchar(86) binary NOT NULL default '',
  `cl_timestamp` timestamp(14) NOT NULL,
  UNIQUE KEY `cl_from` (`cl_from`,`cl_to`),
  KEY `cl_sortkey` (`cl_to`,`cl_sortkey`),
  KEY `cl_timestamp` (`cl_to`,`cl_timestamp`)
) TYPE=MyISAM;

--
-- Dumping data for table `categorylinks`
--


--
-- Table structure for table `hitcounter`
--

CREATE TABLE `hitcounter` (
  `hc_id` int(10) unsigned NOT NULL default '0'
) TYPE=HEAP MAX_ROWS=25000;

--
-- Dumping data for table `hitcounter`
--


--
-- Table structure for table `image`
--

CREATE TABLE `image` (
  `img_name` varchar(255) binary NOT NULL default '',
  `img_size` int(8) unsigned NOT NULL default '0',
  `img_width` int(5) NOT NULL default '0',
  `img_height` int(5) NOT NULL default '0',
  `img_metadata` mediumblob NOT NULL,
  `img_bits` int(3) NOT NULL default '0',
  `img_media_type` enum('UNKNOWN','BITMAP','DRAWING','AUDIO','VIDEO','MULTIMEDIA','OFFICE','TEXT','EXECUTABLE','ARCHIVE') default NULL,
  `img_major_mime` enum('unknown','application','audio','image','text','video','message','model','multipart') NOT NULL default 'unknown',
  `img_minor_mime` varchar(32) NOT NULL default 'unknown',
  `img_description` tinyblob NOT NULL,
  `img_user` int(5) unsigned NOT NULL default '0',
  `img_user_text` varchar(255) binary NOT NULL default '',
  `img_timestamp` varchar(14) binary NOT NULL default '',
  PRIMARY KEY  (`img_name`),
  KEY `img_size` (`img_size`),
  KEY `img_timestamp` (`img_timestamp`)
) TYPE=MyISAM;

--
-- Dumping data for table `image`
--


--
-- Table structure for table `imagelinks`
--

CREATE TABLE `imagelinks` (
  `il_from` int(8) unsigned NOT NULL default '0',
  `il_to` varchar(255) binary NOT NULL default '',
  UNIQUE KEY `il_from` (`il_from`,`il_to`),
  KEY `il_to` (`il_to`)
) TYPE=MyISAM;

--
-- Dumping data for table `imagelinks`
--


--
-- Table structure for table `interwiki`
--

CREATE TABLE `interwiki` (
  `iw_prefix` char(32) NOT NULL default '',
  `iw_url` char(127) NOT NULL default '',
  `iw_local` tinyint(1) NOT NULL default '0',
  `iw_trans` tinyint(1) NOT NULL default '0',
  UNIQUE KEY `iw_prefix` (`iw_prefix`)
) TYPE=MyISAM;

--
-- Dumping data for table `interwiki`
--

INSERT INTO `interwiki` VALUES ('abbenormal','http://www.ourpla.net/cgi-bin/pikie.cgi?$1',0,0);
INSERT INTO `interwiki` VALUES ('acadwiki','http://xarch.tu-graz.ac.at/autocad/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('acronym','http://www.acronymfinder.com/af-query.asp?String=exact&Acronym=$1',0,0);
INSERT INTO `interwiki` VALUES ('advogato','http://www.advogato.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('aiwiki','http://www.ifi.unizh.ch/ailab/aiwiki/aiw.cgi?$1',0,0);
INSERT INTO `interwiki` VALUES ('alife','http://news.alife.org/wiki/index.php?$1',0,0);
INSERT INTO `interwiki` VALUES ('annotation','http://bayle.stanford.edu/crit/nph-med.cgi/$1',0,0);
INSERT INTO `interwiki` VALUES ('annotationwiki','http://www.seedwiki.com/page.cfm?wikiid=368&doc=$1',0,0);
INSERT INTO `interwiki` VALUES ('arxiv','http://www.arxiv.org/abs/$1',0,0);
INSERT INTO `interwiki` VALUES ('aspienetwiki','http://aspie.mela.de/Wiki/index.php?title=$1',0,0);
INSERT INTO `interwiki` VALUES ('bemi','http://bemi.free.fr/vikio/index.php?$1',0,0);
INSERT INTO `interwiki` VALUES ('benefitswiki','http://www.benefitslink.com/cgi-bin/wiki.cgi?$1',0,0);
INSERT INTO `interwiki` VALUES ('brasilwiki','http://rio.ifi.unizh.ch/brasilienwiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('bridgeswiki','http://c2.com/w2/bridges/$1',0,0);
INSERT INTO `interwiki` VALUES ('c2find','http://c2.com/cgi/wiki?FindPage&value=$1',0,0);
INSERT INTO `interwiki` VALUES ('cache','http://www.google.com/search?q=cache:$1',0,0);
INSERT INTO `interwiki` VALUES ('ciscavate','http://ciscavate.org/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('cliki','http://ww.telent.net/cliki/$1',0,0);
INSERT INTO `interwiki` VALUES ('cmwiki','http://www.ourpla.net/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('codersbase','http://www.codersbase.com/$1',0,0);
INSERT INTO `interwiki` VALUES ('commons','http://commons.wikimedia.org/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('consciousness','http://teadvus.inspiral.org/',0,0);
INSERT INTO `interwiki` VALUES ('corpknowpedia','http://corpknowpedia.org/wiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('creationmatters','http://www.ourpla.net/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('dejanews','http://www.deja.com/=dnc/getdoc.xp?AN=$1',0,0);
INSERT INTO `interwiki` VALUES ('demokraatia','http://wiki.demokraatia.ee/',0,0);
INSERT INTO `interwiki` VALUES ('dictionary','http://www.dict.org/bin/Dict?Database=*&Form=Dict1&Strategy=*&Query=$1',0,0);
INSERT INTO `interwiki` VALUES ('disinfopedia','http://www.disinfopedia.org/wiki.phtml?title=$1',0,0);
INSERT INTO `interwiki` VALUES ('diveintoosx','http://diveintoosx.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('docbook','http://docbook.org/wiki/moin.cgi/$1',0,0);
INSERT INTO `interwiki` VALUES ('dolphinwiki','http://www.object-arts.com/wiki/html/Dolphin/$1',0,0);
INSERT INTO `interwiki` VALUES ('drumcorpswiki','http://www.drumcorpswiki.com/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('dwjwiki','http://www.suberic.net/cgi-bin/dwj/wiki.cgi?$1',0,0);
INSERT INTO `interwiki` VALUES ('eĉei','http://www.ikso.net/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('echei','http://www.ikso.net/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('ecxei','http://www.ikso.net/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('efnetceewiki','http://purl.net/wiki/c/$1',0,0);
INSERT INTO `interwiki` VALUES ('efnetcppwiki','http://purl.net/wiki/cpp/$1',0,0);
INSERT INTO `interwiki` VALUES ('efnetpythonwiki','http://purl.net/wiki/python/$1',0,0);
INSERT INTO `interwiki` VALUES ('efnetxmlwiki','http://purl.net/wiki/xml/$1',0,0);
INSERT INTO `interwiki` VALUES ('eljwiki','http://elj.sourceforge.net/phpwiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('emacswiki','http://www.emacswiki.org/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('elibre','http://enciclopedia.us.es/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('eokulturcentro','http://esperanto.toulouse.free.fr/wakka.php?wiki=$1',0,0);
INSERT INTO `interwiki` VALUES ('evowiki','http://www.evowiki.org/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('finalempire','http://final-empire.sourceforge.net/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('firstwiki','http://firstwiki.org/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('foldoc','http://www.foldoc.org/foldoc/foldoc.cgi?$1',0,0);
INSERT INTO `interwiki` VALUES ('foxwiki','http://fox.wikis.com/wc.dll?Wiki~$1',0,0);
INSERT INTO `interwiki` VALUES ('fr.be','http://fr.wikinations.be/$1',0,0);
INSERT INTO `interwiki` VALUES ('fr.ca','http://fr.ca.wikinations.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('fr.fr','http://fr.fr.wikinations.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('fr.org','http://fr.wikinations.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('freebsdman','http://www.FreeBSD.org/cgi/man.cgi?apropos=1&query=$1',0,0);
INSERT INTO `interwiki` VALUES ('gamewiki','http://gamewiki.org/wiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('gej','http://www.esperanto.de/cgi-bin/aktivikio/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('gentoo-wiki','http://gentoo-wiki.com/$1',0,0);
INSERT INTO `interwiki` VALUES ('globalvoices','http://cyber.law.harvard.edu/dyn/globalvoices/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('gmailwiki','http://www.gmailwiki.com/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('google','http://www.google.com/search?q=$1',0,0);
INSERT INTO `interwiki` VALUES ('googlegroups','http://groups.google.com/groups?q=$1',0,0);
INSERT INTO `interwiki` VALUES ('gotamac','http://www.got-a-mac.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('greencheese','http://www.greencheese.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('hammondwiki','http://www.dairiki.org/HammondWiki/index.php3?$1',0,0);
INSERT INTO `interwiki` VALUES ('haribeau','http://wiki.haribeau.de/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('hewikisource','http://he.wikisource.org/wiki/$1',1,0);
INSERT INTO `interwiki` VALUES ('herzkinderwiki','http://www.herzkinderinfo.de/Mediawiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('hrwiki','http://www.hrwiki.org/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('iawiki','http://www.IAwiki.net/$1',0,0);
INSERT INTO `interwiki` VALUES ('imdb','http://us.imdb.com/Title?$1',0,0);
INSERT INTO `interwiki` VALUES ('infosecpedia','http://www.infosecpedia.org/pedia/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('jargonfile','http://sunir.org/apps/meta.pl?wiki=JargonFile&redirect=$1',0,0);
INSERT INTO `interwiki` VALUES ('jefo','http://www.esperanto-jeunes.org/vikio/index.php?$1',0,0);
INSERT INTO `interwiki` VALUES ('jiniwiki','http://www.cdegroot.com/cgi-bin/jini?$1',0,0);
INSERT INTO `interwiki` VALUES ('jspwiki','http://www.ecyrd.com/JSPWiki/Wiki.jsp?page=$1',0,0);
INSERT INTO `interwiki` VALUES ('kerimwiki','http://wiki.oxus.net/$1',0,0);
INSERT INTO `interwiki` VALUES ('kmwiki','http://www.voght.com/cgi-bin/pywiki?$1',0,0);
INSERT INTO `interwiki` VALUES ('knowhow','http://www2.iro.umontreal.ca/~paquetse/cgi-bin/wiki.cgi?$1',0,0);
INSERT INTO `interwiki` VALUES ('lanifexwiki','http://opt.lanifex.com/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('lasvegaswiki','http://wiki.gmnow.com/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('linuxwiki','http://www.linuxwiki.de/$1',0,0);
INSERT INTO `interwiki` VALUES ('lojban','http://www.lojban.org/tiki/tiki-index.php?page=$1',0,0);
INSERT INTO `interwiki` VALUES ('lqwiki','http://wiki.linuxquestions.org/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('lugkr','http://lug-kr.sourceforge.net/cgi-bin/lugwiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('lutherwiki','http://www.lutheranarchives.com/mw/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('mathsongswiki','http://SeedWiki.com/page.cfm?wikiid=237&doc=$1',0,0);
INSERT INTO `interwiki` VALUES ('mbtest','http://www.usemod.com/cgi-bin/mbtest.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('meatball','http://www.usemod.com/cgi-bin/mb.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('mediazilla','http://bugzilla.wikipedia.org/$1',1,0);
INSERT INTO `interwiki` VALUES ('memoryalpha','http://www.memory-alpha.org/en/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('metaweb','http://www.metaweb.com/wiki/wiki.phtml?title=$1',0,0);
INSERT INTO `interwiki` VALUES ('metawiki','http://sunir.org/apps/meta.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('metawikipedia','http://meta.wikimedia.org/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('moinmoin','http://purl.net/wiki/moin/$1',0,0);
INSERT INTO `interwiki` VALUES ('mozillawiki','http://wiki.mozilla.org/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('muweb','http://www.dunstable.com/scripts/MuWebWeb?$1',0,0);
INSERT INTO `interwiki` VALUES ('netvillage','http://www.netbros.com/?$1',0,0);
INSERT INTO `interwiki` VALUES ('oeis','http://www.research.att.com/cgi-bin/access.cgi/as/njas/sequences/eisA.cgi?Anum=$1',0,0);
INSERT INTO `interwiki` VALUES ('openfacts','http://openfacts.berlios.de/index.phtml?title=$1',0,0);
INSERT INTO `interwiki` VALUES ('openwiki','http://openwiki.com/?$1',0,0);
INSERT INTO `interwiki` VALUES ('opera7wiki','http://nontroppo.org/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('orgpatterns','http://www.bell-labs.com/cgi-user/OrgPatterns/OrgPatterns?$1',0,0);
INSERT INTO `interwiki` VALUES ('osi reference model','http://wiki.tigma.ee/',0,0);
INSERT INTO `interwiki` VALUES ('pangalacticorg','http://www.pangalactic.org/Wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('personaltelco','http://www.personaltelco.net/index.cgi/$1',0,0);
INSERT INTO `interwiki` VALUES ('patwiki','http://gauss.ffii.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('phpwiki','http://phpwiki.sourceforge.net/phpwiki/index.php?$1',0,0);
INSERT INTO `interwiki` VALUES ('pikie','http://pikie.darktech.org/cgi/pikie?$1',0,0);
INSERT INTO `interwiki` VALUES ('pmeg','http://www.bertilow.com/pmeg/$1.php',0,0);
INSERT INTO `interwiki` VALUES ('ppr','http://c2.com/cgi/wiki?$1',0,0);
INSERT INTO `interwiki` VALUES ('purlnet','http://purl.oclc.org/NET/$1',0,0);
INSERT INTO `interwiki` VALUES ('pythoninfo','http://www.python.org/cgi-bin/moinmoin/$1',0,0);
INSERT INTO `interwiki` VALUES ('pythonwiki','http://www.pythonwiki.de/$1',0,0);
INSERT INTO `interwiki` VALUES ('pywiki','http://www.voght.com/cgi-bin/pywiki?$1',0,0);
INSERT INTO `interwiki` VALUES ('raec','http://www.raec.clacso.edu.ar:8080/raec/Members/raecpedia/$1',0,0);
INSERT INTO `interwiki` VALUES ('revo','http://purl.org/NET/voko/revo/art/$1.html',0,0);
INSERT INTO `interwiki` VALUES ('rfc','http://www.rfc-editor.org/rfc/rfc$1.txt',0,0);
INSERT INTO `interwiki` VALUES ('s23wiki','http://is-root.de/wiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('scoutpedia','http://www.scoutpedia.info/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('seapig','http://www.seapig.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('seattlewiki','http://seattlewiki.org/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('seattlewireless','http://seattlewireless.net/?$1',0,0);
INSERT INTO `interwiki` VALUES ('seeds','http://www.IslandSeeds.org/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('senseislibrary','http://senseis.xmp.net/?$1',0,0);
INSERT INTO `interwiki` VALUES ('shakti','http://cgi.algonet.se/htbin/cgiwrap/pgd/ShaktiWiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('slashdot','http://slashdot.org/article.pl?sid=$1',0,0);
INSERT INTO `interwiki` VALUES ('smikipedia','http://www.smikipedia.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('sockwiki','http://wiki.socklabs.com/$1',0,0);
INSERT INTO `interwiki` VALUES ('sourceforge','http://sourceforge.net/$1',0,0);
INSERT INTO `interwiki` VALUES ('squeak','http://minnow.cc.gatech.edu/squeak/$1',0,0);
INSERT INTO `interwiki` VALUES ('strikiwiki','http://ch.twi.tudelft.nl/~mostert/striki/teststriki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('susning','http://www.susning.nu/$1',0,0);
INSERT INTO `interwiki` VALUES ('svgwiki','http://www.protocol7.com/svg-wiki/default.asp?$1',0,0);
INSERT INTO `interwiki` VALUES ('tavi','http://tavi.sourceforge.net/$1',0,0);
INSERT INTO `interwiki` VALUES ('tejo','http://www.tejo.org/vikio/$1',0,0);
INSERT INTO `interwiki` VALUES ('terrorwiki','http://www.liberalsagainstterrorism.com/wiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('tmbw','http://www.tmbw.net/wiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('tmnet','http://www.technomanifestos.net/?$1',0,0);
INSERT INTO `interwiki` VALUES ('tmwiki','http://www.EasyTopicMaps.com/?page=$1',0,0);
INSERT INTO `interwiki` VALUES ('turismo','http://www.tejo.org/turismo/$1',0,0);
INSERT INTO `interwiki` VALUES ('theopedia','http://www.theopedia.com/$1',0,0);
INSERT INTO `interwiki` VALUES ('twiki','http://twiki.org/cgi-bin/view/$1',0,0);
INSERT INTO `interwiki` VALUES ('twistedwiki','http://purl.net/wiki/twisted/$1',0,0);
INSERT INTO `interwiki` VALUES ('uea','http://www.tejo.org/uea/$1',0,0);
INSERT INTO `interwiki` VALUES ('unreal','http://wiki.beyondunreal.com/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('ursine','http://ursine.ca/$1',0,0);
INSERT INTO `interwiki` VALUES ('usej','http://www.tejo.org/usej/$1',0,0);
INSERT INTO `interwiki` VALUES ('usemod','http://www.usemod.com/cgi-bin/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('visualworks','http://wiki.cs.uiuc.edu/VisualWorks/$1',0,0);
INSERT INTO `interwiki` VALUES ('warpedview','http://www.warpedview.com/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('webdevwikinl','http://www.promo-it.nl/WebDevWiki/index.php?page=$1',0,0);
INSERT INTO `interwiki` VALUES ('webisodes','http://www.webisodes.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('webseitzwiki','http://webseitz.fluxent.com/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('why','http://clublet.com/c/c/why?$1',0,0);
INSERT INTO `interwiki` VALUES ('wiki','http://c2.com/cgi/wiki?$1',0,0);
INSERT INTO `interwiki` VALUES ('wikia','http://www.wikia.com/wiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('wikibooks','http://en.wikibooks.org/wiki/$1',1,0);
INSERT INTO `interwiki` VALUES ('wikicities','http://www.wikicities.com/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('wikif1','http://www.wikif1.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('wikinfo','http://www.wikinfo.org/wiki.php?title=$1',0,0);
INSERT INTO `interwiki` VALUES ('wikimedia','http://wikimediafoundation.org/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('wikiquote','http://en.wikiquote.org/wiki/$1',1,0);
INSERT INTO `interwiki` VALUES ('wikinews','http://en.wikinews.org/wiki/$1',0,0);
INSERT INTO `interwiki` VALUES ('wikisource','http://sources.wikipedia.org/wiki/$1',1,0);
INSERT INTO `interwiki` VALUES ('wikispecies','http://species.wikipedia.org/wiki/$1',1,0);
INSERT INTO `interwiki` VALUES ('wikitravel','http://wikitravel.org/en/$1',0,0);
INSERT INTO `interwiki` VALUES ('wikiworld','http://WikiWorld.com/wiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('wiktionary','http://en.wiktionary.org/wiki/$1',1,0);
INSERT INTO `interwiki` VALUES ('wlug','http://www.wlug.org.nz/$1',0,0);
INSERT INTO `interwiki` VALUES ('wlwiki','http://winslowslair.supremepixels.net/wiki/index.php/$1',0,0);
INSERT INTO `interwiki` VALUES ('ypsieyeball','http://sknkwrks.dyndns.org:1957/writewiki/wiki.pl?$1',0,0);
INSERT INTO `interwiki` VALUES ('zwiki','http://www.zwiki.org/$1',0,0);
INSERT INTO `interwiki` VALUES ('zzz wiki','http://wiki.zzz.ee/',0,0);
INSERT INTO `interwiki` VALUES ('wikt','http://en.wiktionary.org/wiki/$1',1,0);

--
-- Table structure for table `ipblocks`
--

CREATE TABLE `ipblocks` (
  `ipb_id` int(8) NOT NULL auto_increment,
  `ipb_address` varchar(40) binary NOT NULL default '',
  `ipb_user` int(8) unsigned NOT NULL default '0',
  `ipb_by` int(8) unsigned NOT NULL default '0',
  `ipb_reason` tinyblob NOT NULL,
  `ipb_timestamp` varchar(14) binary NOT NULL default '',
  `ipb_auto` tinyint(1) NOT NULL default '0',
  `ipb_expiry` varchar(14) binary NOT NULL default '',
  PRIMARY KEY  (`ipb_id`),
  KEY `ipb_address` (`ipb_address`),
  KEY `ipb_user` (`ipb_user`)
) TYPE=MyISAM;

--
-- Dumping data for table `ipblocks`
--


--
-- Table structure for table `logging`
--

CREATE TABLE `logging` (
  `log_type` varchar(10) NOT NULL default '',
  `log_action` varchar(10) NOT NULL default '',
  `log_timestamp` varchar(14) NOT NULL default '19700101000000',
  `log_user` int(10) unsigned NOT NULL default '0',
  `log_namespace` int(11) NOT NULL default '0',
  `log_title` varchar(255) binary NOT NULL default '',
  `log_comment` varchar(255) NOT NULL default '',
  `log_params` blob NOT NULL,
  KEY `type_time` (`log_type`,`log_timestamp`),
  KEY `user_time` (`log_user`,`log_timestamp`),
  KEY `page_time` (`log_namespace`,`log_title`,`log_timestamp`)
) TYPE=MyISAM;

--
-- Dumping data for table `logging`
--


--
-- Table structure for table `math`
--

CREATE TABLE `math` (
  `math_inputhash` varchar(16) NOT NULL default '',
  `math_outputhash` varchar(16) NOT NULL default '',
  `math_html_conservativeness` tinyint(1) NOT NULL default '0',
  `math_html` text,
  `math_mathml` text,
  UNIQUE KEY `math_inputhash` (`math_inputhash`)
) TYPE=MyISAM;

--
-- Dumping data for table `math`
--


--
-- Table structure for table `objectcache`
--

CREATE TABLE `objectcache` (
  `keyname` varchar(255) binary NOT NULL default '',
  `value` mediumblob,
  `exptime` datetime default NULL,
  UNIQUE KEY `keyname` (`keyname`),
  KEY `exptime` (`exptime`)
) TYPE=MyISAM;

--
-- Dumping data for table `objectcache`
--

INSERT INTO `objectcache` VALUES ('laborwiki:pcache:idhash:1-0!1!0!0!!de!2','�SKn1����\"+{f��6��6@4Av�e@�83�e�@��I�����b��@�*�B��#�Y����yrm]BN䃗c)vk���˷��ny�	�魆U[�=:�}t��\\՚ڑ.��4F���<떜���(Ξ#4���hB�d��(�pϐ)����Y���d�R���Z�A�ġ�E�ogt0��Xp�rd¶Lٴ{��l�qG6`Э������Z�5(r�l�ܝ\r�4�OeC���h(�� ����h�o?F���K�g^0_�<�hЪM,��\0��>iS�A��M���}nm���m�=I��b~>�y0�~\'�s��A��\"��������u����U���� ���O_Q̌5�w��|�Oϋ\"��&0,{i�^�+�uĚ���z1C�ˇǗ�T���ul�b-O���ƨk�u)f�\'������\\�T�O�;�g_�ynH2����0\'�u���ϰC�','2005-09-01 10:53:27');
INSERT INTO `objectcache` VALUES ('laborwiki:messages','��rI�6�<E�Z��&!�3%�Q�T�-J��R���\0�@*\" �,��,�7�Y�Ym�f��\\\r�d�d�w.������jʺSD�����s?�Q���������VoV|NFuѿ�]u��?�u��/�Q��p\Z��G���\ZN�A��Jo7hsZ&���ܒ��@;����B2��i��֫\"/Eq�V�ѐw��6z\\DyQG�&�ZV�\'�0����ۻ��(�,����F�co��/��9M���,Zd�{�ȫ(K�8�[���/�f]�yE�ώh�<���w=���<����m̈Qdu:��8E�p�TUt�\\T�PG��<9��~�ŭ���>}\\�כ�*)��$Y��5��2�X_�m����Pq�oӄnj�n��vؠ��/�<�(Y+����N�IWA���:�����2\r���uM��pQU��Q<(5YE㲘�d�\n��G���(�q^&�Lrںv�2���ov��i�Vu�Q������*��\"O��F`���8��ncK���2]��u�R;�]DRk�`0\n�g�$��mj��b�8�|X,J�_���F����{F��/�)�7]�nk��kG��tv��I�H���U�P���\"6,��DQ\0�v�.f�D>h؍~���u�sJ-� j\Zn�б&Q�#���U	���`�J�����g3̐6��e�=a���w�t=�)6{^5\r����-�\'�k!]�SKJ��)��CLp�8�\0�oHЁ���>:�\"�.h6JQ.-f�s䒕�����&X�����aS���;zPt��1�s_����g@�}�!�[����qi����pM�v�6�5����*���^���	�H/��Y�ߺU�/N��`���>�z.���i��e[�/b����]Q �T��4��:�Q�Iɛ7Ns��|R�����tM�$�Wa�h4��g�� �!t�G6�m����y��]�D����n���ʆ�i^v��H����ھ/��b��Ԁ�|s�d�+��Y<�{C{H�T�9�~E�)!df-�a����������Aq����qdhqC�_cQN׬��x{��[i<�ᶌ�d�YV�����I�b%EV5�\'h�\n��\rM���˻���@�u\0\Z�{�Z�&t�2���wj8$�l��g;�j��K�tƻI�F�	T�$8�j��8�-o���֙k]9�\0{�U=�{��r�3,�Wm@��i�ƺnl++_�X״�p$����\rCNY��BH�٭�$w��8_��q89E}��ǧw�ڸ�\r�������Nf�GU��*#4���6�p�ڭW!�FKTX=�!�7YA�4aJ+Y`���E���?	��U�\'�;���l�W:���N��{���Փ��=l\\��؁v�knSv�L�`�\'��A�h0#����[��ߝ��(��t�����4/���Z%���*J�j���D�&Z*��k�2y��cc=��*>���H&\"	�s<��u�8=��TbN��Y�5��:I�)v�n=HS��]��L�>�	�ԧ���Hj:�=8�:�u�����G��y\"�|Q_&�#����կyN��ԗ5��)Q��	\'=��Ӓ�%�$��-Ð8A������r\"�@s /1�0@�~\"��aR�1�D��!�y���b���~�ԮG����6u���븬ӳ$�n����-�\\Dϧ%�}�x\"���Q_����1d�s2��k���Ϳ���|X)��\r���-�W��)�Ku&�H�y���\'4%Pgt���G�][��<-An�7?�!M��%�jd8�3�-����8��{w\"��LRZMto��C��@\0�C&�`�b�:�\'A�K�8/�,!��,#�K��x1�n�N����@�\'��N<����h���h�,A_�\rg���~�E�r��i�E\".��@�2�z!���\'T@�(�C�5�o{�(Uinq��4�!=��N�c������<���Q�}�W%��邹�;A紨�@��G�}T�t�F�b�U�\0� dʆ�[��\r�P��AVψ��`a=H���1�5�˲8\'�;�Z8�HP,K�X�P_��v�&��H*��\Z�@��*X4�1x<?���*�F��M����}���ۏf�Q2����X~�ڻ�ژ<ㅣ[���~�FQ+���?D�}Z��5��J+���U%��c�O�W��^۸J\\����;���N�^�8��Po9[�э]�6�[|��w8]��E�j�����vW�mV�8�)1�.��U:��կ��S����\\\nNۚWe��u�k�y�r��5�ϛ���Z�nݛ�^��`�)�O��#�17��{�r	�q�A�:>֋%#�T���ڭr{[�ʂb�\r:�@A�p�j1�ɫK��x>%��|!��(�5�����	]���bH�Ɉ�w��0�-��7+ʱKt��q����k��z�Ac�n�]SzC��\Z�p˄��Q�>�qo\\�g-���O���q�>�7�5�X�@�.�Px�f�I�\r�½�耚����ٖx�Fr�}��Ȋ�߆gy<�5_ zZo��ě��M��v�����O�u�-x�Q�}�t��O.�^\ZBձ\\�/r�q/sT��}��C���&�����t�5��9�ߞ�����7Lq�9E��0%<�\r]���A4*�}pu���H�Z�_\0H�\"7Ɉ�\n��PDe��w��F��0u�I�������˴oa�{�a��-��FR ��h9�m4:����L��Mhמ��y���v��n��S���2�\r&��iލ��Ъ\"CKm��y�\n3��\'�h&�m.��Hs&�hrFiQ�J���k!@.�_~zjD�}��c�%�cz�\"ۆz ��&�0PUJ�S#&/�ЃT���+�T�7i8%�R�I}|��-f�\rgW��㚈��G�6��� J�1KR��ښ�d<s�o̭�F���)Kl՛�Ϳv��4k���@����!�mW�|!`�Y<��sid8?GW�c����kg��ClCt~��4K��05?��N<u#�\Z�ۜ&��4���U��[�k�Q��\'TR2!� �Ħy\0�U1���5�8]\"�	���G�VW��a<�����PS��$D�����]��>P<�,�����_�#��,�ѓQ�.��!��L%b��F��N��%&*I�;z��|����\'�\"�;p|S���y�l��o��ī��a���M�Bf\'��/8����0��\r��EU *p�Q2���a\'�\0�T��m�ҹ�L���� !�EW�8��W�S���@���~*բ�ؿq� ��qU;�m�V$N�Yy�aC�@��q9\0�ͻ��E��K�\0>��Y��3�\0��>)����@d4���<I��T�E�h��ǋ�O��%hȂ�uyA��\\��WFnM�$�4S���:{�1H�g�]nD�݇�έ��T����:�M�oٝ׵�^.}t���Q��`gOq��Q9����֍���qO�����}�O�~ˀz��m�$Wt,MOe\Z\\�Y}@=M����+I4-Y�܈p�6����-��N�e�G~�Ova�p�zG�?$�A��Bֿ��z�񯌰����>�!`�/\"nO�A0�~]��i�x������񙓋M�IB�p$������9Pȩ�3��$f�T(?t|?ze0&���/�������^j\\}7O}�C����!�֚��������p�--�GFܨ(���C��6	�]�nʞ�$��9W`����6k\\�6j1�88�^�]eR6�>ȁ��*�9Ă�Y,G�~�h��)� ���P��J�`j�#lhz3\"5�8��o*a�G,N�����\\�*.�a��G��kB@O��W��O�\Zj\\yCc�ll���F��\Z�ԩu�ۮJU���ss����f�@�u�����?x��h��PSv�v��CZfy��$�K:�\'|\'�W��X;�I�w=�6_��B�3h�@a�)Q�`z\"�fZ����$�+�x��nE�:��u@�>��n!1����2�oB��@��I\0�`�iN!�`o�3������e�Yq8�G��N�p��4��H��m:�J���5���LlM�Po+�����v�vVKvp;^BG�q�yR3��l��|>��%^j��x�)x��<#b|�cPL�A܄�Y�ԩ��<��^�q������.:R�\r��	��+���q>H�Zk8��e,4Mn�����qTf��dN/��w�m��p���)���s-�ܱs�K����r R�I�y�]<\\���ҧ�<f����.$�d�,��tR����xj_ZU��j�m���=z���Xd=�8$��<�Ū��9)Q�+6��PKM\n˅Uz��W.}V^�JW۠���c\r��Dyf���E1g\Z�H���1\0�Z���g�\"ɲ���Q��~e�ܨ,/j1p�Ymz�+ ]Ў		V��+�1}����=%Ѽ������	��Bܶ��	fSC�c�!*��B\"R�ȗ\"b�b���:*\\��F���Ԥ�[��©�*�3����fC³-L$WV�C�:t��~4�|)j_\n4/������	�]�:�ܴe�C�=��Vy�a���Ζ�aCQ�%L�Z�`�~ou_=/\Z��gJ\"Y+}2u{W!�+�F�^Ǉ\ZF��e�מk�T~����(�G��7���7�����C�]��%�冸WB������&��?��t�������0�;�#�)\0�p�9�\Zj����QT�������|U\\�Ys�/�fdL���:�Zkk�|���N�r��vm��\Z>ݍ��K.m�����\Z���L���l�Yj�U��E��N�i�b��>��Y-;q��m��x��ȩvKMXX8�x�܁�8-g�H���0n���\Z+_2�\0�Z�Z8�pGL\0X�8\"V;�P��ݱ���0F�Z� (�_�=q~^�p��|@�#��A%��92d0gx<���f�`�^�0�I0�rӞB���$���x\"E��ӬA:+�<s����f�8ho�l��_�*u+�h�a�\r)�֑pW��b�o0��;��lƠQc5,+�;�P�{>��h��g\Z�TbI&;�aڤ��D����_�n?�Ta���<cr��\'/x���_0���V�t�#Û��\Z0uq���\n^��	_������8W\0a�53f��IE����Q�s{��q\"�o�5�����W>oƝh�*�갘0؄����/�1��,��Ń�{�l�1�T�$�z�L�PŚG���,�qN]�f�ݧ�gb�Es�!-M�tZ�3�p!�(^��_\0�]PC�$�8��wo\\8v���O҂-V�-r�U�w�&��V*� ����}`ͪK�m��1�!�ho�\'� �źrX����-�1�h�v�3�0Q6!�xDuK�4�,���Qm:/ٮ6��*սvyb8w�0 �,L�޸<&�!�^\rՂݑĨ�q}��hE�Nc����U�䏘���1�K1Z+�L�z���z�o�Y�Ò�\0���F���Ǎ�%���� �\\�i!�����.E��ڿ��`.�qҘP�p�FNINԁ��H���i^S<���\0;���H4��/Ԍ*Pvvj>��/�d�n(`i���d��H�N�D0Kd�A����A&\r/s2i�=����y*�-�`��;^���\0S\"��|�t=،v����g|�n.|T֟�~2ݘǴ��K��p��T�\'��`VY��*C�W�^q���L�ceE�v���jZ�OSzd`{j�J�ߚp�1i\"D&�s����1��)�۽�,��?\'q6�]���Vm���t�n�_`<��E�N�rn���s\\�C�Kz��d�*־��F�OE�:�ž\\4:��6�v��݆�f{A���Uw\0��K~���b��k��[{p9�i\nĒ�C2ٗ��(C�d_�ɘ\r�� �q/u�=.J�Z	�Bݺ\"Я���XQ1�(���pY�G٭j�M�Tl�ҹ7 z�O+�r\0S���ذ�aL�G$F���,��bJzr��޻��E�ft����,UH����u�c�Ђ�Q?�������bM����x��\"�@it_C�>�|��S��n5O�t����hq�	�f|~�TmD,h	�HF��c�556�kO�Ӕ�8����q,����b^(��Mwgi/��W�ֳ�4�$z\\�-ئ�/`F0�_ނ��d�\'gW���\r�ϗZ�]��� 2&�!�>AC�����h��!��A���[�{w�KWT�Z_�&���N���b��K�g���p�:�����`B󉙙��i��{��\"*j��]OEYu;��l�0*�5��:��M�~�YR�Sel͚k31ƃ�\"�4]юu��6ΪH��B��P�,�z^�N�Q��s���!�Yŝ�5A�*��RRy\n��G�C��QD+Tw����^�y�������ћ�������=y}x��E�����߼�����\'\'�߼���������=5v�\r8�vEk�{k��Љ͖��1:��\n8���N���ܧ�8�	��̂&�\n$���a݆����aX([�ܼ�S?�1��D�q�d�ɂ�_��Ӹ<b//q�CrFzX�{���M`,�d7�Й��u���5!L_�G�QsQfn�\r�JG�W59e�:�[e1Nn�j��l���\r����*0|@�^ǭ?�,3pv�-80eiU���=6W|=�m$���l}ܧ�n��B�_ƫǊb��L�#z^!�:\"�\0���.�?�W�lRM����wL~�TY��W�s�[�Ϋ���x�\r\"���Ro�(���u���5����Vi����*��[��C�:�v����/8�{��]��1\\�7�\"��.U�Oߒ�:���K�s�Y�����Е	�~�yn��ڥ_��W�\r`���>�]�r���9�GvG�a�N�C\rXm\\���j�ptRNK�7I��\r��4�a��}5�s��T���U%w�`�b����_���r�^�߿�X�Gؔ/%�v�~�-\rў�x�T��2�����K�~�E�%ނȕ��gƞ�4�sW��j��Z��\0�:)��A]r��ɞ�V�H]E���G�L����� Q��>t�Lϕ������+�:�g�3<�0[v�r�L�1����&\nG/C8\n�ͭ.�XU�m�{�vTo5 ӭ����q��,��V�-�uHF5w���>r�#�W�V���5�(։�ܰ1�%f)���	��YA�}��[�\Z���w�@�-2b���=��ОT3a�\0:m`1OJͲ��~,�Il�Z��&��`g&D�3��\n�a�ѭ\'y}��U��<!�x�#�S����\\�`�.�@y\'��3p��u��ӂgF�<�jӉ�]�S�kx���U�%�cwگ��Vtyζ́�莻�i��d;�E�m�&%�Y�eW�@�#��64[9���b�v�4ubȭ����i�>��^$sµ��/Ya�	��HEŞ-��SC�^Y\',��`�$൝�P�vGpr��ٮ� �Dg�0 P�@M�U���N4�_�T�*�,i��`Q�ʜU=D��fq:��VQ�[Np����d��=����\Z:��5�&1UIӐ/b1�\'�*K�VG_��<8��#��%����-�ǎ~(*b-�ue1.�����[�iY���~Cgs���s�3�mS�]̞�5�3��\Z���9�w����JV<u�mڿ�|�Bq�F��嬩a:\'��z��\r���OC?j�˛44���\'΍E�bx\Z��O�H�i���\0¬�5��]��W.4�����`�)Љ\n���g�zA{~!�xWq�s�)fbV�A(ݤm�Ar&��{��|?�K=�w�Ť�g���!��C�� �-zuX!N�z~t����.�\\8�D���]���}����\'2�):��q���L��qHnQ�����k�^�j�\'<�3��̙&��Sf�Ҁҗ>�1!\'�6�\r@�/��>2��\r�}yY�\0���j�d�~���C�����t��7��|p۽ߜ�{\Z.C�֐�?B����h_�A�����W/�����Ѻ�޺.���)�\'�������޿~v�\Zrי�*�#�}�z�weB�\rC�,���M�j��0���r�nS������o��U\\_�	A!��_��z6!��,�Zj\nu�d4	C0X��M`�Rv�f��x,���\n�&�i�[�	P��	\r�����i���L��VW7�*����Y,$�C�b(��!A��@�=�>�2+��T�&�-Ç���DU\0��9=5��n�U�cZ���qkP7kZEsj�}��a.�n��>i\"=�i{{o�)����=���U\n�h��K*�u�V��o+�Ek�+6:lu�\rdr��@���J�L1ܡh�-!\\@��(A�i���$�\\�/�9��8�5�*s�u��83w	zq�h�\ZZUodN��*f��0î���O�9�@����Fp����a	���4t��g��P�L%c���.Dw�CNgbD�iM�\Z$�2~��1qICnJc�̢�Q&�*t�N��.\\OckQӛ�5 ~u�w�a�Y5t���q� ;����܀T�a��2��������^|W��hZ��d[L��p)�F�2#g�\"+u�.��M�9�9;�B؍�[,����\0|�9d���x|\rH��6OXb���a\'���n��p�����_?{��-���N}7�|�����ٓw�߽|z�����߽yˏ��||��It���ǯ�=y���O���\r�{���:\n�?}��e�s����k<bUik��{�*\"��f\'�_�:~�g�����7o1�����i���m0��\'������M�����;�-	�@��k;���ao�9c(t���O;0�]9�� L�����-�2$�*�	L���<�@K�If����t��x��eB����U����n����f�/�r$��������������٢̎�K݅����0���\\*��J��np(��J~&^�����!�4���F%�I�s�\0U*���u�SIh_84�� 8VY�-m\r�g^խ9��y�Y�m���7���[���.\"���\'0R�+.	,�؃t1B}��!+c�Xr[���/]o�cB�]���� ��&/�7 �1Axg���t��u:�\\D	/�Jo<�����c���UF	;�TSܹ�A	����U\0-_$�� S����s!��	ㅑ�\\�zٞ�/��H�cτ=/_4����1\"����lUA�ɲ�ݢc�Ǖ�\' ့IT��EhL����n���8H���7cHFe��sCz��~n^=�L�We�|O�\Z��q��A���#�C��$��7�m��a;���?	0]�s�����m��x�y�I==��O�&����O�Ղ/�#P��6{*\\��>{(��:c�_wj�C��~�U\\U]����?I{�İ)A��4�e{��N6��F��b-�j+��ѭ?���}��\r�P�m�^���;:������~��j�im9���6��#��y�E�>8�����4���\r;06�@\\`�>$���q!���}��L����p��?<y��W�1���/����G��k�Ͳ�u�O��5�����&k��l�6��C����bƮ���b�|�X�e3lC�{+[zЗ�X���8�4q;\n��:��������Q�ZkW��j5J\'i�^�����5�%�\Z��Y��b��_�>��F�\Z��%��!(\nȭ�|\Z���Ubk�GZ�M.U�d0�8�,h�0���c�� �%v#��?�8�9Q��mX�e^T���8@�x 6_����A�Y;���O>F��~�~x}�!��j�g�@al[�EVt}��]�Wq�����.�\0�@���,i>�V}.�x�*X���Vn��Rz�.����yc��bYʍU�󥿊6�k�l�1+�A�R�����f;v�\'��C����1��sbS���lU���1�z�k6N�lt��N)Z���#F����&��n�y�a2^��qӚ��h���dE�,��|JWs*HP���A���x �ʗ�_�!Q�H��v�X����Y�r�$[W��\ZOG�g�P�QF������?`cNq�.c��J��W_6P�yN.��q%����j\\<�KTA��\0x.��S��@z��|av\\�7?�~���I�,=�K����k�L���ٌ�Ad��a�h�EFxq�f��_��H	�1t��BpWs:\\Ĝ(2�Q^�3j}��+D�FdU�{�ӏ��x�Cа�n��7�\Z;́�{�*_��������m�$���x�P�������reA������JN��8�P��fUe���\'\Z�wk�9�k�2����ad߹���b^���;t�t�UP�L�gU#���H9׈Pm�-ņ~��؜Ǎ�M�@U5�:\'�sa6�9�d�2#����SV*0�k��fC]�+oml�7����fO���~����X6�n�nj��o�btC_�i@]n\'�������`��UX��V_E>񓀙�}X�x��6������[�j�U�J��+���E��eW�l��_œ�^�a�ʮo��׼+I�A�_����*��\Z��&T�g�Jg�f_L��lsN�[پ���lU7!�����V��j��Ȓ�A\"\Z�߫�;��Ҫ�\\_W���\0pu喍�8���`.\ra����	ȷ���[�I�Ax�m8ḡ!hܲ9��f�|dх��>K�XT�|N.�1\\�!��L,��B����u�����_C���Q�|7�޶��7�����gI=-FF�����J!lU1���`edˉ�$���Y�Ä�-�?lld�\"�I�Z��3��iV̠�q�X\\�bf��\n���TΕ�}�H�m�*�Foٴ�v�{\\��\r�VW��ڕ7c�t!���qv[����f3b�`����[xa�:8\"\0cb�Y���[�C(\Z�O�.58���\rv���Z-7�ވ]�U�_\"S`?y��!�����%w�� G#u/��pkW��&2�Jt�]$~����Ԟ?n�<OG�Ӈ��:Y�U!\0�����Sر��0�K��\Z���7w\Z����d,������f<����<3:y��^�����D��t&���|�l�R�&G���_Y�غ������,o�a��>���lI�٨��G��bt�\'1�F�����}2�[oM�\Z�d��3��٢�Kȉ��G��[[������~ok�ŝ�{b���b��\\d����Ξv��ssW�r�p�\"[�ׇh�P��ٽ��]#F?����k?��7��o`q��O�l���M�i���&��4yt-�\r,���]���������zVi�J}��C%\'Y�ݺ�Ҏ�էwc1J���;���R+]\0p\0[O�>7u�fm�N��։х9�@׽sm��SOM0����a��\r=�E��ó���	���h����l��xxQ^�W�3�bsnX�������_�U�x_ʫ_�������m�G3D�d�?�Uš�e�d]��+��ʊ=�x������}[�#x��lx2\n���6�	��*�\\?��/P:E}}�=�W��No���K�!�����*��h:$*�{�+��5^\'.^\'6N���f^P�$c)��F�X�������Obsـ�����z�U��v��f�|N��J/9��x�l�Wo��=*z[���um�+�A>�[kx�����4B���V���xG{�\"nt������������=:�p�W,�O��>��Ip�\0F��ł\0�;R��k�o�ۡV�Ş��??\Z����{V�q��ݠ�v�B�N#���.������r���!��%��ޞ��GqYJ�?�2��o��\"	��x�d��YߚI��f%Ϳ�NK�2��Eeoڣ�W��R�W��:A� #���+.?�{z��w	�^�cSY��9���X�L�duTA{�lYY��ߣ��qWr�-�\nX1k�~�bk���u�G�qq�f@Es[�k֍p�k*e�חH��P�4c+l�\r>\Z�0��!0���������|�U�+j��B��0+��mZ,�o�wG\Z&�s��f�w��RY���\n<\rzsm{p�����}��T�7��6����w(oB��-v�����|9Ա��م(&\'e<�&�@pi��&	�Hb{���F���n��确|uݾ݈7y�9��s����\r��WEyC�mk����[�xG��b�]k��IhY������zW������^��1�z� 3��y�b&AyW����X�U�y�Ғ��r����S$�B�%$�8?�v	\"|�֊��`��M�~�t��� �&���ز1g�ʇP��\Zؠ�D�#W!j�p�:qg|Bu��,)D|�E��r���\\���){E8c6�b��ѻG��R�(�Y���%�0_Rߪ	������ON��g\'���)t��X�{�k�����\"����[0�a b�r\Z<_�C�U,��\rL`�*���Ww)���$���77ڶ��f훵��}d��������́>L��A�+E�LMPu-�Vv��c+h����Ghhg�z��~o�`�5݈�\"\Z/�a�����Q����Fa���n귪q3��ę�mh����.��,��j�u��d\"�{Z٨g��Vk��v�f�\"�Z��(�p/\Z�o�{����I�p�9~	�<6����.�����a����/��!B2���afb6_p�%J�&��\r��\n����ټGQ���`B�\n��^�\'��1�g��5��m�B\'�#xɳ��\'D<��8���]R�R�!�@+��K���)>9�A*~��i�G6�r���� D�X��a�d#�}���+H@�&rU�7\n|x,f>\"����C7����<x�(9��ȹS�+��]���I�;a�q\"���萖���~��va�~W�u��H�%���nL_�PY��r�\0Ʒ�Ֆu��iNssqP��*��9ׇ׸#��%>`��:��‭+�\\5����p>�I9I8�+�;��f|�������-��A;2�G<P�y�d��ၬ���=%$)��>�%\n�h��z��!��K���9�#�XNּ�\Z�OR�B�11�S#j��[���#o��$8��\n��%�9� R�p���]Z/�;J8��f���Zkr,��p��RBb�kO.,+��0���u�JJ�7so��o��Q���)gDGT�ӧ�l�$��_�W��%\r˾����S\"y1uF��G��鄋�;$��������a�ą���s9X�bw��G�t���o��%߰p�jhD�C���I:0L�%u�f���NE�l`� u,@�F4�p�4����q��D}܈��i�fs���x�^<�p�TF	GR�7#��`{����ZR�Sx0�rX�����0������6��S	RX9o�	�k\\hzl�Y\\w�q�%w��a����?��r�K����)z������_+n<�p�\'���	��:��h�`��+$�]Ã��d�|��\n���ߑ{-��XS1���@��%��u���!���ƛ�>)����6 �s�20,\"�]��b:��|�����«�m�2�f����h�f��.H�KI�������*\Z.��,t\"D޼G\\��<���ZO�ȿ>��>ݻ��8D\'��c��\0����A���2Y�N�8T��.@@|=��<�]���T�\nNO�t;D��}>K\n��gO��_�#(�`���c�\'f��)6�SI�^���?+�ח�o^�\nT	^E��v�(T�u��$�oi����3�]b=�jQ��a�؁�^��5�娇t�����ߒ��խ�����\'�-D�����b�Z�n��i�\Z\0\r�~�b��r&���_oI,�[�=�P������t�V�,Q���A�?�M��ɽQ�yE�2����[{�H\0�+��������^F4��Z>_�{w����Ç���A��O�����RD�/g�9H�f��ě���9yW.8��$Q�[;KRQ���Q%`�B�Vu\Z��l�@vQ*煟\0>Ӈ�Y��d���\'�U6>O�\r\\NI׵���E��.���`��EU̯��-k+L]i�T��Dnp��c�,�BP�~}�-m��K#�����\r�r�1�k�K��G��14?U#�*՜�\"��ڥ�c�����F.H�k���KՄ`��4L������{L�5��6\0��LV�j�#,��l�����Cg�h�PD�\'ȿt��3��R|�R����wwE�9-l60sC���c�}�v�ec>D\'����Ŭ_�9�@�D�B�:D\01@&�DT��F��>7�&Oٶ�\0��V�#��Qg��T`&���F���P�P\"�C��w�eى�I�[�ԀЊnGqF-�	��HH5���E�Il�����ɫ5qԸ=�f\r��{r���ׯ���~�mNa�$��,�T7%�V;!�����l۾��y	�U�Ҏ��F���]0{פ���ԇ�D��.T�\r�g{k�G�uu؁i���\Zd.�Ծ��P�����S*��M��U.P����9�?���J�Og�GτY�XH9-D�?K����d%G�z�a�E�L�������>�´0����S�|fI(��5��f<�%r��qVo~�(k/y.�Qg]�!܉�G�\0��Wx9�Na��.!UW��\\�Ӥ�}��+������DI����O�I��nmڹ[�j���^v\\� �R\"8~��#F>v��,�v�y3��u~&!�:����rt:Gz�ߗN���T�Dj�h�P��xv�١?��E�Sp|�Y��(�G.�c]�%�[�x��t\\�8k���}���Q�Ox��\Z��I:��\0���μ�F���g��b�7P^D��A\"8/�MJ�<S�W.��B���,vF����,*��y>��j�u���vמ��\"%�e!�Þ#Փ:�&�/Kپ�ܬԥ};X9]C~�\09Y�����~�1=�����˔���L�.��D��2�x(k��H�͹L`]�a\"�9�܊`�.6g/�&�0yCJ_�o���&�2��\n��Q�>\\��..��N��mDM?GE��ɵ.�*��Bm�~Zm�U׃�d�q\'�W`k�\nTͯe���p��\'0����jA�����g�b@��A��4��o@�g,���`Ͼ������OJҜ#M+�ʝvy=��PB����o�|�P �!>!c��`r��Y��P���hG��\\P��\'�t�>��s)z�QC�Ñ��_�Rċ,��ѳ~��K�]P}�%,2����y�ڦY=/DT(�%l9խj�^I8o\Z��A¤J�s�E���QZ��u����/��C1y��8T�m[� =�K���7�C���H �M+>��b���\\55�S�̓K��E�[l���c�;)dՊyI������=�>5����ˠ�@.�kYjP<N��\r�.r@Fy_�����B����)=�:28�vh:�0�t�N��<+B]B��_�\'o;\0��˲�y�t`֦r�Bjd�=�K~����;�\\�\0���y*�\Z���i!�,B,������g��_]�-��{bX�����k�{!�E�2V��>�L�U%˙\0����b�j�	�.\Z�H���?m�\nti^E�H����[_|Z�d5�M���$9��y��ɯH���\Z��l�f���Z�m�s��)pc���v��\\�r��%�oQB��|�C����K����.oJTr0�\rCk!��o�$���U.�\\��~���qU����s�I�3!�3N�vA�-�����g`E��>;�R��F������������?Tr�$gG��F��A;��JgD�N��_j�������g_�jsA����A\\0��D��F�Ҥ��ɖy&9ä_�pRZX8Q�rWS:�2O*�e��E׽���|�kJ,	�R�?F�<����E�\"@���$TJIC���ϓ��]��e1���f���A)�C:ZX:cv%v�ז�i���j@W5�3l;q�!���q����f)YD�Ya��Ϳ��/S^��/��Ț��_��ԕ�Ϳ�8O���-�e�j��`Yxv>�xqrEd��\\;��Z\ZTvm_��$1�|�vI`b�G��mmE/`��cCwun/c/1ߞ��$�q�C����.�8���\rR,��.�-y�9s�0ǀd%�J`��?A؂��Yr�i�rd��eӮ�P�8�����{_j���M�z��}n,�?�RV�����g�z���z�����1޼<���/_�6��e<���wn߭s�`���r,M9����ku��w�����f��\0��`\Z����5������կ_�~�z�����8��M�@P>(Ej/�J�/?8�GN�,�n�y�N�!��-�lh,��g�e���S��~I��Ԣ��>TNe�	f�~�6sx��1�N��K��B-r%�HyX2Dt���o�̶�p�����d���G\'�ygPOSa��]6\"h��Xl�7����hp�0��b}������ER��}x�p9\"�f�[�\nvu=.�ύ%*�����A��\Za?<\'Q<.��4���StsCl�W��OdC�)B�^\Z�}�$�y���\Z�?�AEv��m��N��a\ng�x<��C��Z51��JȮ�Y^��H��G�	�m��l����EJ\\ۣ����t��Y�Пi��	/rw�&@�#�?�j���f���;.��\0,�˞$���h��S�x�Y�o���ms�1��5\n_w���:��l34��r�|�\r$��,ξ�P�����~��EhI�h�nLmQ6������(� ���j�9���e��_\rB�<�9���1��Wfh����yW��4���pH�u?�\0�tL���q�p�Kz�y�M3 w�2M���3?�:�����$2�v=����i���P��0ۤX��|<�ЩX,����`��E�$:�akc��V�k�{�+K$MD?�7�\\g˹�8\'�\"0;�n\0f���+0�\0WΓ�§\r�c��y���I����|�mvΛ$��I��%C;H�*D�F���;^��;�\Z�iT(�Z\'�6&f�ٰ��7�����>u�C�vX��BT�ᅋ��K��_�k��$���g��k�����%�\n�eh$�b5l�e۽s�\"IBòz�;\nҖ���N�s9��K��SUw|�!�:��W��)3���VnEҺ>��F�A���c5Ұ�}��qy�K1�ǷXٖQʔ:�=wB%(:�ż�:�}_cT�EQk���=K��XJ��6V5��i�;ȣ���bf��s*���/>\r�����/��e�l�񱰝���_�>[����\n�D���܂��`��Ϸ{l7B�h(�Yp���Y*}25�(���ˏ�I�?��F=�u��	�TA��_n��[s�F��^+Q��Ř%x�+� �}��a�YDX�;��.D�]0�\0Q͑�����qM4�M^�K~�7�d\nM�����{��*��n�Cg	�XY�*�:Lj�)� �]6\Z���-�n2�0�)��gb2*s���_�F�޸VS��bgNBi��z&�	��蠥	9᠅ƃ�ͪ:�p���3�<�f���-�Ɯ�n�ݠ\'/��ٱ�l�̺f�a0xh١	aWg���?ӭ,��X�#t^3Z�ٯ7i�J^-�=�;��v�s�S79�+z$�\0ef��������,��.�y%���ޗ\"I��kڸ̊G�����Z>����N\n��~i?]�=��V��C���ذH���\'A\"y�^w��m����B�U\0\r���\"@*�I�\'��Q�Sx�a]���pXU��y;�kT<�D��f��=����A<:%\no�t+���&�S�d��B���EA3#8��4�B\r��K4ā+����Gii���Q$���a��,=�63\"��?�ӊ,�:�2��	�����r���`��!w=�QY���ӆ�j�%\\G���n�3����v�?l����e&��$��|�\\�oW~�I,����\'Ǐ_=	o\ZZ���- ��߉Y�5VR]��%X	�*�6v�8кf��ۧ���	�\\�2n���`��	\\7�y�^�M���0l�^��.�_����)�6�H@H]g�3�o�k��L���\Z���a�c]\\�.�����﫽�7�gGU �\\gQ6��~�`<�2�!��\\����|��tB���_���c<�I�v���㳨��\\�2Bv9��C5@@�ǹZ�����Z�%�ׇ`��mR�JhY巼���WL�f̵���\"I�Zqw�c�aMq��� u�w�iuf�n���w`�V�mlM��t�kt¹�U��kq]�\ng����FG�ӫ=�\\��dF$a���o ���螭��N~��	\\?pV,���\"\n���4Y�/��?H��hc��D�E:0\'��(͖x[x�<2��	�̄���J<ʷ�YR+���R��\"�E]���<,�WRĺF!���Ԭ�ׁ���3..�OC���O�z��6�W�����d6 >a\0xc��2\"K�������+�ܧ���*S�=9C��3�J�	S� �t�������6����#ܺ�^��U�!�E��r�(pm0�.&�q�����M�o��L 6?����R��W�E�ԗ��IDϔ�����	���m�=p�h\r��<��2#盈K�2ml�=�A�e��U���S��R���\'fO�~�E:��\0q<�|�\n(/���>�ݻw7��-��U��g[�9��;�C\"\Z�{r�Mld�	[�1��f����}y�ȞS��I�ĞÈ5ǂL[~#%��E�@yU53�)�E�7�X�x�E�XBT�%=��m�T������P~��!�@`����cj	xqZ�\"K,>;�͠��zR�\0գ[�^2��/�G�sn�\n�@��]f��pρ|�+��6�FȺ<����\"A7���XE�a�a���m��[8`[���B�[���	n�ڸ8v�N�ò��u��O�X:��Q�4�P��\r�ýY�o�-UMyC�=_�X�3���4QBo�կP�G���l����L7u��敐B)����7���58���(Ϗ�g�[�b0���P��r\'�d9G���\"��|؍�μ��pi��jt&�X���\\w��;=z#���y��{?����!��֐���}7�c�{<��dlh��Bq��Rf뜉C�g�;M���[N��d4�ݒR�-������3�^���$4��bPwhy�rk)h��\'\0�\"���ꉞ\Z�L�P�4��o\n�yu쇂= �	��[�H�����͛�qXL��s�9��EB��S����z����C�1�2���%]����H`t����?3�\\�\\O�̓QL���R���Rp̚	�WȮ�<=�H\Z.Z=��\\5>;lX���\\0���UjLVg��s���O��S`w�L�0:*Z������Br;o�Wb�DgM����3��p\'ٳ:���\rv$�IC�X���[��#��s:i�m~1��v��y���p悯%b��YC��Ł��9��Ғ�_\'�ԽEPC�É3A\r%�Z���b�Z,�D�`ro��A��K1�a�fa�4A|�_�Z��\r�_2X��l�[��R���u�e��b4�3���6�d_\"�K����7�	���6����)�_[ftG�����~��`��`�^�QJ��Hp�P���:^[(�M��}_^!\nt��7t�G8g[(���h`�:N��/�^\rBG`o��}>\'ڦ87Fn���Ae��y$ew��{�-�^�O�<�R��@��oΊ�����;�T|v�G�]�\nU{q[��	��~�w�/�Km2`a%�n�)�(:���]�~!v�W��֏���Y�n�g�?	�a��*X��r�u�������u��q\"��+x?�_<\'���4i��-����BbTp�t���< �/J��B8.�m.�ɖ(��e�����E��\n����ڟ7T����8�=J��ˆ���!`%����H�Ԣv���i��!{����Dz�t�P	8E�7DW-�Áa�u7j/���\Z¸`�L�iK��e�ΰQ�m�푑QM+S*8c\0g	���֌��䨆�\0&�h��|��>~��,,�r�@������\r\"�6Y>���Zb�!�TiK͢�3�4P�u��h���b���J���.-*���q���E;�1��L��jx�ؕ���T>M�Gˆ�Ʉ�~`��8;�{�}soNZ#v,N��	����?Il�j\Zg�{dh���s��*4�G���,6���dɬF��0�lЁlL���Q\r�+v�A�q;��k����j���v���ާO&VrH�cԏ,��R�gY\r��VA��J�8���dOMHia,�l�V�(�eA&�9��.��7.���mpT�!=4U�݋����z��t�J)��w�(.U�����ۺ�mC���D��,�9�]���|`�m��.���֒Hc	�VI62ںn���J}�\"�p�n�Q�	[�p�`�|Ǣ�uB����2e��#Q��yQ�1\Z����u���Ț\n��*���;E���@!<kY�		�o�d��5�sX�����Q�Fq[S��1Ў#B�g���fk�Œ����Lp��8�.\"`b��(B���Gϟ����$��ٜ���=7c���(�/�(�3NKJȦi�&�T��Pİw��v�d,��?k�Pg���̑d/Y%A��\Z�����<�|\r���GE��Զ[-���ڷ�����\\����������d����WS�9�q(\n����`�21�\0��^����`ww�<\nGi4̯=Ⱂy��#���Y>B0�9���#8�����m\r9f��0��\n�o��� 3\"D��I���ċ��M��+\'���Ϩ��tV�:P��I��BD?�Tst]�k��sX��5�P�Ete��5T��8-$wԼt�=���E��E^Q�v(Hg3�����?ӊ���\"q�k�B�͓����ߦω��o��Ν��Nx J^@�\0I����?L�08��O\"�w\0U�Q�r�%m�6[�y`$�h�b?�{��:�f^��Գ�\\k�\"�o\0�z*Q�6�\"���m��gi��o�ξ؂\r�KƵ�;[*)�*�����q;��]�����mOK;;��u��hT��!(Y�����FZ��v!��Ԉ����1Kj\\K��b|���Fj�oY~��d\r�̵-01kF�C���$C8�l1s|\'	v�ii�=�>Ib�7o\0}���w���w��i��qi\Z�v ;��>r�~��J�P�*���a��7C�o�L#H�\n�	���~!Gܙ���:}���R��nI�v8��~�S����]\0��l�M��<��1dCK��п���֑���H�z^���V��p?�N D���\0�#�Xg�E��Mh��T�`q>}rj|XL����R�J_^^(B�͆��6Cw���8���!0;+f	���:|����#�ALR1���i4ڿ;Ná69p�9�5�����,FlHZX�t; %���L�pdU�Y�#D:�v�̎��Yd,f�$���f��B�x7��/-�������Je}�͒c���˳�\'��[���QŢ2l��l|�T zRΞHK�*J�)Ν��:�5C���I�E�H�=����~n`��(/���3��q��O�X��6�i���!n�ރ^ۊ�E@0��1?�Q�4f�f�@<���79ee��v�G��>�3�/�AqU�GE�\0�)��a�35�]c��\'���T��m����x�\"����o �b��Ţ�%:$G�8�h;=�6\Z�qTz�,E;[�`A��D�h|0������E�[����Wa}�Iy��ݎsh��!~{�Y>h`�=�ƦDK(�mY��E������V���q�����9�;�$ޕ^�˅���\nb�����:��� ��#��Gǳ��6dK��qȬgdK��\'9|T�~�� v{{zn�Q�fv���Z\\���HC�vn�쏾�E��\'���}��]��l$�0n���>t�r��n�I�n�n>$�L�n:�h���~?��C�+��%�H����n�bP�,nzy��=�D�8��d�L�>�:�1Ju��-�W�ǐ��>��yjVk��h���g\\`�<�\Z[�>��T�`�?�����\rS|���7nyR�dm���.&�:��%�In�C�F/U���jg�b��$%ɐQw�~��!9��	��JP�է�:�8���V�\"_��¡T\n�0^�iQ1��C���W���\'7K5���kF��z-�ܱ6sEVm�&�f�!5K	q\roT!��l\r�|��|y�\\c��)��V`��;�{r��P�J(����M��g���Ћ�s`\Zv\'�.c�$�`g�;��|�!InQ���wbV�:�ay[x�ԅ��].����\'c�8��?pxPJ!�z�a��\"0����E�2�;�_	uhZ�v������މ;v+�i�����R����Mm�1��ԅ3>\n^�o������T� �E�YQ���cCM�歸�\r9�:U����\Z�k���o�~����ޜY�\\��o�Z7����JMp�A���B���qGx��8L@w�Q��T+�g}��OF��2�P	D���H�����5 ���6��ڄ�C�����go�#�|\"��h�`��(w��x�m��6{��R�H���\"5������Ǐ�zi�׷D�f�˟,��]*�%��&�M�[\\�nsX���W�z��3��B��$�3B�HM��V������O�4�*`78d-�!�Ĵ��.8���Q��_N!��иt\Z�/�	\\��[����\rpBu�}�\\�9�B`s^��G�I����L��I}�8{g��OJxM\"n��p����vT����\\�\\�uc~ܝ�l\Zv���-½/�7�n��^�͛g����L�X~KW+���T�8\'}����KGS�C4\r\ZԔ��H��p��gI�����II��|(�#�x�1e�&�//�w���� ���1 ��6R���A�ͳY7O��I��.a�2����ER^t��I�o������៓?����ݟFD��?��bX�	���E:��;�����Qg�0C:r�EZ���ф���Eț�?���\0���Elq%ϱ�z�#�(�&�|3U��R)�n���ǰ�\n*�Mb�G��%.����*;o�h�\\�o��)f��O�e��NeK������T\"MJ\r���8����B)��\"\r�]kCl(��0�!T�I@T.���k��^;�R�B��V�|�f�2�䌀LP�%�|%p��6�`*|�:9�­���|lJ��r�F?zg&B|\0C�,�K�\\�l�R�87O��;���j����I�Pc�S+KC��m��p[����\0n��3�1(W5\n���W�\r����7��As�@���s\rޫh�D��f����:��P����O���c���x�@�97��m��ķ��Y���8��ۡ�ԇP�s|+�$��2q������Z;��=Q!��e���BT���H�]4�\\����=��0N�S#2]��>�Ľ�Pu�.9�&/j�4g4*�t#�-�$����*1�K�󈣧�#����i�{��C�a-��5/��ByX$�2�m7e2�jN��U �t4��-$�9��\n\"9jG�i_����/���Pv+g���n�Og�+6���q��\\��u(!O�t�����@�6#�̑�̩�%jZ 34�?X%�0\"�Q��m�����^��r���V\r���l�/�=w՜�k4nW9�n�4k���V��N�I���^ƣ���_b|$\\����\ZKtD����!㴐�Uq�gǆ��N�p4HB^V��FC�a�p�p#ʒ�̓��4�Θ����%��1������k}m�o&��`rꮢ���\\81�SW96S����U�\"s�F\\>?���;��S�5�L2�	]�B�ҕl\Z\Z�M���Y�~;t��BL�H|C��S��/j2����z����h2�x��UI3x\Z.��x׸y�XW�\n�ɒ~g�0$�ehx��G�����Rac�ca��-��Cz[�;�[�2�ǁP)p��Z��zq]���F�[��}�����I2����v�X��T�3u��\0�2�	�-3�^Q)�$bD��{v{ۑ\Z:i`��X2ȣb<5B�$�ڕH:>���9�^���/����eʒ� �_\',qIM<G 勬�Sr��h��0Ukz��� �@�g�H\"KruQ�	���o.R���OO���E�N�\"���	;?���j��K�x����5GGO&Y|��X��3s��?-�՚�#8� y���v���2�)ks�S��oı�&X\"�\'L���j��\n��[%b׸*�vV�R�\"Ѭ3[͚�@耴�n�J�:�Q_����������2\'7/�]cO������\n��S#���h�F��kb1=ٶ:}��eC�X��Pɘ�4c��bk9V���9����\Z(�e?�;�e�X�o�.~��vIJ}\0��)��Bt\0]d���@Og\"�kSg;��O��(�$.�iuςX~�<c��rZs��f(�!�M�u&�gy>�g��^\0�7C\'��:���n��ȡe�tY�2R�O��躉|v�����1ʡ�j$$Ň�Y����\Z���#�q�AF�=�Ӱ�̬��d��6i�-FI��a�sK��J�]�r��\'G�7�����fIl(x:)e�$`��(ˋO�\\�z�-F�_���[)\0���=l����5ر�&3\r���\'9B�,�����fʚN1�Z�8�x�\0�1�d����R$���@��%ʓ�r��j/�\0\\>HV4���=�5�4�9^d����K�ϑw}��,4�-�]{��z�ˈ{�Y,�mJ!ܰX\n=?�<��9����5n��׬ƚ|�J\'�b���pj#�y��\r怅]��ՍI6�]\"� ��S!r��H$�\'�*x�~R=�EW�$��Q��A�m�wZ ȊNw�Ӗj��^�ӱ:n��W��Hf�T��}*���y,��-��h���iCX��\0,�\\4a��>`A�$qx+Ǟ�cc!Q<��zk�[/7fD��ҚFw��?I�ͿxW浳�o�f��g`�%�\n:4�%*�L���mc+�00�Ꮉ�TI2�\'w��������m+�j��.��HPC/:\"�of	��)�q�@~O0�Ҝxן�}��IGӄNiַ���%vNr�T���,a����9��O�c�O�X8u1u��Qi�+f�T^�G+a��@�\Z6LdГV]�޵S���Nk\n�p_���cR����0ѻ+��5#���\'\r������dJ�9\n҂�%ɔ4q	s�V��${�J��{#�iW1OF���,`\\=��� \Z@�V9Yrصy4�:�Y\'r{`�σ�s�~�Wx�(��ԣW�o8}�����P��᭐x�V=̓�ϲ�� ��� }\\���))A\'a	�lE�P��v:���CO�d�z���b�}��?�،jC�3C?�7�qY�-�Wp惢��׶x�s^vz`�ć��@O���l܋��PexU���m����![=��� 8%i��|1[���?׳��ٻF+%9����bG��/&�2�fM���Rɟ��C�~sƓf!��@�4�a\Z�����.�o��f\Z������sg!�U��G�ksL�5��J�)m|�*\r��W��S̀��CEOk��c\\=��8qae���o98>�.в�Um�mj��hC�6���6I�`��5���$!�9��\n!@=e�輪�����}%�+w�i���C���mE>�v�Q�Sd	�Ei�(��b��qHfb�hf#j�S��f2I\0�(\"\Z�9�z���D=�@z�y�z-^�f��3��;>lHs��^�	E��M���	�1g{�������cCKH\"�Z�8h��\r}T�$p��=�&�@�Z���c2�iL���+Γ\0#�jdg��2�ϓP\"sҼ$&(r4:�A\n�ސ�ߴT7,Fu̷�qC��,�D.u�W�>��Y����Ş!e��z��ZJ�1��(�����ۇ\'���\'_��Z~�qH��x�\n����/\n��AD\"��Y��<B@!�\"�z�E=�گ0B7\'����$�w�To٩ext�3pF[?�����ɸ����&�.{ױ��8�y<���!�4�B}q��-i��𝪉fD�Q$���Bq^}�=�,ļ���㜝�R��G�B[]����	����J�!ff~Ƒ\nr�=N�Jw�z��(�|�hWK5��\n�QF��9�Ց�*�n�zA��������\"0\n���!��m�U�kF䛡������+\")p&\'��\r-����#��(0��h�SЏM�ޓ�p!��B�q}į%|�N�r���<����Ȟ�gg���^�{��BE8�iICTY�O+��,�P��W����,��,QG?�i�>���nǁ�� �#a>tl�dMl�7^d��a�e�T�zLM�#�f����qąb,ɅzOy ��[jȓ0C�7�/a�α��Oi`�8+ܜM�C\'B3%}�B�e $G���{~V�Z�u�_�l6��v�?I\\-�����6�w�JZ��}�\rу�5!\\�R�b\rM�I�,92��W\ZH�b1r��0\n#��T���ɝ&��h91Rf6V�NTÃ�w1]XH��XBxI�S��e��g��\Z�w���`	��|k�۾����]�TW;\0\0��<u��Gg�ܻ!L�ti6@\Z>�<����}�.00���lnz7��:�mn:K��Wxl\0���� b��c�s8��K�^:����l>�ra�0p��r�>g6���@���1���8g�1=>7m���S����n��\rr���4�;v@�!Y�������X�_�#��颬�0� �`�TG���K��`�����c�R1������j�f>Y����zx���%k$k�;:��ъ\r��BB��{�����}�A�g���+S{a�9�=_���\n%��&d\"E>\ZdC$�0�B�\n�q1\'&�Sp	\n���?�\',�rv��_���F;�,ZB&�0:K2X�*��\ZŁ��v��\"g1���X]p/���K,�z�U0�m?�y:��\r@�	���}�ҋ��5ܜ�$g�n��q��7G�K2~7/6��mvK�8)ϝ;�fӡ�m���z?L6?�w��\"�p�(h���=O�����$C�X���(.@�Qr��a$��7���ޕw�E$�O�s�U\"ݡ�\\du��_I���j-E��}i?���E%��ζ���`�NF�\'s��d���q\Z\0�v�/�	����t�+�AÛA:��vӅ��f�i,�ЙDmB�O���삸����ᛱ36a�RW�#H\\_����6\n�r��0�������t���pܼ��\nHr�CI������q��E9g�ď\r͜\\��QQ璮q��l���\\=!�V�q{��jɍ+�\rm6S�Ř��M,�X�T��?lh���sٕ���s� �%7��\n��F�+!oc���&���x\'<��˕���L_V��[����s�ŬU椸+^?{�d�\"SA51��M�!�4ͣӝ��]\Z���tG#�B��i����6h�\'�$�MF߸�\"�u��\\5Ih6Dg�	A��/�Y	�#�MFsy����N_��j���Iq��7�1���T�����N�,\nH�KZ��fY�]7,�\Z��M�#���O~e\n��w5ǌ��71�����K1�ΥG1o���ga���ӽ�@���GE:���)�,�#\"���-�+\ry���*ف��A�72�|iO �r�z���<�a��Q��Й�8�;���]�z;�^�]�����\'���xx�l�h;��n��ލ����������so�~�������im��ڃ��\\�h�j�h\"L�s�.5��ښ�\"�X�%�s�:\'�3�p4�=��ɗ!�[w���:���c�U���P��S�vj7�P&fW�#�֡�}���d��w�E�%־�E�\"\'N�,���)7f�\n�{��݃봇���<��П���s�M`��f]�D��8�	7)�P��=��m�����<��2�d�ťĩ��ap��$�$2p��0���G�������7\'������Jc����щq9\0�X�`o���D��ɨ)��ﴊ��T�U�e��햮���U�o��uf��	���\r���79텶�a��v������O�>��_�wa����G�Ʀau1���Cۜ�o���(����c\n�u{x�f��h��\\�\"�E|K3��+� ���5G���D7\Z�d�k��/�qו�x/��|.��3kN�Np�ѐ<��@V��{,�� 9�V��e�v<J4�_ r��q�w��<_\rF{��>��u��\r�v��Qy �M9��#:���D�P�ԥ凱���4��6ֳ�\0�o7tVǒ&�6�h\n1�\rdry3�����>v?�s����f9[��}�<Q�=Ν��\0�ϑj�B�>�0��$l\rf��������#�����ρo�loޝsų��Cfg���{��YK6�����҇d��i���R)ꑽ�h�|����RG�Fd�F�[�:�.�ҟn��9^����.�)ٔ��Ҭ-\0��#����N�ُ	��A���4qQt�(^.�a�������ick��\nA�~-8F���XO#j�A�G�\r�[���>�~���2�j�õb���QKv9�\n؏is�����}��\"\n�[�\r�)�V[O�ԑL��vZ,&S�V�Gݟ�(}�Y|�Ƒ�nA��\0�0z����V�ښK��N��^�)���T\r�$qs;�&\'N�]��4��\n�>w��nMTAD���%^��Q����ڠ�9�#�J֟Ko1�;�,����S\\ߕ���)�4lSF�`W!5Tu�8��ej8/S�e&�ɠ�0G��ǚ׹颷m5���\\�[�vĥ\0��f��\"�|�ff|�Ӗ����ע�$�y�Y�ȬBg�:��$����3\r�D�+LF.�\Z��W����[�`k\r\\@�b��ҹF�(\\z�=����K���tn\nȍ�q�hyb������Z׬��ߪ���^��L��9�m���c3�O4G��w���{�����_�x�<9lC ���^\r�9�Y&3�Q��O�e��h����7�3��.�!3��i��r��pQ�RlB���?l�pf�.�Ln�m��^�ϴ,��f����gD\r,]��\ZF���mB�{��？5�ѐ�K1�V���`�� |K/Ed����sCW�W���_��M�f��#��\Zc$�\03���&?�.筅ғ�¹�c]��&�z�}�nm>�~P�1M��+.GLY�#Z^1��~�s���������c�FQJ,H���3.2H]M{Ѥ��.�ۣb~����Yi<�AW��l�{����t~�ò�\"/�c���8x�懄�x�a�^x�DF�\n%�ʮ�?乄�)h�?:>��aZ�Ee뫢��ɳͧ�ư!�G�ν����lPdr�?���4��y�h/޳����R\"�dU����wb�1���,�zRu���L���〾�N�?�\'�>9�Ւ��7T��0�Y�m�א,����2on����g�z�)e�\'�A0-[C�6q�{c>��\nx�e�\"@�#�}=Nj�h��âA�Q�WH8�n�A{T�	��:�갮ͧt��ۭ�[�G_lI�C�A�]�@����N-��͞Oa�s_�T]\r���ơ1Q4��w�|�3۴���6�l>\\�u�\"�2��\"\Z���+T\0ؚ����@�̣O���}�ٔC\Z�J7��t����a�M��<��!�4�)�j4��V�V��{5\n�u��寊�4�b��_@Ǽ��:�Zkk��kk\'u9�|��v�S�?�ET�R�j��ݵ5.>��q�jѣ�h�ԂE�ҢZQ�����{���3.l$�J%���0	ϫ�e;�y�51�\'���\'���D�7Oě����`e����E��!<7*�kmA��0A���wz\"�`��7�3�znJ9kt� C7!y����&�/*^�X���0N�X�;a}6,r~�F\\B-���?sp�����F~�4�R�����������e���ƙ�36�i���h�F�����b���ܴ��\'��H��	k����ʀ���a��G�޾��5z����Yp��W�}�p�}�h��U�%	E9t�?Ϣ�HW�4�˲�3@�~x����l1�i�8��v騚G�R>g���,%(jn��G�#�\rv�t�W����xW��\0D�v򮦧Ͻ/�Wĕ�$E� Vg��ŝ���B�	J�c8��z\\�w���[������g�.|��W�\nD\nt�\Z���?���]{��4��qy�V�0�z���\"b�{k͡a�:f3r���Ҋc�p�/����^�<���#NE������b��I�4�۟\Z��+N�*�0�Ԕ��[4������ا��Β8�$q�h�\'B3i<�^�^�F\ZM6:g��N���a��)���\Z�qs:Ƃ�����\\[�F�Ņ���]R�\\4+����O\03;��wp�%_��[�O5��mD�8�;��*B��5d�7��@�: ��7+��$��v�|�B�.��CSZ���M7N��cbZ��s�Z[���con=$޶1���R2��.8�܂����\r�H�Y�`S �&y�HS���l�CC�j��]i�a��8T��z��_�ݥ��/Y�űyݯc6��!�b��]�]An�bn%�WƹH��`�-�p���)c�ӆ\'ū���\"\0��L{�,�pb�������8/�Sߘ��vq��;�o�,�,\'`a��p�{�*��H���{�}�78�����\Z2/�mMc.����9!X}5�7�5#���9�n�y\n�Ѽ@����m;��~\'�|��T��?�\'��S�w�	(\0��[�֩���l�����#\Z����`�^];f-��ٽ���i�u�4�~FF�UN�1�T������1�3�C�߄��u\"f�6/�]�n����b�{+\'(��d��\n�Q	\'�E\\%���/���g?z�O���\\��!��:����_K�E���4����#h���AhqXd�<���<x#�̗=x$����ԁ�:���O���.�4�Z}4[�SGA\r�\r�n���@���q�T4�޽9�1�i���T�y�U���W���NZ�O�F�@�nh��:�m�=��gƓN��X��m[�Ҋ�:�b�8�8�7�����|$�k��e1�{/}@�̽��{��u�wÅ�9�G�� %����6@�(��2��7*w�	��.��`�}�0���u��c �;mwvȣ�pHxE?b%r�����8�EZ�c�È>����C\"����{�-w���{�M��@@w(рؖb\'�?p�F�U�,�a0�\Z�r��eN\\*�\0�q�����ÇqQ}cB4=�ua\"4���\Zhe;�	Ǐ�Q���ڼ�*up9��os�1̯�1�$��Ǌ\Z�Dk�+j��ZC�h�O�v�O$Mն��\r���p���S5���^��w�P\'�5p���Fx�}�-�㓙�����ԡy�ߴӐAZMeE�8��F�p�|t�1ש��)뚉���yܕ�U�o�e��#\nN�Ys|��p��fg�d�yZ<\"ASr~�[�x�d>��]=l���Z>�8�o%D�Ϧ�����GA.a�I���U�0��z��AZO�K�����|�����g����A��������z��NG<�H�}eB\r��r�C7��ٶ!����~�H��Qh��,w\Z���F�� S�&>��Z�H�UcY�B,UOq�Ŕo�[��A1r\'#n��\nM\0�\"���K�O���]Y�\r��u�@4��:���[�6m�{=�e2����L��4c v.�=w��������T�b\Z;�X#X#$�x���kp�)�T\n�WuR5\\�2^̪0�Nc�=>Gj�l��5�	�Fa��&=a���p%�n��0�͕1�ߊ�M�v�¡8��mUf��Q��J���Ptܖd�n\'OơG䃛�������ӷ{��X�\"UaV�������l��R���.��������[�4��E䆿�U$I���D���pu�Co�A܌�\n��36���%V`���� 0*VX��z��q�:\nM��\n��_E��0a�\"%y��t�5S�v{K�9�NLݪ��%;k�K}e�&7!������s��mS����n-���ͩ�*�P-ل�ݳ�\r�C�ty5�Y���gW#y.nZ�h�9����f��\Z\0�is�`v;�����w��RI�.��􋡮�$먚	{$���̷���7=bwK����:������[��1����)@a���AV{����qR�f�⦫��	�������m�b&ŹXn�7�YZ�E����������s]�sJC4��C\';��iS����}M6?�J�k�*	lԤ�X0��%w,Y���P�+�>�G	K���� ���<.�Ye�Tl\'��m������w,O�捇�~�\nG*��C�0��1�Ԋ��G��D�<N�L�.��Xe� B9�,��t.5���\"�l=��g�0|��{+$|�����0SZ�eE���O�p#�qw\r����j35�|t˹�ߊ���-LL����i������!Ȱ�5@p�!E�4\n3g���2;Z�<:? ��9�ŝo��9l,m2�3�,��������_','2005-09-01 10:42:03');

--
-- Table structure for table `oldimage`
--

CREATE TABLE `oldimage` (
  `oi_name` varchar(255) binary NOT NULL default '',
  `oi_archive_name` varchar(255) binary NOT NULL default '',
  `oi_size` int(8) unsigned NOT NULL default '0',
  `oi_width` int(5) NOT NULL default '0',
  `oi_height` int(5) NOT NULL default '0',
  `oi_bits` int(3) NOT NULL default '0',
  `oi_description` tinyblob NOT NULL,
  `oi_user` int(5) unsigned NOT NULL default '0',
  `oi_user_text` varchar(255) binary NOT NULL default '',
  `oi_timestamp` varchar(14) binary NOT NULL default '',
  KEY `oi_name` (`oi_name`(10))
) TYPE=MyISAM;

--
-- Dumping data for table `oldimage`
--


--
-- Table structure for table `page`
--

CREATE TABLE `page` (
  `page_id` int(8) unsigned NOT NULL auto_increment,
  `page_namespace` int(11) NOT NULL default '0',
  `page_title` varchar(255) binary NOT NULL default '',
  `page_restrictions` tinyblob NOT NULL,
  `page_counter` bigint(20) unsigned NOT NULL default '0',
  `page_is_redirect` tinyint(1) unsigned NOT NULL default '0',
  `page_is_new` tinyint(1) unsigned NOT NULL default '0',
  `page_random` double unsigned NOT NULL default '0',
  `page_touched` varchar(14) binary NOT NULL default '',
  `page_latest` int(8) unsigned NOT NULL default '0',
  `page_len` int(8) unsigned NOT NULL default '0',
  PRIMARY KEY  (`page_id`),
  UNIQUE KEY `name_title` (`page_namespace`,`page_title`),
  KEY `page_random` (`page_random`),
  KEY `page_len` (`page_len`)
) TYPE=MyISAM;

--
-- Dumping data for table `page`
--

INSERT INTO `page` VALUES (1,0,'Hauptseite','',22,0,0,0.185292957635,'20050831105200',1279,297);
INSERT INTO `page` VALUES (2,8,'1movedto2','sysop',0,0,0,0.918068816336,'20050831104120',2,27);
INSERT INTO `page` VALUES (3,8,'1movedto2_redir','sysop',0,0,0,0.336809329643,'20050831104120',3,28);
INSERT INTO `page` VALUES (4,8,'Monobook.css','sysop',0,0,0,0.779332721808,'20050831104120',4,117);
INSERT INTO `page` VALUES (5,8,'Monobook.js','sysop',0,0,0,0.972215165776,'20050831104120',5,3353);
INSERT INTO `page` VALUES (6,8,'About','sysop',0,0,0,0.547850038737,'20050831104120',6,5);
INSERT INTO `page` VALUES (7,8,'Aboutpage','sysop',0,0,0,0.735453959962,'20050831104120',7,26);
INSERT INTO `page` VALUES (8,8,'Aboutsite','sysop',0,0,0,0.892925896776,'20050831104120',8,18);
INSERT INTO `page` VALUES (9,8,'Accesskey-compareselectedversions','sysop',0,0,0,0.752424712176,'20050831104120',9,1);
INSERT INTO `page` VALUES (10,8,'Accesskey-diff','sysop',0,0,0,0.212719103097,'20050831104120',10,1);
INSERT INTO `page` VALUES (11,8,'Accesskey-minoredit','sysop',0,0,0,0.615967919443,'20050831104120',11,1);
INSERT INTO `page` VALUES (12,8,'Accesskey-preview','sysop',0,0,0,0.723469559254,'20050831104120',12,1);
INSERT INTO `page` VALUES (13,8,'Accesskey-save','sysop',0,0,0,0.892229014134,'20050831104120',13,1);
INSERT INTO `page` VALUES (14,8,'Accesskey-search','sysop',0,0,0,0.039690732529,'20050831104120',14,1);
INSERT INTO `page` VALUES (15,8,'Accmailtext','sysop',0,0,0,0.399393115198,'20050831104120',15,42);
INSERT INTO `page` VALUES (16,8,'Accmailtitle','sysop',0,0,0,0.672364663828,'20050831104120',16,26);
INSERT INTO `page` VALUES (17,8,'Acct_creation_throttle_hit','sysop',0,0,0,0.861385520779,'20050831104120',17,80);
INSERT INTO `page` VALUES (18,8,'Actioncomplete','sysop',0,0,0,0.897738924786,'20050831104120',18,14);
INSERT INTO `page` VALUES (19,8,'Addedwatch','sysop',0,0,0,0.331761061806,'20050831104120',19,34);
INSERT INTO `page` VALUES (20,8,'Addedwatchtext','sysop',0,0,0,0.971991171063,'20050831104120',20,430);
INSERT INTO `page` VALUES (21,8,'Addgroup','sysop',0,0,0,0.116232056891,'20050831104120',21,9);
INSERT INTO `page` VALUES (22,8,'Addgrouplogentry','sysop',0,0,0,0.673335707926,'20050831104120',22,14);
INSERT INTO `page` VALUES (23,8,'Addsection','sysop',0,0,0,0.005117644885,'20050831104120',23,1);
INSERT INTO `page` VALUES (24,8,'Administrators','sysop',0,0,0,0.650796983516,'20050831104120',24,23);
INSERT INTO `page` VALUES (25,8,'Allarticles','sysop',0,0,0,0.554506613182,'20050831104120',25,12);
INSERT INTO `page` VALUES (26,8,'Allinnamespace','sysop',0,0,0,0.167767037349,'20050831104120',26,24);
INSERT INTO `page` VALUES (27,8,'Alllogstext','sysop',0,0,0,0.571741421986,'20050831104120',27,166);
INSERT INTO `page` VALUES (28,8,'Allmessages','sysop',0,0,0,0.648736691318,'20050831104120',28,24);
INSERT INTO `page` VALUES (29,8,'Allmessagescurrent','sysop',0,0,0,0.278667705511,'20050831104120',29,12);
INSERT INTO `page` VALUES (30,8,'Allmessagesdefault','sysop',0,0,0,0.06985940512,'20050831104120',30,12);
INSERT INTO `page` VALUES (31,8,'Allmessagesname','sysop',0,0,0,0.232449776327,'20050831104120',31,4);
INSERT INTO `page` VALUES (32,8,'AllmessagesnotsupportedDB','sysop',0,0,0,0.414403980208,'20050831104120',32,71);
INSERT INTO `page` VALUES (33,8,'AllmessagesnotsupportedUI','sysop',0,0,0,0.741721907765,'20050831104120',33,95);
INSERT INTO `page` VALUES (34,8,'Allmessagestext','sysop',0,0,0,0.363523118002,'20050831104120',34,71);
INSERT INTO `page` VALUES (35,8,'Allnonarticles','sysop',0,0,0,0.226821529961,'20050831104120',35,16);
INSERT INTO `page` VALUES (36,8,'Allnotinnamespace','sysop',0,0,0,0.467760908051,'20050831104120',36,31);
INSERT INTO `page` VALUES (37,8,'Allpages','sysop',0,0,0,0.169332350871,'20050831104120',37,12);
INSERT INTO `page` VALUES (38,8,'Allpagesfrom','sysop',0,0,0,0.278419082291,'20050831104120',38,26);
INSERT INTO `page` VALUES (39,8,'Allpagesnext','sysop',0,0,0,0.336129749741,'20050831104120',39,4);
INSERT INTO `page` VALUES (40,8,'Allpagesprev','sysop',0,0,0,0.982529571412,'20050831104120',40,8);
INSERT INTO `page` VALUES (41,8,'Allpagessubmit','sysop',0,0,0,0.542902056625,'20050831104120',41,2);
INSERT INTO `page` VALUES (42,8,'Alphaindexline','sysop',0,0,0,0.002526672474,'20050831104120',42,9);
INSERT INTO `page` VALUES (43,8,'Already_bureaucrat','sysop',0,0,0,0.030509857605,'20050831104120',43,33);
INSERT INTO `page` VALUES (44,8,'Already_steward','sysop',0,0,0,0.259879523752,'20050831104120',44,30);
INSERT INTO `page` VALUES (45,8,'Already_sysop','sysop',0,0,0,0.065589056364,'20050831104120',45,37);
INSERT INTO `page` VALUES (46,8,'Alreadyloggedin','sysop',0,0,0,0.187556227659,'20050831104120',46,77);
INSERT INTO `page` VALUES (47,8,'Alreadyrolled','sysop',0,0,0,0.422467290013,'20050831104120',47,262);
INSERT INTO `page` VALUES (48,8,'Ancientpages','sysop',0,0,0,0.637865759152,'20050831104120',48,16);
INSERT INTO `page` VALUES (49,8,'And','sysop',0,0,0,0.598106523969,'20050831104120',49,3);
INSERT INTO `page` VALUES (50,8,'Anontalk','sysop',0,0,0,0.430547077271,'20050831104120',50,26);
INSERT INTO `page` VALUES (51,8,'Anontalkpagetext','sysop',0,0,0,0.883981787039,'20050831104120',51,423);
INSERT INTO `page` VALUES (52,8,'Anonymous','sysop',0,0,0,0.39420309543,'20050831104120',52,33);
INSERT INTO `page` VALUES (53,8,'Apr','sysop',0,0,0,0.519337476803,'20050831104120',53,3);
INSERT INTO `page` VALUES (54,8,'April','sysop',0,0,0,0.707764517366,'20050831104120',54,5);
INSERT INTO `page` VALUES (55,8,'Article','sysop',0,0,0,0.348931956436,'20050831104120',55,7);
INSERT INTO `page` VALUES (56,8,'Articleexists','sysop',0,0,0,0.148555713654,'20050831104120',56,88);
INSERT INTO `page` VALUES (57,8,'Articlepage','sysop',0,0,0,0.099751322202,'20050831104120',57,7);
INSERT INTO `page` VALUES (58,8,'Aug','sysop',0,0,0,0.867832760549,'20050831104120',58,3);
INSERT INTO `page` VALUES (59,8,'August','sysop',0,0,0,0.244353098755,'20050831104120',59,6);
INSERT INTO `page` VALUES (60,8,'Autoblocker','sysop',0,0,0,0.965762144435,'20050831104120',60,80);
INSERT INTO `page` VALUES (61,8,'Badaccess','sysop',0,0,0,0.316425730361,'20050831104120',61,16);
INSERT INTO `page` VALUES (62,8,'Badaccesstext','sysop',0,0,0,0.320431241235,'20050831104120',62,92);
INSERT INTO `page` VALUES (63,8,'Badarticleerror','sysop',0,0,0,0.046538023757,'20050831104120',63,61);
INSERT INTO `page` VALUES (64,8,'Badfilename','sysop',0,0,0,0.847477434338,'20050831104120',64,37);
INSERT INTO `page` VALUES (65,8,'Badfiletype','sysop',0,0,0,0.719418888698,'20050831104120',65,39);
INSERT INTO `page` VALUES (66,8,'Badipaddress','sysop',0,0,0,0.257840866457,'20050831104120',66,39);
INSERT INTO `page` VALUES (67,8,'Badquery','sysop',0,0,0,0.323276721635,'20050831104120',67,19);
INSERT INTO `page` VALUES (68,8,'Badquerytext','sysop',0,0,0,0.570419740631,'20050831104120',68,319);
INSERT INTO `page` VALUES (69,8,'Badretype','sysop',0,0,0,0.164295949192,'20050831104120',69,46);
INSERT INTO `page` VALUES (70,8,'Badtitle','sysop',0,0,0,0.798004212571,'20050831104120',70,17);
INSERT INTO `page` VALUES (71,8,'Badtitletext','sysop',0,0,0,0.588614352794,'20050831104120',71,110);
INSERT INTO `page` VALUES (72,8,'Blanknamespace','sysop',0,0,0,0.882333737707,'20050831104120',72,8);
INSERT INTO `page` VALUES (73,8,'Blockedtext','sysop',0,0,0,0.532492617771,'20050831104120',73,178);
INSERT INTO `page` VALUES (74,8,'Blockedtitle','sysop',0,0,0,0.805184519255,'20050831104120',74,22);
INSERT INTO `page` VALUES (75,8,'Blockip','sysop',0,0,0,0.411165482216,'20050831104120',75,21);
INSERT INTO `page` VALUES (76,8,'Blockipsuccesssub','sysop',0,0,0,0.614139290222,'20050831104120',76,20);
INSERT INTO `page` VALUES (77,8,'Blockipsuccesstext','sysop',0,0,0,0.971754922314,'20050831104120',77,114);
INSERT INTO `page` VALUES (78,8,'Blockiptext','sysop',0,0,0,0.761570887284,'20050831104120',78,233);
INSERT INTO `page` VALUES (79,8,'Blocklink','sysop',0,0,0,0.831758333868,'20050831104120',79,10);
INSERT INTO `page` VALUES (80,8,'Blocklistline','sysop',0,0,0,0.79949238396,'20050831104120',80,20);
INSERT INTO `page` VALUES (81,8,'Blocklogentry','sysop',0,0,0,0.099800656483,'20050831104120',81,96);
INSERT INTO `page` VALUES (82,8,'Blocklogpage','sysop',0,0,0,0.633197688638,'20050831104120',82,25);
INSERT INTO `page` VALUES (83,8,'Blocklogtext','sysop',0,0,0,0.135185087222,'20050831104120',83,208);
INSERT INTO `page` VALUES (84,8,'Blockpheading','sysop',0,0,0,0.312651738576,'20050831104120',84,11);
INSERT INTO `page` VALUES (85,8,'Bold_sample','sysop',0,0,0,0.507729982223,'20050831104120',85,11);
INSERT INTO `page` VALUES (86,8,'Bold_tip','sysop',0,0,0,0.449086680804,'20050831104120',86,11);
INSERT INTO `page` VALUES (87,8,'Booksources','sysop',0,0,0,0.061427955097,'20050831104120',87,14);
INSERT INTO `page` VALUES (88,8,'Booksourcetext','sysop',0,0,0,0.93197863249,'20050831104120',88,248);
INSERT INTO `page` VALUES (89,8,'Brokenredirects','sysop',0,0,0,0.664476700952,'20050831104120',89,17);
INSERT INTO `page` VALUES (90,8,'Brokenredirectstext','sysop',0,0,0,0.994697131998,'20050831104120',90,74);
INSERT INTO `page` VALUES (91,8,'Bugreports','sysop',0,0,0,0.348203346493,'20050831104120',91,7);
INSERT INTO `page` VALUES (92,8,'Bugreportspage','sysop',0,0,0,0.697337846188,'20050831104120',92,15);
INSERT INTO `page` VALUES (93,8,'Bureaucratlog','sysop',0,0,0,0.342122063985,'20050831104120',93,14);
INSERT INTO `page` VALUES (94,8,'Bureaucratlogentry','sysop',0,0,0,0.218718444281,'20050831104120',94,42);
INSERT INTO `page` VALUES (95,8,'Bydate','sysop',0,0,0,0.643079404386,'20050831104120',95,10);
INSERT INTO `page` VALUES (96,8,'Byname','sysop',0,0,0,0.468855892771,'20050831104120',96,9);
INSERT INTO `page` VALUES (97,8,'Bysize','sysop',0,0,0,0.378515190558,'20050831104120',97,12);
INSERT INTO `page` VALUES (98,8,'Cachederror','sysop',0,0,0,0.631120132245,'20050831104120',98,76);
INSERT INTO `page` VALUES (99,8,'Cancel','sysop',0,0,0,0.53017879136,'20050831104120',99,7);
INSERT INTO `page` VALUES (100,8,'Cannotdelete','sysop',0,0,0,0.897092690602,'20050831104120',100,113);
INSERT INTO `page` VALUES (101,8,'Cantrollback','sysop',0,0,0,0.801821768796,'20050831104120',101,82);
INSERT INTO `page` VALUES (102,8,'Categories','sysop',0,0,0,0.750420989857,'20050831104120',102,16);
INSERT INTO `page` VALUES (103,8,'Categoriespagetext','sysop',0,0,0,0.87162015391,'20050831104120',103,43);
INSERT INTO `page` VALUES (104,8,'Category','sysop',0,0,0,0.396496117931,'20050831104120',104,9);
INSERT INTO `page` VALUES (105,8,'Category_header','sysop',0,0,0,0.667293409193,'20050831104121',105,29);
INSERT INTO `page` VALUES (106,8,'Categoryarticlecount','sysop',0,0,0,0.023523382553,'20050831104121',106,40);
INSERT INTO `page` VALUES (107,8,'Categoryarticlecount1','sysop',0,0,0,0.914342584173,'20050831104121',107,37);
INSERT INTO `page` VALUES (108,8,'Changed','sysop',0,0,0,0.341517615344,'20050831104121',108,9);
INSERT INTO `page` VALUES (109,8,'Changegrouplogentry','sysop',0,0,0,0.258316135547,'20050831104121',109,16);
INSERT INTO `page` VALUES (110,8,'Changepassword','sysop',0,0,0,0.411701078028,'20050831104121',110,16);
INSERT INTO `page` VALUES (111,8,'Changes','sysop',0,0,0,0.329793063011,'20050831104121',111,11);
INSERT INTO `page` VALUES (112,8,'Clearyourcache','sysop',0,0,0,0.109243935347,'20050831104121',112,239);
INSERT INTO `page` VALUES (113,8,'Columns','sysop',0,0,0,0.564324143496,'20050831104121',113,7);
INSERT INTO `page` VALUES (114,8,'Compareselectedversions','sysop',0,0,0,0.660598616415,'20050831104121',114,31);
INSERT INTO `page` VALUES (115,8,'Confirm','sysop',0,0,0,0.741470555294,'20050831104121',115,11);
INSERT INTO `page` VALUES (116,8,'Confirmdelete','sysop',0,0,0,0.803403446764,'20050831104121',116,21);
INSERT INTO `page` VALUES (117,8,'Confirmdeletetext','sysop',0,0,0,0.460670227315,'20050831104121',117,294);
INSERT INTO `page` VALUES (118,8,'Confirmemail','sysop',0,0,0,0.385506501139,'20050831104121',118,47);
INSERT INTO `page` VALUES (119,8,'Confirmemail_body','sysop',0,0,0,0.108073828587,'20050831104121',119,423);
INSERT INTO `page` VALUES (120,8,'Confirmemail_error','sysop',0,0,0,0.073279167962,'20050831104121',120,61);
INSERT INTO `page` VALUES (121,8,'Confirmemail_invalid','sysop',0,0,0,0.420348455563,'20050831104121',121,88);
INSERT INTO `page` VALUES (122,8,'Confirmemail_loggedin','sysop',0,0,0,0.227548845465,'20050831104121',122,38);
INSERT INTO `page` VALUES (123,8,'Confirmemail_send','sysop',0,0,0,0.955966888582,'20050831104121',123,49);
INSERT INTO `page` VALUES (124,8,'Confirmemail_sendfailed','sysop',0,0,0,0.610056334769,'20050831104121',124,101);
INSERT INTO `page` VALUES (125,8,'Confirmemail_sent','sysop',0,0,0,0.855217627581,'20050831104121',125,59);
INSERT INTO `page` VALUES (126,8,'Confirmemail_subject','sysop',0,0,0,0.579619276544,'20050831104121',126,60);
INSERT INTO `page` VALUES (127,8,'Confirmemail_success','sysop',0,0,0,0.113669156668,'20050831104121',127,70);
INSERT INTO `page` VALUES (128,8,'Confirmemail_text','sysop',0,0,0,0.729895362871,'20050831104121',128,332);
INSERT INTO `page` VALUES (129,8,'Confirmprotect','sysop',0,0,0,0.722363299892,'20050831104121',129,20);
INSERT INTO `page` VALUES (130,8,'Confirmprotecttext','sysop',0,0,0,0.080599346703,'20050831104121',130,44);
INSERT INTO `page` VALUES (131,8,'Confirmunprotect','sysop',0,0,0,0.38411611786,'20050831104121',131,34);
INSERT INTO `page` VALUES (132,8,'Confirmunprotecttext','sysop',0,0,0,0.198384593155,'20050831104121',132,55);
INSERT INTO `page` VALUES (133,8,'Contextchars','sysop',0,0,0,0.270566504163,'20050831104121',133,17);
INSERT INTO `page` VALUES (134,8,'Contextlines','sysop',0,0,0,0.762950303167,'20050831104121',134,18);
INSERT INTO `page` VALUES (135,8,'Contribs-showhideminor','sysop',0,0,0,0.978505830971,'20050831104121',135,14);
INSERT INTO `page` VALUES (136,8,'Contribslink','sysop',0,0,0,0.103035135548,'20050831104121',136,9);
INSERT INTO `page` VALUES (137,8,'Contribsub','sysop',0,0,0,0.670134334954,'20050831104121',137,7);
INSERT INTO `page` VALUES (138,8,'Contributions','sysop',0,0,0,0.044344775383,'20050831104121',138,17);
INSERT INTO `page` VALUES (139,8,'Contributionsall','sysop',0,0,0,0.064275382341,'20050831104121',139,3);
INSERT INTO `page` VALUES (140,8,'Copyright','sysop',0,0,0,0.82557784219,'20050831104121',140,35);
INSERT INTO `page` VALUES (141,8,'Copyrightpage','sysop',0,0,0,0.727108628521,'20050831104121',141,17);
INSERT INTO `page` VALUES (142,8,'Copyrightpagename','sysop',0,0,0,0.781635815929,'20050831104121',142,22);
INSERT INTO `page` VALUES (143,8,'Copyrightwarning','sysop',0,0,0,0.500598312749,'20050831104121',143,843);
INSERT INTO `page` VALUES (144,8,'Copyrightwarning2','sysop',0,0,0,0.247721748128,'20050831104121',144,403);
INSERT INTO `page` VALUES (145,8,'Couldntremove','sysop',0,0,0,0.876744097167,'20050831104121',145,47);
INSERT INTO `page` VALUES (146,8,'Createaccount','sysop',0,0,0,0.421664035351,'20050831104121',146,27);
INSERT INTO `page` VALUES (147,8,'Createaccountmail','sysop',0,0,0,0.618157348943,'20050831104121',147,11);
INSERT INTO `page` VALUES (148,8,'Createaccountpheading','sysop',0,0,0,0.641803917269,'20050831104121',148,19);
INSERT INTO `page` VALUES (149,8,'Createarticle','sysop',0,0,0,0.992141121288,'20050831104121',149,14);
INSERT INTO `page` VALUES (150,8,'Created','sysop',0,0,0,0.566326230515,'20050831104121',150,12);
INSERT INTO `page` VALUES (151,8,'Creditspage','sysop',0,0,0,0.188231817651,'20050831104121',151,12);
INSERT INTO `page` VALUES (152,8,'Cur','sysop',0,0,0,0.50120039651,'20050831104121',152,7);
INSERT INTO `page` VALUES (153,8,'Currentevents','sysop',0,0,0,0.339092898769,'20050831104121',153,19);
INSERT INTO `page` VALUES (154,8,'Currentevents-url','sysop',0,0,0,0.428226204738,'20050831104121',154,14);
INSERT INTO `page` VALUES (155,8,'Currentrev','sysop',0,0,0,0.259278401801,'20050831104121',155,16);
INSERT INTO `page` VALUES (156,8,'Currentrevisionlink','sysop',0,0,0,0.717371888464,'20050831104121',156,21);
INSERT INTO `page` VALUES (157,8,'Data','sysop',0,0,0,0.933721298768,'20050831104121',157,4);
INSERT INTO `page` VALUES (158,8,'Databaseerror','sysop',0,0,0,0.586460647443,'20050831104121',158,23);
INSERT INTO `page` VALUES (159,8,'Dateformat','sysop',0,0,0,0.854525427735,'20050831104121',159,12);
INSERT INTO `page` VALUES (160,8,'Dberrortext','sysop',0,0,0,0.032168640074,'20050831104121',160,200);
INSERT INTO `page` VALUES (161,8,'Dberrortextcl','sysop',0,0,0,0.14296929379,'20050831104121',161,169);
INSERT INTO `page` VALUES (162,8,'Deadendpages','sysop',0,0,0,0.562860628561,'20050831104121',162,17);
INSERT INTO `page` VALUES (163,8,'Debug','sysop',0,0,0,0.190900888594,'20050831104121',163,5);
INSERT INTO `page` VALUES (164,8,'Dec','sysop',0,0,0,0.661817509226,'20050831104121',164,3);
INSERT INTO `page` VALUES (165,8,'December','sysop',0,0,0,0.692138271866,'20050831104121',165,8);
INSERT INTO `page` VALUES (166,8,'Default','sysop',0,0,0,0.138578991795,'20050831104121',166,7);
INSERT INTO `page` VALUES (167,8,'Defaultns','sysop',0,0,0,0.127247702452,'20050831104121',167,60);
INSERT INTO `page` VALUES (168,8,'Defemailsubject','sysop',0,0,0,0.459093317328,'20050831104121',168,19);
INSERT INTO `page` VALUES (169,8,'Delete','sysop',0,0,0,0.107170797884,'20050831104121',169,8);
INSERT INTO `page` VALUES (170,8,'Delete_and_move','sysop',0,0,0,0.93886387356,'20050831104121',170,15);
INSERT INTO `page` VALUES (171,8,'Delete_and_move_reason','sysop',0,0,0,0.554739987538,'20050831104121',171,28);
INSERT INTO `page` VALUES (172,8,'Delete_and_move_text','sysop',0,0,0,0.53910994475,'20050831104121',172,122);
INSERT INTO `page` VALUES (173,8,'Deletecomment','sysop',0,0,0,0.807617226469,'20050831104121',173,19);
INSERT INTO `page` VALUES (174,8,'Deletedarticle','sysop',0,0,0,0.453496785495,'20050831104121',174,14);
INSERT INTO `page` VALUES (175,8,'Deletedrev','sysop',0,0,0,0.785562675954,'20050831104121',175,9);
INSERT INTO `page` VALUES (176,8,'Deletedrevision','sysop',0,0,0,0.044038015098,'20050831104121',176,24);
INSERT INTO `page` VALUES (177,8,'Deletedtext','sysop',0,0,0,0.619973620104,'20050831104121',177,74);
INSERT INTO `page` VALUES (178,8,'Deleteimg','sysop',0,0,0,0.572281276016,'20050831104121',178,8);
INSERT INTO `page` VALUES (179,8,'Deleteimgcompletely','sysop',0,0,0,0.991762645643,'20050831104121',179,8);
INSERT INTO `page` VALUES (180,8,'Deletepage','sysop',0,0,0,0.72439107576,'20050831104121',180,14);
INSERT INTO `page` VALUES (181,8,'Deletepheading','sysop',0,0,0,0.501976377166,'20050831104121',181,12);
INSERT INTO `page` VALUES (182,8,'Deletesub','sysop',0,0,0,0.282220622183,'20050831104121',182,14);
INSERT INTO `page` VALUES (183,8,'Deletethispage','sysop',0,0,0,0.43226431721,'20050831104121',183,20);
INSERT INTO `page` VALUES (184,8,'Deletionlog','sysop',0,0,0,0.854729401688,'20050831104121',184,14);
INSERT INTO `page` VALUES (185,8,'Dellogpage','sysop',0,0,0,0.404928710363,'20050831104121',185,14);
INSERT INTO `page` VALUES (186,8,'Dellogpagetext','sysop',0,0,0,0.079700855277,'20050831104121',186,61);
INSERT INTO `page` VALUES (187,8,'Destfilename','sysop',0,0,0,0.802489168524,'20050831104121',187,20);
INSERT INTO `page` VALUES (188,8,'Developertext','sysop',0,0,0,0.300641175629,'20050831104121',188,121);
INSERT INTO `page` VALUES (189,8,'Developertitle','sysop',0,0,0,0.377061590894,'20050831104121',189,27);
INSERT INTO `page` VALUES (190,8,'Diff','sysop',0,0,0,0.029869504331,'20050831104121',190,11);
INSERT INTO `page` VALUES (191,8,'Difference','sysop',0,0,0,0.880459372171,'20050831104121',191,32);
INSERT INTO `page` VALUES (192,8,'Disambiguations','sysop',0,0,0,0.353266177039,'20050831104121',192,23);
INSERT INTO `page` VALUES (193,8,'Disambiguationspage','sysop',0,0,0,0.071534223446,'20050831104121',193,24);
INSERT INTO `page` VALUES (194,8,'Disambiguationstext','sysop',0,0,0,0.118172538385,'20050831104121',194,300);
INSERT INTO `page` VALUES (195,8,'Disclaimerpage','sysop',0,0,0,0.658992529434,'20050831104121',195,27);
INSERT INTO `page` VALUES (196,8,'Disclaimers','sysop',0,0,0,0.914517725715,'20050831104121',196,18);
INSERT INTO `page` VALUES (197,8,'Doubleredirects','sysop',0,0,0,0.300445809685,'20050831104121',197,18);
INSERT INTO `page` VALUES (198,8,'Doubleredirectstext','sysop',0,0,0,0.713145291609,'20050831104121',198,221);
INSERT INTO `page` VALUES (199,8,'Eauthentsent','sysop',0,0,0,0.096121478247,'20050831104121',199,214);
INSERT INTO `page` VALUES (200,8,'Edit','sysop',0,0,0,0.116782173099,'20050831104121',200,10);
INSERT INTO `page` VALUES (201,8,'Edit-externally','sysop',0,0,0,0.006729344181,'20050831104121',201,50);
INSERT INTO `page` VALUES (202,8,'Edit-externally-help','sysop',0,0,0,0.184060536726,'20050831104121',202,114);
INSERT INTO `page` VALUES (203,8,'Editcomment','sysop',0,0,0,0.220316309524,'20050831104121',203,41);
INSERT INTO `page` VALUES (204,8,'Editconflict','sysop',0,0,0,0.734670951835,'20050831104121',204,25);
INSERT INTO `page` VALUES (205,8,'Editcurrent','sysop',0,0,0,0.593108357337,'20050831104121',205,47);
INSERT INTO `page` VALUES (206,8,'Editgroup','sysop',0,0,0,0.711926202958,'20050831104121',206,10);
INSERT INTO `page` VALUES (207,8,'Edithelp','sysop',0,0,0,0.946208236581,'20050831104121',207,17);
INSERT INTO `page` VALUES (208,8,'Edithelppage','sysop',0,0,0,0.723486162326,'20050831104121',208,25);
INSERT INTO `page` VALUES (209,8,'Editing','sysop',0,0,0,0.344114427052,'20050831104121',209,17);
INSERT INTO `page` VALUES (210,8,'Editingcomment','sysop',0,0,0,0.801496280975,'20050831104121',210,29);
INSERT INTO `page` VALUES (211,8,'Editingold','sysop',0,0,0,0.919113115747,'20050831104121',211,141);
INSERT INTO `page` VALUES (212,8,'Editingsection','sysop',0,0,0,0.581451212352,'20050831104121',212,26);
INSERT INTO `page` VALUES (213,8,'Editsection','sysop',0,0,0,0.744917495001,'20050831104121',213,10);
INSERT INTO `page` VALUES (214,8,'Editthispage','sysop',0,0,0,0.687196453267,'20050831104121',214,16);
INSERT INTO `page` VALUES (215,8,'Editusergroup','sysop',0,0,0,0.96159415297,'20050831104121',215,16);
INSERT INTO `page` VALUES (216,8,'Email','sysop',0,0,0,0.361564745172,'20050831104121',216,5);
INSERT INTO `page` VALUES (217,8,'Emailauthenticated','sysop',0,0,0,0.595798671632,'20050831104121',217,48);
INSERT INTO `page` VALUES (218,8,'Emailconfirmlink','sysop',0,0,0,0.380038873798,'20050831104121',218,46);
INSERT INTO `page` VALUES (219,8,'Emailflag','sysop',0,0,0,0.47717532068,'20050831104121',219,43);
INSERT INTO `page` VALUES (220,8,'Emailforlost','sysop',0,0,0,0.608167117384,'20050831104121',220,100);
INSERT INTO `page` VALUES (221,8,'Emailfrom','sysop',0,0,0,0.503920730075,'20050831104121',221,3);
INSERT INTO `page` VALUES (222,8,'Emailmessage','sysop',0,0,0,0.434441074518,'20050831104121',222,9);
INSERT INTO `page` VALUES (223,8,'Emailnotauthenticated','sysop',0,0,0,0.963745308909,'20050831104121',223,336);
INSERT INTO `page` VALUES (224,8,'Emailpage','sysop',0,0,0,0.587848049042,'20050831104121',224,18);
INSERT INTO `page` VALUES (225,8,'Emailpagetext','sysop',0,0,0,0.124295637544,'20050831104121',225,247);
INSERT INTO `page` VALUES (226,8,'Emailsend','sysop',0,0,0,0.437219812172,'20050831104121',226,6);
INSERT INTO `page` VALUES (227,8,'Emailsent','sysop',0,0,0,0.017748571381,'20050831104121',227,17);
INSERT INTO `page` VALUES (228,8,'Emailsenttext','sysop',0,0,0,0.32518085354,'20050831104121',228,29);
INSERT INTO `page` VALUES (229,8,'Emailsubject','sysop',0,0,0,0.356879719619,'20050831104121',229,7);
INSERT INTO `page` VALUES (230,8,'Emailto','sysop',0,0,0,0.592084107456,'20050831104121',230,2);
INSERT INTO `page` VALUES (231,8,'Emailuser','sysop',0,0,0,0.080428288868,'20050831104121',231,25);
INSERT INTO `page` VALUES (232,8,'Emptyfile','sysop',0,0,0,0.128121912032,'20050831104121',232,151);
INSERT INTO `page` VALUES (233,8,'Enotif_body','sysop',0,0,0,0.141870817013,'20050831104121',233,691);
INSERT INTO `page` VALUES (234,8,'Enotif_lastvisited','sysop',0,0,0,0.421065841125,'20050831104121',234,42);
INSERT INTO `page` VALUES (235,8,'Enotif_mailer','sysop',0,0,0,0.32168539856,'20050831104121',235,43);
INSERT INTO `page` VALUES (236,8,'Enotif_newpagetext','sysop',0,0,0,0.364763901523,'20050831104121',236,25);
INSERT INTO `page` VALUES (237,8,'Enotif_reset','sysop',0,0,0,0.699878175288,'20050831104121',237,80);
INSERT INTO `page` VALUES (238,8,'Enotif_subject','sysop',0,0,0,0.057731897776,'20050831104121',238,73);
INSERT INTO `page` VALUES (239,8,'Enterlockreason','sysop',0,0,0,0.884240168773,'20050831104121',239,124);
INSERT INTO `page` VALUES (240,8,'Error','sysop',0,0,0,0.048901460609,'20050831104121',240,6);
INSERT INTO `page` VALUES (241,8,'Errorpagetitle','sysop',0,0,0,0.224698041148,'20050831104121',241,6);
INSERT INTO `page` VALUES (242,8,'Exbeforeblank','sysop',0,0,0,0.492930515987,'20050831104121',242,37);
INSERT INTO `page` VALUES (243,8,'Exblank','sysop',0,0,0,0.930436051557,'20050831104121',243,14);
INSERT INTO `page` VALUES (244,8,'Excontent','sysop',0,0,0,0.083581572248,'20050831104121',244,18);
INSERT INTO `page` VALUES (245,8,'Excontentauthor','sysop',0,0,0,0.518221663238,'20050831104121',245,53);
INSERT INTO `page` VALUES (246,8,'Exif-aperturevalue','sysop',0,0,0,0.192679351333,'20050831104121',246,11);
INSERT INTO `page` VALUES (247,8,'Exif-artist','sysop',0,0,0,0.759847178891,'20050831104121',247,8);
INSERT INTO `page` VALUES (248,8,'Exif-bitspersample','sysop',0,0,0,0.545159395366,'20050831104121',248,18);
INSERT INTO `page` VALUES (249,8,'Exif-brightnessvalue','sysop',0,0,0,0.019807091388,'20050831104121',249,16);
INSERT INTO `page` VALUES (250,8,'Exif-cfapattern','sysop',0,0,0,0.993420845563,'20050831104121',250,11);
INSERT INTO `page` VALUES (251,8,'Exif-colorspace','sysop',0,0,0,0.054557683833,'20050831104121',251,8);
INSERT INTO `page` VALUES (252,8,'Exif-colorspace-1','sysop',0,0,0,0.766110742675,'20050831104121',252,4);
INSERT INTO `page` VALUES (253,8,'Exif-colorspace-ffff.h','sysop',0,0,0,0.283127918944,'20050831104121',253,6);
INSERT INTO `page` VALUES (254,8,'Exif-componentsconfiguration','sysop',0,0,0,0.306652998713,'20050831104121',254,25);
INSERT INTO `page` VALUES (255,8,'Exif-componentsconfiguration-0','sysop',0,0,0,0.211742168355,'20050831104121',255,14);
INSERT INTO `page` VALUES (256,8,'Exif-componentsconfiguration-1','sysop',0,0,0,0.842895822787,'20050831104121',256,1);
INSERT INTO `page` VALUES (257,8,'Exif-componentsconfiguration-2','sysop',0,0,0,0.453404935471,'20050831104121',257,2);
INSERT INTO `page` VALUES (258,8,'Exif-componentsconfiguration-3','sysop',0,0,0,0.286943234391,'20050831104121',258,2);
INSERT INTO `page` VALUES (259,8,'Exif-componentsconfiguration-4','sysop',0,0,0,0.373647014425,'20050831104121',259,1);
INSERT INTO `page` VALUES (260,8,'Exif-componentsconfiguration-5','sysop',0,0,0,0.905858989664,'20050831104121',260,1);
INSERT INTO `page` VALUES (261,8,'Exif-componentsconfiguration-6','sysop',0,0,0,0.353631246222,'20050831104121',261,1);
INSERT INTO `page` VALUES (262,8,'Exif-compressedbitsperpixel','sysop',0,0,0,0.277130969361,'20050831104121',262,22);
INSERT INTO `page` VALUES (263,8,'Exif-compression','sysop',0,0,0,0.182162119401,'20050831104121',263,18);
INSERT INTO `page` VALUES (264,8,'Exif-compression-1','sysop',0,0,0,0.488080116803,'20050831104121',264,12);
INSERT INTO `page` VALUES (265,8,'Exif-compression-6','sysop',0,0,0,0.665628596798,'20050831104121',265,4);
INSERT INTO `page` VALUES (266,8,'Exif-contrast','sysop',0,0,0,0.897636002701,'20050831104121',266,8);
INSERT INTO `page` VALUES (267,8,'Exif-contrast-0','sysop',0,0,0,0.698125678693,'20050831104121',267,6);
INSERT INTO `page` VALUES (268,8,'Exif-contrast-1','sysop',0,0,0,0.596678130782,'20050831104121',268,4);
INSERT INTO `page` VALUES (269,8,'Exif-contrast-2','sysop',0,0,0,0.327888533291,'20050831104121',269,4);
INSERT INTO `page` VALUES (270,8,'Exif-copyright','sysop',0,0,0,0.131952949713,'20050831104121',270,9);
INSERT INTO `page` VALUES (271,8,'Exif-customrendered','sysop',0,0,0,0.358398318893,'20050831104121',271,23);
INSERT INTO `page` VALUES (272,8,'Exif-customrendered-0','sysop',0,0,0,0.584446822401,'20050831104121',272,14);
INSERT INTO `page` VALUES (273,8,'Exif-customrendered-1','sysop',0,0,0,0.494356185872,'20050831104121',273,14);
INSERT INTO `page` VALUES (274,8,'Exif-datetime','sysop',0,0,0,0.150329427406,'20050831104121',274,25);
INSERT INTO `page` VALUES (275,8,'Exif-datetimedigitized','sysop',0,0,0,0.816939008417,'20050831104121',275,27);
INSERT INTO `page` VALUES (276,8,'Exif-datetimeoriginal','sysop',0,0,0,0.819435989233,'20050831104121',276,32);
INSERT INTO `page` VALUES (277,8,'Exif-devicesettingdescription','sysop',0,0,0,0.512840457441,'20050831104121',277,27);
INSERT INTO `page` VALUES (278,8,'Exif-digitalzoomratio','sysop',0,0,0,0.770402000399,'20050831104121',278,18);
INSERT INTO `page` VALUES (279,8,'Exif-exifversion','sysop',0,0,0,0.864262910927,'20050831104121',279,12);
INSERT INTO `page` VALUES (280,8,'Exif-exposurebiasvalue','sysop',0,0,0,0.394892855698,'20050831104121',280,18);
INSERT INTO `page` VALUES (281,8,'Exif-exposureindex','sysop',0,0,0,0.551065140924,'20050831104121',281,14);
INSERT INTO `page` VALUES (282,8,'Exif-exposuremode','sysop',0,0,0,0.608284839378,'20050831104121',282,16);
INSERT INTO `page` VALUES (283,8,'Exif-exposuremode-0','sysop',0,0,0,0.957239252239,'20050831104121',283,13);
INSERT INTO `page` VALUES (284,8,'Exif-exposuremode-1','sysop',0,0,0,0.853308024067,'20050831104121',284,15);
INSERT INTO `page` VALUES (285,8,'Exif-exposuremode-2','sysop',0,0,0,0.452055372252,'20050831104121',285,12);
INSERT INTO `page` VALUES (286,8,'Exif-exposureprogram','sysop',0,0,0,0.848116116407,'20050831104121',286,19);
INSERT INTO `page` VALUES (287,8,'Exif-exposureprogram-0','sysop',0,0,0,0.51433893927,'20050831104121',287,11);
INSERT INTO `page` VALUES (288,8,'Exif-exposureprogram-1','sysop',0,0,0,0.018535015443,'20050831104121',288,6);
INSERT INTO `page` VALUES (289,8,'Exif-exposureprogram-2','sysop',0,0,0,0.92309786875,'20050831104121',289,14);
INSERT INTO `page` VALUES (290,8,'Exif-exposureprogram-3','sysop',0,0,0,0.806051880032,'20050831104121',290,17);
INSERT INTO `page` VALUES (291,8,'Exif-exposureprogram-4','sysop',0,0,0,0.80612413123,'20050831104121',291,16);
INSERT INTO `page` VALUES (292,8,'Exif-exposureprogram-5','sysop',0,0,0,0.539316392095,'20050831104121',292,47);
INSERT INTO `page` VALUES (293,8,'Exif-exposureprogram-6','sysop',0,0,0,0.582579049689,'20050831104121',293,49);
INSERT INTO `page` VALUES (294,8,'Exif-exposureprogram-7','sysop',0,0,0,0.510187518044,'20050831104121',294,67);
INSERT INTO `page` VALUES (295,8,'Exif-exposureprogram-8','sysop',0,0,0,0.933515930522,'20050831104121',295,66);
INSERT INTO `page` VALUES (296,8,'Exif-exposuretime','sysop',0,0,0,0.700134581959,'20050831104122',296,16);
INSERT INTO `page` VALUES (297,8,'Exif-filesource','sysop',0,0,0,0.890158472024,'20050831104122',297,11);
INSERT INTO `page` VALUES (298,8,'Exif-filesource-3','sysop',0,0,0,0.176220228921,'20050831104122',298,3);
INSERT INTO `page` VALUES (299,8,'Exif-flash','sysop',0,0,0,0.447062880681,'20050831104122',299,5);
INSERT INTO `page` VALUES (300,8,'Exif-flashenergy','sysop',0,0,0,0.284622794093,'20050831104122',300,12);
INSERT INTO `page` VALUES (301,8,'Exif-flashpixversion','sysop',0,0,0,0.994369615937,'20050831104122',301,26);
INSERT INTO `page` VALUES (302,8,'Exif-fnumber','sysop',0,0,0,0.140041552171,'20050831104122',302,6);
INSERT INTO `page` VALUES (303,8,'Exif-focallength','sysop',0,0,0,0.262372436059,'20050831104122',303,10);
INSERT INTO `page` VALUES (304,8,'Exif-focallengthin35mmfilm','sysop',0,0,0,0.315225556703,'20050831104122',304,33);
INSERT INTO `page` VALUES (305,8,'Exif-focalplaneresolutionunit','sysop',0,0,0,0.571212944818,'20050831104122',305,27);
INSERT INTO `page` VALUES (306,8,'Exif-focalplaneresolutionunit-2','sysop',0,0,0,0.502196720653,'20050831104122',306,6);
INSERT INTO `page` VALUES (307,8,'Exif-focalplanexresolution','sysop',0,0,0,0.954322429963,'20050831104122',307,24);
INSERT INTO `page` VALUES (308,8,'Exif-focalplaneyresolution','sysop',0,0,0,0.758975527879,'20050831104122',308,24);
INSERT INTO `page` VALUES (309,8,'Exif-gaincontrol','sysop',0,0,0,0.850888927584,'20050831104122',309,13);
INSERT INTO `page` VALUES (310,8,'Exif-gaincontrol-0','sysop',0,0,0,0.292530034223,'20050831104122',310,4);
INSERT INTO `page` VALUES (311,8,'Exif-gaincontrol-1','sysop',0,0,0,0.953999481805,'20050831104122',311,11);
INSERT INTO `page` VALUES (312,8,'Exif-gaincontrol-2','sysop',0,0,0,0.993023882649,'20050831104122',312,12);
INSERT INTO `page` VALUES (313,8,'Exif-gaincontrol-3','sysop',0,0,0,0.976297689535,'20050831104122',313,13);
INSERT INTO `page` VALUES (314,8,'Exif-gaincontrol-4','sysop',0,0,0,0.285317581768,'20050831104122',314,14);
INSERT INTO `page` VALUES (315,8,'Exif-gpsaltitude','sysop',0,0,0,0.850202698542,'20050831104122',315,8);
INSERT INTO `page` VALUES (316,8,'Exif-gpsaltituderef','sysop',0,0,0,0.766052765028,'20050831104122',316,18);
INSERT INTO `page` VALUES (317,8,'Exif-gpsareainformation','sysop',0,0,0,0.32663750409,'20050831104122',317,16);
INSERT INTO `page` VALUES (318,8,'Exif-gpsdatestamp','sysop',0,0,0,0.072565352239,'20050831104122',318,8);
INSERT INTO `page` VALUES (319,8,'Exif-gpsdestbearing','sysop',0,0,0,0.376403581955,'20050831104122',319,22);
INSERT INTO `page` VALUES (320,8,'Exif-gpsdestbearingref','sysop',0,0,0,0.634267262236,'20050831104122',320,36);
INSERT INTO `page` VALUES (321,8,'Exif-gpsdestdistance','sysop',0,0,0,0.324577940247,'20050831104122',321,23);
INSERT INTO `page` VALUES (322,8,'Exif-gpsdestdistanceref','sysop',0,0,0,0.234544516035,'20050831104122',322,37);
INSERT INTO `page` VALUES (323,8,'Exif-gpsdestlatitude','sysop',0,0,0,0.778096972688,'20050831104122',323,20);
INSERT INTO `page` VALUES (324,8,'Exif-gpsdestlatituderef','sysop',0,0,0,0.344571169856,'20050831104122',324,37);
INSERT INTO `page` VALUES (325,8,'Exif-gpsdestlongitude','sysop',0,0,0,0.779207867327,'20050831104122',325,24);
INSERT INTO `page` VALUES (326,8,'Exif-gpsdestlongituderef','sysop',0,0,0,0.978785894163,'20050831104122',326,38);
INSERT INTO `page` VALUES (327,8,'Exif-gpsdifferential','sysop',0,0,0,0.247263144104,'20050831104122',327,27);
INSERT INTO `page` VALUES (328,8,'Exif-gpsdirection-m','sysop',0,0,0,0.151232658723,'20050831104122',328,18);
INSERT INTO `page` VALUES (329,8,'Exif-gpsdirection-t','sysop',0,0,0,0.911896882037,'20050831104122',329,14);
INSERT INTO `page` VALUES (330,8,'Exif-gpsdop','sysop',0,0,0,0.109070779366,'20050831104122',330,21);
INSERT INTO `page` VALUES (331,8,'Exif-gpsimgdirection','sysop',0,0,0,0.331336591605,'20050831104122',331,18);
INSERT INTO `page` VALUES (332,8,'Exif-gpsimgdirectionref','sysop',0,0,0,0.666246884162,'20050831104122',332,32);
INSERT INTO `page` VALUES (333,8,'Exif-gpslatitude','sysop',0,0,0,0.395428387598,'20050831104122',333,8);
INSERT INTO `page` VALUES (334,8,'Exif-gpslatitude-n','sysop',0,0,0,0.262709461614,'20050831104122',334,14);
INSERT INTO `page` VALUES (335,8,'Exif-gpslatitude-s','sysop',0,0,0,0.242578529511,'20050831104122',335,14);
INSERT INTO `page` VALUES (336,8,'Exif-gpslatituderef','sysop',0,0,0,0.145447500314,'20050831104122',336,23);
INSERT INTO `page` VALUES (337,8,'Exif-gpslongitude','sysop',0,0,0,0.89670589174,'20050831104122',337,9);
INSERT INTO `page` VALUES (338,8,'Exif-gpslongitude-e','sysop',0,0,0,0.11913039261,'20050831104122',338,14);
INSERT INTO `page` VALUES (339,8,'Exif-gpslongitude-w','sysop',0,0,0,0.434970555193,'20050831104122',339,14);
INSERT INTO `page` VALUES (340,8,'Exif-gpslongituderef','sysop',0,0,0,0.260096108356,'20050831104122',340,22);
INSERT INTO `page` VALUES (341,8,'Exif-gpsmapdatum','sysop',0,0,0,0.090017525225,'20050831104122',341,25);
INSERT INTO `page` VALUES (342,8,'Exif-gpsmeasuremode','sysop',0,0,0,0.293808648399,'20050831104122',342,16);
INSERT INTO `page` VALUES (343,8,'Exif-gpsmeasuremode-2','sysop',0,0,0,0.060804714387,'20050831104122',343,25);
INSERT INTO `page` VALUES (344,8,'Exif-gpsmeasuremode-3','sysop',0,0,0,0.39050100422,'20050831104122',344,25);
INSERT INTO `page` VALUES (345,8,'Exif-gpsprocessingmethod','sysop',0,0,0,0.779715887073,'20050831104122',345,29);
INSERT INTO `page` VALUES (346,8,'Exif-gpssatellites','sysop',0,0,0,0.686788249176,'20050831104122',346,31);
INSERT INTO `page` VALUES (347,8,'Exif-gpsspeed','sysop',0,0,0,0.327108766504,'20050831104122',347,21);
INSERT INTO `page` VALUES (348,8,'Exif-gpsspeed-k','sysop',0,0,0,0.967275445543,'20050831104122',348,19);
INSERT INTO `page` VALUES (349,8,'Exif-gpsspeed-m','sysop',0,0,0,0.266719992823,'20050831104122',349,14);
INSERT INTO `page` VALUES (350,8,'Exif-gpsspeed-n','sysop',0,0,0,0.034994934588,'20050831104122',350,5);
INSERT INTO `page` VALUES (351,8,'Exif-gpsspeedref','sysop',0,0,0,0.239945341,'20050831104122',351,10);
INSERT INTO `page` VALUES (352,8,'Exif-gpsstatus','sysop',0,0,0,0.006130234463,'20050831104122',352,15);
INSERT INTO `page` VALUES (353,8,'Exif-gpsstatus-a','sysop',0,0,0,0.873695918934,'20050831104122',353,23);
INSERT INTO `page` VALUES (354,8,'Exif-gpsstatus-v','sysop',0,0,0,0.113737225941,'20050831104122',354,28);
INSERT INTO `page` VALUES (355,8,'Exif-gpstimestamp','sysop',0,0,0,0.803366792789,'20050831104122',355,23);
INSERT INTO `page` VALUES (356,8,'Exif-gpstrack','sysop',0,0,0,0.094391530691,'20050831104122',356,21);
INSERT INTO `page` VALUES (357,8,'Exif-gpstrackref','sysop',0,0,0,0.449664661949,'20050831104122',357,35);
INSERT INTO `page` VALUES (358,8,'Exif-gpsversionid','sysop',0,0,0,0.659699178306,'20050831104122',358,15);
INSERT INTO `page` VALUES (359,8,'Exif-imagedescription','sysop',0,0,0,0.324011726389,'20050831104122',359,11);
INSERT INTO `page` VALUES (360,8,'Exif-imagelength','sysop',0,0,0,0.69157383217,'20050831104122',360,6);
INSERT INTO `page` VALUES (361,8,'Exif-imageuniqueid','sysop',0,0,0,0.96536906107,'20050831104122',361,15);
INSERT INTO `page` VALUES (362,8,'Exif-imagewidth','sysop',0,0,0,0.987375659915,'20050831104122',362,6);
INSERT INTO `page` VALUES (363,8,'Exif-isospeedratings','sysop',0,0,0,0.45020850241,'20050831104122',363,25);
INSERT INTO `page` VALUES (364,8,'Exif-jpeginterchangeformat','sysop',0,0,0,0.216893405294,'20050831104122',364,18);
INSERT INTO `page` VALUES (365,8,'Exif-jpeginterchangeformatlength','sysop',0,0,0,0.844468778263,'20050831104122',365,18);
INSERT INTO `page` VALUES (366,8,'Exif-lightsource','sysop',0,0,0,0.958482483911,'20050831104122',366,11);
INSERT INTO `page` VALUES (367,8,'Exif-lightsource-0','sysop',0,0,0,0.555970624725,'20050831104122',367,7);
INSERT INTO `page` VALUES (368,8,'Exif-lightsource-1','sysop',0,0,0,0.399639719838,'20050831104122',368,8);
INSERT INTO `page` VALUES (369,8,'Exif-lightsource-10','sysop',0,0,0,0.867412437338,'20050831104122',369,13);
INSERT INTO `page` VALUES (370,8,'Exif-lightsource-11','sysop',0,0,0,0.729043194117,'20050831104122',370,5);
INSERT INTO `page` VALUES (371,8,'Exif-lightsource-12','sysop',0,0,0,0.7854117237,'20050831104122',371,39);
INSERT INTO `page` VALUES (372,8,'Exif-lightsource-13','sysop',0,0,0,0.784603887354,'20050831104122',372,40);
INSERT INTO `page` VALUES (373,8,'Exif-lightsource-14','sysop',0,0,0,0.740967013655,'20050831104122',373,41);
INSERT INTO `page` VALUES (374,8,'Exif-lightsource-15','sysop',0,0,0,0.878991372711,'20050831104122',374,37);
INSERT INTO `page` VALUES (375,8,'Exif-lightsource-17','sysop',0,0,0,0.941211034562,'20050831104122',375,16);
INSERT INTO `page` VALUES (376,8,'Exif-lightsource-18','sysop',0,0,0,0.378742918738,'20050831104122',376,16);
INSERT INTO `page` VALUES (377,8,'Exif-lightsource-19','sysop',0,0,0,0.03134158437,'20050831104122',377,16);
INSERT INTO `page` VALUES (378,8,'Exif-lightsource-2','sysop',0,0,0,0.389428029127,'20050831104122',378,11);
INSERT INTO `page` VALUES (379,8,'Exif-lightsource-20','sysop',0,0,0,0.563509553732,'20050831104122',379,3);
INSERT INTO `page` VALUES (380,8,'Exif-lightsource-21','sysop',0,0,0,0.27956177841,'20050831104122',380,3);
INSERT INTO `page` VALUES (381,8,'Exif-lightsource-22','sysop',0,0,0,0.185505482639,'20050831104122',381,3);
INSERT INTO `page` VALUES (382,8,'Exif-lightsource-23','sysop',0,0,0,0.348295398326,'20050831104122',382,3);
INSERT INTO `page` VALUES (383,8,'Exif-lightsource-24','sysop',0,0,0,0.434076160257,'20050831104122',383,19);
INSERT INTO `page` VALUES (384,8,'Exif-lightsource-255','sysop',0,0,0,0.405253689655,'20050831104122',384,18);
INSERT INTO `page` VALUES (385,8,'Exif-lightsource-3','sysop',0,0,0,0.889028788307,'20050831104122',385,29);
INSERT INTO `page` VALUES (386,8,'Exif-lightsource-4','sysop',0,0,0,0.923976318579,'20050831104122',386,5);
INSERT INTO `page` VALUES (387,8,'Exif-lightsource-9','sysop',0,0,0,0.513619329875,'20050831104122',387,12);
INSERT INTO `page` VALUES (388,8,'Exif-make','sysop',0,0,0,0.985274009828,'20050831104122',388,10);
INSERT INTO `page` VALUES (389,8,'Exif-make-value','sysop',0,0,0,0.400889756357,'20050831104122',389,2);
INSERT INTO `page` VALUES (390,8,'Exif-makernote','sysop',0,0,0,0.256145789036,'20050831104122',390,18);
INSERT INTO `page` VALUES (391,8,'Exif-maxaperturevalue','sysop',0,0,0,0.474129353999,'20050831104122',391,15);
INSERT INTO `page` VALUES (392,8,'Exif-meteringmode','sysop',0,0,0,0.357280640678,'20050831104122',392,13);
INSERT INTO `page` VALUES (393,8,'Exif-meteringmode-0','sysop',0,0,0,0.337338642684,'20050831104122',393,7);
INSERT INTO `page` VALUES (394,8,'Exif-meteringmode-1','sysop',0,0,0,0.267401164401,'20050831104122',394,7);
INSERT INTO `page` VALUES (395,8,'Exif-meteringmode-2','sysop',0,0,0,0.843901920927,'20050831104122',395,21);
INSERT INTO `page` VALUES (396,8,'Exif-meteringmode-255','sysop',0,0,0,0.839476449963,'20050831104122',396,5);
INSERT INTO `page` VALUES (397,8,'Exif-meteringmode-3','sysop',0,0,0,0.745714162109,'20050831104122',397,4);
INSERT INTO `page` VALUES (398,8,'Exif-meteringmode-4','sysop',0,0,0,0.916708466333,'20050831104122',398,9);
INSERT INTO `page` VALUES (399,8,'Exif-meteringmode-5','sysop',0,0,0,0.358189448184,'20050831104122',399,7);
INSERT INTO `page` VALUES (400,8,'Exif-meteringmode-6','sysop',0,0,0,0.14436584201,'20050831104122',400,7);
INSERT INTO `page` VALUES (401,8,'Exif-model','sysop',0,0,0,0.895308045301,'20050831104122',401,6);
INSERT INTO `page` VALUES (402,8,'Exif-model-value','sysop',0,0,0,0.657437368486,'20050831104122',402,2);
INSERT INTO `page` VALUES (403,8,'Exif-oecf','sysop',0,0,0,0.423848468865,'20050831104122',403,32);
INSERT INTO `page` VALUES (404,8,'Exif-orientation','sysop',0,0,0,0.145248844679,'20050831104122',404,12);
INSERT INTO `page` VALUES (405,8,'Exif-orientation-1','sysop',0,0,0,0.128319305168,'20050831104122',405,6);
INSERT INTO `page` VALUES (406,8,'Exif-orientation-2','sysop',0,0,0,0.872905192707,'20050831104122',406,20);
INSERT INTO `page` VALUES (407,8,'Exif-orientation-3','sysop',0,0,0,0.091113569811,'20050831104122',407,13);
INSERT INTO `page` VALUES (408,8,'Exif-orientation-4','sysop',0,0,0,0.018575337603,'20050831104122',408,18);
INSERT INTO `page` VALUES (409,8,'Exif-orientation-5','sysop',0,0,0,0.409430035345,'20050831104122',409,39);
INSERT INTO `page` VALUES (410,8,'Exif-orientation-6','sysop',0,0,0,0.133755023776,'20050831104122',410,15);
INSERT INTO `page` VALUES (411,8,'Exif-orientation-7','sysop',0,0,0,0.22212069438,'20050831104122',411,38);
INSERT INTO `page` VALUES (412,8,'Exif-orientation-8','sysop',0,0,0,0.856758375614,'20050831104122',412,16);
INSERT INTO `page` VALUES (413,8,'Exif-photometricinterpretation','sysop',0,0,0,0.600736140443,'20050831104122',413,17);
INSERT INTO `page` VALUES (414,8,'Exif-photometricinterpretation-1','sysop',0,0,0,0.287221624787,'20050831104122',414,3);
INSERT INTO `page` VALUES (415,8,'Exif-photometricinterpretation-6','sysop',0,0,0,0.430680063194,'20050831104122',415,5);
INSERT INTO `page` VALUES (416,8,'Exif-pixelxdimension','sysop',0,0,0,0.473310677968,'20050831104122',416,19);
INSERT INTO `page` VALUES (417,8,'Exif-pixelydimension','sysop',0,0,0,0.476709435541,'20050831104122',417,17);
INSERT INTO `page` VALUES (418,8,'Exif-planarconfiguration','sysop',0,0,0,0.546844011798,'20050831104122',418,16);
INSERT INTO `page` VALUES (419,8,'Exif-planarconfiguration-1','sysop',0,0,0,0.104393274612,'20050831104122',419,13);
INSERT INTO `page` VALUES (420,8,'Exif-planarconfiguration-2','sysop',0,0,0,0.860618445581,'20050831104122',420,13);
INSERT INTO `page` VALUES (421,8,'Exif-primarychromaticities','sysop',0,0,0,0.847576375215,'20050831104122',421,29);
INSERT INTO `page` VALUES (422,8,'Exif-referenceblackwhite','sysop',0,0,0,0.822049354479,'20050831104122',422,28);
INSERT INTO `page` VALUES (423,8,'Exif-relatedsoundfile','sysop',0,0,0,0.518581735116,'20050831104122',423,18);
INSERT INTO `page` VALUES (424,8,'Exif-resolutionunit','sysop',0,0,0,0.00219794368,'20050831104122',424,26);
INSERT INTO `page` VALUES (425,8,'Exif-rowsperstrip','sysop',0,0,0,0.244954082061,'20050831104122',425,24);
INSERT INTO `page` VALUES (426,8,'Exif-samplesperpixel','sysop',0,0,0,0.898270857996,'20050831104122',426,20);
INSERT INTO `page` VALUES (427,8,'Exif-saturation','sysop',0,0,0,0.964408162855,'20050831104122',427,10);
INSERT INTO `page` VALUES (428,8,'Exif-saturation-0','sysop',0,0,0,0.339631886732,'20050831104122',428,6);
INSERT INTO `page` VALUES (429,8,'Exif-saturation-1','sysop',0,0,0,0.978554934895,'20050831104122',429,14);
INSERT INTO `page` VALUES (430,8,'Exif-saturation-2','sysop',0,0,0,0.070511107152,'20050831104122',430,15);
INSERT INTO `page` VALUES (431,8,'Exif-scenecapturetype','sysop',0,0,0,0.609358327965,'20050831104122',431,18);
INSERT INTO `page` VALUES (432,8,'Exif-scenecapturetype-0','sysop',0,0,0,0.45995913768,'20050831104122',432,8);
INSERT INTO `page` VALUES (433,8,'Exif-scenecapturetype-1','sysop',0,0,0,0.454451375178,'20050831104122',433,9);
INSERT INTO `page` VALUES (434,8,'Exif-scenecapturetype-2','sysop',0,0,0,0.334279817181,'20050831104122',434,8);
INSERT INTO `page` VALUES (435,8,'Exif-scenecapturetype-3','sysop',0,0,0,0.120604548865,'20050831104122',435,11);
INSERT INTO `page` VALUES (436,8,'Exif-scenetype','sysop',0,0,0,0.81731041862,'20050831104122',436,10);
INSERT INTO `page` VALUES (437,8,'Exif-scenetype-1','sysop',0,0,0,0.399095851975,'20050831104122',437,29);
INSERT INTO `page` VALUES (438,8,'Exif-sensingmethod','sysop',0,0,0,0.268561979869,'20050831104122',438,14);
INSERT INTO `page` VALUES (439,8,'Exif-sensingmethod-1','sysop',0,0,0,0.132845114647,'20050831104122',439,9);
INSERT INTO `page` VALUES (440,8,'Exif-sensingmethod-2','sysop',0,0,0,0.978297486397,'20050831104122',440,26);
INSERT INTO `page` VALUES (441,8,'Exif-sensingmethod-3','sysop',0,0,0,0.065072387215,'20050831104122',441,26);
INSERT INTO `page` VALUES (442,8,'Exif-sensingmethod-4','sysop',0,0,0,0.438428018733,'20050831104122',442,28);
INSERT INTO `page` VALUES (443,8,'Exif-sensingmethod-5','sysop',0,0,0,0.718500375635,'20050831104122',443,28);
INSERT INTO `page` VALUES (444,8,'Exif-sensingmethod-7','sysop',0,0,0,0.860112799245,'20050831104122',444,16);
INSERT INTO `page` VALUES (445,8,'Exif-sensingmethod-8','sysop',0,0,0,0.830838806608,'20050831104122',445,30);
INSERT INTO `page` VALUES (446,8,'Exif-sharpness','sysop',0,0,0,0.842751234714,'20050831104122',446,8);
INSERT INTO `page` VALUES (447,8,'Exif-sharpness-0','sysop',0,0,0,0.533940693093,'20050831104122',447,6);
INSERT INTO `page` VALUES (448,8,'Exif-sharpness-1','sysop',0,0,0,0.930812387967,'20050831104122',448,4);
INSERT INTO `page` VALUES (449,8,'Exif-sharpness-2','sysop',0,0,0,0.563166978632,'20050831104122',449,4);
INSERT INTO `page` VALUES (450,8,'Exif-shutterspeedvalue','sysop',0,0,0,0.416970113565,'20050831104122',450,19);
INSERT INTO `page` VALUES (451,8,'Exif-software','sysop',0,0,0,0.952455510336,'20050831104122',451,8);
INSERT INTO `page` VALUES (452,8,'Exif-software-value','sysop',0,0,0,0.34830614246,'20050831104122',452,2);
INSERT INTO `page` VALUES (453,8,'Exif-spatialfrequencyresponse','sysop',0,0,0,0.655803489387,'20050831104122',453,26);
INSERT INTO `page` VALUES (454,8,'Exif-spectralsensitivity','sysop',0,0,0,0.781464624203,'20050831104122',454,20);
INSERT INTO `page` VALUES (455,8,'Exif-stripbytecounts','sysop',0,0,0,0.140780864299,'20050831104122',455,26);
INSERT INTO `page` VALUES (456,8,'Exif-stripoffsets','sysop',0,0,0,0.3921190674,'20050831104122',456,19);
INSERT INTO `page` VALUES (457,8,'Exif-subjectarea','sysop',0,0,0,0.111591560338,'20050831104122',457,12);
INSERT INTO `page` VALUES (458,8,'Exif-subjectdistance','sysop',0,0,0,0.376094933488,'20050831104122',458,10);
INSERT INTO `page` VALUES (459,8,'Exif-subjectdistance-value','sysop',0,0,0,0.866361850438,'20050831104122',459,9);
INSERT INTO `page` VALUES (460,8,'Exif-subjectdistancerange','sysop',0,0,0,0.825530022307,'20050831104122',460,22);
INSERT INTO `page` VALUES (461,8,'Exif-subjectdistancerange-0','sysop',0,0,0,0.202606012862,'20050831104122',461,7);
INSERT INTO `page` VALUES (462,8,'Exif-subjectdistancerange-1','sysop',0,0,0,0.053512508897,'20050831104122',462,5);
INSERT INTO `page` VALUES (463,8,'Exif-subjectdistancerange-2','sysop',0,0,0,0.218462890494,'20050831104122',463,10);
INSERT INTO `page` VALUES (464,8,'Exif-subjectdistancerange-3','sysop',0,0,0,0.172738507248,'20050831104122',464,12);
INSERT INTO `page` VALUES (465,8,'Exif-subjectlocation','sysop',0,0,0,0.879971743678,'20050831104122',465,16);
INSERT INTO `page` VALUES (466,8,'Exif-subsectime','sysop',0,0,0,0.02886828651,'20050831104122',466,19);
INSERT INTO `page` VALUES (467,8,'Exif-subsectimedigitized','sysop',0,0,0,0.314970051378,'20050831104122',467,28);
INSERT INTO `page` VALUES (468,8,'Exif-subsectimeoriginal','sysop',0,0,0,0.931501149329,'20050831104122',468,27);
INSERT INTO `page` VALUES (469,8,'Exif-transferfunction','sysop',0,0,0,0.066202916289,'20050831104122',469,17);
INSERT INTO `page` VALUES (470,8,'Exif-usercomment','sysop',0,0,0,0.110429354093,'20050831104122',470,13);
INSERT INTO `page` VALUES (471,8,'Exif-whitebalance','sysop',0,0,0,0.676486342579,'20050831104122',471,13);
INSERT INTO `page` VALUES (472,8,'Exif-whitebalance-0','sysop',0,0,0,0.609553413761,'20050831104122',472,18);
INSERT INTO `page` VALUES (473,8,'Exif-whitebalance-1','sysop',0,0,0,0.081737600902,'20050831104122',473,20);
INSERT INTO `page` VALUES (474,8,'Exif-whitepoint','sysop',0,0,0,0.862772913446,'20050831104122',474,24);
INSERT INTO `page` VALUES (475,8,'Exif-xresolution','sysop',0,0,0,0.380289888291,'20050831104122',475,22);
INSERT INTO `page` VALUES (476,8,'Exif-xyresolution-c','sysop',0,0,0,0.430160773479,'20050831104122',476,6);
INSERT INTO `page` VALUES (477,8,'Exif-xyresolution-i','sysop',0,0,0,0.350924820095,'20050831104122',477,6);
INSERT INTO `page` VALUES (478,8,'Exif-ycbcrcoefficients','sysop',0,0,0,0.552027359346,'20050831104122',478,19);
INSERT INTO `page` VALUES (479,8,'Exif-ycbcrpositioning','sysop',0,0,0,0.280105858657,'20050831104122',479,19);
INSERT INTO `page` VALUES (480,8,'Exif-ycbcrsubsampling','sysop',0,0,0,0.067322660749,'20050831104122',480,27);
INSERT INTO `page` VALUES (481,8,'Exif-yresolution','sysop',0,0,0,0.089933495252,'20050831104122',481,20);
INSERT INTO `page` VALUES (482,8,'Explainconflict','sysop',0,0,0,0.469024621087,'20050831104122',482,353);
INSERT INTO `page` VALUES (483,8,'Export','sysop',0,0,0,0.011811713999,'20050831104122',483,18);
INSERT INTO `page` VALUES (484,8,'Exportcuronly','sysop',0,0,0,0.342659157011,'20050831104123',484,46);
INSERT INTO `page` VALUES (485,8,'Exporttext','sysop',0,0,0,0.746554576234,'20050831104123',485,234);
INSERT INTO `page` VALUES (486,8,'Externaldberror','sysop',0,0,0,0.571158486909,'20050831104123',486,114);
INSERT INTO `page` VALUES (487,8,'Extlink_sample','sysop',0,0,0,0.892982852702,'20050831104123',487,32);
INSERT INTO `page` VALUES (488,8,'Extlink_tip','sysop',0,0,0,0.708678573852,'20050831104123',488,32);
INSERT INTO `page` VALUES (489,8,'Faq','sysop',0,0,0,0.258817876052,'20050831104123',489,3);
INSERT INTO `page` VALUES (490,8,'Faqpage','sysop',0,0,0,0.408528833697,'20050831104123',490,37);
INSERT INTO `page` VALUES (491,8,'Feb','sysop',0,0,0,0.436772252012,'20050831104123',491,3);
INSERT INTO `page` VALUES (492,8,'February','sysop',0,0,0,0.528123050192,'20050831104123',492,7);
INSERT INTO `page` VALUES (493,8,'Feedlinks','sysop',0,0,0,0.8021788706,'20050831104123',493,5);
INSERT INTO `page` VALUES (494,8,'Filecopyerror','sysop',0,0,0,0.777608284612,'20050831104123',494,43);
INSERT INTO `page` VALUES (495,8,'Filedeleteerror','sysop',0,0,0,0.383264624451,'20050831104123',495,33);
INSERT INTO `page` VALUES (496,8,'Filedesc','sysop',0,0,0,0.379645156637,'20050831104123',496,12);
INSERT INTO `page` VALUES (497,8,'Fileexists','sysop',0,0,0,0.933236416394,'20050831104123',497,99);
INSERT INTO `page` VALUES (498,8,'Fileinfo','sysop',0,0,0,0.946675867675,'20050831104123',498,77);
INSERT INTO `page` VALUES (499,8,'Filemissing','sysop',0,0,0,0.959277824678,'20050831104123',499,12);
INSERT INTO `page` VALUES (500,8,'Filename','sysop',0,0,0,0.017521713734,'20050831104123',500,9);
INSERT INTO `page` VALUES (501,8,'Filenotfound','sysop',0,0,0,0.311746118316,'20050831104123',501,31);
INSERT INTO `page` VALUES (502,8,'Filerenameerror','sysop',0,0,0,0.044304456717,'20050831104123',502,45);
INSERT INTO `page` VALUES (503,8,'Files','sysop',0,0,0,0.556958894157,'20050831104123',503,5);
INSERT INTO `page` VALUES (504,8,'Filesource','sysop',0,0,0,0.395690861302,'20050831104123',504,6);
INSERT INTO `page` VALUES (505,8,'Filestatus','sysop',0,0,0,0.993709691465,'20050831104123',505,16);
INSERT INTO `page` VALUES (506,8,'Fileuploaded','sysop',0,0,0,0.569523951408,'20050831104123',506,442);
INSERT INTO `page` VALUES (507,8,'Formerror','sysop',0,0,0,0.424842200059,'20050831104123',507,41);
INSERT INTO `page` VALUES (508,8,'Friday','sysop',0,0,0,0.188771903931,'20050831104123',508,7);
INSERT INTO `page` VALUES (509,8,'Geo','sysop',0,0,0,0.785780970061,'20050831104123',509,15);
INSERT INTO `page` VALUES (510,8,'Getimagelist','sysop',0,0,0,0.119470988006,'20050831104123',510,16);
INSERT INTO `page` VALUES (511,8,'Go','sysop',0,0,0,0.566285685221,'20050831104123',511,3);
INSERT INTO `page` VALUES (512,8,'Googlesearch','sysop',0,0,0,0.08788525614,'20050831104123',512,660);
INSERT INTO `page` VALUES (513,8,'Group-admin-desc','sysop',0,0,0,0.526235437672,'20050831104123',513,53);
INSERT INTO `page` VALUES (514,8,'Group-admin-name','sysop',0,0,0,0.725542448838,'20050831104123',514,13);
INSERT INTO `page` VALUES (515,8,'Group-anon-desc','sysop',0,0,0,0.373698443742,'20050831104123',515,15);
INSERT INTO `page` VALUES (516,8,'Group-anon-name','sysop',0,0,0,0.157931452121,'20050831104123',516,9);
INSERT INTO `page` VALUES (517,8,'Group-bureaucrat-desc','sysop',0,0,0,0.373151533039,'20050831104123',517,43);
INSERT INTO `page` VALUES (518,8,'Group-bureaucrat-name','sysop',0,0,0,0.518683361149,'20050831104123',518,10);
INSERT INTO `page` VALUES (519,8,'Group-loggedin-desc','sysop',0,0,0,0.517676701115,'20050831104123',519,23);
INSERT INTO `page` VALUES (520,8,'Group-loggedin-name','sysop',0,0,0,0.922785625459,'20050831104123',520,4);
INSERT INTO `page` VALUES (521,8,'Group-steward-desc','sysop',0,0,0,0.176943457728,'20050831104123',521,11);
INSERT INTO `page` VALUES (522,8,'Group-steward-name','sysop',0,0,0,0.464282920021,'20050831104123',522,7);
INSERT INTO `page` VALUES (523,8,'Grouprightspheading','sysop',0,0,0,0.322301646797,'20050831104123',523,17);
INSERT INTO `page` VALUES (524,8,'Groups','sysop',0,0,0,0.703172063336,'20050831104123',524,11);
INSERT INTO `page` VALUES (525,8,'Groups-addgroup','sysop',0,0,0,0.277840056928,'20050831104123',525,9);
INSERT INTO `page` VALUES (526,8,'Groups-already-exists','sysop',0,0,0,0.04418710446,'20050831104123',526,35);
INSERT INTO `page` VALUES (527,8,'Groups-editgroup','sysop',0,0,0,0.030292824741,'20050831104123',527,10);
INSERT INTO `page` VALUES (528,8,'Groups-editgroup-description','sysop',0,0,0,0.582523828598,'20050831104123',528,45);
INSERT INTO `page` VALUES (529,8,'Groups-editgroup-name','sysop',0,0,0,0.474456479848,'20050831104123',529,11);
INSERT INTO `page` VALUES (530,8,'Groups-editgroup-preamble','sysop',0,0,0,0.957247242689,'20050831104123',530,163);
INSERT INTO `page` VALUES (531,8,'Groups-existing','sysop',0,0,0,0.034621862063,'20050831104123',531,15);
INSERT INTO `page` VALUES (532,8,'Groups-group-edit','sysop',0,0,0,0.57005043175,'20050831104123',532,16);
INSERT INTO `page` VALUES (533,8,'Groups-lookup-group','sysop',0,0,0,0.409905704212,'20050831104123',533,19);
INSERT INTO `page` VALUES (534,8,'Groups-noname','sysop',0,0,0,0.219240825273,'20050831104123',534,33);
INSERT INTO `page` VALUES (535,8,'Groups-tableheader','sysop',0,0,0,0.873329549717,'20050831104123',535,35);
INSERT INTO `page` VALUES (536,8,'Guesstimezone','sysop',0,0,0,0.417668466144,'20050831104123',536,25);
INSERT INTO `page` VALUES (537,8,'Headline_sample','sysop',0,0,0,0.985132395267,'20050831104123',537,20);
INSERT INTO `page` VALUES (538,8,'Headline_tip','sysop',0,0,0,0.143129512338,'20050831104123',538,20);
INSERT INTO `page` VALUES (539,8,'Help','sysop',0,0,0,0.213348110071,'20050831104123',539,5);
INSERT INTO `page` VALUES (540,8,'Helppage','sysop',0,0,0,0.424336808956,'20050831104123',540,13);
INSERT INTO `page` VALUES (541,8,'Hide','sysop',0,0,0,0.553446906239,'20050831104123',541,10);
INSERT INTO `page` VALUES (542,8,'Hidetoc','sysop',0,0,0,0.965092007784,'20050831104123',542,9);
INSERT INTO `page` VALUES (543,8,'Hist','sysop',0,0,0,0.280940692439,'20050831104123',543,9);
INSERT INTO `page` VALUES (544,8,'Histfirst','sysop',0,0,0,0.204572724784,'20050831104123',544,8);
INSERT INTO `page` VALUES (545,8,'Histlast','sysop',0,0,0,0.322489084835,'20050831104123',545,6);
INSERT INTO `page` VALUES (546,8,'Histlegend','sysop',0,0,0,0.902787735144,'20050831104123',546,248);
INSERT INTO `page` VALUES (547,8,'History','sysop',0,0,0,0.287798325002,'20050831104123',547,9);
INSERT INTO `page` VALUES (548,8,'History_copyright','sysop',0,0,0,0.455283623033,'20050831104123',548,1);
INSERT INTO `page` VALUES (549,8,'History_short','sysop',0,0,0,0.755128677777,'20050831104123',549,9);
INSERT INTO `page` VALUES (550,8,'Historywarning','sysop',0,0,0,0.714937163658,'20050831104123',550,76);
INSERT INTO `page` VALUES (551,8,'Hr_tip','sysop',0,0,0,0.431643997636,'20050831104123',551,37);
INSERT INTO `page` VALUES (552,8,'Ignorewarning','sysop',0,0,0,0.695701386216,'20050831104123',552,48);
INSERT INTO `page` VALUES (553,8,'Illegalfilename','sysop',0,0,0,0.519600401781,'20050831104123',553,125);
INSERT INTO `page` VALUES (554,8,'Ilsubmit','sysop',0,0,0,0.447933832654,'20050831104123',554,5);
INSERT INTO `page` VALUES (555,8,'Image_sample','sysop',0,0,0,0.524382888245,'20050831104123',555,12);
INSERT INTO `page` VALUES (556,8,'Image_tip','sysop',0,0,0,0.41989032201,'20050831104123',556,12);
INSERT INTO `page` VALUES (557,8,'Imagelinks','sysop',0,0,0,0.247002836725,'20050831104123',557,12);
INSERT INTO `page` VALUES (558,8,'Imagelist','sysop',0,0,0,0.515825169865,'20050831104123',558,11);
INSERT INTO `page` VALUES (559,8,'Imagelistall','sysop',0,0,0,0.351461285937,'20050831104123',559,3);
INSERT INTO `page` VALUES (560,8,'Imagelisttext','sysop',0,0,0,0.831546418573,'20050831104123',560,48);
INSERT INTO `page` VALUES (561,8,'Imagemaxsize','sysop',0,0,0,0.324602911243,'20050831104123',561,43);
INSERT INTO `page` VALUES (562,8,'Imagepage','sysop',0,0,0,0.484097584471,'20050831104123',562,9);
INSERT INTO `page` VALUES (563,8,'Imagereverted','sysop',0,0,0,0.611936678975,'20050831104123',563,37);
INSERT INTO `page` VALUES (564,8,'Imgdelete','sysop',0,0,0,0.64838693834,'20050831104123',564,8);
INSERT INTO `page` VALUES (565,8,'Imgdesc','sysop',0,0,0,0.070372143517,'20050831104123',565,12);
INSERT INTO `page` VALUES (566,8,'Imghistlegend','sysop',0,0,0,0.571478543146,'20050831104123',566,139);
INSERT INTO `page` VALUES (567,8,'Imghistory','sysop',0,0,0,0.369660866007,'20050831104123',567,14);
INSERT INTO `page` VALUES (568,8,'Imglegend','sysop',0,0,0,0.273397007967,'20050831104123',568,59);
INSERT INTO `page` VALUES (569,8,'Immobile_namespace','sysop',0,0,0,0.162860925285,'20050831104123',569,78);
INSERT INTO `page` VALUES (570,8,'Import','sysop',0,0,0,0.399379125543,'20050831104123',570,18);
INSERT INTO `page` VALUES (571,8,'Importfailed','sysop',0,0,0,0.969914751623,'20050831104123',571,25);
INSERT INTO `page` VALUES (572,8,'Importhistoryconflict','sysop',0,0,0,0.604038958999,'20050831104123',572,128);
INSERT INTO `page` VALUES (573,8,'Importinterwiki','sysop',0,0,0,0.421954917839,'20050831104123',573,16);
INSERT INTO `page` VALUES (574,8,'Importnosources','sysop',0,0,0,0.230211172815,'20050831104123',574,86);
INSERT INTO `page` VALUES (575,8,'Importnotext','sysop',0,0,0,0.922892875886,'20050831104123',575,19);
INSERT INTO `page` VALUES (576,8,'Importsuccess','sysop',0,0,0,0.934744683385,'20050831104123',576,19);
INSERT INTO `page` VALUES (577,8,'Importtext','sysop',0,0,0,0.567026566787,'20050831104123',577,128);
INSERT INTO `page` VALUES (578,8,'Info_short','sysop',0,0,0,0.610026769789,'20050831104123',578,11);
INSERT INTO `page` VALUES (579,8,'Infobox','sysop',0,0,0,0.096214114795,'20050831104123',579,60);
INSERT INTO `page` VALUES (580,8,'Infobox_alert','sysop',0,0,0,0.144482085736,'20050831104123',580,145);
INSERT INTO `page` VALUES (581,8,'Infosubtitle','sysop',0,0,0,0.343941379523,'20050831104123',581,20);
INSERT INTO `page` VALUES (582,8,'Internalerror','sysop',0,0,0,0.930032532617,'20050831104123',582,15);
INSERT INTO `page` VALUES (583,8,'Intl','sysop',0,0,0,0.233243335351,'20050831104123',583,14);
INSERT INTO `page` VALUES (584,8,'Invalidemailaddress','sysop',0,0,0,0.136527932067,'20050831104123',584,179);
INSERT INTO `page` VALUES (585,8,'Invert','sysop',0,0,0,0.816718997393,'20050831104123',585,16);
INSERT INTO `page` VALUES (586,8,'Ip_range_invalid','sysop',0,0,0,0.259318256003,'20050831104123',586,30);
INSERT INTO `page` VALUES (587,8,'Ipaddress','sysop',0,0,0,0.510210530926,'20050831104123',587,10);
INSERT INTO `page` VALUES (588,8,'Ipadressorusername','sysop',0,0,0,0.82030590907,'20050831104123',588,22);
INSERT INTO `page` VALUES (589,8,'Ipb_expiry_invalid','sysop',0,0,0,0.866594065253,'20050831104123',589,41);
INSERT INTO `page` VALUES (590,8,'Ipbexpiry','sysop',0,0,0,0.476687184584,'20050831104123',590,10);
INSERT INTO `page` VALUES (591,8,'Ipblocklist','sysop',0,0,0,0.48636211037,'20050831104123',591,29);
INSERT INTO `page` VALUES (592,8,'Ipboptions','sysop',0,0,0,0.466227091012,'20050831104123',592,155);
INSERT INTO `page` VALUES (593,8,'Ipbother','sysop',0,0,0,0.563862288188,'20050831104123',593,10);
INSERT INTO `page` VALUES (594,8,'Ipbotheroption','sysop',0,0,0,0.888399751663,'20050831104123',594,5);
INSERT INTO `page` VALUES (595,8,'Ipbreason','sysop',0,0,0,0.185026815314,'20050831104123',595,5);
INSERT INTO `page` VALUES (596,8,'Ipbsubmit','sysop',0,0,0,0.814091060653,'20050831104123',596,18);
INSERT INTO `page` VALUES (597,8,'Ipusubmit','sysop',0,0,0,0.284696024559,'20050831104123',597,23);
INSERT INTO `page` VALUES (598,8,'Ipusuccess','sysop',0,0,0,0.033397142457,'20050831104123',598,33);
INSERT INTO `page` VALUES (599,8,'Isbn','sysop',0,0,0,0.691554014316,'20050831104123',599,4);
INSERT INTO `page` VALUES (600,8,'Isredirect','sysop',0,0,0,0.736632039026,'20050831104123',600,20);
INSERT INTO `page` VALUES (601,8,'Italic_sample','sysop',0,0,0,0.773870795934,'20050831104123',601,13);
INSERT INTO `page` VALUES (602,8,'Italic_tip','sysop',0,0,0,0.3730098556,'20050831104123',602,13);
INSERT INTO `page` VALUES (603,8,'Iteminvalidname','sysop',0,0,0,0.673953957381,'20050831104123',603,49);
INSERT INTO `page` VALUES (604,8,'Jan','sysop',0,0,0,0.801423996263,'20050831104123',604,3);
INSERT INTO `page` VALUES (605,8,'January','sysop',0,0,0,0.550548202771,'20050831104123',605,6);
INSERT INTO `page` VALUES (606,8,'Jul','sysop',0,0,0,0.324632621639,'20050831104123',606,3);
INSERT INTO `page` VALUES (607,8,'July','sysop',0,0,0,0.139438966781,'20050831104123',607,4);
INSERT INTO `page` VALUES (608,8,'Jun','sysop',0,0,0,0.129343237469,'20050831104123',608,3);
INSERT INTO `page` VALUES (609,8,'June','sysop',0,0,0,0.70588441498,'20050831104123',609,4);
INSERT INTO `page` VALUES (610,8,'Laggedslavemode','sysop',0,0,0,0.663719223789,'20050831104123',610,45);
INSERT INTO `page` VALUES (611,8,'Largefile','sysop',0,0,0,0.856220957174,'20050831104123',611,45);
INSERT INTO `page` VALUES (612,8,'Last','sysop',0,0,0,0.152760646869,'20050831104123',612,9);
INSERT INTO `page` VALUES (613,8,'Lastmodified','sysop',0,0,0,0.690671548055,'20050831104123',613,42);
INSERT INTO `page` VALUES (614,8,'Lastmodifiedby','sysop',0,0,0,0.01257608006,'20050831104123',614,37);
INSERT INTO `page` VALUES (615,8,'Lineno','sysop',0,0,0,0.219551466991,'20050831104123',615,9);
INSERT INTO `page` VALUES (616,8,'Link_sample','sysop',0,0,0,0.186370490439,'20050831104123',616,9);
INSERT INTO `page` VALUES (617,8,'Link_tip','sysop',0,0,0,0.9779718477,'20050831104123',617,13);
INSERT INTO `page` VALUES (618,8,'Linklistsub','sysop',0,0,0,0.289689156625,'20050831104123',618,20);
INSERT INTO `page` VALUES (619,8,'Linkprefix','sysop',0,0,0,0.567743581052,'20050831104123',619,31);
INSERT INTO `page` VALUES (620,8,'Linkshere','sysop',0,0,0,0.404253406951,'20050831104123',620,40);
INSERT INTO `page` VALUES (621,8,'Linkstoimage','sysop',0,0,0,0.084624334707,'20050831104123',621,43);
INSERT INTO `page` VALUES (622,8,'Linktrail','sysop',0,0,0,0.251036510864,'20050831104123',622,30);
INSERT INTO `page` VALUES (623,8,'Listform','sysop',0,0,0,0.438918754334,'20050831104123',623,5);
INSERT INTO `page` VALUES (624,8,'Listingcontinuesabbrev','sysop',0,0,0,0.077271229423,'20050831104123',624,6);
INSERT INTO `page` VALUES (625,8,'Listusers','sysop',0,0,0,0.520154183464,'20050831104123',625,19);
INSERT INTO `page` VALUES (626,8,'Loadhist','sysop',0,0,0,0.973578143037,'20050831104123',626,34);
INSERT INTO `page` VALUES (627,8,'Loadingrev','sysop',0,0,0,0.66022215553,'20050831104123',627,33);
INSERT INTO `page` VALUES (628,8,'Localtime','sysop',0,0,0,0.749392842673,'20050831104123',628,8);
INSERT INTO `page` VALUES (629,8,'Lockbtn','sysop',0,0,0,0.910220247386,'20050831104123',629,17);
INSERT INTO `page` VALUES (630,8,'Lockconfirm','sysop',0,0,0,0.202234508403,'20050831104123',630,38);
INSERT INTO `page` VALUES (631,8,'Lockdb','sysop',0,0,0,0.508668078617,'20050831104123',631,17);
INSERT INTO `page` VALUES (632,8,'Lockdbsuccesssub','sysop',0,0,0,0.165248797453,'20050831104123',632,36);
INSERT INTO `page` VALUES (633,8,'Lockdbsuccesstext','sysop',0,0,0,0.313902373723,'20050831104123',633,129);
INSERT INTO `page` VALUES (634,8,'Lockdbtext','sysop',0,0,0,0.248172548584,'20050831104123',634,180);
INSERT INTO `page` VALUES (635,8,'Locknoconfirm','sysop',0,0,0,0.611456197074,'20050831104123',635,47);
INSERT INTO `page` VALUES (636,8,'Log','sysop',0,0,0,0.267019867818,'20050831104123',636,4);
INSERT INTO `page` VALUES (637,8,'Login','sysop',0,0,0,0.335728728283,'20050831104123',637,8);
INSERT INTO `page` VALUES (638,8,'Loginend','sysop',0,0,0,0.403081851085,'20050831104123',638,6);
INSERT INTO `page` VALUES (639,8,'Loginerror','sysop',0,0,0,0.960426561741,'20050831104123',639,24);
INSERT INTO `page` VALUES (640,8,'Loginpagetitle','sysop',0,0,0,0.197185591998,'20050831104123',640,18);
INSERT INTO `page` VALUES (641,8,'Loginproblem','sysop',0,0,0,0.759299952579,'20050831104123',641,83);
INSERT INTO `page` VALUES (642,8,'Loginprompt','sysop',0,0,0,0.481645518113,'20050831104123',642,77);
INSERT INTO `page` VALUES (643,8,'Loginreqtext','sysop',0,0,0,0.521168325366,'20050831104123',643,88);
INSERT INTO `page` VALUES (644,8,'Loginreqtitle','sysop',0,0,0,0.318297542751,'20050831104123',644,22);
INSERT INTO `page` VALUES (645,8,'Loginsuccess','sysop',0,0,0,0.009674018973,'20050831104123',645,52);
INSERT INTO `page` VALUES (646,8,'Loginsuccesstitle','sysop',0,0,0,0.058576193911,'20050831104123',646,21);
INSERT INTO `page` VALUES (647,8,'Logout','sysop',0,0,0,0.715664178122,'20050831104123',647,8);
INSERT INTO `page` VALUES (648,8,'Logouttext','sysop',0,0,0,0.378947088331,'20050831104123',648,155);
INSERT INTO `page` VALUES (649,8,'Logouttitle','sysop',0,0,0,0.884455550481,'20050831104123',649,18);
INSERT INTO `page` VALUES (650,8,'Lonelypages','sysop',0,0,0,0.633890872792,'20050831104123',650,16);
INSERT INTO `page` VALUES (651,8,'Longpages','sysop',0,0,0,0.368907115683,'20050831104123',651,13);
INSERT INTO `page` VALUES (652,8,'Longpagewarning','sysop',0,0,0,0.175067750137,'20050831104123',652,231);
INSERT INTO `page` VALUES (653,8,'Mailerror','sysop',0,0,0,0.97282412731,'20050831104123',653,31);
INSERT INTO `page` VALUES (654,8,'Mailmypassword','sysop',0,0,0,0.319174313907,'20050831104123',654,41);
INSERT INTO `page` VALUES (655,8,'Mailnologin','sysop',0,0,0,0.074322707209,'20050831104123',655,26);
INSERT INTO `page` VALUES (656,8,'Mailnologintext','sysop',0,0,0,0.203788272567,'20050831104123',656,130);
INSERT INTO `page` VALUES (657,8,'Mainpage','sysop',0,0,0,0.238166751659,'20050831104123',657,10);
INSERT INTO `page` VALUES (658,8,'Mainpagedocfooter','sysop',0,0,0,0.702539359104,'20050831104123',658,236);
INSERT INTO `page` VALUES (659,8,'Mainpagetext','sysop',0,0,0,0.868969394135,'20050831104123',659,48);
INSERT INTO `page` VALUES (660,8,'Maintenance','sysop',0,0,0,0.301146768137,'20050831104123',660,13);
INSERT INTO `page` VALUES (661,8,'Maintenancebacklink','sysop',0,0,0,0.376267184193,'20050831104123',661,25);
INSERT INTO `page` VALUES (662,8,'Maintnancepagetext','sysop',0,0,0,0.702376958078,'20050831104123',662,210);
INSERT INTO `page` VALUES (663,8,'Makesysop','sysop',0,0,0,0.3554142455,'20050831104123',663,38);
INSERT INTO `page` VALUES (664,8,'Makesysopfail','sysop',0,0,0,0.960973780895,'20050831104123',664,115);
INSERT INTO `page` VALUES (665,8,'Makesysopname','sysop',0,0,0,0.172249133519,'20050831104123',665,19);
INSERT INTO `page` VALUES (666,8,'Makesysopok','sysop',0,0,0,0.902947226327,'20050831104123',666,47);
INSERT INTO `page` VALUES (667,8,'Makesysopsubmit','sysop',0,0,0,0.370922359756,'20050831104123',667,44);
INSERT INTO `page` VALUES (668,8,'Makesysoptext','sysop',0,0,0,0.944949289273,'20050831104123',668,91);
INSERT INTO `page` VALUES (669,8,'Makesysoptitle','sysop',0,0,0,0.473613323019,'20050831104123',669,38);
INSERT INTO `page` VALUES (670,8,'Mar','sysop',0,0,0,0.396841503235,'20050831104123',670,4);
INSERT INTO `page` VALUES (671,8,'March','sysop',0,0,0,0.923387040602,'20050831104123',671,5);
INSERT INTO `page` VALUES (672,8,'Markaspatrolleddiff','sysop',0,0,0,0.772715881983,'20050831104123',672,17);
INSERT INTO `page` VALUES (673,8,'Markaspatrolledlink','sysop',0,0,0,0.773954192868,'20050831104124',673,4);
INSERT INTO `page` VALUES (674,8,'Markaspatrolledtext','sysop',0,0,0,0.940699525214,'20050831104124',674,30);
INSERT INTO `page` VALUES (675,8,'Markedaspatrolled','sysop',0,0,0,0.730765632815,'20050831104124',675,19);
INSERT INTO `page` VALUES (676,8,'Markedaspatrolledtext','sysop',0,0,0,0.308461767604,'20050831104124',676,51);
INSERT INTO `page` VALUES (677,8,'Matchtotals','sysop',0,0,0,0.444591741232,'20050831104124',677,91);
INSERT INTO `page` VALUES (678,8,'Math','sysop',0,0,0,0.087749883302,'20050831104124',678,3);
INSERT INTO `page` VALUES (679,8,'Math_bad_output','sysop',0,0,0,0.17328111408,'20050831104124',679,83);
INSERT INTO `page` VALUES (680,8,'Math_bad_tmpdir','sysop',0,0,0,0.500040342573,'20050831104124',680,88);
INSERT INTO `page` VALUES (681,8,'Math_failure','sysop',0,0,0,0.7334263069,'20050831104124',681,13);
INSERT INTO `page` VALUES (682,8,'Math_image_error','sysop',0,0,0,0.995849284794,'20050831104124',682,34);
INSERT INTO `page` VALUES (683,8,'Math_lexing_error','sysop',0,0,0,0.511124764427,'20050831104124',683,15);
INSERT INTO `page` VALUES (684,8,'Math_notexvc','sysop',0,0,0,0.632346774768,'20050831104124',684,78);
INSERT INTO `page` VALUES (685,8,'Math_sample','sysop',0,0,0,0.800596113376,'20050831104124',685,21);
INSERT INTO `page` VALUES (686,8,'Math_syntax_error','sysop',0,0,0,0.355510255313,'20050831104124',686,12);
INSERT INTO `page` VALUES (687,8,'Math_tip','sysop',0,0,0,0.390134703219,'20050831104124',687,28);
INSERT INTO `page` VALUES (688,8,'Math_unknown_error','sysop',0,0,0,0.658018535781,'20050831104124',688,18);
INSERT INTO `page` VALUES (689,8,'Math_unknown_function','sysop',0,0,0,0.240978304755,'20050831104124',689,19);
INSERT INTO `page` VALUES (690,8,'May','sysop',0,0,0,0.758510924576,'20050831104124',690,3);
INSERT INTO `page` VALUES (691,8,'May_long','sysop',0,0,0,0.460587234982,'20050831104124',691,3);
INSERT INTO `page` VALUES (692,8,'Media_sample','sysop',0,0,0,0.1704782963,'20050831104124',692,12);
INSERT INTO `page` VALUES (693,8,'Media_tip','sysop',0,0,0,0.350405777163,'20050831104124',693,19);
INSERT INTO `page` VALUES (694,8,'Mediawarning','sysop',0,0,0,0.116613397435,'20050831104124',694,520);
INSERT INTO `page` VALUES (695,8,'Metadata','sysop',0,0,0,0.260736002479,'20050831104124',695,8);
INSERT INTO `page` VALUES (696,8,'Metadata_page','sysop',0,0,0,0.379671134108,'20050831104124',696,18);
INSERT INTO `page` VALUES (697,8,'Minlength','sysop',0,0,0,0.41471767388,'20050831104124',697,53);
INSERT INTO `page` VALUES (698,8,'Minoredit','sysop',0,0,0,0.858351129107,'20050831104124',698,36);
INSERT INTO `page` VALUES (699,8,'Minoreditletter','sysop',0,0,0,0.56969762185,'20050831104124',699,1);
INSERT INTO `page` VALUES (700,8,'Mispeelings','sysop',0,0,0,0.509198303783,'20050831104124',700,38);
INSERT INTO `page` VALUES (701,8,'Mispeelingspage','sysop',0,0,0,0.545187386455,'20050831104124',701,21);
INSERT INTO `page` VALUES (702,8,'Mispeelingstext','sysop',0,0,0,0.656800614657,'20050831104124',702,299);
INSERT INTO `page` VALUES (703,8,'Missingarticle','sysop',0,0,0,0.536965556293,'20050831104124',703,198);
INSERT INTO `page` VALUES (704,8,'Missingimage','sysop',0,0,0,0.808859990104,'20050831104124',704,36);
INSERT INTO `page` VALUES (705,8,'Missinglanguagelinks','sysop',0,0,0,0.43608612419,'20050831104124',705,23);
INSERT INTO `page` VALUES (706,8,'Missinglanguagelinksbutton','sysop',0,0,0,0.880923369286,'20050831104124',706,34);
INSERT INTO `page` VALUES (707,8,'Missinglanguagelinkstext','sysop',0,0,0,0.784904653667,'20050831104124',707,126);
INSERT INTO `page` VALUES (708,8,'Monday','sysop',0,0,0,0.007061534947,'20050831104124',708,6);
INSERT INTO `page` VALUES (709,8,'Moredotdotdot','sysop',0,0,0,0.714235174869,'20050831104124',709,7);
INSERT INTO `page` VALUES (710,8,'Move','sysop',0,0,0,0.872048191612,'20050831104124',710,11);
INSERT INTO `page` VALUES (711,8,'Movearticle','sysop',0,0,0,0.309490318677,'20050831104124',711,19);
INSERT INTO `page` VALUES (712,8,'Movedto','sysop',0,0,0,0.560722153216,'20050831104124',712,15);
INSERT INTO `page` VALUES (713,8,'Movelogpage','sysop',0,0,0,0.051681894035,'20050831104124',713,8);
INSERT INTO `page` VALUES (714,8,'Movelogpagetext','sysop',0,0,0,0.164585575103,'20050831104124',714,30);
INSERT INTO `page` VALUES (715,8,'Movenologin','sysop',0,0,0,0.161830321717,'20050831104124',715,25);
INSERT INTO `page` VALUES (716,8,'Movenologintext','sysop',0,0,0,0.14747882677,'20050831104124',716,111);
INSERT INTO `page` VALUES (717,8,'Movepage','sysop',0,0,0,0.984089218158,'20050831104124',717,19);
INSERT INTO `page` VALUES (718,8,'Movepagebtn','sysop',0,0,0,0.325611026224,'20050831104124',718,19);
INSERT INTO `page` VALUES (719,8,'Movepagetalktext','sysop',0,0,0,0.016129202109,'20050831104124',719,343);
INSERT INTO `page` VALUES (720,8,'Movepagetext','sysop',0,0,0,0.613286842589,'20050831104124',720,235);
INSERT INTO `page` VALUES (721,8,'Movereason','sysop',0,0,0,0.817449337636,'20050831104124',721,6);
INSERT INTO `page` VALUES (722,8,'Movetalk','sysop',0,0,0,0.611910324841,'20050831104124',722,54);
INSERT INTO `page` VALUES (723,8,'Movethispage','sysop',0,0,0,0.46063732817,'20050831104124',723,19);
INSERT INTO `page` VALUES (724,8,'Mw_math_html','sysop',0,0,0,0.955990839293,'20050831104124',724,44);
INSERT INTO `page` VALUES (725,8,'Mw_math_mathml','sysop',0,0,0,0.893954568848,'20050831104124',725,22);
INSERT INTO `page` VALUES (726,8,'Mw_math_modern','sysop',0,0,0,0.765036100451,'20050831104124',726,35);
INSERT INTO `page` VALUES (727,8,'Mw_math_png','sysop',0,0,0,0.738667783542,'20050831104124',727,24);
INSERT INTO `page` VALUES (728,8,'Mw_math_simple','sysop',0,0,0,0.743134286378,'20050831104124',728,44);
INSERT INTO `page` VALUES (729,8,'Mw_math_source','sysop',0,0,0,0.80885429522,'20050831104124',729,35);
INSERT INTO `page` VALUES (730,8,'Mycontris','sysop',0,0,0,0.915532758669,'20050831104124',730,15);
INSERT INTO `page` VALUES (731,8,'Mypage','sysop',0,0,0,0.741487669545,'20050831104124',731,11);
INSERT INTO `page` VALUES (732,8,'Mytalk','sysop',0,0,0,0.374750854602,'20050831104124',732,16);
INSERT INTO `page` VALUES (733,8,'Namespace','sysop',0,0,0,0.744221830785,'20050831104124',733,10);
INSERT INTO `page` VALUES (734,8,'Navigation','sysop',0,0,0,0.06484417328,'20050831104124',734,10);
INSERT INTO `page` VALUES (735,8,'Nbytes','sysop',0,0,0,0.583293161779,'20050831104124',735,7);
INSERT INTO `page` VALUES (736,8,'Nchanges','sysop',0,0,0,0.968708782804,'20050831104124',736,14);
INSERT INTO `page` VALUES (737,8,'Newarticle','sysop',0,0,0,0.48352920167,'20050831104124',737,5);
INSERT INTO `page` VALUES (738,8,'Newarticletext','sysop',0,0,0,0.428057956107,'20050831104124',738,141);
INSERT INTO `page` VALUES (739,8,'Newbies','sysop',0,0,0,0.142002587026,'20050831104124',739,7);
INSERT INTO `page` VALUES (740,8,'Newimages','sysop',0,0,0,0.170730038395,'20050831104124',740,20);
INSERT INTO `page` VALUES (741,8,'Newmessages','sysop',0,0,0,0.591210440938,'20050831104124',741,13);
INSERT INTO `page` VALUES (742,8,'Newmessageslink','sysop',0,0,0,0.285385134313,'20050831104124',742,16);
INSERT INTO `page` VALUES (743,8,'Newpage','sysop',0,0,0,0.111877260304,'20050831104124',743,10);
INSERT INTO `page` VALUES (744,8,'Newpageletter','sysop',0,0,0,0.530926625697,'20050831104124',744,1);
INSERT INTO `page` VALUES (745,8,'Newpages','sysop',0,0,0,0.604043669155,'20050831104124',745,12);
INSERT INTO `page` VALUES (746,8,'Newpassword','sysop',0,0,0,0.216640545408,'20050831104124',746,14);
INSERT INTO `page` VALUES (747,8,'Newtitle','sysop',0,0,0,0.524121097343,'20050831104124',747,14);
INSERT INTO `page` VALUES (748,8,'Newusersonly','sysop',0,0,0,0.947226080205,'20050831104124',748,27);
INSERT INTO `page` VALUES (749,8,'Newwindow','sysop',0,0,0,0.402829197539,'20050831104124',749,21);
INSERT INTO `page` VALUES (750,8,'Next','sysop',0,0,0,0.998530744575,'20050831104124',750,8);
INSERT INTO `page` VALUES (751,8,'Nextdiff','sysop',0,0,0,0.922062633472,'20050831104124',751,16);
INSERT INTO `page` VALUES (752,8,'Nextn','sysop',0,0,0,0.440710631829,'20050831104124',752,11);
INSERT INTO `page` VALUES (753,8,'Nextpage','sysop',0,0,0,0.653200336843,'20050831104124',753,14);
INSERT INTO `page` VALUES (754,8,'Nextrevision','sysop',0,0,0,0.800433199385,'20050831104124',754,29);
INSERT INTO `page` VALUES (755,8,'Nlinks','sysop',0,0,0,0.741906026294,'20050831104124',755,11);
INSERT INTO `page` VALUES (756,8,'Noarticletext','sysop',0,0,0,0.42325329983,'20050831104124',756,46);
INSERT INTO `page` VALUES (757,8,'Noconnect','sysop',0,0,0,0.32482858116,'20050831104124',757,55);
INSERT INTO `page` VALUES (758,8,'Nocontribs','sysop',0,0,0,0.300425220617,'20050831104124',758,58);
INSERT INTO `page` VALUES (759,8,'Nocookieslogin','sysop',0,0,0,0.666494729392,'20050831104124',759,139);
INSERT INTO `page` VALUES (760,8,'Nocookiesnew','sysop',0,0,0,0.430972897883,'20050831104124',760,218);
INSERT INTO `page` VALUES (761,8,'Nocreativecommons','sysop',0,0,0,0.882710270899,'20050831104124',761,55);
INSERT INTO `page` VALUES (762,8,'Nocredits','sysop',0,0,0,0.672208250053,'20050831104124',762,49);
INSERT INTO `page` VALUES (763,8,'Nodb','sysop',0,0,0,0.393913283865,'20050831104124',763,36);
INSERT INTO `page` VALUES (764,8,'Nodublincore','sysop',0,0,0,0.639523975724,'20050831104124',764,50);
INSERT INTO `page` VALUES (765,8,'Noemail','sysop',0,0,0,0.991170021058,'20050831104124',765,49);
INSERT INTO `page` VALUES (766,8,'Noemailprefs','sysop',0,0,0,0.077517287075,'20050831104124',766,121);
INSERT INTO `page` VALUES (767,8,'Noemailtext','sysop',0,0,0,0.824231142103,'20050831104124',767,119);
INSERT INTO `page` VALUES (768,8,'Noemailtitle','sysop',0,0,0,0.307594548056,'20050831104124',768,20);
INSERT INTO `page` VALUES (769,8,'Nogomatch','sysop',0,0,0,0.335998187231,'20050831104124',769,116);
INSERT INTO `page` VALUES (770,8,'Nohistory','sysop',0,0,0,0.230420535655,'20050831104124',770,53);
INSERT INTO `page` VALUES (771,8,'Noimage','sysop',0,0,0,0.674580033453,'20050831104124',771,51);
INSERT INTO `page` VALUES (772,8,'Noimages','sysop',0,0,0,0.546031481391,'20050831104124',772,15);
INSERT INTO `page` VALUES (773,8,'Nolinkshere','sysop',0,0,0,0.596315655153,'20050831104124',773,30);
INSERT INTO `page` VALUES (774,8,'Nolinkstoimage','sysop',0,0,0,0.302192756579,'20050831104124',774,33);
INSERT INTO `page` VALUES (775,8,'Noname','sysop',0,0,0,0.472043017671,'20050831104124',775,40);
INSERT INTO `page` VALUES (776,8,'Nonefound','sysop',0,0,0,0.408914121371,'20050831104124',776,161);
INSERT INTO `page` VALUES (777,8,'Nonunicodebrowser','sysop',0,0,0,0.833348542335,'20050831104124',777,198);
INSERT INTO `page` VALUES (778,8,'Nospecialpagetext','sysop',0,0,0,0.013938515591,'20050831104124',778,69);
INSERT INTO `page` VALUES (779,8,'Nosuchaction','sysop',0,0,0,0.825853134111,'20050831104124',779,26);
INSERT INTO `page` VALUES (780,8,'Nosuchactiontext','sysop',0,0,0,0.872800617929,'20050831104124',780,63);
INSERT INTO `page` VALUES (781,8,'Nosuchspecialpage','sysop',0,0,0,0.02802785914,'20050831104124',781,32);
INSERT INTO `page` VALUES (782,8,'Nosuchuser','sysop',0,0,0,0.112400846578,'20050831104124',782,117);
INSERT INTO `page` VALUES (783,8,'Nosuchusershort','sysop',0,0,0,0.546383128011,'20050831104124',783,55);
INSERT INTO `page` VALUES (784,8,'Notacceptable','sysop',0,0,0,0.422377829713,'20050831104124',784,68);
INSERT INTO `page` VALUES (785,8,'Notanarticle','sysop',0,0,0,0.962970479577,'20050831104124',785,12);
INSERT INTO `page` VALUES (786,8,'Notargettext','sysop',0,0,0,0.929696173417,'20050831104124',786,79);
INSERT INTO `page` VALUES (787,8,'Notargettitle','sysop',0,0,0,0.711674022564,'20050831104124',787,22);
INSERT INTO `page` VALUES (788,8,'Note','sysop',0,0,0,0.105458310713,'20050831104124',788,25);
INSERT INTO `page` VALUES (789,8,'Notextmatches','sysop',0,0,0,0.976014229084,'20050831104124',789,24);
INSERT INTO `page` VALUES (790,8,'Notitlematches','sysop',0,0,0,0.133942078118,'20050831104124',790,24);
INSERT INTO `page` VALUES (791,8,'Notloggedin','sysop',0,0,0,0.003026041116,'20050831104124',791,16);
INSERT INTO `page` VALUES (792,8,'Nov','sysop',0,0,0,0.598426103729,'20050831104124',792,3);
INSERT INTO `page` VALUES (793,8,'November','sysop',0,0,0,0.223229211189,'20050831104124',793,8);
INSERT INTO `page` VALUES (794,8,'Nowatchlist','sysop',0,0,0,0.913299198496,'20050831104124',794,54);
INSERT INTO `page` VALUES (795,8,'Nowiki_sample','sysop',0,0,0,0.340161064446,'20050831104124',795,34);
INSERT INTO `page` VALUES (796,8,'Nowiki_tip','sysop',0,0,0,0.947424055772,'20050831104124',796,19);
INSERT INTO `page` VALUES (797,8,'Nstab-category','sysop',0,0,0,0.956056334653,'20050831104124',797,9);
INSERT INTO `page` VALUES (798,8,'Nstab-help','sysop',0,0,0,0.831903621631,'20050831104124',798,5);
INSERT INTO `page` VALUES (799,8,'Nstab-image','sysop',0,0,0,0.798481227677,'20050831104124',799,4);
INSERT INTO `page` VALUES (800,8,'Nstab-main','sysop',0,0,0,0.006165651116,'20050831104124',800,7);
INSERT INTO `page` VALUES (801,8,'Nstab-media','sysop',0,0,0,0.18164895542,'20050831104124',801,5);
INSERT INTO `page` VALUES (802,8,'Nstab-mediawiki','sysop',0,0,0,0.11988202429,'20050831104124',802,7);
INSERT INTO `page` VALUES (803,8,'Nstab-special','sysop',0,0,0,0.654983541992,'20050831104124',803,7);
INSERT INTO `page` VALUES (804,8,'Nstab-template','sysop',0,0,0,0.192417114891,'20050831104124',804,8);
INSERT INTO `page` VALUES (805,8,'Nstab-user','sysop',0,0,0,0.969430302332,'20050831104124',805,13);
INSERT INTO `page` VALUES (806,8,'Nstab-wp','sysop',0,0,0,0.528899421752,'20050831104124',806,12);
INSERT INTO `page` VALUES (807,8,'Numauthors','sysop',0,0,0,0.29056799909,'20050831104124',807,40);
INSERT INTO `page` VALUES (808,8,'Number_of_watching_users_RCview','sysop',0,0,0,0.157647148089,'20050831104124',808,4);
INSERT INTO `page` VALUES (809,8,'Number_of_watching_users_pageview','sysop',0,0,0,0.337455365935,'20050831104124',809,36);
INSERT INTO `page` VALUES (810,8,'Numedits','sysop',0,0,0,0.644567484609,'20050831104124',810,29);
INSERT INTO `page` VALUES (811,8,'Numtalkauthors','sysop',0,0,0,0.088697015725,'20050831104124',811,48);
INSERT INTO `page` VALUES (812,8,'Numtalkedits','sysop',0,0,0,0.017766028013,'20050831104124',812,37);
INSERT INTO `page` VALUES (813,8,'Numwatchers','sysop',0,0,0,0.432878441905,'20050831104124',813,22);
INSERT INTO `page` VALUES (814,8,'Nviews','sysop',0,0,0,0.321948575747,'20050831104124',814,11);
INSERT INTO `page` VALUES (815,8,'Oct','sysop',0,0,0,0.867830597667,'20050831104124',815,3);
INSERT INTO `page` VALUES (816,8,'October','sysop',0,0,0,0.429957008671,'20050831104124',816,7);
INSERT INTO `page` VALUES (817,8,'Ok','sysop',0,0,0,0.844737754738,'20050831104124',817,2);
INSERT INTO `page` VALUES (818,8,'Oldpassword','sysop',0,0,0,0.444836093959,'20050831104124',818,14);
INSERT INTO `page` VALUES (819,8,'Orig','sysop',0,0,0,0.651870536502,'20050831104124',819,8);
INSERT INTO `page` VALUES (820,8,'Orphans','sysop',0,0,0,0.557355880564,'20050831104124',820,16);
INSERT INTO `page` VALUES (821,8,'Othercontribs','sysop',0,0,0,0.843850849305,'20050831104124',821,20);
INSERT INTO `page` VALUES (822,8,'Otherlanguages','sysop',0,0,0,0.321818674386,'20050831104124',822,15);
INSERT INTO `page` VALUES (823,8,'Others','sysop',0,0,0,0.150626712447,'20050831104124',823,6);
INSERT INTO `page` VALUES (824,8,'Pagemovedsub','sysop',0,0,0,0.15314622653,'20050831104124',824,24);
INSERT INTO `page` VALUES (825,8,'Pagemovedtext','sysop',0,0,0,0.23708009362,'20050831104124',825,48);
INSERT INTO `page` VALUES (826,8,'Pagetitle','sysop',0,0,0,0.531193762638,'20050831104124',826,17);
INSERT INTO `page` VALUES (827,8,'Passwordremindertext','sysop',0,0,0,0.834426640528,'20050831104124',827,192);
INSERT INTO `page` VALUES (828,8,'Passwordremindertitle','sysop',0,0,0,0.270338582974,'20050831104124',828,35);
INSERT INTO `page` VALUES (829,8,'Passwordsent','sysop',0,0,0,0.167109485608,'20050831104124',829,136);
INSERT INTO `page` VALUES (830,8,'Passwordtooshort','sysop',0,0,0,0.961079808032,'20050831104124',830,66);
INSERT INTO `page` VALUES (831,8,'Perfcached','sysop',0,0,0,0.61711126442,'20050831104124',831,81);
INSERT INTO `page` VALUES (832,8,'Perfdisabled','sysop',0,0,0,0.157138284108,'20050831104124',832,186);
INSERT INTO `page` VALUES (833,8,'Perfdisabledsub','sysop',0,0,0,0.62833025083,'20050831104124',833,40);
INSERT INTO `page` VALUES (834,8,'Personaltools','sysop',0,0,0,0.367087579128,'20050831104124',834,23);
INSERT INTO `page` VALUES (835,8,'Popularpages','sysop',0,0,0,0.702398975182,'20050831104124',835,15);
INSERT INTO `page` VALUES (836,8,'Portal','sysop',0,0,0,0.489022100913,'20050831104124',836,19);
INSERT INTO `page` VALUES (837,8,'Portal-url','sysop',0,0,0,0.200808616909,'20050831104124',837,15);
INSERT INTO `page` VALUES (838,8,'Postcomment','sysop',0,0,0,0.841169321231,'20050831104124',838,21);
INSERT INTO `page` VALUES (839,8,'Poweredby','sysop',0,0,0,0.486838931785,'20050831104124',839,89);
INSERT INTO `page` VALUES (840,8,'Powersearch','sysop',0,0,0,0.341367951342,'20050831104124',840,5);
INSERT INTO `page` VALUES (841,8,'Powersearchtext','sysop',0,0,0,0.076289881917,'20050831104124',841,88);
INSERT INTO `page` VALUES (842,8,'Preferences','sysop',0,0,0,0.247639696024,'20050831104124',842,13);
INSERT INTO `page` VALUES (843,8,'Prefs-help-email','sysop',0,0,0,0.074300009682,'20050831104124',843,280);
INSERT INTO `page` VALUES (844,8,'Prefs-help-email-enotif','sysop',0,0,0,0.527930080466,'20050831104124',844,102);
INSERT INTO `page` VALUES (845,8,'Prefs-help-realname','sysop',0,0,0,0.528936216086,'20050831104124',845,121);
INSERT INTO `page` VALUES (846,8,'Prefs-misc','sysop',0,0,0,0.212987615209,'20050831104124',846,26);
INSERT INTO `page` VALUES (847,8,'Prefs-personal','sysop',0,0,0,0.67932200182,'20050831104124',847,13);
INSERT INTO `page` VALUES (848,8,'Prefs-rc','sysop',0,0,0,0.216928188129,'20050831104124',848,45);
INSERT INTO `page` VALUES (849,8,'Prefslogintext','sysop',0,0,0,0.504278588518,'20050831104124',849,60);
INSERT INTO `page` VALUES (850,8,'Prefsnologin','sysop',0,0,0,0.571737221071,'20050831104124',850,16);
INSERT INTO `page` VALUES (851,8,'Prefsnologintext','sysop',0,0,0,0.763701150726,'20050831104124',851,84);
INSERT INTO `page` VALUES (852,8,'Prefsreset','sysop',0,0,0,0.539941550719,'20050831104124',852,49);
INSERT INTO `page` VALUES (853,8,'Preview','sysop',0,0,0,0.332754806452,'20050831104124',853,8);
INSERT INTO `page` VALUES (854,8,'Previewconflict','sysop',0,0,0,0.049839993149,'20050831104124',854,116);
INSERT INTO `page` VALUES (855,8,'Previewnote','sysop',0,0,0,0.037828949135,'20050831104124',855,69);
INSERT INTO `page` VALUES (856,8,'Previousdiff','sysop',0,0,0,0.502480835093,'20050831104124',856,20);
INSERT INTO `page` VALUES (857,8,'Previousrevision','sysop',0,0,0,0.608927011587,'20050831104124',857,28);
INSERT INTO `page` VALUES (858,8,'Prevn','sysop',0,0,0,0.562217495402,'20050831104124',858,12);
INSERT INTO `page` VALUES (859,8,'Print','sysop',0,0,0,0.461276106173,'20050831104124',859,5);
INSERT INTO `page` VALUES (860,8,'Printableversion','sysop',0,0,0,0.358744956798,'20050831104125',860,12);
INSERT INTO `page` VALUES (861,8,'Printsubtitle','sysop',0,0,0,0.358132593257,'20050831104125',861,16);
INSERT INTO `page` VALUES (862,8,'Protect','sysop',0,0,0,0.801739485013,'20050831104125',862,9);
INSERT INTO `page` VALUES (863,8,'Protectcomment','sysop',0,0,0,0.906393520101,'20050831104125',863,18);
INSERT INTO `page` VALUES (864,8,'Protectedarticle','sysop',0,0,0,0.734692338989,'20050831104125',864,25);
INSERT INTO `page` VALUES (865,8,'Protectedpage','sysop',0,0,0,0.692120918682,'20050831104125',865,17);
INSERT INTO `page` VALUES (866,8,'Protectedpagewarning','sysop',0,0,0,0.404327229831,'20050831104125',866,206);
INSERT INTO `page` VALUES (867,8,'Protectedtext','sysop',0,0,0,0.505658991167,'20050831104125',867,187);
INSERT INTO `page` VALUES (868,8,'Protectlogpage','sysop',0,0,0,0.083722695701,'20050831104125',868,20);
INSERT INTO `page` VALUES (869,8,'Protectlogtext','sysop',0,0,0,0.215080338076,'20050831104125',869,105);
INSERT INTO `page` VALUES (870,8,'Protectmoveonly','sysop',0,0,0,0.088548723552,'20050831104125',870,23);
INSERT INTO `page` VALUES (871,8,'Protectpage','sysop',0,0,0,0.537727140333,'20050831104125',871,17);
INSERT INTO `page` VALUES (872,8,'Protectsub','sysop',0,0,0,0.461840675515,'20050831104125',872,18);
INSERT INTO `page` VALUES (873,8,'Protectthispage','sysop',0,0,0,0.535302520975,'20050831104125',873,17);
INSERT INTO `page` VALUES (874,8,'Proxyblocker','sysop',0,0,0,0.397830534983,'20050831104125',874,12);
INSERT INTO `page` VALUES (875,8,'Proxyblockreason','sysop',0,0,0,0.972483101501,'20050831104125',875,190);
INSERT INTO `page` VALUES (876,8,'Proxyblocksuccess','sysop',0,0,0,0.392648284871,'20050831104125',876,7);
INSERT INTO `page` VALUES (877,8,'Pubmedurl','sysop',0,0,0,0.308463476211,'20050831104125',877,95);
INSERT INTO `page` VALUES (878,8,'Qbbrowse','sysop',0,0,0,0.636222331548,'20050831104125',878,9);
INSERT INTO `page` VALUES (879,8,'Qbedit','sysop',0,0,0,0.573822594956,'20050831104125',879,7);
INSERT INTO `page` VALUES (880,8,'Qbfind','sysop',0,0,0,0.125636325153,'20050831104125',880,6);
INSERT INTO `page` VALUES (881,8,'Qbmyoptions','sysop',0,0,0,0.122294247329,'20050831104125',881,13);
INSERT INTO `page` VALUES (882,8,'Qbpageinfo','sysop',0,0,0,0.602488982746,'20050831104125',882,11);
INSERT INTO `page` VALUES (883,8,'Qbpageoptions','sysop',0,0,0,0.930718402349,'20050831104125',883,14);
INSERT INTO `page` VALUES (884,8,'Qbsettings','sysop',0,0,0,0.266808761212,'20050831104125',884,12);
INSERT INTO `page` VALUES (885,8,'Qbspecialpages','sysop',0,0,0,0.872166886049,'20050831104125',885,13);
INSERT INTO `page` VALUES (886,8,'Randompage','sysop',0,0,0,0.961331599259,'20050831104125',886,19);
INSERT INTO `page` VALUES (887,8,'Randompage-url','sysop',0,0,0,0.561174405103,'20050831104125',887,14);
INSERT INTO `page` VALUES (888,8,'Range_block_disabled','sysop',0,0,0,0.67331788413,'20050831104125',888,69);
INSERT INTO `page` VALUES (889,8,'Rchide','sysop',0,0,0,0.049939296304,'20050831104125',889,88);
INSERT INTO `page` VALUES (890,8,'Rclinks','sysop',0,0,0,0.152126044875,'20050831104125',890,60);
INSERT INTO `page` VALUES (891,8,'Rclistfrom','sysop',0,0,0,0.57879428577,'20050831104125',891,30);
INSERT INTO `page` VALUES (892,8,'Rcliu','sysop',0,0,0,0.298666747709,'20050831104125',892,41);
INSERT INTO `page` VALUES (893,8,'Rcloaderr','sysop',0,0,0,0.798949834783,'20050831104125',893,23);
INSERT INTO `page` VALUES (894,8,'Rclsub','sysop',0,0,0,0.719697195243,'20050831104125',894,22);
INSERT INTO `page` VALUES (895,8,'Rcnote','sysop',0,0,0,0.145168950732,'20050831104125',895,127);
INSERT INTO `page` VALUES (896,8,'Rcnotefrom','sysop',0,0,0,0.361078914491,'20050831104125',896,68);
INSERT INTO `page` VALUES (897,8,'Rcpatroldisabled','sysop',0,0,0,0.771230987954,'20050831104125',897,30);
INSERT INTO `page` VALUES (898,8,'Rcpatroldisabledtext','sysop',0,0,0,0.966708236747,'20050831104125',898,56);
INSERT INTO `page` VALUES (899,8,'Readonly','sysop',0,0,0,0.543569396863,'20050831104125',899,22);
INSERT INTO `page` VALUES (900,8,'Readonly_lag','sysop',0,0,0,0.487088127246,'20050831104125',900,98);
INSERT INTO `page` VALUES (901,8,'Readonlytext','sysop',0,0,0,0.75729366229,'20050831104125',901,127);
INSERT INTO `page` VALUES (902,8,'Readonlywarning','sysop',0,0,0,0.716891767123,'20050831104125',902,247);
INSERT INTO `page` VALUES (903,8,'Recentchanges','sysop',0,0,0,0.65121263789,'20050831104125',903,18);
INSERT INTO `page` VALUES (904,8,'Recentchanges-url','sysop',0,0,0,0.384563230893,'20050831104125',904,21);
INSERT INTO `page` VALUES (905,8,'Recentchangesall','sysop',0,0,0,0.503415908689,'20050831104125',905,3);
INSERT INTO `page` VALUES (906,8,'Recentchangescount','sysop',0,0,0,0.338860057908,'20050831104125',906,27);
INSERT INTO `page` VALUES (907,8,'Recentchangeslinked','sysop',0,0,0,0.234261067114,'20050831104125',907,16);
INSERT INTO `page` VALUES (908,8,'Recentchangestext','sysop',0,0,0,0.869615745831,'20050831104125',908,541);
INSERT INTO `page` VALUES (909,8,'Redirectedfrom','sysop',0,0,0,0.892054330917,'20050831104125',909,23);
INSERT INTO `page` VALUES (910,8,'Remembermypassword','sysop',0,0,0,0.148197760166,'20050831104125',910,21);
INSERT INTO `page` VALUES (911,8,'Removechecked','sysop',0,0,0,0.693542788414,'20050831104125',911,28);
INSERT INTO `page` VALUES (912,8,'Removedwatch','sysop',0,0,0,0.324518712901,'20050831104125',912,34);
INSERT INTO `page` VALUES (913,8,'Removedwatchtext','sysop',0,0,0,0.142870284748,'20050831104125',913,60);
INSERT INTO `page` VALUES (914,8,'Removingchecked','sysop',0,0,0,0.583869018287,'20050831104125',914,72);
INSERT INTO `page` VALUES (915,8,'Renamegrouplogentry','sysop',0,0,0,0.716855461136,'20050831104125',915,22);
INSERT INTO `page` VALUES (916,8,'Resetprefs','sysop',0,0,0,0.306571512361,'20050831104125',916,27);
INSERT INTO `page` VALUES (917,8,'Restorelink','sysop',0,0,0,0.464563215162,'20050831104125',917,35);
INSERT INTO `page` VALUES (918,8,'Restrictedpheading','sysop',0,0,0,0.762455415072,'20050831104125',918,24);
INSERT INTO `page` VALUES (919,8,'Resultsperpage','sysop',0,0,0,0.650623874235,'20050831104125',919,17);
INSERT INTO `page` VALUES (920,8,'Retrievedfrom','sysop',0,0,0,0.654183369549,'20050831104125',920,8);
INSERT INTO `page` VALUES (921,8,'Returnto','sysop',0,0,0,0.262601091664,'20050831104125',921,14);
INSERT INTO `page` VALUES (922,8,'Retypenew','sysop',0,0,0,0.246925091355,'20050831104125',922,24);
INSERT INTO `page` VALUES (923,8,'Reupload','sysop',0,0,0,0.592644602072,'20050831104125',923,16);
INSERT INTO `page` VALUES (924,8,'Reuploaddesc','sysop',0,0,0,0.369105941804,'20050831104125',924,28);
INSERT INTO `page` VALUES (925,8,'Reverted','sysop',0,0,0,0.7473007419,'20050831104125',925,36);
INSERT INTO `page` VALUES (926,8,'Revertimg','sysop',0,0,0,0.465750228021,'20050831104125',926,13);
INSERT INTO `page` VALUES (927,8,'Revertmove','sysop',0,0,0,0.477586349997,'20050831104125',927,6);
INSERT INTO `page` VALUES (928,8,'Revertpage','sysop',0,0,0,0.961576831327,'20050831104125',928,46);
INSERT INTO `page` VALUES (929,8,'Revhistory','sysop',0,0,0,0.622672782023,'20050831104125',929,18);
INSERT INTO `page` VALUES (930,8,'Revisionasof','sysop',0,0,0,0.043547922138,'20050831104125',930,14);
INSERT INTO `page` VALUES (931,8,'Revisionasofwithlink','sysop',0,0,0,0.881857823086,'20050831104125',931,31);
INSERT INTO `page` VALUES (932,8,'Revnotfound','sysop',0,0,0,0.905522245772,'20050831104125',932,34);
INSERT INTO `page` VALUES (933,8,'Revnotfoundtext','sysop',0,0,0,0.146013235803,'20050831104125',933,124);
INSERT INTO `page` VALUES (934,8,'Rfcurl','sysop',0,0,0,0.214075324444,'20050831104125',934,35);
INSERT INTO `page` VALUES (935,8,'Rights','sysop',0,0,0,0.042650493895,'20050831104125',935,7);
INSERT INTO `page` VALUES (936,8,'Rightslogtext','sysop',0,0,0,0.953325908012,'20050831104125',936,40);
INSERT INTO `page` VALUES (937,8,'Rollback','sysop',0,0,0,0.05743121632,'20050831104125',937,28);
INSERT INTO `page` VALUES (938,8,'Rollback_short','sysop',0,0,0,0.739738970667,'20050831104125',938,8);
INSERT INTO `page` VALUES (939,8,'Rollbackfailed','sysop',0,0,0,0.982001823827,'20050831104125',939,24);
INSERT INTO `page` VALUES (940,8,'Rollbacklink','sysop',0,0,0,0.682385052801,'20050831104125',940,8);
INSERT INTO `page` VALUES (941,8,'Rows','sysop',0,0,0,0.451071660656,'20050831104125',941,6);
INSERT INTO `page` VALUES (942,8,'Saturday','sysop',0,0,0,0.419275277707,'20050831104125',942,7);
INSERT INTO `page` VALUES (943,8,'Savearticle','sysop',0,0,0,0.599358130902,'20050831104125',943,17);
INSERT INTO `page` VALUES (944,8,'Savedprefs','sysop',0,0,0,0.313778522571,'20050831104125',944,38);
INSERT INTO `page` VALUES (945,8,'Savefile','sysop',0,0,0,0.087959198888,'20050831104125',945,15);
INSERT INTO `page` VALUES (946,8,'Savegroup','sysop',0,0,0,0.15088813563,'20050831104125',946,10);
INSERT INTO `page` VALUES (947,8,'Saveprefs','sysop',0,0,0,0.79057978786,'20050831104125',947,23);
INSERT INTO `page` VALUES (948,8,'Saveusergroups','sysop',0,0,0,0.24301720381,'20050831104125',948,16);
INSERT INTO `page` VALUES (949,8,'Scarytranscludedisabled','sysop',0,0,0,0.450888169531,'20050831104125',949,36);
INSERT INTO `page` VALUES (950,8,'Scarytranscludefailed','sysop',0,0,0,0.921441819602,'20050831104125',950,37);
INSERT INTO `page` VALUES (951,8,'Scarytranscludetoolong','sysop',0,0,0,0.377188406384,'20050831104125',951,24);
INSERT INTO `page` VALUES (952,8,'Search','sysop',0,0,0,0.60980807839,'20050831104125',952,5);
INSERT INTO `page` VALUES (953,8,'Searchdisabled','sysop',0,0,0,0.556307285365,'20050831104125',953,201);
INSERT INTO `page` VALUES (954,8,'Searchfulltext','sysop',0,0,0,0.85238779784,'20050831104125',954,16);
INSERT INTO `page` VALUES (955,8,'Searchquery','sysop',0,0,0,0.430773633267,'20050831104125',955,25);
INSERT INTO `page` VALUES (956,8,'Searchresults','sysop',0,0,0,0.256115383092,'20050831104125',956,14);
INSERT INTO `page` VALUES (957,8,'Searchresultshead','sysop',0,0,0,0.269906583555,'20050831104125',957,14);
INSERT INTO `page` VALUES (958,8,'Searchresulttext','sysop',0,0,0,0.093931214551,'20050831104125',958,91);
INSERT INTO `page` VALUES (959,8,'Sectionlink','sysop',0,0,0,0.245554050597,'20050831104125',959,6);
INSERT INTO `page` VALUES (960,8,'Selectnewerversionfordiff','sysop',0,0,0,0.340942768369,'20050831104125',960,44);
INSERT INTO `page` VALUES (961,8,'Selectolderversionfordiff','sysop',0,0,0,0.995811541377,'20050831104125',961,45);
INSERT INTO `page` VALUES (962,8,'Selflinks','sysop',0,0,0,0.691431576035,'20050831104125',962,37);
INSERT INTO `page` VALUES (963,8,'Selflinkstext','sysop',0,0,0,0.49626624045,'20050831104125',963,71);
INSERT INTO `page` VALUES (964,8,'Selfmove','sysop',0,0,0,0.073689153298,'20050831104125',964,74);
INSERT INTO `page` VALUES (965,8,'Sep','sysop',0,0,0,0.230417841576,'20050831104125',965,3);
INSERT INTO `page` VALUES (966,8,'September','sysop',0,0,0,0.504352572289,'20050831104125',966,9);
INSERT INTO `page` VALUES (967,8,'Servertime','sysop',0,0,0,0.213757427356,'20050831104125',967,28);
INSERT INTO `page` VALUES (968,8,'Sessionfailure','sysop',0,0,0,0.33198794022,'20050831104125',968,194);
INSERT INTO `page` VALUES (969,8,'Set_rights_fail','sysop',0,0,0,0.445497403336,'20050831104125',969,103);
INSERT INTO `page` VALUES (970,8,'Set_user_rights','sysop',0,0,0,0.467184096408,'20050831104125',970,21);
INSERT INTO `page` VALUES (971,8,'Setbureaucratflag','sysop',0,0,0,0.602685709956,'20050831104125',971,19);
INSERT INTO `page` VALUES (972,8,'Setstewardflag','sysop',0,0,0,0.921011595674,'20050831104125',972,16);
INSERT INTO `page` VALUES (973,8,'Shareddescriptionfollows','sysop',0,0,0,0.925451434005,'20050831104125',973,1);
INSERT INTO `page` VALUES (974,8,'Sharedupload','sysop',0,0,0,0.725690573846,'20050831104125',974,63);
INSERT INTO `page` VALUES (975,8,'Shareduploadwiki','sysop',0,0,0,0.434744850572,'20050831104125',975,66);
INSERT INTO `page` VALUES (976,8,'Shortpages','sysop',0,0,0,0.74030033651,'20050831104125',976,13);
INSERT INTO `page` VALUES (977,8,'Show','sysop',0,0,0,0.132453246725,'20050831104125',977,10);
INSERT INTO `page` VALUES (978,8,'Showbigimage','sysop',0,0,0,0.997166080394,'20050831104125',978,47);
INSERT INTO `page` VALUES (979,8,'Showdiff','sysop',0,0,0,0.721917486711,'20050831104125',979,22);
INSERT INTO `page` VALUES (980,8,'Showhideminor','sysop',0,0,0,0.018785074644,'20050831104125',980,73);
INSERT INTO `page` VALUES (981,8,'Showingresults','sysop',0,0,0,0.241573991189,'20050831104125',981,57);
INSERT INTO `page` VALUES (982,8,'Showingresultsnum','sysop',0,0,0,0.212660433361,'20050831104125',982,57);
INSERT INTO `page` VALUES (983,8,'Showlast','sysop',0,0,0,0.968247040093,'20050831104125',983,46);
INSERT INTO `page` VALUES (984,8,'Showpreview','sysop',0,0,0,0.023898054656,'20050831104125',984,15);
INSERT INTO `page` VALUES (985,8,'Showtoc','sysop',0,0,0,0.612765733681,'20050831104125',985,8);
INSERT INTO `page` VALUES (986,8,'Sidebar','sysop',0,0,0,0.961362394726,'20050831104125',986,202);
INSERT INTO `page` VALUES (987,8,'Sig_tip','sysop',0,0,0,0.048900478648,'20050831104125',987,29);
INSERT INTO `page` VALUES (988,8,'Siteadminpheading','sysop',0,0,0,0.639035160013,'20050831104125',988,15);
INSERT INTO `page` VALUES (989,8,'Sitenotice','sysop',0,0,0,0.037170132506,'20050831104125',989,1);
INSERT INTO `page` VALUES (990,8,'Sitestats','sysop',0,0,0,0.257657797678,'20050831104125',990,15);
INSERT INTO `page` VALUES (991,8,'Sitestatstext','sysop',0,0,0,0.235699271537,'20050831104125',991,489);
INSERT INTO `page` VALUES (992,8,'Sitesubtitle','sysop',0,0,0,0.853914395061,'20050831104125',992,26);
INSERT INTO `page` VALUES (993,8,'Sitesupport','sysop',0,0,0,0.370954447004,'20050831104125',993,7);
INSERT INTO `page` VALUES (994,8,'Sitesupport-url','sysop',0,0,0,0.948802852504,'20050831104125',994,20);
INSERT INTO `page` VALUES (995,8,'Sitetitle','sysop',0,0,0,0.39370326299,'20050831104125',995,12);
INSERT INTO `page` VALUES (996,8,'Siteuser','sysop',0,0,0,0.899849342902,'20050831104125',996,24);
INSERT INTO `page` VALUES (997,8,'Siteusers','sysop',0,0,0,0.406908279107,'20050831104125',997,24);
INSERT INTO `page` VALUES (998,8,'Skin','sysop',0,0,0,0.265089327849,'20050831104125',998,4);
INSERT INTO `page` VALUES (999,8,'Skinpreview','sysop',0,0,0,0.934909422051,'20050831104125',999,9);
INSERT INTO `page` VALUES (1000,8,'Sorbs','sysop',0,0,0,0.825663340291,'20050831104125',1000,11);
INSERT INTO `page` VALUES (1001,8,'Sorbs_create_account_reason','sysop',0,0,0,0.627717033994,'20050831104125',1001,114);
INSERT INTO `page` VALUES (1002,8,'Sorbsreason','sysop',0,0,0,0.646516822932,'20050831104125',1002,85);
INSERT INTO `page` VALUES (1003,8,'Sourcefilename','sysop',0,0,0,0.426445426768,'20050831104125',1003,15);
INSERT INTO `page` VALUES (1004,8,'Spamprotectionmatch','sysop',0,0,0,0.704235333076,'20050831104125',1004,56);
INSERT INTO `page` VALUES (1005,8,'Spamprotectiontext','sysop',0,0,0,0.180332289466,'20050831104125',1005,114);
INSERT INTO `page` VALUES (1006,8,'Spamprotectiontitle','sysop',0,0,0,0.948214313231,'20050831104125',1006,22);
INSERT INTO `page` VALUES (1007,8,'Speciallogtitlelabel','sysop',0,0,0,0.185806557203,'20050831104125',1007,6);
INSERT INTO `page` VALUES (1008,8,'Specialloguserlabel','sysop',0,0,0,0.266041294618,'20050831104125',1008,9);
INSERT INTO `page` VALUES (1009,8,'Specialpage','sysop',0,0,0,0.784539503976,'20050831104125',1009,12);
INSERT INTO `page` VALUES (1010,8,'Specialpages','sysop',0,0,0,0.992037595678,'20050831104125',1010,13);
INSERT INTO `page` VALUES (1011,8,'Spheading','sysop',0,0,0,0.527987841682,'20050831104125',1011,13);
INSERT INTO `page` VALUES (1012,8,'Sqlhidden','sysop',0,0,0,0.074126454533,'20050831104125',1012,18);
INSERT INTO `page` VALUES (1013,8,'Statistics','sysop',0,0,0,0.823023287206,'20050831104125',1013,9);
INSERT INTO `page` VALUES (1014,8,'Storedversion','sysop',0,0,0,0.22406160262,'20050831104125',1014,20);
INSERT INTO `page` VALUES (1015,8,'Stubthreshold','sysop',0,0,0,0.020529907505,'20050831104125',1015,27);
INSERT INTO `page` VALUES (1016,8,'Subcategories','sysop',0,0,0,0.495591713758,'20050831104125',1016,15);
INSERT INTO `page` VALUES (1017,8,'Subcategorycount','sysop',0,0,0,0.010316623574,'20050831104125',1017,39);
INSERT INTO `page` VALUES (1018,8,'Subcategorycount1','sysop',0,0,0,0.561799532352,'20050831104125',1018,41);
INSERT INTO `page` VALUES (1019,8,'Subject','sysop',0,0,0,0.482863596055,'20050831104125',1019,7);
INSERT INTO `page` VALUES (1020,8,'Subjectpage','sysop',0,0,0,0.170392986611,'20050831104125',1020,12);
INSERT INTO `page` VALUES (1021,8,'Successfulupload','sysop',0,0,0,0.803084077513,'20050831104125',1021,23);
INSERT INTO `page` VALUES (1022,8,'Summary','sysop',0,0,0,0.263587984466,'20050831104125',1022,15);
INSERT INTO `page` VALUES (1023,8,'Sunday','sysop',0,0,0,0.638941987306,'20050831104125',1023,7);
INSERT INTO `page` VALUES (1024,8,'Sysoptext','sysop',0,0,0,0.310670433906,'20050831104125',1024,116);
INSERT INTO `page` VALUES (1025,8,'Sysoptitle','sysop',0,0,0,0.332534003328,'20050831104125',1025,22);
INSERT INTO `page` VALUES (1026,8,'Tableform','sysop',0,0,0,0.774598059179,'20050831104125',1026,7);
INSERT INTO `page` VALUES (1027,8,'Tagline','sysop',0,0,0,0.088044733529,'20050831104125',1027,45);
INSERT INTO `page` VALUES (1028,8,'Talk','sysop',0,0,0,0.020045984216,'20050831104125',1028,10);
INSERT INTO `page` VALUES (1029,8,'Talkexists','sysop',0,0,0,0.488195473763,'20050831104125',1029,169);
INSERT INTO `page` VALUES (1030,8,'Talkpage','sysop',0,0,0,0.953346379911,'20050831104125',1030,10);
INSERT INTO `page` VALUES (1031,8,'Talkpagemoved','sysop',0,0,0,0.027426463341,'20050831104125',1031,51);
INSERT INTO `page` VALUES (1032,8,'Talkpagenotmoved','sysop',0,0,0,0.987298062272,'20050831104125',1032,64);
INSERT INTO `page` VALUES (1033,8,'Talkpagetext','sysop',0,0,0,0.135649089997,'20050831104125',1033,31);
INSERT INTO `page` VALUES (1034,8,'Templatesused','sysop',0,0,0,0.637209988067,'20050831104125',1034,28);
INSERT INTO `page` VALUES (1035,8,'Textboxsize','sysop',0,0,0,0.342474732891,'20050831104125',1035,16);
INSERT INTO `page` VALUES (1036,8,'Textmatches','sysop',0,0,0,0.021080667612,'20050831104125',1036,29);
INSERT INTO `page` VALUES (1037,8,'Thisisdeleted','sysop',0,0,0,0.662368906668,'20050831104125',1037,37);
INSERT INTO `page` VALUES (1038,8,'Thumbnail-more','sysop',0,0,0,0.362599691986,'20050831104125',1038,12);
INSERT INTO `page` VALUES (1039,8,'Thumbsize','sysop',0,0,0,0.753258916488,'20050831104125',1039,16);
INSERT INTO `page` VALUES (1040,8,'Thursday','sysop',0,0,0,0.738779056439,'20050831104125',1040,10);
INSERT INTO `page` VALUES (1041,8,'Timezonelegend','sysop',0,0,0,0.178064234882,'20050831104125',1041,8);
INSERT INTO `page` VALUES (1042,8,'Timezoneoffset','sysop',0,0,0,0.733467539473,'20050831104125',1042,11);
INSERT INTO `page` VALUES (1043,8,'Timezonetext','sysop',0,0,0,0.272582852768,'20050831104125',1043,81);
INSERT INTO `page` VALUES (1044,8,'Titlematches','sysop',0,0,0,0.221395868741,'20050831104125',1044,37);
INSERT INTO `page` VALUES (1045,8,'Toc','sysop',0,0,0,0.706256933397,'20050831104125',1045,18);
INSERT INTO `page` VALUES (1046,8,'Tog-editondblclick','sysop',0,0,0,0.137817165768,'20050831104125',1046,46);
INSERT INTO `page` VALUES (1047,8,'Tog-editsection','sysop',0,0,0,0.826584209052,'20050831104125',1047,48);
INSERT INTO `page` VALUES (1048,8,'Tog-editsectiononrightclick','sysop',0,0,0,0.766335856988,'20050831104125',1048,57);
INSERT INTO `page` VALUES (1049,8,'Tog-editwidth','sysop',0,0,0,0.980218564739,'20050831104126',1049,34);
INSERT INTO `page` VALUES (1050,8,'Tog-enotifminoredits','sysop',0,0,0,0.983425452142,'20050831104126',1050,57);
INSERT INTO `page` VALUES (1051,8,'Tog-enotifrevealaddr','sysop',0,0,0,0.961798523896,'20050831104126',1051,58);
INSERT INTO `page` VALUES (1052,8,'Tog-enotifusertalkpages','sysop',0,0,0,0.092124735282,'20050831104126',1052,63);
INSERT INTO `page` VALUES (1053,8,'Tog-enotifwatchlistpages','sysop',0,0,0,0.707905845457,'20050831104126',1053,54);
INSERT INTO `page` VALUES (1054,8,'Tog-externaldiff','sysop',0,0,0,0.568622690372,'20050831104126',1054,28);
INSERT INTO `page` VALUES (1055,8,'Tog-externaleditor','sysop',0,0,0,0.66159650421,'20050831104126',1055,30);
INSERT INTO `page` VALUES (1056,8,'Tog-fancysig','sysop',0,0,0,0.816459039529,'20050831104126',1056,43);
INSERT INTO `page` VALUES (1057,8,'Tog-hideminor','sysop',0,0,0,0.339998498331,'20050831104126',1057,56);
INSERT INTO `page` VALUES (1058,8,'Tog-highlightbroken','sysop',0,0,0,0.772154913118,'20050831104126',1058,37);
INSERT INTO `page` VALUES (1059,8,'Tog-justify','sysop',0,0,0,0.958016602016,'20050831104126',1059,18);
INSERT INTO `page` VALUES (1060,8,'Tog-minordefault','sysop',0,0,0,0.830151123908,'20050831104126',1060,43);
INSERT INTO `page` VALUES (1061,8,'Tog-nocache','sysop',0,0,0,0.249492542692,'20050831104126',1061,24);
INSERT INTO `page` VALUES (1062,8,'Tog-numberheadings','sysop',0,0,0,0.398526134734,'20050831104126',1062,38);
INSERT INTO `page` VALUES (1063,8,'Tog-previewonfirst','sysop',0,0,0,0.682359012772,'20050831104126',1063,39);
INSERT INTO `page` VALUES (1064,8,'Tog-previewontop','sysop',0,0,0,0.655294907507,'20050831104126',1064,40);
INSERT INTO `page` VALUES (1065,8,'Tog-rememberpassword','sysop',0,0,0,0.249501864376,'20050831104126',1065,21);
INSERT INTO `page` VALUES (1066,8,'Tog-shownumberswatching','sysop',0,0,0,0.221139052022,'20050831104126',1066,106);
INSERT INTO `page` VALUES (1067,8,'Tog-showtoc','sysop',0,0,0,0.909173716034,'20050831104126',1067,79);
INSERT INTO `page` VALUES (1068,8,'Tog-showtoolbar','sysop',0,0,0,0.938615801282,'20050831104126',1068,31);
INSERT INTO `page` VALUES (1069,8,'Tog-underline','sysop',0,0,0,0.479185339743,'20050831104126',1069,23);
INSERT INTO `page` VALUES (1070,8,'Tog-usenewrc','sysop',0,0,0,0.278530271816,'20050831104126',1070,64);
INSERT INTO `page` VALUES (1071,8,'Tog-watchdefault','sysop',0,0,0,0.637985920635,'20050831104126',1071,37);
INSERT INTO `page` VALUES (1072,8,'Toolbox','sysop',0,0,0,0.07378041802,'20050831104126',1072,9);
INSERT INTO `page` VALUES (1073,8,'Tooltip-compareselectedversions','sysop',0,0,0,0.630845341139,'20050831104126',1073,84);
INSERT INTO `page` VALUES (1074,8,'Tooltip-diff','sysop',0,0,0,0.119509995749,'20050831104126',1074,54);
INSERT INTO `page` VALUES (1075,8,'Tooltip-minoredit','sysop',0,0,0,0.52015822865,'20050831104126',1075,44);
INSERT INTO `page` VALUES (1076,8,'Tooltip-preview','sysop',0,0,0,0.793350602161,'20050831104126',1076,86);
INSERT INTO `page` VALUES (1077,8,'Tooltip-save','sysop',0,0,0,0.158321022331,'20050831104126',1077,29);
INSERT INTO `page` VALUES (1078,8,'Tooltip-search','sysop',0,0,0,0.426438821483,'20050831104126',1078,14);
INSERT INTO `page` VALUES (1079,8,'Tooltip-watch','sysop',0,0,0,0.85226005096,'20050831104126',1079,31);
INSERT INTO `page` VALUES (1080,8,'Trackback','sysop',0,0,0,0.326704780865,'20050831104126',1080,16);
INSERT INTO `page` VALUES (1081,8,'Trackbackbox','sysop',0,0,0,0.317526961136,'20050831104126',1081,68);
INSERT INTO `page` VALUES (1082,8,'Trackbackdeleteok','sysop',0,0,0,0.395299030768,'20050831104126',1082,39);
INSERT INTO `page` VALUES (1083,8,'Trackbackexcerpt','sysop',0,0,0,0.823909214272,'20050831104126',1083,37);
INSERT INTO `page` VALUES (1084,8,'Trackbacklink','sysop',0,0,0,0.014315399451,'20050831104126',1084,9);
INSERT INTO `page` VALUES (1085,8,'Trackbackremove','sysop',0,0,0,0.963777389445,'20050831104126',1085,14);
INSERT INTO `page` VALUES (1086,8,'Tryexact','sysop',0,0,0,0.958020089805,'20050831104126',1086,15);
INSERT INTO `page` VALUES (1087,8,'Tuesday','sysop',0,0,0,0.030632469306,'20050831104126',1087,8);
INSERT INTO `page` VALUES (1088,8,'Uclinks','sysop',0,0,0,0.266650704824,'20050831104126',1088,58);
INSERT INTO `page` VALUES (1089,8,'Ucnote','sysop',0,0,0,0.861773663571,'20050831104126',1089,87);
INSERT INTO `page` VALUES (1090,8,'Uctop','sysop',0,0,0,0.54414488667,'20050831104126',1090,6);
INSERT INTO `page` VALUES (1091,8,'Unblockip','sysop',0,0,0,0.36534955071,'20050831104126',1091,20);
INSERT INTO `page` VALUES (1092,8,'Unblockiptext','sysop',0,0,0,0.531728992147,'20050831104126',1092,69);
INSERT INTO `page` VALUES (1093,8,'Unblocklink','sysop',0,0,0,0.178709642916,'20050831104126',1093,9);
INSERT INTO `page` VALUES (1094,8,'Unblocklogentry','sysop',0,0,0,0.693924117417,'20050831104126',1094,39);
INSERT INTO `page` VALUES (1095,8,'Uncategorizedcategories','sysop',0,0,0,0.150443186946,'20050831104126',1095,24);
INSERT INTO `page` VALUES (1096,8,'Uncategorizedpages','sysop',0,0,0,0.629581674849,'20050831104126',1096,19);
INSERT INTO `page` VALUES (1097,8,'Undelete','sysop',0,0,0,0.526422566053,'20050831104126',1097,33);
INSERT INTO `page` VALUES (1098,8,'Undelete_short','sysop',0,0,0,0.679058806487,'20050831104126',1098,16);
INSERT INTO `page` VALUES (1099,8,'Undelete_short1','sysop',0,0,0,0.823785550837,'20050831104126',1099,16);
INSERT INTO `page` VALUES (1100,8,'Undeletearticle','sysop',0,0,0,0.136038020632,'20050831104126',1100,36);
INSERT INTO `page` VALUES (1101,8,'Undeletebtn','sysop',0,0,0,0.531347333903,'20050831104126',1101,17);
INSERT INTO `page` VALUES (1102,8,'Undeletedarticle','sysop',0,0,0,0.845981426526,'20050831104126',1102,22);
INSERT INTO `page` VALUES (1103,8,'Undeletedrevisions','sysop',0,0,0,0.808856150249,'20050831104126',1103,21);
INSERT INTO `page` VALUES (1104,8,'Undeletedtext','sysop',0,0,0,0.520662896797,'20050831104126',1104,55);
INSERT INTO `page` VALUES (1105,8,'Undeletehistory','sysop',0,0,0,0.497379023279,'20050831104126',1105,252);
INSERT INTO `page` VALUES (1106,8,'Undeletepage','sysop',0,0,0,0.219206970369,'20050831104126',1106,34);
INSERT INTO `page` VALUES (1107,8,'Undeletepagetext','sysop',0,0,0,0.008516167542,'20050831104126',1107,109);
INSERT INTO `page` VALUES (1108,8,'Undeleterevision','sysop',0,0,0,4.9156058e-05,'20050831104126',1108,25);
INSERT INTO `page` VALUES (1109,8,'Undeleterevisions','sysop',0,0,0,0.62865329932,'20050831104126',1109,23);
INSERT INTO `page` VALUES (1110,8,'Underline-always','sysop',0,0,0,0.854972489753,'20050831104126',1110,6);
INSERT INTO `page` VALUES (1111,8,'Underline-default','sysop',0,0,0,0.36296695686,'20050831104126',1111,15);
INSERT INTO `page` VALUES (1112,8,'Underline-never','sysop',0,0,0,0.815898741725,'20050831104126',1112,5);
INSERT INTO `page` VALUES (1113,8,'Unexpected','sysop',0,0,0,0.858689011697,'20050831104126',1113,29);
INSERT INTO `page` VALUES (1114,8,'Unlockbtn','sysop',0,0,0,0.801562532249,'20050831104126',1114,19);
INSERT INTO `page` VALUES (1115,8,'Unlockconfirm','sysop',0,0,0,0.463088507872,'20050831104126',1115,40);
INSERT INTO `page` VALUES (1116,8,'Unlockdb','sysop',0,0,0,0.917077499127,'20050831104126',1116,19);
INSERT INTO `page` VALUES (1117,8,'Unlockdbsuccesssub','sysop',0,0,0,0.301448385702,'20050831104126',1117,39);
INSERT INTO `page` VALUES (1118,8,'Unlockdbsuccesstext','sysop',0,0,0,0.470254696553,'20050831104126',1118,45);
INSERT INTO `page` VALUES (1119,8,'Unlockdbtext','sysop',0,0,0,0.868707004752,'20050831104126',1119,133);
INSERT INTO `page` VALUES (1120,8,'Unprotect','sysop',0,0,0,0.065516620002,'20050831104126',1120,9);
INSERT INTO `page` VALUES (1121,8,'Unprotectcomment','sysop',0,0,0,0.124612918347,'20050831104126',1121,36);
INSERT INTO `page` VALUES (1122,8,'Unprotectedarticle','sysop',0,0,0,0.691230499034,'20050831104126',1122,26);
INSERT INTO `page` VALUES (1123,8,'Unprotectsub','sysop',0,0,0,0.483099997745,'20050831104126',1123,33);
INSERT INTO `page` VALUES (1124,8,'Unprotectthispage','sysop',0,0,0,0.765744265502,'20050831104126',1124,15);
INSERT INTO `page` VALUES (1125,8,'Unusedcategories','sysop',0,0,0,0.701908296727,'20050831104126',1125,17);
INSERT INTO `page` VALUES (1126,8,'Unusedcategoriestext','sysop',0,0,0,0.331470532916,'20050831104126',1126,90);
INSERT INTO `page` VALUES (1127,8,'Unusedimages','sysop',0,0,0,0.984563123441,'20050831104126',1127,16);
INSERT INTO `page` VALUES (1128,8,'Unusedimagestext','sysop',0,0,0,0.129042529689,'20050831104126',1128,87);
INSERT INTO `page` VALUES (1129,8,'Unwatch','sysop',0,0,0,0.294805262574,'20050831104126',1129,21);
INSERT INTO `page` VALUES (1130,8,'Unwatchthispage','sysop',0,0,0,0.983776546774,'20050831104126',1130,21);
INSERT INTO `page` VALUES (1131,8,'Updated','sysop',0,0,0,0.568097547249,'20050831104126',1131,11);
INSERT INTO `page` VALUES (1132,8,'Upload','sysop',0,0,0,0.782938651005,'20050831104126',1132,9);
INSERT INTO `page` VALUES (1133,8,'Upload_directory_read_only','sysop',0,0,0,0.759064143597,'20050831104126',1133,59);
INSERT INTO `page` VALUES (1134,8,'Uploadbtn','sysop',0,0,0,0.916925774345,'20050831104126',1134,17);
INSERT INTO `page` VALUES (1135,8,'Uploadcorrupt','sysop',0,0,0,0.751660269929,'20050831104126',1135,120);
INSERT INTO `page` VALUES (1136,8,'Uploaddisabled','sysop',0,0,0,0.282414395122,'20050831104126',1136,46);
INSERT INTO `page` VALUES (1137,8,'Uploadedfiles','sysop',0,0,0,0.615049651159,'20050831104126',1137,20);
INSERT INTO `page` VALUES (1138,8,'Uploadedimage','sysop',0,0,0,0.344638949062,'20050831104126',1138,20);
INSERT INTO `page` VALUES (1139,8,'Uploaderror','sysop',0,0,0,0.193466137622,'20050831104126',1139,21);
INSERT INTO `page` VALUES (1140,8,'Uploadlink','sysop',0,0,0,0.657922981826,'20050831104126',1140,16);
INSERT INTO `page` VALUES (1141,8,'Uploadlog','sysop',0,0,0,0.945163331852,'20050831104126',1141,13);
INSERT INTO `page` VALUES (1142,8,'Uploadlogpage','sysop',0,0,0,0.082694774468,'20050831104126',1142,13);
INSERT INTO `page` VALUES (1143,8,'Uploadlogpagetext','sysop',0,0,0,0.17105135467,'20050831104126',1143,86);
INSERT INTO `page` VALUES (1144,8,'Uploadnewversion','sysop',0,0,0,0.951482774003,'20050831104126',1144,38);
INSERT INTO `page` VALUES (1145,8,'Uploadnologin','sysop',0,0,0,0.456251273505,'20050831104126',1145,16);
INSERT INTO `page` VALUES (1146,8,'Uploadnologintext','sysop',0,0,0,0.568874477465,'20050831104126',1146,84);
INSERT INTO `page` VALUES (1147,8,'Uploadscripted','sysop',0,0,0,0.887780345047,'20050831104126',1147,107);
INSERT INTO `page` VALUES (1148,8,'Uploadtext','sysop',0,0,0,0.533379570584,'20050831104126',1148,951);
INSERT INTO `page` VALUES (1149,8,'Uploadvirus','sysop',0,0,0,0.004085752158,'20050831104126',1149,45);
INSERT INTO `page` VALUES (1150,8,'Uploadwarning','sysop',0,0,0,0.116884860301,'20050831104126',1150,7);
INSERT INTO `page` VALUES (1151,8,'Usenewcategorypage','sysop',0,0,0,0.292410329931,'20050831104126',1151,70);
INSERT INTO `page` VALUES (1152,8,'User_rights_set','sysop',0,0,0,0.051705837921,'20050831104126',1152,44);
INSERT INTO `page` VALUES (1153,8,'Usercssjsyoucanpreview','sysop',0,0,0,0.030860045264,'20050831104126',1153,105);
INSERT INTO `page` VALUES (1154,8,'Usercsspreview','sysop',0,0,0,0.425429801426,'20050831104126',1154,274);
INSERT INTO `page` VALUES (1155,8,'Userexists','sysop',0,0,0,0.289981172811,'20050831104126',1155,72);
INSERT INTO `page` VALUES (1156,8,'Userjspreview','sysop',0,0,0,0.869966474225,'20050831104126',1156,281);
INSERT INTO `page` VALUES (1157,8,'Userlogin','sysop',0,0,0,0.677537231511,'20050831104126',1157,8);
INSERT INTO `page` VALUES (1158,8,'Userlogout','sysop',0,0,0,0.575363726123,'20050831104126',1158,8);
INSERT INTO `page` VALUES (1159,8,'Usermailererror','sysop',0,0,0,0.466410406684,'20050831104126',1159,41);
INSERT INTO `page` VALUES (1160,8,'Userpage','sysop',0,0,0,0.186240024639,'20050831104126',1160,13);
INSERT INTO `page` VALUES (1161,8,'Userrights','sysop',0,0,0,0.546948487915,'20050831104126',1161,22);
INSERT INTO `page` VALUES (1162,8,'Userrights-editusergroup','sysop',0,0,0,0.088577823844,'20050831104126',1162,16);
INSERT INTO `page` VALUES (1163,8,'Userrights-groupsavailable','sysop',0,0,0,0.44508826131,'20050831104126',1163,17);
INSERT INTO `page` VALUES (1164,8,'Userrights-groupshelp','sysop',0,0,0,0.137446734429,'20050831104126',1164,150);
INSERT INTO `page` VALUES (1165,8,'Userrights-groupsmember','sysop',0,0,0,0.067774415152,'20050831104126',1165,10);
INSERT INTO `page` VALUES (1166,8,'Userrights-logcomment','sysop',0,0,0,0.160323244813,'20050831104126',1166,38);
INSERT INTO `page` VALUES (1167,8,'Userrights-lookup-user','sysop',0,0,0,0.353551475809,'20050831104126',1167,18);
INSERT INTO `page` VALUES (1168,8,'Userrights-user-editname','sysop',0,0,0,0.397401042272,'20050831104126',1168,17);
INSERT INTO `page` VALUES (1169,8,'Userrightspheading','sysop',0,0,0,0.481563862122,'20050831104126',1169,16);
INSERT INTO `page` VALUES (1170,8,'Userstats','sysop',0,0,0,0.787674232658,'20050831104126',1170,17);
INSERT INTO `page` VALUES (1171,8,'Userstatstext','sysop',0,0,0,0.80895824541,'20050831104126',1171,95);
INSERT INTO `page` VALUES (1172,8,'Val_add','sysop',0,0,0,0.999383534187,'20050831104126',1172,3);
INSERT INTO `page` VALUES (1173,8,'Val_article_lists','sysop',0,0,0,0.847175513359,'20050831104126',1173,26);
INSERT INTO `page` VALUES (1174,8,'Val_clear_old','sysop',0,0,0,0.289795893441,'20050831104126',1174,30);
INSERT INTO `page` VALUES (1175,8,'Val_del','sysop',0,0,0,0.900678062417,'20050831104126',1175,6);
INSERT INTO `page` VALUES (1176,8,'Val_details_th','sysop',0,0,0,0.656702092991,'20050831104126',1176,34);
INSERT INTO `page` VALUES (1177,8,'Val_details_th_user','sysop',0,0,0,0.28797651473,'20050831104126',1177,7);
INSERT INTO `page` VALUES (1178,8,'Val_form_note','sysop',0,0,0,0.487564943603,'20050831104126',1178,500);
INSERT INTO `page` VALUES (1179,8,'Val_iamsure','sysop',0,0,0,0.53013684004,'20050831104126',1179,37);
INSERT INTO `page` VALUES (1180,8,'Val_list_header','sysop',0,0,0,0.924160876918,'20050831104126',1180,53);
INSERT INTO `page` VALUES (1181,8,'Val_merge_old','sysop',0,0,0,0.566968876965,'20050831104126',1181,54);
INSERT INTO `page` VALUES (1182,8,'Val_my_stats_title','sysop',0,0,0,0.273332665653,'20050831104126',1182,22);
INSERT INTO `page` VALUES (1183,8,'Val_no','sysop',0,0,0,0.853164179471,'20050831104126',1183,2);
INSERT INTO `page` VALUES (1184,8,'Val_no_anon_validation','sysop',0,0,0,0.192811916854,'20050831104126',1184,48);
INSERT INTO `page` VALUES (1185,8,'Val_noop','sysop',0,0,0,0.625532677122,'20050831104126',1185,10);
INSERT INTO `page` VALUES (1186,8,'Val_of','sysop',0,0,0,0.932910117958,'20050831104126',1186,8);
INSERT INTO `page` VALUES (1187,8,'Val_page_validation_statistics','sysop',0,0,0,0.633568694139,'20050831104126',1187,33);
INSERT INTO `page` VALUES (1188,8,'Val_percent','sysop',0,0,0,0.317365527333,'20050831104126',1188,50);
INSERT INTO `page` VALUES (1189,8,'Val_percent_single','sysop',0,0,0,0.026082581692,'20050831104126',1189,50);
INSERT INTO `page` VALUES (1190,8,'Val_rev_for','sysop',0,0,0,0.352200256067,'20050831104126',1190,16);
INSERT INTO `page` VALUES (1191,8,'Val_rev_stats','sysop',0,0,0,0.481572056437,'20050831104126',1191,60);
INSERT INTO `page` VALUES (1192,8,'Val_revision','sysop',0,0,0,0.068744544243,'20050831104126',1192,8);
INSERT INTO `page` VALUES (1193,8,'Val_revision_changes_ok','sysop',0,0,0,0.205223789286,'20050831104126',1193,30);
INSERT INTO `page` VALUES (1194,8,'Val_revision_number','sysop',0,0,0,0.806486543523,'20050831104126',1194,12);
INSERT INTO `page` VALUES (1195,8,'Val_revision_of','sysop',0,0,0,0.081015546469,'20050831104126',1195,14);
INSERT INTO `page` VALUES (1196,8,'Val_revision_stats_link','sysop',0,0,0,0.558172601796,'20050831104126',1196,7);
INSERT INTO `page` VALUES (1197,8,'Val_show_my_ratings','sysop',0,0,0,0.979427908505,'20050831104126',1197,19);
INSERT INTO `page` VALUES (1198,8,'Val_stat_link_text','sysop',0,0,0,0.885344468833,'20050831104126',1198,38);
INSERT INTO `page` VALUES (1199,8,'Val_tab','sysop',0,0,0,0.918421770483,'20050831104126',1199,8);
INSERT INTO `page` VALUES (1200,8,'Val_table_header','sysop',0,0,0,0.952126151705,'20050831104126',1200,69);
INSERT INTO `page` VALUES (1201,8,'Val_this_is_current_version','sysop',0,0,0,0.051835803005,'20050831104126',1201,26);
INSERT INTO `page` VALUES (1202,8,'Val_time','sysop',0,0,0,0.810223308644,'20050831104126',1202,4);
INSERT INTO `page` VALUES (1203,8,'Val_topic_desc_page','sysop',0,0,0,0.142586405727,'20050831104126',1203,25);
INSERT INTO `page` VALUES (1204,8,'Val_total','sysop',0,0,0,0.834078020319,'20050831104126',1204,5);
INSERT INTO `page` VALUES (1205,8,'Val_user_stats_title','sysop',0,0,0,0.849073914487,'20050831104126',1205,30);
INSERT INTO `page` VALUES (1206,8,'Val_user_validations','sysop',0,0,0,0.744509603648,'20050831104126',1206,33);
INSERT INTO `page` VALUES (1207,8,'Val_validate_article_namespace_only','sysop',0,0,0,0.285874850702,'20050831104126',1207,81);
INSERT INTO `page` VALUES (1208,8,'Val_validate_version','sysop',0,0,0,0.753172471979,'20050831104126',1208,21);
INSERT INTO `page` VALUES (1209,8,'Val_validated','sysop',0,0,0,0.781291398074,'20050831104126',1209,16);
INSERT INTO `page` VALUES (1210,8,'Val_validation_of','sysop',0,0,0,0.211718825678,'20050831104126',1210,18);
INSERT INTO `page` VALUES (1211,8,'Val_version','sysop',0,0,0,0.461415662505,'20050831104126',1211,7);
INSERT INTO `page` VALUES (1212,8,'Val_version_of','sysop',0,0,0,0.903872400161,'20050831104126',1212,13);
INSERT INTO `page` VALUES (1213,8,'Val_view_version','sysop',0,0,0,0.149485425745,'20050831104126',1213,18);
INSERT INTO `page` VALUES (1214,8,'Val_votepage_intro','sysop',0,0,0,0.29879248649,'20050831104126',1214,88);
INSERT INTO `page` VALUES (1215,8,'Val_warning','sysop',0,0,0,0.795333332717,'20050831104126',1215,93);
INSERT INTO `page` VALUES (1216,8,'Val_yes','sysop',0,0,0,0.413783445523,'20050831104126',1216,3);
INSERT INTO `page` VALUES (1217,8,'Validate','sysop',0,0,0,0.046196353726,'20050831104126',1217,13);
INSERT INTO `page` VALUES (1218,8,'Variantname-zh','sysop',0,0,0,0.133670382969,'20050831104126',1218,2);
INSERT INTO `page` VALUES (1219,8,'Variantname-zh-cn','sysop',0,0,0,0.87714751571,'20050831104126',1219,2);
INSERT INTO `page` VALUES (1220,8,'Variantname-zh-hk','sysop',0,0,0,0.58996315025,'20050831104126',1220,2);
INSERT INTO `page` VALUES (1221,8,'Variantname-zh-sg','sysop',0,0,0,0.744595393295,'20050831104126',1221,2);
INSERT INTO `page` VALUES (1222,8,'Variantname-zh-tw','sysop',0,0,0,0.574064998285,'20050831104126',1222,2);
INSERT INTO `page` VALUES (1223,8,'Version','sysop',0,0,0,0.660475361023,'20050831104126',1223,7);
INSERT INTO `page` VALUES (1224,8,'Versionrequired','sysop',0,0,0,0.036184660717,'20050831104126',1224,32);
INSERT INTO `page` VALUES (1225,8,'Versionrequiredtext','sysop',0,0,0,0.39158641954,'20050831104126',1225,77);
INSERT INTO `page` VALUES (1226,8,'Viewcount','sysop',0,0,0,0.432854692195,'20050831104126',1226,42);
INSERT INTO `page` VALUES (1227,8,'Viewprevnext','sysop',0,0,0,0.243859402784,'20050831104126',1227,21);
INSERT INTO `page` VALUES (1228,8,'Views','sysop',0,0,0,0.261035986358,'20050831104126',1228,5);
INSERT INTO `page` VALUES (1229,8,'Viewsource','sysop',0,0,0,0.601484359671,'20050831104126',1229,20);
INSERT INTO `page` VALUES (1230,8,'Viewtalkpage','sysop',0,0,0,0.111732935645,'20050831104126',1230,10);
INSERT INTO `page` VALUES (1231,8,'Wantedpages','sysop',0,0,0,0.882109957692,'20050831104126',1231,18);
INSERT INTO `page` VALUES (1232,8,'Watch','sysop',0,0,0,0.118144807779,'20050831104126',1232,10);
INSERT INTO `page` VALUES (1233,8,'Watchdetails','sysop',0,0,0,0.399607623249,'20050831104126',1233,177);
INSERT INTO `page` VALUES (1234,8,'Watcheditlist','sysop',0,0,0,0.328613920095,'20050831104126',1234,144);
INSERT INTO `page` VALUES (1235,8,'Watchlist','sysop',0,0,0,0.302575138209,'20050831104126',1235,17);
INSERT INTO `page` VALUES (1236,8,'Watchlistall1','sysop',0,0,0,0.668212872079,'20050831104126',1236,3);
INSERT INTO `page` VALUES (1237,8,'Watchlistall2','sysop',0,0,0,0.06797888128,'20050831104127',1237,3);
INSERT INTO `page` VALUES (1238,8,'Watchlistcontains','sysop',0,0,0,0.127406319018,'20050831104127',1238,42);
INSERT INTO `page` VALUES (1239,8,'Watchlistsub','sysop',0,0,0,0.644659965696,'20050831104127',1239,20);
INSERT INTO `page` VALUES (1240,8,'Watchmethod-list','sysop',0,0,0,0.597101323026,'20050831104127',1240,61);
INSERT INTO `page` VALUES (1241,8,'Watchmethod-recent','sysop',0,0,0,0.628642128619,'20050831104127',1241,65);
INSERT INTO `page` VALUES (1242,8,'Watchnochange','sysop',0,0,0,0.662505460593,'20050831104127',1242,85);
INSERT INTO `page` VALUES (1243,8,'Watchnologin','sysop',0,0,0,0.758693119266,'20050831104127',1243,25);
INSERT INTO `page` VALUES (1244,8,'Watchnologintext','sysop',0,0,0,0.853982293429,'20050831104127',1244,91);
INSERT INTO `page` VALUES (1245,8,'Watchthis','sysop',0,0,0,0.997991779345,'20050831104127',1245,25);
INSERT INTO `page` VALUES (1246,8,'Watchthispage','sysop',0,0,0,0.685721054931,'20050831104127',1246,16);
INSERT INTO `page` VALUES (1247,8,'Wednesday','sysop',0,0,0,0.767618693832,'20050831104127',1247,8);
INSERT INTO `page` VALUES (1248,8,'Welcomecreation','sysop',0,0,0,0.598041635933,'20050831104127',1248,116);
INSERT INTO `page` VALUES (1249,8,'Whatlinkshere','sysop',0,0,0,0.197400284471,'20050831104127',1249,17);
INSERT INTO `page` VALUES (1250,8,'Whitelistacctext','sysop',0,0,0,0.539870286456,'20050831104127',1250,137);
INSERT INTO `page` VALUES (1251,8,'Whitelistacctitle','sysop',0,0,0,0.003952163767,'20050831104127',1251,51);
INSERT INTO `page` VALUES (1252,8,'Whitelistedittext','sysop',0,0,0,0.462169344852,'20050831104127',1252,86);
INSERT INTO `page` VALUES (1253,8,'Whitelistedittitle','sysop',0,0,0,0.039597462922,'20050831104127',1253,53);
INSERT INTO `page` VALUES (1254,8,'Whitelistreadtext','sysop',0,0,0,0.030292873733,'20050831104127',1254,81);
INSERT INTO `page` VALUES (1255,8,'Whitelistreadtitle','sysop',0,0,0,0.662756939839,'20050831104127',1255,48);
INSERT INTO `page` VALUES (1256,8,'Wikipediapage','sysop',0,0,0,0.05956673042,'20050831104127',1256,9);
INSERT INTO `page` VALUES (1257,8,'Wikititlesuffix','sysop',0,0,0,0.950063224948,'20050831104127',1257,12);
INSERT INTO `page` VALUES (1258,8,'Wlheader-enotif','sysop',0,0,0,0.966900833042,'20050831104127',1258,51);
INSERT INTO `page` VALUES (1259,8,'Wlheader-showupdated','sysop',0,0,0,0.313006126999,'20050831104127',1259,76);
INSERT INTO `page` VALUES (1260,8,'Wlhide','sysop',0,0,0,0.047893497631,'20050831104127',1260,9);
INSERT INTO `page` VALUES (1261,8,'Wlhideshowown','sysop',0,0,0,0.084332849886,'20050831104127',1261,31);
INSERT INTO `page` VALUES (1262,8,'Wlnote','sysop',0,0,0,0.947639353552,'20050831104127',1262,67);
INSERT INTO `page` VALUES (1263,8,'Wlsaved','sysop',0,0,0,0.193137658442,'20050831104127',1263,59);
INSERT INTO `page` VALUES (1264,8,'Wlshow','sysop',0,0,0,0.782635365387,'20050831104127',1264,5);
INSERT INTO `page` VALUES (1265,8,'Wlshowlast','sysop',0,0,0,0.307793212736,'20050831104127',1265,38);
INSERT INTO `page` VALUES (1266,8,'Wrong_wfQuery_params','sysop',0,0,0,0.859079070703,'20050831104127',1266,67);
INSERT INTO `page` VALUES (1267,8,'Wrongpassword','sysop',0,0,0,0.142369249187,'20050831104127',1267,68);
INSERT INTO `page` VALUES (1268,8,'Yourdiff','sysop',0,0,0,0.294318695501,'20050831104127',1268,12);
INSERT INTO `page` VALUES (1269,8,'Yourdomainname','sysop',0,0,0,0.872115322668,'20050831104127',1269,11);
INSERT INTO `page` VALUES (1270,8,'Youremail','sysop',0,0,0,0.224797208642,'20050831104127',1270,21);
INSERT INTO `page` VALUES (1271,8,'Yourlanguage','sysop',0,0,0,0.373800120896,'20050831104127',1271,8);
INSERT INTO `page` VALUES (1272,8,'Yourname','sysop',0,0,0,0.520804051068,'20050831104127',1272,16);
INSERT INTO `page` VALUES (1273,8,'Yournick','sysop',0,0,0,0.242874199106,'20050831104127',1273,38);
INSERT INTO `page` VALUES (1274,8,'Yourpassword','sysop',0,0,0,0.736890925505,'20050831104127',1274,12);
INSERT INTO `page` VALUES (1275,8,'Yourpasswordagain','sysop',0,0,0,0.5401119942,'20050831104127',1275,20);
INSERT INTO `page` VALUES (1276,8,'Yourrealname','sysop',0,0,0,0.659637550502,'20050831104127',1276,38);
INSERT INTO `page` VALUES (1277,8,'Yourtext','sysop',0,0,0,0.987869268305,'20050831104127',1277,8);
INSERT INTO `page` VALUES (1278,8,'Yourvariant','sysop',0,0,0,0.710874452927,'20050831104127',1278,7);

--
-- Table structure for table `pagelinks`
--

CREATE TABLE `pagelinks` (
  `pl_from` int(8) unsigned NOT NULL default '0',
  `pl_namespace` int(11) NOT NULL default '0',
  `pl_title` varchar(255) binary NOT NULL default '',
  UNIQUE KEY `pl_from` (`pl_from`,`pl_namespace`,`pl_title`),
  KEY `pl_namespace` (`pl_namespace`,`pl_title`)
) TYPE=MyISAM;

--
-- Dumping data for table `pagelinks`
--


--
-- Table structure for table `querycache`
--

CREATE TABLE `querycache` (
  `qc_type` char(32) NOT NULL default '',
  `qc_value` int(5) unsigned NOT NULL default '0',
  `qc_namespace` int(11) NOT NULL default '0',
  `qc_title` char(255) binary NOT NULL default '',
  KEY `qc_type` (`qc_type`,`qc_value`)
) TYPE=MyISAM;

--
-- Dumping data for table `querycache`
--


--
-- Table structure for table `recentchanges`
--

CREATE TABLE `recentchanges` (
  `rc_id` int(8) NOT NULL auto_increment,
  `rc_timestamp` varchar(14) binary NOT NULL default '',
  `rc_cur_time` varchar(14) binary NOT NULL default '',
  `rc_user` int(10) unsigned NOT NULL default '0',
  `rc_user_text` varchar(255) binary NOT NULL default '',
  `rc_namespace` int(11) NOT NULL default '0',
  `rc_title` varchar(255) binary NOT NULL default '',
  `rc_comment` varchar(255) binary NOT NULL default '',
  `rc_minor` tinyint(3) unsigned NOT NULL default '0',
  `rc_bot` tinyint(3) unsigned NOT NULL default '0',
  `rc_new` tinyint(3) unsigned NOT NULL default '0',
  `rc_cur_id` int(10) unsigned NOT NULL default '0',
  `rc_this_oldid` int(10) unsigned NOT NULL default '0',
  `rc_last_oldid` int(10) unsigned NOT NULL default '0',
  `rc_type` tinyint(3) unsigned NOT NULL default '0',
  `rc_moved_to_ns` tinyint(3) unsigned NOT NULL default '0',
  `rc_moved_to_title` varchar(255) binary NOT NULL default '',
  `rc_patrolled` tinyint(3) unsigned NOT NULL default '0',
  `rc_ip` varchar(15) NOT NULL default '',
  PRIMARY KEY  (`rc_id`),
  KEY `rc_timestamp` (`rc_timestamp`),
  KEY `rc_namespace_title` (`rc_namespace`,`rc_title`),
  KEY `rc_cur_id` (`rc_cur_id`),
  KEY `new_name_timestamp` (`rc_new`,`rc_namespace`,`rc_timestamp`),
  KEY `rc_ip` (`rc_ip`)
) TYPE=MyISAM;

--
-- Dumping data for table `recentchanges`
--

INSERT INTO `recentchanges` VALUES (1,'20050831105200','20050831105200',0,'83.236.12.68',0,'Hauptseite','',0,0,0,1,1279,1,0,0,'',0,'');

--
-- Table structure for table `revision`
--

CREATE TABLE `revision` (
  `rev_id` int(8) unsigned NOT NULL auto_increment,
  `rev_page` int(8) unsigned NOT NULL default '0',
  `rev_text_id` int(8) unsigned NOT NULL default '0',
  `rev_comment` tinyblob NOT NULL,
  `rev_user` int(5) unsigned NOT NULL default '0',
  `rev_user_text` varchar(255) binary NOT NULL default '',
  `rev_timestamp` varchar(14) binary NOT NULL default '',
  `rev_minor_edit` tinyint(1) unsigned NOT NULL default '0',
  `rev_deleted` tinyint(1) unsigned NOT NULL default '0',
  PRIMARY KEY  (`rev_page`,`rev_id`),
  UNIQUE KEY `rev_id` (`rev_id`),
  KEY `rev_timestamp` (`rev_timestamp`),
  KEY `page_timestamp` (`rev_page`,`rev_timestamp`),
  KEY `user_timestamp` (`rev_user`,`rev_timestamp`),
  KEY `usertext_timestamp` (`rev_user_text`,`rev_timestamp`)
) TYPE=MyISAM;

--
-- Dumping data for table `revision`
--

INSERT INTO `revision` VALUES (1,1,1,'',0,'MediaWiki default','20050831104119',0,0);
INSERT INTO `revision` VALUES (2,2,2,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (3,3,3,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (4,4,4,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (5,5,5,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (6,6,6,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (7,7,7,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (8,8,8,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (9,9,9,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (10,10,10,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (11,11,11,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (12,12,12,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (13,13,13,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (14,14,14,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (15,15,15,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (16,16,16,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (17,17,17,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (18,18,18,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (19,19,19,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (20,20,20,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (21,21,21,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (22,22,22,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (23,23,23,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (24,24,24,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (25,25,25,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (26,26,26,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (27,27,27,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (28,28,28,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (29,29,29,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (30,30,30,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (31,31,31,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (32,32,32,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (33,33,33,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (34,34,34,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (35,35,35,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (36,36,36,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (37,37,37,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (38,38,38,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (39,39,39,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (40,40,40,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (41,41,41,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (42,42,42,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (43,43,43,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (44,44,44,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (45,45,45,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (46,46,46,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (47,47,47,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (48,48,48,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (49,49,49,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (50,50,50,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (51,51,51,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (52,52,52,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (53,53,53,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (54,54,54,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (55,55,55,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (56,56,56,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (57,57,57,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (58,58,58,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (59,59,59,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (60,60,60,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (61,61,61,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (62,62,62,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (63,63,63,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (64,64,64,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (65,65,65,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (66,66,66,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (67,67,67,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (68,68,68,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (69,69,69,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (70,70,70,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (71,71,71,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (72,72,72,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (73,73,73,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (74,74,74,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (75,75,75,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (76,76,76,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (77,77,77,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (78,78,78,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (79,79,79,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (80,80,80,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (81,81,81,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (82,82,82,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (83,83,83,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (84,84,84,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (85,85,85,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (86,86,86,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (87,87,87,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (88,88,88,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (89,89,89,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (90,90,90,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (91,91,91,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (92,92,92,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (93,93,93,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (94,94,94,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (95,95,95,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (96,96,96,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (97,97,97,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (98,98,98,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (99,99,99,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (100,100,100,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (101,101,101,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (102,102,102,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (103,103,103,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (104,104,104,'',0,'MediaWiki default','20050831104120',0,0);
INSERT INTO `revision` VALUES (105,105,105,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (106,106,106,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (107,107,107,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (108,108,108,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (109,109,109,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (110,110,110,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (111,111,111,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (112,112,112,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (113,113,113,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (114,114,114,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (115,115,115,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (116,116,116,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (117,117,117,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (118,118,118,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (119,119,119,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (120,120,120,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (121,121,121,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (122,122,122,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (123,123,123,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (124,124,124,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (125,125,125,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (126,126,126,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (127,127,127,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (128,128,128,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (129,129,129,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (130,130,130,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (131,131,131,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (132,132,132,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (133,133,133,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (134,134,134,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (135,135,135,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (136,136,136,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (137,137,137,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (138,138,138,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (139,139,139,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (140,140,140,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (141,141,141,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (142,142,142,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (143,143,143,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (144,144,144,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (145,145,145,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (146,146,146,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (147,147,147,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (148,148,148,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (149,149,149,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (150,150,150,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (151,151,151,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (152,152,152,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (153,153,153,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (154,154,154,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (155,155,155,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (156,156,156,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (157,157,157,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (158,158,158,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (159,159,159,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (160,160,160,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (161,161,161,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (162,162,162,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (163,163,163,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (164,164,164,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (165,165,165,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (166,166,166,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (167,167,167,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (168,168,168,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (169,169,169,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (170,170,170,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (171,171,171,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (172,172,172,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (173,173,173,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (174,174,174,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (175,175,175,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (176,176,176,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (177,177,177,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (178,178,178,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (179,179,179,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (180,180,180,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (181,181,181,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (182,182,182,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (183,183,183,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (184,184,184,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (185,185,185,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (186,186,186,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (187,187,187,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (188,188,188,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (189,189,189,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (190,190,190,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (191,191,191,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (192,192,192,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (193,193,193,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (194,194,194,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (195,195,195,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (196,196,196,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (197,197,197,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (198,198,198,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (199,199,199,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (200,200,200,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (201,201,201,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (202,202,202,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (203,203,203,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (204,204,204,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (205,205,205,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (206,206,206,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (207,207,207,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (208,208,208,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (209,209,209,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (210,210,210,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (211,211,211,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (212,212,212,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (213,213,213,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (214,214,214,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (215,215,215,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (216,216,216,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (217,217,217,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (218,218,218,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (219,219,219,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (220,220,220,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (221,221,221,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (222,222,222,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (223,223,223,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (224,224,224,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (225,225,225,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (226,226,226,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (227,227,227,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (228,228,228,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (229,229,229,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (230,230,230,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (231,231,231,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (232,232,232,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (233,233,233,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (234,234,234,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (235,235,235,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (236,236,236,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (237,237,237,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (238,238,238,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (239,239,239,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (240,240,240,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (241,241,241,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (242,242,242,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (243,243,243,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (244,244,244,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (245,245,245,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (246,246,246,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (247,247,247,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (248,248,248,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (249,249,249,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (250,250,250,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (251,251,251,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (252,252,252,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (253,253,253,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (254,254,254,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (255,255,255,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (256,256,256,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (257,257,257,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (258,258,258,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (259,259,259,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (260,260,260,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (261,261,261,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (262,262,262,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (263,263,263,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (264,264,264,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (265,265,265,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (266,266,266,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (267,267,267,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (268,268,268,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (269,269,269,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (270,270,270,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (271,271,271,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (272,272,272,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (273,273,273,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (274,274,274,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (275,275,275,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (276,276,276,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (277,277,277,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (278,278,278,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (279,279,279,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (280,280,280,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (281,281,281,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (282,282,282,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (283,283,283,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (284,284,284,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (285,285,285,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (286,286,286,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (287,287,287,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (288,288,288,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (289,289,289,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (290,290,290,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (291,291,291,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (292,292,292,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (293,293,293,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (294,294,294,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (295,295,295,'',0,'MediaWiki default','20050831104121',0,0);
INSERT INTO `revision` VALUES (296,296,296,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (297,297,297,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (298,298,298,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (299,299,299,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (300,300,300,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (301,301,301,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (302,302,302,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (303,303,303,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (304,304,304,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (305,305,305,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (306,306,306,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (307,307,307,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (308,308,308,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (309,309,309,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (310,310,310,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (311,311,311,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (312,312,312,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (313,313,313,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (314,314,314,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (315,315,315,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (316,316,316,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (317,317,317,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (318,318,318,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (319,319,319,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (320,320,320,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (321,321,321,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (322,322,322,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (323,323,323,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (324,324,324,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (325,325,325,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (326,326,326,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (327,327,327,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (328,328,328,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (329,329,329,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (330,330,330,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (331,331,331,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (332,332,332,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (333,333,333,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (334,334,334,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (335,335,335,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (336,336,336,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (337,337,337,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (338,338,338,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (339,339,339,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (340,340,340,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (341,341,341,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (342,342,342,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (343,343,343,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (344,344,344,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (345,345,345,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (346,346,346,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (347,347,347,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (348,348,348,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (349,349,349,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (350,350,350,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (351,351,351,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (352,352,352,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (353,353,353,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (354,354,354,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (355,355,355,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (356,356,356,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (357,357,357,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (358,358,358,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (359,359,359,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (360,360,360,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (361,361,361,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (362,362,362,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (363,363,363,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (364,364,364,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (365,365,365,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (366,366,366,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (367,367,367,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (368,368,368,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (369,369,369,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (370,370,370,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (371,371,371,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (372,372,372,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (373,373,373,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (374,374,374,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (375,375,375,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (376,376,376,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (377,377,377,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (378,378,378,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (379,379,379,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (380,380,380,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (381,381,381,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (382,382,382,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (383,383,383,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (384,384,384,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (385,385,385,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (386,386,386,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (387,387,387,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (388,388,388,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (389,389,389,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (390,390,390,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (391,391,391,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (392,392,392,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (393,393,393,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (394,394,394,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (395,395,395,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (396,396,396,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (397,397,397,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (398,398,398,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (399,399,399,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (400,400,400,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (401,401,401,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (402,402,402,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (403,403,403,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (404,404,404,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (405,405,405,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (406,406,406,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (407,407,407,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (408,408,408,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (409,409,409,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (410,410,410,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (411,411,411,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (412,412,412,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (413,413,413,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (414,414,414,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (415,415,415,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (416,416,416,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (417,417,417,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (418,418,418,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (419,419,419,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (420,420,420,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (421,421,421,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (422,422,422,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (423,423,423,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (424,424,424,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (425,425,425,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (426,426,426,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (427,427,427,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (428,428,428,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (429,429,429,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (430,430,430,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (431,431,431,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (432,432,432,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (433,433,433,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (434,434,434,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (435,435,435,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (436,436,436,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (437,437,437,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (438,438,438,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (439,439,439,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (440,440,440,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (441,441,441,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (442,442,442,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (443,443,443,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (444,444,444,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (445,445,445,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (446,446,446,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (447,447,447,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (448,448,448,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (449,449,449,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (450,450,450,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (451,451,451,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (452,452,452,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (453,453,453,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (454,454,454,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (455,455,455,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (456,456,456,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (457,457,457,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (458,458,458,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (459,459,459,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (460,460,460,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (461,461,461,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (462,462,462,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (463,463,463,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (464,464,464,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (465,465,465,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (466,466,466,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (467,467,467,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (468,468,468,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (469,469,469,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (470,470,470,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (471,471,471,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (472,472,472,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (473,473,473,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (474,474,474,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (475,475,475,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (476,476,476,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (477,477,477,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (478,478,478,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (479,479,479,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (480,480,480,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (481,481,481,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (482,482,482,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (483,483,483,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (484,484,484,'',0,'MediaWiki default','20050831104122',0,0);
INSERT INTO `revision` VALUES (485,485,485,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (486,486,486,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (487,487,487,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (488,488,488,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (489,489,489,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (490,490,490,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (491,491,491,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (492,492,492,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (493,493,493,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (494,494,494,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (495,495,495,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (496,496,496,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (497,497,497,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (498,498,498,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (499,499,499,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (500,500,500,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (501,501,501,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (502,502,502,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (503,503,503,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (504,504,504,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (505,505,505,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (506,506,506,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (507,507,507,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (508,508,508,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (509,509,509,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (510,510,510,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (511,511,511,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (512,512,512,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (513,513,513,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (514,514,514,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (515,515,515,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (516,516,516,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (517,517,517,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (518,518,518,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (519,519,519,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (520,520,520,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (521,521,521,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (522,522,522,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (523,523,523,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (524,524,524,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (525,525,525,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (526,526,526,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (527,527,527,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (528,528,528,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (529,529,529,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (530,530,530,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (531,531,531,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (532,532,532,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (533,533,533,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (534,534,534,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (535,535,535,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (536,536,536,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (537,537,537,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (538,538,538,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (539,539,539,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (540,540,540,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (541,541,541,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (542,542,542,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (543,543,543,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (544,544,544,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (545,545,545,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (546,546,546,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (547,547,547,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (548,548,548,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (549,549,549,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (550,550,550,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (551,551,551,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (552,552,552,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (553,553,553,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (554,554,554,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (555,555,555,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (556,556,556,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (557,557,557,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (558,558,558,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (559,559,559,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (560,560,560,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (561,561,561,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (562,562,562,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (563,563,563,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (564,564,564,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (565,565,565,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (566,566,566,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (567,567,567,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (568,568,568,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (569,569,569,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (570,570,570,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (571,571,571,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (572,572,572,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (573,573,573,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (574,574,574,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (575,575,575,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (576,576,576,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (577,577,577,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (578,578,578,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (579,579,579,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (580,580,580,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (581,581,581,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (582,582,582,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (583,583,583,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (584,584,584,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (585,585,585,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (586,586,586,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (587,587,587,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (588,588,588,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (589,589,589,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (590,590,590,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (591,591,591,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (592,592,592,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (593,593,593,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (594,594,594,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (595,595,595,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (596,596,596,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (597,597,597,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (598,598,598,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (599,599,599,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (600,600,600,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (601,601,601,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (602,602,602,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (603,603,603,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (604,604,604,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (605,605,605,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (606,606,606,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (607,607,607,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (608,608,608,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (609,609,609,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (610,610,610,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (611,611,611,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (612,612,612,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (613,613,613,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (614,614,614,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (615,615,615,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (616,616,616,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (617,617,617,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (618,618,618,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (619,619,619,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (620,620,620,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (621,621,621,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (622,622,622,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (623,623,623,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (624,624,624,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (625,625,625,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (626,626,626,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (627,627,627,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (628,628,628,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (629,629,629,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (630,630,630,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (631,631,631,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (632,632,632,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (633,633,633,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (634,634,634,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (635,635,635,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (636,636,636,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (637,637,637,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (638,638,638,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (639,639,639,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (640,640,640,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (641,641,641,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (642,642,642,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (643,643,643,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (644,644,644,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (645,645,645,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (646,646,646,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (647,647,647,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (648,648,648,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (649,649,649,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (650,650,650,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (651,651,651,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (652,652,652,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (653,653,653,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (654,654,654,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (655,655,655,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (656,656,656,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (657,657,657,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (658,658,658,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (659,659,659,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (660,660,660,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (661,661,661,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (662,662,662,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (663,663,663,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (664,664,664,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (665,665,665,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (666,666,666,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (667,667,667,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (668,668,668,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (669,669,669,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (670,670,670,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (671,671,671,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (672,672,672,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (673,673,673,'',0,'MediaWiki default','20050831104123',0,0);
INSERT INTO `revision` VALUES (674,674,674,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (675,675,675,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (676,676,676,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (677,677,677,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (678,678,678,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (679,679,679,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (680,680,680,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (681,681,681,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (682,682,682,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (683,683,683,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (684,684,684,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (685,685,685,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (686,686,686,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (687,687,687,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (688,688,688,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (689,689,689,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (690,690,690,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (691,691,691,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (692,692,692,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (693,693,693,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (694,694,694,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (695,695,695,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (696,696,696,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (697,697,697,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (698,698,698,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (699,699,699,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (700,700,700,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (701,701,701,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (702,702,702,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (703,703,703,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (704,704,704,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (705,705,705,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (706,706,706,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (707,707,707,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (708,708,708,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (709,709,709,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (710,710,710,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (711,711,711,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (712,712,712,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (713,713,713,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (714,714,714,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (715,715,715,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (716,716,716,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (717,717,717,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (718,718,718,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (719,719,719,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (720,720,720,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (721,721,721,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (722,722,722,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (723,723,723,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (724,724,724,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (725,725,725,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (726,726,726,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (727,727,727,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (728,728,728,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (729,729,729,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (730,730,730,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (731,731,731,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (732,732,732,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (733,733,733,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (734,734,734,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (735,735,735,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (736,736,736,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (737,737,737,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (738,738,738,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (739,739,739,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (740,740,740,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (741,741,741,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (742,742,742,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (743,743,743,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (744,744,744,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (745,745,745,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (746,746,746,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (747,747,747,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (748,748,748,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (749,749,749,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (750,750,750,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (751,751,751,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (752,752,752,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (753,753,753,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (754,754,754,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (755,755,755,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (756,756,756,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (757,757,757,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (758,758,758,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (759,759,759,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (760,760,760,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (761,761,761,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (762,762,762,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (763,763,763,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (764,764,764,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (765,765,765,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (766,766,766,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (767,767,767,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (768,768,768,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (769,769,769,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (770,770,770,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (771,771,771,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (772,772,772,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (773,773,773,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (774,774,774,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (775,775,775,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (776,776,776,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (777,777,777,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (778,778,778,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (779,779,779,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (780,780,780,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (781,781,781,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (782,782,782,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (783,783,783,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (784,784,784,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (785,785,785,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (786,786,786,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (787,787,787,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (788,788,788,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (789,789,789,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (790,790,790,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (791,791,791,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (792,792,792,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (793,793,793,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (794,794,794,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (795,795,795,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (796,796,796,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (797,797,797,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (798,798,798,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (799,799,799,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (800,800,800,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (801,801,801,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (802,802,802,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (803,803,803,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (804,804,804,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (805,805,805,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (806,806,806,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (807,807,807,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (808,808,808,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (809,809,809,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (810,810,810,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (811,811,811,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (812,812,812,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (813,813,813,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (814,814,814,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (815,815,815,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (816,816,816,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (817,817,817,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (818,818,818,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (819,819,819,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (820,820,820,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (821,821,821,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (822,822,822,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (823,823,823,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (824,824,824,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (825,825,825,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (826,826,826,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (827,827,827,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (828,828,828,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (829,829,829,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (830,830,830,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (831,831,831,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (832,832,832,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (833,833,833,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (834,834,834,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (835,835,835,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (836,836,836,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (837,837,837,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (838,838,838,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (839,839,839,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (840,840,840,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (841,841,841,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (842,842,842,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (843,843,843,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (844,844,844,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (845,845,845,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (846,846,846,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (847,847,847,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (848,848,848,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (849,849,849,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (850,850,850,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (851,851,851,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (852,852,852,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (853,853,853,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (854,854,854,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (855,855,855,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (856,856,856,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (857,857,857,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (858,858,858,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (859,859,859,'',0,'MediaWiki default','20050831104124',0,0);
INSERT INTO `revision` VALUES (860,860,860,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (861,861,861,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (862,862,862,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (863,863,863,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (864,864,864,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (865,865,865,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (866,866,866,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (867,867,867,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (868,868,868,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (869,869,869,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (870,870,870,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (871,871,871,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (872,872,872,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (873,873,873,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (874,874,874,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (875,875,875,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (876,876,876,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (877,877,877,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (878,878,878,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (879,879,879,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (880,880,880,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (881,881,881,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (882,882,882,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (883,883,883,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (884,884,884,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (885,885,885,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (886,886,886,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (887,887,887,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (888,888,888,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (889,889,889,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (890,890,890,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (891,891,891,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (892,892,892,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (893,893,893,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (894,894,894,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (895,895,895,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (896,896,896,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (897,897,897,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (898,898,898,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (899,899,899,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (900,900,900,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (901,901,901,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (902,902,902,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (903,903,903,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (904,904,904,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (905,905,905,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (906,906,906,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (907,907,907,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (908,908,908,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (909,909,909,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (910,910,910,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (911,911,911,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (912,912,912,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (913,913,913,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (914,914,914,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (915,915,915,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (916,916,916,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (917,917,917,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (918,918,918,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (919,919,919,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (920,920,920,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (921,921,921,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (922,922,922,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (923,923,923,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (924,924,924,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (925,925,925,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (926,926,926,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (927,927,927,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (928,928,928,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (929,929,929,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (930,930,930,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (931,931,931,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (932,932,932,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (933,933,933,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (934,934,934,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (935,935,935,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (936,936,936,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (937,937,937,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (938,938,938,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (939,939,939,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (940,940,940,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (941,941,941,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (942,942,942,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (943,943,943,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (944,944,944,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (945,945,945,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (946,946,946,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (947,947,947,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (948,948,948,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (949,949,949,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (950,950,950,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (951,951,951,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (952,952,952,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (953,953,953,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (954,954,954,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (955,955,955,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (956,956,956,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (957,957,957,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (958,958,958,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (959,959,959,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (960,960,960,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (961,961,961,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (962,962,962,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (963,963,963,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (964,964,964,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (965,965,965,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (966,966,966,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (967,967,967,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (968,968,968,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (969,969,969,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (970,970,970,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (971,971,971,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (972,972,972,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (973,973,973,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (974,974,974,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (975,975,975,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (976,976,976,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (977,977,977,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (978,978,978,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (979,979,979,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (980,980,980,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (981,981,981,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (982,982,982,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (983,983,983,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (984,984,984,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (985,985,985,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (986,986,986,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (987,987,987,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (988,988,988,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (989,989,989,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (990,990,990,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (991,991,991,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (992,992,992,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (993,993,993,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (994,994,994,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (995,995,995,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (996,996,996,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (997,997,997,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (998,998,998,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (999,999,999,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1000,1000,1000,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1001,1001,1001,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1002,1002,1002,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1003,1003,1003,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1004,1004,1004,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1005,1005,1005,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1006,1006,1006,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1007,1007,1007,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1008,1008,1008,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1009,1009,1009,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1010,1010,1010,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1011,1011,1011,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1012,1012,1012,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1013,1013,1013,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1014,1014,1014,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1015,1015,1015,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1016,1016,1016,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1017,1017,1017,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1018,1018,1018,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1019,1019,1019,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1020,1020,1020,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1021,1021,1021,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1022,1022,1022,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1023,1023,1023,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1024,1024,1024,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1025,1025,1025,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1026,1026,1026,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1027,1027,1027,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1028,1028,1028,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1029,1029,1029,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1030,1030,1030,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1031,1031,1031,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1032,1032,1032,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1033,1033,1033,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1034,1034,1034,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1035,1035,1035,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1036,1036,1036,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1037,1037,1037,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1038,1038,1038,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1039,1039,1039,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1040,1040,1040,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1041,1041,1041,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1042,1042,1042,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1043,1043,1043,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1044,1044,1044,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1045,1045,1045,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1046,1046,1046,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1047,1047,1047,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1048,1048,1048,'',0,'MediaWiki default','20050831104125',0,0);
INSERT INTO `revision` VALUES (1049,1049,1049,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1050,1050,1050,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1051,1051,1051,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1052,1052,1052,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1053,1053,1053,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1054,1054,1054,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1055,1055,1055,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1056,1056,1056,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1057,1057,1057,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1058,1058,1058,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1059,1059,1059,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1060,1060,1060,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1061,1061,1061,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1062,1062,1062,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1063,1063,1063,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1064,1064,1064,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1065,1065,1065,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1066,1066,1066,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1067,1067,1067,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1068,1068,1068,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1069,1069,1069,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1070,1070,1070,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1071,1071,1071,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1072,1072,1072,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1073,1073,1073,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1074,1074,1074,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1075,1075,1075,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1076,1076,1076,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1077,1077,1077,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1078,1078,1078,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1079,1079,1079,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1080,1080,1080,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1081,1081,1081,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1082,1082,1082,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1083,1083,1083,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1084,1084,1084,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1085,1085,1085,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1086,1086,1086,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1087,1087,1087,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1088,1088,1088,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1089,1089,1089,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1090,1090,1090,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1091,1091,1091,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1092,1092,1092,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1093,1093,1093,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1094,1094,1094,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1095,1095,1095,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1096,1096,1096,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1097,1097,1097,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1098,1098,1098,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1099,1099,1099,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1100,1100,1100,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1101,1101,1101,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1102,1102,1102,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1103,1103,1103,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1104,1104,1104,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1105,1105,1105,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1106,1106,1106,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1107,1107,1107,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1108,1108,1108,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1109,1109,1109,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1110,1110,1110,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1111,1111,1111,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1112,1112,1112,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1113,1113,1113,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1114,1114,1114,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1115,1115,1115,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1116,1116,1116,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1117,1117,1117,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1118,1118,1118,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1119,1119,1119,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1120,1120,1120,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1121,1121,1121,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1122,1122,1122,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1123,1123,1123,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1124,1124,1124,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1125,1125,1125,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1126,1126,1126,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1127,1127,1127,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1128,1128,1128,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1129,1129,1129,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1130,1130,1130,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1131,1131,1131,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1132,1132,1132,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1133,1133,1133,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1134,1134,1134,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1135,1135,1135,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1136,1136,1136,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1137,1137,1137,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1138,1138,1138,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1139,1139,1139,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1140,1140,1140,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1141,1141,1141,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1142,1142,1142,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1143,1143,1143,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1144,1144,1144,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1145,1145,1145,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1146,1146,1146,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1147,1147,1147,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1148,1148,1148,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1149,1149,1149,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1150,1150,1150,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1151,1151,1151,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1152,1152,1152,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1153,1153,1153,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1154,1154,1154,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1155,1155,1155,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1156,1156,1156,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1157,1157,1157,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1158,1158,1158,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1159,1159,1159,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1160,1160,1160,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1161,1161,1161,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1162,1162,1162,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1163,1163,1163,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1164,1164,1164,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1165,1165,1165,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1166,1166,1166,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1167,1167,1167,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1168,1168,1168,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1169,1169,1169,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1170,1170,1170,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1171,1171,1171,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1172,1172,1172,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1173,1173,1173,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1174,1174,1174,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1175,1175,1175,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1176,1176,1176,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1177,1177,1177,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1178,1178,1178,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1179,1179,1179,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1180,1180,1180,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1181,1181,1181,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1182,1182,1182,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1183,1183,1183,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1184,1184,1184,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1185,1185,1185,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1186,1186,1186,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1187,1187,1187,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1188,1188,1188,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1189,1189,1189,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1190,1190,1190,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1191,1191,1191,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1192,1192,1192,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1193,1193,1193,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1194,1194,1194,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1195,1195,1195,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1196,1196,1196,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1197,1197,1197,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1198,1198,1198,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1199,1199,1199,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1200,1200,1200,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1201,1201,1201,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1202,1202,1202,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1203,1203,1203,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1204,1204,1204,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1205,1205,1205,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1206,1206,1206,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1207,1207,1207,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1208,1208,1208,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1209,1209,1209,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1210,1210,1210,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1211,1211,1211,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1212,1212,1212,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1213,1213,1213,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1214,1214,1214,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1215,1215,1215,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1216,1216,1216,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1217,1217,1217,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1218,1218,1218,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1219,1219,1219,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1220,1220,1220,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1221,1221,1221,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1222,1222,1222,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1223,1223,1223,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1224,1224,1224,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1225,1225,1225,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1226,1226,1226,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1227,1227,1227,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1228,1228,1228,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1229,1229,1229,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1230,1230,1230,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1231,1231,1231,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1232,1232,1232,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1233,1233,1233,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1234,1234,1234,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1235,1235,1235,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1236,1236,1236,'',0,'MediaWiki default','20050831104126',0,0);
INSERT INTO `revision` VALUES (1237,1237,1237,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1238,1238,1238,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1239,1239,1239,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1240,1240,1240,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1241,1241,1241,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1242,1242,1242,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1243,1243,1243,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1244,1244,1244,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1245,1245,1245,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1246,1246,1246,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1247,1247,1247,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1248,1248,1248,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1249,1249,1249,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1250,1250,1250,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1251,1251,1251,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1252,1252,1252,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1253,1253,1253,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1254,1254,1254,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1255,1255,1255,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1256,1256,1256,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1257,1257,1257,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1258,1258,1258,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1259,1259,1259,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1260,1260,1260,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1261,1261,1261,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1262,1262,1262,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1263,1263,1263,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1264,1264,1264,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1265,1265,1265,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1266,1266,1266,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1267,1267,1267,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1268,1268,1268,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1269,1269,1269,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1270,1270,1270,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1271,1271,1271,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1272,1272,1272,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1273,1273,1273,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1274,1274,1274,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1275,1275,1275,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1276,1276,1276,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1277,1277,1277,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1278,1278,1278,'',0,'MediaWiki default','20050831104127',0,0);
INSERT INTO `revision` VALUES (1279,1,1279,'',0,'83.236.12.68','20050831105200',0,0);

--
-- Table structure for table `searchindex`
--

CREATE TABLE `searchindex` (
  `si_page` int(8) unsigned NOT NULL default '0',
  `si_title` varchar(255) NOT NULL default '',
  `si_text` mediumtext NOT NULL,
  UNIQUE KEY `si_page` (`si_page`),
  FULLTEXT KEY `si_title` (`si_title`),
  FULLTEXT KEY `si_text` (`si_text`)
) TYPE=MyISAM;

--
-- Dumping data for table `searchindex`
--

INSERT INTO `searchindex` VALUES (1,'hauptseite',' die wiki software wurde erfolgreich installiert siehe die dokumentation zur anpassung der benutzeroberflu8c3a4che und das benutzerhandbuch fu8c3bcr hilfe zur benutzung und konfiguration asdf test ');

--
-- Table structure for table `site_stats`
--

CREATE TABLE `site_stats` (
  `ss_row_id` int(8) unsigned NOT NULL default '0',
  `ss_total_views` bigint(20) unsigned default '0',
  `ss_total_edits` bigint(20) unsigned default '0',
  `ss_good_articles` bigint(20) unsigned default '0',
  `ss_total_pages` bigint(20) default '-1',
  `ss_users` bigint(20) default '-1',
  `ss_admins` int(10) default '-1',
  UNIQUE KEY `ss_row_id` (`ss_row_id`)
) TYPE=MyISAM;

--
-- Dumping data for table `site_stats`
--

INSERT INTO `site_stats` VALUES (1,22,1,0,1278,2,-1);

--
-- Table structure for table `text`
--

CREATE TABLE `text` (
  `old_id` int(8) unsigned NOT NULL auto_increment,
  `old_text` mediumblob NOT NULL,
  `old_flags` tinyblob NOT NULL,
  PRIMARY KEY  (`old_id`)
) TYPE=MyISAM;

--
-- Dumping data for table `text`
--

INSERT INTO `text` VALUES (1,'Die Wiki Software wurde erfolgreich installiert.\n\nSiehe die [http://meta.wikipedia.org/wiki/MediaWiki_i18n Dokumentation zur Anpassung der Benutzeroberfläche]\nund das [http://meta.wikipedia.org/wiki/MediaWiki_User%27s_Guide Benutzerhandbuch] für Hilfe zur Benutzung und Konfiguration.','utf-8');
INSERT INTO `text` VALUES (2,'$1 wurde nach $2 verschoben','utf-8');
INSERT INTO `text` VALUES (3,'$1 moved to $2 over redirect','utf-8');
INSERT INTO `text` VALUES (4,'/** Do not force \'lowercase\' */\n.portlet h5,\n.portlet h6,\n#p-personal ul,\n#p-cactions li a {\n	text-transform: none;\n}','utf-8');
INSERT INTO `text` VALUES (5,'/* tooltips and access keys */\nta = new Object();\nta[\'pt-userpage\'] = new Array(\'.\',\'My user page\');\nta[\'pt-anonuserpage\'] = new Array(\'.\',\'The user page for the ip you\\\'re editing as\');\nta[\'pt-mytalk\'] = new Array(\'n\',\'My talk page\');\nta[\'pt-anontalk\'] = new Array(\'n\',\'Discussion about edits from this ip address\');\nta[\'pt-preferences\'] = new Array(\'\',\'My preferences\');\nta[\'pt-watchlist\'] = new Array(\'l\',\'The list of pages you\\\'re monitoring for changes.\');\nta[\'pt-mycontris\'] = new Array(\'y\',\'List of my contributions\');\nta[\'pt-login\'] = new Array(\'o\',\'You are encouraged to log in, it is not mandatory however.\');\nta[\'pt-anonlogin\'] = new Array(\'o\',\'You are encouraged to log in, it is not mandatory however.\');\nta[\'pt-logout\'] = new Array(\'o\',\'Log out\');\nta[\'ca-talk\'] = new Array(\'t\',\'Discussion about the content page\');\nta[\'ca-edit\'] = new Array(\'e\',\'You can edit this page. Please use the preview button before saving.\');\nta[\'ca-addsection\'] = new Array(\'+\',\'Add a comment to this discussion.\');\nta[\'ca-viewsource\'] = new Array(\'e\',\'This page is protected. You can view its source.\');\nta[\'ca-history\'] = new Array(\'h\',\'Past versions of this page.\');\nta[\'ca-protect\'] = new Array(\'=\',\'Protect this page\');\nta[\'ca-delete\'] = new Array(\'d\',\'Delete this page\');\nta[\'ca-undelete\'] = new Array(\'d\',\'Restore the edits done to this page before it was deleted\');\nta[\'ca-move\'] = new Array(\'m\',\'Move this page\');\nta[\'ca-watch\'] = new Array(\'w\',\'Add this page to your watchlist\');\nta[\'ca-unwatch\'] = new Array(\'w\',\'Remove this page from your watchlist\');\nta[\'search\'] = new Array(\'f\',\'Search this wiki\');\nta[\'p-logo\'] = new Array(\'\',\'Main Page\');\nta[\'n-mainpage\'] = new Array(\'z\',\'Visit the Main Page\');\nta[\'n-portal\'] = new Array(\'\',\'About the project, what you can do, where to find things\');\nta[\'n-currentevents\'] = new Array(\'\',\'Find background information on current events\');\nta[\'n-recentchanges\'] = new Array(\'r\',\'The list of recent changes in the wiki.\');\nta[\'n-randompage\'] = new Array(\'x\',\'Load a random page\');\nta[\'n-help\'] = new Array(\'\',\'The place to find out.\');\nta[\'n-sitesupport\'] = new Array(\'\',\'Support us\');\nta[\'t-whatlinkshere\'] = new Array(\'j\',\'List of all wiki pages that link here\');\nta[\'t-recentchangeslinked\'] = new Array(\'k\',\'Recent changes in pages linked from this page\');\nta[\'feed-rss\'] = new Array(\'\',\'RSS feed for this page\');\nta[\'feed-atom\'] = new Array(\'\',\'Atom feed for this page\');\nta[\'t-contributions\'] = new Array(\'\',\'View the list of contributions of this user\');\nta[\'t-emailuser\'] = new Array(\'\',\'Send a mail to this user\');\nta[\'t-upload\'] = new Array(\'u\',\'Upload images or media files\');\nta[\'t-specialpages\'] = new Array(\'q\',\'List of all special pages\');\nta[\'ca-nstab-main\'] = new Array(\'c\',\'View the content page\');\nta[\'ca-nstab-user\'] = new Array(\'c\',\'View the user page\');\nta[\'ca-nstab-media\'] = new Array(\'c\',\'View the media page\');\nta[\'ca-nstab-special\'] = new Array(\'\',\'This is a special page, you can\\\'t edit the page itself.\');\nta[\'ca-nstab-wp\'] = new Array(\'a\',\'View the project page\');\nta[\'ca-nstab-image\'] = new Array(\'c\',\'View the image page\');\nta[\'ca-nstab-mediawiki\'] = new Array(\'c\',\'View the system message\');\nta[\'ca-nstab-template\'] = new Array(\'c\',\'View the template\');\nta[\'ca-nstab-help\'] = new Array(\'c\',\'View the help page\');\nta[\'ca-nstab-category\'] = new Array(\'c\',\'View the category page\');','utf-8');
INSERT INTO `text` VALUES (6,'Über','utf-8');
INSERT INTO `text` VALUES (7,'Project:Über_{{SITENAME}}','utf-8');
INSERT INTO `text` VALUES (8,'Über {{SITENAME}}','utf-8');
INSERT INTO `text` VALUES (9,'v','utf-8');
INSERT INTO `text` VALUES (10,'d','utf-8');
INSERT INTO `text` VALUES (11,'i','utf-8');
INSERT INTO `text` VALUES (12,'p','utf-8');
INSERT INTO `text` VALUES (13,'s','utf-8');
INSERT INTO `text` VALUES (14,'f','utf-8');
INSERT INTO `text` VALUES (15,'Das Passwort von $1 wurde an $2 geschickt.','utf-8');
INSERT INTO `text` VALUES (16,'Passwort wurde verschickt.','utf-8');
INSERT INTO `text` VALUES (17,'Sie haben schon $1 Benutzerkonten und können jetzt keine weiteren mehr anlegen.','utf-8');
INSERT INTO `text` VALUES (18,'Aktion beendet','utf-8');
INSERT INTO `text` VALUES (19,'Zur Beobachtungsliste hinzugefügt','utf-8');
INSERT INTO `text` VALUES (20,'Der Artikel \"$1\" wurde zu Ihrer [[Special:Watchlist|Beobachtungsliste]] hinzugefügt.\nSpätere Änderungen an diesem Artikel und der zugehörigen Diskussions-Seite\nwerden dort gelistet und der Artikel wird in der [[Special:Recentchanges|Liste der letzten Änderungen]] <b>fett</b> angezeigt. <p>Wenn Sie den Artikel wieder von ihrer\nBeobachtungsliste entfernen wollen, klicken Sie auf \"Nicht mehr beobachten\"\nam Ende des Artikels.','utf-8');
INSERT INTO `text` VALUES (21,'Add Group','utf-8');
INSERT INTO `text` VALUES (22,'Added group $2','utf-8');
INSERT INTO `text` VALUES (23,'+','utf-8');
INSERT INTO `text` VALUES (24,'Project:Administratoren','utf-8');
INSERT INTO `text` VALUES (25,'All articles','utf-8');
INSERT INTO `text` VALUES (26,'All pages ($1 namespace)','utf-8');
INSERT INTO `text` VALUES (27,'Combined display of upload, deletion, protection, blocking, and sysop logs.\nYou can narrow down the view by selecting a log type, the user name, or the affected page.','utf-8');
INSERT INTO `text` VALUES (28,'Alle MediaWiki-Meldungen','utf-8');
INSERT INTO `text` VALUES (29,'Current text','utf-8');
INSERT INTO `text` VALUES (30,'Default text','utf-8');
INSERT INTO `text` VALUES (31,'Name','utf-8');
INSERT INTO `text` VALUES (32,'Special:AllMessages not supported because wgUseDatabaseMessages is off.','utf-8');
INSERT INTO `text` VALUES (33,'Your current interface language <b>$1</b> is not supported by Special:AllMessages at this site.','utf-8');
INSERT INTO `text` VALUES (34,'Dies ist eine Liste aller möglichen Meldungen im MediaWiki-Namensraum.','utf-8');
INSERT INTO `text` VALUES (35,'All non-articles','utf-8');
INSERT INTO `text` VALUES (36,'All pages (not in $1 namespace)','utf-8');
INSERT INTO `text` VALUES (37,'Alle Artikel','utf-8');
INSERT INTO `text` VALUES (38,'Display pages starting at:','utf-8');
INSERT INTO `text` VALUES (39,'Next','utf-8');
INSERT INTO `text` VALUES (40,'Previous','utf-8');
INSERT INTO `text` VALUES (41,'Go','utf-8');
INSERT INTO `text` VALUES (42,'$1 bis $2','utf-8');
INSERT INTO `text` VALUES (43,'This user is already a bureaucrat','utf-8');
INSERT INTO `text` VALUES (44,'This user is already a steward','utf-8');
INSERT INTO `text` VALUES (45,'This user is already an administrator','utf-8');
INSERT INTO `text` VALUES (46,'<font color=red><b>Benutzer $1, Sie sind bereits angemeldet!</b></font><br />','utf-8');
INSERT INTO `text` VALUES (47,'Die Zurücknahme des Artikels [[$1]] von [[Benutzer:$2|$2]]\n([[Benutzer Diskussion:$2|Diskussion]]) ist nicht möglich, da eine andere\nÄnderung oder Rücknahme erfolgt ist.\n\nDie letzte Änderung ist von [[Benutzer:$3|$3]]\n([[Benutzer Diskussion:$3|Diskussion]])','utf-8');
INSERT INTO `text` VALUES (48,'Älteste Artikel','utf-8');
INSERT INTO `text` VALUES (49,'und','utf-8');
INSERT INTO `text` VALUES (50,'Diskussionsseite dieser IP','utf-8');
INSERT INTO `text` VALUES (51,'---- \'\'Dies ist die Diskussions-Seite eines nicht angemeldeten Benutzers. Wir müssen hier die numerische [[IP-Adresse]] zur Identifizierung verwenden. Eine solche Adresse kann nacheinander von mehreren Benutzern verwendet werden. Wenn Sie ein anonymer Benutzer sind und denken, dass irrelevante Kommentare an Sie gerichtet wurden, [[Spezial:Userlogin|melden Sie sich bitte\n an]], um zukünftige Verwirrung zu vermeiden. \'\'','utf-8');
INSERT INTO `text` VALUES (52,'Anonymous user(s) of {{SITENAME}}','utf-8');
INSERT INTO `text` VALUES (53,'Apr','utf-8');
INSERT INTO `text` VALUES (54,'April','utf-8');
INSERT INTO `text` VALUES (55,'Artikel','utf-8');
INSERT INTO `text` VALUES (56,'Unter diesem Namen existiert bereits ein Artikel.\nBitte wählen Sie einen anderen Namen.','utf-8');
INSERT INTO `text` VALUES (57,'Artikel','utf-8');
INSERT INTO `text` VALUES (58,'Aug','utf-8');
INSERT INTO `text` VALUES (59,'August','utf-8');
INSERT INTO `text` VALUES (60,'Automatische Blockierung, da Sie eine IP-Adresse benutzen mit \"$1\". Grund: \"$2\".','utf-8');
INSERT INTO `text` VALUES (61,'Permission error','utf-8');
INSERT INTO `text` VALUES (62,'The action you have requested is limited\nto users with the \"$2\" permission assigned.\nSee $1.','utf-8');
INSERT INTO `text` VALUES (63,'Diese Aktion kann auf diesen Artikel nicht angewendet werden.','utf-8');
INSERT INTO `text` VALUES (64,'Der Bildname wurde in \"$1\" geändert.','utf-8');
INSERT INTO `text` VALUES (65,'\".$1\" ist kein empfohlenes Dateiformat.','utf-8');
INSERT INTO `text` VALUES (66,'Die IP-Adresse hat ein falsches Format.','utf-8');
INSERT INTO `text` VALUES (67,'Falsche Suchanfrage','utf-8');
INSERT INTO `text` VALUES (68,'Wir konnten Ihre Suchanfrage nicht verarbeiten.\nVermutlich haben Sie versucht, ein Wort zu suchen, das kürzer als zwei Buchstaben ist.\nDies funktioniert im Moment noch nicht.\nMöglicherweise haben Sie auch die Anfrage falsch formuliert, z.B.\n\"Lohn und und Steuern\".\nBitte versuchen Sie eine anders formulierte Anfrage.','utf-8');
INSERT INTO `text` VALUES (69,'Die beiden Passwörter stimmen nicht überein.','utf-8');
INSERT INTO `text` VALUES (70,'Ungültiger Titel','utf-8');
INSERT INTO `text` VALUES (71,'Der Titel der angeforderten Seite war ungültig, leer, oder ein ungültiger Sprachlink von einem anderen Wiki.','utf-8');
INSERT INTO `text` VALUES (72,'(Haupt-)','utf-8');
INSERT INTO `text` VALUES (73,'Ihr Benutzername oder Ihre IP-Adresse wurde von $1 blockiert.\nAls Grund wurde angegeben:<br />$2<p>Bitte kontaktieren Sie den Administrator, um über die Blockierung zu sprechen.','utf-8');
INSERT INTO `text` VALUES (74,'Benutzer ist blockiert','utf-8');
INSERT INTO `text` VALUES (75,'IP-Adresse blockieren','utf-8');
INSERT INTO `text` VALUES (76,'Blockade erfolgreich','utf-8');
INSERT INTO `text` VALUES (77,'Die IP-Adresse \"$1\" wurde blockiert.\n<br />Auf [[Spezial:Ipblocklist|IP block list]] ist eine Liste der Blockaden.','utf-8');
INSERT INTO `text` VALUES (78,'Benutzen Sie das Formular, um eine IP-Adresse zu blockieren.\nDies sollte nur erfolgen, um Vandalismus zu verhindern, in Übereinstimmung mit unseren [[Project:Leitlinien|Leitlinien]].\nBitte tragen Sie den Grund für die Blockade ein.','utf-8');
INSERT INTO `text` VALUES (79,'blockieren','utf-8');
INSERT INTO `text` VALUES (80,'$1, $2 blockierte $3','utf-8');
INSERT INTO `text` VALUES (81,'blockiert [[Benutzer:$1]] - ([[Spezial:Contributions/$1|Beiträge]]) für einen Zeitraum von: $2','utf-8');
INSERT INTO `text` VALUES (82,'Benutzerblockaden-Logbuch','utf-8');
INSERT INTO `text` VALUES (83,'Dies ist ein Log über Sperrungen und Entsperrungen von Benutzern. Automatisch geblockte IP-Adressen werden nicht erfasst. Siehe [[Special:Ipblocklist|IP block list]] für eine Liste der gesperrten Benutzern.','utf-8');
INSERT INTO `text` VALUES (84,'block level','utf-8');
INSERT INTO `text` VALUES (85,'Fetter Text','utf-8');
INSERT INTO `text` VALUES (86,'Fetter Text','utf-8');
INSERT INTO `text` VALUES (87,'Buchhandlungen','utf-8');
INSERT INTO `text` VALUES (88,'Dies ist eine Liste mit Links zu Internetseiten, die neue und gebrauchte Bücher verkaufen. Dort kann es auch weitere Informationen über die Bücher geben, die Sie interessieren. {{SITENAME}} ist mit keinem dieser Anbieter geschäftlich verbunden.','utf-8');
INSERT INTO `text` VALUES (89,'Kaputte Redirects','utf-8');
INSERT INTO `text` VALUES (90,'Die folgenden Redirects leiten zu einem nicht existierenden Artikel weiter','utf-8');
INSERT INTO `text` VALUES (91,'Kontakt','utf-8');
INSERT INTO `text` VALUES (92,'Project:Kontakt','utf-8');
INSERT INTO `text` VALUES (93,'Bureaucrat_log','utf-8');
INSERT INTO `text` VALUES (94,'Rechte für Benutzer \"$1\" auf \"$2\" gesetzt','utf-8');
INSERT INTO `text` VALUES (95,'nach Datum','utf-8');
INSERT INTO `text` VALUES (96,'nach Name','utf-8');
INSERT INTO `text` VALUES (97,'nach Grösse','utf-8');
INSERT INTO `text` VALUES (98,'Das folgende ist eine Kopie aus dem Cache und möglicherweise nicht aktuell.','utf-8');
INSERT INTO `text` VALUES (99,'Abbruch','utf-8');
INSERT INTO `text` VALUES (100,'Kann spezifizierte Seite oder Artikel nicht löschen. (Wurde möglicherweise schon von jemand anderem gelöscht.)','utf-8');
INSERT INTO `text` VALUES (101,'Die Änderung kann nicht zurückgenommen werden; der\nletzte Autor ist der einzige.','utf-8');
INSERT INTO `text` VALUES (102,'Seitenkategorien','utf-8');
INSERT INTO `text` VALUES (103,'The following categories exist in the wiki.','utf-8');
INSERT INTO `text` VALUES (104,'Kategorie','utf-8');
INSERT INTO `text` VALUES (105,'Artikel in der Kategorie \"$1\"','utf-8');
INSERT INTO `text` VALUES (106,'Dieser Kategorie gehören $1 Artikel an.','utf-8');
INSERT INTO `text` VALUES (107,'There is $1 article in this category.','utf-8');
INSERT INTO `text` VALUES (108,'geändert','utf-8');
INSERT INTO `text` VALUES (109,'Changed group $2','utf-8');
INSERT INTO `text` VALUES (110,'Passwort ändern','utf-8');
INSERT INTO `text` VALUES (111,'Änderungen','utf-8');
INSERT INTO `text` VALUES (112,'\'\'\'Beachten Sie:\'\'\' Nach dem Speichern müssen Sie ihrem Browser sagen, die neue Version zu laden: \'\'\'Mozilla:\'\'\' Klick auf \'\'Neu laden\'\'(oder \'\'Strg-R\'\'), \'\'\'IE / Opera:\'\'\' \'\'Strg-F5\'\', \'\'\'Safari:\'\'\' \'\'Cmd-r\'\', \'\'\'Konqueror\'\'\' \'\'Strg-R\'\'.','utf-8');
INSERT INTO `text` VALUES (113,'Spalten','utf-8');
INSERT INTO `text` VALUES (114,'Gewählte Versionen vergleichen','utf-8');
INSERT INTO `text` VALUES (115,'Bestätigen','utf-8');
INSERT INTO `text` VALUES (116,'Löschung bestätigen','utf-8');
INSERT INTO `text` VALUES (117,'Sie sind dabei, einen Artikel oder ein Bild und alle älteren Versionen permanent aus der Datenbank zu löschen.\nBitte bestätigen Sie Ihre Absicht, dies zu tun, dass Sie sich der Konsequenzen bewusst sind, und dass Sie in Übereinstimmung mit unseren [[Project:Leitlinien|Leitlinien]] handeln.','utf-8');
INSERT INTO `text` VALUES (118,'E-Mail-Adressenbestätigung (Authentifizierung)','utf-8');
INSERT INTO `text` VALUES (119,'Jemand, vielleicht Sie, hat von IP-Adresse $1\nein Benutzerkonto \"$2\" mit dieser E-Mailadresse bei {{SITENAME}} angemeldet.\n\nZur Bestätigung, dass dieses Konto wirklich Ihnen gehört, und um die erweiterten\nE-Mailfunktionen für Sie bei {{SITENAME}} einzuschalten, öffnen Sie bitte den folgenden Link\nin Ihrem Browser:\n\n$3\n\nWenn Sie *nicht* $2 sind, folgen Sie dem Link bitte nicht.\n\nDer Bestätigungskode läuft am $4 ab.','utf-8');
INSERT INTO `text` VALUES (120,'Es gab einen Fehler bei der Bestätigung Ihrer E-Mailadresse.','utf-8');
INSERT INTO `text` VALUES (121,'Ungültiger Bestätigungskode. Die Gültigkeitsdauer des Kodes ist eventuell abgelaufen.','utf-8');
INSERT INTO `text` VALUES (122,'Ihre E-Mailadresse ist nun bestätigt.','utf-8');
INSERT INTO `text` VALUES (123,'Anforderung einer E-Mail zur Adressenbestätigung','utf-8');
INSERT INTO `text` VALUES (124,'Could not send confirmation mail due to misconfigured server or invalid characters in e-mail address.','utf-8');
INSERT INTO `text` VALUES (125,'Es wurde Ihnen eine Mail zur Adressenbestätigung gesendet.','utf-8');
INSERT INTO `text` VALUES (126,'{{SITENAME}} E-Mail-Adressenbestätigung (Authentifizierung)','utf-8');
INSERT INTO `text` VALUES (127,'Ihre E-Mailadresse wurde bestätigt. Sie können sich jetzt einloggen.','utf-8');
INSERT INTO `text` VALUES (128,'Diese Wiki erfordert, dass Sie Ihre E-Mailadresse bestätigen (authentifizieren),\nbevor Sie die erweiterten Mailfunktionen benutzen können. Ein Klick auf die Schaltfläche unten sendet eine E-Mail zu Ihnen.\nDiese Mail enthält einen Link mit einem Kode; durch Klicken auf diesen Link bestätigen Sie, dass Ihre Adresse gültig ist.','utf-8');
INSERT INTO `text` VALUES (129,'Sperrung bestätigen','utf-8');
INSERT INTO `text` VALUES (130,'Soll diese Seite wirklich geschützt werden?','utf-8');
INSERT INTO `text` VALUES (131,'Aufhebung der Sperrung bestätigen','utf-8');
INSERT INTO `text` VALUES (132,'Wollen Sie wirklich die Sperrung dieser Seite aufheben?','utf-8');
INSERT INTO `text` VALUES (133,'Zeichen pro Zeile','utf-8');
INSERT INTO `text` VALUES (134,'Zeilen pro Treffer','utf-8');
INSERT INTO `text` VALUES (135,'$1 minor edits','utf-8');
INSERT INTO `text` VALUES (136,'Beiträge','utf-8');
INSERT INTO `text` VALUES (137,'Für $1','utf-8');
INSERT INTO `text` VALUES (138,'Benutzerbeiträge','utf-8');
INSERT INTO `text` VALUES (139,'all','utf-8');
INSERT INTO `text` VALUES (140,'Inhalt ist verfügbar unter der $1.','utf-8');
INSERT INTO `text` VALUES (141,'Project:Copyright','utf-8');
INSERT INTO `text` VALUES (142,'{{SITENAME}} copyright','utf-8');
INSERT INTO `text` VALUES (143,'\n<b>Bitte <font size=\'+1\'>kopieren Sie keine Webseiten</font>, die nicht Ihre eigenen sind, benutzen Sie <font size=\'+1\'>keine urheberrechtlich geschützten Werke</font> ohne Erlaubnis des Copyright-Inhabers!</b>\n<p>Sie geben uns hiermit ihre Zusage, dass Sie den Text <strong>selbst verfasst</strong> haben, dass der Text Allgemeingut (<strong>public domain</strong>) ist, oder dass der <strong>Copyright-Inhaber</strong> seine <strong>Zustimmung</strong> gegeben hat. Falls dieser Text bereits woanders veröffentlicht wurde, weisen Sie bitte auf der \'Diskussion:\'-Seite darauf hin.\n<p><i>Bitte beachten Sie, dass alle {{SITENAME}}-Beiträge automatisch unter der \"GNU Freie Dokumentationslizenz\" stehen. Falls Sie nicht möchten, dass Ihre Arbeit hier von anderen verändert und verbreitet wird, dann drücken Sie nicht auf \"Speichern\".</i>','utf-8');
INSERT INTO `text` VALUES (144,'Please note that all contributions to {{SITENAME}}\nmay be edited, altered, or removed by other contributors.\nIf you don\'t want your writing to be edited mercilessly, then don\'t submit it here.<br />\nYou are also promising us that you wrote this yourself, or copied it from a\npublic domain or similar free resource (see $1 for details).\n<strong>DO NOT SUBMIT COPYRIGHTED WORK WITHOUT PERMISSION!</strong>','utf-8');
INSERT INTO `text` VALUES (145,'Der Eintrag \'$1\' kann nicht gelöscht werden...','utf-8');
INSERT INTO `text` VALUES (146,'Neues Benutzerkonto anlegen','utf-8');
INSERT INTO `text` VALUES (147,'über eMail','utf-8');
INSERT INTO `text` VALUES (148,'createaccount level','utf-8');
INSERT INTO `text` VALUES (149,'Create article','utf-8');
INSERT INTO `text` VALUES (150,'neu angelegt','utf-8');
INSERT INTO `text` VALUES (151,'Page credits','utf-8');
INSERT INTO `text` VALUES (152,'Aktuell','utf-8');
INSERT INTO `text` VALUES (153,'Aktuelle Ereignisse','utf-8');
INSERT INTO `text` VALUES (154,'Current events','utf-8');
INSERT INTO `text` VALUES (155,'Aktuelle Version','utf-8');
INSERT INTO `text` VALUES (156,'view current revision','utf-8');
INSERT INTO `text` VALUES (157,'Data','utf-8');
INSERT INTO `text` VALUES (158,'Fehler in der Datenbank','utf-8');
INSERT INTO `text` VALUES (159,'Datumsformat','utf-8');
INSERT INTO `text` VALUES (160,'Es gab einen Syntaxfehler in der Datenbankabfrage.\nDie letzte Datenbankabfrage lautete:\n<blockquote><tt>$1</tt></blockquote>\naus der Funktion \"<tt>$2</tt>\".\nMySQL meldete den Fehler \"<tt>$3: $4</tt>\".','utf-8');
INSERT INTO `text` VALUES (161,'Es gab einen Syntaxfehler in der Datenbankabfrage.\nDie letzte Datenbankabfrage lautete: \"$1\" aus der Funktion \"<tt>$2</tt>\".\nMySQL meldete den Fehler: \"<tt>$3: $4</tt>\".','utf-8');
INSERT INTO `text` VALUES (162,'Sackgassenartikel','utf-8');
INSERT INTO `text` VALUES (163,'Debug','utf-8');
INSERT INTO `text` VALUES (164,'Dez','utf-8');
INSERT INTO `text` VALUES (165,'Dezember','utf-8');
INSERT INTO `text` VALUES (166,'default','utf-8');
INSERT INTO `text` VALUES (167,'In diesen Namensräumen soll standardmäßig gesucht werden:','utf-8');
INSERT INTO `text` VALUES (168,'{{SITENAME}} e-mail','utf-8');
INSERT INTO `text` VALUES (169,'löschen','utf-8');
INSERT INTO `text` VALUES (170,'Delete and move','utf-8');
INSERT INTO `text` VALUES (171,'Deleted to make way for move','utf-8');
INSERT INTO `text` VALUES (172,'==Deletion required==\n\nThe destination article \"[[$1]]\" already exists. Do you want to delete it to make way for the move?','utf-8');
INSERT INTO `text` VALUES (173,'Grund der Löschung','utf-8');
INSERT INTO `text` VALUES (174,'\"$1\" gelöscht','utf-8');
INSERT INTO `text` VALUES (175,'[deleted]','utf-8');
INSERT INTO `text` VALUES (176,'Deleted old revision $1.','utf-8');
INSERT INTO `text` VALUES (177,'\"$1\" wurde gelöscht.\nIm $2 finden Sie eine Liste der letzten Löschungen.','utf-8');
INSERT INTO `text` VALUES (178,'Löschen','utf-8');
INSERT INTO `text` VALUES (179,'Löschen','utf-8');
INSERT INTO `text` VALUES (180,'Seite löschen','utf-8');
INSERT INTO `text` VALUES (181,'delete level','utf-8');
INSERT INTO `text` VALUES (182,'(Lösche \"$1\")','utf-8');
INSERT INTO `text` VALUES (183,'Diese Seite löschen','utf-8');
INSERT INTO `text` VALUES (184,'Lösch-Logbuch','utf-8');
INSERT INTO `text` VALUES (185,'Lösch-Logbuch','utf-8');
INSERT INTO `text` VALUES (186,'Hier ist eine Liste der letzten Löschungen (UTC).\n<ul>\n</ul>','utf-8');
INSERT INTO `text` VALUES (187,'Destination filename','utf-8');
INSERT INTO `text` VALUES (188,'Dieser Vorgang kann aus Sicherheitsgründen nur von Benutzern mit\"Entwickler\"-Status durchgeführt werden. Siehe auch $1.','utf-8');
INSERT INTO `text` VALUES (189,'Entwickler-Zugang notwendig','utf-8');
INSERT INTO `text` VALUES (190,'Unterschied','utf-8');
INSERT INTO `text` VALUES (191,'(Unterschied zwischen Versionen)','utf-8');
INSERT INTO `text` VALUES (192,'Begriffsklärungsseiten','utf-8');
INSERT INTO `text` VALUES (193,'Project:Begriffsklärung','utf-8');
INSERT INTO `text` VALUES (194,'Die folgenden Artikel verweisen auf eine <i>Seite zur Begriffsklärung</i>. Sie sollten statt dessen auf die eigentlich gemeinte Seite verweisen.<br />Eine Seite wird als Begriffsklärungsseite behandelt, wenn $1 auf sie verweist.<br />Verweise aus Namensräumen werden hier <i>nicht</i> aufgelistet.','utf-8');
INSERT INTO `text` VALUES (195,'{{ns:4}}:Lizenzbestimmungen','utf-8');
INSERT INTO `text` VALUES (196,'Lizenzbestimmungen','utf-8');
INSERT INTO `text` VALUES (197,'Doppelte Redirects','utf-8');
INSERT INTO `text` VALUES (198,'<b>Achtung:</b> Diese Liste kann \"falsche Positive\" enthalten. Das ist dann der Fall, wenn ein Redirect außer dem Redirect-Verweis noch weiteren Text mit anderen Verweisen enthält. Letztere sollten dann entfernt werden.','utf-8');
INSERT INTO `text` VALUES (199,'A confirmation email has been sent to the nominated email address.\nBefore any other mail is sent to the account, you will have to follow the instructions in the email,\nto confirm that the account is actually yours.','utf-8');
INSERT INTO `text` VALUES (200,'bearbeiten','utf-8');
INSERT INTO `text` VALUES (201,'Diese Datei mit einem externen Programm bearbeiten','utf-8');
INSERT INTO `text` VALUES (202,'Siehe [http://meta.wikimedia.org/wiki/Hilfe:Externe_Editoren Installations-Anweisungen] für weitere Informationen','utf-8');
INSERT INTO `text` VALUES (203,'Der Änderungskommentar war: \"<i>$1</i>\".','utf-8');
INSERT INTO `text` VALUES (204,'Bearbeitungs-Konflikt: $1','utf-8');
INSERT INTO `text` VALUES (205,'Die aktuelle Version dieses Artikels bearbeiten','utf-8');
INSERT INTO `text` VALUES (206,'Edit Group','utf-8');
INSERT INTO `text` VALUES (207,'Bearbeitungshilfe','utf-8');
INSERT INTO `text` VALUES (208,'{{SITENAME}}:Editierhilfe','utf-8');
INSERT INTO `text` VALUES (209,'Bearbeiten von $1','utf-8');
INSERT INTO `text` VALUES (210,'Bearbeiten von $1 (Kommentar)','utf-8');
INSERT INTO `text` VALUES (211,'<strong>ACHTUNG: Sie bearbeiten eine alte Version dieses Artikels.\nWenn Sie speichern, werden alle neueren Versionen überschrieben.</strong>','utf-8');
INSERT INTO `text` VALUES (212,'Bearbeiten von $1 (Absatz)','utf-8');
INSERT INTO `text` VALUES (213,'bearbeiten','utf-8');
INSERT INTO `text` VALUES (214,'Seite bearbeiten','utf-8');
INSERT INTO `text` VALUES (215,'Edit User Groups','utf-8');
INSERT INTO `text` VALUES (216,'Email','utf-8');
INSERT INTO `text` VALUES (217,'Ihre E-Mail-Adresse wurde am $1 authentifiziert.','utf-8');
INSERT INTO `text` VALUES (218,'E-Mail-Adresse bestätigen (authentifizieren).','utf-8');
INSERT INTO `text` VALUES (219,'Keine E-Mail von anderen Benutzern erhalten','utf-8');
INSERT INTO `text` VALUES (220,'Falls Sie Ihr Passwort vergessen haben, kann Ihnen ein neues an Ihre E-Mail-Adresse gesendet werden.','utf-8');
INSERT INTO `text` VALUES (221,'Von','utf-8');
INSERT INTO `text` VALUES (222,'Nachricht','utf-8');
INSERT INTO `text` VALUES (223,'Ihre E-Mail-Adresse ist <strong>noch nicht authentifiziert</strong> und die erweiterten E-Mailfunktionen sind bis zur Authentifizierung ohne Funktion <strong>(aus)</strong>.<br />\nFür die Authentifizierung melden Sie sich bitte mit dem per E-Mail geschickten temporären Passwort an, oder fordern Sie auf der Anmeldeseite ein neues an.','utf-8');
INSERT INTO `text` VALUES (224,'E-Mail an Benutzer','utf-8');
INSERT INTO `text` VALUES (225,'Wenn dieser Benutzer eine gültige E-Mail-Adresse angegeben hat, können Sie ihm mit dem untenstehenden Formular eine E-Mail senden. Als Absender wird die E-Mail-Adresse aus Ihren Einstellungen eingetragen, damit der Benutzer Ihnen antworten kann.','utf-8');
INSERT INTO `text` VALUES (226,'Senden','utf-8');
INSERT INTO `text` VALUES (227,'E-Mail verschickt','utf-8');
INSERT INTO `text` VALUES (228,'Ihre E-Mail wurde verschickt.','utf-8');
INSERT INTO `text` VALUES (229,'Betreff','utf-8');
INSERT INTO `text` VALUES (230,'An','utf-8');
INSERT INTO `text` VALUES (231,'E-Mail an diesen Benutzer','utf-8');
INSERT INTO `text` VALUES (232,'Die hochgeladene Datei ist leer. Der Grund kann ein Tippfehler im Dateinamen sein. Bitte kontrollieren Sie, ob Sie die Datei wirklich hochladen wollen.','utf-8');
INSERT INTO `text` VALUES (233,'Liebe/r $WATCHINGUSERNAME,\n\ndie {{SITENAME}} Seite $PAGETITLE wurde von $PAGEEDITOR am $PAGEEDITDATE $CHANGEDORCREATED,\ndie aktuelle Version ist: $PAGETITLE_URL\n\n$NEWPAGE\n\nZusammenfassung des Editors: $PAGESUMMARY $PAGEMINOREDIT\nKontakt zum Editor:\nMail $PAGEEDITOR_EMAIL\nWiki $PAGEEDITOR_WIKI\n\nEs werden solange keine weiteren Benachrichtigungsmails gesendet, bis Sie die Seite wieder besuchen. Auf Ihrer Beobachtungsseite können Sie alle Benachrichtigungsmarker zusammen zurücksetzen.\n\n             Ihr freundliches {{SITENAME}} Benachrichtigungssystem\n\n---\nIhre Beobachtungsliste {{SERVER}}{{localurl:Special:Watchlist/edit}}\nHilfe zur Benutzung gibt {{SERVER}}{{localurl:WikiHelpdesk}}','utf-8');
INSERT INTO `text` VALUES (234,'$1 zeigt alle Änderungen auf einen Blick.','utf-8');
INSERT INTO `text` VALUES (235,'{{SITENAME}} E-Mail-Benachrichtigungsdienst','utf-8');
INSERT INTO `text` VALUES (236,'Dies ist eine neue Seite.','utf-8');
INSERT INTO `text` VALUES (237,'Alle Benachrichtigungsmarker zurücksetzen (alle Seiten als \"gesehen\" markieren)','utf-8');
INSERT INTO `text` VALUES (238,'Die {{SITENAME}} Seite $PAGETITLE wurde von $PAGEEDITOR $CHANGEDORCREATED','utf-8');
INSERT INTO `text` VALUES (239,'Bitte geben Sie einen Grund ein, warum die Datenbank\ngesperrt werden soll und eine Abschätzung über die Dauer der Sperrung','utf-8');
INSERT INTO `text` VALUES (240,'Fehler','utf-8');
INSERT INTO `text` VALUES (241,'Fehler','utf-8');
INSERT INTO `text` VALUES (242,'Inhalt vor dem Leeren der Seite: \'$1\'','utf-8');
INSERT INTO `text` VALUES (243,'Seite war leer','utf-8');
INSERT INTO `text` VALUES (244,'Alter Inhalt: \'$1\'','utf-8');
INSERT INTO `text` VALUES (245,'content was: \'$1\' (and the only contributor was \'$2\')','utf-8');
INSERT INTO `text` VALUES (246,'Blendenwert','utf-8');
INSERT INTO `text` VALUES (247,'Fotograf','utf-8');
INSERT INTO `text` VALUES (248,'Bits per component','utf-8');
INSERT INTO `text` VALUES (249,'Brightness Value','utf-8');
INSERT INTO `text` VALUES (250,'CFA pattern','utf-8');
INSERT INTO `text` VALUES (251,'Farbraum','utf-8');
INSERT INTO `text` VALUES (252,'sRGB','utf-8');
INSERT INTO `text` VALUES (253,'FFFF.H','utf-8');
INSERT INTO `text` VALUES (254,'Meaning of each component','utf-8');
INSERT INTO `text` VALUES (255,'does not exist','utf-8');
INSERT INTO `text` VALUES (256,'Y','utf-8');
INSERT INTO `text` VALUES (257,'Cb','utf-8');
INSERT INTO `text` VALUES (258,'Cr','utf-8');
INSERT INTO `text` VALUES (259,'R','utf-8');
INSERT INTO `text` VALUES (260,'G','utf-8');
INSERT INTO `text` VALUES (261,'B','utf-8');
INSERT INTO `text` VALUES (262,'Image compression mode','utf-8');
INSERT INTO `text` VALUES (263,'Compression scheme','utf-8');
INSERT INTO `text` VALUES (264,'Uncompressed','utf-8');
INSERT INTO `text` VALUES (265,'JPEG','utf-8');
INSERT INTO `text` VALUES (266,'Kontrast','utf-8');
INSERT INTO `text` VALUES (267,'Normal','utf-8');
INSERT INTO `text` VALUES (268,'Soft','utf-8');
INSERT INTO `text` VALUES (269,'Hard','utf-8');
INSERT INTO `text` VALUES (270,'Copyright','utf-8');
INSERT INTO `text` VALUES (271,'Custom image processing','utf-8');
INSERT INTO `text` VALUES (272,'Normal process','utf-8');
INSERT INTO `text` VALUES (273,'Custom process','utf-8');
INSERT INTO `text` VALUES (274,'File change date and time','utf-8');
INSERT INTO `text` VALUES (275,'Date and time of digitizing','utf-8');
INSERT INTO `text` VALUES (276,'Date and time of data generation','utf-8');
INSERT INTO `text` VALUES (277,'Device settings description','utf-8');
INSERT INTO `text` VALUES (278,'Digital zoom ratio','utf-8');
INSERT INTO `text` VALUES (279,'Exif version','utf-8');
INSERT INTO `text` VALUES (280,'Belichtungsvorgabe','utf-8');
INSERT INTO `text` VALUES (281,'Exposure index','utf-8');
INSERT INTO `text` VALUES (282,'Belichtungsmodus','utf-8');
INSERT INTO `text` VALUES (283,'Auto exposure','utf-8');
INSERT INTO `text` VALUES (284,'Manual exposure','utf-8');
INSERT INTO `text` VALUES (285,'Auto bracket','utf-8');
INSERT INTO `text` VALUES (286,'Belichtungsprogramm','utf-8');
INSERT INTO `text` VALUES (287,'Not defined','utf-8');
INSERT INTO `text` VALUES (288,'Manual','utf-8');
INSERT INTO `text` VALUES (289,'Normal program','utf-8');
INSERT INTO `text` VALUES (290,'Aperture priority','utf-8');
INSERT INTO `text` VALUES (291,'Shutter priority','utf-8');
INSERT INTO `text` VALUES (292,'Creative program (biased toward depth of field)','utf-8');
INSERT INTO `text` VALUES (293,'Action program (biased toward fast shutter speed)','utf-8');
INSERT INTO `text` VALUES (294,'Portrait mode (for closeup photos with the background out of focus)','utf-8');
INSERT INTO `text` VALUES (295,'Landscape mode (for landscape photos with the background in focus)','utf-8');
INSERT INTO `text` VALUES (296,'Belichtungsdauer','utf-8');
INSERT INTO `text` VALUES (297,'File source','utf-8');
INSERT INTO `text` VALUES (298,'DSC','utf-8');
INSERT INTO `text` VALUES (299,'Blitz','utf-8');
INSERT INTO `text` VALUES (300,'Blitzstärke','utf-8');
INSERT INTO `text` VALUES (301,'Supported Flashpix version','utf-8');
INSERT INTO `text` VALUES (302,'F-Wert','utf-8');
INSERT INTO `text` VALUES (303,'Brennweite','utf-8');
INSERT INTO `text` VALUES (304,'Brennweite (Kleinbildäquivalent)','utf-8');
INSERT INTO `text` VALUES (305,'Focal plane resolution unit','utf-8');
INSERT INTO `text` VALUES (306,'inches','utf-8');
INSERT INTO `text` VALUES (307,'Focal plane X resolution','utf-8');
INSERT INTO `text` VALUES (308,'Focal plane Y resolution','utf-8');
INSERT INTO `text` VALUES (309,'Scene control','utf-8');
INSERT INTO `text` VALUES (310,'None','utf-8');
INSERT INTO `text` VALUES (311,'Low gain up','utf-8');
INSERT INTO `text` VALUES (312,'High gain up','utf-8');
INSERT INTO `text` VALUES (313,'Low gain down','utf-8');
INSERT INTO `text` VALUES (314,'High gain down','utf-8');
INSERT INTO `text` VALUES (315,'Altitude','utf-8');
INSERT INTO `text` VALUES (316,'Altitude reference','utf-8');
INSERT INTO `text` VALUES (317,'Name of GPS area','utf-8');
INSERT INTO `text` VALUES (318,'GPS date','utf-8');
INSERT INTO `text` VALUES (319,'Bearing of destination','utf-8');
INSERT INTO `text` VALUES (320,'Reference for bearing of destination','utf-8');
INSERT INTO `text` VALUES (321,'Distance to destination','utf-8');
INSERT INTO `text` VALUES (322,'Reference for distance to destination','utf-8');
INSERT INTO `text` VALUES (323,'Latitude destination','utf-8');
INSERT INTO `text` VALUES (324,'Reference for latitude of destination','utf-8');
INSERT INTO `text` VALUES (325,'Longitude of destination','utf-8');
INSERT INTO `text` VALUES (326,'Reference for longitude of destination','utf-8');
INSERT INTO `text` VALUES (327,'GPS differential correction','utf-8');
INSERT INTO `text` VALUES (328,'Magnetic direction','utf-8');
INSERT INTO `text` VALUES (329,'True direction','utf-8');
INSERT INTO `text` VALUES (330,'Measurement precision','utf-8');
INSERT INTO `text` VALUES (331,'Direction of image','utf-8');
INSERT INTO `text` VALUES (332,'Reference for direction of image','utf-8');
INSERT INTO `text` VALUES (333,'Latitude','utf-8');
INSERT INTO `text` VALUES (334,'North latitude','utf-8');
INSERT INTO `text` VALUES (335,'South latitude','utf-8');
INSERT INTO `text` VALUES (336,'North or South Latitude','utf-8');
INSERT INTO `text` VALUES (337,'Longitude','utf-8');
INSERT INTO `text` VALUES (338,'East longitude','utf-8');
INSERT INTO `text` VALUES (339,'West longitude','utf-8');
INSERT INTO `text` VALUES (340,'East or West Longitude','utf-8');
INSERT INTO `text` VALUES (341,'Geodetic survey data used','utf-8');
INSERT INTO `text` VALUES (342,'Measurement mode','utf-8');
INSERT INTO `text` VALUES (343,'2-dimensional measurement','utf-8');
INSERT INTO `text` VALUES (344,'3-dimensional measurement','utf-8');
INSERT INTO `text` VALUES (345,'Name of GPS processing method','utf-8');
INSERT INTO `text` VALUES (346,'Satellites used for measurement','utf-8');
INSERT INTO `text` VALUES (347,'Speed of GPS receiver','utf-8');
INSERT INTO `text` VALUES (348,'Kilometres per hour','utf-8');
INSERT INTO `text` VALUES (349,'Miles per hour','utf-8');
INSERT INTO `text` VALUES (350,'Knots','utf-8');
INSERT INTO `text` VALUES (351,'Speed unit','utf-8');
INSERT INTO `text` VALUES (352,'Receiver status','utf-8');
INSERT INTO `text` VALUES (353,'Measurement in progress','utf-8');
INSERT INTO `text` VALUES (354,'Measurement interoperability','utf-8');
INSERT INTO `text` VALUES (355,'GPS time (atomic clock)','utf-8');
INSERT INTO `text` VALUES (356,'Direction of movement','utf-8');
INSERT INTO `text` VALUES (357,'Reference for direction of movement','utf-8');
INSERT INTO `text` VALUES (358,'GPS tag version','utf-8');
INSERT INTO `text` VALUES (359,'Image title','utf-8');
INSERT INTO `text` VALUES (360,'Länge','utf-8');
INSERT INTO `text` VALUES (361,'Unique image ID','utf-8');
INSERT INTO `text` VALUES (362,'Breite','utf-8');
INSERT INTO `text` VALUES (363,'Filmempfindlichkeit (ISO)','utf-8');
INSERT INTO `text` VALUES (364,'Offset to JPEG SOI','utf-8');
INSERT INTO `text` VALUES (365,'Bytes of JPEG data','utf-8');
INSERT INTO `text` VALUES (366,'Lichtquelle','utf-8');
INSERT INTO `text` VALUES (367,'Unknown','utf-8');
INSERT INTO `text` VALUES (368,'Daylight','utf-8');
INSERT INTO `text` VALUES (369,'Clody weather','utf-8');
INSERT INTO `text` VALUES (370,'Shade','utf-8');
INSERT INTO `text` VALUES (371,'Daylight fluorescent (D 5700 – 7100K)','utf-8');
INSERT INTO `text` VALUES (372,'Day white fluorescent (N 4600 – 5400K)','utf-8');
INSERT INTO `text` VALUES (373,'Cool white fluorescent (W 3900 – 4500K)','utf-8');
INSERT INTO `text` VALUES (374,'White fluorescent (WW 3200 – 3700K)','utf-8');
INSERT INTO `text` VALUES (375,'Standard light A','utf-8');
INSERT INTO `text` VALUES (376,'Standard light B','utf-8');
INSERT INTO `text` VALUES (377,'Standard light C','utf-8');
INSERT INTO `text` VALUES (378,'Fluorescent','utf-8');
INSERT INTO `text` VALUES (379,'D55','utf-8');
INSERT INTO `text` VALUES (380,'D65','utf-8');
INSERT INTO `text` VALUES (381,'D75','utf-8');
INSERT INTO `text` VALUES (382,'D50','utf-8');
INSERT INTO `text` VALUES (383,'ISO studio tungsten','utf-8');
INSERT INTO `text` VALUES (384,'Other light source','utf-8');
INSERT INTO `text` VALUES (385,'Tungsten (incandescent light)','utf-8');
INSERT INTO `text` VALUES (386,'Flash','utf-8');
INSERT INTO `text` VALUES (387,'Fine weather','utf-8');
INSERT INTO `text` VALUES (388,'Hersteller','utf-8');
INSERT INTO `text` VALUES (389,'$1','utf-8');
INSERT INTO `text` VALUES (390,'Manufacturer notes','utf-8');
INSERT INTO `text` VALUES (391,'Größte Blende','utf-8');
INSERT INTO `text` VALUES (392,'Messverfahren','utf-8');
INSERT INTO `text` VALUES (393,'Unknown','utf-8');
INSERT INTO `text` VALUES (394,'Average','utf-8');
INSERT INTO `text` VALUES (395,'CenterWeightedAverage','utf-8');
INSERT INTO `text` VALUES (396,'Other','utf-8');
INSERT INTO `text` VALUES (397,'Spot','utf-8');
INSERT INTO `text` VALUES (398,'MultiSpot','utf-8');
INSERT INTO `text` VALUES (399,'Pattern','utf-8');
INSERT INTO `text` VALUES (400,'Partial','utf-8');
INSERT INTO `text` VALUES (401,'Modell','utf-8');
INSERT INTO `text` VALUES (402,'$1','utf-8');
INSERT INTO `text` VALUES (403,'Optoelectronic conversion factor','utf-8');
INSERT INTO `text` VALUES (404,'Orientierung','utf-8');
INSERT INTO `text` VALUES (405,'Normal','utf-8');
INSERT INTO `text` VALUES (406,'Flipped horizontally','utf-8');
INSERT INTO `text` VALUES (407,'Rotated 180°','utf-8');
INSERT INTO `text` VALUES (408,'Flipped vertically','utf-8');
INSERT INTO `text` VALUES (409,'Rotated 90° CCW and flipped vertically','utf-8');
INSERT INTO `text` VALUES (410,'Rotated 90° CW','utf-8');
INSERT INTO `text` VALUES (411,'Rotated 90° CW and flipped vertically','utf-8');
INSERT INTO `text` VALUES (412,'Rotated 90° CCW','utf-8');
INSERT INTO `text` VALUES (413,'Pixel composition','utf-8');
INSERT INTO `text` VALUES (414,'RGB','utf-8');
INSERT INTO `text` VALUES (415,'YCbCr','utf-8');
INSERT INTO `text` VALUES (416,'Valind image height','utf-8');
INSERT INTO `text` VALUES (417,'Valid image width','utf-8');
INSERT INTO `text` VALUES (418,'Data arrangement','utf-8');
INSERT INTO `text` VALUES (419,'chunky format','utf-8');
INSERT INTO `text` VALUES (420,'planar format','utf-8');
INSERT INTO `text` VALUES (421,'Chromaticities of primarities','utf-8');
INSERT INTO `text` VALUES (422,'Schwarz/Weiß-Referenzpunkte','utf-8');
INSERT INTO `text` VALUES (423,'Related audio file','utf-8');
INSERT INTO `text` VALUES (424,'Masseinheit der Auflösung','utf-8');
INSERT INTO `text` VALUES (425,'Number of rows per strip','utf-8');
INSERT INTO `text` VALUES (426,'Number of components','utf-8');
INSERT INTO `text` VALUES (427,'Sättigung','utf-8');
INSERT INTO `text` VALUES (428,'Normal','utf-8');
INSERT INTO `text` VALUES (429,'Low saturation','utf-8');
INSERT INTO `text` VALUES (430,'High saturation','utf-8');
INSERT INTO `text` VALUES (431,'Scene capture type','utf-8');
INSERT INTO `text` VALUES (432,'Standard','utf-8');
INSERT INTO `text` VALUES (433,'Landscape','utf-8');
INSERT INTO `text` VALUES (434,'Portrait','utf-8');
INSERT INTO `text` VALUES (435,'Night scene','utf-8');
INSERT INTO `text` VALUES (436,'Scene type','utf-8');
INSERT INTO `text` VALUES (437,'A directly photographed image','utf-8');
INSERT INTO `text` VALUES (438,'Sensing method','utf-8');
INSERT INTO `text` VALUES (439,'Undefined','utf-8');
INSERT INTO `text` VALUES (440,'One-chip color area sensor','utf-8');
INSERT INTO `text` VALUES (441,'Two-chip color area sensor','utf-8');
INSERT INTO `text` VALUES (442,'Three-chip color area sensor','utf-8');
INSERT INTO `text` VALUES (443,'Color sequential area sensor','utf-8');
INSERT INTO `text` VALUES (444,'Trilinear sensor','utf-8');
INSERT INTO `text` VALUES (445,'Color sequential linear sensor','utf-8');
INSERT INTO `text` VALUES (446,'Schärfe','utf-8');
INSERT INTO `text` VALUES (447,'Normal','utf-8');
INSERT INTO `text` VALUES (448,'Soft','utf-8');
INSERT INTO `text` VALUES (449,'Hard','utf-8');
INSERT INTO `text` VALUES (450,'Shutter Speed Value','utf-8');
INSERT INTO `text` VALUES (451,'Software','utf-8');
INSERT INTO `text` VALUES (452,'$1','utf-8');
INSERT INTO `text` VALUES (453,'Spatial frequency response','utf-8');
INSERT INTO `text` VALUES (454,'Spectral Sensitivity','utf-8');
INSERT INTO `text` VALUES (455,'Bytes per compressed strip','utf-8');
INSERT INTO `text` VALUES (456,'Image data location','utf-8');
INSERT INTO `text` VALUES (457,'Subject area','utf-8');
INSERT INTO `text` VALUES (458,'Entfernung','utf-8');
INSERT INTO `text` VALUES (459,'$1 metres','utf-8');
INSERT INTO `text` VALUES (460,'Subject distance range','utf-8');
INSERT INTO `text` VALUES (461,'Unknown','utf-8');
INSERT INTO `text` VALUES (462,'Macro','utf-8');
INSERT INTO `text` VALUES (463,'Close view','utf-8');
INSERT INTO `text` VALUES (464,'Distant view','utf-8');
INSERT INTO `text` VALUES (465,'Subject location','utf-8');
INSERT INTO `text` VALUES (466,'DateTime subseconds','utf-8');
INSERT INTO `text` VALUES (467,'DateTimeDigitized subseconds','utf-8');
INSERT INTO `text` VALUES (468,'DateTimeOriginal subseconds','utf-8');
INSERT INTO `text` VALUES (469,'Transfer function','utf-8');
INSERT INTO `text` VALUES (470,'User comments','utf-8');
INSERT INTO `text` VALUES (471,'Weißabgleich','utf-8');
INSERT INTO `text` VALUES (472,'Auto white balance','utf-8');
INSERT INTO `text` VALUES (473,'Manual white balance','utf-8');
INSERT INTO `text` VALUES (474,'White point chromaticity','utf-8');
INSERT INTO `text` VALUES (475,'Horizontale Auflösung','utf-8');
INSERT INTO `text` VALUES (476,'$1 dpc','utf-8');
INSERT INTO `text` VALUES (477,'$1 dpi','utf-8');
INSERT INTO `text` VALUES (478,'YCbCr-Koeffizienten','utf-8');
INSERT INTO `text` VALUES (479,'Y and C positioning','utf-8');
INSERT INTO `text` VALUES (480,'Subsampling ratio of Y to C','utf-8');
INSERT INTO `text` VALUES (481,'Vertikale Auflösung','utf-8');
INSERT INTO `text` VALUES (482,'Jemand anders hat diesen Artikel geändert, nachdem Sie angefangen haben, ihn zu bearbeiten.\nDas obere Textfeld enthält den aktuellen Artikel.\nDas untere Textfeld enthält Ihre Änderungen.\nBitte fügen Sie Ihre Änderungen in das obere Textfeld ein.\n<b>Nur</b> der Inhalt des oberen Textfeldes wird gespeichert, wenn Sie auf \"Speichern\" klicken!<br />','utf-8');
INSERT INTO `text` VALUES (483,'Seiten exportieren','utf-8');
INSERT INTO `text` VALUES (484,'Nur die aktuelle Version der Seite exportieren','utf-8');
INSERT INTO `text` VALUES (485,'Sie können den Text und die Bearbeitungshistorie einer bestimmten oder einer Auswahl von Seiten nach XML exportieren. Das Ergebnis kann in ein anderes Wiki mit WikiMedia Software eingespielt werden, bearbeitet oder archiviert werden.','utf-8');
INSERT INTO `text` VALUES (486,'There was either an external authentication database error or you are not allowed to update your external account.','utf-8');
INSERT INTO `text` VALUES (487,'http://www.beispiel.de Link-Text','utf-8');
INSERT INTO `text` VALUES (488,'Externer Link (http:// beachten)','utf-8');
INSERT INTO `text` VALUES (489,'FAQ','utf-8');
INSERT INTO `text` VALUES (490,'{{SITENAME}}:Häufig_gestellte_Fragen','utf-8');
INSERT INTO `text` VALUES (491,'Feb','utf-8');
INSERT INTO `text` VALUES (492,'Februar','utf-8');
INSERT INTO `text` VALUES (493,'Feed:','utf-8');
INSERT INTO `text` VALUES (494,'Konnte Datei \"$1\" nicht nach \"$2\" kopieren.','utf-8');
INSERT INTO `text` VALUES (495,'Konnte Datei \"$1\" nicht löschen.','utf-8');
INSERT INTO `text` VALUES (496,'Beschreibung','utf-8');
INSERT INTO `text` VALUES (497,'A file with this name exists already, please check $1 if you are not sure if you want to change it.','utf-8');
INSERT INTO `text` VALUES (498,'$1KB, [http://de.wikipedia.org/wiki/Multipurpose_Internet_Mail_Extensions $2]','utf-8');
INSERT INTO `text` VALUES (499,'File missing','utf-8');
INSERT INTO `text` VALUES (500,'Dateiname','utf-8');
INSERT INTO `text` VALUES (501,'Konnte Datei \"$1\" nicht finden.','utf-8');
INSERT INTO `text` VALUES (502,'Konnte Datei \"$1\" nicht nach \"$2\" umbenennen.','utf-8');
INSERT INTO `text` VALUES (503,'Files','utf-8');
INSERT INTO `text` VALUES (504,'Quelle','utf-8');
INSERT INTO `text` VALUES (505,'Copyright-Status','utf-8');
INSERT INTO `text` VALUES (506,'Die Datei \"$1\" wurde erfolgreich hochgeladen.\nBitte verwenden Sie diesen ($2) Link zur Beschreibungsseite und füllen Sie die Informationen über die Datei\n aus, insbesondere seine Herkunft, von wem und wann es\n gemacht wurde und besondere Angaben zum Copyright, falls notwendig.\n Falls es sich um ein Bild handelte, so können Sie mit\n <tt><nowiki>[[Image:$1|thumb|Description]]</nowiki></tt> ein Vorschaubild\n auf der Seite erzeugen lassen.','utf-8');
INSERT INTO `text` VALUES (507,'Fehler: Konnte Formular nicht verarbeiten','utf-8');
INSERT INTO `text` VALUES (508,'Freitag','utf-8');
INSERT INTO `text` VALUES (509,'GEO coordinates','utf-8');
INSERT INTO `text` VALUES (510,'Lade Bilderliste','utf-8');
INSERT INTO `text` VALUES (511,'Los','utf-8');
INSERT INTO `text` VALUES (512,'\n<form method=\"get\" action=\"http://www.google.com/search\" id=\"googlesearch\">\n    <input type=\"hidden\" name=\"domains\" value=\"{{SERVER}}\" />\n    <input type=\"hidden\" name=\"num\" value=\"50\" />\n    <input type=\"hidden\" name=\"ie\" value=\"$2\" />\n    <input type=\"hidden\" name=\"oe\" value=\"$2\" />\n\n    <input type=\"text\" name=\"q\" size=\"31\" maxlength=\"255\" value=\"$1\" />\n    <input type=\"submit\" name=\"btnG\" value=\"$3\" />\n  <div>\n    <input type=\"radio\" name=\"sitesearch\" id=\"gwiki\" value=\"{{SERVER}}\" checked=\"checked\" /><label for=\"gwiki\">{{SITENAME}}</label>\n    <input type=\"radio\" name=\"sitesearch\" id=\"gWWW\" value=\"\" /><label for=\"gWWW\">WWW</label>\n  </div>\n</form>','utf-8');
INSERT INTO `text` VALUES (513,'Trusted users able to block users and delete articles','utf-8');
INSERT INTO `text` VALUES (514,'Administrator','utf-8');
INSERT INTO `text` VALUES (515,'Anonymous users','utf-8');
INSERT INTO `text` VALUES (516,'Anonymous','utf-8');
INSERT INTO `text` VALUES (517,'The bureaucrat group is able to make sysops','utf-8');
INSERT INTO `text` VALUES (518,'Bureaucrat','utf-8');
INSERT INTO `text` VALUES (519,'General logged in users','utf-8');
INSERT INTO `text` VALUES (520,'User','utf-8');
INSERT INTO `text` VALUES (521,'Full access','utf-8');
INSERT INTO `text` VALUES (522,'Steward','utf-8');
INSERT INTO `text` VALUES (523,'grouprights level','utf-8');
INSERT INTO `text` VALUES (524,'User groups','utf-8');
INSERT INTO `text` VALUES (525,'Add group','utf-8');
INSERT INTO `text` VALUES (526,'A group of that name already exists','utf-8');
INSERT INTO `text` VALUES (527,'Edit group','utf-8');
INSERT INTO `text` VALUES (528,'Group description (max 255 characters):<br />','utf-8');
INSERT INTO `text` VALUES (529,'Group name:','utf-8');
INSERT INTO `text` VALUES (530,'If the name or description starts with a colon, the\nremainder will be treated as a message name, and hence the text will be localised\nusing the MediaWiki namespace','utf-8');
INSERT INTO `text` VALUES (531,'Existing groups','utf-8');
INSERT INTO `text` VALUES (532,'Existing groups:','utf-8');
INSERT INTO `text` VALUES (533,'Manage group rights','utf-8');
INSERT INTO `text` VALUES (534,'Please specify a valid group name','utf-8');
INSERT INTO `text` VALUES (535,'ID || Name || Description || Rights','utf-8');
INSERT INTO `text` VALUES (536,'Einfügen aus dem Browser','utf-8');
INSERT INTO `text` VALUES (537,'Ebene 2 Überschrift','utf-8');
INSERT INTO `text` VALUES (538,'Ebene 2 Überschrift','utf-8');
INSERT INTO `text` VALUES (539,'Hilfe','utf-8');
INSERT INTO `text` VALUES (540,'Project:Hilfe','utf-8');
INSERT INTO `text` VALUES (541,'Ausblenden','utf-8');
INSERT INTO `text` VALUES (542,'Verbergen','utf-8');
INSERT INTO `text` VALUES (543,'Versionen','utf-8');
INSERT INTO `text` VALUES (544,'Earliest','utf-8');
INSERT INTO `text` VALUES (545,'Latest','utf-8');
INSERT INTO `text` VALUES (546,'Diff Auswahl: Die Boxen der gewünschten\nVersionen markieren und \'Enter\' drücken oder den Button unten klicken/alt-v.<br />\nLegende:\n(Aktuell) = Unterschied zur aktuellen Version,\n(Letzte) = Unterschied zur vorherigen Version,\nM = Kleine Änderung','utf-8');
INSERT INTO `text` VALUES (547,'Versionen','utf-8');
INSERT INTO `text` VALUES (548,'-','utf-8');
INSERT INTO `text` VALUES (549,'Versionen','utf-8');
INSERT INTO `text` VALUES (550,'WARNUNG: Die Seite die Sie zu löschen gedenken hat\neine Versionsgeschichte:','utf-8');
INSERT INTO `text` VALUES (551,'Horizontale Linie (sparsam verwenden)','utf-8');
INSERT INTO `text` VALUES (552,'Warnung ignorieren und Datei trotzdem speichern.','utf-8');
INSERT INTO `text` VALUES (553,'The filename \"$1\" contains characters that are not allowed in page titles. Please rename the file and try uploading it again.','utf-8');
INSERT INTO `text` VALUES (554,'Suche','utf-8');
INSERT INTO `text` VALUES (555,'Beispiel.jpg','utf-8');
INSERT INTO `text` VALUES (556,'Bild-Verweis','utf-8');
INSERT INTO `text` VALUES (557,'Bildverweise','utf-8');
INSERT INTO `text` VALUES (558,'Bilderliste','utf-8');
INSERT INTO `text` VALUES (559,'all','utf-8');
INSERT INTO `text` VALUES (560,'Hier ist eine Liste von $1 Bildern, sortiert $2.','utf-8');
INSERT INTO `text` VALUES (561,'Limit images on image description pages to:','utf-8');
INSERT INTO `text` VALUES (562,'Bildseite','utf-8');
INSERT INTO `text` VALUES (563,'Auf eine alte Version zurückgesetzt.','utf-8');
INSERT INTO `text` VALUES (564,'Löschen','utf-8');
INSERT INTO `text` VALUES (565,'Beschreibung','utf-8');
INSERT INTO `text` VALUES (566,'Legende: (cur) = Dies ist das aktuelle Bild, (Löschen) = lösche\ndiese alte Version, (Zurücksetzen) = verwende wieder diese alte Version.','utf-8');
INSERT INTO `text` VALUES (567,'Bild-Versionen','utf-8');
INSERT INTO `text` VALUES (568,'Legende: (Beschreibung) = Zeige/Bearbeite Bildbeschreibung.','utf-8');
INSERT INTO `text` VALUES (569,'Destination title is of a special type; cannot move pages into that namespace.','utf-8');
INSERT INTO `text` VALUES (570,'Seiten importieren','utf-8');
INSERT INTO `text` VALUES (571,'Import fehlgeschlagen: $1','utf-8');
INSERT INTO `text` VALUES (572,'Es existieren bereits ältere Versionen, die mit diesen kollidieren. (Möglicherweise wurde die Seite bereits vorher importiert)','utf-8');
INSERT INTO `text` VALUES (573,'Transwiki import','utf-8');
INSERT INTO `text` VALUES (574,'No transwiki import sources have been defined and direct history uploads are disabled.','utf-8');
INSERT INTO `text` VALUES (575,'Leer oder kein Text','utf-8');
INSERT INTO `text` VALUES (576,'Import erfolgreich!','utf-8');
INSERT INTO `text` VALUES (577,'Bitte exportieren Sie die Seite vom Quellwiki mittels Spezial:Export und laden Sie die Datei dann über diese Seite wieder hoch.','utf-8');
INSERT INTO `text` VALUES (578,'Information','utf-8');
INSERT INTO `text` VALUES (579,'Klicken Sie einen Button, um einen Beispieltext zu erhalten.','utf-8');
INSERT INTO `text` VALUES (580,'Bitte geben Sie den Text ein, den Sie formatiert haben möchten.\\nEr wird dann zum Kopieren in der Infobox angezeigt.\\nBeispiel:\\n$1\\nwird zu\\n$2','utf-8');
INSERT INTO `text` VALUES (581,'Information for page','utf-8');
INSERT INTO `text` VALUES (582,'Interner Fehler','utf-8');
INSERT INTO `text` VALUES (583,'InterWikiLinks','utf-8');
INSERT INTO `text` VALUES (584,'Die E-Mail-Adresse wurde nicht akzeptiert, da sie ein ungültiges Format aufzuweisen scheint. Bitte geben Sie eine Adresse in einem gültigen Format ein, oder leeren Sie das Feld.','utf-8');
INSERT INTO `text` VALUES (585,'Invert selection','utf-8');
INSERT INTO `text` VALUES (586,'Ungültiger IP-Addressbereich.','utf-8');
INSERT INTO `text` VALUES (587,'IP-Adresse','utf-8');
INSERT INTO `text` VALUES (588,'IP Address or username','utf-8');
INSERT INTO `text` VALUES (589,'Die angegebeben Ablaufzeit ist ungültig.','utf-8');
INSERT INTO `text` VALUES (590,'Ablaufzeit','utf-8');
INSERT INTO `text` VALUES (591,'Liste blockierter IP-Adressen','utf-8');
INSERT INTO `text` VALUES (592,'2 hours:2 hours,1 day:1 day,3 days:3 days,1 week:1 week,2 weeks:2 weeks,1 month:1 month,3 months:3 months,6 months:6 months,1 year:1 year,infinite:infinite','utf-8');
INSERT INTO `text` VALUES (593,'Other time','utf-8');
INSERT INTO `text` VALUES (594,'other','utf-8');
INSERT INTO `text` VALUES (595,'Grund','utf-8');
INSERT INTO `text` VALUES (596,'Adresse blockieren','utf-8');
INSERT INTO `text` VALUES (597,'Diese Adresse freigeben','utf-8');
INSERT INTO `text` VALUES (598,'IP-Adresse \"$1\" wurde freigegeben','utf-8');
INSERT INTO `text` VALUES (599,'ISBN','utf-8');
INSERT INTO `text` VALUES (600,'Weiterleitungs-Seite','utf-8');
INSERT INTO `text` VALUES (601,'Kursiver Text','utf-8');
INSERT INTO `text` VALUES (602,'Kursiver Text','utf-8');
INSERT INTO `text` VALUES (603,'Problem mit dem Eintrag \'$1\', ungültiger Name...','utf-8');
INSERT INTO `text` VALUES (604,'Jan','utf-8');
INSERT INTO `text` VALUES (605,'Januar','utf-8');
INSERT INTO `text` VALUES (606,'Jul','utf-8');
INSERT INTO `text` VALUES (607,'Juli','utf-8');
INSERT INTO `text` VALUES (608,'Jun','utf-8');
INSERT INTO `text` VALUES (609,'Juni','utf-8');
INSERT INTO `text` VALUES (610,'Warning: Page may not contain recent updates.','utf-8');
INSERT INTO `text` VALUES (611,'Bitte keine Bilder über 100 KByte hochladen.','utf-8');
INSERT INTO `text` VALUES (612,'Vorherige','utf-8');
INSERT INTO `text` VALUES (613,'Diese Seite wurde zuletzt geändert um $1.','utf-8');
INSERT INTO `text` VALUES (614,'This page was last modified $1 by $2.','utf-8');
INSERT INTO `text` VALUES (615,'Zeile $1:','utf-8');
INSERT INTO `text` VALUES (616,'Link-Text','utf-8');
INSERT INTO `text` VALUES (617,'Interner Link','utf-8');
INSERT INTO `text` VALUES (618,'(Liste der Verweise)','utf-8');
INSERT INTO `text` VALUES (619,'/^(.*?)([a-zA-Z\\x80-\\xff]+)$/sD','utf-8');
INSERT INTO `text` VALUES (620,'Die folgenden Artikel verweisen hierhin:','utf-8');
INSERT INTO `text` VALUES (621,'Die folgenden Artikel benutzen dieses Bild:','utf-8');
INSERT INTO `text` VALUES (622,'/^([ä|ö|ü|ß|a-z]+)(.*)$/sD','utf-8');
INSERT INTO `text` VALUES (623,'Liste','utf-8');
INSERT INTO `text` VALUES (624,' cont.','utf-8');
INSERT INTO `text` VALUES (625,'Benutzerverzeichnis','utf-8');
INSERT INTO `text` VALUES (626,'Lade Liste mit früheren Versionen','utf-8');
INSERT INTO `text` VALUES (627,'lage Versionen zur Unterscheidung','utf-8');
INSERT INTO `text` VALUES (628,'Ortszeit','utf-8');
INSERT INTO `text` VALUES (629,'Datenbank sperren','utf-8');
INSERT INTO `text` VALUES (630,'Ja, ich möchte die Datenbank sperren.','utf-8');
INSERT INTO `text` VALUES (631,'Datenbank sperren','utf-8');
INSERT INTO `text` VALUES (632,'Datenbank wurde erfolgreich gesperrt','utf-8');
INSERT INTO `text` VALUES (633,'Die {{SITENAME}}-Datenbank wurde gesperrt.\n<br />Bitte geben Sie die Datenbank wieder frei, sobald die Wartung abgeschlossen ist.','utf-8');
INSERT INTO `text` VALUES (634,'Mit dem Sperren der Datenbank werden alle Änderungen an Benutzereinstellungen, watchlisten, Artikeln usw. verhindert. Bitte bestätigen Sie Ihre Absicht, die Datenbank zu sperren.','utf-8');
INSERT INTO `text` VALUES (635,'Sie haben das Bestätigungsfeld nicht markiert.','utf-8');
INSERT INTO `text` VALUES (636,'Logs','utf-8');
INSERT INTO `text` VALUES (637,'Anmelden','utf-8');
INSERT INTO `text` VALUES (638,'&nbsp;','utf-8');
INSERT INTO `text` VALUES (639,'Fehler bei der Anmeldung','utf-8');
INSERT INTO `text` VALUES (640,'Benutzer-Anmeldung','utf-8');
INSERT INTO `text` VALUES (641,'<b>Es gab ein Problem mit Ihrer Anmeldung.</b><br />Bitte versuchen Sie es nochmal!','utf-8');
INSERT INTO `text` VALUES (642,'Um sich bei {{SITENAME}} anmelden zu können, müssen Cookies aktiviert sein.','utf-8');
INSERT INTO `text` VALUES (643,'Sie müssen sich [[Spezial:Userlogin|anmelden]], um andere Seiten betrachten zu können.','utf-8');
INSERT INTO `text` VALUES (644,'Anmeldung erforderlich','utf-8');
INSERT INTO `text` VALUES (645,'Sie sind jetzt als \"$1\" bei {{SITENAME}} angemeldet.','utf-8');
INSERT INTO `text` VALUES (646,'Anmeldung erfolgreich','utf-8');
INSERT INTO `text` VALUES (647,'Abmelden','utf-8');
INSERT INTO `text` VALUES (648,'Sie sind nun abgemeldet.\nSie können {{SITENAME}} jetzt anonym weiterbenutzen, oder sich unter dem selben oder einem anderen Benutzernamen wieder anmelden.','utf-8');
INSERT INTO `text` VALUES (649,'Benutzer-Abmeldung','utf-8');
INSERT INTO `text` VALUES (650,'Verwaiste Seiten','utf-8');
INSERT INTO `text` VALUES (651,'Lange Artikel','utf-8');
INSERT INTO `text` VALUES (652,'<strong>WARNUNG: Diese Seite ist $1KB groß; einige Browser könnten Probleme haben, Seiten zu bearbeiten, die größer als 32KB sind.\nÜberlegen Sie bitte, ob eine Aufteilung der Seite in kleinere Abschnitte möglich ist.</strong>','utf-8');
INSERT INTO `text` VALUES (653,'Fehler beim Senden von Mail: $1','utf-8');
INSERT INTO `text` VALUES (654,'Ein neues (temporäres) Passwort schicken','utf-8');
INSERT INTO `text` VALUES (655,'Sie sind nicht angemeldet.','utf-8');
INSERT INTO `text` VALUES (656,'Sie müssen [[Spezial:Userlogin|angemeldet sein]]\nund eine gültige E-Mail-Adresse haben, um anderen Benutzern E-Mail zu schicken.','utf-8');
INSERT INTO `text` VALUES (657,'Hauptseite','utf-8');
INSERT INTO `text` VALUES (658,'Siehe die [http://meta.wikipedia.org/wiki/MediaWiki_i18n Dokumentation zur Anpassung der Benutzeroberfläche]\nund das [http://meta.wikipedia.org/wiki/MediaWiki_User%27s_Guide Benutzerhandbuch] für Hilfe zur Benutzung und Konfiguration.','utf-8');
INSERT INTO `text` VALUES (659,'Die Wiki Software wurde erfolgreich installiert.','utf-8');
INSERT INTO `text` VALUES (660,'Wartungsseite','utf-8');
INSERT INTO `text` VALUES (661,'Zurück zur Wartungsseite','utf-8');
INSERT INTO `text` VALUES (662,'Diese Seite enthält mehrere praktische Funktionen zur täglichen Wartung von {{SITENAME}}. Einige dieser Funktionen können die Datenbank stark beanspruchen, also bitte nicht nach jeder Änderung neu laden ;-)','utf-8');
INSERT INTO `text` VALUES (663,'Mache einen Benutzer zum Administrator','utf-8');
INSERT INTO `text` VALUES (664,'<b>Benutzer \"$1\" konnte nicht zu einem Administrator gemacht werden. (Haben Sie den Namen richtig geschrieben?)</b>','utf-8');
INSERT INTO `text` VALUES (665,'Name des Benutzers:','utf-8');
INSERT INTO `text` VALUES (666,'<b>Benutzer \"$1\" ist nun ein Administrator.</b>','utf-8');
INSERT INTO `text` VALUES (667,'Mache diesen Benutzer zu einem Administrator','utf-8');
INSERT INTO `text` VALUES (668,'Diese Maske wird von Bürokraten benutzt, um normale Benutzer zu Administratoren zu machen.','utf-8');
INSERT INTO `text` VALUES (669,'Mache einen Benutzer zum Administrator','utf-8');
INSERT INTO `text` VALUES (670,'Mär','utf-8');
INSERT INTO `text` VALUES (671,'März','utf-8');
INSERT INTO `text` VALUES (672,'Mark as patrolled','utf-8');
INSERT INTO `text` VALUES (673,'[$1]','utf-8');
INSERT INTO `text` VALUES (674,'Mark this article as patrolled','utf-8');
INSERT INTO `text` VALUES (675,'Marked as patrolled','utf-8');
INSERT INTO `text` VALUES (676,'The selected revision has been marked as patrolled.','utf-8');
INSERT INTO `text` VALUES (677,'Die Anfrage \"$1\" stimmt mit $2 Artikelüberschriften\nund dem Text von $3 Artikeln überein.','utf-8');
INSERT INTO `text` VALUES (678,'TeX','utf-8');
INSERT INTO `text` VALUES (679,'Kann das Zielverzeichnis für mathematische Formeln nicht anlegen oder beschreiben.','utf-8');
INSERT INTO `text` VALUES (680,'Kann das Temporärverzeichnis für mathematische Formeln nicht anlegen oder beschreiben.','utf-8');
INSERT INTO `text` VALUES (681,'Parser-Fehler','utf-8');
INSERT INTO `text` VALUES (682,'die PNG-Konvertierung schlug fehl.','utf-8');
INSERT INTO `text` VALUES (683,'\'Lexing\'-Fehler','utf-8');
INSERT INTO `text` VALUES (684,'Das texvc-Programm kann nicht gefunden werden. Bitte beachten Sie math/README.','utf-8');
INSERT INTO `text` VALUES (685,'Formel hier einfügen','utf-8');
INSERT INTO `text` VALUES (686,'Syntaxfehler','utf-8');
INSERT INTO `text` VALUES (687,'Mathematische Formel (LaTeX)','utf-8');
INSERT INTO `text` VALUES (688,'Unbekannter Fehler','utf-8');
INSERT INTO `text` VALUES (689,'Unbekannte Funktion','utf-8');
INSERT INTO `text` VALUES (690,'Mai','utf-8');
INSERT INTO `text` VALUES (691,'Mai','utf-8');
INSERT INTO `text` VALUES (692,'Beispiel.mp3','utf-8');
INSERT INTO `text` VALUES (693,'Mediendatei-Verweis','utf-8');
INSERT INTO `text` VALUES (694,'\n===Warnung!===\nDiese Art von Datei kann böswilligen Programmcode enthalten.\nDurch das Herunterladen oder Öffnen der Datei kann der Computer beschädigt werden.\nBereits das Anklicken des Links kann dazu führen dass der Browser die Datei öffnet\nund unbekannter Programmcode zur Ausführung kommt.\n\nDie Betreiber dieses Wikis können keine Verantwortung für den Inhalte\ndieser Datei übernehmen. Sollte diese Datei tatsächlich böswilligen Programmcode enthalten,\nsollte umgehend ein Administrator informiert werden!','utf-8');
INSERT INTO `text` VALUES (695,'Metadata','utf-8');
INSERT INTO `text` VALUES (696,'Wikipedia:Metadata','utf-8');
INSERT INTO `text` VALUES (697,'Bilddateien müssen mindestens drei Buchstaben haben.','utf-8');
INSERT INTO `text` VALUES (698,'Nur Kleinigkeiten wurden verändert.','utf-8');
INSERT INTO `text` VALUES (699,'M','utf-8');
INSERT INTO `text` VALUES (700,'Seiten mit falsch geschriebenen Worten','utf-8');
INSERT INTO `text` VALUES (701,'Liste von Tippfehlern','utf-8');
INSERT INTO `text` VALUES (702,'Die folgenden Seiten enthalten falsch geschriebene Worte, wie sie auf $1 definiert sind. In Klammern angegebene Worte geben die korrekte Schreibweise wieder.<p><strong>Zitate, Buchtitel u.ä. bitte im Originalzustand belassen, also ggf. in alter Rechtschreibung und mit Rechtschreibfehlern!</strong>','utf-8');
INSERT INTO `text` VALUES (703,'Der Text für den Artikel \"$1\" wurde nicht in der Datenbank gefunden. Das ist wahrscheinlich ein Fehler in der Software. Bitte melden Sie dies einem Administrator, und geben sie den Artikelnamen an.','utf-8');
INSERT INTO `text` VALUES (704,'<b>Fehlendes Bild</b><br /><i>$1</i>','utf-8');
INSERT INTO `text` VALUES (705,'Fehlende Sprachverweise','utf-8');
INSERT INTO `text` VALUES (706,'Zeige fehlende Sprachverweise nach','utf-8');
INSERT INTO `text` VALUES (707,'Diese Artikel haben <i>keinen</i> Verweis zu ihrem Gegenstück in $1. Redirects und Unterseiten werden <i>nicht</i> angezeigt.','utf-8');
INSERT INTO `text` VALUES (708,'Montag','utf-8');
INSERT INTO `text` VALUES (709,'Mehr...','utf-8');
INSERT INTO `text` VALUES (710,'verschieben','utf-8');
INSERT INTO `text` VALUES (711,'Artikel verschieben','utf-8');
INSERT INTO `text` VALUES (712,'verschoben nach','utf-8');
INSERT INTO `text` VALUES (713,'Move log','utf-8');
INSERT INTO `text` VALUES (714,'Below is a list of page moved.','utf-8');
INSERT INTO `text` VALUES (715,'Sie sind nicht angemeldet','utf-8');
INSERT INTO `text` VALUES (716,'Sie müssen ein registrierter Benutzer und\n[[Special:Userlogin|angemeldet]] sein,\num eine Seite zu verschieben.','utf-8');
INSERT INTO `text` VALUES (717,'Artikel verschieben','utf-8');
INSERT INTO `text` VALUES (718,'Artikel verschieben','utf-8');
INSERT INTO `text` VALUES (719,'Die dazugehörige Diskussionsseite wird, sofern vorhanden, mitverschoben, \'\'\'es sei denn:\'\'\'\n*Sie verschieben die Seite in einen anderen Namensraum, oder\n*Es existiert bereits eine Diskussionsseite mit diesem Namen, oder\n*Sie wählen die untenstehende Option ab\n\nIn diesen Fällen müssen Sie die Seite, falls gewünscht, von Hand verschieben.','utf-8');
INSERT INTO `text` VALUES (720,'Mit diesem Formular können Sie einen Artikel umbenennen, mitsamt allen Versionen. Der alte Titel wird zum neuen weiterleiten. Verweise auf den alten Titel werden nicht geändert, und die Diskussionsseite wird auch nicht mitverschoben.','utf-8');
INSERT INTO `text` VALUES (721,'Reason','utf-8');
INSERT INTO `text` VALUES (722,'Die \"Diskussions\"-Seite mitverschieben, wenn möglich.','utf-8');
INSERT INTO `text` VALUES (723,'Artikel verschieben','utf-8');
INSERT INTO `text` VALUES (724,'Wenn möglich als HTML darstellen, sonst PNG','utf-8');
INSERT INTO `text` VALUES (725,'MathML (experimentell)','utf-8');
INSERT INTO `text` VALUES (726,'Empfehlenswert für moderne Browser','utf-8');
INSERT INTO `text` VALUES (727,'Immer als PNG darstellen','utf-8');
INSERT INTO `text` VALUES (728,'Einfaches TeX als HTML darstellen, sonst PNG','utf-8');
INSERT INTO `text` VALUES (729,'Als TeX belassen (für Textbrowser)','utf-8');
INSERT INTO `text` VALUES (730,'Meine Beiträge','utf-8');
INSERT INTO `text` VALUES (731,'Meine Seite','utf-8');
INSERT INTO `text` VALUES (732,'Meine Diskussion','utf-8');
INSERT INTO `text` VALUES (733,'Namespace:','utf-8');
INSERT INTO `text` VALUES (734,'Navigation','utf-8');
INSERT INTO `text` VALUES (735,'$1 Byte','utf-8');
INSERT INTO `text` VALUES (736,'$1 Änderungen','utf-8');
INSERT INTO `text` VALUES (737,'(Neu)','utf-8');
INSERT INTO `text` VALUES (738,'Hier den Text des neuen Artikels eintragen.\nBitte nur in ganzen Sätzen schreiben und keine urheberrechtsgeschützten Texte anderer kopieren.','utf-8');
INSERT INTO `text` VALUES (739,'newbies','utf-8');
INSERT INTO `text` VALUES (740,'Gallery of new files','utf-8');
INSERT INTO `text` VALUES (741,'Sie haben $1.','utf-8');
INSERT INTO `text` VALUES (742,'neue Nachrichten','utf-8');
INSERT INTO `text` VALUES (743,'Neue Seite','utf-8');
INSERT INTO `text` VALUES (744,'N','utf-8');
INSERT INTO `text` VALUES (745,'Neue Artikel','utf-8');
INSERT INTO `text` VALUES (746,'Neues Passwort','utf-8');
INSERT INTO `text` VALUES (747,'Zu neuem Titel','utf-8');
INSERT INTO `text` VALUES (748,' (nur für neue Mitglieder)','utf-8');
INSERT INTO `text` VALUES (749,'(opens in new window)','utf-8');
INSERT INTO `text` VALUES (750,'Nächste','utf-8');
INSERT INTO `text` VALUES (751,'Next diff &rarr;','utf-8');
INSERT INTO `text` VALUES (752,'nächste $1','utf-8');
INSERT INTO `text` VALUES (753,'Next page ($1)','utf-8');
INSERT INTO `text` VALUES (754,'&larr;Nächstjüngere Version','utf-8');
INSERT INTO `text` VALUES (755,'$1 Verweise','utf-8');
INSERT INTO `text` VALUES (756,'(Dieser Artikel enthält momentan keinen Text)','utf-8');
INSERT INTO `text` VALUES (757,'Konnte keine Verbindung zur Datenbank auf $1 herstellen','utf-8');
INSERT INTO `text` VALUES (758,'Es wurden keine Änderungen für diese Kriterien gefunden.','utf-8');
INSERT INTO `text` VALUES (759,'{{SITENAME}} benutzt Cookies zum Einloggen der Benutzer. Sie haben Cookies deaktiviert, bitte aktivieren Sie diese und versuchen es erneut.','utf-8');
INSERT INTO `text` VALUES (760,'Der Benutzerzugang wurde erstellt, aber Sie sind nicht eingeloggt. {{SITENAME}} benötigt für diese Funktion Cookies, bitte aktivieren Sie diese und loggen sich dann mit Ihrem neuen Benutzernamen und dem Passwort ein.','utf-8');
INSERT INTO `text` VALUES (761,'Creative Commons RDF metadata disabled for this server.','utf-8');
INSERT INTO `text` VALUES (762,'There is no credits info available for this page.','utf-8');
INSERT INTO `text` VALUES (763,'Konnte Datenbank $1 nicht auswählen','utf-8');
INSERT INTO `text` VALUES (764,'Dublin Core RDF metadata disabled for this server.','utf-8');
INSERT INTO `text` VALUES (765,'Benutzer \"$1\" hat keine E-Mail-Adresse angegeben.','utf-8');
INSERT INTO `text` VALUES (766,'<strong>Sie haben keine E-Mail-Adresse angegeben</strong>, die folgenden\nFunktionen sind zur Zeit deshalb nicht möglich.','utf-8');
INSERT INTO `text` VALUES (767,'Dieser Benutzer hat keine gültige E-Mail-Adresse angegeben, oder möchte keine E-Mail von anderen Benutzern empfangen.','utf-8');
INSERT INTO `text` VALUES (768,'Keine E-Mail-Adresse','utf-8');
INSERT INTO `text` VALUES (769,'Es existiert kein Artikel [[$1|$1]]. Bitte versuchen\nSie die Volltextsuche oder [[$1|legen Sie den Artikel neu an]].','utf-8');
INSERT INTO `text` VALUES (770,'Es gibt keine früheren Versionen von diesem Artikel.','utf-8');
INSERT INTO `text` VALUES (771,'No file by this name exists, you can [$1 upload it]','utf-8');
INSERT INTO `text` VALUES (772,'Nothing to see.','utf-8');
INSERT INTO `text` VALUES (773,'Kein Artikel verweist hierhin.','utf-8');
INSERT INTO `text` VALUES (774,'Kein Artikel benutzt dieses Bild.','utf-8');
INSERT INTO `text` VALUES (775,'Sie müssen einen Benutzernamen angeben.','utf-8');
INSERT INTO `text` VALUES (776,'<strong>Hinweis</strong>:\nErfolglose Suchanfragen werden häufig verursacht durch den Versuch, nach \'gewöhnlichen\' Worten zu suchen; diese sind nicht indiziert.','utf-8');
INSERT INTO `text` VALUES (777,'<strong>WARNING: Your browser is not unicode compliant. A workaround is in place to allow you to safely edit articles: non-ASCII characters will appear in the edit box as hexadecimal codes.</strong>','utf-8');
INSERT INTO `text` VALUES (778,'Diese Spezialseite wird von der MediaWiki-Software nicht unterstützt','utf-8');
INSERT INTO `text` VALUES (779,'Diese Aktion gibt es nicht','utf-8');
INSERT INTO `text` VALUES (780,'Diese Aktion wird von der MediaWiki-Software nicht unterstützt','utf-8');
INSERT INTO `text` VALUES (781,'Diese Spezialseite gibt es nicht','utf-8');
INSERT INTO `text` VALUES (782,'Der Benutzername \"$1\" existiert nicht.\nÜberprüfen Sie die Schreibweise, oder melden Sie sich als neuer Benutzer an.','utf-8');
INSERT INTO `text` VALUES (783,'There is no user by the name \"$1\". Check your spelling.','utf-8');
INSERT INTO `text` VALUES (784,'The wiki server can\'t provide data in a format your client can read.','utf-8');
INSERT INTO `text` VALUES (785,'Kein Artikel','utf-8');
INSERT INTO `text` VALUES (786,'Sie haben nicht angegeben, auf welche Seite Sie diese Funktion anwenden wollen.','utf-8');
INSERT INTO `text` VALUES (787,'Kein Artikel angegeben','utf-8');
INSERT INTO `text` VALUES (788,'<strong>Hinweis:</strong>','utf-8');
INSERT INTO `text` VALUES (789,'Keine Übereinstimmungen','utf-8');
INSERT INTO `text` VALUES (790,'Keine Übereinstimmungen','utf-8');
INSERT INTO `text` VALUES (791,'Nicht angemeldet','utf-8');
INSERT INTO `text` VALUES (792,'Nov','utf-8');
INSERT INTO `text` VALUES (793,'November','utf-8');
INSERT INTO `text` VALUES (794,'Sie haben keine Einträge auf Ihrer Beobachtungsliste.','utf-8');
INSERT INTO `text` VALUES (795,'Unformatierten Text hier einfügen','utf-8');
INSERT INTO `text` VALUES (796,'Unformatierter Text','utf-8');
INSERT INTO `text` VALUES (797,'Kategorie','utf-8');
INSERT INTO `text` VALUES (798,'Hilfe','utf-8');
INSERT INTO `text` VALUES (799,'Bild','utf-8');
INSERT INTO `text` VALUES (800,'Artikel','utf-8');
INSERT INTO `text` VALUES (801,'Media','utf-8');
INSERT INTO `text` VALUES (802,'Message','utf-8');
INSERT INTO `text` VALUES (803,'Spezial','utf-8');
INSERT INTO `text` VALUES (804,'Template','utf-8');
INSERT INTO `text` VALUES (805,'Benutzerseite','utf-8');
INSERT INTO `text` VALUES (806,'Project page','utf-8');
INSERT INTO `text` VALUES (807,'Number of distinct authors (article): $1','utf-8');
INSERT INTO `text` VALUES (808,'[$1]','utf-8');
INSERT INTO `text` VALUES (809,'[$1 Benutzer beobachten diese Seite]','utf-8');
INSERT INTO `text` VALUES (810,'Number of edits (article): $1','utf-8');
INSERT INTO `text` VALUES (811,'Number of distinct authors (discussion page): $1','utf-8');
INSERT INTO `text` VALUES (812,'Number of edits (discussion page): $1','utf-8');
INSERT INTO `text` VALUES (813,'Number of watchers: $1','utf-8');
INSERT INTO `text` VALUES (814,'$1 Abfragen','utf-8');
INSERT INTO `text` VALUES (815,'Okt','utf-8');
INSERT INTO `text` VALUES (816,'Oktober','utf-8');
INSERT INTO `text` VALUES (817,'OK','utf-8');
INSERT INTO `text` VALUES (818,'Altes Passwort','utf-8');
INSERT INTO `text` VALUES (819,'Original','utf-8');
INSERT INTO `text` VALUES (820,'Verwaiste Seiten','utf-8');
INSERT INTO `text` VALUES (821,'Based on work by $1.','utf-8');
INSERT INTO `text` VALUES (822,'Andere Sprachen','utf-8');
INSERT INTO `text` VALUES (823,'others','utf-8');
INSERT INTO `text` VALUES (824,'Verschiebung erfolgreich','utf-8');
INSERT INTO `text` VALUES (825,'Artikel \"[[$1]]\" wurde nach \"[[$2]]\" verschoben.','utf-8');
INSERT INTO `text` VALUES (826,'$1 - {{SITENAME}}','utf-8');
INSERT INTO `text` VALUES (827,'Someone (probably you, from IP address $1)\nrequested that we send you a new {{SITENAME}} login password.\nThe password for user \"$2\" is now \"$3\".\nYou should log in and change your password now.','utf-8');
INSERT INTO `text` VALUES (828,'Password reminder from {{SITENAME}}','utf-8');
INSERT INTO `text` VALUES (829,'Ein temporäres Passwort wurde an die E-Mail-Adresse von Benutzer \"$1\" gesendet.\nBitte melden Sie sich damit an, sobald Sie es erhalten.','utf-8');
INSERT INTO `text` VALUES (830,'Ihr Passwort ist zu kurz. Es muss mindestens $1 Zeichen lang sein.','utf-8');
INSERT INTO `text` VALUES (831,'Die folgenden Daten stammen aus dem Cache und sind möglicherweise nicht aktuell:','utf-8');
INSERT INTO `text` VALUES (832,'Diese Funktion wurde wegen Überlastung des Servers vorübergehend deaktiviert. Versuchen Sie es bitte zwischen 02:00 und 14:00 UTC noch einmal<br />(Aktuelle Serverzeit : 12:41:18 UTC).','utf-8');
INSERT INTO `text` VALUES (833,'Hier ist eine gespeicherte Kopie von $1:','utf-8');
INSERT INTO `text` VALUES (834,'\'Persönliche Werkzeuge','utf-8');
INSERT INTO `text` VALUES (835,'Beliebte Seiten','utf-8');
INSERT INTO `text` VALUES (836,'{{SITENAME}}-Portal','utf-8');
INSERT INTO `text` VALUES (837,'{{ns:4}}:Portal','utf-8');
INSERT INTO `text` VALUES (838,'Kommentar hinzufügen','utf-8');
INSERT INTO `text` VALUES (839,'{{SITENAME}} benutzt [http://www.mediawiki.org/ MediaWiki], eine Open Source Wiki-Engine.','utf-8');
INSERT INTO `text` VALUES (840,'Suche','utf-8');
INSERT INTO `text` VALUES (841,'\nSuche in Namensräumen :<br />\n$1<br />\n$2 Zeige auch REDIRECTs &nbsp; Suche nach $3 $9','utf-8');
INSERT INTO `text` VALUES (842,'Einstellungen','utf-8');
INSERT INTO `text` VALUES (843,'** <strong>E-Mail-Adresse</strong> (optional): Erlaubt andern, Sie über Ihre Benutzerseiten zu kontaktieren, ohne dass Sie Ihre E-Mail-Adresse offenbaren müssen.\nFür den Fall, dass Sie Ihr Passwort vergessen haben, kann Ihnen so ein temporäres Einmal-Passwort gesendet werden.','utf-8');
INSERT INTO `text` VALUES (844,'An diese Adresse werden auch die Benachrichtigungsmails geschickt, sofern Sie das eingeschaltet haben.','utf-8');
INSERT INTO `text` VALUES (845,'* <strong>Echter Name</strong> (optional): für anerkennende Nennungen Ihres Namens im Zusammenhang mit Ihren Beiträgen.','utf-8');
INSERT INTO `text` VALUES (846,'Verschiedene Einstellungen','utf-8');
INSERT INTO `text` VALUES (847,'Benutzerdaten','utf-8');
INSERT INTO `text` VALUES (848,'Letzte Änderungen und Anzeige kurzer Artikel','utf-8');
INSERT INTO `text` VALUES (849,'Sie sind angemeldet als \"$1\".\nIhre interne ID-Nummer ist $2.','utf-8');
INSERT INTO `text` VALUES (850,'Nicht angemeldet','utf-8');
INSERT INTO `text` VALUES (851,'Sie müssen [[Special:Userlogin|angemeldet]] sein, um Ihre Einstellungen zu ändern.','utf-8');
INSERT INTO `text` VALUES (852,'Einstellungen wurden auf Standard zurückgesetzt.','utf-8');
INSERT INTO `text` VALUES (853,'Vorschau','utf-8');
INSERT INTO `text` VALUES (854,'Diese Vorschau gibt den Inhalt des oberen Textfeldes wieder; so wird der Artikel aussehen, wenn Sie jetzt speichern.','utf-8');
INSERT INTO `text` VALUES (855,'Dies ist nur eine Vorschau, der Artikel wurde noch nicht gespeichert!','utf-8');
INSERT INTO `text` VALUES (856,'&larr; Previous diff','utf-8');
INSERT INTO `text` VALUES (857,'Nächstältere Version&rarr;','utf-8');
INSERT INTO `text` VALUES (858,'vorherige $1','utf-8');
INSERT INTO `text` VALUES (859,'Print','utf-8');
INSERT INTO `text` VALUES (860,'Druckversion','utf-8');
INSERT INTO `text` VALUES (861,'(Von {{SERVER}})','utf-8');
INSERT INTO `text` VALUES (862,'Schützen','utf-8');
INSERT INTO `text` VALUES (863,'Grund der Sperrung','utf-8');
INSERT INTO `text` VALUES (864,'Artikel [[$1]] geschützt','utf-8');
INSERT INTO `text` VALUES (865,'Geschützte Seite','utf-8');
INSERT INTO `text` VALUES (866,'<strong>WARNUNG: Diese Seite wurde gesperrt, so dass sie nur\nBenutzer mit Sysop-Rechten bearbeitet werden kann. Beachten Sie bitte die\n[[Project:Geschützte Seiten|Regeln für geschützte Seiten]].</strong>','utf-8');
INSERT INTO `text` VALUES (867,'Diese Seite ist für das Bearbeiten gesperrt. Dafür kann es diverse Gründe geben; siehe [[{{ns:4}}:Geschützte Seiten]].\n\nSie können den Quelltext dieser Seite betrachten und kopieren:','utf-8');
INSERT INTO `text` VALUES (868,'Seitenschutz-Logbuch','utf-8');
INSERT INTO `text` VALUES (869,'Dies ist eine Liste der blockierten Seiten. Siehe [[Project:Geschützte Seiten]] für mehr Informationen.','utf-8');
INSERT INTO `text` VALUES (870,'Protect from moves only','utf-8');
INSERT INTO `text` VALUES (871,'Artikel schützen','utf-8');
INSERT INTO `text` VALUES (872,'(Sperren von \"$1\")','utf-8');
INSERT INTO `text` VALUES (873,'Artikel schützen','utf-8');
INSERT INTO `text` VALUES (874,'Proxyblocker','utf-8');
INSERT INTO `text` VALUES (875,'Ihre IP-Adresse wurde gesperrt, da sie ein offener Proxy ist. Bitte kontaktieren Sie Ihren Provider oder Ihre Systemtechnik und informieren Sie sie über dieses mögliche Sicherheitsproblem.','utf-8');
INSERT INTO `text` VALUES (876,'Fertig.','utf-8');
INSERT INTO `text` VALUES (877,'http://www.ncbi.nlm.nih.gov/entrez/query.fcgi?cmd=Retrieve&db=pubmed&dopt=Abstract&list_uids=$1','utf-8');
INSERT INTO `text` VALUES (878,'Blättern','utf-8');
INSERT INTO `text` VALUES (879,'Ändern','utf-8');
INSERT INTO `text` VALUES (880,'Finden','utf-8');
INSERT INTO `text` VALUES (881,'Einstellungen','utf-8');
INSERT INTO `text` VALUES (882,'Seitendaten','utf-8');
INSERT INTO `text` VALUES (883,'Seitenoptionen','utf-8');
INSERT INTO `text` VALUES (884,'Seitenleiste','utf-8');
INSERT INTO `text` VALUES (885,'Spezialseiten','utf-8');
INSERT INTO `text` VALUES (886,'Zufälliger Artikel','utf-8');
INSERT INTO `text` VALUES (887,'Special:Random','utf-8');
INSERT INTO `text` VALUES (888,'Die Möglichkeit, ganze Adressräume zu sperren, ist nicht aktiviert.','utf-8');
INSERT INTO `text` VALUES (889,'in $4 form; $1 kleine Änderungen; $2 sekundäre Namensräume; $3 mehrfache Änderungen.','utf-8');
INSERT INTO `text` VALUES (890,'Zeige die letzten $1 Änderungen; zeige die letzten $2 Tage.','utf-8');
INSERT INTO `text` VALUES (891,'Zeige neue Änderungen seit $1','utf-8');
INSERT INTO `text` VALUES (892,'$1 Änderungen durch eingeloggte Benutzer','utf-8');
INSERT INTO `text` VALUES (893,'Lade Letzte Änderungen','utf-8');
INSERT INTO `text` VALUES (894,'(auf Artikel von \"$1\")','utf-8');
INSERT INTO `text` VALUES (895,'Hier sind die letzten <b>$1</b> Änderungen der letzten <b>$2</b> Tage. (<b>N</b> - Neuer Artikel; <b>M</b> - kleine Änderung)','utf-8');
INSERT INTO `text` VALUES (896,'Dies sind die Änderungen seit <b>$2</b> (bis zu <b>$1</b> gezeigt).','utf-8');
INSERT INTO `text` VALUES (897,'Recent Changes Patrol disabled','utf-8');
INSERT INTO `text` VALUES (898,'The Recent Changes Patrol feature is currently disabled.','utf-8');
INSERT INTO `text` VALUES (899,'Datenbank ist gesperrt','utf-8');
INSERT INTO `text` VALUES (900,'The database has been automatically locked while the slave database servers catch up to the master','utf-8');
INSERT INTO `text` VALUES (901,'Die {{SITENAME}}-Datenbank ist vorübergehend gesperrt, z.B. für Wartungsarbeiten. Bitte versuchen Sie es später noch einmal.','utf-8');
INSERT INTO `text` VALUES (902,'<strong>WARNUNG: Die Datenbank wurde während dem Ändern der\nSeite für Wartungsarbeiten gesperrt, so dass Sie die Seite im Moment nicht\nspeichern können. Sichern Sie sich den Text und versuchen Sie die Änderungen\nspäter einzuspielen.</strong>','utf-8');
INSERT INTO `text` VALUES (903,'Letzte Änderungen','utf-8');
INSERT INTO `text` VALUES (904,'Special:Recentchanges','utf-8');
INSERT INTO `text` VALUES (905,'all','utf-8');
INSERT INTO `text` VALUES (906,'Anzahl \"Letzte Änderungen\"','utf-8');
INSERT INTO `text` VALUES (907,'Verlinkte Seiten','utf-8');
INSERT INTO `text` VALUES (908,'\nDiese Seite wird beim Laden automatisch aktualisiert. Angezeigt werden Seiten, die zuletzt bearbeitet wurden, sowie die Zeit und der Name des Autors.<br />\nFalls Sie neu bei {{SITENAME}} sind, lesen Sie bitte die [[Project:Willkommen|Willkommensseite]] und [[Project:Erste Schritte|Erste Schritte]].<br />\nWenn Sie möchten, dass {{SITENAME}} zu einem Erfolg wird, dann fügen Sie bitte keine Texte hinzu, die dem [[Project:Urheberrechte beachten|Urheberrecht]] anderer unterliegen. Dies könnte dem Projekt sonst schweren Schaden zufügen.','utf-8');
INSERT INTO `text` VALUES (909,'(Weitergeleitet von $1)','utf-8');
INSERT INTO `text` VALUES (910,'Dauerhaftes Einloggen','utf-8');
INSERT INTO `text` VALUES (911,'Markierte Einträge löschen','utf-8');
INSERT INTO `text` VALUES (912,'Von der Beobachtungsliste entfernt','utf-8');
INSERT INTO `text` VALUES (913,'Der Artikel \"$1\" wurde von Ihrer Beobachtungsliste entfernt.','utf-8');
INSERT INTO `text` VALUES (914,'Wunschgemäß werden die Einträge aus der Beobachtungsliste entfernt...','utf-8');
INSERT INTO `text` VALUES (915,'Renamed group $2 to $3','utf-8');
INSERT INTO `text` VALUES (916,'Einstellungen zurücksetzen','utf-8');
INSERT INTO `text` VALUES (917,'$1 gelöschte Bearbeitungsvorgänge','utf-8');
INSERT INTO `text` VALUES (918,'Restricted special pages','utf-8');
INSERT INTO `text` VALUES (919,'Treffer pro Seite','utf-8');
INSERT INTO `text` VALUES (920,'Von \"$1\"','utf-8');
INSERT INTO `text` VALUES (921,'Zurück zu $1.','utf-8');
INSERT INTO `text` VALUES (922,'Neues Passwort (nochmal)','utf-8');
INSERT INTO `text` VALUES (923,'Erneut hochladen','utf-8');
INSERT INTO `text` VALUES (924,'Zurück zur Hochladen-Seite.','utf-8');
INSERT INTO `text` VALUES (925,'Auf eine alte Version zurückgesetzt','utf-8');
INSERT INTO `text` VALUES (926,'Zurücksetzen','utf-8');
INSERT INTO `text` VALUES (927,'revert','utf-8');
INSERT INTO `text` VALUES (928,'Wiederhergestellt zur letzten Änderung von $1','utf-8');
INSERT INTO `text` VALUES (929,'Frühere Versionen','utf-8');
INSERT INTO `text` VALUES (930,'Version vom $1','utf-8');
INSERT INTO `text` VALUES (931,'Version vom $1; $2<br />$3 | $4','utf-8');
INSERT INTO `text` VALUES (932,'Keine früheren Versionen gefunden','utf-8');
INSERT INTO `text` VALUES (933,'Die Version dieses Artikels, nach der Sie suchen, konnte nicht gefunden werden. Bitte überprüfen Sie die URL dieser Seite.','utf-8');
INSERT INTO `text` VALUES (934,'http://www.faqs.org/rfcs/rfc$1.html','utf-8');
INSERT INTO `text` VALUES (935,'Rechte:','utf-8');
INSERT INTO `text` VALUES (936,'This is a log of changes to user rights.','utf-8');
INSERT INTO `text` VALUES (937,'Zurücknahme der Änderungen','utf-8');
INSERT INTO `text` VALUES (938,'Rollback','utf-8');
INSERT INTO `text` VALUES (939,'Zurücknahme gescheitert','utf-8');
INSERT INTO `text` VALUES (940,'Rollback','utf-8');
INSERT INTO `text` VALUES (941,'Zeilen','utf-8');
INSERT INTO `text` VALUES (942,'Samstag','utf-8');
INSERT INTO `text` VALUES (943,'Artikel speichern','utf-8');
INSERT INTO `text` VALUES (944,'Ihre Einstellungen wurden gespeichert.','utf-8');
INSERT INTO `text` VALUES (945,'Datei speichern','utf-8');
INSERT INTO `text` VALUES (946,'Save Group','utf-8');
INSERT INTO `text` VALUES (947,'Einstellungen speichern','utf-8');
INSERT INTO `text` VALUES (948,'Save User Groups','utf-8');
INSERT INTO `text` VALUES (949,'[Interwiki transcluding is disabled]','utf-8');
INSERT INTO `text` VALUES (950,'[Template fetch failed for $1; sorry]','utf-8');
INSERT INTO `text` VALUES (951,'[URL is too long; sorry]','utf-8');
INSERT INTO `text` VALUES (952,'Suche','utf-8');
INSERT INTO `text` VALUES (953,'<p>Entschuldigung! Die Volltextsuche wurde wegen Überlastung temporär deaktiviert. Derweil können Sie die folgende Google Suche verwenden, die allerdings nicht den aktuellen Stand wiederspiegelt.<p>','utf-8');
INSERT INTO `text` VALUES (954,'Search full text','utf-8');
INSERT INTO `text` VALUES (955,'Für die Suchanfrage \"$1\"','utf-8');
INSERT INTO `text` VALUES (956,'Suchergebnisse','utf-8');
INSERT INTO `text` VALUES (957,'Suchergebnisse','utf-8');
INSERT INTO `text` VALUES (958,'Für mehr Information über {{SITENAME}}, siehe [[Project:Suche|{{SITENAME}} durchsuchen]].','utf-8');
INSERT INTO `text` VALUES (959,'&rarr;','utf-8');
INSERT INTO `text` VALUES (960,'Eine neuere Version zum Vergleich auswählen','utf-8');
INSERT INTO `text` VALUES (961,'Eine ältere Version zum Vergleich auswählen','utf-8');
INSERT INTO `text` VALUES (962,'Seiten, die auf sich selbst verweisen','utf-8');
INSERT INTO `text` VALUES (963,'Die folgenden Artikel verweisen auf sich selbst, was sie nicht sollten.','utf-8');
INSERT INTO `text` VALUES (964,'Source and destination titles are the same; can\'t move a page over itself.','utf-8');
INSERT INTO `text` VALUES (965,'Sep','utf-8');
INSERT INTO `text` VALUES (966,'September','utf-8');
INSERT INTO `text` VALUES (967,'Aktuelle Zeit auf dem Server','utf-8');
INSERT INTO `text` VALUES (968,'There seems to be a problem with your login session;\nthis action has been canceled as a precaution against session hijacking.\nPlease hit \"back\" and reload the page you came from, then try again.','utf-8');
INSERT INTO `text` VALUES (969,'<b>Benutzerrechte für \"$1\" konnten nicht gesetzt werden. (Haben Sie den Namen korrekt eingegeben?)</b>','utf-8');
INSERT INTO `text` VALUES (970,'Benutzerrechte setzen','utf-8');
INSERT INTO `text` VALUES (971,'Set bureaucrat flag','utf-8');
INSERT INTO `text` VALUES (972,'Set steward flag','utf-8');
INSERT INTO `text` VALUES (973,'-','utf-8');
INSERT INTO `text` VALUES (974,'This file is a shared upload and may be used by other projects.','utf-8');
INSERT INTO `text` VALUES (975,'Please see the [$1 file description page] for further information.','utf-8');
INSERT INTO `text` VALUES (976,'Kurze Artikel','utf-8');
INSERT INTO `text` VALUES (977,'Einblenden','utf-8');
INSERT INTO `text` VALUES (978,'Download high resolution version ($1x$2, $3 KB)','utf-8');
INSERT INTO `text` VALUES (979,'&Auml;nderungen zeigen','utf-8');
INSERT INTO `text` VALUES (980,'kleine Änderungen $1 | $2 bots | $3 logged in users | $4 patrolled edits','utf-8');
INSERT INTO `text` VALUES (981,'Hier sind <b>$1</b> Ergebnisse, beginnend mit #<b>$2</b>.','utf-8');
INSERT INTO `text` VALUES (982,'Hier sind <b>$3</b> Ergebnisse, beginnend mit #<b>$2</b>.','utf-8');
INSERT INTO `text` VALUES (983,'Zeige die letzten $1 Bilder, sortiert nach $2.','utf-8');
INSERT INTO `text` VALUES (984,'Vorschau zeigen','utf-8');
INSERT INTO `text` VALUES (985,'Anzeigen','utf-8');
INSERT INTO `text` VALUES (986,'\n* navigation\n** mainpage|mainpage\n** portal-url|portal\n** currentevents-url|currentevents\n** recentchanges-url|recentchanges\n** randompage-url|randompage\n** helppage|help\n** sitesupport-url|sitesupport','utf-8');
INSERT INTO `text` VALUES (987,'Ihre Signatur mit Zeitstempel','utf-8');
INSERT INTO `text` VALUES (988,'siteadmin level','utf-8');
INSERT INTO `text` VALUES (989,'-','utf-8');
INSERT INTO `text` VALUES (990,'Seitenstatistik','utf-8');
INSERT INTO `text` VALUES (991,'Es gibt insgesamt <b>$1</b> Seiten in der Datenbank.\nDas schliesst \"Diskussion\"-Seiten, Seiten über {{SITENAME}}, extrem kurze Artikel, Weiterleitungen und andere Seiten ein, die nicht als Artikel gelten können.\nDiese ausgenommen, gibt es <b>$2</b> Seiten, die als Artikel gelten können.<p>\nEs wurden insgesamt <b>$3</b>&times; Seiten aufgerufen, und <b>$4</b>&times; Seiten bearbeitet.\nDaraus ergeben sich <b>$5</b> Bearbeitungen pro Seite, und <b>$6</b> Betrachtungen pro Bearbeitung.','utf-8');
INSERT INTO `text` VALUES (992,'Die freie Wissensdatenbank','utf-8');
INSERT INTO `text` VALUES (993,'Spenden','utf-8');
INSERT INTO `text` VALUES (994,'Project:Site support','utf-8');
INSERT INTO `text` VALUES (995,'{{SITENAME}}','utf-8');
INSERT INTO `text` VALUES (996,'{{SITENAME}}-Benutzer $1','utf-8');
INSERT INTO `text` VALUES (997,'{{SITENAME}}-Benutzer $1','utf-8');
INSERT INTO `text` VALUES (998,'Skin','utf-8');
INSERT INTO `text` VALUES (999,'(Preview)','utf-8');
INSERT INTO `text` VALUES (1000,'SORBS DNSBL','utf-8');
INSERT INTO `text` VALUES (1001,'Your IP address is listed as an open proxy in the [http://www.sorbs.net SORBS] DNSBL. You cannot create an account','utf-8');
INSERT INTO `text` VALUES (1002,'Your IP address is listed as an open proxy in the [http://www.sorbs.net SORBS] DNSBL.','utf-8');
INSERT INTO `text` VALUES (1003,'Source filename','utf-8');
INSERT INTO `text` VALUES (1004,'The following text is what triggered our spam filter: $1','utf-8');
INSERT INTO `text` VALUES (1005,'The page you wanted to save was blocked by the spam filter. This is probably caused by a link to an external site.','utf-8');
INSERT INTO `text` VALUES (1006,'Spam protection filter','utf-8');
INSERT INTO `text` VALUES (1007,'Titel:','utf-8');
INSERT INTO `text` VALUES (1008,'Benutzer:','utf-8');
INSERT INTO `text` VALUES (1009,'Spezialseite','utf-8');
INSERT INTO `text` VALUES (1010,'Spezialseiten','utf-8');
INSERT INTO `text` VALUES (1011,'Spezialseiten','utf-8');
INSERT INTO `text` VALUES (1012,'(SQL query hidden)','utf-8');
INSERT INTO `text` VALUES (1013,'Statistik','utf-8');
INSERT INTO `text` VALUES (1014,'Gespeicherte Version','utf-8');
INSERT INTO `text` VALUES (1015,'Kurze Artikel markieren bis','utf-8');
INSERT INTO `text` VALUES (1016,'Unterkategorien','utf-8');
INSERT INTO `text` VALUES (1017,'Diese Kategorie hat $1 Unterkategorien.','utf-8');
INSERT INTO `text` VALUES (1018,'There is $1 subcategory to this category.','utf-8');
INSERT INTO `text` VALUES (1019,'Betreff','utf-8');
INSERT INTO `text` VALUES (1020,'View subject','utf-8');
INSERT INTO `text` VALUES (1021,'Erfolgreich hochgeladen','utf-8');
INSERT INTO `text` VALUES (1022,'Zusammenfassung','utf-8');
INSERT INTO `text` VALUES (1023,'Sonntag','utf-8');
INSERT INTO `text` VALUES (1024,'Dieser Vorgang kann aus Sicherheitsgründen nur von Benutzern mit\"Sysop\"-Status durchgeführt werden. Siehe auch $1.','utf-8');
INSERT INTO `text` VALUES (1025,'Sysop-Zugang notwendig','utf-8');
INSERT INTO `text` VALUES (1026,'Tabelle','utf-8');
INSERT INTO `text` VALUES (1027,'aus {{SITENAME}}, der freien Wissensdatenbank','utf-8');
INSERT INTO `text` VALUES (1028,'Diskussion','utf-8');
INSERT INTO `text` VALUES (1029,'Die Seite selbst wurde erfolgreich verschoben, aber die\nDiskussions-Seite nicht, da schon eine mit dem neuen Titel existiert. Bitte gleichen Sie die Inhalte von Hand ab.','utf-8');
INSERT INTO `text` VALUES (1030,'Diskussion','utf-8');
INSERT INTO `text` VALUES (1031,'Die \"Diskussions\"-Seite wurde ebenfalls verschoben.','utf-8');
INSERT INTO `text` VALUES (1032,'Die \"Diskussions\"-Seite wurde <strong>nicht</strong> verschoben.','utf-8');
INSERT INTO `text` VALUES (1033,'<!-- MediaWiki:talkpagetext -->','utf-8');
INSERT INTO `text` VALUES (1034,'Templates used on this page:','utf-8');
INSERT INTO `text` VALUES (1035,'Textfeld-Größe','utf-8');
INSERT INTO `text` VALUES (1036,'Übereinstimmungen mit Texten','utf-8');
INSERT INTO `text` VALUES (1037,'Ansehen oder wiederherstellen von $1?','utf-8');
INSERT INTO `text` VALUES (1038,'vergrößern','utf-8');
INSERT INTO `text` VALUES (1039,'Thumbnail size :','utf-8');
INSERT INTO `text` VALUES (1040,'Donnerstag','utf-8');
INSERT INTO `text` VALUES (1041,'Zeitzone','utf-8');
INSERT INTO `text` VALUES (1042,'Unterschied','utf-8');
INSERT INTO `text` VALUES (1043,'Geben Sie die Anzahl der Stunden ein, die zwischen Ihrer Zeitzone und UTC liegen.','utf-8');
INSERT INTO `text` VALUES (1044,'Übereinstimmungen mit Überschriften','utf-8');
INSERT INTO `text` VALUES (1045,'Inhaltsverzeichnis','utf-8');
INSERT INTO `text` VALUES (1046,'Seiten mit Doppelklick bearbeiten (JavaScript)','utf-8');
INSERT INTO `text` VALUES (1047,'Links zum Bearbeiten einzelner Absätze anzeigen','utf-8');
INSERT INTO `text` VALUES (1048,'Einzelne Absätze per Rechtsklick bearbeiten (Javascript)','utf-8');
INSERT INTO `text` VALUES (1049,'Text-Eingabefeld mit voller Breite','utf-8');
INSERT INTO `text` VALUES (1050,'Benachrichtigungsmails auch für kleine Seitenänderungen','utf-8');
INSERT INTO `text` VALUES (1051,'Ihre E-Mail-Adresse wird in Benachrichtigungsmails gezeigt','utf-8');
INSERT INTO `text` VALUES (1052,'Benachrichtigungsmails für Änderungen an Ihren Benutzerseiten','utf-8');
INSERT INTO `text` VALUES (1053,'Benachrichtigungsmails für Änderungen an Wiki-Seiten','utf-8');
INSERT INTO `text` VALUES (1054,'Use external diff by default','utf-8');
INSERT INTO `text` VALUES (1055,'Use external editor by default','utf-8');
INSERT INTO `text` VALUES (1056,'Einfache Unterschrift (Spitzname ohne Link)','utf-8');
INSERT INTO `text` VALUES (1057,'Keine kleinen Änderungen in Letzte Änderungen anzeigen','utf-8');
INSERT INTO `text` VALUES (1058,'Verweise auf leere Themen hervorheben','utf-8');
INSERT INTO `text` VALUES (1059,'Text als Blocksatz','utf-8');
INSERT INTO `text` VALUES (1060,'Alle Änderungen als geringfügig markieren','utf-8');
INSERT INTO `text` VALUES (1061,'Seitencache deaktivieren','utf-8');
INSERT INTO `text` VALUES (1062,'Überschriften automatisch nummerieren','utf-8');
INSERT INTO `text` VALUES (1063,'Vorschau beim ersten Editieren anzeigen','utf-8');
INSERT INTO `text` VALUES (1064,'Vorschau vor dem Editierfenster anzeigen','utf-8');
INSERT INTO `text` VALUES (1065,'Dauerhaftes Einloggen','utf-8');
INSERT INTO `text` VALUES (1066,'Zeige die Anzahl seitenbeobachtender Benutzer (in Letzte Änderungen, Beobachtungsliste und Artikelseiten)','utf-8');
INSERT INTO `text` VALUES (1067,'Anzeigen eines Inhaltsverzeichnisses bei Artikeln mit mehr als 3 Überschriften','utf-8');
INSERT INTO `text` VALUES (1068,'Editier-Werkzeugleiste anzeigen','utf-8');
INSERT INTO `text` VALUES (1069,'Verweise unterstreichen','utf-8');
INSERT INTO `text` VALUES (1070,'Erweiterte letzte Änderungen (nicht für alle Browser geeignet)','utf-8');
INSERT INTO `text` VALUES (1071,'Neue und geänderte Seiten beobachten','utf-8');
INSERT INTO `text` VALUES (1072,'Werkzeuge','utf-8');
INSERT INTO `text` VALUES (1073,'Unterschiede zwischen zwei ausgewählten Versionen dieser Seite vergleichen. [alt-v]','utf-8');
INSERT INTO `text` VALUES (1074,'Zeigt Ihre Änderungen am Text tabellarisch an [alt-d]','utf-8');
INSERT INTO `text` VALUES (1075,'Diese Änderung als klein markieren. [alt-i]','utf-8');
INSERT INTO `text` VALUES (1076,'Vorschau der Änderungen an dieser Seite. Benutzen Sie dies vor dem Speichern! [alt-p]','utf-8');
INSERT INTO `text` VALUES (1077,'Änderungen speichern [alt-s]','utf-8');
INSERT INTO `text` VALUES (1078,'Suchen [alt-f]','utf-8');
INSERT INTO `text` VALUES (1079,'Diese Seite beobachten. [alt-w]','utf-8');
INSERT INTO `text` VALUES (1080,'; $4$5 : [$2 $1]','utf-8');
INSERT INTO `text` VALUES (1081,'<div id=\'mw_trackbacks\'>\nTrackbacks for this article:<br/>\n$1\n</div>','utf-8');
INSERT INTO `text` VALUES (1082,'The trackback was successfully deleted.','utf-8');
INSERT INTO `text` VALUES (1083,'; $4$5 : [$2 $1]: <nowiki>$3</nowiki>','utf-8');
INSERT INTO `text` VALUES (1084,'Trackback','utf-8');
INSERT INTO `text` VALUES (1085,' ([$1 Delete])','utf-8');
INSERT INTO `text` VALUES (1086,'Try exact match','utf-8');
INSERT INTO `text` VALUES (1087,'Dienstag','utf-8');
INSERT INTO `text` VALUES (1088,'Zeige die letzten $1 Beiträge; zeige die letzten $2 Tage.','utf-8');
INSERT INTO `text` VALUES (1089,'Dies sind die letzten <b>$1</b> Beiträge des Benutzers in den letzten <b>$2</b> Tagen.','utf-8');
INSERT INTO `text` VALUES (1090,' (top)','utf-8');
INSERT INTO `text` VALUES (1091,'IP-Adresse freigeben','utf-8');
INSERT INTO `text` VALUES (1092,'Benutzen Sie das Formular, um eine blockierte IP-Adresse freizugeben.','utf-8');
INSERT INTO `text` VALUES (1093,'freigeben','utf-8');
INSERT INTO `text` VALUES (1094,'Blockade von [[Benutzer:$1]] aufgehoben','utf-8');
INSERT INTO `text` VALUES (1095,'Uncategorized categories','utf-8');
INSERT INTO `text` VALUES (1096,'Uncategorized pages','utf-8');
INSERT INTO `text` VALUES (1097,'Gelöschte Seite wiederherstellen','utf-8');
INSERT INTO `text` VALUES (1098,'Wiederherstellen','utf-8');
INSERT INTO `text` VALUES (1099,'Wiederherstellen','utf-8');
INSERT INTO `text` VALUES (1100,'Gelöschten Artikel wiederherstellen','utf-8');
INSERT INTO `text` VALUES (1101,'Wiederherstellen!','utf-8');
INSERT INTO `text` VALUES (1102,'\"$1\" wiederhergestellt','utf-8');
INSERT INTO `text` VALUES (1103,'$1 revisions restored','utf-8');
INSERT INTO `text` VALUES (1104,'Der Artikel [[$1]] wurde erfolgreich wiederhergestellt.','utf-8');
INSERT INTO `text` VALUES (1105,'Wenn Sie diese Seite wiederherstellen, werden auch alle alten\nVersionen wiederhergestellt. Wenn seit der Löschung ein neuer Artikel gleichen\nNamens erstellt wurde, werden die wiederhergestellten Versionen als alte Versionen\ndieses Artikels erscheinen.','utf-8');
INSERT INTO `text` VALUES (1106,'Gelöschte Seiten wiederherstellen','utf-8');
INSERT INTO `text` VALUES (1107,'Die folgenden Seiten wurden gelöscht, sind aber immer noch\ngespeichert und können wiederhergestellt werden.','utf-8');
INSERT INTO `text` VALUES (1108,'Gelöschte Version vom $1','utf-8');
INSERT INTO `text` VALUES (1109,'$1 Versionen archiviert','utf-8');
INSERT INTO `text` VALUES (1110,'Always','utf-8');
INSERT INTO `text` VALUES (1111,'Browser default','utf-8');
INSERT INTO `text` VALUES (1112,'Never','utf-8');
INSERT INTO `text` VALUES (1113,'Unerwarteter Wert: \"$1\"=\"$2\".','utf-8');
INSERT INTO `text` VALUES (1114,'Datenbank freigeben','utf-8');
INSERT INTO `text` VALUES (1115,'Ja, ich möchte die Datenbank freigeben.','utf-8');
INSERT INTO `text` VALUES (1116,'Datenbank freigeben','utf-8');
INSERT INTO `text` VALUES (1117,'Datenbank wurde erfolgreich freigegeben','utf-8');
INSERT INTO `text` VALUES (1118,'Die {{SITENAME}}-Datenbank wurde freigegeben.','utf-8');
INSERT INTO `text` VALUES (1119,'Das Aufheben der Datenbank-Sperre wird alle Änderungen wieder zulassen. Bitte bestätigen Sie Ihre Absicht, die Sperrung aufzuheben.','utf-8');
INSERT INTO `text` VALUES (1120,'Freigeben','utf-8');
INSERT INTO `text` VALUES (1121,'Grund für das Aufheben der Sperrung','utf-8');
INSERT INTO `text` VALUES (1122,'Artikel [[$1]] freigegeben','utf-8');
INSERT INTO `text` VALUES (1123,'(Aufhebung der Sperrung von \"$1\")','utf-8');
INSERT INTO `text` VALUES (1124,'Schutz aufheben','utf-8');
INSERT INTO `text` VALUES (1125,'Unused categories','utf-8');
INSERT INTO `text` VALUES (1126,'The following category pages exist although no other article or category make use of them.','utf-8');
INSERT INTO `text` VALUES (1127,'Verwaiste Bilder','utf-8');
INSERT INTO `text` VALUES (1128,'<p>Bitte beachten Sie, dass andere Wikis möglicherweise einige dieser Bilder benutzen.','utf-8');
INSERT INTO `text` VALUES (1129,'nicht mehr beobachten','utf-8');
INSERT INTO `text` VALUES (1130,'Nicht mehr beobachten','utf-8');
INSERT INTO `text` VALUES (1131,'(Geändert)','utf-8');
INSERT INTO `text` VALUES (1132,'Hochladen','utf-8');
INSERT INTO `text` VALUES (1133,'The upload directory ($1) is not writable by the webserver.','utf-8');
INSERT INTO `text` VALUES (1134,'Dateien hochladen','utf-8');
INSERT INTO `text` VALUES (1135,'Die Datei ist beschädigt oder hat einen falschen Namen. Bitte überprüfen Sie die Datei und laden Sie sie erneut hoch.','utf-8');
INSERT INTO `text` VALUES (1136,'Entschuldigung, das Hochladen ist deaktiviert.','utf-8');
INSERT INTO `text` VALUES (1137,'Hochgeladene Dateien','utf-8');
INSERT INTO `text` VALUES (1138,'\"[[$1]]\" hochgeladen','utf-8');
INSERT INTO `text` VALUES (1139,'Fehler beim Hochladen','utf-8');
INSERT INTO `text` VALUES (1140,'Bilder hochladen','utf-8');
INSERT INTO `text` VALUES (1141,'Datei-Logbuch','utf-8');
INSERT INTO `text` VALUES (1142,'Datei-Logbuch','utf-8');
INSERT INTO `text` VALUES (1143,'Hier ist die Liste der letzten hochgeladenen Dateien.\nAlle Zeiten sind UTC.\n<ul>\n</ul>','utf-8');
INSERT INTO `text` VALUES (1144,'[$1 Upload a new version of this file]','utf-8');
INSERT INTO `text` VALUES (1145,'Nicht angemeldet','utf-8');
INSERT INTO `text` VALUES (1146,'Sie müssen [[Spezial:Userlogin|angemeldet sein]],\n um Dateien hochladen zu können.','utf-8');
INSERT INTO `text` VALUES (1147,'Diese Datei enthält HTML- oder Scriptcode der irrtümlich von einem Webbrowser ausgeführt werden könnte.','utf-8');
INSERT INTO `text` VALUES (1148,'\nUm hochgeladene Bilder zu suchen und anzusehen,\ngehen Sie zu der [[Spezial:Imagelist|Liste hochgeladener Bilder]].\n\nBenutzen Sie das Formular, um neue Bilder hochzuladen und\nsie in Artikeln zu verwenden.\nIn den meisten Browsern werden Sie ein \"Durchsuchen\"-Feld sehen,\ndas einen Standard-Dateidialog öffnet.\nSuchen Sie sich eine Datei aus. Die Datei wird dann im Textfeld angezeigt.\nBestätigen Sie dann die Copyright-Vereinbarung.\nSchließlich drücken Sie den \"Hochladen\"-Knopf.\nDies kann eine Weile dauern, besonders bei einer langsamen Internet-Verbindung.\n\nFür Photos wird das JPEG-Format, für Zeichnungen und Symbole das PNG-Format bevorzugt.\nUm ein Bild in einem Artikel zu verwenden, schreiben Sie an Stelle des Bildes\n\'\'\'<nowiki>[[bild:datei.jpg]]</nowiki>\'\'\' oder\n\'\'\'<nowiki>[[bild:datei.jpg|Beschreibung]]</nowiki>\'\'\'.\n\nBitte beachten Sie, dass, genau wie bei den Artikeln, andere Benutzer Ihre Dateien löschen oder verändern können.','utf-8');
INSERT INTO `text` VALUES (1149,'Diese Datei enthält einen Virus! Details: $1','utf-8');
INSERT INTO `text` VALUES (1150,'Warnung','utf-8');
INSERT INTO `text` VALUES (1151,'1\n\nSet first character to \"0\" to disable the new category page layout.','utf-8');
INSERT INTO `text` VALUES (1152,'<b>Benutzerrechte für \"$1\" aktualisiert</b>','utf-8');
INSERT INTO `text` VALUES (1153,'<strong>Tipp:</strong> Benutzen Sie den Vorschau-Button, um Ihr neues css/js vor dem Speichern zu testen.','utf-8');
INSERT INTO `text` VALUES (1154,'== Vorschau ihres Benutzer-CSS. ==\n\'\'\'Beachten Sie:\'\'\' Nach dem Speichern müssen Sie ihrem Browser sagen, die neue Version zu laden: \'\'\'Mozilla:\'\'\' Klick auf \'\'Neu laden\'\'(oder \'\'Strg-R\'\'), \'\'\'IE / Opera:\'\'\' \'\'Strg-F5\'\', \'\'\'Safari:\'\'\' \'\'Cmd-r\'\', \'\'\'Konqueror\'\'\' \'\'Strg-R\'\'.','utf-8');
INSERT INTO `text` VALUES (1155,'Dieser Benutzername ist schon vergeben. Bitte wählen Sie einen anderen.','utf-8');
INSERT INTO `text` VALUES (1156,'== Vorschau Ihres Benutzer-Javascript. ==\n\'\'\'Beachten Sie:\'\'\' Nach dem Speichern müssen Sie ihrem Browser sagen, die neue Version zu laden: \'\'\'Mozilla:\'\'\' Klick auf \'\'Neu laden\'\'(oder \'\'Strg-R\'\'), \'\'\'IE / Opera:\'\'\' \'\'Strg-F5\'\', \'\'\'Safari:\'\'\' \'\'Cmd-r\'\', \'\'\'Konqueror\'\'\' \'\'Strg-R\'\'.','utf-8');
INSERT INTO `text` VALUES (1157,'Anmelden','utf-8');
INSERT INTO `text` VALUES (1158,'Abmelden','utf-8');
INSERT INTO `text` VALUES (1159,'Das Mail-Objekt gab einen Fehler zurück:','utf-8');
INSERT INTO `text` VALUES (1160,'Benutzerseite','utf-8');
INSERT INTO `text` VALUES (1161,'User rights management','utf-8');
INSERT INTO `text` VALUES (1162,'Edit user groups','utf-8');
INSERT INTO `text` VALUES (1163,'Available groups:','utf-8');
INSERT INTO `text` VALUES (1164,'Select groups you want the user to be removed from or added to.\nUnselected groups will not be changed. You can deselect a group with CTRL + Left Click','utf-8');
INSERT INTO `text` VALUES (1165,'Member of:','utf-8');
INSERT INTO `text` VALUES (1166,'Changed group membership from $1 to $2','utf-8');
INSERT INTO `text` VALUES (1167,'Manage user groups','utf-8');
INSERT INTO `text` VALUES (1168,'Enter a username:','utf-8');
INSERT INTO `text` VALUES (1169,'userrights level','utf-8');
INSERT INTO `text` VALUES (1170,'Benutzerstatistik','utf-8');
INSERT INTO `text` VALUES (1171,'Es gibt <b>$1</b> registrierte Benutzer.\nDavon haben <b>$2</b> Administrator-Rechte (siehe $3).','utf-8');
INSERT INTO `text` VALUES (1172,'Add','utf-8');
INSERT INTO `text` VALUES (1173,'List of validated articles','utf-8');
INSERT INTO `text` VALUES (1174,'Clear my older validation data','utf-8');
INSERT INTO `text` VALUES (1175,'Delete','utf-8');
INSERT INTO `text` VALUES (1176,'<sub>User</sub> \\ <sup>Topic</sup>','utf-8');
INSERT INTO `text` VALUES (1177,'User $1','utf-8');
INSERT INTO `text` VALUES (1178,'\'\'\'Hint:\'\'\' Merging your data means that for the article revision you select, all options where you have specified \'\'no opinion\'\' will be set to the value and comment of the most recent revision for which you have expressed an opinion. For example, if you want to change a single option for a newer revision, but also keep your other settings for this article in this revision, just select which option you intend to \'\'change\'\', and merging will fill in the other options with your previous settings.','utf-8');
INSERT INTO `text` VALUES (1179,'Check this box if you really mean it!','utf-8');
INSERT INTO `text` VALUES (1180,'<th>#</th><th>Topic</th><th>Range</th><th>Action</th>','utf-8');
INSERT INTO `text` VALUES (1181,'Use my previous assessment where selected \'No opinion\'','utf-8');
INSERT INTO `text` VALUES (1182,'My validation overview','utf-8');
INSERT INTO `text` VALUES (1183,'No','utf-8');
INSERT INTO `text` VALUES (1184,'You have to be logged in to validate an article.','utf-8');
INSERT INTO `text` VALUES (1185,'No opinion','utf-8');
INSERT INTO `text` VALUES (1186,'$1 of $2','utf-8');
INSERT INTO `text` VALUES (1187,'Page validation statistics for $1','utf-8');
INSERT INTO `text` VALUES (1188,'<b>$1%</b><br />($2 of $3 points<br />by $4 users)','utf-8');
INSERT INTO `text` VALUES (1189,'<b>$1%</b><br />($2 of $3 points<br />by one user)','utf-8');
INSERT INTO `text` VALUES (1190,'Revisions for $1','utf-8');
INSERT INTO `text` VALUES (1191,'See the validation statistics for \"$1\" <a href=\"$2\">here</a>','utf-8');
INSERT INTO `text` VALUES (1192,'Revision','utf-8');
INSERT INTO `text` VALUES (1193,'Your ratings have been stored!','utf-8');
INSERT INTO `text` VALUES (1194,'Revision #$1','utf-8');
INSERT INTO `text` VALUES (1195,'Revision of $1','utf-8');
INSERT INTO `text` VALUES (1196,'details','utf-8');
INSERT INTO `text` VALUES (1197,'Show my validations','utf-8');
INSERT INTO `text` VALUES (1198,'Validation statistics for this article','utf-8');
INSERT INTO `text` VALUES (1199,'Validate','utf-8');
INSERT INTO `text` VALUES (1200,'<tr><th>Class</th>$1<th colspan=4>Opinion</th>$1<th>Comment</th></tr>','utf-8');
INSERT INTO `text` VALUES (1201,'this is the latest version','utf-8');
INSERT INTO `text` VALUES (1202,'Time','utf-8');
INSERT INTO `text` VALUES (1203,'Project:Validation topics','utf-8');
INSERT INTO `text` VALUES (1204,'Total','utf-8');
INSERT INTO `text` VALUES (1205,'Validation overview of user $1','utf-8');
INSERT INTO `text` VALUES (1206,'This user has validated $1 pages.','utf-8');
INSERT INTO `text` VALUES (1207,'Only articles can be validated. This page is <i>not</i> in the article namespace.','utf-8');
INSERT INTO `text` VALUES (1208,'Validate this version','utf-8');
INSERT INTO `text` VALUES (1209,'Validation done.','utf-8');
INSERT INTO `text` VALUES (1210,'Validation of \"$1\"','utf-8');
INSERT INTO `text` VALUES (1211,'Version','utf-8');
INSERT INTO `text` VALUES (1212,'Version of $1','utf-8');
INSERT INTO `text` VALUES (1213,'View this revision','utf-8');
INSERT INTO `text` VALUES (1214,'Change this text <a href=\"{{SERVER}}{{localurl:MediaWiki:Val_votepage_intro}}\">here</a>!','utf-8');
INSERT INTO `text` VALUES (1215,'<b>Never, <i>ever</i>, change something here without <i>explicit</i> community consensus!</b>','utf-8');
INSERT INTO `text` VALUES (1216,'Yes','utf-8');
INSERT INTO `text` VALUES (1217,'Validate page','utf-8');
INSERT INTO `text` VALUES (1218,'zh','utf-8');
INSERT INTO `text` VALUES (1219,'cn','utf-8');
INSERT INTO `text` VALUES (1220,'hk','utf-8');
INSERT INTO `text` VALUES (1221,'sg','utf-8');
INSERT INTO `text` VALUES (1222,'tw','utf-8');
INSERT INTO `text` VALUES (1223,'Version','utf-8');
INSERT INTO `text` VALUES (1224,'Version $1 of MediaWiki required','utf-8');
INSERT INTO `text` VALUES (1225,'Version $1 of MediaWiki is required to use this page. See [[Special:Version]]','utf-8');
INSERT INTO `text` VALUES (1226,'Diese Seite wurde bisher $1 mal abgerufen.','utf-8');
INSERT INTO `text` VALUES (1227,'Zeige ($1) ($2) ($3).','utf-8');
INSERT INTO `text` VALUES (1228,'Views','utf-8');
INSERT INTO `text` VALUES (1229,'Quelltext betrachten','utf-8');
INSERT INTO `text` VALUES (1230,'Diskussion','utf-8');
INSERT INTO `text` VALUES (1231,'Gewünschte Seiten','utf-8');
INSERT INTO `text` VALUES (1232,'Beobachten','utf-8');
INSERT INTO `text` VALUES (1233,'* Sie beobachten zur Zeit insgesamt $1 Artikel (Diskussionsseiten wurden hier nicht mitgezählt).\n* [[Special:Watchlist/edit|Gesamte Beobachtungsliste]] anzeigen und bearbeiten.','utf-8');
INSERT INTO `text` VALUES (1234,'Alphabetische Liste der von Ihnen beobachteten Seiten.<br>\nHier können Sie Seiten markieren, um Sie dann von der Beobachtungsliste zu löschen.','utf-8');
INSERT INTO `text` VALUES (1235,'Beobachtungsliste','utf-8');
INSERT INTO `text` VALUES (1236,'all','utf-8');
INSERT INTO `text` VALUES (1237,'all','utf-8');
INSERT INTO `text` VALUES (1238,'Ihre Beobachtungsliste enthält $1 Seiten.','utf-8');
INSERT INTO `text` VALUES (1239,'(für Benutzer \"$1\")','utf-8');
INSERT INTO `text` VALUES (1240,'Überprüfen der Beobachtungsliste nach letzten Bearbeitungen','utf-8');
INSERT INTO `text` VALUES (1241,'Überprüfen der letzten Bearbeitungen für die Beobachtungsliste','utf-8');
INSERT INTO `text` VALUES (1242,'Keine Ihrer beobachteten Artikel wurde während des angezeigten Zeitraums bearbeitet.','utf-8');
INSERT INTO `text` VALUES (1243,'Sie sind nicht angemeldet','utf-8');
INSERT INTO `text` VALUES (1244,'Sie müssen [[Special:Userlogin|angemeldet]]\nsein, um Ihre Beobachtungsliste zu bearbeiten.','utf-8');
INSERT INTO `text` VALUES (1245,'Diesen Artikel beobachten','utf-8');
INSERT INTO `text` VALUES (1246,'Seite beobachten','utf-8');
INSERT INTO `text` VALUES (1247,'Mittwoch','utf-8');
INSERT INTO `text` VALUES (1248,'<h2>Willkommen, $1!</h2><p>Ihr Benutzerkonto wurde eingerichtet.\nVergessen Sie nicht, Ihre Einstellungen anzupassen.','utf-8');
INSERT INTO `text` VALUES (1249,'Was zeigt hierhin','utf-8');
INSERT INTO `text` VALUES (1250,'Um in diesem Wiki Accounts anlegen zu dürfen müssen Sie sich [[Spezial:Userlogin|hier anmelden]] und die nötigen Berechtigungen haben.','utf-8');
INSERT INTO `text` VALUES (1251,'Sie sind nicht berechtigt einen Account zu erzeugen','utf-8');
INSERT INTO `text` VALUES (1252,'Sie müssen sich [[Spezial:Userlogin|hier anmelden]] um Artikel bearbeiten zu können.','utf-8');
INSERT INTO `text` VALUES (1253,'Zum Bearbeiten ist es erforderlich angemeldet zu sein','utf-8');
INSERT INTO `text` VALUES (1254,'Sie müssen sich [[Spezial:Userlogin|hier anmelden]] um Artikel lesen zu können.','utf-8');
INSERT INTO `text` VALUES (1255,'Zum Lesen ist es erforderlich angemeldet zu sein','utf-8');
INSERT INTO `text` VALUES (1256,'Meta-Text','utf-8');
INSERT INTO `text` VALUES (1257,'{{SITENAME}}','utf-8');
INSERT INTO `text` VALUES (1258,'* E-Mail-Benachrichtigungsdienst ist eingeschaltet.','utf-8');
INSERT INTO `text` VALUES (1259,'* Seiten mit noch nicht gesehenen Änderungen werden \'\'\'fett\'\'\' dargestellt.','utf-8');
INSERT INTO `text` VALUES (1260,'Verstecke','utf-8');
INSERT INTO `text` VALUES (1261,'$1 von mir bearbeitete Artikel.','utf-8');
INSERT INTO `text` VALUES (1262,'Es folgen die letzten $1 Änderungen der letzten <b>$2</b> Stunden.','utf-8');
INSERT INTO `text` VALUES (1263,'Dies ist eine gespeicherte Version Ihrer Beobachtungsliste.','utf-8');
INSERT INTO `text` VALUES (1264,'Zeige','utf-8');
INSERT INTO `text` VALUES (1265,'Zeige die letzen $1 Stunden $2 Tage $3','utf-8');
INSERT INTO `text` VALUES (1266,'Falsche Parameter für wfQuery()<br />\nFunktion: $1<br />\nQuery: $2','utf-8');
INSERT INTO `text` VALUES (1267,'Das Passwort ist falsch (oder fehlt). Bitte versuchen Sie es erneut.','utf-8');
INSERT INTO `text` VALUES (1268,'Unterschiede','utf-8');
INSERT INTO `text` VALUES (1269,'Your domain','utf-8');
INSERT INTO `text` VALUES (1270,'Ihre E-Mail-Adresse**','utf-8');
INSERT INTO `text` VALUES (1271,'Language','utf-8');
INSERT INTO `text` VALUES (1272,'Ihr Benutzername','utf-8');
INSERT INTO `text` VALUES (1273,'Ihr \"Spitzname\" (zum \"Unterschreiben\")','utf-8');
INSERT INTO `text` VALUES (1274,'Ihr Passwort','utf-8');
INSERT INTO `text` VALUES (1275,'Passwort wiederholen','utf-8');
INSERT INTO `text` VALUES (1276,'Ihr echter Name (keine Pflichtangabe)*','utf-8');
INSERT INTO `text` VALUES (1277,'Ihr Text','utf-8');
INSERT INTO `text` VALUES (1278,'Variant','utf-8');
INSERT INTO `text` VALUES (1279,'Die Wiki Software wurde erfolgreich installiert.\n\nSiehe die [http://meta.wikipedia.org/wiki/MediaWiki_i18n Dokumentation zur Anpassung der Benutzeroberfläche]\nund das [http://meta.wikipedia.org/wiki/MediaWiki_User%27s_Guide Benutzerhandbuch] für Hilfe zur Benutzung und Konfiguration.\n\nasdf test','utf-8');

--
-- Table structure for table `trackbacks`
--

CREATE TABLE `trackbacks` (
  `tb_id` int(11) NOT NULL auto_increment,
  `tb_page` int(11) default NULL,
  `tb_title` varchar(255) NOT NULL default '',
  `tb_url` varchar(255) NOT NULL default '',
  `tb_ex` text,
  `tb_name` varchar(255) default NULL,
  PRIMARY KEY  (`tb_id`),
  KEY `tb_page` (`tb_page`)
) TYPE=MyISAM;

--
-- Dumping data for table `trackbacks`
--


--
-- Table structure for table `user`
--

CREATE TABLE `user` (
  `user_id` int(5) unsigned NOT NULL auto_increment,
  `user_name` varchar(255) binary NOT NULL default '',
  `user_real_name` varchar(255) binary NOT NULL default '',
  `user_password` tinyblob NOT NULL,
  `user_newpassword` tinyblob NOT NULL,
  `user_email` tinytext NOT NULL,
  `user_options` blob NOT NULL,
  `user_touched` varchar(14) binary NOT NULL default '',
  `user_token` varchar(32) binary NOT NULL default '',
  `user_email_authenticated` varchar(14) binary default NULL,
  `user_email_token` varchar(32) binary default NULL,
  `user_email_token_expires` varchar(14) binary default NULL,
  PRIMARY KEY  (`user_id`),
  UNIQUE KEY `user_name` (`user_name`),
  KEY `user_email_token` (`user_email_token`)
) TYPE=MyISAM;

--
-- Dumping data for table `user`
--

INSERT INTO `user` VALUES (1,'WikiSysop','','fafe1a468d76f1be4778665f81c4cce3','','','quickbar=1\nunderline=2\ncols=80\nrows=25\nsearchlimit=20\ncontextlines=5\ncontextchars=50\nskin=monobook\nmath=1\nrcdays=7\nrclimit=50\nhighlightbroken=1\nstubthreshold=0\npreviewontop=1\neditsection=1\neditsectiononrightclick=0\nshowtoc=1\nshowtoolbar=1\ndate=0\nimagesize=2\nthumbsize=2\nrememberpassword=0\nenotifwatchlistpages=0\nenotifusertalkpages=1\nenotifminoredits=0\nenotifrevealaddr=0\nshownumberswatching=1\nfancysig=0\nexternaleditor=0\nexternaldiff=0\nvariant=de\nlanguage=de\nsearchNs0=1','20050831104124','a65f612aab3e6fd0334a0062adcdb070',NULL,NULL,NULL);
INSERT INTO `user` VALUES (2,'Felix','','f742e17a7b4403041a696c359de9e07d','','','quickbar=1\nunderline=2\ncols=80\nrows=25\nsearchlimit=20\ncontextlines=5\ncontextchars=50\nskin=labor3\nmath=1\nrcdays=7\nrclimit=50\nhighlightbroken=1\nstubthreshold=0\npreviewontop=1\neditsection=1\neditsectiononrightclick=0\nshowtoc=1\nshowtoolbar=1\ndate=0\nimagesize=2\nthumbsize=2\nrememberpassword=0\nenotifwatchlistpages=0\nenotifusertalkpages=1\nenotifminoredits=0\nenotifrevealaddr=0\nshownumberswatching=1\nfancysig=0\nexternaleditor=0\nexternaldiff=0\nvariant=de\nlanguage=de\nsearchNs0=1','20050831112839','42adc866895a2f5b42d61bae1ffca1ed',NULL,NULL,NULL);

--
-- Table structure for table `user_groups`
--

CREATE TABLE `user_groups` (
  `ug_user` int(5) unsigned NOT NULL default '0',
  `ug_group` char(16) NOT NULL default '',
  PRIMARY KEY  (`ug_user`,`ug_group`),
  KEY `ug_group` (`ug_group`)
) TYPE=MyISAM;

--
-- Dumping data for table `user_groups`
--

INSERT INTO `user_groups` VALUES (1,'bureaucrat');
INSERT INTO `user_groups` VALUES (1,'sysop');

--
-- Table structure for table `user_newtalk`
--

CREATE TABLE `user_newtalk` (
  `user_id` int(5) NOT NULL default '0',
  `user_ip` varchar(40) NOT NULL default '',
  KEY `user_id` (`user_id`),
  KEY `user_ip` (`user_ip`)
) TYPE=MyISAM;

--
-- Dumping data for table `user_newtalk`
--


--
-- Table structure for table `validate`
--

CREATE TABLE `validate` (
  `val_user` int(11) NOT NULL default '0',
  `val_page` int(11) unsigned NOT NULL default '0',
  `val_revision` int(11) unsigned NOT NULL default '0',
  `val_type` int(11) unsigned NOT NULL default '0',
  `val_value` int(11) default '0',
  `val_comment` varchar(255) NOT NULL default '',
  `val_ip` varchar(20) NOT NULL default '',
  KEY `val_user` (`val_user`,`val_revision`)
) TYPE=MyISAM;

--
-- Dumping data for table `validate`
--


--
-- Table structure for table `watchlist`
--

CREATE TABLE `watchlist` (
  `wl_user` int(5) unsigned NOT NULL default '0',
  `wl_namespace` int(11) NOT NULL default '0',
  `wl_title` varchar(255) binary NOT NULL default '',
  `wl_notificationtimestamp` varchar(14) binary NOT NULL default '0',
  UNIQUE KEY `wl_user` (`wl_user`,`wl_namespace`,`wl_title`),
  KEY `namespace_title` (`wl_namespace`,`wl_title`)
) TYPE=MyISAM;

--
-- Dumping data for table `watchlist`
--


