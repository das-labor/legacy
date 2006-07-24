/*
 * Dpi for FTP.
 *
 * This server checks the ftp-URL to be a directory (requires wget).
 * If true, it sends back an html representation of it, and if not
 * a dpip message (which is catched by dillo who redirects the ftp URL
 * to the downloads server).
 *
 * THIS IS A PROTOTYPE to illustrate how it is done. Feel free to polish!
 *
 * Copyright 2003 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

/*
 * TODO:
 * - Check dpi command syntax
 * - Handle simultaneous connections.
 *   (note that if it becomes resident there's less forking).
 * - Send feedback about the FTP login process from wget's stderr.
 *   i.e. capture our child's stderr, process it, and report back.
 * - Add error handling
 * - etc
 *
 *   If ftp.dpi is implemented with a low level ftp library, it becomes
 *   possible to keep the connection alive, and thus make browsing of ftp
 *   directories faster (this avoids one login per page, and forks). Perhaps
 *   it's not worth, but can be done.
 *
 *   Currently, anything the user clicks is sent back to Dillo. This
 *   may be better handled by inserting file type detection here. If
 *   we detect a binary, the connection is aborted and a download request
 *   dpi tag is sent instead.
 *
 *   This ftp plugin profits from the fact that given no MIME type in
 *   the HTTP header, dillo tries to detect the filetype itself.
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/time.h>
#include <ctype.h>
#include <glib.h>
#include "dpiutil.h"

#define BUFLEN 256
#define TOUT 300

/*
 * Global variables
 */
static SockHandler *sh = NULL;

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

/*
 * Filter '\n', '\r', "%0D" and "%0A" from the authority part of an FTP url.
 * This helps to avoid a SMTP relaying hack. This filtering could be done
 * only when port == 25, but if the mail server is listening on another
 * port it wouldn't work.
 * Note: AFAIS this should be done by wget.
 */
static void ftpurl_filter_smtp_hack(GString *gs_url)
{
   int i;
   char c;

   if (gs_url->len > 6) { /* ftp:// */
      for (i = 6; (c = gs_url->str[i]) && c != '/'; ++i) {
         if (c == '\n' || c == '\r') {
            g_string_erase(gs_url, i--, 1);
         } else if (c == '%' && gs_url->str[i+1] == '0' &&
                    (tolower(gs_url->str[i+2]) == 'a' ||
                     tolower(gs_url->str[i+2]) == 'd')) {
            g_string_erase(gs_url, i--, 3);
         }
      }
   }
}

/*
 * Escape "'" as "%27"
 */
static GString *Url_escape_quote(const char *str)
{
   int i;
   GString *gstr = g_string_sized_new(64);

   for (i = 0; str[i]; ++i) {
      if (str[i] == '\'')
         g_string_append(gstr, "%27");
      else
         g_string_append_c(gstr, str[i]);
   }

   return gstr;
}

/*
 * Build a shell command using wget for this URL.
 */
static gchar *make_wget_cmd(gchar *url)
{
   gchar *p, *wcmd = NULL;
   GString *gs_url;

   if ((p = strrchr(url, '/')) != NULL){
      gs_url = Url_escape_quote(url);
      ftpurl_filter_smtp_hack(gs_url);
      wcmd = g_strdup_printf("wget -t 2 -O - '%s' 2>/dev/null", gs_url->str);
      g_string_free(gs_url, TRUE);
   }

   return wcmd;
}

/*
 * Fork, exec command, get its output and redirect to stdout.
 * Return: Number of bytes transfered.
 */
static gint try_ftp_transfer(gchar *wget_cmd, gchar *url)
{
   FILE *in_stream;
   size_t n;
   gint nb, ret;
   char buf[4096];

   if ((in_stream = popen(wget_cmd, "r")) == NULL) {
      perror("popen");
      exit (EXIT_FAILURE);
   }

   /* Read/Write the real data */
   for (nb = 0; (n = fread (buf, 1, 4096, in_stream)) > 0; nb += n) {
      if (nb == 0) {
         /* Send dpip tag */
         sock_handler_printf(sh, 1, 
            "<dpi cmd='start_send_page' url='%s'>", url);
         /* Send HTTP header. */
         // sock_handler_printf("Content-type: text/html\n\n");
         sock_handler_printf(sh, 0, "\n\n");
      }
      sock_handler_write(sh, buf, n, 0);
   }

   if ((ret = pclose(in_stream)) != 0)
      g_printerr("ftp.dpi::pclose: [%d] %s\n", ret, g_strerror(errno));

   return nb;
}

/*
 *
 */
int main(void)
{
   gchar *dpip_tag = NULL, *cmd = NULL, *url = NULL, *url2 = NULL,
         *wget_cmd = NULL, *wget_cmd2 = NULL;
   gint nb;
   gchar *p;

   /* Initialize the SockHandler */
   sh = sock_handler_new(STDIN_FILENO, STDOUT_FILENO, 8*1024);

   /* wget may need to write a temporary file... */
   chdir("/tmp");

   /* Read the dpi command from STDIN */
   dpip_tag = sock_handler_read(sh);
   // g_printerr("[%s]\n", dpip_tag);

   cmd = Get_attr_value(dpip_tag, strlen(dpip_tag), "cmd");
   url = Get_attr_value(dpip_tag, strlen(dpip_tag), "url");
   if (!cmd || !url) {
      g_printerr("ftp.dpi:: Error, cmd=%s, url=%s\n", cmd, url);
      exit (EXIT_FAILURE);
   }

   /* make command string */
   wget_cmd = make_wget_cmd(url);
   g_printerr("ftp.dpi::[%s]\n", wget_cmd);

   if ((nb = try_ftp_transfer(wget_cmd, url)) == 0) {
      /* Transfer failed, the requested file may not exist or be a symlnk
       * to a directory. Try again... */
      if ((p = strrchr(url, '/')) && p[1]) {
         url2 = g_strconcat(url, "/", NULL);
         wget_cmd2 = make_wget_cmd(url2);
         nb = try_ftp_transfer(wget_cmd2, url);
      }
   }

   if (nb == 0) {
      /* The transfer failed, let dillo know... */
      sock_handler_printf(sh, 1,
         "<dpi cmd='answer' to_cmd='open_url' msg='not a directory'>");
   }

   g_free(cmd);
   g_free(url);
   g_free(url2);
   g_free(dpip_tag);
   g_free(wget_cmd);
   g_free(wget_cmd2);

   /* Finish the SockHandler */
   sock_handler_close(sh);
   sock_handler_free(sh);

   return 0;
}

