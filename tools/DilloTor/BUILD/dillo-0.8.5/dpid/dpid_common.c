#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include "dpid_common.h"

/*
 * Send a verbose error message.
 */
void errmsg(char *caller, char *called, int errornum, char *file, int line)
{
   fprintf(stderr, "%s:%d: %s: %s : ", file, line, caller, called);
   if (errornum > 0)
      fprintf(stderr, "%s\n", g_strerror(errornum));
}

/*! Selector function for scandir
 * Do not scan files starting with '.'
 */
int no_dotfiles(const struct dirent *filedat)
{
   if (filedat->d_name[0] == '.')
      return 0;
   else
      return 1;
}

/*!
 * Provides an error checked write command.
 * Call this via the CKD_WRITE macro
 * \return write return value
 */
ssize_t ckd_write(int fd, char *msg, char *file, int line)
{
   ssize_t ret;

   do {
      ret = write(fd, msg, strlen(msg));
   } while (ret == -1 && errno == EINTR);
   if (ret == -1) {
      fprintf(stderr, "%s:%d: write: %s\n", file, line, g_strerror(errno));
   }
   return (ret);
}
