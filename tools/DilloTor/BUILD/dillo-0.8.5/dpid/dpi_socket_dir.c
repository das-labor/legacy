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
 * Create a per user temporary directory for dpi sockets
 */

#include <errno.h>
#include "dpid_common.h"
#include "dpi.h"
#include "misc_new.h"
#include "dpi_socket_dir.h" /* for function prototypes */

/*! Save socket directory name in ~/.dillo/dpi_socket_dir
 * \Return
 * \li 1 on success
 * \li -1 on failure
 */
int w_dpi_socket_dir(char *dirname, char *sockdir)
{
   FILE *dir;

   if ((dir = fopen(dirname, "w")) == NULL) {
      ERRMSG("w_dpi_socket_dir", "fopen", errno);
      return (-1);
   }
   fprintf(dir, "%s", sockdir);
   fclose(dir);
   return (1);
}

/*! Test that socket directory exists and is a directory
 * \Return
 * \li 1 on success
 * \li 0 on failure
 * \bug Does not check permissions or that it's a symbolic link
 * to another directory.
 */
int tst_dir(char *dir)
{
   char *dirtest;
   gint ret = 0;

   /* test for a directory */
   dirtest = g_strconcat(dir, "/", NULL);
   if (access(dirtest, F_OK) == -1) {
      ERRMSG("tst_dir", "access", errno);
      fprintf(stderr, " - %s\n", dirtest);
   } else {
      ret = 1;
   }
   g_free(dirtest);

   return ret;
}

/*! Create socket directory
 * \Return
 * \li Socket directory path on success
 * \li NULL on failure
 */
char *mk_sockdir(void)
{
   char *username, *template;

   username = a_Misc_get_user();
   template = g_strconcat("/tmp/", username, "-", "XXXXXX", NULL);
   if (a_Misc_mkdtemp(template) == NULL) {
      ERRMSG("mk_sockdir", "a_Misc_mkdtemp", 0);
      fprintf(stderr, " - %s\n", template);
      g_free(template);
      return (NULL);
   }
   return template;
}

/*! Create socket directory if it does not exist and save its name in
 * ~/.dillo/dpi_socket_dir.
 * \Return
 * \li Socket directory name on success
 * \li NULL on failure.
 */
char *init_sockdir(char *dpi_socket_dir)
{
   char *sockdir = NULL;
   int dir_ok = 0;

   if ((sockdir = a_Dpi_rd_dpi_socket_dir(dpi_socket_dir)) == NULL) {
      fprintf(stderr, "debug_msg - init_sockdir: ");
      fprintf(stderr, "The dpi_socket_dir file does not exist\n");
   } else {
      if ((dir_ok = tst_dir(sockdir)) == 1) {
         fprintf(stderr,
                 "debug_msg - init_sockdir: The socket directory ");
         fprintf(stderr, "%s exists and is OK\n", sockdir);
      } else {
         fprintf(stderr,
                 "debug_msg - init_sockdir: The socket directory ");
         fprintf(stderr, "%s does not exist or is not a directory\n",
                 sockdir);
         g_free(sockdir);
      }
   }
   if (!dir_ok) {
      sockdir = mk_sockdir();
      if (sockdir == NULL) {
         ERRMSG("init_sockdir", "mk_sockdir", 0);
         fprintf(stderr, " - Failed to create dpi socket directory\n");
      } else if ((w_dpi_socket_dir(dpi_socket_dir, sockdir)) == -1) {
         ERRMSG("init_sockdir", "w_dpi_socket_dir", 0);
         fprintf(stderr, " - failed to save %s\n", sockdir);
         g_free(sockdir);
         sockdir = NULL;
      }
   }
   return (sockdir);
}
