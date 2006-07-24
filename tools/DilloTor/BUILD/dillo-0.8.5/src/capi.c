/*
 * File: capi.c
 *
 * Copyright 2002 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Cache API
 * This is the module that manages the cache and starts the CCC chains
 * to get the requests served. Kind of a broker.
 */

#include <string.h>
#include <unistd.h>   /* for pipe */

#include "msg.h"
#include "capi.h"
#include "IO/Url.h"
#include "chain.h"
#include "list.h"
#include "interface.h"
#include "history.h"
#include "nav.h"
#include "misc.h"
#include "dpiapi.h"

/* for testing dpi chat */
#include "bookmark.h"

#define DEBUG_LEVEL 5
#include "debug.h"

typedef struct {
   DilloWeb *web;
   DilloUrl *url; /* local copy of web->url. Used when the latter is freed */
   void *bw;
   gchar *server;
   gchar *datastr;
   gint SockFD;
   gint Flags;
   gint DpiPipe[2];
   ChainLink *InfoSend;
   ChainLink *InfoRecv;
   ChainLink *InfoPipe;

   gint Ref;
} dpi_conn_t;

/* Flags for conn */
enum {
   PENDING = 1,
   TIMEOUT = 2,  /* unused */
   ABORTED = 4
};

/*
 * Local data
 */
/* Data list for active dpi connections */
static dpi_conn_t **DpiConn = NULL;
static gint DpiConnSize;
static gint DpiConnMax = 4;

/* ID for the timeout function that waits for dpid to start */
static guint dpi_conn_timeout_id = 0;

/*
 * Forward declarations
 */
void a_Capi_ccc(int Op, int Branch, int Dir, ChainLink *Info,
                void *Data1, void *Data2);


/* ------------------------------------------------------------------------- */

/*
 * Create a new connection data structure
 */
static dpi_conn_t *
 Capi_dpi_conn_new(DilloWeb *web, void *bw, char *server, gchar *datastr)
{
   dpi_conn_t *conn;

   conn = g_new(dpi_conn_t, 1);
   conn->web = web;
   conn->url = (web) ? a_Url_dup(web->url) : NULL;
   conn->bw = bw;
   conn->server = g_strdup(server);
   conn->datastr = g_strdup(datastr);
   conn->SockFD = -1;
   conn->Flags = PENDING;
   conn->InfoSend = a_Chain_new();
   conn->InfoRecv = NULL;   /* will be set later */
   conn->InfoPipe = NULL;   /* may be set later */
   conn->Ref = 0;           /* Reference count */
   return conn;
}

/*
 * Increment the reference count and add to the list if not present
 */
static void Capi_dpi_conn_ref(dpi_conn_t *conn)
{
   if (++conn->Ref == 1) {
      /* add the connection data to list */
      a_List_add(DpiConn, DpiConnSize, DpiConnMax);
      DpiConn[DpiConnSize] = conn;
      DpiConnSize++;
   }
}

/*
 * Decrement the reference count (and remove from list when zero)
 */
static void Capi_dpi_conn_unref(dpi_conn_t *conn)
{
   gint i, j;

   --conn->Ref;
   if (conn->Ref == 0) {
      for (i = 0; i < DpiConnSize; ++i)
         if (DpiConn[i] == conn) {
            /* remove conn preserving the list order */
            for (j = i; j + 1 < DpiConnSize; ++j)
               DpiConn[j] = DpiConn[j + 1];
            --DpiConnSize;
            /* free dynamic memory */
            a_Url_free(conn->url);
            g_free(conn->server);
            g_free(conn->datastr);
            g_free(conn);
            break;
         }
   }
}

/*
 * Find connection data by server
 */
static dpi_conn_t *Capi_dpi_conn_find(gchar *server)
{
   gint i;

   for (i = 0; i < DpiConnSize; ++i)
      if (strcmp(server, DpiConn[i]->server) == 0)
         return DpiConn[i];

   return NULL;
}

/*
 * Resume connections that were waiting for dpid to start.
 */
static void Capi_dpi_conn_resume(void)
{
   gint i;
   DataBuf *dbuf;

   for (i = 0; i < DpiConnSize; ++i)
      if (DpiConn[i]->Flags & PENDING) {
         dpi_conn_t *conn = DpiConn[i];
         dbuf = a_Chain_dbuf_new(conn->datastr,(gint)strlen(conn->datastr), 0);
         a_Capi_ccc(OpSend, 1, BCK, conn->InfoSend, dbuf, NULL);
         g_free(dbuf);
         conn->Flags &= ~PENDING;
      }
}

/*
 * Test dpid and resume connections if it already started.
 */
static gint Capi_dpi_conn_timeout(gpointer data)
{
   static gint try = 0;
   gint i;

   ++try;
   DEBUG_MSG(5, "Capi_dpi_conn_timeout:: try %d\n", try);

   for (i = 0; i < DpiConnSize; ++i)
      if (DpiConn[i]->Flags & PENDING) {
         dpi_conn_t *conn = DpiConn[i];
         a_Chain_bcb(OpStart, conn->InfoSend, conn->server, NULL);
         break;
      }

   for (i = 0; i < DpiConnSize; ++i)
      if (DpiConn[i]->Flags & PENDING)
         return TRUE;
   try = 0;
   dpi_conn_timeout_id = 0;
   return FALSE;
}

/* ------------------------------------------------------------------------- */

/*
 * Safety test: only allow dpi-urls from dpi-generated pages.
 */
static gint Capi_verify_dpi_url_request(DilloWeb *web)
{
   DilloUrl *referer;
   gint allow = FALSE;

   /* test POST and GET */
   if (strchr(URL_STR(web->url), '?') || URL_DATA_(web->url)) {
      /* safety measure: only allow dpi requests from dpi-generated urls */
      if (a_Nav_stack_size(web->bw)) {
         referer = a_History_get_url(NAV_TOP(web->bw));
         if (g_strncasecmp(URL_STR(referer), "dpi:/", 5) == 0)
            allow = TRUE;
      }
   } else {
      allow = TRUE;
   }

   if (!allow) {
      MSG("Capi_verify_dpi_url_request: Permission Denied!\n");
      MSG("  URL_STR : %s\n", URL_STR(web->url));
      if (URL_DATA_(web->url))
         MSG("  URL_DATA: %s\n", URL_DATA(web->url));
   }
   return allow;
}

/*
 * If the url belongs to a dpi server, return its name.
 */
gint a_Capi_url_uses_dpi(gchar *url_str, gchar **server_ptr)
{
   gchar *p, *server = NULL;

   if (g_strncasecmp(url_str, "dpi:/", 5) == 0) {
      /* dpi prefix, get this server's name */
      if ((p = strchr(url_str + 5, '/')) != NULL)
         server = g_strndup(url_str + 5, (guint)(p - url_str - 5));
      else
         server = g_strdup("?");

      if (strcmp(server, "bm") == 0) {
         g_free(server);
         server = g_strdup("bookmarks");
      }

   } else if (g_strncasecmp(url_str, "ftp:/", 5) == 0) {
      server = g_strdup("ftp");

   } else if (g_strncasecmp(url_str, "https:/", 7) == 0) {
      server = g_strdup("https");
   } else if (g_strncasecmp(url_str, "file:", 5) == 0) {
      server = g_strdup("file");
   }

   return ((*server_ptr = server) ? 1 : 0);
}

/*
 * Build the dpip command tag, according to URL and server.
 * todo: make it PROXY-aware (AFAIS, it should be easy)
 */
static gchar *Capi_dpi_build_cmd(DilloUrl *url, gchar *server)
{
   gchar *cmd, *http_query, *s_http_query, *s_url;

   if (strcmp(server, "https") == 0) {
      /* Let's be kind and make the HTTP query string for the dpi */
      http_query = a_Http_make_query_str(url, FALSE);
      s_http_query = a_Misc_stuff_chars(http_query, "'\"");

      cmd = g_strdup_printf("<dpi cmd='open_url' url='%s' query='%s'>",
                            URL_STR(url), s_http_query);
      g_free(s_http_query);
      g_free(http_query);

   } else {
      /* For everyone else, the url_str is enough... */
      s_url = a_Misc_stuff_chars(URL_STR(url), "'\"");
      cmd = g_strdup_printf("<dpi cmd='open_url' url='%s'>", s_url);
      g_free(s_url);
   }
   return cmd;
}

/*
 * Most used function.
 * todo: clean up the ad-hoc bindings with an API that allows dynamic
 *       addition of new plugins.
 */
gint a_Capi_open_url(DilloWeb *web, CA_Callback_t Call, void *CbData)
{
   gint buf_size;
   char *cmd, *server, *buf;
   char *url_str = URL_STR(web->url);
   gint use_cache = 0, safe = 0, reload = 0, ret = 0;

   _MSG("  a_Capi_open_url:: web->Image=%p\n", web->Image);

   if (a_Capi_url_uses_dpi(url_str, &server) && !Call) {

      _MSG("  url_str = %s\n", url_str);

      if (g_strncasecmp(url_str, "dpi:/", 5) == 0) {
         /* safety check... */
         safe = Capi_verify_dpi_url_request(web);
         /* make "dpi:/" prefixed urls always reload. */
         a_Url_set_flags(web->url, URL_FLAGS(web->url) | URL_E2EReload);
         reload = 1;
      } else {
         /* reload test */
         reload = (!a_Capi_get_buf(web->url, &buf, &buf_size) ||
                   (URL_FLAGS(web->url) & URL_E2EReload));
         safe = 1;
      }

      _MSG("  reload=%d URL_E2EReload=%d\n", reload,
           (URL_FLAGS(web->url) & URL_E2EReload));

      if (safe && reload) {
         /* Send dpip command */
         cmd = Capi_dpi_build_cmd(web->url, server);
         a_Capi_dpi_send_cmd(web, web->bw, cmd, server, 1);
         g_free(cmd);

         /* test the new dpi-cache connection! */
         use_cache = 1;

      } else if (safe && !reload) {
         use_cache = 1;
      }
      g_free(server);

   } else {
      use_cache = 1;
   }

   if (use_cache)
      ret = a_Cache_open_url(web, Call, CbData);
   else
      a_Web_free(web);

   return ret;
}

/*
 * Get the cache's buffer for the URL, and its size.
 * Return: 1 cached, 0 not cached.
 */
gint a_Capi_get_buf(const DilloUrl *Url, gchar **PBuf, gint *BufSize)
{
   return a_Cache_get_buf(Url, PBuf, BufSize);
}

/*
 * Send a dpi cmd.
 * (For instance: add_bookmark, open_url, send_preferences, ...)
 */
gint a_Capi_dpi_send_cmd(DilloWeb *web, void *bw, char *cmd, char *server,
                         gint flags)
{
   dpi_conn_t *conn;
   DataBuf *dbuf;

   //**/sleep(1);
   //**/a_Interface_msg(bw, cmd);

   if (flags & 1) {
      /* open a new connection to server */

      /* Create a new connection data struct and add it to the list */
      conn = Capi_dpi_conn_new(web, bw, server, cmd);
      /* start the CCC operations */
      a_Capi_ccc(OpStart, 1, BCK, conn->InfoSend, conn, server);

   } else {
      /* Re-use an open connection */
      conn = Capi_dpi_conn_find(server);
      if (conn) {
         /* found */
         dbuf = a_Chain_dbuf_new(cmd, (gint)strlen(cmd), 0);
         a_Capi_ccc(OpSend, 1, BCK, conn->InfoSend, dbuf, NULL);
         g_free(dbuf);
      } else {
         MSG(" ERROR: [a_Capi_dpi_send_cmd] No open connection found\n");
      }
   }

   return 0;
}


/*
 * CCC function for the CAPI module
 */
void a_Capi_ccc(int Op, int Branch, int Dir, ChainLink *Info,
                void *Data1, void *Data2)
{
   dpi_conn_t *conn;

   a_Chain_debug_msg("a_Capi_ccc", Op, Branch, Dir);

   if (Branch == 1) {
      if (Dir == BCK) {
         /* Command sending branch */
         switch (Op) {
         case OpStart:
            conn = Data1;
            Capi_dpi_conn_ref(conn);
            Info->LocalKey = Data1;
            a_Chain_link_new(Info, a_Capi_ccc, BCK, a_Dpi_ccc, 2, 1);
            a_Chain_bcb(OpStart, Info, Data2, NULL);
            break;
         case OpSend:
            a_Chain_bcb(OpSend, Info, Data1, NULL);
            break;
         case OpEnd:
            a_Chain_bcb(OpEnd, Info, NULL, NULL);
            Capi_dpi_conn_unref(Info->LocalKey);
            g_free(Info);
            break;
         case OpStop:
         case OpAbort:
            MSG(" Not implemented\n");
            break;
         }
      } else {  /* FWD */
         /* Command sending branch (status) */
         switch (Op) {
         case OpSend:
            if (!Data2) {
               g_warning("Capi.c: Opsend [1F] Data2 = NULL\n");
            } else if (strcmp(Data2, "SockFD") == 0) {
               /* start the receiving branch */
               dpi_conn_t *conn = Info->LocalKey;
               conn->SockFD = *(int*)Data1;
               a_Capi_ccc(OpStart, 2, BCK, a_Chain_new(),Info->LocalKey, NULL);
            } else if (strcmp(Data2, "DpidOK") == 0) {
               /* send the data inmediatly! */
               Capi_dpi_conn_resume();
            } else if (strcmp(Data2, "DpidEAGAIN") == 0) {
               /* set a timeout function to retry later... */
               if (dpi_conn_timeout_id == 0)
                  dpi_conn_timeout_id =
                     gtk_timeout_add(250, (GtkFunction) Capi_dpi_conn_timeout,
                                     NULL);
            }
            break;
         case OpStop:
         case OpAbort:
            conn = Info->LocalKey;
            if (Data1 && !strcmp(Data1, "ERR_dpid"))
               a_Interface_msg(conn->bw, "ERROR: can't start dpid daemon!");
            Capi_dpi_conn_unref(conn);
            g_free(Info);
            break;
         }
      }

   } else if (Branch == 2) {
      if (Dir == BCK) {
         /* Server listening branch (status) */
         switch (Op) {
         case OpStart:
            {
            dpi_conn_t *conn = Data1;
            Capi_dpi_conn_ref(conn);
            Info->LocalKey = Data1;
            conn->InfoRecv = Info;
            a_Chain_link_new(Info, a_Capi_ccc, BCK, a_Dpi_ccc, 3, 2);
            a_Chain_bcb(OpStart, Info, &conn->SockFD, "SockFD");
            break;
            }
         case OpStop:
         case OpAbort:
            Capi_dpi_conn_unref(Info->LocalKey);
            MSG(" Not implemented\n");
            break;
         }
      } else {  /* FWD */
         /* Server listening branch */
         switch (Op) {
         case OpSend:
            conn = Info->LocalKey;
            if (conn->Flags & ABORTED ||
                (conn->web && !a_Web_valid(conn->web))) {
               /* there's no client for this transfer!*/
               _MSG("  ** Capi 2F catched an invalid 'web' structure\n");
               /* this flag is used just in case the same memory address
                * is reused for a new 'web' and the test passes */
               conn->Flags &= ABORTED;
               /* Make the dpi module stop this transfer */
               a_Chain_bcb(OpStop, Info, conn->url, NULL);

            } else if (strcmp(Data2, "send_status_message") == 0) {
               a_Interface_msg(conn->bw, "%s", Data1);
            } else if (strcmp(Data2, "chat") == 0) {
               a_Interface_msg(conn->bw, "%s", Data1);
               a_Bookmarks_chat_add(NULL, NULL, Data1);
            } else if (strcmp(Data2, "dialog") == 0) {
               a_Dpiapi_dialog(conn->bw, conn->server, Data1);
            } else if (strcmp(Data2, "start_send_page") == 0) {
               /* start the pipe-writing chain */
               a_Capi_ccc(OpStart, 3, BCK, a_Chain_new(), Info->LocalKey,NULL);
               /* let the dpi know the reading end of the pipe */
               a_Chain_bcb(OpSend, Info, &conn->DpiPipe[0], conn->web->url);
            } else if (strcmp(Data2, "send_page_2eof") == 0) {
               a_Capi_ccc(OpSend, 3, BCK, conn->InfoPipe, Data1, NULL);
            } else if (strcmp(Data2, "reload_request") == 0) {
               a_Nav_reload(conn->bw);
            }
            break;
         case OpEnd:
            {
            dpi_conn_t *conn = Info->LocalKey;
            a_Chain_del_link(Info, BCK);
            conn->InfoRecv = NULL;
            if (conn->InfoSend) {
               /* Propagate OpEnd to the sending branch too */
               a_Capi_ccc(OpEnd, 1, BCK, conn->InfoSend, NULL, NULL);
            }
            if (conn->InfoPipe) {
               /* Propagate OpEnd to the pipe branch too */
               a_Capi_ccc(OpEnd, 3, BCK, conn->InfoPipe, NULL, NULL);
            }
            Capi_dpi_conn_unref(conn);
            g_free(Info);
            break;
            }
         case OpStop:
         case OpAbort:
            MSG(" Not implemented\n");
            break;
         }
      }

   } else if (Branch == 3) {
      if (Dir == BCK) {
         /* Pipe writing branch */
         switch (Op) {
         case OpStart:
            {
            dpi_conn_t *conn = Data1;
            Info->LocalKey = Data1;
            Capi_dpi_conn_ref(conn);
            conn->InfoPipe = Info;
            if (pipe(conn->DpiPipe)) {
               MSG(" Error with pipe\n");
               return;
            }
            a_Chain_link_new(Info, a_Capi_ccc, BCK, a_IO_ccc, 3, 3);
            a_Chain_bcb(OpStart, Info, &conn->DpiPipe[1], "SockFD");
            break;
            }
         case OpSend:
            a_Chain_bcb(OpSend, Info, Data1, NULL);
            break;
         case OpEnd:
            a_Chain_bcb(OpEnd, Info, (void*)1, NULL);
            Capi_dpi_conn_unref(Info->LocalKey);
            g_free(Info);
            break;
         case OpStop:
         case OpAbort:
            Capi_dpi_conn_unref(Info->LocalKey);
            MSG(" Not implemented\n");
            break;
         }
      } else {  /* FWD */
         /* Pipe branch (status) */
      }
   }
}
