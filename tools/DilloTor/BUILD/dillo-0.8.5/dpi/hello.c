/*
 * Dpi for "Hello World".
 *
 * This server is an example. Play with it and modify to your taste.
 *
 * Copyright 2003 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <glib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "dpiutil.h"

/*---------------------------------------------------------------------------*/

/*
 * Escape unsafe characters as html entities.
 * Return value: NULL if there's no need to escape, New string otherwise.
 * (borrowed from Dillo's file.c)
 */
static gchar *Html_escape(const gchar *str)
{
   static const char *unsafe_chars = "&<>\"'";
   static const char *unsafe_rep[] =
     { "&amp;", "&lt;", "&gt;", "&quot;", "&#39;" };

   gchar *p;
   GString *gstr;
   gint i;

   if ((p = strpbrk(str, unsafe_chars))) {
      gstr = g_string_sized_new(64);

      for (i = 0; str[i]; ++i) {
         if ((p = strchr(unsafe_chars, str[i])))
            g_string_append(gstr, unsafe_rep[p - unsafe_chars]);
         else
            g_string_append_c(gstr, str[i]);
      }
      p = gstr->str;
      g_string_free(gstr, FALSE);
   }
   return p;
}


/*
 *
 */
int main(void)
{
   FILE *in_stream;
   SockHandler *sh;
   gchar *dpip_tag, *cmd = NULL, *url = NULL, *child_cmd = NULL;
   gchar *esc_tag;
   size_t n;
   gint ret;
   gchar buf[4096];
   gchar *choice[] = {"Window was closed", "YES", "NO",
                      "COULD BE", "HUH?", "CANCEL"};
   gint choice_num;

   g_printerr("hello.dpi:: starting...\n");

   /* Initialize the SockHandler */
   sh = sock_handler_new(STDIN_FILENO, STDOUT_FILENO, 2*1024);

   /* Read the dpi command from STDIN */
   dpip_tag = sock_handler_read(sh);
   g_printerr("[%s]\n", dpip_tag);

   cmd = Get_attr_value(dpip_tag, strlen(dpip_tag), "cmd");
   url = Get_attr_value(dpip_tag, strlen(dpip_tag), "url");

//-- Dialog part
{
   gchar *dpip_tag2, *dialog_msg;

   /* Let's confirm the request */
   /* NOTE: you can send less alternatives (e.g. only alt1 and alt2) */
   sock_handler_printf(sh, 1,
      "<dpi cmd='dialog' msg='%s' %s>",
      "Do you want to see the hello page?",
      "alt1='Yes' alt2='No' alt3='Could be', alt4='Huh?' alt5='Cancel'");

   /* Get the answer */
   dpip_tag2 = sock_handler_read(sh);
   g_printerr("[%s]\n", dpip_tag2);

   /* Get "msg" value */
   dialog_msg = Get_attr_value(dpip_tag2, strlen(dpip_tag2), "msg");
   choice_num = 0;
   if (dialog_msg)
      choice_num = *dialog_msg - '0';

   g_free(dialog_msg);
   g_free(dpip_tag2);
}
//-- EOD part

   /* Start sending our answer */
   sock_handler_printf(sh, 0,
      "<dpi cmd='start_send_page' url='%s'>"
      "Content-type: text/html\n\n"
      "<html>\n"
      "<head><title>Simple dpi test page (hello.dpi)</title></head>\n"
      "<body><hr><h1>Hello world!</h1><hr>\n<br><br>\n",
      url);

   /* Show the choice received with the dialog */
   sock_handler_printf(sh, 0,
      "<hr>\n"
      "<table width='100%%' border='1' bgcolor='burlywood'><tr><td>\n"
      "<big><em>Dialog question:</em> Do you want to see the hello page?<br>\n"
      "<em>Answer received:</em> <b>%s</b></big> </table>\n"
      "<hr>\n",
      choice[choice_num]);

   /* Show the dpip tag we received */
   esc_tag = Html_escape(dpip_tag);
   sock_handler_printf(sh, 0,
      "<h3>dpip tag received:</h3>\n"
      "<pre>\n%s</pre>\n"
      "<br><small>(<b>dpip:</b> dpi protocol)</small><br><br><br>\n",
      esc_tag);
   g_free(esc_tag);


   /* Now something more interesting,
    * fork a command and show its feedback */
   if (cmd && url) {
      child_cmd = g_strdup("date -R");
      g_printerr("[%s]\n", child_cmd);

      /* Fork, exec command, get its output and answer */
      if ((in_stream = popen(child_cmd, "r")) == NULL) {
         perror("popen");
         return EXIT_FAILURE;
      }

      sock_handler_printf(sh, 0, "<h3>date:</h3>\n");
      sock_handler_printf(sh, 0, "<pre>\n");

      /* Read/Write */
      while ((n = fread (buf, 1, 4096, in_stream)) > 0) {
         sock_handler_write(sh, buf, n, 0);
      }                   

      sock_handler_printf(sh, 0, "</pre>\n");

      if ((ret = pclose(in_stream)) != 0)
         g_printerr("popen: [%d]\n", ret);

      g_free(child_cmd);
   }

   sock_handler_printf(sh, 1, "</body></html>\n");

   g_free(cmd);
   g_free(url);
   g_free(dpip_tag);

   /* Finish the SockHandler */
   sock_handler_close(sh);
   sock_handler_free(sh);

   return 0;
}

