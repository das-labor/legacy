/*
   Copyright (C) 2003  Ferdi Franceschini <ferdif@optusnet.com.au>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*! \file
 * \todo
 * This module should be removed because its original functions
 * have been removed or modified.
 * Put these functions in dpid.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <glib.h>
#include "dpid_common.h"
#include "dpid.h"

#ifdef TEST
#include "testdat.h"
#endif

/* exported functions */
char *get_dpi_dir(char *dpidrc);


/*
 * Read a line of text up to the newline character, store it into a newly
 * allocated string and return it.
 * \Note
 * Copied from dpi/bookmarks.c
 */
static char *Get_line(FILE *stream)
{
   guint i, size = 64;
   int ch;
   char *buf;

   buf = g_new(char, size);

   for (i = 0; (ch = fgetc(stream)) != EOF; ++i) {
      if (i + 1 == size) {
         size *= 2;
         buf = g_realloc(buf, size);
      }
      if ((buf[i] = ch) == '\n' && ++i)
         break;
   }
   buf[i] = 0;

   if (i > 0) {
      buf = g_realloc(buf, i + 1);
   } else {
      g_free(buf);
      buf = NULL;
   }
   return buf;
}

/*! Get dpi directory path from dpidrc
 * \Return
 * dpi directory on success, NULL on failure
 * \Important
 * The dpi_dir definition in dpidrc must have no leading white space.
 */
char *get_dpi_dir(char *dpidrc)
{
   FILE *In;
   gint len;
   char *rcline = NULL, *value = NULL, *p;

   if ((In = fopen(dpidrc, "r")) == NULL) {
      ERRMSG("dpi_dir", "fopen", errno);
      fprintf(stderr, " - %s\n", dpidrc);
      return (NULL);
   }

   while ((rcline = Get_line(In)) != NULL) {
      if (strncmp(rcline, "dpi_dir", 7) == 0)
         break;
      g_free(rcline);
   }
   fclose(In);

   if (!rcline) {
      ERRMSG("dpi_dir", "Failed to find a dpi_dir entry in dpidrc", 0);
      fprintf(stderr, "Put your dillo plugins path in %s\n", dpidrc);
      fprintf(stderr, "eg. dpi_dir=/usr/local/lib/dillo/dpi ");
      fprintf(stderr, "with no leading spaces.\n");
      value = NULL;
   } else {
      len = (int) strlen(rcline);
      if (len && rcline[len - 1] == '\n')
         rcline[len - 1] = 0;

      if ((p = strchr(rcline, '='))) {
         while (*++p == ' ');
         value = g_strdup(p);
      } else {
         ERRMSG("dpi_dir", "strchr", 0);
         fprintf(stderr, " - '=' not found in %s\n", rcline);
         value = NULL;
      }
   }

   if (rcline)
      g_free(rcline);
   return (value);
}

/*! Send the list of available dpi IDs to a client
 * \Return
 * 1 on success, -1 on failure.
 *
static int send_service_list(int sock, struct dp *dpi_attr_list, int srv_num)
{
   int i;
   char *start_tag = "<dpi cmd='send_data' msg='", *end_tag = "'>";
   gchar *buf;
   ssize_t wlen = 0;

   for (i = 0; i < srv_num && wlen != -1; i++) {
      buf = g_strconcat(start_tag, dpi_attr_list[i].id, end_tag, NULL);
      wlen = write(sock, buf, strlen(buf));
      g_free(buf);
   }
   if (wlen == -1) {
      ERRMSG("send_service_list", "write", errno);
      return (-1);
   }
   return (1);
}
 */
