#ifndef PROJECT_H_INCLUDED
#define PROJECT_H_INCLUDED
/** Version string. */
#define PROJECT_H_VERSION "$Id: project.h,v 1.72.2.5 2004/01/30 15:29:29 oes Exp $"
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/project.h,v $
 *
 * Purpose     :  Defines data structures which are widely used in the
 *                project.  Does not define any variables or functions
 *                (though it does declare some macros).
 *
 * Copyright   :  Written by and Copyright (C) 2001 - 2004 the SourceForge
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
 *    $Log: project.h,v $
 *    Revision 1.72.2.5  2004/01/30 15:29:29  oes
 *    Updated the copyright note
 *
 *    Revision 1.72.2.4  2004/01/13 16:12:14  oes
 *    Fixed double slash in USER_MANUAL_URL. Closes BR #867088.
 *
 *    Revision 1.72.2.3  2003/03/11 11:54:37  oes
 *    Introduced RC_FLAG_* flags for use in child process return code
 *
 *    Revision 1.72.2.2  2002/11/28 18:15:44  oes
 *    Added flag to each cgi_dispatcher that allows or denies
 *    external linking and removed const qualifier from
 *    struct list_entry.str.
 *
 *    Revision 1.72.2.1  2002/08/10 11:25:18  oes
 *    - Include config.h for access to config data
 *    - Include <pcre*.h> depending on where they are
 *
 *    Revision 1.72  2002/05/14 21:35:49  oes
 *    Split HELP_LINK_PREFIX into ACTIONS_HELP_PREFIX and CONFIG_HELP_PREFIX
 *    because of split in user-manual
 *
 *    Revision 1.71  2002/05/12 21:39:36  jongfoster
 *    - Adding Doxygen-style comments to structures and #defines.
 *
 *    Revision 1.70  2002/05/12 16:05:50  jongfoster
 *    Fixing ACTION_MASK_ALL to be unsigned long rather than
 *    just unsigned int.  I don't know if anyone is porting
 *    Privoxy to 16-bit platforms, but if so, +limit-connect
 *    wouldn't have worked because of this bug.
 *
 *    Revision 1.69  2002/05/08 16:00:16  oes
 *    Added size member to struct iob, so it can
 *    be alloced larger than needed.
 *
 *    Revision 1.68  2002/04/26 12:56:00  oes
 *    Killed REDIRECT_URL, added USER_MANUAL_URL and HELP_LINK_PREFIX
 *
 *    Revision 1.67  2002/04/24 02:12:43  oes
 *     - Jon's multiple AF patch:
 *       - Make csp->actions_list an array
 *       - #define MAX_ACTION_FILES
 *     - Moved CGI_PARAM_LEN_MAX (500) here
 *
 *    Revision 1.66  2002/04/15 19:06:43  jongfoster
 *    Typos
 *
 *    Revision 1.65  2002/04/04 00:36:36  gliptak
 *    always use pcre for matching
 *
 *    Revision 1.64  2002/04/03 22:28:03  gliptak
 *    Removed references to gnu_regex
 *
 *    Revision 1.63  2002/03/31 17:19:00  jongfoster
 *    Win32 only: Enabling STRICT to fix a VC++ compile warning.
 *
 *    Revision 1.62  2002/03/26 22:48:49  swa
 *    new homepage url
 *
 *    Revision 1.61  2002/03/26 22:29:55  swa
 *    we have a new homepage!
 *
 *    Revision 1.60  2002/03/24 15:52:17  jongfoster
 *    Changing CGI URL prefixes for new name
 *
 *    Revision 1.59  2002/03/24 15:23:33  jongfoster
 *    Name changes
 *
 *    Revision 1.58  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.57  2002/03/16 20:28:34  oes
 *    Added descriptions to the filters so users will know what they select in the cgi editor
 *
 *    Revision 1.56  2002/03/13 20:27:30  oes
 *    Fixing bug with CT_TABOO
 *
 *    Revision 1.55  2002/03/12 01:42:50  oes
 *    Introduced modular filters
 *
 *    Revision 1.54  2002/03/09 20:03:52  jongfoster
 *    - Making various functions return int rather than size_t.
 *      (Undoing a recent change).  Since size_t is unsigned on
 *      Windows, functions like read_socket that return -1 on
 *      error cannot return a size_t.
 *
 *      THIS WAS A MAJOR BUG - it caused frequent, unpredictable
 *      crashes, and also frequently caused JB to jump to 100%
 *      CPU and stay there.  (Because it thought it had just
 *      read ((unsigned)-1) == 4Gb of data...)
 *
 *    - The signature of write_socket has changed, it now simply
 *      returns success=0/failure=nonzero.
 *
 *    - Trying to get rid of a few warnings --with-debug on
 *      Windows, I've introduced a new type "jb_socket".  This is
 *      used for the socket file descriptors.  On Windows, this
 *      is SOCKET (a typedef for unsigned).  Everywhere else, it's
 *      an int.  The error value can't be -1 any more, so it's
 *      now JB_INVALID_SOCKET (which is -1 on UNIX, and in
 *      Windows it maps to the #define INVALID_SOCKET.)
 *
 *    - The signature of bind_port has changed.
 *
 *    Revision 1.53  2002/03/08 16:48:55  oes
 *    Added FEATURE_NO_GIFS and BUILTIN_IMAGE_MIMETYPE
 *
 *    Revision 1.52  2002/03/07 03:46:17  oes
 *    Fixed compiler warnings
 *
 *    Revision 1.51  2002/03/05 04:52:42  oes
 *    Deleted non-errlog debugging code
 *
 *    Revision 1.50  2002/03/04 19:32:07  oes
 *    Changed default port to 8118
 *
 *    Revision 1.49  2002/03/04 18:28:55  oes
 *    Deleted PID_FILE_NAME
 *
 *    Revision 1.48  2002/03/03 14:50:40  oes
 *    Fixed CLF logging: Added ocmd member for client's request to struct http_request
 *
 *    Revision 1.47  2002/02/20 23:15:13  jongfoster
 *    Parsing functions now handle out-of-memory gracefully by returning
 *    an error code.
 *
 *    Revision 1.46  2002/01/17 21:06:09  jongfoster
 *    Now #defining the URLs of the config interface
 *
 *    Minor changes to struct http_request and struct url_spec due to
 *    standardizing that struct http_request is used to represent a URL, and
 *    struct url_spec is used to represent a URL pattern.  (Before, URLs were
 *    represented as seperate variables and a partially-filled-in url_spec).
 *
 *    Revision 1.45  2002/01/09 14:33:27  oes
 *    Added HOSTENT_BUFFER_SIZE
 *
 *    Revision 1.44  2001/12/30 14:07:32  steudten
 *    - Add signal handling (unix)
 *    - Add SIGHUP handler (unix)
 *    - Add creation of pidfile (unix)
 *    - Add action 'top' in rc file (RH)
 *    - Add entry 'SIGNALS' to manpage
 *    - Add exit message to logfile (unix)
 *
 *    Revision 1.43  2001/11/22 21:57:51  jongfoster
 *    Making action_spec->flags into an unsigned long rather than just an
 *    unsigned int.
 *    Adding ACTION_NO_COOKIE_KEEP
 *
 *    Revision 1.42  2001/11/05 21:42:41  steudten
 *    Include DBG() macro.
 *
 *    Revision 1.41  2001/10/28 19:12:06  jongfoster
 *    Adding ijb_toupper()
 *
 *    Revision 1.40  2001/10/26 17:40:47  oes
 *    Moved ijb_isspace and ijb_tolower to project.h
 *    Removed http->user_agent, csp->referrer and csp->accept_types
 *
 *    Revision 1.39  2001/10/25 03:45:02  david__schmidt
 *    Adding a (void*) cast to freez() because Visual Age C++ won't expand the
 *    macro when called with a cast; so moving the cast to the macro def'n
 *    seems to both eliminate compiler warnings (on darwin and OS/2, anyway) and
 *    doesn't make macro expansion complain.  Hope this works for everyone else
 *    too...
 *
 *    Revision 1.38  2001/10/23 21:19:04  jongfoster
 *    New error-handling support: jb_err type and JB_ERR_xxx constants
 *    CGI functions now return a jb_err, and their parameters map is const.
 *    Support for RUNTIME_FEATUREs to enable/disable config editor
 *    Adding a few comments
 *
 *    Revision 1.37  2001/10/14 22:14:01  jongfoster
 *    Removing name_length field from struct cgi_dispatcher, as this is
 *    now calculated at runtime from the "name" field.
 *
 *    Revision 1.36  2001/10/10 16:45:15  oes
 *    Added LIMIT_CONNECT action and string
 *    Fixed HTTP message line termination
 *    Added CFORBIDDEN HTTP message
 *
 *    Revision 1.35  2001/10/07 18:06:43  oes
 *    Added status member to struct http_request
 *
 *    Revision 1.34  2001/10/07 15:45:25  oes
 *    Added url member to struct http_request and commented all
 *      members
 *
 *    Added CT_TABOO
 *
 *    Added ACTION_DOWNGRADE and ACTION_NO_COMPRESSION
 *
 *    Replaced struct client_state members rejected,
 *      force, active and toggled_on with "flags" bitmap.
 *
 *    Added CSP_FLAG_MODIFIED and CSP_FLAG_CHUNKED
 *
 *    Added buffer_limit to struct configuration_spec
 *
 *    Revision 1.33  2001/09/20 13:30:08  steudten
 *
 *    Make freez() more secure in case of: if (exp) { free(z) ; a=*z }
 *    Last case will set z to NULL in free(z) and thats bad..
 *
 *    Revision 1.32  2001/09/16 23:02:51  jongfoster
 *    Fixing warning
 *
 *    Revision 1.31  2001/09/16 13:20:29  jongfoster
 *    Rewrite of list library.  Now has seperate header and list_entry
 *    structures.  Also added a large sprinking of assert()s to the list
 *    code.
 *
 *    Revision 1.30  2001/09/13 23:52:00  jongfoster
 *    Support for both static and dynamically generated CGI pages
 *
 *    Revision 1.29  2001/09/13 23:29:43  jongfoster
 *    Defining FORWARD_SPEC_INITIALIZER
 *
 *    Revision 1.28  2001/09/13 23:05:50  jongfoster
 *    Changing the string paramater to the header parsers a "const".
 *
 *    Revision 1.27  2001/08/05 16:06:20  jongfoster
 *    Modifiying "struct map" so that there are now separate header and
 *    "map_entry" structures.  This means that functions which modify a
 *    map no longer need to return a pointer to the modified map.
 *    Also, it no longer reverses the order of the entries (which may be
 *    important with some advanced template substitutions).
 *
 *    Revision 1.26  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.25  2001/07/29 18:43:08  jongfoster
 *    Changing #ifdef _FILENAME_H to FILENAME_H_INCLUDED, to conform to
 *    ANSI C rules.
 *
 *    Revision 1.24  2001/07/25 17:20:27  oes
 *    Introduced http->user_agent
 *
 *    Revision 1.23  2001/07/18 12:32:23  oes
 *    - Added ACTION_STRING_DEANIMATE
 *    - moved #define freez from jcc.h to project.h
 *
 *    Revision 1.22  2001/07/15 17:51:41  jongfoster
 *    Renaming #define STATIC to STATIC_PCRE
 *
 *    Revision 1.21  2001/07/13 14:03:19  oes
 *     - Reorganized regex header inclusion and #defines to
 *       comply to the scheme in configure.in
 *     - Added csp->content_type and its CT_* keys
 *     - Added ACTION_DEANIMATE
 *     - Removed all #ifdef PCRS
 *
 *    Revision 1.20  2001/06/29 21:45:41  oes
 *    Indentation, CRLF->LF, Tab-> Space
 *
 *    Revision 1.19  2001/06/29 13:33:36  oes
 *    - Improved comments
 *    - Introduced http_request.host_ip_addr_str
 *    - Introduced http_response.head_length
 *    - Introduced config.my_ip_addr_str, config.my_hostname,
 *      config.admin_address and config.proxy_info_url
 *    - Removed config.proxy_args_header and config.proxy_args_trailer,
 *      renamed config.proxy_args_invocation to config.proxy_args
 *    - Removed HTML snipplets and GIFs
 *    - Removed logentry from cancelled commit
 *
 *    Revision 1.18  2001/06/09 10:57:39  jongfoster
 *    Adding definition of BUFFER_SIZE.
 *    Changing struct cgi_dispatcher to use "const" strings.
 *
 *    Revision 1.17  2001/06/07 23:15:09  jongfoster
 *    Merging ACL and forward files into config file.
 *    Moving struct gateway members into struct forward_spec
 *    Removing config->proxy_args_gateways
 *    Cosmetic: Adding a few comments
 *
 *    Revision 1.16  2001/06/04 18:31:58  swa
 *    files are now prefixed with either `confdir' or `logdir'.
 *    `make redhat-dist' replaces both entries confdir and logdir
 *    with redhat values
 *
 *    Revision 1.15  2001/06/04 11:28:53  swa
 *    redirect did not work due to missing /
 *
 *    Revision 1.14  2001/06/03 11:03:48  oes
 *    Added struct map,
 *    added struct http_response,
 *    changed struct interceptors to struct cgi_dispatcher,
 *    moved HTML stuff to cgi.h
 *
 *    Revision 1.13  2001/06/01 20:05:36  jongfoster
 *    Support for +image-blocker{}: added ACTION_IMAGE_BLOCKER
 *    constant, and removed csp->tinygif.
 *
 *    Revision 1.12  2001/06/01 18:49:17  jongfoster
 *    Replaced "list_share" with "list" - the tiny memory gain was not
 *    worth the extra complexity.
 *
 *    Revision 1.11  2001/06/01 10:32:47  oes
 *    Added constants for anchoring selection bitmap
 *
 *    Revision 1.10  2001/05/31 21:33:53  jongfoster
 *    Changes for new actions file, replacing permissionsfile
 *    and parts of the config file.  Also added support for
 *    list_shared.
 *
 *    Revision 1.9  2001/05/31 17:32:31  oes
 *
 *     - Enhanced domain part globbing with infix and prefix asterisk
 *       matching and optional unanchored operation
 *
 *    Revision 1.8  2001/05/29 20:09:15  joergs
 *    HTTP_REDIRECT_TEMPLATE fixed.
 *
 *    Revision 1.7  2001/05/29 09:50:24  jongfoster
 *    Unified blocklist/imagelist/actionslist.
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
 *    Revision 1.6  2001/05/27 22:17:04  oes
 *
 *    - re_process_buffer no longer writes the modified buffer
 *      to the client, which was very ugly. It now returns the
 *      buffer, which it is then written by chat.
 *
 *    - content_length now adjusts the Content-Length: header
 *      for modified documents rather than crunch()ing it.
 *      (Length info in csp->content_length, which is 0 for
 *      unmodified documents)
 *
 *    - For this to work, sed() is called twice when filtering.
 *
 *    Revision 1.5  2001/05/26 00:28:36  jongfoster
 *    Automatic reloading of config file.
 *    Removed obsolete SIGHUP support (Unix) and Reload menu option (Win32).
 *    Most of the global variables have been moved to a new
 *    struct configuration_spec, accessed through csp->config->globalname
 *    Most of the globals remaining are used by the Win32 GUI.
 *
 *    Revision 1.4  2001/05/22 18:46:04  oes
 *
 *    - Enabled filtering banners by size rather than URL
 *      by adding patterns that replace all standard banner
 *      sizes with the "Junkbuster" gif to the re_filterfile
 *
 *    - Enabled filtering WebBugs by providing a pattern
 *      which kills all 1x1 images
 *
 *    - Added support for PCRE_UNGREEDY behaviour to pcrs,
 *      which is selected by the (nonstandard and therefore
 *      capital) letter 'U' in the option string.
 *      It causes the quantifiers to be ungreedy by default.
 *      Appending a ? turns back to greedy (!).
 *
 *    - Added a new interceptor ijb-send-banner, which
 *      sends back the "Junkbuster" gif. Without imagelist or
 *      MSIE detection support, or if tinygif = 1, or the
 *      URL isn't recognized as an imageurl, a lame HTML
 *      explanation is sent instead.
 *
 *    - Added new feature, which permits blocking remote
 *      script redirects and firing back a local redirect
 *      to the browser.
 *      The feature is conditionally compiled, i.e. it
 *      can be disabled with --disable-fast-redirects,
 *      plus it must be activated by a "fast-redirects"
 *      line in the config file, has its own log level
 *      and of course wants to be displayed by show-proxy-args
 *      Note: Boy, all the #ifdefs in 1001 locations and
 *      all the fumbling with configure.in and acconfig.h
 *      were *way* more work than the feature itself :-(
 *
 *    - Because a generic redirect template was needed for
 *      this, tinygif = 3 now uses the same.
 *
 *    - Moved GIFs, and other static HTTP response templates
 *      to project.h
 *
 *    - Some minor fixes
 *
 *    - Removed some >400 CRs again (Jon, you really worked
 *      a lot! ;-)
 *
 *    Revision 1.3  2001/05/20 01:21:20  jongfoster
 *    Version 2.9.4 checkin.
 *    - Merged popupfile and cookiefile, and added control over PCRS
 *      filtering, in new "actionsfile".
 *    - Implemented LOG_LEVEL_FATAL, so that if there is a configuration
 *      file error you now get a message box (in the Win32 GUI) rather
 *      than the program exiting with no explanation.
 *    - Made killpopup use the PCRS MIME-type checking and HTTP-header
 *      skipping.
 *    - Removed tabs from "config"
 *    - Moved duplicated url parsing code in "loaders.c" to a new funcition.
 *    - Bumped up version number.
 *
 *    Revision 1.2  2001/05/17 23:01:01  oes
 *     - Cleaned CRLF's from the sources and related files
 *
 *    Revision 1.1.1.1  2001/05/15 13:59:03  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


/* Declare struct FILE for vars and funcs. */
#include <stdio.h>

/* Need time_t for file_list */
#include <time.h>
/* Needed for pcre choice */
#include "config.h"

/*
 * Include appropriate regular expression libraries.
 * Note that pcrs and pcre (native) are needed for cgi
 * and are included anyway.
 */

#ifdef STATIC_PCRE
#  include "pcre.h"
#else
#  ifdef PCRE_H_IN_SUBDIR
#    include <pcre/pcre.h>
#  else
#    include <pcre.h>
#  endif
#endif

#ifdef STATIC_PCRS
#  include "pcrs.h"
#else
#  include <pcrs.h>
#endif

#ifdef STATIC_PCRE
#  include "pcreposix.h"
#else
#  ifdef PCRE_H_IN_SUBDIR
#    include <pcre/pcreposix.h>
#  else
#    include <pcreposix.h>
#  endif
#endif

#ifdef AMIGA
#include "amiga.h"
#endif /* def AMIGA */

#ifdef _WIN32
/*
 * I don't want to have to #include all this just for the declaration
 * of SOCKET.  However, it looks like we have to...
 */
#ifndef STRICT
#define STRICT
#endif
#include <windows.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32

typedef SOCKET jb_socket;

#define JB_INVALID_SOCKET INVALID_SOCKET

#else /* ndef _WIN32 */

/**
 * The type used by sockets.  On UNIX it's an int.  Microsoft decided to
 * make it an unsigned.
 */
typedef int jb_socket;

/**
 * The error value used for variables of type jb_socket.  On UNIX this
 * is -1, however Microsoft decided to make socket handles unsigned, so
 * they use a different value.
 */

#define JB_INVALID_SOCKET (-1)

#endif /* ndef _WIN32 */


/**
 * A standard error code.  This should be JB_ERR_OK or one of the JB_ERR_xxx
 * series of errors.
 */
typedef int jb_err;

#define JB_ERR_OK         0 /**< Success, no error                        */
#define JB_ERR_MEMORY     1 /**< Out of memory                            */
#define JB_ERR_CGI_PARAMS 2 /**< Missing or corrupt CGI parameters        */
#define JB_ERR_FILE       3 /**< Error opening, reading or writing a file */
#define JB_ERR_PARSE      4 /**< Error parsing file                       */
#define JB_ERR_MODIFIED   5 /**< File has been modified outside of the  
                                 CGI actions editor.                      */


/**
 * This macro is used to free a pointer that may be NULL.
 * It also sets the variable to NULL after it's been freed.
 * The paramater should be a simple variable without side effects.
 */
#define freez(X)  { if(X) { free((void*)X); X = NULL ; } }


/**
 * Fix a problem with Solaris.  There should be no effect on other
 * platforms.
 *
 * Solaris's isspace() is a macro which uses it's argument directly
 * as an array index.  Therefore we need to make sure that high-bit
 * characters generate +ve values, and ideally we also want to make
 * the argument match the declared parameter type of "int".
 *
 * Note: Remember to #include <ctype.h> if you use these macros.
 */
#define ijb_toupper(__X) toupper((int)(unsigned char)(__X))
#define ijb_tolower(__X) tolower((int)(unsigned char)(__X))
#define ijb_isspace(__X) isspace((int)(unsigned char)(__X))  

/**
 * Use for statically allocated buffers if you have no other choice.
 * Remember to check the length of what you write into the buffer
 * - we don't want any buffer overflows!
 */
#define BUFFER_SIZE 5000

/**
 * Max length of CGI parameters (arbitrary limit).
 */
#define CGI_PARAM_LEN_MAX 500

/**
 * Buffer size for capturing struct hostent data in the
 * gethostby(name|addr)_r library calls. Since we don't
 * loop over gethostbyname_r, the buffer must be sufficient
 * to accomodate multiple IN A RRs, as used in DNS round robin
 * load balancing. W3C's wwwlib uses 1K, so that should be
 * good enough for us, too.
 */
#define HOSTENT_BUFFER_SIZE 1024

/**
 * Do not use.  Originally this was so that you can
 * say "while (FOREVER) { ...do something... }".
 * However, this gives a warning with some compilers (e.g. VC++).
 * Instead, use "for (;;) { ...do something... }".
 */
#define FOREVER 1

/**
 * Default IP address to listen on, as a string.
 * Set to "127.0.0.1".
 */
#define HADDR_DEFAULT   "127.0.0.1"

/**
 * Default port to listen on, as a number.
 * Set to 8118.
 */
#define HADDR_PORT      8118


/* Forward def for struct client_state */
struct configuration_spec;


/**
 * Entry in a linked list of strings.
 */
struct list_entry
{
   /**
    * The string pointer. It must point to a dynamically malloc()ed
    * string or be NULL for the list functions to work. In the latter
    * case, just be careful next time you iterate through the list in
    * your own code.
    */
   char *str;
   
   /** Next entry in the linked list, or NULL if no more. */
   struct list_entry *next;
};

/**
 * A header for a linked list of strings.
 */
struct list
{
   /** First entry in the list, or NULL if the list is empty. */
   struct list_entry *first;

   /** Last entry in the list, or NULL if the list is empty. */
   struct list_entry *last;
};


/**
 * An entry in a map.  This is a name=value pair.
 */
struct map_entry
{
   /** The key for the map. */
   const char *name;
   /** The value associated with that key. */
   const char *value;
   /** The next map entry, or NULL if none. */
   struct map_entry *next;
};

/**
 * A map from a string to another string.
 * This is used for the paramaters passed in a HTTP GET request, and
 * to store the exports when the CGI interface is filling in a template.
 */
struct map
{
   /** The first map entry, or NULL if the map is empty. */
   struct map_entry *first;
   /** The last map entry, or NULL if the map is empty. */
   struct map_entry *last;
};


/**
 * A HTTP request.  This includes the method (GET, POST) and
 * the parsed URL.
 *
 * This is also used whenever we want to match a URL against a
 * URL pattern.  This always contains the URL to match, and never
 * a URL pattern.  (See struct url_spec).
 */
struct http_request
{
   char *cmd;      /**< Whole command line: method, URL, Version */
   char *ocmd;     /**< Backup of original cmd for CLF logging */
   char *gpc;      /**< HTTP method: GET, POST, ... */
   char *url;      /**< The URL */
   char *ver;      /**< Protocol version */
   int status;     /**< HTTP Status */

   char *host;     /**< Host part of URL */
   int   port;     /**< Port of URL or 80 (default) */
   char *path;     /**< Path of URL */
   char *hostport; /**< host[:port] */
   int   ssl;      /**< Flag if protocol is https */

   char *host_ip_addr_str; /**< String with dotted decimal representation
                                of host's IP. NULL before connect_to() */

   char  *dbuffer; /**< Buffer with '\0'-delimited domain name.           */
   char **dvec;    /**< List of pointers to the strings in dbuffer.       */
   int    dcount;  /**< How many parts to this domain? (length of dvec)   */
};


/**
 * Response generated by CGI, blocker, or error handler
 */
struct http_response
{
  char  *status;          /**< HTTP status (string). */
  struct list headers[1]; /**< List of header lines. */
  char  *head;            /**< Formatted http response head. */
  size_t head_length;     /**< Length of http response head. */
  char  *body;            /**< HTTP document body. */
  size_t content_length;  /**< Length of body, REQUIRED if binary body. */
  int    is_static;       /**< Nonzero if the content will never change and
                               should be cached by the browser (e.g. images). */
};

/**
 * A URL pattern.
 */
struct url_spec
{
   /** The string which was parsed to produce this url_spec.
       Used for debugging or display only.  */
   char  *spec;

   char  *dbuffer;     /**< Buffer with '\0'-delimited domain name, or NULL to match all hosts. */
   char **dvec;        /**< List of pointers to the strings in dbuffer.       */
   int    dcount;      /**< How many parts to this domain? (length of dvec)   */
   int    unanchored;  /**< Bitmap - flags are ANCHOR_LEFT and ANCHOR_RIGHT.  */

   int   port;         /**< The port number, or 0 to match all ports.         */

   char *path;         /**< The source for the regex.                         */
   int   pathlen;      /**< ==strlen(path).  Needed for prefix matching.  FIXME: Now obsolete?     */
   regex_t *preg;      /**< Regex for matching path part                      */
};

/**
 * If you declare a static url_spec, this is the value to initialize it to zero.
 */
#define URL_SPEC_INITIALIZER { NULL, NULL, NULL, 0, 0, 0, NULL, 0, NULL }

/**
 * Constant for host part matching in URLs.  If set, indicates that the start of
 * the pattern must match the start of the URL.  E.g. this is not set for the
 * pattern ".example.com", so that it will match both "example.com" and
 * "www.example.com".  It is set for the pattern "example.com", which makes it
 * match "example.com" only, not "www.example.com".
 */
#define ANCHOR_LEFT  1

/**
 * Constant for host part matching in URLs.  If set, indicates that the end of
 * the pattern must match the end of the URL.  E.g. this is not set for the
 * pattern "ad.", so that it will match any host called "ad", irrespective
 * of how many subdomains are in the fully-qualified domain name.
 */
#define ANCHOR_RIGHT 2


/**
 * An I/O buffer.  Holds a string which can be appended to, and can have data
 * removed from the beginning.
 */
struct iob
{
   char *buf;    /**< Start of buffer        */
   char *cur;    /**< Start of relevant data */
   char *eod;    /**< End of relevant data   */
   size_t size;  /**< Size as malloc()ed     */
};


/**
 * Return the number of bytes in the I/O buffer associated with the passed
 * client_state pointer.
 * May be zero.
 */
#define IOB_PEEK(CSP) ((CSP->iob->cur > CSP->iob->eod) ? (CSP->iob->eod - CSP->iob->cur) : 0)


/**
 * Remove any data in the I/O buffer associated with the passed
 * client_state pointer.
 */
#define IOB_RESET(CSP) if(CSP->iob->buf) free(CSP->iob->buf); memset(CSP->iob, '\0', sizeof(CSP->iob));

/* Bits for csp->content_type */
#define CT_TEXT   1 /**< csp->content_type bitmask:
                         Suitable for pcrs filtering. */
#define CT_GIF    2 /**< csp->content_type bitmask:
                         Suitable for GIF filtering.  */
#define CT_TABOO  4 /**< csp->content_type bitmask:
                         DO NOT filter, irrespective of other flags. */

/**
 * The mask which includes all actions.
 */
#define ACTION_MASK_ALL        (~0UL)

/**
 * The most compatible set of actions - i.e. none.
 */
#define ACTION_MOST_COMPATIBLE 0x00000000UL

/** Action bitmap: Block the request. */
#define ACTION_BLOCK           0x00000001UL
/** Action bitmap: Deanimate if it's a GIF. */
#define ACTION_DEANIMATE       0x00000002UL
/** Action bitmap: Downgrade HTTP/1.1 to 1.0. */
#define ACTION_DOWNGRADE       0x00000004UL
/** Action bitmap: Fast redirects. */
#define ACTION_FAST_REDIRECTS  0x00000008UL
/** Action bitmap: Remove existing "Forwarded" header, and do not add another. */
#define ACTION_HIDE_FORWARDED  0x00000010UL
/** Action bitmap: Hide "From" header. */
#define ACTION_HIDE_FROM       0x00000020UL
/** Action bitmap: Hide "Referer" header.  (sic - follow HTTP, not English). */
#define ACTION_HIDE_REFERER    0x00000040UL
/** Action bitmap: Hide "User-Agent" and similar headers. */
#define ACTION_HIDE_USER_AGENT 0x00000080UL
/** Action bitmap: This is an image. */
#define ACTION_IMAGE           0x00000100UL
/** Action bitmap: Sets the image blocker. */
#define ACTION_IMAGE_BLOCKER   0x00000200UL
/** Action bitmap: Prevent compression. */
#define ACTION_NO_COMPRESSION  0x00000400UL
/** Action bitmap: Change cookies to session only cookies. */
#define ACTION_NO_COOKIE_KEEP  0x00000800UL
/** Action bitmap: Block rending cookies. */
#define ACTION_NO_COOKIE_READ  0x00001000UL
/** Action bitmap: Block setting cookies. */
#define ACTION_NO_COOKIE_SET   0x00002000UL
/** Action bitmap: Filter out popups. */
#define ACTION_NO_POPUPS       0x00004000UL
/** Action bitmap: Send a vanilla wafer. */
#define ACTION_VANILLA_WAFER   0x00008000UL
/** Action bitmap: Limit CONNECT requests to safe ports. */
#define ACTION_LIMIT_CONNECT   0x00010000UL

/** Action string index: How to deanimate GIFs */
#define ACTION_STRING_DEANIMATE     0
/** Action string index: Replacement for "From:" header */
#define ACTION_STRING_FROM          1
/** Action string index: How to block images */
#define ACTION_STRING_IMAGE_BLOCKER 2
/** Action string index: Replacement for "Referer:" header */
#define ACTION_STRING_REFERER       3
/** Action string index: Replacement for "User-Agent:" header */
#define ACTION_STRING_USER_AGENT    4
/** Action string index: Legal CONNECT ports. */
#define ACTION_STRING_LIMIT_CONNECT 5
/** Number of string actions. */
#define ACTION_STRING_COUNT         6

/** Index into current_action_spec::multi[] for headers to add. */
#define ACTION_MULTI_ADD_HEADER     0
/** Index into current_action_spec::multi[] for headers to add. */
#define ACTION_MULTI_WAFER          1
/** Index into current_action_spec::multi[] for filters to apply. */
#define ACTION_MULTI_FILTER         2
/** Number of multi-string actions. */
#define ACTION_MULTI_COUNT          3


/**
 * This structure contains a list of actions to apply to a URL.
 * It only contains positive instructions - no "-" options.
 * It is not used to store the actions list itself, only for
 * url_actions() to return the current values.
 */
struct current_action_spec
{
   /** Actions to apply.  A bit set to "1" means perform the action. */
   unsigned long flags;

   /**
    * Paramaters for those actions that require them.
    * Each entry is valid if & only if the corresponding entry in "flags" is
    * set.
    */
   char * string[ACTION_STRING_COUNT];

   /** Lists of strings for multi-string actions. */
   struct list multi[ACTION_MULTI_COUNT][1];
};


/**
 * This structure contains a set of changes to actions.
 * It can contain both positive and negative instructions.
 * It is used to store an entry in the actions list.
 */
struct action_spec
{
   unsigned long mask; /**< Actions to keep. A bit set to "0" means remove action. */
   unsigned long add;  /**< Actions to add.  A bit set to "1" means add action.    */

   /**
    * Paramaters for those actions that require them.
    * Each entry is valid if & only if the corresponding entry in "flags" is
    * set.
    */
   char * string[ACTION_STRING_COUNT];

   /** Lists of strings to remove, for multi-string actions. */
   struct list multi_remove[ACTION_MULTI_COUNT][1];

   /** If nonzero, remove *all* strings from the multi-string action. */
   int         multi_remove_all[ACTION_MULTI_COUNT];

   /** Lists of strings to add, for multi-string actions. */
   struct list multi_add[ACTION_MULTI_COUNT][1];
};


/**
 * This structure is used to store the actions list.
 *
 * It contains a URL pattern, and the chages to the actions.
 * It is a linked list.
 */
struct url_actions
{
   struct url_spec url[1];        /**< URL pattern. */

   struct action_spec action[1];  /**< Actions. */

   struct url_actions * next;     /**< Next action in file, or NULL. */
};


/*
 * Flags for use in csp->flags
 */
 
/**
 * Flag for csp->flags: Set if this client is processing data.
 * Cleared when the thread associated with this structure dies.
 */
#define CSP_FLAG_ACTIVE     0x01

/**
 * Flag for csp->flags: Set if the server's reply is in "chunked"
 * transfer encoding
 */
#define CSP_FLAG_CHUNKED    0x02

/**
 * Flag for csp->flags: Set if this request was enforced, although it would
 * normally have been blocked.
 */
#define CSP_FLAG_FORCED     0x04

/**
 * Flag for csp->flags: Set if any modification to the body was done.
 */
#define CSP_FLAG_MODIFIED   0x08

/**
 * Flag for csp->flags: Set if request was blocked.
 */
#define CSP_FLAG_REJECTED   0x10

/**
 * Flag for csp->flags: Set if we are toggled on (FEATURE_TOGGLE).
 */
#define CSP_FLAG_TOGGLED_ON 0x20


/*
 * Flags for use in return codes of child processes
 */

/**
 * Flag for process return code: Set if exiting porcess has been toggled
 * during its lifetime.
 */
#define RC_FLAG_TOGGLED   0x10

/**
 * Flag for process return code: Set if exiting porcess has blocked its
 * request.
 */
#define RC_FLAG_BLOCKED   0x20

/**
 * Maximum number of actions files.  This limit is arbitrary - it's just used
 * to size an array.
 */
#define MAX_ACTION_FILES 10

/**
 * The state of a Privoxy processing thread.
 */
struct client_state
{
   /** The proxy's configuration */
   struct configuration_spec * config;

   /** The actions to perform on the current request */
   struct current_action_spec  action[1];

   /** socket to talk to client (web browser) */
   jb_socket cfd;

   /** socket to talk to server (web server or proxy) */
   jb_socket sfd;

   /** Multi-purpose flag container, see CSP_FLAG_* above */
   unsigned short int flags;

   /** Client PC's IP address, as reported by the accept() function.
       As a string. */
   char *ip_addr_str;
   /** Client PC's IP address, as reported by the accept() function.
       As a number. */
   long  ip_addr_long;

   /** Our IP address. I.e. the IP address that the client used to reach us,
       as a string. */
   char *my_ip_addr_str;

   /** Our hostname. I.e. the reverse DNS of the IP address that the client
       used to reach us, as a string. */
   char *my_hostname;

   /** The URL that was requested */
   struct http_request http[1];

   /** An I/O buffer used for buffering data read from the network */
   struct iob iob[1];

   /** List of all headers for this request */
   struct list headers[1];

   /** List of all cookies for this request */
   struct list cookie_list[1];

   /** MIME-Type key, see CT_* above */
   unsigned short int content_type;

   /** The "X-Forwarded-For:" header sent by the client */
   char   *x_forwarded;

   /** Actions files associated with this client */
   struct file_list *actions_list[MAX_ACTION_FILES];

   /** pcrs job file. */
   struct file_list *rlist;

   /** Length after content modification. */
   size_t content_length;

#ifdef FEATURE_TRUST

   /** Trust file. */
   struct file_list *tlist;

#endif /* def FEATURE_TRUST */

   /** Next thread in linked list. Only read or modify from the main thread! */
   struct client_state *next;
};


/**
 * A function to add a header
 */
typedef jb_err (*add_header_func_ptr)(struct client_state *);

/**
 * A function to process a header
 */
typedef jb_err (*parser_func_ptr    )(struct client_state *, char **);


/**
 * List of functions to run on a list of headers
 */
struct parsers
{
   /** The header prefix to match */
   char   *str;
   
   /** The length of the prefix to match */
   size_t len;
   
   /** The function to apply to this line */
   parser_func_ptr parser;
};


/**
 * List of available CGI functions.
 */
struct cgi_dispatcher
{
   /** The URL of the CGI, relative to the CGI root. */
   const char * const name;

   /** The handler function for the CGI */
   jb_err    (* const handler)(struct client_state *csp, struct http_response *rsp, const struct map *parameters);

   /** The description of the CGI, to appear on the main menu, or NULL to hide it. */
   const char * const description;

   /** A flag that indicates whether unintentional calls to this CGI can cause damage */
   int harmless;
};


/**
 * A data file used by Privoxy.  Kept in a linked list.
 */
struct file_list
{
   /**
    * This is a pointer to the data structures associated with the file.
    * Read-only once the structure has been created.
    */
   void *f;

   /**
    * The unloader function.
    * Normally NULL.  When we are finished with file (i.e. when we have
    * loaded a new one), set to a pointer to an unloader function.
    * Unloader will be called by sweep() (called from main loop) when
    * all clients using this file are done.  This prevents threading
    * problems.
    */
   void (*unloader)(void *);

   /**
    * Used internally by sweep().  Do not access from elsewhere.
    */
   int active;

   /**
    * File last-modified time, so we can check if file has been changed.
    * Read-only once the structure has been created.
    */
   time_t lastmodified;
   
   /**
    * The full filename.
    */
   char * filename;

   /**
    * Pointer to next entry in the linked list of all "file_list"s.
    * This linked list is so that sweep() can navigate it.
    * Since sweep() can remove items from the list, we must be careful
    * to only access this value from main thread (when we know sweep
    * won't be running).
    */
   struct file_list *next;
};


#ifdef FEATURE_TRUST

/**
 * The format of a trust file when loaded into memory.
 */
struct block_spec
{
   struct url_spec url[1];   /**< The URL pattern              */
   int    reject;            /**< FIXME: Please document this! */
   struct block_spec *next;  /**< Next entry in linked list    */
};

#endif /* def FEATURE_TRUST */


#define SOCKS_NONE    0    /**< Don't use a SOCKS server               */
#define SOCKS_4      40    /**< original SOCKS 4 protocol              */
#define SOCKS_4A     41    /**< as modified for hosts w/o external DNS */


/**
 * How to forward a connection to a parent proxy.
 */
struct forward_spec
{
   /** URL pattern that this forward_spec is for. */
   struct url_spec url[1];

   /** Connection type.  Must be SOCKS_NONE, SOCKS_4, or SOCKS_4A. */
   int   type;

   /** SOCKS server hostname.  Only valid if "type" is SOCKS_4 or SOCKS_4A. */
   char *gateway_host;

   /** SOCKS server port. */
   int   gateway_port;

   /** Parent HTTP proxy hostname, or NULL for none. */
   char *forward_host;

   /** Parent HTTP proxy port. */
   int   forward_port;

   /** Next entry in the linked list. */
   struct forward_spec *next;
};


/**
 * Initializer for a static struct forward_spec.
 */
#define FORWARD_SPEC_INITIALIZER { { URL_SPEC_INITIALIZER }, 0, NULL, 0, NULL, 0, NULL }


/**
 * This struct represents one filter (one block) from
 * the re_filterfile. If there is more than one filter
 * in the file, the file will be represented by a
 * chained list of re_filterfile specs.
 */
struct re_filterfile_spec
{
   char *name;                      /**< Name from FILTER: statement in re_filterfile. */
   char *description;               /**< Description from FILTER: statement in re_filterfile. */
   struct list patterns[1];         /**< The patterns from the re_filterfile. */
   pcrs_job *joblist;               /**< The resulting compiled pcrs_jobs. */
   struct re_filterfile_spec *next; /**< The pointer for chaining. */
};


#ifdef FEATURE_ACL

#define ACL_PERMIT   1  /**< Accept connection request */
#define ACL_DENY     2  /**< Reject connection request */

/**
 * An IP address pattern.  Used to specify networks in the ACL.
 */
struct access_control_addr
{
   unsigned long addr;  /**< The IP address as an integer. */
   unsigned long mask;  /**< The network mask as an integer. */
   unsigned long port;  /**< The port number. */
};

/**
 * An access control list (ACL) entry.
 *
 * This is a linked list.
 */
struct access_control_list
{
   struct access_control_addr src[1];  /**< Client IP address */
   struct access_control_addr dst[1];  /**< Website or parent proxy IP address */

   short action;                       /**< ACL_PERMIT or ACL_DENY */
   struct access_control_list *next;   /**< The next entry in the ACL. */
};

#endif /* def FEATURE_ACL */


/** Maximum number of loaders (actions, re_filter, ...) */
#define NLOADERS 8


/** configuration_spec::feature_flags: CGI actions editor. */
#define RUNTIME_FEATURE_CGI_EDIT_ACTIONS  1

/** configuration_spec::feature_flags: Web-based toggle. */
#define RUNTIME_FEATURE_CGI_TOGGLE        2


/**
 * Data loaded from the configuration file.
 *
 * (Anomaly: toggle is still handled through a global, not this structure)
 */
struct configuration_spec
{
   /** What to log */
   int debug;
   
   /** Nonzero to enable multithreading. */
   int multi_threaded;

   /**
    * Bitmask of features that can be enabled/disabled through the config
    * file.  Currently defined bits:
    *
    * - RUNTIME_FEATURE_CGI_EDIT_ACTIONS
    * - RUNTIME_FEATURE_CGI_TOGGLE
    */
   unsigned feature_flags;

   /** The log file name. */
   const char *logfile;

   /** The config file directory. */
   const char *confdir;

   /** The log file directory. */
   const char *logdir;

   /** The full paths to the actions files. */
   const char *actions_file[MAX_ACTION_FILES];

   /** The short names of the actions files. */
   const char *actions_file_short[MAX_ACTION_FILES];

   /** The administrator's email address */
   char *admin_address;

   /** A URL with info on this proxy */
   char *proxy_info_url;

   /** URL to the user manual (on our website or local copy) */
   char *usermanual;

   /** The file name of the pcre filter file */
   const char *re_filterfile;

#ifdef FEATURE_COOKIE_JAR

   /** The file name of the cookie jar file */
   const char * jarfile;

   /** The handle to the cookie jar file */
   FILE * jar;

#endif /* def FEATURE_COOKIE_JAR */

   /** IP address to bind to.  Defaults to HADDR_DEFAULT == 127.0.0.1. */
   const char *haddr;

   /** Port to bind to.  Defaults to HADDR_PORT == 8118. */
   int         hport;

   /** Size limit for IOB */
   size_t buffer_limit;

#ifdef FEATURE_TRUST

   /** The file name of the trust file. */
   const char * trustfile;

   /** FIXME: DOCME: Document this. */
   struct list trust_info[1];

   /** FIXME: DOCME: Document this. */
   struct url_spec *trust_list[64];

#endif /* def FEATURE_TRUST */

#ifdef FEATURE_ACL

   /** The access control list (ACL). */
   struct access_control_list *acl;

#endif /* def FEATURE_ACL */

   /** Information about parent proxies (forwarding). */
   struct forward_spec *forward;

   /** All options from the config file, HTML-formatted. */
   char *proxy_args;

   /** The configuration file object. */
   struct file_list *config_file_list;

   /** List of loaders */
   int (*loaders[NLOADERS])(struct client_state *);

   /** Nonzero if we need to bind() to the new port. */
   int need_bind;
};

/** Calculates the number of elements in an array, using sizeof. */
#define SZ(X)  (sizeof(X) / sizeof(*X))

#ifdef FEATURE_FORCE_LOAD
/** The force load URL prefix. */
#define FORCE_PREFIX "/PRIVOXY-FORCE"
#endif /* def FEATURE_FORCE_LOAD */

#ifdef FEATURE_NO_GIFS
/** The MIME type for images ("image/png" or "image/gif"). */
#define BUILTIN_IMAGE_MIMETYPE "image/png"
#else
#define BUILTIN_IMAGE_MIMETYPE "image/gif"
#endif /* def FEATURE_NO_GIFS */


/* 
 * Hardwired URLs
 */

/** URL for the Privoxy home page. */
#define HOME_PAGE_URL     "http://www.privoxy.org/"

/** URL for the Privoxy user manual. */
#define USER_MANUAL_URL   HOME_PAGE_URL VERSION "/user-manual/"

/** Prefix for actions help links  (append to USER_MANUAL_URL). */
#define ACTIONS_HELP_PREFIX "actions-file.html#"

/** Prefix for config option help links (append to USER_MANUAL_URL). */
#define CONFIG_HELP_PREFIX  "config.html#"

/*
 * The "hosts" to intercept and display CGI pages.
 * First one is a hostname only, second one can specify host and path.
 *
 * Notes:
 * 1) Do not specify the http: prefix
 * 2) CGI_SITE_2_PATH must not end with /, one will be added automatically.
 * 3) CGI_SITE_2_PATH must start with /, unless it is the empty string.
 */
#define CGI_SITE_1_HOST "p.p"
#define CGI_SITE_2_HOST "config.privoxy.org"
#define CGI_SITE_2_PATH ""

/**
 * The prefix for CGI pages.  Written out in generated HTML.
 * INCLUDES the trailing slash.
 */
#define CGI_PREFIX  "http://" CGI_SITE_2_HOST CGI_SITE_2_PATH "/"


/* HTTP snipplets.
 *
 * FIXME: This is very inefficient.  There could be one copy of these strings
 * for each .c file!!  They should be "extern", not "static".
 */
static const char CSUCCEED[] =
   "HTTP/1.0 200 Connection established\n"
   "Proxy-Agent: Privoxy/" VERSION "\r\n\r\n";

static const char CHEADER[] =
   "HTTP/1.0 400 Invalid header received from browser\r\n\r\n";

static const char CFORBIDDEN[] =
   "HTTP/1.0 403 Connection not allowable\r\nX-Hint: If you read this message interactively, then you know why this happens ,-)\r\n\r\n";

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ndef PROJECT_H_INCLUDED */

/*
  Local Variables:
  tab-width: 3
  end:
*/
