/*
 * Downloads server (chat version).
 *
 * NOTE: A simple downloads dpi that illustrates how to make a dpi-server.
 *
 * It uses wget to download a link.  This has been tested with wget 1.8.1
 * The server accepts multiple connections once it has been started.
 * If there are no requests within 5 minutes it waits for all child processes
 * to finish and then it exits.
 *
 * Copyright 2002-2004 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */


#include <config.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/time.h>
#include <glib.h>
#include "dpiutil.h"

/* Uncomment for verbose output */
//#define DEBUG_MSG

/* Convinience for the error message stream */
#define eprintf(fmt...) fprintf(stderr, fmt)

#ifdef DEBUG_MSG
#  define ceprintf(fmt...) eprintf(fmt)
#else
#  define ceprintf(fmt...)
#endif

pid_t origpid, fpid;

/*---------------------------------------------------------------------------*/

/*
 * Make a new name and place it in 'dl_dest'.
 */
static void make_new_name(gchar **dl_dest, const gchar *url)
{
   GString *gstr = g_string_new(*dl_dest);
   gint idx = gstr->len;

   if (gstr->str[idx - 1] != '/'){
      g_string_append_c(gstr, '/');
      ++idx;
   }

   /* Use a mangled url as name */
   g_string_append(gstr, url);
   for (   ; idx < gstr->len; ++idx)
      if (!isalnum(gstr->str[idx]))
         gstr->str[idx] = '_';

   /* free memory */
   g_free(*dl_dest);
   *dl_dest = gstr->str;
   g_string_free(gstr, FALSE);
}

/*
 * Filter '\n', '\r', "%0D" and "%0A" from the authority part of an FTP url.
 * This helps to avoid a SMTP relaying hack. This filtering could be done
 * only when port == 25, but if the mail server is listening on another
 * port it wouldn't work.
 * Note: AFAIS this should be done by wget.
 * (copied from ftp.c)
 */
static void ftpurl_filter_smtp_hack(GString *gs_url)
{
   int i;
   char c;

   if (gs_url->len > 6) { /* ftp:// */
      for (i = 6; (c = gs_url->str[i]) && c != '/'; ++i) {
         if (c == '\n' || c == '\r') {
            g_string_erase(gs_url, i--, 1);
         } else if (c == '%' && gs_url->str[i+1] == '0' &&
                    (tolower(gs_url->str[i+2]) == 'a' ||
                     tolower(gs_url->str[i+2]) == 'd')) {
            g_string_erase(gs_url, i--, 3);
         }
      }
   }
}

/*
 * Escape "'" as "%27"
 * (copied from ftp.c)
 */
static GString *Url_escape_quote(const char *str)
{
   int i;
   GString *gstr = g_string_sized_new(64);

   for (i = 0; str[i]; ++i) {
      if (str[i] == '\'')
         g_string_append(gstr, "%27");
      else
         g_string_append_c(gstr, str[i]);
   }

   return gstr;
}


/*---------------------------------------------------------------------------*/

/*
 * SIGCHLD handler
 */
static void sigchld(int sig)
{
   ceprintf("downloads server %d: sigchld\n", origpid);
   fflush(stderr);
   while (waitpid(0, NULL, WNOHANG) > 0) {
   }
}

/*
 * Establish SIGCHLD handler
 */
static void est_sigchld(void)
{
   struct sigaction act;
   sigset_t block;

   sigemptyset(&block);
   sigaddset(&block, SIGCHLD);
   act.sa_handler = sigchld;
   act.sa_mask = block;
   act.sa_flags = SA_NOCLDSTOP;
   sigaction(SIGCHLD, &act, NULL);
}

/*
 * Read a single line from a socket and store it in a GString.
 */
static ssize_t readline(int socket, GString ** msg)
{
   ssize_t rdlen;
   char buf[BUFLEN], c = 0;
   ssize_t i = 0;

   buf[0] = '\0';

   for (i = 0;
        (rdlen = read(socket, &c, 1)) != 0 && c != '\n' && rdlen != -1; i++)
      g_string_append_c(*msg, c);

   if (rdlen == -1) {
      perror("downloads server:readline: read");
      exit(EXIT_FAILURE);
   }

   return i;
}


/*!
 * Main download process.
 */
int main(void)
{
   int new_socket, ns;
   socklen_t csz;
   ssize_t rdlen;
   struct sockaddr_un clnt_addr;

   //char *wget_cmd = "wget --no-parent -t 1 -nc -k -nH --cut-dirs=30 -P";
   //char *wget_cmd = "wget -t 1 -nH -P";  --doesn't rename, +other problems
   char *wget_cmd = "wget -O - ";

   char *url = NULL, *dl_dest = NULL, *cmd = NULL;
   GString *gs_dl_cmd, *gs_url, *tag;
   fd_set active_set, selected_set;
   struct timeval tout;
   sigset_t blockSC;

   origpid = getpid();
   fpid = origpid;
   printf("\n\nDL_SRV PARENT %u: DOWNLOAD SERVER STARTED\n", origpid);
   fflush(stdout);
   sigemptyset(&blockSC);
   sigaddset(&blockSC, SIGCHLD);
   est_sigchld();

   csz = (socklen_t) sizeof(clnt_addr);
   FD_ZERO(&active_set);
   FD_SET(STDIN_FILENO, &active_set);
   while (1) {
      do {
         /* exit if there are no download requests after this time */
         tout.tv_sec = TOUT;
         tout.tv_usec = 0;
         selected_set = active_set;
         ns = select(STDIN_FILENO + 1, &selected_set, NULL, NULL, &tout);
      } while (ns == -1 && errno == EINTR);

      if (ns == -1) {
         perror("downloads server: select");
         exit(1);

      } else if (ns == 0) {     /* exit if no download requests */
         close(STDIN_FILENO);
         printf("downloads server %d:Terminating.\n"
                "Waiting for children to finish\n", origpid);
         fflush(stdout);
         /* BUG? Any further calls to downloads server will be queued by dpid
          * until all the children have finished.  This could be a long time */
         while (waitpid(-1, NULL, 0) >= 0) {
         }
         printf("\n\nDL_SRV %d: EXITING\n", origpid);
         fflush(stdout);
         exit(0);

      } else {
         /* accept the request */
         do {
            new_socket = accept(STDIN_FILENO, (struct sockaddr *) &clnt_addr,
                                &csz);
         } while (new_socket == -1 && errno == EINTR);

         if (new_socket == -1) {
            perror("downloads server: accept");
            exit(1);
         }
      }

      sigprocmask(SIG_BLOCK, &blockSC, NULL);
      tag = g_string_new(NULL);
      rdlen = readline(new_socket, &tag);
      if ((cmd = Get_attr_value(tag->str, (size_t)tag->len, "cmd")) == NULL) {
         eprintf("Failed to parse 'cmd' in %s\n", tag->str);
         exit(1);
      }
      if (strcmp(cmd, "download") != 0) {
         printf("downloads dpi: unknown command: '%s' terminating.\n", cmd);
         exit(1);
      }
      if (strcmp(cmd, "DpiBye") == 0) {
         printf("downloads dpi: Got DpiBye, terminating.\n");
         exit(0);
      }

      fpid = fork();
      if (fpid == 0) {
         pid_t ppid, cpid;
         FILE *in_stream, *out_stream;
         gchar buf[4096];
         struct stat sb;
         size_t n;

         origpid = cpid = getpid();
         ppid = getppid();
         ceprintf("\n\ndownloads server child %u: Started- Parent=%d\n", cpid,
                  ppid);
         if ((url = Get_attr_value(tag->str,(size_t)tag->len, "url")) == NULL){
            eprintf("Failed to parse 'url' in %s\n", tag->str);
            exit(1);
         }
         
         dl_dest = Get_attr_value(tag->str, (size_t)tag->len, "destination");
         if (dl_dest == NULL) {
            eprintf("Failed to parse 'destination' in %s\n", tag->str);
            exit(1);
         }

         ceprintf("downloads server child %u:parent=%u, url = %s\n",
                  cpid, ppid, url);
         ceprintf("downloads server child %u:parent=%u, dl_dest = %s\n",
                  cpid, ppid, dl_dest);

         /* 'dl_dest' may be a directory */
         if (stat(dl_dest, &sb) == 0 && S_ISDIR(sb.st_mode))
            make_new_name(&dl_dest, url);

         /* open the target stream */
         if ((out_stream = fopen(dl_dest, "w")) == NULL) {
            perror("downloads server child");
            exit(1);
         }

         /* make the download command string */
         gs_dl_cmd = g_string_new(NULL);
         /* escape "'" character for the shell */
         gs_url = Url_escape_quote(url);
         /* avoid malicious SMTP relaying with FTP urls */
         if (g_strncasecmp(gs_url->str, "ftp:/", 5) == 0)
            ftpurl_filter_smtp_hack(gs_url);
         g_string_sprintf(gs_dl_cmd, "%s '%s'", wget_cmd, gs_url->str);
         eprintf(" cmd: %s\n", gs_dl_cmd->str);
         eprintf("  to: %s\n", dl_dest);

         g_free(dl_dest);
         g_string_free(gs_url, TRUE);
         g_free(url);

         ceprintf("downloads server child %u:parent=%u, Running: %s\n", cpid,
                  ppid, gs_dl_cmd->str);

         /* fork through popen */
         if ((in_stream = popen(gs_dl_cmd->str, "r")) == NULL) {
            perror("popen");
            exit(1);
         }

         /* do the file transfer */
         while ((n = fread (buf, 1, 4096, in_stream)) > 0)
            fwrite(buf, 1, n, out_stream);

         /* close transfer */
         if (pclose(in_stream) != 0)
            perror("pclose");
         if (fclose(out_stream) != 0)
            perror("fclose");

         g_string_free(gs_dl_cmd, TRUE);


         if (close(new_socket) == -1) {
            perror("downloads server: close");
            exit(EXIT_FAILURE);
         }

         ceprintf("downloads server child %u:parent=%u, Child finished\n",
                  cpid, ppid);
         exit(0);
      }
      g_string_free(tag, TRUE);
      sigprocmask(SIG_UNBLOCK, &blockSC, NULL);
      close(new_socket);
   }
}

