/*
 * File: about.c
 *
 * Copyright (C) 1997 Raph Levien <raph@acm.org>
 * Copyright (C) 1999, 2001 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <config.h>
#include "IO.h"
#include "Url.h"
#include "../nav.h"
#include "../web.h"

typedef struct _SplashInfo SplashInfo_t;

struct _SplashInfo {
   gint FD_Read;
   gint FD_Write;
};


/*
 * HTML text for startup screen
 */
static char *Splash=
"Content-type: text/html\n"
"\n"
"<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.01 Transitional//EN'>\n"
"<html>\n"
"<head>\n"
"<title>Splash screen for dillo-" VERSION "</title>\n"
"</head>\n"
"<body bgcolor='#778899' text='#000000' link='#000000' vlink='#000000'>\n"
"\n"
"\n"
"<!--   the head of the page   -->\n"
"\n"
"<table width='100%' border='0' cellspacing='1' cellpadding='3'>\n"
" <tr><td>\n"
"  <table border='1' cellspacing='1' cellpadding='0'>\n"
"   <tr>\n"
"   <td bgcolor='#000000'>\n"
"    <table width='100%' border='0' bgcolor='#ffffff'>\n"
"    <tr>\n"
"     <td valign='top' align='left'>\n"
"      <h1>&nbsp;Welcome to Dillo " VERSION "&nbsp;</h1>\n"
"    </table>\n"
"  </table>\n"
"</table>\n"
"\n"
"<br>\n"
"\n"
"\n"
"<!-- the main layout table, definition -->\n"
"\n"
"<table width='100%' border='0' cellspacing='0' cellpadding='0'>\n"
"<tr><td valign='top' width='150' align='center'>\n"
"\n"
"\n"
"<!--   The navigation bar   -->\n"
"\n"
"<table border='0' cellspacing='0' cellpadding='0' width='140' bgcolor='#000000'>\n"
"<tr>\n"
" <td>\n"
"  <table width='100%' border='0' cellspacing='1' cellpadding='3'>\n"
"  <tr>\n"
"   <td colspan='1' bgcolor='#CCCCCC'>Dillo\n"
"  <tr>\n"
"   <td bgcolor='#FFFFFF'>\n"
"    <table border='0' cellspacing='0' cellpadding='5'><tr><td>\n"
"    <table border='0' cellspacing='0' cellpadding='2'><tr>\n"
"    <td>\n"
"    <td>\n"
"     <a href='http://www.dillo.org/dillo-help.html'>\n"
"     Help</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.dillo.org/'>Home</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.dillo.org/funding/objectives.html'>\n"
"     Objectives</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.dillo.org/ChangeLog.html'>\n"
"     ChangeLog</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.dillo.org/interview.html'>\n"
"       Interview</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.dillo.org/D_authors.html'>\n"
"     Authors</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.dillo.org/donations.html'>\n"
"     Donate</a>\n"
"    </table>\n"
"    </table>\n"
"  </table>\n"
"</table>\n"
"\n"
"<br>\n"
"\n"
"<table border='0' cellspacing='0' cellpadding='0' width='140' bgcolor='#000000'>\n"
"<tr>\n"
" <td>\n"
"  <table width='100%' border='0' cellspacing='1' cellpadding='3'>\n"
"  <tr>\n"
"    <td colspan='1' bgcolor='#CCCCCC'>Magazines\n"
"\n"
"  <tr>\n"
"   <td bgcolor='#FFFFFF'>\n"
"    <table border='0' cellspacing='0' cellpadding='5'><tr><td>\n"
"    <table border='0' cellpadding='2'>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://lwn.net/'>LWN</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://slashdot.org/'>Slashdot</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.kuro5hin.org/?op=section;section=__all__'>KuroShin</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.nexusmagazine.com/'>Nexus&nbsp;M.</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.gnu-darwin.org/update.html'>Monster News</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.theregister.co.uk/index.html'>The Register</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.infoanarchy.org/'>InfoAnarchy</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td>\n"
"     <a href='http://www.linuxforkids.org/'>Linux4Kids</a>\n"
"    </table>\n"
"    </table>\n"
"  </table>\n"
"</table>\n"
"\n"
"<br>\n"
"\n"
"<table border='0' cellspacing='0' cellpadding='0' width='140' bgcolor='#000000'>\n"
"<tr>\n"
" <td>\n"
"  <table width='100%' border='0' cellspacing='1' cellpadding='3'>\n"
"  <tr>\n"
"   <td colspan='1' bgcolor='#CCCCCC'>Additional Stuff\n"
"\n"
"  <tr>\n"
"   <td bgcolor='#FFFFFF'>\n"
"    <table border='0' cellspacing='0' cellpadding='5'><tr><td>\n"
"    <table border='0' cellpadding='2'><tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td><a href='http://www.google.com/'>Google</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td><a href='http://www.wikipedia.org/'>Wikipedia</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td><a href='http://www.gutenberg.org/'>P. Gutenberg</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td><a href='http://freshmeat.net/'>FreshMeat</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td><a href='http://www.gnu.org/gnu/thegnuproject.html'>GNU\n"
"     project</a>\n"
"    <tr>\n"
"    <td>&nbsp;&nbsp;\n"
"    <td><a href='http://www.linuxfund.org/'>LinuxFund</a>\n"
"    </table>\n"
"    </table>\n"
"  </table>\n"
"</table>\n"
"\n"
"<br>\n"
"\n"
"<table border='0' cellspacing='0' cellpadding='0' width='140' bgcolor='#000000'>\n"
"<tr>\n"
" <td>\n"
"   <table width='100%' border='0' cellspacing='1' cellpadding='3'>\n"
"   <tr>\n"
"    <td colspan='1' bgcolor='#CCCCCC'>Essential Readings\n"
"\n"
"   <tr>\n"
"    <td bgcolor='#FFFFFF'>\n"
"     <table border='0' cellspacing='0' cellpadding='5'><tr><td>\n"
"     <table border='0' cellpadding='2'>\n"
"     <tr><td>&nbsp;&nbsp;\n"
"     <td><a href='http://www.violence.de'>Peace&amp;Violence</a>\n"
"     <tr><td>&nbsp;&nbsp;\n"
"     <td><a href='http://www.fsf.org/philosophy/right-to-read.html'>"
"      Right to Read</a>\n"
"     </table>\n"
"     </table>\n"
"   </table>\n"
"</table>\n"
"\n"
"<table border='0' width='100%' cellpadding='0' cellspacing='0'><tr><td height='10'></table>\n"
"\n"
"\n"
"<!-- the main layout table, a small vertical spacer -->\n"
"\n"
"<td width='20'><td valign='top'>\n"
"\n"
"\n"
"<!--   Main Part of the page   -->\n"
"\n"
"<table border='0' cellpadding='0' cellspacing='0' align='center' bgcolor='#000000' width='100%'><tr><td>\n"
"<table border='0' cellpadding='5' cellspacing='1' width='100%'>\n"
"<tr>\n"
" <td bgcolor='#CCCCCC'>\n"
"  <h4>Free Software</h4>\n"
"<tr>\n"
" <td bgcolor='#FFFFFF'>\n"
"  <table border='0' cellspacing='0' cellpadding='5'><tr><td>\n"
"  <p>\n"
"  Dillo is Free Software in the terms of the GPL.\n"
"  This means you have four basic freedoms:\n"
"  <ul>\n"
"   <li>Freedom to use the program any way you see fit.\n"
"   <li>Freedom to study and modify the source code.\n"
"   <li>Freedom to make backup copies.\n"
"   <li>Freedom to redistribute it.\n"
"  </ul>\n"
"  The <a href='http://www.gnu.org/licenses/gpl.html'>GPL</a>\n"
"  is the legal mechanism that gives you these freedoms.\n"
"  It also protects them from being taken away: any derivative work\n"
"  based on the program must be under the GPL.<br>\n"
"  </table>\n"
"</table>\n"
"</table>\n"
"\n"
"<br>\n"
"\n"
"<table border='0' cellpadding='0' cellspacing='0' align='center' bgcolor='#000000' width='100%'><tr><td>\n"
"<table border='0' cellpadding='5' cellspacing='1' width='100%'>\n"
"<tr>\n"
" <td bgcolor='#CCCCCC'>\n"
"  <h4>Release overview</h4>\n"
"  15 June 2005\n"
"<tr>\n"
" <td bgcolor='#FFFFFF'>\n"
"  <table border='0' cellspacing='0' cellpadding='5'>\n"
"  <tr>\n"
"   <td>\n"
"<p>\n"
"The  new dillo-" VERSION " is a bugfix release that addresses a couple\n"
"of security issues. Users are advised to upgrade ASAP.\n"
"<p>\n"
"This release is one of the last on the GTK1-based dillo series.\n"
"Currently the main development is focused on a "
"<a href='http://www.fltk.org/'>FLTK2</a>-based dillo prototype.\n"
"If everything goes as planned, this new branch will become our next release!"
" (stay tuned).\n"
"<p>\n"
"Remember that dillo project uses a release model where every new\n"
"browser shall be better than the former.\n"
"<EM>Keep up with the latest one!</EM>\n"
"  </table>\n"
"</table>\n"
"</table>\n"
"\n"
"<br>\n"
"\n"
"<table border='0' cellpadding='0' cellspacing='0' align='center' bgcolor='#000000' width='100%'><tr><td>\n"
"<table border='0' cellpadding='5' cellspacing='1' width='100%'>\n"
"<tr>\n"
" <td bgcolor='#CCCCCC'>\n"
"  <h4>ChangeLog highlights</h4>\n"
"  (Extracted from the\n"
"  <a href='http://www.dillo.org/ChangeLog.html'>full\n"
"  ChangeLog</a>)\n"
"<tr>\n"
" <td bgcolor='#FFFFFF'>\n"
"  <table border='0' cellspacing='0' cellpadding='5'>\n"
"  <tr>\n"
"   <td>\n"
"<ul>\n"
"<li>Added a \"small\" dillorc option for panel size.\n"
"<li>Set \"file:\" to work as URI for current directory.\n"
"<li>Fixed the shell escaping code in the ftp plugin.\n"
"<li>Added some checks for sane values in html.c.\n"
"<li>Added URL filtering to the ftp and downloads dpis to avoid SMTP hacks.\n"
"</ul>\n"
"  </table>\n"
"</table>\n"
"</table>\n"
"\n"
"<br>\n"
"\n"
"<table border='0' cellpadding='0' cellspacing='0' align='center' bgcolor='#000000' width='100%'><tr><td>\n"
"<table border='0' cellpadding='5' cellspacing='1' width='100%'>\n"
"<tr>\n"
" <td bgcolor='#CCCCCC'>\n"
"  <h4>Notes</h4>\n"
"<tr>\n"
" <td bgcolor='#FFFFFF'>\n"
"  <table border='0' cellspacing='0' cellpadding='5'>\n"
"  <tr>\n"
"   <td>\n"
"<ul>\n"
" <li> There's a\n"
"   <a href='http://www.dillo.org/dillorc'>dillorc</a>\n"
"   (readable  config)  file within the tarball; It is well commented\n"
"   and  has  plenty  of  options to customize dillo, so <STRONG>copy\n"
"   it</STRONG>  to  your  <STRONG>~/.dillo/</STRONG>  directory, and\n"
"   modify to your taste.\n"
" <li> There's documentation for developers in the <CODE>/doc</CODE>\n"
"   dir  within  the  tarball;  you can find directions on everything\n"
"   else at the home page.\n"
" <li> Dillo has context sensitive menus using the\n"
"   right mouse button (available on pages, links, images,\n"
"   the Back and Forward buttons, and bug meter).\n"
" <li> Dillo behaves very nicely when browsing local files, images, and HTML.\n"
"   It's also very good for Internet searching (try Google!).\n"
" <li> This release is mainly intended <strong>for developers</strong>\n"
"   and <em>advanced users</em>.\n"
" <li> Frames, Java and Javascript are not supported.\n"
"</ul>\n"
"<br>\n"
"  </table>\n"
"</table>\n"
"</table>\n"
"\n"
"<table border='0' width='100%' cellpadding='0' cellspacing='0'><tr><td height='10'></table>\n"
"\n"
"\n"
"<!-- the main layout table, a small vertical spacer -->\n"
"\n"
"<td width='20'>\n"
"\n"
"\n"
"\n"
"<!--   The right column (info)   -->\n"
"<td valign='top' align='center'>\n"
"\n"
"\n"
"\n"
"<!-- end of the main layout table -->\n"
"\n"
"\n"
"</table>\n"
"\n"
"<!--   footnotes   -->\n"
"\n"
"<br><br><center>\n"
"<hr size='2'>\n"
"<hr size='2'>\n"
"</center>\n"
"</body>\n"
"</html>\n";



/*
 * Send the splash screen through the IO using a pipe.
 */
static gint About_send_splash(ChainLink *Info, DilloUrl *Url)
{
   gint SplashPipe[2];
   IOData_t *io1;
   SplashInfo_t *SpInfo;

   if (pipe(SplashPipe))
      return -1;

   SpInfo = g_new(SplashInfo_t, 1);
   SpInfo->FD_Read  = SplashPipe[0];
   SpInfo->FD_Write = SplashPipe[1];
   Info->LocalKey = SpInfo;

   /* send splash */
   io1 = a_IO_new(IOWrite, SpInfo->FD_Write);
   a_IO_set_buf(io1, Splash, strlen(Splash));
   io1->Flags |= (IOFlag_ForceClose + IOFlag_SingleWrite);
   a_Chain_link_new(Info, a_About_ccc, BCK, a_IO_ccc, 1, 1);
   a_Chain_bcb(OpStart, Info, io1, NULL);
   a_Chain_bcb(OpSend, Info, io1, NULL);

   /* Tell the cache to receive answer */
   a_Chain_fcb(OpSend, Info, &SpInfo->FD_Read, NULL);
   return SpInfo->FD_Read;
}

/*
 * Push the right URL for each supported "about"
 * ( Data1 = Requested URL; Data2 = Web structure )
 */
static gint About_get(ChainLink *Info, void *Data1, void *Data2)
{
   char *loc;
   const char *tail;
   DilloUrl *Url = Data1;
   DilloWeb *web = Data2;
   DilloUrl *LocUrl;

   /* Don't allow the "about:" method for non-root URLs */
   if (!(web->flags & WEB_RootUrl))
      return -1;

   tail = URL_PATH(Url);

   if (!strcmp(tail, "splash")) {
      return About_send_splash(Info, Url);
   }

   if (!strcmp(tail, "jwz"))
      loc = "http://www.jwz.org/";
   else if (!strcmp(tail, "raph"))
      loc = "http://www.levien.com/";
   else if (!strcmp(tail, "yosh"))
      loc = "http://yosh.gimp.org/";
   else if (!strcmp(tail, "snorfle"))
      loc = "http://www.snorfle.net/";
   else if (!strcmp(tail, "dillo"))
      loc = "http://www.dillo.org/";
   else if (!strcmp(tail, "help"))
      loc = "http://www.dillo.org/dillo-help.html";
   else
      loc = "http://www.google.com/";

   LocUrl = a_Url_new(loc, NULL, 0, 0, 0);
   a_Nav_push(web->bw, LocUrl);
   a_Url_free(LocUrl);
   return -1;
}

/*
 * CCC function for the ABOUT module
 */
void a_About_ccc(int Op, int Branch, int Dir, ChainLink *Info,
                 void *Data1, void *Data2)
{
   int FD;

   a_Chain_debug_msg("a_About_ccc", Op, Branch, Dir);

   if ( Branch == 1 ) {
      /* Start about method */
      if (Dir == BCK) {
         switch (Op) {
         case OpStart:
            /* (Data1 = Url;  Data2 = Web) */
            // Info->LocalKey gets set in About_get
            if ((FD = About_get(Info, Data1, Data2)) == -1)
               a_Chain_fcb(OpAbort, Info, NULL, NULL);
            break;
         case OpAbort:
            a_Chain_bcb(OpAbort, Info, NULL, NULL);
            g_free(Info->LocalKey);
            g_free(Info);
            break;
         }
      } else {  /* FWD */
         switch (Op) {
         case OpSend:
            /* This means the sending framework was set OK */
            FD = ((SplashInfo_t *)Info->LocalKey)->FD_Read;
            a_Chain_fcb(OpSend, Info, &FD, NULL);
            break;
         case OpEnd:
            /* Everything sent! */
            a_Chain_del_link(Info, BCK);
            g_free(Info->LocalKey);
            a_Chain_fcb(OpEnd, Info, NULL, NULL);
            break;
         case OpAbort:
            g_free(Info->LocalKey);
            a_Chain_fcb(OpAbort, Info, NULL, NULL);
            break;
         }
      }
   }
}

