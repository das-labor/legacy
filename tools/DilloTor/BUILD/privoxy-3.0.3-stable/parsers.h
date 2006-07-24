#ifndef PARSERS_H_INCLUDED
#define PARSERS_H_INCLUDED
#define PARSERS_H_VERSION "$Id: parsers.h,v 1.26.2.1 2002/09/25 14:52:46 oes Exp $"
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/parsers.h,v $
 *
 * Purpose     :  Declares functions to parse/crunch headers and pages.
 *                Functions declared include:
 *                   `add_to_iob', `client_cookie_adder', `client_from',
 *                   `client_referrer', `client_send_cookie', `client_ua',
 *                   `client_uagent', `client_x_forwarded',
 *                   `client_x_forwarded_adder', `client_xtra_adder',
 *                   `content_type', `crumble', `destroy_list', `enlist',
 *                   `flush_socket', `free_http_request', `get_header',
 *                   `list_to_text', `parse_http_request', `sed',
 *                   and `server_set_cookie'.
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
 *    $Log: parsers.h,v $
 *    Revision 1.26.2.1  2002/09/25 14:52:46  oes
 *    Added basic support for OPTIONS and TRACE HTTP methods:
 *     - New parser function client_max_forwards which decrements
 *       the Max-Forwards HTTP header field of OPTIONS and TRACE
 *       requests by one before forwarding
 *     - New parser function client_host which extracts the host
 *       and port information from the HTTP header field if the
 *       request URI was not absolute
 *     - Don't crumble and re-add the Host: header, but only generate
 *       and append if missing
 *
 *    Revision 1.26  2002/05/08 15:59:53  oes
 *    Changed add_to_iob signature (now returns jb_err)
 *
 *    Revision 1.25  2002/03/26 22:29:55  swa
 *    we have a new homepage!
 *
 *    Revision 1.24  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.23  2002/03/13 00:27:05  jongfoster
 *    Killing warnings
 *
 *    Revision 1.22  2002/03/09 20:03:52  jongfoster
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
 *    Revision 1.21  2002/03/07 03:46:17  oes
 *    Fixed compiler warnings
 *
 *    Revision 1.20  2002/02/20 23:15:13  jongfoster
 *    Parsing functions now handle out-of-memory gracefully by returning
 *    an error code.
 *
 *    Revision 1.19  2002/01/17 21:03:47  jongfoster
 *    Moving all our URL and URL pattern parsing code to urlmatch.c.
 *
 *    Revision 1.18  2001/10/26 17:40:23  oes
 *    Introduced get_header_value()
 *    Removed client_accept()
 *
 *    Revision 1.17  2001/10/13 12:47:32  joergs
 *    Removed client_host, added client_host_adder
 *
 *    Revision 1.16  2001/10/07 18:50:16  oes
 *    Added server_content_encoding, renamed server_transfer_encoding
 *
 *    Revision 1.15  2001/10/07 18:01:55  oes
 *    Changed server_http11 to server_http
 *
 *    Revision 1.14  2001/10/07 15:45:48  oes
 *    added client_accept_encoding, client_te, client_accept_encoding_adder
 *
 *    renamed content_type and content_length
 *
 *    fixed client_host and strclean prototypes
 *
 *    Revision 1.13  2001/09/29 12:56:03  joergs
 *    IJB now changes HTTP/1.1 to HTTP/1.0 in requests and answers.
 *
 *    Revision 1.12  2001/09/13 23:05:50  jongfoster
 *    Changing the string paramater to the header parsers a "const".
 *
 *    Revision 1.11  2001/07/31 14:46:53  oes
 *    Added prototype for connection_close_adder
 *
 *    Revision 1.10  2001/07/30 22:08:36  jongfoster
 *    Tidying up #defines:
 *    - All feature #defines are now of the form FEATURE_xxx
 *    - Permanently turned off WIN_GUI_EDIT
 *    - Permanently turned on WEBDAV and SPLIT_PROXY_ARGS
 *
 *    Revision 1.9  2001/07/29 18:43:08  jongfoster
 *    Changing #ifdef _FILENAME_H to FILENAME_H_INCLUDED, to conform to
 *    ANSI C rules.
 *
 *    Revision 1.8  2001/07/13 14:01:54  oes
 *    Removed all #ifdef PCRS
 *
 *    Revision 1.7  2001/06/29 13:32:14  oes
 *    Removed logentry from cancelled commit
 *
 *    Revision 1.6  2001/06/03 19:12:38  oes
 *    deleted const struct interceptors
 *
 *    Revision 1.5  2001/05/31 21:30:33  jongfoster
 *    Removed list code - it's now in list.[ch]
 *    Renamed "permission" to "action", and changed many features
 *    to use the actions file rather than the global config.
 *
 *    Revision 1.4  2001/05/27 13:19:06  oes
 *    Patched Joergs solution for the content-length in.
 *
 *    Revision 1.3  2001/05/26 13:39:32  jongfoster
 *    Only crunches Content-Length header if applying RE filtering.
 *    Without this fix, Microsoft Windows Update wouldn't work.
 *
 *    Revision 1.2  2001/05/20 01:21:20  jongfoster
 *    Version 2.9.4 checkin.
 *    - Merged popupfile and cookiefile, and added control over PCRS
 *      filtering, in new "permissionsfile".
 *    - Implemented LOG_LEVEL_FATAL, so that if there is a configuration
 *      file error you now get a message box (in the Win32 GUI) rather
 *      than the program exiting with no explanation.
 *    - Made killpopup use the PCRS MIME-type checking and HTTP-header
 *      skipping.
 *    - Removed tabs from "config"
 *    - Moved duplicated url parsing code in "loaders.c" to a new funcition.
 *    - Bumped up version number.
 *
 *    Revision 1.1.1.1  2001/05/15 13:59:01  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#include "project.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const struct parsers client_patterns[];
extern const struct parsers server_patterns[];

extern const add_header_func_ptr add_client_headers[];
extern const add_header_func_ptr add_server_headers[];

extern int flush_socket(jb_socket fd, struct client_state *csp);
extern jb_err add_to_iob(struct client_state *csp, char *buf, int n);
extern char *get_header(struct client_state *csp);
extern char *get_header_value(const struct list *header_list, const char *header_name);
extern char *sed(const struct parsers pats[], const add_header_func_ptr more_headers[], struct client_state *csp);

extern jb_err crumble                (struct client_state *csp, char **header);
extern jb_err client_referrer        (struct client_state *csp, char **header);
extern jb_err client_uagent          (struct client_state *csp, char **header);
extern jb_err client_ua              (struct client_state *csp, char **header);
extern jb_err client_from            (struct client_state *csp, char **header);
extern jb_err client_send_cookie     (struct client_state *csp, char **header);
extern jb_err client_x_forwarded     (struct client_state *csp, char **header);
extern jb_err client_accept_encoding (struct client_state *csp, char **header);
extern jb_err client_te              (struct client_state *csp, char **header);
extern jb_err client_max_forwards    (struct client_state *csp, char **header);
extern jb_err client_host(struct client_state *csp, char **header);


extern jb_err client_host_adder           (struct client_state *csp);
extern jb_err client_cookie_adder         (struct client_state *csp);
extern jb_err client_xtra_adder           (struct client_state *csp);
extern jb_err client_accept_encoding_adder(struct client_state *csp);
extern jb_err client_x_forwarded_adder    (struct client_state *csp);

extern jb_err connection_close_adder      (struct client_state *csp); 

extern jb_err server_set_cookie      (struct client_state *csp, char **header);
extern jb_err server_content_type    (struct client_state *csp, char **header);
extern jb_err server_content_length  (struct client_state *csp, char **header);
extern jb_err server_content_md5     (struct client_state *csp, char **header);
extern jb_err server_content_encoding(struct client_state *csp, char **header);
extern jb_err server_transfer_coding (struct client_state *csp, char **header);
extern jb_err server_http            (struct client_state *csp, char **header);

#ifdef FEATURE_FORCE_LOAD
extern int strclean(const char *string, const char *substring);
#endif /* def FEATURE_FORCE_LOAD */

/* Revision control strings from this header and associated .c file */
extern const char parsers_rcs[];
extern const char parsers_h_rcs[];

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ndef PARSERS_H_INCLUDED */

/*
  Local Variables:
  tab-width: 3
  end:
*/
