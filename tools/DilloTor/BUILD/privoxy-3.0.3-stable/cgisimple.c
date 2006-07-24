const char cgisimple_rcs[] = "$Id: cgisimple.c,v 1.35.2.3 2003/12/17 16:34:15 oes Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/cgisimple.c,v $
 *
 * Purpose     :  Simple CGIs to get information about Privoxy's
 *                status.
 *                
 *                Functions declared include:
 * 
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
 *    $Log: cgisimple.c,v $
 *    Revision 1.35.2.3  2003/12/17 16:34:15  oes
 *     - Prevent line wrap beween "View/Edit" link buttons on status page
 *     - Some (mostly irrelevant) fixes for Out-of-mem-case handling
 *
 *    Revision 1.35.2.2  2003/04/03 13:48:28  oes
 *    Don't show "Edit" buttons #ifndef FEATURE_CGI_EDIT_ACTIONS
 *
 *    Revision 1.35.2.1  2002/07/04 15:02:38  oes
 *    Added ability to send redirects to send-banner CGI, so that it can completely mimic the image blocking action if called with type=auto
 *
 *    Revision 1.35.2.1  2002/07/01 17:32:04  morcego
 *    Applying patch from Andreas as provided by Hal on the list.
 *    Message-ID: <20020701121218.V1606@feenix.burgiss.net>
 *
 *    Revision 1.35  2002/05/12 21:44:44  jongfoster
 *    Adding amiga.[ch] revision information, if on an amiga.
 *
 *    Revision 1.34  2002/04/30 12:06:12  oes
 *    Deleted unused code from default_cgi
 *
 *    Revision 1.33  2002/04/30 11:14:52  oes
 *    Made csp the first parameter in *action_to_html
 *
 *    Revision 1.32  2002/04/26 18:29:13  jongfoster
 *    Fixing this Visual C++ warning:
 *    cgisimple.c(775) : warning C4018: '<' : signed/unsigned mismatch
 *
 *    Revision 1.31  2002/04/26 12:54:36  oes
 *     - Kill obsolete REDIRECT_URL code
 *     - Error handling fixes
 *     - Style sheet related HTML snipplet changes
 *     - cgi_show_url_info:
 *       - Matches now in table, actions on single lines,
 *         linked to help
 *       - standard.action suppressed
 *       - Buttons to View and Edit AFs
 *
 *    Revision 1.30  2002/04/24 02:18:08  oes
 *     - show-status is now the starting point for editing
 *       the actions files, generate list of all AFs with buttons
 *       for viewing and editing, new look for file list (Jon:
 *       buttons now aligned ;-P ), view mode now supports multiple
 *       AFs, name changes, no view links for unspecified files,
 *       no edit link for standard.action.
 *
 *     - Jon's multiple AF patch: cgi_show_url_info now uses all
 *       AFs and marks the output accordingly
 *
 *    Revision 1.29  2002/04/10 13:38:35  oes
 *    load_template signature changed
 *
 *    Revision 1.28  2002/04/07 15:42:12  jongfoster
 *    Fixing send-banner?type=auto when the image-blocker is
 *    a redirect to send-banner
 *
 *    Revision 1.27  2002/04/05 15:50:48  oes
 *    added send-stylesheet CGI
 *
 *    Revision 1.26  2002/04/04 00:36:36  gliptak
 *    always use pcre for matching
 *
 *    Revision 1.25  2002/04/03 22:28:03  gliptak
 *    Removed references to gnu_regex
 *
 *    Revision 1.24  2002/04/02 16:12:47  oes
 *    Fix: moving misplaced lines into #ifdef FEATURE_FORCE
 *
 *    Revision 1.23  2002/03/26 22:29:54  swa
 *    we have a new homepage!
 *
 *    Revision 1.22  2002/03/24 16:18:15  jongfoster
 *    Removing old logo
 *
 *    Revision 1.21  2002/03/24 15:23:33  jongfoster
 *    Name changes
 *
 *    Revision 1.20  2002/03/24 13:25:43  swa
 *    name change related issues
 *
 *    Revision 1.19  2002/03/16 23:54:06  jongfoster
 *    Adding graceful termination feature, to help look for memory leaks.
 *    If you enable this (which, by design, has to be done by hand
 *    editing config.h) and then go to http://i.j.b/die, then the program
 *    will exit cleanly after the *next* request.  It should free all the
 *    memory that was used.
 *
 *    Revision 1.18  2002/03/12 01:44:49  oes
 *    Changed default for "blocked" image from jb logo to checkboard pattern
 *
 *    Revision 1.17  2002/03/08 16:43:18  oes
 *    Added choice beween GIF and PNG built-in images
 *
 *    Revision 1.16  2002/03/07 03:48:38  oes
 *     - Changed built-in images from GIF to PNG
 *       (with regard to Unisys patent issue)
 *     - Added a 4x4 pattern PNG which is less intrusive
 *       than the logo but also clearly marks the deleted banners
 *
 *    Revision 1.15  2002/03/06 22:54:35  jongfoster
 *    Automated function-comment nitpicking.
 *
 *    Revision 1.14  2002/03/02 04:14:50  david__schmidt
 *    Clean up a little CRLF unpleasantness that suddenly appeared
 *
 *    Revision 1.13  2002/02/21 00:10:37  jongfoster
 *    Adding send-banner?type=auto option
 *
 *    Revision 1.12  2002/01/23 01:03:32  jongfoster
 *    Fixing gcc [CygWin] compiler warnings
 *
 *    Revision 1.11  2002/01/23 00:01:04  jongfoster
 *    Adding cgi_transparent_gif() for http://i.j.b/t
 *    Adding missing html_encode() to many CGI functions.
 *    Adding urlmatch.[ch] to http://i.j.b/show-version
 *
 *    Revision 1.10  2002/01/17 21:10:37  jongfoster
 *    Changes to cgi_show_url_info to use new matching code from urlmatch.c.
 *    Also fixing a problem in the same function with improperly quoted URLs
 *    in output HTML, and adding code to handle https:// URLs correctly.
 *
 *    Revision 1.9  2001/11/30 23:09:15  jongfoster
 *    Now reports on FEATURE_CGI_EDIT_ACTIONS
 *    Removing FEATURE_DENY_GZIP from template
 *
 *    Revision 1.8  2001/11/13 00:14:07  jongfoster
 *    Fixing stupid bug now I've figured out what || means.
 *    (It always returns 0 or 1, not one of it's paramaters.)
 *
 *    Revision 1.7  2001/10/23 21:48:19  jongfoster
 *    Cleaning up error handling in CGI functions - they now send back
 *    a HTML error page and should never cause a FATAL error.  (Fixes one
 *    potential source of "denial of service" attacks).
 *
 *    CGI actions file editor that works and is actually useful.
 *
 *    Ability to toggle JunkBuster remotely using a CGI call.
 *
 *    You can turn off both the above features in the main configuration
 *    file, e.g. if you are running a multi-user proxy.
 *
 *    Revision 1.6  2001/10/14 22:00:32  jongfoster
 *    Adding support for a 404 error when an invalid CGI page is requested.
 *
 *    Revision 1.5  2001/10/07 15:30:41  oes
 *    Removed FEATURE_DENY_GZIP
 *
 *    Revision 1.4  2001/10/02 15:31:12  oes
 *    Introduced show-request cgi
 *
 *    Revision 1.3  2001/09/22 16:34:44  jongfoster
 *    Removing unneeded #includes
 *
 *    Revision 1.2  2001/09/19 18:01:11  oes
 *    Fixed comments; cosmetics
 *
 *    Revision 1.1  2001/09/16 17:08:54  jongfoster
 *    Moving simple CGI functions from cgi.c to new file cgisimple.c
 *
 *
 **********************************************************************/


#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#define snprintf _snprintf
#endif /* def _WIN32 */

#include "project.h"
#include "cgi.h"
#include "cgisimple.h"
#include "list.h"
#include "encode.h"
#include "jcc.h"
#include "filters.h"
#include "actions.h"
#include "miscutil.h"
#include "loadcfg.h"
#include "parsers.h"
#include "urlmatch.h"
#include "errlog.h"

const char cgisimple_h_rcs[] = CGISIMPLE_H_VERSION;


static char *show_rcs(void);
static jb_err show_defines(struct map *exports);


/*********************************************************************
 *
 * Function    :  cgi_default
 *
 * Description :  CGI function that is called for the CGI_SITE_1_HOST
 *                and CGI_SITE_2_HOST/CGI_SITE_2_PATH base URLs.
 *                Boring - only exports the default exports.
 *               
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : none
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory
 *
 *********************************************************************/
jb_err cgi_default(struct client_state *csp,
                   struct http_response *rsp,
                   const struct map *parameters)
{
   struct map *exports;

   assert(csp);
   assert(rsp);

   if (NULL == (exports = default_exports(csp, "")))
   {
      return JB_ERR_MEMORY;
   }

   return template_fill_for_cgi(csp, "default", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_error_404
 *
 * Description :  CGI function that is called if an unknown action was
 *                given.
 *               
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : none
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_error_404(struct client_state *csp,
                     struct http_response *rsp,
                     const struct map *parameters)
{
   struct map *exports;

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (NULL == (exports = default_exports(csp, NULL)))
   {
      return JB_ERR_MEMORY;
   }

   rsp->status = strdup("404 Privoxy configuration page not found");
   if (rsp->status == NULL)
   {
      free_map(exports);
      return JB_ERR_MEMORY;
   }

   return template_fill_for_cgi(csp, "cgi-error-404", exports, rsp);
}


#ifdef FEATURE_GRACEFUL_TERMINATION
/*********************************************************************
 *
 * Function    :  cgi_die
 *
 * Description :  CGI function to shut down Privoxy.
 *                NOTE: Turning this on in a production build
 *                would be a BAD idea.  An EXTREMELY BAD idea.
 *                In short, don't do it.
 *               
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : none
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_die (struct client_state *csp,
                struct http_response *rsp,
                const struct map *parameters)
{
   assert(csp);
   assert(rsp);
   assert(parameters);

   /* quit */
   g_terminate = 1;

   /*
    * I don't really care what gets sent back to the browser.
    * Take the easy option - "out of memory" page.
    */

   return JB_ERR_MEMORY;
}
#endif /* def FEATURE_GRACEFUL_TERMINATION */


/*********************************************************************
 *
 * Function    :  cgi_show_request
 *
 * Description :  Show the client's request and what sed() would have
 *                made of it.
 *               
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : none
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_show_request(struct client_state *csp,
                        struct http_response *rsp,
                        const struct map *parameters)
{
   char *p;
   struct map *exports;

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (NULL == (exports = default_exports(csp, "show-request")))
   {
      return JB_ERR_MEMORY;
   }
   
   /*
    * Repair the damage done to the IOB by get_header()
    */
   for (p = csp->iob->buf; p < csp->iob->eod; p++)
   {
      if (*p == '\0') *p = '\n';
   }

   /*
    * Export the original client's request and the one we would
    * be sending to the server if this wasn't a CGI call
    */

   if (map(exports, "client-request", 1, html_encode(csp->iob->buf), 0))
   {
      free_map(exports);
      return JB_ERR_MEMORY;
   }

   if (map(exports, "processed-request", 1, html_encode_and_free_original(
      sed(client_patterns, add_client_headers, csp)), 0))
   {
      free_map(exports);
      return JB_ERR_MEMORY;
   }

   return template_fill_for_cgi(csp, "show-request", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_send_banner
 *
 * Description :  CGI function that returns a banner. 
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *           type : Selects the type of banner between "trans", "logo",
 *                  and "auto". Defaults to "logo" if absent or invalid.
 *                  "auto" means to select as if we were image-blocking.
 *                  (Only the first character really counts; b and t are
 *                  equivalent).
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_send_banner(struct client_state *csp,
                       struct http_response *rsp,
                       const struct map *parameters)
{
   char imagetype = lookup(parameters, "type")[0];

   /*
    * If type is auto, then determine the right thing
    * to do from the set-image-blocker action
    */
   if (imagetype == 'a') 
   {
      /*
       * Default to pattern
       */
      imagetype = 'p';

#ifdef FEATURE_IMAGE_BLOCKING
      if ((csp->action->flags & ACTION_IMAGE_BLOCKER) != 0)
      {
         static const char prefix1[] = CGI_PREFIX "send-banner?type=";
         static const char prefix2[] = "http://" CGI_SITE_1_HOST "/send-banner?type=";
         const char *p = csp->action->string[ACTION_STRING_IMAGE_BLOCKER];

         if (p == NULL)
         {
            /* Use default - nothing to do here. */
         }
         else if (0 == strcmpic(p, "blank"))
         {
            imagetype = 'b';
         }
         else if (0 == strcmpic(p, "pattern"))
         {
            imagetype = 'p';
         }

         /*
          * If the action is to call this CGI, determine
          * the argument:
          */
         else if (0 == strncmpic(p, prefix1, sizeof(prefix1) - 1))
         {
            imagetype = p[sizeof(prefix1) - 1];
         }
         else if (0 == strncmpic(p, prefix2, sizeof(prefix2) - 1))
         {
            imagetype = p[sizeof(prefix2) - 1];
         }

         /*
          * Everything else must (should) be a URL to
          * redirect to.
          */
         else
         {
            imagetype = 'r';
         }
      }
#endif /* def FEATURE_IMAGE_BLOCKING */
   }
      
   /*
    * Now imagetype is either the non-auto type we were called with,
    * or it was auto and has since been determined. In any case, we
    * can proceed to actually answering the request by sending a redirect
    * or an image as appropriate:
    */
   if (imagetype == 'r') 
   {
      rsp->status = strdup("302 Local Redirect from Privoxy");
      if (rsp->status == NULL)
      {
         return JB_ERR_MEMORY;
      }
      if (enlist_unique_header(rsp->headers, "Location",
                               csp->action->string[ACTION_STRING_IMAGE_BLOCKER]))
      {
         return JB_ERR_MEMORY;
      }
   }
   else
   {
      if ((imagetype == 'b') || (imagetype == 't')) 
      {
         rsp->body = bindup(image_blank_data, image_blank_length);
         rsp->content_length = image_blank_length;
      }
      else
      {
         rsp->body = bindup(image_pattern_data, image_pattern_length);
         rsp->content_length = image_pattern_length;
      }

      if (rsp->body == NULL)
      {
         return JB_ERR_MEMORY;
      }
      if (enlist(rsp->headers, "Content-Type: " BUILTIN_IMAGE_MIMETYPE))
      {
         return JB_ERR_MEMORY;
      }

      rsp->is_static = 1;
   }

   return JB_ERR_OK;

}


/*********************************************************************
 *
 * Function    :  cgi_transparent_image
 *
 * Description :  CGI function that sends a 1x1 transparent image.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : None
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_transparent_image(struct client_state *csp,
                             struct http_response *rsp,
                             const struct map *parameters)
{
   rsp->body = bindup(image_blank_data, image_blank_length);
   rsp->content_length = image_blank_length;

   if (rsp->body == NULL)
   {
      return JB_ERR_MEMORY;
   }

   if (enlist(rsp->headers, "Content-Type: " BUILTIN_IMAGE_MIMETYPE))
   {
      return JB_ERR_MEMORY;
   }

   rsp->is_static = 1;

   return JB_ERR_OK;

}


/*********************************************************************
 *
 * Function    :  cgi_send_stylesheet
 *
 * Description :  CGI function that sends a css stylesheet found
 *                in the cgi-style.css template
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : None
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_send_stylesheet(struct client_state *csp,
                           struct http_response *rsp,
                           const struct map *parameters)
{
   jb_err err;
   
   assert(csp);
   assert(rsp);

   err = template_load(csp, &rsp->body, "cgi-style.css", 0);

   if (err == JB_ERR_FILE)
   {
      /*
       * No way to tell user; send empty stylesheet
       */
      log_error(LOG_LEVEL_ERROR, "Could not find cgi-style.css template");
   }
   else if (err)
   {
      return err; /* JB_ERR_MEMORY */
   }

   if (enlist(rsp->headers, "Content-Type: text/css"))
   {
      return JB_ERR_MEMORY;
   }

   return JB_ERR_OK;

}


/*********************************************************************
 *
 * Function    :  cgi_show_version
 *
 * Description :  CGI function that returns a a web page describing the
 *                file versions of Privoxy.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : none
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_show_version(struct client_state *csp,
                        struct http_response *rsp,
                        const struct map *parameters)
{
   struct map *exports;

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (NULL == (exports = default_exports(csp, "show-version")))
   {
      return JB_ERR_MEMORY;
   }

   if (map(exports, "sourceversions", 1, show_rcs(), 0))
   {
      free_map(exports);
      return JB_ERR_MEMORY;
   }

   return template_fill_for_cgi(csp, "show-version", exports, rsp);
}

 
/*********************************************************************
 *
 * Function    :  cgi_show_status
 *
 * Description :  CGI function that returns a a web page describing the
 *                current status of Privoxy.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *        file :  Which file to show.  Only first letter is checked,
 *                valid values are:
 *                - "p"ermissions (actions) file
 *                - "r"egex
 *                - "t"rust
 *                Default is to show menu and other information.
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_show_status(struct client_state *csp,
                       struct http_response *rsp,
                       const struct map *parameters)
{
   char *s = NULL;
   unsigned i;
   int j;

   FILE * fp;
   char buf[BUFFER_SIZE];
   const char * filename = NULL;
   char * file_description = NULL;
#ifdef FEATURE_STATISTICS
   float perc_rej;   /* Percentage of http requests rejected */
   int local_urls_read;
   int local_urls_rejected;
#endif /* ndef FEATURE_STATISTICS */
   struct file_list * fl;
   struct url_actions * b;
   jb_err err = JB_ERR_OK;

   struct map *exports;

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (NULL == (exports = default_exports(csp, "show-status")))
   {
      return JB_ERR_MEMORY;
   }

   switch (*(lookup(parameters, "file")))
   {
   case 'a':
      if (!get_number_param(csp, parameters, "index", &i) && i < MAX_ACTION_FILES && csp->actions_list[i])
      {
         filename = csp->actions_list[i]->filename;
         file_description = "Actions File";
      }
      break;

   case 'f':
      if (csp->rlist)
      {
         filename = csp->rlist->filename;
         file_description = "Filter File";
      }
      break;

#ifdef FEATURE_TRUST
   case 't':
      if (csp->tlist)
      {
         filename = csp->tlist->filename;
         file_description = "Trust File";
      }
      break;
#endif /* def FEATURE_TRUST */
   }

   if (NULL != filename)
   {
      if ( map(exports, "file-description", 1, file_description, 1)
        || map(exports, "filepath", 1, html_encode(filename), 0) )
      {
         free_map(exports);
         return JB_ERR_MEMORY;
      }

      if ((fp = fopen(filename, "r")) == NULL)
      {
         if (map(exports, "content", 1, "<h1>ERROR OPENING FILE!</h1>", 1))
         {
            free_map(exports);
            return JB_ERR_MEMORY;
         }
      }
      else
      {
         s = strdup("");
         while ((s != NULL) && fgets(buf, sizeof(buf), fp))
         {
            string_join  (&s, html_encode(buf));
         }
         fclose(fp);

         if (map(exports, "contents", 1, s, 0))
         {
            free_map(exports);
            return JB_ERR_MEMORY;
         }
      }

      return template_fill_for_cgi(csp, "show-status-file", exports, rsp);
   }

   s = strdup("");
   for (j = 0; (s != NULL) && (j < Argc); j++)
   {
      if (!err) err = string_join  (&s, html_encode(Argv[j]));
      if (!err) err = string_append(&s, " ");
   }
   if (!err) err = map(exports, "invocation", 1, s, 0);

   if (!err) err = map(exports, "options", 1, csp->config->proxy_args, 1);
   if (!err) err = show_defines(exports);

   if (err) 
   {
      free_map(exports);
      return JB_ERR_MEMORY;
   }

#ifdef FEATURE_STATISTICS
   local_urls_read     = urls_read;
   local_urls_rejected = urls_rejected;

   /*
    * Need to alter the stats not to include the fetch of this
    * page.
    *
    * Can't do following thread safely! doh!
    *
    * urls_read--;
    * urls_rejected--; * This will be incremented subsequently *
    */

   if (local_urls_read == 0)
   {
      if (!err) err = map_block_killer(exports, "have-stats");
   }
   else
   {
      if (!err) err = map_block_killer(exports, "have-no-stats");

      perc_rej = (float)local_urls_rejected * 100.0F /
            (float)local_urls_read;

      sprintf(buf, "%d", local_urls_read);
      if (!err) err = map(exports, "requests-received", 1, buf, 1);

      sprintf(buf, "%d", local_urls_rejected);
      if (!err) err = map(exports, "requests-blocked", 1, buf, 1);

      sprintf(buf, "%6.2f", perc_rej);
      if (!err) err = map(exports, "percent-blocked", 1, buf, 1);
   }

#else /* ndef FEATURE_STATISTICS */
   err = err || map_block_killer(exports, "statistics");
#endif /* ndef FEATURE_STATISTICS */
   
   /* 
    * List all action files in use, together with view and edit links,
    * except for standard.action, which should only be viewable. (Not
    * enforced in the editor itself)
    * FIXME: Shouldn't include hardwired HTML here, use line template instead!
    */
   s = strdup("");
   for (i = 0; i < MAX_ACTION_FILES; i++)
   {
      if (((fl = csp->actions_list[i]) != NULL) && ((b = fl->f) != NULL))
      {
         if (!err) err = string_append(&s, "<tr><td>");
         if (!err) err = string_join(&s, html_encode(csp->actions_list[i]->filename));
         snprintf(buf, 100, "</td><td class=\"buttons\"><a href=\"/show-status?file=actions&index=%d\">View</a>", i);
         if (!err) err = string_append(&s, buf);

#ifdef FEATURE_CGI_EDIT_ACTIONS
         if (NULL == strstr(csp->actions_list[i]->filename, "standard.action") && NULL != csp->config->actions_file_short[i])
         {
            snprintf(buf, 100, "&nbsp;&nbsp;<a href=\"/edit-actions-list?f=%s\">Edit</a>", csp->config->actions_file_short[i]);
            if (!err) err = string_append(&s, buf);
         }
#endif

         if (!err) err = string_append(&s, "</td></tr>\n");
      }
   }
   if (*s != '\0')   
   {
      if (!err) err = map(exports, "actions-filenames", 1, s, 0);
   }
   else
   {
      if (!err) err = map(exports, "actions-filenames", 1, "<tr><td>None specified</td></tr>", 1);
   }

   if (csp->rlist)
   {
      if (!err) err = map(exports, "re-filter-filename", 1, html_encode(csp->rlist->filename), 0);
   }
   else
   {
      if (!err) err = map(exports, "re-filter-filename", 1, "None specified", 1);
      if (!err) err = map_block_killer(exports, "have-filterfile");
   }

#ifdef FEATURE_TRUST
   if (csp->tlist)
   {
      if (!err) err = map(exports, "trust-filename", 1, html_encode(csp->tlist->filename), 0);
   }
   else
   {
      if (!err) err = map(exports, "trust-filename", 1, "None specified", 1);
      if (!err) err = map_block_killer(exports, "have-trustfile");
   }
#else
   if (!err) err = map_block_killer(exports, "trust-support");
#endif /* ndef FEATURE_TRUST */

   if (err)
   {
      free_map(exports);
      return JB_ERR_MEMORY;
   }

   return template_fill_for_cgi(csp, "show-status", exports, rsp);
}

 
/*********************************************************************
 *
 * Function    :  cgi_show_url_info
 *
 * Description :  CGI function that determines and shows which actions
 *                Privoxy will perform for a given url, and which
 *                matches starting from the defaults have lead to that.
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters :
 *            url : The url whose actions are to be determined.
 *                  If url is unset, the url-given conditional will be
 *                  set, so that all but the form can be suppressed in
 *                  the template.
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_show_url_info(struct client_state *csp,
                         struct http_response *rsp,
                         const struct map *parameters)
{
   char *url_param;
   struct map *exports;
   char buf[150];

   assert(csp);
   assert(rsp);
   assert(parameters);

   if (NULL == (exports = default_exports(csp, "show-url-info")))
   {
      return JB_ERR_MEMORY;
   }

   /*
    * Get the url= parameter (if present) and remove any leading/trailing spaces.
    */
   url_param = strdup(lookup(parameters, "url"));
   if (url_param == NULL)
   {
      free_map(exports);
      return JB_ERR_MEMORY;
   }
   chomp(url_param);

   /*
    * Handle prefixes.  4 possibilities:
    * 1) "http://" or "https://" prefix present and followed by URL - OK
    * 2) Only the "http://" or "https://" part is present, no URL - change
    *    to empty string so it will be detected later as "no URL".
    * 3) Parameter specified but doesn't contain "http(s?)://" - add a
    *    "http://" prefix.
    * 4) Parameter not specified or is empty string - let this fall through
    *    for now, next block of code will handle it.
    */
   if (0 == strncmp(url_param, "http://", 7))
   {
      if (url_param[7] == '\0')
      {
         /*
          * Empty URL (just prefix).
          * Make it totally empty so it's caught by the next if()
          */
         url_param[0] = '\0';
      }
   }
   else if (0 == strncmp(url_param, "https://", 8))
   {
      if (url_param[8] == '\0')
      {
         /*
          * Empty URL (just prefix).
          * Make it totally empty so it's caught by the next if()
          */
         url_param[0] = '\0';
      }
   }
   else if (url_param[0] != '\0')
   {
      /*
       * Unknown prefix - assume http://
       */
      char * url_param_prefixed = malloc(7 + 1 + strlen(url_param));
      if (NULL == url_param_prefixed)
      {
         free(url_param);
         free_map(exports);
         return JB_ERR_MEMORY;
      }
      strcpy(url_param_prefixed, "http://");
      strcpy(url_param_prefixed + 7, url_param);
      free(url_param);
      url_param = url_param_prefixed;
   }


   if (url_param[0] == '\0')
   {
      /* URL paramater not specified, display query form only. */
      free(url_param);
      if (map_block_killer(exports, "url-given")
        || map(exports, "url", 1, "", 1))
      {
         free_map(exports);
         return JB_ERR_MEMORY;
      }
   }
   else
   {
      /* Given a URL, so query it. */
      jb_err err;
      char *matches;
      char *s;
      int hits = 0;
      struct file_list *fl;
      struct url_actions *b;
      struct http_request url_to_query[1];
      struct current_action_spec action[1];
      int i;
      
      if (map(exports, "url", 1, html_encode(url_param), 0))
      {
         free(url_param);
         free_map(exports);
         return JB_ERR_MEMORY;
      }

      init_current_action(action);

      if (map(exports, "default", 1, current_action_to_html(csp, action), 0))
      {
         free_current_action(action);
         free(url_param);
         free_map(exports);
         return JB_ERR_MEMORY;
      }

      err = parse_http_url(url_param, url_to_query, csp);

      free(url_param);

      if (err == JB_ERR_MEMORY)
      {
         free_http_request(url_to_query);
         free_current_action(action);
         free_map(exports);
         return JB_ERR_MEMORY;
      }
      else if (err)
      {
         /* Invalid URL */

         err = map(exports, "matches", 1, "<b>[Invalid URL specified!]</b>" , 1);
         if (!err) err = map(exports, "final", 1, lookup(exports, "default"), 1);

         free_current_action(action);
         free_http_request(url_to_query);

         if (err)
         {
            free_map(exports);
            return JB_ERR_MEMORY;
         }

         return template_fill_for_cgi(csp, "show-url-info", exports, rsp);
      }

      /*
       * We have a warning about SSL paths.  Hide it for insecure sites.
       */
      if (!url_to_query->ssl)
      {
         if (map_block_killer(exports, "https"))
         {
            free_current_action(action);
            free_map(exports);
            free_http_request(url_to_query);
            return JB_ERR_MEMORY;
         }
      }

      matches = strdup("<table class=\"transparent\">");

      for (i = 0; i < MAX_ACTION_FILES; i++)
      {
         if (NULL == csp->config->actions_file_short[i]
             || !strcmp(csp->config->actions_file_short[i], "standard")) continue;

         b = NULL;
         hits = 1;
         if ((fl = csp->actions_list[i]) != NULL)
         {
            if ((b = fl->f) != NULL)
            {
               /* FIXME: Hardcoded HTML! */
               string_append(&matches, "<tr><th>In file: ");
               string_join  (&matches, html_encode(csp->config->actions_file_short[i]));
               snprintf(buf, 150, ".action <a class=\"cmd\" href=\"/show-status?file=actions&index=%d\">", i);
               string_append(&matches, buf);
               string_append(&matches, "View</a> <a class=\"cmd\" href=\"/edit-actions-list?f=");
               string_join  (&matches, html_encode(csp->config->actions_file_short[i]));
               string_append(&matches, "\">Edit</a></th></tr>\n");

               hits = 0;
               b = b->next;
            }
         }

         for (; (b != NULL) && (matches != NULL); b = b->next)
         {
            if (url_match(b->url, url_to_query))
            {
               string_append(&matches, "<tr><td>{");
               string_join  (&matches, actions_to_html(csp, b->action));
               string_append(&matches, " }</b><br>\n<code>");
               string_join  (&matches, html_encode(b->url->spec));
               string_append(&matches, "</code></td></tr>\n");

               if (merge_current_action(action, b->action))
               {
                  freez(matches);
                  free_http_request(url_to_query);
                  free_current_action(action);
                  free_map(exports);
                  return JB_ERR_MEMORY;
               }
               hits++;
            }
         }

         if (!hits)
         {
            string_append(&matches, "<tr><td>(no matches in this file)</td></tr>\n");
         }
      }
      string_append(&matches, "</table>\n");

      free_http_request(url_to_query);

      if (matches == NULL)
      {
         free_current_action(action);
         free_map(exports);
         return JB_ERR_MEMORY;
      }

      if (map(exports, "matches", 1, matches , 0))
      {
         free_current_action(action);
         free_map(exports);
         return JB_ERR_MEMORY;
      }

      s = current_action_to_html(csp, action);

      free_current_action(action);

      if (map(exports, "final", 1, s, 0))
      {
         free_map(exports);
         return JB_ERR_MEMORY;
      }
   }

   return template_fill_for_cgi(csp, "show-url-info", exports, rsp);
}


/*********************************************************************
 *
 * Function    :  cgi_robots_txt
 *
 * Description :  CGI function to return "/robots.txt".
 *
 * Parameters  :
 *          1  :  csp = Current client state (buffers, headers, etc...)
 *          2  :  rsp = http_response data structure for output
 *          3  :  parameters = map of cgi parameters
 *
 * CGI Parameters : None
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
jb_err cgi_robots_txt(struct client_state *csp,
                      struct http_response *rsp,
                      const struct map *parameters)
{
   char buf[100];
   jb_err err;

   rsp->body = strdup(
      "# This is the Privoxy control interface.\n"
      "# It isn't very useful to index it, and you're likely to break stuff.\n"
      "# So go away!\n"
      "\n"
      "User-agent: *\n"
      "Disallow: /\n"
      "\n");
   if (rsp->body == NULL)
   {
      return JB_ERR_MEMORY;
   }

   err = enlist_unique(rsp->headers, "Content-Type: text/plain", 13);

   rsp->is_static = 1;

   get_http_time(7 * 24 * 60 * 60, buf); /* 7 days into future */
   if (!err) err = enlist_unique_header(rsp->headers, "Expires", buf);

   return (err ? JB_ERR_MEMORY : JB_ERR_OK);
}


/*********************************************************************
 *
 * Function    :  show_defines
 *
 * Description :  Add to a map the state od all conditional #defines
 *                used when building
 *
 * Parameters  :
 *          1  :  exports = map to extend
 *
 * Returns     :  JB_ERR_OK on success
 *                JB_ERR_MEMORY on out-of-memory error.  
 *
 *********************************************************************/
static jb_err show_defines(struct map *exports)
{
   jb_err err = JB_ERR_OK;

#ifdef FEATURE_ACL
   if (!err) err = map_conditional(exports, "FEATURE_ACL", 1);
#else /* ifndef FEATURE_ACL */
   if (!err) err = map_conditional(exports, "FEATURE_ACL", 0);
#endif /* ndef FEATURE_ACL */

#ifdef FEATURE_CGI_EDIT_ACTIONS
   if (!err) err = map_conditional(exports, "FEATURE_CGI_EDIT_ACTIONS", 1);
#else /* ifndef FEATURE_COOKIE_JAR */
   if (!err) err = map_conditional(exports, "FEATURE_CGI_EDIT_ACTIONS", 0);
#endif /* ndef FEATURE_COOKIE_JAR */

#ifdef FEATURE_COOKIE_JAR
   if (!err) err = map_conditional(exports, "FEATURE_COOKIE_JAR", 1);
#else /* ifndef FEATURE_COOKIE_JAR */
   if (!err) err = map_conditional(exports, "FEATURE_COOKIE_JAR", 0);
#endif /* ndef FEATURE_COOKIE_JAR */

#ifdef FEATURE_FAST_REDIRECTS
   if (!err) err = map_conditional(exports, "FEATURE_FAST_REDIRECTS", 1);
#else /* ifndef FEATURE_FAST_REDIRECTS */
   if (!err) err = map_conditional(exports, "FEATURE_FAST_REDIRECTS", 0);
#endif /* ndef FEATURE_FAST_REDIRECTS */

#ifdef FEATURE_FORCE_LOAD
   if (!err) err = map_conditional(exports, "FEATURE_FORCE_LOAD", 1);
   if (!err) err = map(exports, "FORCE_PREFIX", 1, FORCE_PREFIX, 1);
#else /* ifndef FEATURE_FORCE_LOAD */
   if (!err) err = map_conditional(exports, "FEATURE_FORCE_LOAD", 0);
   if (!err) err = map(exports, "FORCE_PREFIX", 1, "(none - disabled)", 1);
#endif /* ndef FEATURE_FORCE_LOAD */

#ifdef FEATURE_IMAGE_BLOCKING
   if (!err) err = map_conditional(exports, "FEATURE_IMAGE_BLOCKING", 1);
#else /* ifndef FEATURE_IMAGE_BLOCKING */
   if (!err) err = map_conditional(exports, "FEATURE_IMAGE_BLOCKING", 0);
#endif /* ndef FEATURE_IMAGE_BLOCKING */

#ifdef FEATURE_IMAGE_DETECT_MSIE
   if (!err) err = map_conditional(exports, "FEATURE_IMAGE_DETECT_MSIE", 1);
#else /* ifndef FEATURE_IMAGE_DETECT_MSIE */
   if (!err) err = map_conditional(exports, "FEATURE_IMAGE_DETECT_MSIE", 0);
#endif /* ndef FEATURE_IMAGE_DETECT_MSIE */

#ifdef FEATURE_KILL_POPUPS
   if (!err) err = map_conditional(exports, "FEATURE_KILL_POPUPS", 1);
#else /* ifndef FEATURE_KILL_POPUPS */
   if (!err) err = map_conditional(exports, "FEATURE_KILL_POPUPS", 0);
#endif /* ndef FEATURE_KILL_POPUPS */

#ifdef FEATURE_NO_GIFS
   if (!err) err = map_conditional(exports, "FEATURE_NO_GIFS", 1);
#else /* ifndef FEATURE_NO_GIFS */
   if (!err) err = map_conditional(exports, "FEATURE_NO_GIFS", 0);
#endif /* ndef FEATURE_NO_GIFS */

#ifdef FEATURE_PTHREAD
   if (!err) err = map_conditional(exports, "FEATURE_PTHREAD", 1);
#else /* ifndef FEATURE_PTHREAD */
   if (!err) err = map_conditional(exports, "FEATURE_PTHREAD", 0);
#endif /* ndef FEATURE_PTHREAD */

#ifdef FEATURE_STATISTICS
   if (!err) err = map_conditional(exports, "FEATURE_STATISTICS", 1);
#else /* ifndef FEATURE_STATISTICS */
   if (!err) err = map_conditional(exports, "FEATURE_STATISTICS", 0);
#endif /* ndef FEATURE_STATISTICS */

#ifdef FEATURE_TOGGLE
   if (!err) err = map_conditional(exports, "FEATURE_TOGGLE", 1);
#else /* ifndef FEATURE_TOGGLE */
   if (!err) err = map_conditional(exports, "FEATURE_TOGGLE", 0);
#endif /* ndef FEATURE_TOGGLE */

#ifdef FEATURE_TRUST
   if (!err) err = map_conditional(exports, "FEATURE_TRUST", 1);
#else /* ifndef FEATURE_TRUST */
   if (!err) err = map_conditional(exports, "FEATURE_TRUST", 0);
#endif /* ndef FEATURE_TRUST */

#ifdef STATIC_PCRE
   if (!err) err = map_conditional(exports, "STATIC_PCRE", 1);
#else /* ifndef STATIC_PCRE */
   if (!err) err = map_conditional(exports, "STATIC_PCRE", 0);
#endif /* ndef STATIC_PCRE */

#ifdef STATIC_PCRS
   if (!err) err = map_conditional(exports, "STATIC_PCRS", 1);
#else /* ifndef STATIC_PCRS */
   if (!err) err = map_conditional(exports, "STATIC_PCRS", 0);
#endif /* ndef STATIC_PCRS */

   return err;
}


/*********************************************************************
 *
 * Function    :  show_rcs
 *
 * Description :  Create a string with the rcs info for all sourcefiles
 *
 * Parameters  :  None
 *
 * Returns     :  A string, or NULL on out-of-memory.
 *
 *********************************************************************/
static char *show_rcs(void)
{
   char *result = strdup("");
   char buf[BUFFER_SIZE];

   /* Instead of including *all* dot h's in the project (thus creating a
    * tremendous amount of dependencies), I will concede to declaring them
    * as extern's.  This forces the developer to add to this list, but oh well.
    */

#define SHOW_RCS(__x)              \
   {                               \
      extern const char __x[];     \
      sprintf(buf, "%s\n", __x);   \
      string_append(&result, buf); \
   }

   /* In alphabetical order */
   SHOW_RCS(actions_h_rcs)
   SHOW_RCS(actions_rcs)
#ifdef AMIGA
   SHOW_RCS(amiga_h_rcs)
   SHOW_RCS(amiga_rcs)
#endif /* def AMIGA */
   SHOW_RCS(cgi_h_rcs)
   SHOW_RCS(cgi_rcs)
#ifdef FEATURE_CGI_EDIT_ACTIONS
   SHOW_RCS(cgiedit_h_rcs)
   SHOW_RCS(cgiedit_rcs)
#endif /* def FEATURE_CGI_EDIT_ACTIONS */
   SHOW_RCS(cgisimple_h_rcs)
   SHOW_RCS(cgisimple_rcs)
#ifdef __MINGW32__
   SHOW_RCS(cygwin_h_rcs)
#endif
   SHOW_RCS(deanimate_h_rcs)
   SHOW_RCS(deanimate_rcs)
   SHOW_RCS(encode_h_rcs)
   SHOW_RCS(encode_rcs)
   SHOW_RCS(errlog_h_rcs)
   SHOW_RCS(errlog_rcs)
   SHOW_RCS(filters_h_rcs)
   SHOW_RCS(filters_rcs)
   SHOW_RCS(gateway_h_rcs)
   SHOW_RCS(gateway_rcs)
   SHOW_RCS(jbsockets_h_rcs)
   SHOW_RCS(jbsockets_rcs)
   SHOW_RCS(jcc_h_rcs)
   SHOW_RCS(jcc_rcs)
#ifdef FEATURE_KILL_POPUPS
   SHOW_RCS(killpopup_h_rcs)
   SHOW_RCS(killpopup_rcs)
#endif /* def FEATURE_KILL_POPUPS */
   SHOW_RCS(list_h_rcs)
   SHOW_RCS(list_rcs)
   SHOW_RCS(loadcfg_h_rcs)
   SHOW_RCS(loadcfg_rcs)
   SHOW_RCS(loaders_h_rcs)
   SHOW_RCS(loaders_rcs)
   SHOW_RCS(miscutil_h_rcs)
   SHOW_RCS(miscutil_rcs)
   SHOW_RCS(parsers_h_rcs)
   SHOW_RCS(parsers_rcs)
   SHOW_RCS(pcrs_rcs)
   SHOW_RCS(pcrs_h_rcs)
   SHOW_RCS(project_h_rcs)
   SHOW_RCS(ssplit_h_rcs)
   SHOW_RCS(ssplit_rcs)
   SHOW_RCS(urlmatch_h_rcs)
   SHOW_RCS(urlmatch_rcs)
#ifdef _WIN32
#ifndef _WIN_CONSOLE
   SHOW_RCS(w32log_h_rcs)
   SHOW_RCS(w32log_rcs)
   SHOW_RCS(w32res_h_rcs)
   SHOW_RCS(w32taskbar_h_rcs)
   SHOW_RCS(w32taskbar_rcs)
#endif /* ndef _WIN_CONSOLE */
   SHOW_RCS(win32_h_rcs)
   SHOW_RCS(win32_rcs)
#endif /* def _WIN32 */

#undef SHOW_RCS

   return result;

}


/*
  Local Variables:
  tab-width: 3
  end:
*/
