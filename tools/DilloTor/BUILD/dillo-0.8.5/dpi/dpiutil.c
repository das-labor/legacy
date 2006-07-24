/*
 * File: dpiutil.c
 *
 * Copyright 2004 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include "dpiutil.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <glib.h>

/*
 * Task: given a tag and an attribute name, return its value.
 *       (character stuffing is removed here)
 * Return value: the attribute value, or NULL if not present or malformed.
 * (copied from bookmarks.c)
 */
char *Get_attr_value(char *tag, size_t tagsize, char *attrname)
{
   char *p, *q, *ltag, quote, *start, *val = NULL;

   ltag = g_strndup(tag, tagsize);
   if ((p = strstr(ltag, attrname)) &&
       (p = strchr(p, '=')) &&
       (p = strpbrk(p, "'\"")) ) {
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

/* Streamed Sockets API (not mandatory)  ----------------------------------*/

/*
 * Create and initialize the SockHandler structure
 */ 
SockHandler *sock_handler_new(int fd_in, int fd_out, int flush_sz)
{
   SockHandler *sh = g_new(SockHandler, 1);

   /* init descriptors and streams */
   sh->fd_in  = fd_in;
   sh->fd_out = fd_out;
   sh->out = fdopen(fd_out, "w");

   /* init buffer */
   sh->buf_max = 8 * 1024 + 128;
   sh->buf = g_new(char, sh->buf_max);
   sh->buf_sz = 0;
   sh->flush_sz = flush_sz;

   return sh;
}

/*
 * Streamed write to socket
 * Return: 0 on success, 1 on error.
 */
int sock_handler_write(SockHandler *sh, char *Data, size_t DataSize, int flush)
{
   gint retval = 1;

   /* append to buf */
   while (sh->buf_max < sh->buf_sz + DataSize) {
      sh->buf_max <<= 1;
      sh->buf = g_realloc(sh->buf, sh->buf_max);
   }
   memcpy(sh->buf + sh->buf_sz, Data, DataSize);
   sh->buf_sz += DataSize;

// g_printerr(
//    "sh->buf=%p, sh->buf_sz=%d, sh->buf_max=%d, sh->flush_sz=%d\n",
//    sh->buf, sh->buf_sz, sh->buf_max, sh->flush_sz);

   /* flush data if necessary */
   if (flush || sh->buf_sz >= sh->flush_sz) {
      if (sh->buf_sz && fwrite (sh->buf, sh->buf_sz, 1, sh->out) != 1) {
         perror("[sock_handler_write]");
      } else {
         fflush(sh->out);
         sh->buf_sz = 0;
         retval = 0;
      }

   } else {
      retval = 0;
   }

   return retval;
}

/*
 * Convinience function.
 */
int sock_handler_write_str(SockHandler *sh, char *str, int flush)
{
   return sock_handler_write(sh, str, strlen(str), flush);
}

/*
 * Return a newlly allocated string with the contents read from the socket.
 */
gchar *sock_handler_read(SockHandler *sh)
{
   ssize_t st;
   gchar buf[16384];

   /* can't use fread() */
   do
      st = read(sh->fd_in, buf, 16384);
   while (st < 0 && errno == EINTR);
   
   if (st == -1)
      perror("[sock_handler_read]");

   return (st > 0) ? g_strndup(buf, (guint)st) : NULL;
}

/*
 * Close this socket for reading and writing.
 */
void sock_handler_close(SockHandler *sh)
{
   /* flush before closing */
   sock_handler_write(sh, "", 0, 1);

   fclose(sh->out);
   close(sh->fd_out);
}

/*
 * Free the SockHandler structure
 */
void sock_handler_free(SockHandler *sh)
{
   g_free(sh->buf);
   g_free(sh);
}

/* ------------------------------------------------------------------------ */

