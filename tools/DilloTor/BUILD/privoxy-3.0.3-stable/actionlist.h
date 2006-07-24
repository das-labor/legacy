/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/actionlist.h,v $
 *
 * Purpose     :  Master list of supported actions.
 *                Not really a header, since it generates code.
 *                This is included (3 times!) from actions.c
 *                Each time, the following macros are defined to
 *                suitable values beforehand:
 *                    DEFINE_ACTION_MULTI()
 *                    DEFINE_ACTION_STRING()
 *                    DEFINE_ACTION_BOOL()
 *                    DEFINE_ACTION_ALIAS
 *
 * Copyright   :  Written by and Copyright (C) 2001 the SourceForge
 *                Privoxy team. http://www.privoxy.org/
 *
 *                Based on the Internet Junkbuster originally written
 *                by and Copyright (C) 1997 Anonymous Coders and 
 *                Junkbusters Corporation.  http://www.junkbusters.com
 *
 *                This program is free software; you can redistribute it 
 *                and/or modify it under the terms of the GNU General
 *                Public License as published by the Free Software
 *                Foundation; either version 2 of the License, or (at
 *                your option) any later version.
 *
 *                This program is distributed in the hope that it will
 *                be useful, but WITHOUT ANY WARRANTY; without even the
 *                implied warranty of MERCHANTABILITY or FITNESS FOR A
 *                PARTICULAR PURPOSE.  See the GNU General Public
 *                License for more details.
 *
 *                The GNU General Public License should be included with
 *                this file.  If not, you can view it at
 *                http://www.gnu.org/copyleft/gpl.html
 *                or write to the Free Software Foundation, Inc., 59
 *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Revisions   :
 *    $Log: actionlist.h,v $
 *    Revision 1.17.2.2  2002/09/25 15:25:25  oes
 *    Added more aliases for prehistoric action names
 *
 *    Revision 1.17.2.1  2002/08/02 12:50:47  oes
 *    Consistency with docs: Change default name for action from hide-referer to hide-referrer
 *
 *    Revision 1.17  2002/05/14 21:25:55  oes
 *    Renamed prevent-(setting/reading)-cookies to crunch-(incoming/outgoing)-cookies
 *
 *    Revision 1.16  2002/04/24 02:15:18  oes
 *    Renamed actions as discussed, Aliased old action names to new ones.
 *
 *    Revision 1.15  2002/03/26 22:29:54  swa
 *    we have a new homepage!
 *
 *    Revision 1.14  2002/03/24 16:32:08  jongfoster
 *    Removing logo option
 *
 *    Revision 1.13  2002/03/24 15:23:33  jongfoster
 *    Name changes
 *
 *    Revision 1.12  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.11  2002/03/12 01:42:49  oes
 *    Introduced modular filters
 *
 *    Revision 1.10  2002/03/08 18:19:14  jongfoster
 *    Adding +image-blocker{pattern} option to edit interface
 *
 *    Revision 1.9  2001/11/22 21:58:41  jongfoster
 *    Adding action +no-cookies-keep
 *
 *    Revision 1.8  2001/10/10 16:42:52  oes
 *    Fixed a bug, Added +limit-connect string action
 *
 *    Revision 1.7  2001/10/07 15:33:59  oes
 *    Introduced a +no-compression action
 *    Introduced a +downgrade action
 *
 *    Revision 1.6  2001/09/16 15:47:37  jongfoster
 *    First version of CGI-based edit interface.  This is very much a
 *    work-in-progress, and you can't actually use it to edit anything
 *    yet.  You must #define FEATURE_CGI_EDIT_ACTIONS for these changes
 *    to have any effect.
 *
 *    Revision 1.5  2001/07/18 12:27:03  oes
 *    Changed deanimate-gifs to string action
 *
 *    Revision 1.4  2001/07/13 13:52:12  oes
 *     - Formatting
 *     - Introduced new action ACTION_DEANIMATE
 *
 *    Revision 1.3  2001/06/07 23:03:56  jongfoster
 *    Added standard comment at top of file.
 *
 *
 *********************************************************************/


#if !(defined(DEFINE_ACTION_BOOL) && defined(DEFINE_ACTION_MULTI) && defined(DEFINE_ACTION_STRING))
#error Please define lots of macros before including "actionlist.h".
#endif /* !defined(all the DEFINE_ACTION_xxx macros) */

#ifndef DEFINE_CGI_PARAM_RADIO
#define DEFINE_CGI_PARAM_RADIO(name, bit, index, value, is_default)
#define DEFINE_CGI_PARAM_CUSTOM(name, bit, index, default_val)
#define DEFINE_CGI_PARAM_NO_RADIO(name, bit, index, default_val)
#endif /* ndef DEFINE_CGI_PARAM_RADIO */

DEFINE_ACTION_MULTI      ("add-header",                 ACTION_MULTI_ADD_HEADER)
DEFINE_ACTION_BOOL       ("block",                      ACTION_BLOCK)
DEFINE_ACTION_BOOL       ("crunch-incoming-cookies",    ACTION_NO_COOKIE_SET)
DEFINE_ACTION_BOOL       ("crunch-outgoing-cookies",    ACTION_NO_COOKIE_READ)
DEFINE_ACTION_STRING     ("deanimate-gifs",             ACTION_DEANIMATE,       ACTION_STRING_DEANIMATE)
DEFINE_CGI_PARAM_RADIO   ("deanimate-gifs",             ACTION_DEANIMATE,       ACTION_STRING_DEANIMATE,     "first", 0)
DEFINE_CGI_PARAM_RADIO   ("deanimate-gifs",             ACTION_DEANIMATE,       ACTION_STRING_DEANIMATE,     "last",  1)
DEFINE_ACTION_BOOL       ("downgrade-http-version",     ACTION_DOWNGRADE)
DEFINE_ACTION_BOOL       ("fast-redirects",             ACTION_FAST_REDIRECTS)
DEFINE_ACTION_MULTI      ("filter",                     ACTION_MULTI_FILTER)
DEFINE_ACTION_BOOL       ("handle-as-image",            ACTION_IMAGE)
DEFINE_ACTION_BOOL       ("hide-forwarded-for-headers", ACTION_HIDE_FORWARDED)
DEFINE_ACTION_STRING     ("hide-from-header",           ACTION_HIDE_FROM,       ACTION_STRING_FROM)
DEFINE_CGI_PARAM_RADIO   ("hide-from-header",           ACTION_HIDE_FROM,       ACTION_STRING_FROM,          "block", 1)
DEFINE_CGI_PARAM_CUSTOM  ("hide-from-header",           ACTION_HIDE_FROM,       ACTION_STRING_FROM,          "spam_me_senseless@sittingduck.xyz")
DEFINE_ACTION_STRING     ("hide-referrer",              ACTION_HIDE_REFERER,    ACTION_STRING_REFERER)
DEFINE_CGI_PARAM_RADIO   ("hide-referrer",              ACTION_HIDE_REFERER,    ACTION_STRING_REFERER,       "forge", 1)
DEFINE_CGI_PARAM_RADIO   ("hide-referrer",              ACTION_HIDE_REFERER,    ACTION_STRING_REFERER,       "block", 0)
DEFINE_CGI_PARAM_CUSTOM  ("hide-referrer",              ACTION_HIDE_REFERER,    ACTION_STRING_REFERER,       "http://www.google.com/")
DEFINE_ACTION_STRING     ("hide-user-agent",            ACTION_HIDE_USER_AGENT, ACTION_STRING_USER_AGENT)
DEFINE_CGI_PARAM_NO_RADIO("hide-user-agent",            ACTION_HIDE_USER_AGENT, ACTION_STRING_USER_AGENT,    "Privoxy/3.0 (Anonymous)")
DEFINE_ACTION_BOOL       ("kill-popups",                ACTION_NO_POPUPS)
DEFINE_ACTION_STRING     ("limit-connect",              ACTION_LIMIT_CONNECT,   ACTION_STRING_LIMIT_CONNECT)
DEFINE_CGI_PARAM_NO_RADIO("limit-connect",              ACTION_LIMIT_CONNECT,   ACTION_STRING_LIMIT_CONNECT,  "443")
DEFINE_ACTION_BOOL       ("prevent-compression",        ACTION_NO_COMPRESSION)
DEFINE_ACTION_BOOL       ("send-vanilla-wafer",         ACTION_VANILLA_WAFER)
DEFINE_ACTION_MULTI      ("send-wafer",                 ACTION_MULTI_WAFER)
DEFINE_ACTION_BOOL       ("session-cookies-only",       ACTION_NO_COOKIE_KEEP)
DEFINE_ACTION_STRING     ("set-image-blocker",          ACTION_IMAGE_BLOCKER,   ACTION_STRING_IMAGE_BLOCKER)
DEFINE_CGI_PARAM_RADIO   ("set-image-blocker",          ACTION_IMAGE_BLOCKER,   ACTION_STRING_IMAGE_BLOCKER, "pattern", 1)
DEFINE_CGI_PARAM_RADIO   ("set-image-blocker",          ACTION_IMAGE_BLOCKER,   ACTION_STRING_IMAGE_BLOCKER, "blank", 0)
DEFINE_CGI_PARAM_CUSTOM  ("set-image-blocker",          ACTION_IMAGE_BLOCKER,   ACTION_STRING_IMAGE_BLOCKER,  CGI_PREFIX "show-banner?type=pattern")


#if DEFINE_ACTION_ALIAS

/* 
 * Alternative spellings
 */
DEFINE_ACTION_BOOL       ("kill-popup",      ACTION_NO_POPUPS)
DEFINE_ACTION_STRING     ("hide-referer",   ACTION_HIDE_REFERER,    ACTION_STRING_REFERER)
DEFINE_ACTION_BOOL       ("prevent-keeping-cookies", ACTION_NO_COOKIE_KEEP)

/* 
 * Pre-3.0 compatibility
 */
DEFINE_ACTION_BOOL       ("no-cookie-read",          ACTION_NO_COOKIE_READ)
DEFINE_ACTION_BOOL       ("no-cookie-set",           ACTION_NO_COOKIE_SET)
DEFINE_ACTION_BOOL       ("prevent-reading-cookies", ACTION_NO_COOKIE_READ)
DEFINE_ACTION_BOOL       ("prevent-setting-cookies", ACTION_NO_COOKIE_SET)
DEFINE_ACTION_BOOL       ("downgrade",               ACTION_DOWNGRADE)
DEFINE_ACTION_BOOL       ("hide-forwarded",          ACTION_HIDE_FORWARDED)
DEFINE_ACTION_STRING     ("hide-from",               ACTION_HIDE_FROM,       ACTION_STRING_FROM)
DEFINE_ACTION_BOOL       ("image",                   ACTION_IMAGE)
DEFINE_ACTION_STRING     ("image-blocker",           ACTION_IMAGE_BLOCKER,   ACTION_STRING_IMAGE_BLOCKER)
DEFINE_ACTION_BOOL       ("no-compression",          ACTION_NO_COMPRESSION)
DEFINE_ACTION_BOOL       ("no-cookies-keep",         ACTION_NO_COOKIE_KEEP)
DEFINE_ACTION_BOOL       ("no-cookies-read",         ACTION_NO_COOKIE_READ)
DEFINE_ACTION_BOOL       ("no-cookies-set",          ACTION_NO_COOKIE_SET)
DEFINE_ACTION_BOOL       ("no-popups",               ACTION_NO_POPUPS)
DEFINE_ACTION_BOOL       ("vanilla-wafer",           ACTION_VANILLA_WAFER)
DEFINE_ACTION_MULTI      ("wafer",                   ACTION_MULTI_WAFER)
#endif /* if DEFINE_ACTION_ALIAS */

#undef DEFINE_ACTION_MULTI
#undef DEFINE_ACTION_STRING
#undef DEFINE_ACTION_BOOL
#undef DEFINE_ACTION_ALIAS
#undef DEFINE_CGI_PARAM_CUSTOM
#undef DEFINE_CGI_PARAM_RADIO
#undef DEFINE_CGI_PARAM_NO_RADIO

