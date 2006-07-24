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

#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <glib.h>
#include "dpid_common.h"
#include "dpid.h"
#include "dpi.h"
#include "dpi_socket_dir.h"
#include "misc_new.h"

sigset_t mask_sigchld;


/* Start a dpi filter plugin after accepting the pending connection
 * \Return
 * \li Child process ID on success
 * \li 0 on failure
 */
static int start_filter_plugin(struct dp dpi_attr)
{
   int newsock, old_stdout=-1, old_stdin=-1;
   socklen_t csz;
   struct sockaddr_un clnt_addr;
   pid_t pid;

   csz = (socklen_t) sizeof(clnt_addr);

   newsock = accept(dpi_attr.socket, (struct sockaddr *) &clnt_addr, &csz);
   if (newsock == -1)
      ERRMSG("start_plugin", "accept", errno);

   dup2(STDIN_FILENO, old_stdin);
   if (dup2(newsock, STDIN_FILENO) == -1) {
      ERRMSG("start_plugin", "dup2", errno);
      fprintf(stderr, "ERROR in child proc for %s\n", dpi_attr.path);
      exit(1);
   }

   dup2(STDOUT_FILENO, old_stdout);
   if (dup2(newsock, STDOUT_FILENO) == -1) {
      ERRMSG("start_plugin", "dup2", errno);
      fprintf(stderr, "ERROR in child proc for %s\n", dpi_attr.path);
      exit(1);
   }
   if ((pid = fork()) == -1) {
      ERRMSG("main", "fork", errno);
      return 0;
   }
   if ( pid == 0) {
      /* Child, start plugin */
      if (execl(dpi_attr.path, dpi_attr.path, NULL) == -1) {
         ERRMSG("start_plugin", "execl", errno);
         fprintf(stderr, "ERROR in child proc for %s\n", dpi_attr.path);
         exit(1);
      }
   }

   /* Parent, Close sockets fix stdio and return pid */
   if (a_Misc_close_fd(newsock) == -1) {
      ERRMSG("start_plugin", "close", errno);
      fprintf(stderr, "ERROR in child proc for %s\n", dpi_attr.path);
      exit(1);
   }
   a_Misc_close_fd(STDIN_FILENO);
   a_Misc_close_fd(STDOUT_FILENO);
   dup2(old_stdin, STDIN_FILENO);
   dup2(old_stdout, STDOUT_FILENO);
   return pid;
}

static void start_server_plugin(struct dp dpi_attr)
{
   if (dup2(dpi_attr.socket, STDIN_FILENO) == -1) {
      ERRMSG("start_plugin", "dup2", errno);
      fprintf(stderr, "ERROR in child proc for %s\n", dpi_attr.path);
      exit(1);
   }
   if (a_Misc_close_fd(dpi_attr.socket) == -1) {
      ERRMSG("start_plugin", "close", errno);
      fprintf(stderr, "ERROR in child proc for %s\n", dpi_attr.path);
      exit(1);
   }
   if (execl(dpi_attr.path, dpi_attr.path, NULL) == -1) {
      ERRMSG("start_plugin", "execl", errno);
      fprintf(stderr, "ERROR in child proc for %s\n", dpi_attr.path);
      exit(1);
   }
}

/*!
 * Read service request from sock
 * \Return
 * pointer to dynamically allocated request tag
 */
static char *get_request(int sock)
{
   char *req, buf[10];
   size_t buflen;
   size_t rqsz;
   ssize_t rdln;

   req = NULL;
   buf[0] = '\0';
   buflen = sizeof(buf) / sizeof(buf[0]);

   (void) sigprocmask(SIG_BLOCK, &mask_sigchld, NULL);
   for (rqsz = 0; (rdln = read(sock, buf, buflen)) != 0; rqsz += rdln) {
      if (rdln == -1)
         break;
      req = (char *) realloc(req, rqsz + rdln + 1);
      if (rqsz == 0)
         req[0] = '\0';
      strncat(req, buf, (size_t) rdln);
   }
   (void) sigprocmask(SIG_UNBLOCK, &mask_sigchld, NULL);
   if (rdln == -1) {
      ERRMSG("get_request", "read", errno);
   }

   return (req);
}

/*!
 * Get value of cmd field in dpi_tag
 * \Return
 * command code on success, -1 on failure
 */
static int get_command(int sock, char *dpi_tag, struct dp *dpi_attr_list)
{
   char *cmd;
   int COMMAND;

   if (dpi_tag == NULL) {
      ERRMSG("get_command", "dpid tag is NULL\n", 0);
      return (-1);
   }

   cmd = a_Misc_get_attr_value(dpi_tag, strlen(dpi_tag), "cmd");

   if (cmd == NULL) {
      ERRMSG("get_command", "a_Misc_get_attr_value", 0);
      fprintf(stderr, ": dpid failed to parse cmd in %s\n", dpi_tag);
      (void) CKD_WRITE(sock, tag_prserr);
      COMMAND = -1;
   } else if (strcmp("DpiBye", cmd) == 0) {
      COMMAND = BYE_CMD;
   } else if (strcmp("check_server", cmd) == 0) {
      COMMAND = CHECK_SERVER_CMD;
   } else if (strcmp("register_all", cmd) == 0) {
      COMMAND = REGISTER_ALL_CMD;
   } else if (strcmp("register_service", cmd) == 0) {
      COMMAND = REGISTER_SERVICE_CMD;
   } else {                     /* Error unknown command */
      COMMAND = UNKNOWN_CMD;
   }

   g_free(cmd);
   return (COMMAND);
}

/*
 * Get MAX open FD limit (yes, it's tricky --Jcid).
 */
static int get_open_max(void)
{
#ifdef OPEN_MAX
   return OPEN_MAX;
#else
   int ret = sysconf(_SC_OPEN_MAX);
   if (ret < 0)
      ret = 256;
   return ret;
#endif
}

/*! \todo
 * \li Add a dpid_idle_timeout variable to dpidrc
 * \bug Infinite loop if plugin crashes before it accepts a connection
 */
int main(void)
{
   int i, n = 0, open_max;
   char *dirname = NULL, *sockdir = NULL;
   int dpid_idle_timeout = 60 * 60; /* default, in seconds */
   struct timeval select_timeout;
   sigset_t mask_none;
   fd_set selected_set;

   dpi_attr_list = NULL;
   //daemon(0,0); /* Use 0,1 for feedback */
   /* todo: call setsid() ?? */

   /* Allow read and write access, but only for the user.
    * todo: can this cause trouble with umount? */
   umask(0077);
   /* todo: make dpid work on any directory. */
   // chdir("/");

   /* close inherited file descriptors */
   open_max = get_open_max();
   for (i = 3; i < open_max; i++)
      a_Misc_close_fd(i);

   /* this sleep used to unmask a race condition */
   // sleep(2);

   dpi_errno = no_errors;

   /* Get list of available dpis */
   numdpis = register_all(&dpi_attr_list);

   /* Get name of socket directory */
   dirname = a_Dpi_sockdir_file();
   if ((sockdir = init_sockdir(dirname)) == NULL) {
      ERRMSG("main", "init_sockdir", 0);
      fprintf(stderr, "Failed to create socket directory\n");
      exit(1);
   }

   /* Remove any sockets that may have been leftover from a crash */
   cleanup(sockdir);
   /* Initialise sockets */
   if ((numsocks = init_srs_socket(sockdir)) == -1) {
      switch (dpi_errno) {
      case dpid_srs_addrinuse:
         fprintf(stderr, "dpid refuses to start, possibly because:\n");
         fprintf(stderr, "\t1) An instance of dpid is already running.\n");
         fprintf(stderr, "\t2) A previous dpid didn't clean up on exit.\n");
         exit(1);
      default:
         ERRMSG("main", "init_srs_sockets failed\n", 0);
         exit(1);
      }
   }
   numsocks = init_all_dpi_sockets(dpi_attr_list, sockdir);
   //est_terminator(); /* Do we still want to clean up on an abnormal exit? */
   est_dpi_sigchld();

   (void) sigemptyset(&mask_sigchld);
   (void) sigaddset(&mask_sigchld, SIGCHLD);
   (void) sigemptyset(&mask_none);
   (void) sigprocmask(SIG_SETMASK, &mask_none, NULL);

   printf("dpid started\n");
/* Start main loop */
   while (1) {
      do {
         (void) sigprocmask(SIG_BLOCK, &mask_sigchld, NULL);
         if (caught_sigchld) {
            handle_sigchld();
            caught_sigchld = 0;
         }
         (void) sigprocmask(SIG_UNBLOCK, &mask_sigchld, NULL);
         select_timeout.tv_sec = dpid_idle_timeout;
         select_timeout.tv_usec = 0;
         selected_set = sock_set;
         n = select(FD_SETSIZE, &selected_set, NULL, NULL, &select_timeout);
         if ( n == 0 ) { /* select timed out, exit */
            stop_active_dpis(dpi_attr_list, numdpis);
            cleanup(sockdir);
            exit(0);
         }
      } while (n == -1 && errno == EINTR);

/* g_mem_profile(); */
      if (n == -1) {
         ERRMSG("main", "select", errno);
         exit(1);
      }
      /* If the service req socket is selected then service the req. */
      if (FD_ISSET(srs, &selected_set)) {
         int sock;
         socklen_t csz;
         struct sockaddr_un clnt_addr;
         char *req = NULL;

         --n;
         g_assert(n >= 0);
         csz = (socklen_t) sizeof(clnt_addr);
         sock = accept(srs, (struct sockaddr *) &clnt_addr, &csz);
         if (sock == -1) {
            ERRMSG("main", "accept", errno);
            fprintf(stderr, "accept on srs socket failed\n");
            fprintf(stderr, "service pending connections, and continue\n");
         } else {
            int command;

            req = get_request(sock);
            command = get_command(sock, req, dpi_attr_list);
            switch (command) {
            case BYE_CMD:
               stop_active_dpis(dpi_attr_list, numdpis);
               cleanup(sockdir);
               exit(0);
               break;
            case CHECK_SERVER_CMD:
               send_sockpath(sock, req, dpi_attr_list);
               break;
            case REGISTER_ALL_CMD:
               register_all_cmd(sockdir);
               break;
            case UNKNOWN_CMD:
               (void) CKD_WRITE(sock, tag_cmderr);
               ERRMSG("main", "Unknown command", 0);
               fprintf(stderr, " for request: %s\n", req);
               break;
            case -1:
               ERRMSG("main", "get_command failed\n", 0);
               break;
            }
            if (req)
               free(req);
            a_Misc_close_fd(sock);
         }
      }

      /* While there's a request on one of the plugin sockets
       * find the matching plugin and start it. */
      for (i = 0; n > 0 && i < numdpis; i++) {
         if (FD_ISSET(dpi_attr_list[i].socket, &selected_set)) {
            --n;
            g_assert(n >= 0);

            if (dpi_attr_list[i].filter) {
               /* start a dpi filter plugin and continue watching its socket
                * for new connections */
               (void) sigprocmask(SIG_SETMASK, &mask_none, NULL);
               start_filter_plugin(dpi_attr_list[i]);
            } else {
               /* start a dpi server plugin but don't wait for new connections
                * on its socket */
               numsocks--;
               g_assert(numsocks >= 0);
               FD_CLR(dpi_attr_list[i].socket, &sock_set);
               if ((dpi_attr_list[i].pid = fork()) == -1) {
                  ERRMSG("main", "fork", errno);
                  /* exit(1); */
               } else if (dpi_attr_list[i].pid == 0) {
                  (void) sigprocmask(SIG_SETMASK, &mask_none, NULL);
                  start_server_plugin(dpi_attr_list[i]);
               }
            }
         }
      }
   }
}
