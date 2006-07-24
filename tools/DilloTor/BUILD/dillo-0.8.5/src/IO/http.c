/*
 * File: http.c
 *
 * Copyright (C) 2000, 2001 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * HTTP connect functions
 */


#include <config.h>

#include <unistd.h>
#include <errno.h>              /* for errno */
#include <string.h>             /* for strstr */
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>         /* for lots of socket stuff */
#include <netinet/in.h>         /* for ntohl and stuff */
#include <arpa/inet.h>          /* for inet_ntop */

#include "IO.h"
#include "Url.h"
#include "../klist.h"
#include "../dns.h"
#include "../cache.h"
#include "../web.h"
#include "../interface.h"
#include "../cookies.h"
#include "../prefs.h"
#include "../misc.h"

/* Used to send a message to the bw's status bar */
#define BW_MSG(web, root, fmt...)                                     \
G_STMT_START {                                                        \
   if (a_Web_valid((web)) && (!(root) || (web)->flags & WEB_RootUrl)) \
      a_Interface_msg((web)->bw, fmt);                                \
} G_STMT_END

#define DEBUG_LEVEL 5
#include "../debug.h"



/* 'Url' and 'web' are just references (no need to deallocate them here). */
typedef struct {
   gint SockFD;
   const DilloUrl *Url;    /* reference to original URL */
   guint port;             /* need a separate port in order to support PROXY */
   gboolean use_proxy;     /* indicates whether to use proxy or not */
   DilloWeb *web;          /* reference to client's web structure */
   GSList *addr_list;      /* Holds the DNS answer */
   GSList *addr_list_iter; /* Points to address currently being used */
   GIOChannel *GioCh;      /* GIOChannel to monitor the connecting process */
   gint Err;               /* Holds the errno of the connect() call */
   ChainLink *Info;        /* Used for CCC asynchronous operations */
} SocketData_t;


/*
 * Local data
 */
static Klist_t *ValidSocks = NULL; /* Active sockets list. It holds pointers to
                                    * SocketData_t structures. */

static DilloUrl *HTTP_Proxy = NULL;
static gchar *HTTP_Proxy_Auth_base64 = NULL;

/*
 * Initialize proxy vars.
 */
gint a_Http_init(void)
{
   gchar *env_proxy = getenv("http_proxy");

   if (env_proxy && strlen(env_proxy))
      HTTP_Proxy = a_Url_new(env_proxy, NULL, 0, 0, 0);
   if (!HTTP_Proxy && prefs.http_proxy)
      HTTP_Proxy = a_Url_dup(prefs.http_proxy);

/*  This allows for storing the proxy password in "user:passwd" format
 * in dillorc, but as this constitutes a security problem, it was disabled.
 *
   if (HTTP_Proxy && prefs.http_proxyuser && strchr(prefs.http_proxyuser, ':'))
      HTTP_Proxy_Auth_base64 = a_Misc_encode_base64(prefs.http_proxyuser);
 */
   return 0;
}

/*
 * Tell whether the proxy auth is already set (user:password)
 * Return: 1 Yes, 0 No
 */
gint a_Http_proxy_auth(void)
{
   return (HTTP_Proxy_Auth_base64 ? 1 : 0);
}

/*
 * Activate entered proxy password for HTTP.
 */
void a_Http_set_proxy_passwd(gchar *str)
{
   gchar *http_proxyauth = g_strconcat(prefs.http_proxyuser, ":", str, NULL);
   HTTP_Proxy_Auth_base64 = a_Misc_encode_base64(http_proxyauth);
   g_free(http_proxyauth);
}

/*
 * Forward declarations
 */
static void Http_send_query(ChainLink *Info, SocketData_t *S);
static int Http_connect_socket(ChainLink *Info);

/*
 * Create and init a new SocketData_t struct, insert into ValidSocks,
 * and return a primary key for it.
 */
static gint Http_sock_new(void)
{
   SocketData_t *S = g_new0(SocketData_t, 1);
   return a_Klist_insert(&ValidSocks, (gpointer)S);
}

/*
 * Free SocketData_t struct
 */
static void Http_socket_free(gint SKey)
{
   SocketData_t *S;

   if ((S = a_Klist_get_data(ValidSocks, SKey))) {
      a_Klist_remove(ValidSocks, SKey);
      if (S->GioCh)
        g_io_channel_unref(S->GioCh);
      g_free(S);
   }
}

/*
 * Make the http query string
 */
gchar *a_Http_make_query_str(const DilloUrl *url, gboolean use_proxy)
{
   gchar *str, *ptr, *cookies;
   GString *s_port     = g_string_new(""),
           *query      = g_string_new(""),
           *full_path  = g_string_new(""),
           *proxy_auth = g_string_new("");

   /* Sending the default port in the query may cause a 302-answer.  --Jcid */
   if (URL_PORT(url) && URL_PORT(url) != DILLO_URL_HTTP_PORT)
      g_string_sprintfa(s_port, ":%d", URL_PORT(url));

   if (use_proxy) {
      g_string_sprintfa(full_path, "%s%s",
                        URL_STR(url),
                        (URL_PATH_(url) || URL_QUERY_(url)) ? "" : "/");
      if ((ptr = strrchr(full_path->str, '#')))
         g_string_truncate(full_path, ptr - full_path->str);
      if (HTTP_Proxy_Auth_base64)
         g_string_sprintf(proxy_auth, "Proxy-Authorization: Basic %s\r\n",
                          HTTP_Proxy_Auth_base64);
   } else {
      g_string_sprintfa(full_path, "%s%s%s%s",
                        URL_PATH(url),
                        URL_QUERY_(url) ? "?" : "",
                        URL_QUERY(url),
                        (URL_PATH_(url) || URL_QUERY_(url)) ? "" : "/");
   }

   cookies = a_Cookies_get(url);
   if ( URL_FLAGS(url) & URL_Post ){
      g_string_sprintfa(
         query,
         "POST %s HTTP/1.0\r\n"
         "Host: %s%s\r\n"
         "%s"
         "User-Agent: Dillo/%s\r\n"
         "Cookie2: $Version=\"1\"\r\n"
         "%s"
         "Content-type: application/x-www-form-urlencoded\r\n"
         "Content-length: %ld\r\n"
         "\r\n"
         "%s",
         full_path->str, URL_HOST(url), s_port->str,
         proxy_auth->str, VERSION, cookies,
         (glong)strlen(URL_DATA(url)),
         URL_DATA(url));

   } else {
      g_string_sprintfa(
         query,
         "GET %s HTTP/1.0\r\n"
         "%s"
         "Host: %s%s\r\n"
         "%s"
         "User-Agent: Dillo/%s\r\n"
         "Cookie2: $Version=\"1\"\r\n"
         "%s"
         "\r\n",
         full_path->str,
         (URL_FLAGS(url) & URL_E2EReload) ?
            "Cache-Control: no-cache\r\nPragma: no-cache\r\n" : "",
         URL_HOST(url), s_port->str,
         proxy_auth->str,
         VERSION,
         cookies);
   }
   g_free(cookies);

   str = query->str;
   g_string_free(query, FALSE);
   g_string_free(s_port, TRUE);
   g_string_free(full_path, TRUE);
   g_string_free(proxy_auth, TRUE);
   DEBUG_MSG(4, "Query:\n%s", str);
   return str;
}

/*
 * This function is called after the socket has been successfuly connected,
 * or upon an error condition on the connecting process.
 * Task: use the socket to send the HTTP-query and expect its answer
 */
static gboolean
 Http_use_socket(GIOChannel *src, GIOCondition cond, gpointer data)
{
   ChainLink *Info;
   SocketData_t *S;
   gint SKey = GPOINTER_TO_INT(data);

   DEBUG_MSG(3, "Http_use_socket: %s [errno %d] [GIOcond %d]\n",
             g_strerror(errno), errno, cond);

   /* This check is required because glib may asynchronously
    * call this function with data that's no longer used  --Jcid   */
   if ( !(S = a_Klist_get_data(ValidSocks, SKey)) )
      return FALSE;

   Info = S->Info;
   if ( cond & G_IO_HUP ) {
      DEBUG_MSG(3, "--Connection broken\n");
      /* get rid of S->GioCh */
      g_io_channel_close(S->GioCh);
      g_io_channel_unref(S->GioCh);
      S->GioCh = NULL;
      S->addr_list_iter = S->addr_list_iter->next;
      if (!S->addr_list_iter || Http_connect_socket(Info) < 0) {
         BW_MSG(S->web, 0, "ERROR: unable to connect to remote host");
         Http_socket_free(SKey);
         a_Chain_fcb(OpAbort, Info, NULL, NULL);
      }
   } else if ( S->Err ) {
      BW_MSG(S->web, 0, "ERROR: %s", g_strerror(S->Err));
      DEBUG_MSG(3, "Http_use_socket ERROR: %s\n", g_strerror(S->Err));
      a_Chain_fcb(OpAbort, Info, NULL, NULL);
      g_io_channel_close(S->GioCh);
      Http_socket_free(SKey);
   } else if ( cond & G_IO_OUT ) {
      DEBUG_MSG(3, "--Connection established\n");
      g_io_channel_unref(S->GioCh);
      S->GioCh = NULL;
      Http_send_query(Info, S);
   }
   return FALSE;
}

/*
 * This function gets called after the DNS succeeds solving a hostname.
 * Task: Finish socket setup and start connecting the socket.
 * Return value: 0 on success;  -1 on error.
 */
static int Http_connect_socket(ChainLink *Info)
{
   gint status;
   guint watch_id;
#ifdef ENABLE_IPV6
   struct sockaddr_in6 name;
#else
   struct sockaddr_in name;
#endif
   SocketData_t *S;
   DilloHost *dh;
   socklen_t socket_len = 0;

   S = a_Klist_get_data(ValidSocks, GPOINTER_TO_INT(Info->LocalKey));

   dh = S->addr_list_iter->data;

   if ( (S->SockFD = socket(dh->af, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
      S->Err = errno;
      DEBUG_MSG(5, "Http_connect_socket ERROR: %s\n", g_strerror(errno));
      return -1;
   }
   /* set NONBLOCKING and close on exec. */
   fcntl(S->SockFD, F_SETFL,
         O_NONBLOCK | fcntl(S->SockFD, F_GETFL));
   fcntl(S->SockFD, F_SETFD,
         FD_CLOEXEC | fcntl(S->SockFD, F_GETFD));

   /* Some OSes require this...  */
   memset(&name, 0, sizeof(name));
   /* Set remaining parms. */
   switch (dh->af) {
   case AF_INET:
   {
      struct sockaddr_in *sin = (struct sockaddr_in *)&name;
      socket_len = sizeof(struct sockaddr_in);
      sin->sin_family = dh->af;
      sin->sin_port = S->port ? htons(S->port) : htons(DILLO_URL_HTTP_PORT);
      memcpy(&sin->sin_addr, dh->data, (size_t)dh->alen);
      if (a_Web_valid(S->web) && (S->web->flags & WEB_RootUrl))
         DEBUG_MSG(5, "Connecting to %s\n", inet_ntoa(sin->sin_addr));
      break;
   }
#ifdef ENABLE_IPV6
   case AF_INET6:
   {
      char buf[128];
      struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)&name;
      socket_len = sizeof(struct sockaddr_in6);
      sin6->sin6_family = dh->af;
      sin6->sin6_port = S->port ? htons(S->port) : htons(DILLO_URL_HTTP_PORT);
      memcpy(&sin6->sin6_addr, dh->data, dh->alen);
      inet_ntop(dh->af, dh->data, buf, sizeof(buf));
      if (a_Web_valid(S->web) && (S->web->flags & WEB_RootUrl))
         DEBUG_MSG(5, "Connecting to %s\n", buf);
      break;
   }
#endif
   }

   if (!S->GioCh)
      S->GioCh = g_io_channel_unix_new(S->SockFD);
   watch_id = g_io_add_watch(S->GioCh, G_IO_ERR | G_IO_HUP | G_IO_OUT,
                             Http_use_socket, Info->LocalKey);
   status = connect(S->SockFD, (struct sockaddr *)&name, socket_len);
   if ( status == -1 && errno != EINPROGRESS ) {
      DEBUG_MSG(5, "Http_connect_socket ERROR: %s\n", g_strerror(errno));
      g_io_channel_close(S->GioCh);
      g_source_remove(watch_id);
      S->Err = errno;
      return -1;
   }

   BW_MSG(S->web, 1, "Contacting host...");

   return 0; /* Success */
}


/*
 * Create and submit the HTTP query to the IO engine
 */
static void Http_send_query(ChainLink *Info, SocketData_t *S)
{
   IOData_t *io;
   gchar *query;
   void *link;

   /* Create the query */
   query = a_Http_make_query_str(S->Url, S->use_proxy);

   /* send query */
   BW_MSG(S->web, 1, "Sending query to %s...", URL_HOST_(S->Url));
   io = a_IO_new(IOWrite, S->SockFD);
   a_IO_set_buf(io, query, strlen(query));
   io->Flags |= IOFlag_FreeIOBuf;
   link = a_Chain_link_new(Info, a_Http_ccc, BCK, a_IO_ccc, 1, 2);
   a_Chain_bcb(OpStart, Info, io, NULL);
   a_Chain_bcb(OpSend, Info, io, NULL);

   /* Tell the cache to start the receiving CCC for the answer */
   a_Chain_fcb(OpSend, Info, &S->SockFD, NULL);
}

/*
 * Test proxy settings and check the no_proxy domains list
 * Return value: whether to use proxy or not.
 */
static gint Http_must_use_proxy(const DilloUrl *url)
{
   gint i;
   gchar **NoProxyVec = prefs.no_proxy_vec;

   if (HTTP_Proxy) {
      if (NoProxyVec)
         for (i = 0; NoProxyVec[i]; ++i)
            if (strstr(URL_AUTHORITY(url), NoProxyVec[i]))
               return 0;
      return 1;
   }
   return 0;
}

/*
 * Asynchronously create a new http connection for 'Url'
 * We'll set some socket parameters; the rest will be set later
 * when the IP is known.
 * ( Data1 = Requested Url; Data2 = Web structure )
 * Return value: 0 on success, -1 otherwise
 */
static gint Http_get(ChainLink *Info, void *Data1, void *Data2)
{
   void *link;
   const DilloUrl *Url = Data1;
   SocketData_t *S;
   gchar *hostname;

   S = a_Klist_get_data(ValidSocks, GPOINTER_TO_INT(Info->LocalKey));

   /* Reference Info data */
   S->Info = Info;
   /* Reference Web data */
   S->web = Data2;

   /* Proxy support */
   if (Http_must_use_proxy(Url)) {
      hostname = g_strdup(URL_HOST(HTTP_Proxy));
      S->port = URL_PORT(HTTP_Proxy);
      S->use_proxy = TRUE;
   } else {
      hostname = g_strdup(URL_HOST(Url));
      S->port = URL_PORT(Url);
      S->use_proxy = FALSE;
   }

   /* Set more socket parameters */
   S->Url = Url;

   /* Let the user know what we'll do */
   BW_MSG(S->web, 1, "DNS solving %s", URL_HOST_(S->Url));

   /* Let the DNS engine solve the hostname, and when done,
    * we'll try to connect the socket */
   link = a_Chain_link_new(Info, a_Http_ccc, BCK, a_Dns_ccc, 1, 1);
   a_Chain_bcb(OpStart, Info, hostname, NULL);

   g_free(hostname);
   return 0;
}

/*
 * CCC function for the HTTP module
 */
void a_Http_ccc(int Op, int Branch, int Dir, ChainLink *Info,
                void *Data1, void *Data2)
{
   gint SKey = GPOINTER_TO_INT(Info->LocalKey);
   SocketData_t *S = a_Klist_get_data(ValidSocks, SKey);

   a_Chain_debug_msg("a_Http_ccc", Op, Branch, Dir);

   if ( Branch == 1 ) {
      if (Dir == BCK) {
        /* HTTP query branch */
         switch (Op) {
         case OpStart:
            SKey = Http_sock_new();
            Info->LocalKey = GINT_TO_POINTER(SKey);
            if (Http_get(Info, Data1, Data2) < 0) {
               S = a_Klist_get_data(ValidSocks, SKey);
               BW_MSG(S->web, 1, "ERROR: %s", g_strerror(S->Err));
               Http_socket_free(SKey);
               a_Chain_fcb(OpAbort, Info, NULL, NULL);
            }
            break;
         case OpAbort:
            /* something bad happened... */
            DEBUG_MSG(2, "Http: OpAbort [1B]\n");
            Http_socket_free(SKey);
            a_Chain_bcb(OpAbort, Info, NULL, NULL);
            g_free(Info);
            break;
         }
      } else {  /* FWD */
        /* DNS answer branch */
         switch (Op) {
         case OpSend:
            /* Successful DNS answer; save the IP */
            if (S) {
               S->addr_list = (GSList *)Data1;
               S->addr_list_iter = S->addr_list;
            }
            break;
         case OpEnd:
            if (S) {
               /* Unlink DNS_Info */
               a_Chain_del_link(Info, BCK);
               /* start connecting the socket */
               if (Http_connect_socket(Info) < 0) {
                  BW_MSG(S->web, 1, "ERROR: %s", g_strerror(S->Err));
                  Http_socket_free(SKey);
                  a_Chain_fcb(OpAbort, Info, NULL, NULL);
               }
            }
            break;
         case OpAbort:
            /* DNS wasn't able to resolve the hostname */
            if (S) {
               /* Unlink DNS_Info */
               a_Chain_del_link(Info, BCK);
               BW_MSG(S->web, 0, "ERROR: Dns can't solve %s",
                  (S->use_proxy) ? URL_HOST_(HTTP_Proxy) : URL_HOST_(S->Url));
               Http_socket_free(SKey);
               /* send abort message to higher-level functions */
               a_Chain_fcb(OpAbort, Info, NULL, NULL);
            }
            break;
         }
      }

   } else if ( Branch == 2 ) {
      if (Dir == FWD) {
         /* IO send-query status branch */
         switch (Op) {
         case OpStart:
            /* LocalKey was set by branch 1 */
            break;
         case OpSend:
            /* Not used */
            break;
         case OpEnd:
            /* finished sending the HTTP query */
            if (S) {
               a_Chain_del_link(Info, BCK);
               a_Chain_fcb(OpEnd, Info, &S->SockFD, (void *)S->Url);
               BW_MSG(S->web, 1, "Query sent, waiting for reply...");
               Http_socket_free(SKey);
            }
            break;
         case OpAbort:
            /* something bad happened... */
            /* unlink IO_Info */
            if (S) {
               a_Chain_del_link(Info, BCK);
               Http_socket_free(SKey);
               a_Chain_fcb(OpAbort, Info, NULL, NULL);
            }
            break;
         }
      }
   }
}



/*
 * Deallocate memory used by http module
 * (Call this one at exit time)
 */
void a_Http_freeall(void)
{
   a_Klist_free(&ValidSocks);
   a_Url_free(HTTP_Proxy);
   g_free(HTTP_Proxy_Auth_base64);
}
