#ifndef MISCUTIL_H_INCLUDED
#define MISCUTIL_H_INCLUDED
#define MISCUTIL_H_VERSION "$Id: miscutil.h,v 1.21 2002/04/26 12:55:38 oes Exp $"
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/miscutil.h,v $
 *
 * Purpose     :  zalloc, hash_string, safe_strerror, strcmpic,
 *                strncmpic, and MinGW32 strdup functions.  These are
 *                each too small to deserve their own file but don't 
 *                really fit in any other file.
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
 *    $Log: miscutil.h,v $
 *    Revision 1.21  2002/04/26 12:55:38  oes
 *    New function string_toupper
 *
 *    Revision 1.20  2002/03/26 22:29:55  swa
 *    we have a new homepage!
 *
 *    Revision 1.19  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.18  2002/03/07 03:46:17  oes
 *    Fixed compiler warnings
 *
 *    Revision 1.17  2002/03/04 18:28:32  oes
 *    Deleted deletePidFile, played syleguide police
 *
 *    Revision 1.16  2002/01/21 00:53:36  jongfoster
 *    Adding string_join()
 *
 *    Revision 1.15  2001/12/30 14:07:32  steudten
 *    - Add signal handling (unix)
 *    - Add SIGHUP handler (unix)
 *    - Add creation of pidfile (unix)
 *    - Add action 'top' in rc file (RH)
 *    - Add entry 'SIGNALS' to manpage
 *    - Add exit message to logfile (unix)
 *
 *    Revision 1.14  2001/11/05 21:43:48  steudten
 *    Add global var 'basedir' for unix os.
 *
 *    Revision 1.13  2001/10/29 03:48:10  david__schmidt
 *    OS/2 native needed a snprintf() routine.  Added one to miscutil, brackedted
 *    by and __OS2__ ifdef.
 *
 *    Revision 1.12  2001/10/23 21:27:50  jongfoster
 *    Standardising error codes in string_append
 *    make_path() no longer adds '\\' if the dir already ends in '\\' (this
 *    is just copying a UNIX-specific fix to the Windows-specific part)
 *
 *    Revision 1.11  2001/10/14 22:02:57  jongfoster
 *    New function string_append() which is like strsav(), but running
 *    out of memory isn't automatically FATAL.
 *
 *    Revision 1.10  2001/09/20 13:34:09  steudten
 *
 *    change long to int for prototype hash_string()
 *
 *    Revision 1.9  2001/07/29 18:43:08  jongfoster
 *    Changing #ifdef _FILENAME_H to FILENAME_H_INCLUDED, to conform to
 *    ANSI C rules.
 *
 *    Revision 1.8  2001/06/29 13:32:14  oes
 *    Removed logentry from cancelled commit
 *
 *    Revision 1.7  2001/06/05 22:32:01  jongfoster
 *    New function make_path() to splice directory and file names together.
 *
 *    Revision 1.6  2001/06/03 19:12:30  oes
 *    introduced bindup()
 *
 *    Revision 1.5  2001/06/01 10:31:51  oes
 *    Added character class matching to trivimatch; renamed to simplematch
 *
 *    Revision 1.4  2001/05/31 17:32:31  oes
 *
 *     - Enhanced domain part globbing with infix and prefix asterisk
 *       matching and optional unanchored operation
 *
 *    Revision 1.3  2001/05/29 23:10:09  oes
 *
 *
 *     - Introduced chomp()
 *     - Moved strsav() from showargs to miscutil
 *
 *    Revision 1.2  2001/05/29 09:50:24  jongfoster
 *    Unified blocklist/imagelist/permissionslist.
 *    File format is still under discussion, but the internal changes
 *    are (mostly) done.
 *
 *    Also modified interceptor behaviour:
 *    - We now intercept all URLs beginning with one of the following
 *      prefixes (and *only* these prefixes):
 *        * http://i.j.b/
 *        * http://ijbswa.sf.net/config/
 *        * http://ijbswa.sourceforge.net/config/
 *    - New interceptors "home page" - go to http://i.j.b/ to see it.
 *    - Internal changes so that intercepted and fast redirect pages
 *      are not replaced with an image.
 *    - Interceptors now have the option to send a binary page direct
 *      to the client. (i.e. ijb-send-banner uses this)
 *    - Implemented show-url-info interceptor.  (Which is why I needed
 *      the above interceptors changes - a typical URL is
 *      "http://i.j.b/show-url-info?url=www.somesite.com/banner.gif".
 *      The previous mechanism would not have intercepted that, and
 *      if it had been intercepted then it then it would have replaced
 *      it with an image.)
 *
 *    Revision 1.1.1.1  2001/05/15 13:59:00  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#include "project.h"

#if defined(__cplusplus)
extern "C" {
#endif

extern const char *basedir;
extern void *zalloc(size_t size);

#if defined(unix)
extern void write_pid_file(void);
#endif /* unix */

extern unsigned int hash_string(const char* s);

extern char *safe_strerror(int err);

extern int strcmpic(const char *s1, const char *s2);
extern int strncmpic(const char *s1, const char *s2, size_t n);

extern char *strsav(char *old, const char *text_to_append);
extern jb_err string_append(char **target_string, const char *text_to_append);
extern jb_err string_join  (char **target_string,       char *text_to_append);

extern char *string_toupper(const char *string);
extern char *chomp(char *string);
extern int simplematch(char *pattern, char *text);

extern char *bindup(const char *string, size_t len);

extern char *make_path(const char * dir, const char * file);

#ifdef __MINGW32__
extern char *strdup(const char *s);
#endif /* def __MINGW32__ */

#ifdef __OS2__
extern int snprintf(char *, size_t, const char *, /*args*/ ...);
#endif /* def __OS2__ */

/* Revision control strings from this header and associated .c file */
extern const char miscutil_rcs[];
extern const char miscutil_h_rcs[];

#if defined(__cplusplus)
}
#endif

#endif /* ndef MISCUTIL_H_INCLUDED */

/*
  Local Variables:
  tab-width: 3
  end:
*/
