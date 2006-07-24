#ifndef JCC_H_INCLUDED
#define JCC_H_INCLUDED
#define JCC_H_VERSION "$Id: jcc.h,v 1.12.2.1 2003/03/07 03:41:05 david__schmidt Exp $"
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/jcc.h,v $
 *
 * Purpose     :  Main file.  Contains main() method, main loop, and 
 *                the main connection-handling function.
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
 *    $Log: jcc.h,v $
 *    Revision 1.12.2.1  2003/03/07 03:41:05  david__schmidt
 *    Wrapping all *_r functions (the non-_r versions of them) with mutex semaphores for OSX.  Hopefully this will take care of all of those pesky crash reports.
 *
 *    Revision 1.12  2002/03/26 22:29:55  swa
 *    we have a new homepage!
 *
 *    Revision 1.11  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.10  2002/03/16 23:54:06  jongfoster
 *    Adding graceful termination feature, to help look for memory leaks.
 *    If you enable this (which, by design, has to be done by hand
 *    editing config.h) and then go to http://i.j.b/die, then the program
 *    will exit cleanly after the *next* request.  It should free all the
 *    memory that was used.
 *
 *    Revision 1.9  2002/03/07 03:52:44  oes
 *    Set logging to tty for --no-daemon mode
 *
 *    Revision 1.8  2002/03/04 18:19:49  oes
 *    Added extern const char *pidfile
 *
 *    Revision 1.7  2001/11/05 21:41:43  steudten
 *    Add changes to be a real daemon just for unix os.
 *    (change cwd to /, detach from controlling tty, set
 *    process group and session leader to the own process.
 *    Add DBG() Macro.
 *    Add some fatal-error log message for failed malloc().
 *    Add '-d' if compiled with 'configure --with-debug' to
 *    enable debug output.
 *
 *    Revision 1.6  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.5  2001/07/29 19:32:00  jongfoster
 *    Renaming _main() [mingw32 only] to real_main(), for ANSI compliance.
 *
 *    Revision 1.4  2001/07/29 18:58:15  jongfoster
 *    Removing nested #includes, adding forward declarations for needed
 *    structures, and changing the #define _FILENAME_H to FILENAME_H_INCLUDED.
 *
 *    Revision 1.3  2001/07/18 12:31:58  oes
 *    moved #define freez from jcc.h to project.h
 *
 *    Revision 1.2  2001/05/31 21:24:47  jongfoster
 *    Changed "permission" to "action" throughout.
 *    Removed DEFAULT_USER_AGENT - it must now be specified manually.
 *    Moved vanilla wafer check into chat(), since we must now
 *    decide whether or not to add it based on the URL.
 *
 *    Revision 1.1.1.1  2001/05/15 13:58:56  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

struct client_state;
struct file_list;

/* Global variables */

#ifdef FEATURE_STATISTICS
extern int urls_read;
extern int urls_rejected;
#endif /*def FEATURE_STATISTICS*/

extern struct client_state clients[];
extern struct file_list    files[];

#ifdef unix
extern const char *pidfile;
#endif
extern int no_daemon;

#ifdef FEATURE_GRACEFUL_TERMINATION
extern int g_terminate;
#endif

#ifdef OSX_DARWIN
extern pthread_mutex_t gmtime_mutex;
extern pthread_mutex_t localtime_mutex;
extern pthread_mutex_t gethostbyaddr_mutex;
extern pthread_mutex_t gethostbyname_mutex;
#endif /* def OSX_DARWIN */

/* Functions */

#ifdef __MINGW32__
int real_main(int argc, const char *argv[]);
#else
int main(int argc, const char *argv[]);
#endif

/* Revision control strings from this header and associated .c file */
extern const char jcc_rcs[];
extern const char jcc_h_rcs[];

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ndef JCC_H_INCLUDED */

/*
  Local Variables:
  tab-width: 3
  end:
*/
