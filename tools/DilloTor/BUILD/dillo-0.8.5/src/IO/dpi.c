/*
 * File: dpi.c
 *
 * Copyright (C) 2002, 2003 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * (Prototype code)
 *
 * Dillo plugins (small programs that interact with dillo)
 * This should be able to handle:
 *   bookmarks, cookies, FTP, downloads, preferences, https and
 *   a lot of any-to-html filters.
 */


#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>           /* for errno */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../msg.h"
#include "IO.h"
#include "Url.h"
#include "../misc.h"

//#define DEBUG_LEVEL 2
#define DEBUG_LEVEL 4
#include "../debug.h"

/* This one is tricky, some sources state it should include the byte
 * for the terminating NULL, and others say it shouldn't. */
# define D_SUN_LEN(ptr) ((size_t) (((struct sockaddr_un *) 0)->sun_path) \
                        + strlen ((ptr)->sun_path))

/* Solaris may not have this one... */
#ifndef AF_LOCAL
#define AF_LOCAL AF_UNIX
#endif


typedef struct {
   gint FreeBuf;
   gint InTag, InData;
   gint PipeActive;
   gint Send2EOF;

   gint DataTotalSize;
   gint DataRecvSize;

   void *Buf;
   gint BufIdx;
   gint BufSize;

   gchar *Tok;
   gint TokSize;
   gint TokIsTag;

   ChainLink *InfoRecv;
} conn_data_t;


/*
 * Local data
 */

/* list for dpi connections waiting for a FD;
 * it holds pointers to Info structures (i.e. CCC Nodes). */
static GSList *PendingNodes = NULL;

/*
 * Forward references
 */


/*
 * Close a FD handling EINTR
 */
static void Dpi_close_fd(gint fd)
{
   gint st;

   do
      st = close(fd);
   while (st < 0 && errno == EINTR);
}

/*
 * Task: given a tag and an attribute name, return its value.
 *       (character stuffing is removed here)
 * Return value: the attribute value, or NULL if not present or malformed.
 */
static char *Get_attr_value(char *tag, gint tagsize, char *attrname)
{
   char *p, *q, *ltag, quote, *start, *val = NULL;

   ltag = g_strndup(tag, (guint)tagsize);
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

/*
 * Create a new connection data structure
 */
static conn_data_t *Dpi_conn_data_new(ChainLink *Info)
{
   conn_data_t *conn = g_new0(conn_data_t, 1);

   conn->Buf = NULL;
   conn->Tok = NULL;
   conn->InfoRecv = Info;
   return conn;
}

/*
 * Free a connection data structure
 */
static void Dpi_conn_data_free(conn_data_t *conn)
{
   if (conn->FreeBuf)
      g_free(conn->Buf);
   g_free(conn);
}

/*
 * Append the new buffer in 'io' to Buf in 'conn'
 */
static void Dpi_append_io_buf(conn_data_t *conn, IOData_t *io)
{
   if (io->Status > 0) {
      conn->Buf = g_realloc(conn->Buf, (gulong)io->Status);
      memcpy((gchar*)conn->Buf + conn->BufSize, io->Buf, (size_t)io->Status);
      conn->BufSize += io->Status;
      conn->FreeBuf = 0;
   }
}

/*
 * Split the data stream into tokens.
 * Here, a token is either:
 *    a) a dpi tag
 *    b) a raw data chunk
 */
static gint Dpi_get_token(conn_data_t *conn)
{
   gint resp = -1;
   gchar *buf = conn->Buf;

   if (conn->FreeBuf || conn->BufIdx == conn->BufSize) {
      g_free(conn->Buf);
      conn->Buf = NULL;
      conn->BufIdx = conn->BufSize = 0;
      conn->FreeBuf = 0;
      return resp;
   }

   if (conn->Send2EOF) {
      conn->Tok = buf + conn->BufIdx;
      conn->TokSize = conn->BufSize - conn->BufIdx;
      conn->BufIdx = conn->BufSize;
      return 0;
   }

   if (!conn->InTag && !conn->InData) {
      /* search for start of tag */
/*
gchar *pbuf=NULL;
MSG("conn->BufIdx = %d; conn->BufSize = %d\n", conn->BufIdx,conn->BufSize);
pbuf = g_strndup(buf, conn->BufSize - conn->BufIdx);
MSG("buf: [%s]\n", pbuf);
g_free(pbuf);
*/
      while (conn->BufIdx < conn->BufSize && buf[conn->BufIdx] != '<')
         ++conn->BufIdx;
      if (conn->BufIdx < conn->BufSize) {
         /* found */
         conn->InTag = 1;
         conn->Tok = buf + conn->BufIdx;
      } else {
         MSG("ERROR: [Dpi_get_token] Can't find token start\n");
         conn->FreeBuf = 1;
         return Dpi_get_token(conn);
      }
   }

   if (conn->InTag) {
      /* search for end of tag */
      while (conn->BufIdx < conn->BufSize && buf[conn->BufIdx] != '>')
         ++conn->BufIdx;
      if (conn->BufIdx < conn->BufSize) {
         /* found EOT */
         conn->TokIsTag = 1;
         conn->TokSize = buf + conn->BufIdx - conn->Tok + 1;
         ++conn->BufIdx;
         conn->InTag = 0;
         resp = 0;
      }
   }

   if (conn->InData) {
      conn->TokIsTag = 0;
      if (conn->DataRecvSize + conn->BufSize - conn->BufIdx <
          conn-> DataTotalSize) {
         conn->TokSize += conn->BufSize - conn->BufIdx;
         conn->DataRecvSize += conn->BufSize - conn->BufIdx;
         conn->FreeBuf = 1;
         resp = 0;
      } else {
         /* srch end of data */
         MSG("ERROR: [Dpi_get_token] *** NULL code here ***\n");
         while (conn->BufIdx < conn->BufSize)
            ++conn->BufIdx;
         resp = -1;
      }
   }

   return resp;
}

/*
 * Parse a dpi tag and take the appropriate actions
 */
static void Dpi_parse_token(conn_data_t *conn)
{
   gchar *tag, *cmd, *msg, *urlstr;
   DataBuf *dbuf;

   if (conn->Send2EOF) {
      /* we're receiving data chunks from a HTML page */
      dbuf = a_Chain_dbuf_new(conn->Tok, conn->TokSize, 0);
      a_Chain_fcb(OpSend, conn->InfoRecv, dbuf, "send_page_2eof");
      g_free(dbuf);
      return;
   }

   tag = g_strndup(conn->Tok, (guint)conn->TokSize);
   MSG("Dpi_parse_token: [%s]\n", tag);

   cmd = Get_attr_value(conn->Tok, conn->TokSize, "cmd");
   if (strcmp(cmd, "send_status_message") == 0) {
      msg = Get_attr_value(conn->Tok, conn->TokSize, "msg");
      a_Chain_fcb(OpSend, conn->InfoRecv, msg, cmd);
      g_free(msg);

   } else if (strcmp(cmd, "chat") == 0) {
      msg = Get_attr_value(conn->Tok, conn->TokSize, "msg");
      a_Chain_fcb(OpSend, conn->InfoRecv, msg, cmd);
      g_free(msg);

   } else if (strcmp(cmd, "dialog") == 0) {
      /* For now will send the dpip tag... */
      a_Chain_fcb(OpSend, conn->InfoRecv, tag, cmd);

   } else if (strcmp(cmd, "start_send_page") == 0) {
      urlstr = Get_attr_value(conn->Tok, conn->TokSize, "url");
      a_Chain_fcb(OpSend, conn->InfoRecv, urlstr, cmd);
      g_free(urlstr);
      /* todo:  Get_attr_value(conn->Tok, conn->TokSize, "send_mode")  */
      conn->Send2EOF = 1;

   } else if (strcmp(cmd, "reload_request") == 0) {
      urlstr = Get_attr_value(conn->Tok, conn->TokSize, "url");
      a_Chain_fcb(OpSend, conn->InfoRecv, urlstr, cmd);
      g_free(urlstr);
   }
   g_free(cmd);

   g_free(tag);
}

/*
 * Compare function for searching a CCC Node with a 'web' pointer.
 */
static gint Dpi_node_cmp(gconstpointer node, gconstpointer key)
{
   return a_Url_cmp(key, ((ChainLink *)node)->LocalKey);
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*
 * Get a new data buffer (within an 'io'), save it into local data,
 * split in tokens and parse the contents.
 */
static void Dpi_process_io(int Op, void *Data1, conn_data_t *conn)
{
   IOData_t *io = Data1;

   /* Very useful for debugging: show the data stream as received. */
   // fwrite(io->Buf, io->Status, 1, stdout);

   if (Op == IORead) {
      Dpi_append_io_buf(conn, io);
      while (Dpi_get_token(conn) != -1) {
         Dpi_parse_token(conn);
      }

   } else if (Op == IOClose) {
      MSG("Dpi: [Dpi_process_io] IOClose\n");
   }
}

/*
 * Start dpid.
 * Return: 0 starting now, 1 Error.
 */
static gint Dpi_start_dpid(void)
{
   pid_t pid;
   gint st_pipe[2], n, ret = 1;
   gchar buf[16];

   /* create a pipe to track our child's status */
   if (pipe(st_pipe))
      return 1;

   pid = fork();
   if (pid == 0) {
      /* This is the child process.  Execute the command. */
      gchar *path1 = a_Misc_prepend_user_home(".dillo/dpid");
      Dpi_close_fd(st_pipe[0]);
      if (execl(path1, "dpid", NULL) == -1) {
         g_free(path1);
         if (execlp("dpid", "dpid", NULL) == -1) {
            DEBUG_MSG(4, "Dpi_start_dpid (child): %s\n", g_strerror(errno));
            do
               n = write(st_pipe[1], "ERROR", 5);
            while (n == -1 && errno == EINTR);
            Dpi_close_fd(st_pipe[1]);
            _exit (EXIT_FAILURE);
         }
      }
   } else if (pid < 0) {
      /* The fork failed.  Report failure.  */
      DEBUG_MSG(4, "Dpi_start_dpid: %s\n", g_strerror(errno));
      /* close the unused pipe */
      Dpi_close_fd(st_pipe[0]);
      Dpi_close_fd(st_pipe[1]);
   } else {
      /* This is the parent process, check our child status... */
      Dpi_close_fd(st_pipe[1]);
      do
         n = read(st_pipe[0], buf, 16);
      while (n == -1 && errno == EINTR);
      DEBUG_MSG(2, "Dpi_start_dpid: n = %d\n", n);
      if (n != 5)
         ret = 0;
      else
         DEBUG_MSG(4, "Dpi_start_dpid: %s\n", g_strerror(errno));
   }

   return ret;
}

/*
 * Make a connection test for a UDS.
 * Return: 0 OK, 1 Not working.
 */
static gint Dpi_check_uds(gchar *uds_name)
{
   struct sockaddr_un pun;
   gint SockFD, ret = 1;

   if (access(uds_name, W_OK) == 0) {
      /* socket connection test */
      memset(&pun, 0, sizeof(struct sockaddr_un));
      pun.sun_family = AF_LOCAL;
      strncpy(pun.sun_path, uds_name, sizeof (pun.sun_path));

      if ((SockFD = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1 ||
          connect(SockFD, (void*)&pun, D_SUN_LEN(&pun)) == -1) {
         DEBUG_MSG(4, "Dpi_check_uds: %s %s\n", g_strerror(errno), uds_name);
      } else {
         Dpi_close_fd(SockFD);
         ret = 0;
      }
   }
   return ret;
}

/*
 * Return the directory where the UDS are in,
 * NULL if it can't be found.
 */
static gchar *Dpi_get_dpid_uds_dir(void)
{
   FILE *in;
   gchar *saved_name_filename;    /*  :)  */
   gchar dpid_uds_dir[256], *p;

   saved_name_filename =
      g_strconcat(g_get_home_dir(), "/", ".dillo/dpi_socket_dir", NULL);
   in = fopen(saved_name_filename, "r");
   g_free(saved_name_filename);

   if (in != NULL) {
      fgets(dpid_uds_dir, 256, in);
      fclose(in);
      if ((p = strchr(dpid_uds_dir, '\n')))
         *p = 0;
      if (access(dpid_uds_dir, F_OK) == 0)
         p = g_strdup(dpid_uds_dir);
         _MSG("Dpi_get_dpid_uds_dir:: %s\n", p);
         return p;
   }

   _MSG("Dpi_get_dpid_uds_dir: %s \n", g_strerror(errno));
   return NULL;
}

/*
 * Return the dpid's UDS name, NULL on failure.
 */
static gchar *Dpi_get_dpid_uds_name(void)
{
   gchar *dpid_uds_dir, *dpid_uds_name = NULL;

   if ((dpid_uds_dir = Dpi_get_dpid_uds_dir()) != NULL)
      dpid_uds_name= g_strconcat(dpid_uds_dir, "/", "dpid.srs", NULL);

   g_free(dpid_uds_dir);
   return dpid_uds_name;
}

/*
 * Confirm that the dpid is running. If not, start it.
 * Return: 0 running OK, 1 starting (EAGAIN), 2 Error.
 */
static gint Dpi_check_dpid(void)
{
   static gint starting = 0;
   gchar *dpid_uds_name;
   gint check_st = 1, ret = 2;

   if ((dpid_uds_name = Dpi_get_dpid_uds_name()))
      check_st = Dpi_check_uds(dpid_uds_name);

   _MSG("Dpi_check_dpid: dpid_uds_name=%s, check_st=%d\n",
        dpid_uds_name, check_st);

   if (check_st == 0) {
      /* connection test with dpi server passed */
      starting = 0;
      ret = 0;
   } else if (!dpid_uds_name || check_st) {
      if (!starting) {
         /* start dpid */
         if (Dpi_start_dpid() == 0) {
            starting = 1;
            ret = 1;
         }
      } else if (++starting < 25) {
         ret = 1;
      } else {
         /* we waited too much, report an error... */
         starting = 0;
      }
   }

   g_free(dpid_uds_name);
   DEBUG_MSG(2, "Dpi_check_dpid:: %s\n",
             (ret == 0) ? "OK" : (ret == 1 ? "EAGAIN" : "ERROR"));
   return ret;
}

/*
 * Return the UDS name of a dpi server.
 * (A query is sent to dpid and then its answer parsed)
 * note: as the available servers and/or the dpi socket directory can
 *       change at any time, we'll ask each time. If someday we find
 *       that connecting each time significantly degrades performance,
 *       an optimized approach can be tried.
 */
static gchar *Dpi_get_server_uds_name(gchar *server_name)
{
   gchar *dpid_uds_dir, *dpid_uds_name = NULL,
         *server_uds_name = NULL;
   gint st;

   g_return_val_if_fail (server_name != NULL, NULL);
   DEBUG_MSG(2, "Dpi_get_server_uds_name:: server_name = [%s]\n", server_name);

   dpid_uds_dir = Dpi_get_dpid_uds_dir();
   if (dpid_uds_dir) {
      struct sockaddr_un dpid;
      gint sock, req_sz, rdlen;
      gchar buf[128], *cmd, *request, *rply;
      size_t buflen;

      /* Get the server's uds name from dpid */
      sock = socket(AF_LOCAL, SOCK_STREAM, 0);
      dpid.sun_family = AF_LOCAL;
      dpid_uds_name = g_strconcat(dpid_uds_dir, "/", "dpid.srs", NULL);
      _MSG("dpid_uds_name = [%s]\n", dpid_uds_name);
      strncpy(dpid.sun_path, dpid_uds_name, sizeof(dpid.sun_path));

      if (connect(sock, (struct sockaddr *) &dpid, sizeof(dpid)) == -1)
         perror("connect");
      /* ask dpid to check the server plugin and send its UDS name back */
      request = g_strdup_printf("<dpi cmd='check_server' msg='%s'>",
                                server_name);
      DEBUG_MSG(2, "[%s]\n", request);
      do
         st = write(sock, request, strlen(request));
      while (st < 0 && errno == EINTR);
      if (st < 0 && errno != EINTR)
         perror("writing request");
      g_free(request);
      shutdown(sock, 1); /* signals no more writes to dpid */

      /* Get the reply */
      rply = NULL;
      buf[0] = '\0';
      buflen = sizeof(buf)/sizeof(buf[0]);
      for (req_sz = 0; (rdlen = read(sock, buf, buflen)) != 0;
           req_sz += rdlen) {
         if (rdlen == -1 && errno == EINTR)
               continue;
         if (rdlen == -1) {
            perror(" ** Dpi_get_server_uds_name **");
            break;
         }
         rply = g_realloc(rply, (guint)(req_sz + rdlen + 1));
         if (req_sz == 0)
            rply[0] = '\0';
         strncat(rply, buf, (size_t)rdlen);
      }
      Dpi_close_fd(sock);
      DEBUG_MSG(2, "rply = [%s]\n", rply);

      /* Parse reply */
      if (rdlen == 0 && rply) {
         cmd = Get_attr_value(rply, (gint)strlen(rply), "cmd");
         if (strcmp(cmd, "send_data") == 0)
            server_uds_name = Get_attr_value(rply, (gint)strlen(rply), "msg");
         g_free(cmd);
         g_free(rply);
      }
   }
   g_free(dpid_uds_dir);
   g_free(dpid_uds_name);
   DEBUG_MSG(2, "Dpi_get_server_uds_name:: %s\n", server_uds_name);
   return server_uds_name;
}


/*
 * Connect a socket to a dpi server and return the socket's FD.
 * We have to ask 'dpid' (dpi daemon) for the UDS of the target dpi server.
 * Once we have it, then the proper file descriptor is returned (-1 on error).
 */
static gint Dpi_connect_socket(gchar *server_name, gint retry)
{
   char *server_uds_name;
   struct sockaddr_un pun;
   gint SockFD, err;

   /* Query dpid for the UDS name for this server */
   server_uds_name = Dpi_get_server_uds_name(server_name);
   DEBUG_MSG(2, "server_uds_name = [%s]\n", server_uds_name);

   if (access(server_uds_name, F_OK) != 0) {
      MSG("server socket was NOT found\n");
      return -1;
   }

   /* connect with this server's socket */
   memset(&pun, 0, sizeof(struct sockaddr_un));
   pun.sun_family = AF_LOCAL;
   strncpy(pun.sun_path, server_uds_name, sizeof (pun.sun_path));
   g_free(server_uds_name);

   if ((SockFD = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1)
      perror("[dpi::socket]");
   else if (connect(SockFD, (void*)&pun, D_SUN_LEN(&pun)) == -1) {
      err = errno;
      SockFD = -1;
      MSG("[dpi::connect] errno:%d %s\n", errno, g_strerror(errno));
      if (retry) {
         switch (err) {
            case ECONNREFUSED: case EBADF: case ENOTSOCK: case EADDRNOTAVAIL:
               /* the server may crash and its socket name survive */
               unlink(pun.sun_path);
               SockFD = Dpi_connect_socket(server_name, FALSE);
               break;
         }
      }
   }

   return SockFD;
}


/*
 * CCC function for the Dpi module
 */
void a_Dpi_ccc(int Op, int Branch, int Dir, ChainLink *Info,
               void *Data1, void *Data2)
{
   GSList *list;
   gint SockFD = -1, st;

   a_Chain_debug_msg("a_Dpi_ccc", Op, Branch, Dir);

   if (Branch == 1) {
      if (Dir == BCK) {
         /* Cache request, return the FD. */
         /* (Data1 = Url;  Data2 = Web) */
         switch (Op) {
         case OpStart:
            /* We'll know the FD later, enqueue this connection.
             * (The Url is used as an identifier for the connection) */
            Info->LocalKey = a_Url_dup(Data1);
            PendingNodes = g_slist_append(PendingNodes, (gpointer)Info);
            break;
         }
      } else {  /* FWD */
         switch (Op) {
         case OpEnd:
            /* End this requesting branch */
            a_Url_free(Info->LocalKey);
            a_Chain_fcb(OpEnd, Info, NULL, NULL);
            break;
         case OpAbort:
            list = g_slist_find_custom(
                      PendingNodes, Info->LocalKey, Dpi_node_cmp);
            if (list) {
               /* The connection is not pending anymore */
               PendingNodes = g_slist_remove(PendingNodes, list->data);
            }
            a_Url_free(Info->LocalKey);
            a_Chain_fcb(OpAbort, Info, NULL, NULL);
            break;
         }
      }

   } else if (Branch == 2) {
      if (Dir == BCK) {
         /* Send commands to dpi-server */
         switch (Op) {
         case OpStart:
            if ((st = Dpi_check_dpid()) == 0) {
               SockFD = Dpi_connect_socket(Data1, TRUE);
               if (SockFD != -1) {
                  gint *fd = g_new(gint, 1);
                  *fd = SockFD;
                  Info->LocalKey = fd;
                  a_Chain_link_new(Info, a_Dpi_ccc, BCK, a_IO_ccc, 3, 2);
                  a_Chain_bcb(OpStart, Info, Info->LocalKey, NULL);
                  /* tell the capi to start the receiving branch */
                  a_Chain_fcb(OpSend, Info, Info->LocalKey, "SockFD");
               }
            }

            if (st == 0 && SockFD != -1)
               a_Chain_fcb(OpSend, Info, NULL, (void*)"DpidOK");
            else if (st == 1)
               a_Chain_fcb(OpSend, Info, NULL, (void*)"DpidEAGAIN");
            else {
               DEBUG_MSG(4, "dpi.c: ERROR, can't start dpi daemon\n");
               a_Dpi_ccc(OpAbort, 2, FWD, Info, "ERR_dpid", NULL);
            }
            break;
         case OpSend:
            a_Chain_bcb(OpSend, Info, Data1, NULL);
            break;
         case OpEnd:
            a_Chain_bcb(OpEnd, Info, NULL, NULL);
            g_free(Info->LocalKey);
            g_free(Info);
            break;
         case OpAbort:
            MSG("a_Dpi_ccc: OpAbort[2B], Not implemented\n");
            g_free(Info->LocalKey);
            g_free(Info);
            break;
         }
      } else {  /* FWD */
         /* Send commands to dpi-server (status) */
         switch (Op) {
         case OpEnd:
            a_Chain_del_link(Info, BCK);
            g_free(Info);
            break;
         case OpSend:
         case OpAbort:
            a_Chain_fcb(OpAbort, Info, Data1, NULL);
            g_free(Info);
            break;
         }
      }

   } else if (Branch == 3) {
      if (Dir == FWD) {
         /* Receiving from server */
         switch (Op) {
         case OpSend:
            Dpi_process_io(IORead, Data1, Info->LocalKey);
            break;
         case OpEnd:
            a_Chain_del_link(Info, BCK);
            Dpi_process_io(IOClose, Data1, Info->LocalKey);
            Dpi_conn_data_free(Info->LocalKey);
            a_Chain_fcb(OpEnd, Info, NULL, NULL);
            break;
         case OpAbort:
            MSG(" Not implemented\n");
            break;
         }
      } else {  /* BCK */
         switch (Op) {
         case OpStart:
            {
            IOData_t *io2;
            Info->LocalKey = Dpi_conn_data_new(Info);
            io2 = a_IO_new(IORead, *(int*)Data1); /* SockFD */
            a_IO_set_buf(io2, NULL, IOBufLen);
            a_Chain_link_new(Info, a_Dpi_ccc, BCK, a_IO_ccc, 2, 3);
            a_Chain_bcb(OpStart, Info, io2, NULL);
            break;
            }
         case OpSend:
            /* now that we have the FD, resume the connection.
             * (Data1 = FD, Data2 = Url) */
            list = g_slist_find_custom(PendingNodes, Data2, Dpi_node_cmp);
            if (list) {
               ChainLink *P_Info = list->data;
               /* Tell the cache to start the receiving CCC */
               a_Chain_fcb(OpSend, P_Info, Data1, NULL);
               /* The connection is not pending anymore */
               PendingNodes = g_slist_remove(PendingNodes, list->data);
               /* End this requesting CCC */
               a_Dpi_ccc(OpEnd, 1, FWD, P_Info, NULL, NULL);
            }
            break;
         case OpStop:
            /* Stop transfer, abort the pending node.
             * (Data1 = Url, Data2 = NULL) */
            list = g_slist_find_custom(PendingNodes, Data1, Dpi_node_cmp);
            if (list) {
               ChainLink *P_Info = list->data;
               /* Abort this requesting CCC */
               a_Dpi_ccc(OpAbort, 1, FWD, P_Info, Data1, NULL);
            }
            break;
         case OpAbort:
            MSG(" Not implemented\n");
            break;
         }
      }

   } else if (Branch == 4) {
      /* Unused */
      g_assert_not_reached();
   }
}

/*! Send DpiBye to dpid
 * Note: currently disabled. Maybe it'd be better to have a
 * dpid_idle_timeout variable in the config file.
 */
void a_Dpi_bye_dpid()
{
   char *DpiBye_cmd = "<dpi cmd='DpiBye'>";
   struct sockaddr_un sa;
   size_t sun_path_len, addr_len;
   char *srs_name;
   int new_socket;

   srs_name = Dpi_get_dpid_uds_name();
   sun_path_len = sizeof(sa.sun_path);
   addr_len = sizeof(sa);

   sa.sun_family = AF_LOCAL;

   if ((new_socket = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1) {
      DEBUG_MSG(4, "a_Dpi_bye_dpid: %s\n", g_strerror(errno));
   }
   strncpy(sa.sun_path, srs_name, sizeof (sa.sun_path));
   if (connect(new_socket, (struct sockaddr *) &sa, addr_len) == -1) {
      DEBUG_MSG(4, "a_Dpi_bye_dpid: %s\n", g_strerror(errno));
      fprintf(stderr, "%s\n", sa.sun_path);
   }
   (void) write(new_socket, DpiBye_cmd, strlen(DpiBye_cmd));
   Dpi_close_fd(new_socket);
}

