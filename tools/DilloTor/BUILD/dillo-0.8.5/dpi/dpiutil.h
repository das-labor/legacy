/* This file contains all of the core utilies which will be used by
dpis written.  This is done to break out common code for faster
bugfixes, and possible conversion to shared library to reduce
code size*/


#include <stdio.h>
#include <glib.h>

#define BUFLEN 256
#define TOUT 300


/* Streamed Sockets API (not mandatory)  ----------------------------------*/

typedef struct _SockHandler SockHandler;
struct _SockHandler {
   gint fd_in;
   gint fd_out;
   // FILE *in;    /* Unused. The stream functions block when reading. */
   FILE *out;

   gchar *buf;     /* internal buffer */
   guint buf_sz;   /* data size */
   guint buf_max;  /* allocated size */
   guint flush_sz; /* max size before flush */
};

SockHandler *sock_handler_new(int fd_in, int fd_out, int flush_sz);
int sock_handler_write(SockHandler *sh, char *Data,size_t DataSize, int flush);
int sock_handler_write_str(SockHandler *sh, char *str, int flush);
gchar *sock_handler_read(SockHandler *sh);
void sock_handler_close(SockHandler *sh);
void sock_handler_free(SockHandler *sh);

#define sock_handler_printf(sh, flush, fmt...)           \
   G_STMT_START {                                        \
      gchar *str = g_strdup_printf(fmt);                 \
      sock_handler_write(sh, str, strlen(str), flush);   \
      g_free(str);                                       \
   } G_STMT_END

/* ----------------------------------------------------------------------- */


/*
 * Task: given a tag and an attribute name, return its value.
 *       (character stuffing is removed here)
 * Return value: the attribute value, or NULL if not present or malformed.
 * (copied from bookmarks.c)
 */
char *Get_attr_value(char *tag, size_t tagsize, char *attrname);

