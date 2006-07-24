#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <glib.h>
#include "dpid_common.h"

#include "misc_new.h"  /* for function prototypes */

/* define to 1 when checking for memory leaks
 * \todo
 * Eliminate need for a_Misc_get_home and a_Misc_get_user when testing for
 * memory leaks by using ld --wrap option to replace g_get_home_dir and
 * g_get_user_name with wrapper functions.
 */
#ifndef TEST
#define TEST 0
#endif


/*
 * Close a FD handling EINTR.
 */
gint a_Misc_close_fd(gint fd)
{
   gint st;

   do {
      st = close(fd);
   } while (st < 0 && errno == EINTR);
   return st;
}

/*
 * Return the user's home directory.
 * Don't free the returned string!
 */
gchar *a_Misc_get_home(void)
{
   gchar *ret;
   ret = (TEST) ? getenv("HOME") : g_get_home_dir();
   return ret;
}

/*
 * Return the user.
 * Don't free the returned string!
 */
gchar *a_Misc_get_user(void)
{
   gchar *ret;
   ret = (TEST) ? getenv("USER") : g_get_user_name();
   return ret;
}

/*
 * Prepend the users home-dir to 'file' string i.e,
 * pass in .dillo/bookmarks.html and it will return
 * /home/imain/.dillo/bookmarks.html
 *
 * Remember to g_free() returned value!
 * copied from misc.c
 */
gchar *a_Misc_prepend_user_home(const char *file)
{
   return (g_strconcat(a_Misc_get_home(), "/", file, NULL));
}

/*!
 * This was copied from dpi/bm_srv12.c
 * - Task: given a tag and an attribute name, return its value.
 *       (character stuffing is removed here)
 * - Return value: the attribute value, or NULL if not present or malformed.
 */
char *a_Misc_get_attr_value(char *tag, size_t tagsize, char *attrname)
{
   char *p, *q, *ltag, quote, *start, *val = NULL;

   ltag = g_strndup(tag, tagsize);
   if ((p = strstr(ltag, attrname)) &&
       (p = strchr(p, '=')) && (p = strpbrk(p, "'\""))) {
      quote = *p;
      start = ++p;
      while ((q = strchr(p, quote)) && q[1] == quote)
         p = q + 2;
      if (q) {
         val = g_strndup(start, (guint)(q - start));
         for (p = q = val; (*q = *p); ++p, ++q)
            if ((*p == '"' || *p == '\'') && p[1] == p[0])
               ++p;
      }
   }
   g_free(ltag);

   return val;
}

/*
 * Read a line of text up to the newline character, store it into a newly
 * allocated string and return it.
 * (copied from dpi/bm_srv12.c)
 */
char *a_Misc_get_line(FILE *stream)
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

/*! Reads a dpi tag from a socket
 * \li Continues after a signal interrupt
 * \Return
 * Gstring pointer to tag on success, NULL on failure
 * \important Caller is responsible for freeing the returned GString *
 */
GString *a_Misc_rdtag(int socket)
{
   char c = '\0';
   ssize_t rdlen;
   GString *tag;

   tag = g_string_new(NULL);

   errno = 0;

   do {
      rdlen = read(socket, &c, 1);
      if (rdlen == -1 && errno != EINTR)
         break;
      g_string_append_c(tag, c);
   } while (c != '>');

   if (rdlen == -1) {
      perror("a_Misc_rdtag");
      g_string_free(tag, TRUE);
      return (NULL);
   }
   return (tag);
}

/*!
 * Read a dpi tag from sock
 * \return
 * pointer to dynamically allocated request tag
 */
char *a_Misc_readtag(int sock)
{
   char *tag, c, buf[10];
   size_t buflen, i;
   size_t taglen = 0, tagmem = 10;
   ssize_t rdln = 1;

   tag = NULL;
   buf[0] = '\0';
   buflen = sizeof(buf) / sizeof(buf[0]);
   // new start
   tag = (char *) g_malloc(tagmem + 1);
   for (i = 0; (rdln = read(sock, &c, 1)) != 0; i++) {
      if (i == tagmem) {
         tagmem += tagmem;
         tag = (char *) g_realloc(tag, tagmem + 1);
      }
      tag[i] = c;
      taglen += rdln;
      if (c == '>') {
         tag[i + 1] = '\0';
         break;
      }
   }
   // new end
   if (rdln == -1) {
      ERRMSG("a_Misc_readtag", "read", errno);
   }

   return (tag);
}

/*! Reads a dpi tag from a socket without hanging on read.
 * \li Continues after a signal interrupt
 * \Return
 * \li 1 on success
 * \li 0 if input is not available within timeout microseconds.
 * \li -1 on failure
 * \important Caller is responsible for freeing the returned GString *
 */
/* Is this useful?
int a_Misc_nohang_rdtag(int socket, int timeout, GString **tag)
{
   int n_fd;
   fd_set sock_set, select_set;
   struct timeval tout;

   FD_ZERO(&sock_set);
   FD_SET(socket, &sock_set);

   errno = 0;
   do {
      select_set = sock_set;
      tout.tv_sec = 0;
      tout.tv_usec = timeout;
      n_fd = select(socket + 1, &select_set, NULL, NULL, &tout);
   } while (n_fd == -1 && errno == EINTR);

   if (n_fd == -1) {
      fprintf(stderr, "%s:%d: a_Misc_nohang_rdtag: %s\n",
              __FILE__, __LINE__, g_strerror(errno));
      return(-1);
   }
   if (n_fd == 0) {
      return(0);
   } else {
      *tag = a_Misc_rdtag(socket);
      return(1);
   }
}
*/

/*
 * Alternative to mkdtemp().
 * Not as strong as mkdtemp, but enough for creating a directory.
 * (adapted from dietlibc)
 */
char *a_Misc_mkdtemp(char *template)
{
   char *tmp = template + strlen(template) - 6;
   int i;
   unsigned int random;

   if (tmp < template)
      goto error;
   for (i = 0; i < 6; ++i)
      if (tmp[i] != 'X') {
       error:
         errno = EINVAL;
         return 0;
      }
   srand((guint)(time(0) ^ getpid()));
   for (;;) {
      random = (unsigned) rand();
      for (i = 0; i < 6; ++i) {
         int hexdigit = (random >> (i * 5)) & 0x1f;

         tmp[i] = hexdigit > 9 ? hexdigit + 'a' - 10 : hexdigit + '0';
      }
      if (mkdir(template, 0700) == 0)
         break;
      if (errno == EEXIST)
         continue;
      return 0;
   }
   return template;
}
