/*
 * Dillo web browser
 *
 * Copyright 1997 Raph Levien <raph@acm.org>
 * Copyright 1999-2004 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <gtk/gtk.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <locale.h>

#include <config.h>
#include "msg.h"
#include "dillo.h"
#include "misc.h"
#include "nav.h"
#include "history.h"
#include "bookmark.h"
#include "dicache.h"
#include "dns.h"
#include "IO/mime.h"
#include "IO/Url.h"
#include "prefs.h"
#include "interface.h"
#include "dw.h"
#include "cookies.h"


/*
 * Command line options structure
 */
typedef struct {
   char *sh_opt; /* must be non NULL */
   char *lg_opt; /* must be non NULL */
   gint opt_argc; /* positive: mandatory, negative: optional */
   gint opt_flg;  /* 1 << n */
   char *help;   /* can be NULL */
} CLI_options;

enum {
   DILLO_CLI_XID           = 1 << 1,
   DILLO_CLI_FULLWINDOW    = 1 << 2,
   DILLO_CLI_HELP          = 1 << 3,
   DILLO_CLI_VERSION       = 1 << 4,
   DILLO_CLI_LOCAL         = 1 << 5,
   DILLO_CLI_GEOMETRY      = 1 << 6,
   DILLO_CLI_DBG_RENDERING = 1 << 7,
   DILLO_CLI_ERROR         = 1 << 15
};

enum {
   O_SEARCH, O_FOUND, O_NOTFOUND, O_OK, O_ERROR, O_DONE
};

/*
 * Global variables from command line options;
 */
gboolean dillo_dbg_rendering = FALSE;

/*
 * Forward declarations
 */
static void Dillo_check_home_dir(char *dir);
static gint Dillo_get_opt(CLI_options *options, int argc,
                          char **argv, char ***opt_arg, gint *idx);
static void Dillo_print_help(CLI_options *options);
static void Dillo_print_version(void);

/*
 * Local data
 */
static CLI_options Options[] = {
   {"-x", "--xid",        1, DILLO_CLI_XID,
    "  -x, --xid XID          Open first Dillo window in an existing\n"
    "                         GtkSocket which window ID is XID (decimal)."},
   {"-v", "--version",    0, DILLO_CLI_VERSION,
    "  -v, --version          Display version info and exit."},
   {"-h", "--help",       0, DILLO_CLI_HELP,
    "  -h, --help             Display this help text and exit."},
   {"-f", "--fullwindow", 0, DILLO_CLI_FULLWINDOW,
    "  -f, --fullwindow       Start in full window mode: hide address bar,\n"
    "                         navigation buttons, menu, and status bar."},
   {"-l", "--local",      0, DILLO_CLI_LOCAL,
    "  -l, --local            Don't follow links for this URL(s)."},
   {"-g", "-geometry",    1, DILLO_CLI_GEOMETRY,
    "  -g, -geometry GEO      Set initial window position where GEO is\n"
    "                         <width>x<height>{+-}<x>{+-}<y>"},
   {"-D", "--debug-rendering",
                          0, DILLO_CLI_DBG_RENDERING,
    "  -D, --debug-rendering  Draw additionaly several lines in a web page,\n"
    "                         representing its structure. For debugging. "},
   {NULL, NULL, 0, 0, NULL}
};

/*
 * Return the maximum number of option arguments
 */
static gint Dillo_get_max_opt(CLI_options *options)
{
   gint i, max;

   for (i = 0, max = 0; options[i].sh_opt; i++)
      if (abs(options[i].opt_argc) > max)
         max = abs(options[i].opt_argc);
   return max;
}

/*
 * Given a command line argument, build a DilloUrl for it.
 */
static DilloUrl *Dillo_make_start_url(gchar *str)
{
   gchar *url_str, *p, *cd;
   DilloUrl *start_url;
   gint is_file = FALSE;

   /* This check is for files with non-URL characters */
   if (access(str, F_OK) == 0) {
      p = a_Misc_escape_chars(str, "% ");
      url_str = g_strdup(p ? p : str);
      is_file = TRUE;
   } else {
      /* Filter URL string */
      url_str = a_Url_string_strip_delimiters(str);
   }

   if (is_file || access(url_str, F_OK) == 0) {
      GString *UrlStr = g_string_sized_new(128);

      if (url_str[0] == '/') {
         /* prepend the "file:" protocol specifier. */
         g_string_sprintf(UrlStr, "file:%s", url_str);
      } else {
         cd = g_get_current_dir();
         p = a_Misc_escape_chars(cd, "% ");
         g_string_sprintf(UrlStr, "file:%s", p ? p : cd);
         g_free(p);
         g_free(cd);
         if (UrlStr->str[UrlStr->len - 1] != '/')
            g_string_append(UrlStr, "/");
         g_string_append(UrlStr, url_str);
      }
      start_url = a_Url_new(UrlStr->str, NULL, 0, 0, 0);
      g_string_free(UrlStr, TRUE);
   } else {
      start_url = a_Url_new(url_str, NULL, 0, 0, 0);
   }

   g_free(url_str);
   return start_url;
}

/*
 * Get status code to avoid zombies!
 */
static void Dillo_sigchld_handler(int signum)
{
  int pid, status, serrno;
  serrno = errno;
  while (1) {
     pid = waitpid (-1, &status, WNOHANG);
     if (pid < 0) {
        perror ("waitpid");
        break;
     }
     if (pid == 0)
       break;
     /* notice_termination (pid, status); */
  }
  errno = serrno;
}


/*
 * ******************************** MAIN *************************************
 */
gint main(int argc, char *argv[])
{
   gchar *dir, *curr_locale;
   DilloUrl *start_url;
   BrowserWindow *bw;
   guint32 xid = 0;
   guint options_got = 0;
   gint idx = 0;
   gint opt_flg;
   gint i;
   char **opt_argv = NULL;
   gint xpos = D_GEOMETRY_DEFAULT_XPOS, ypos = D_GEOMETRY_DEFAULT_YPOS;
   gint width = D_GEOMETRY_DEFAULT_WIDTH, height = D_GEOMETRY_DEFAULT_HEIGHT;

   /* set locale */
   curr_locale = g_strdup(setlocale(LC_ALL, NULL));
   gtk_set_locale();
   /* Initialize GUI and parse GTK related args */
   gtk_init(&argc, &argv);
   gdk_rgb_init();
   gtk_widget_set_default_colormap (gdk_rgb_get_cmap());
   gtk_widget_set_default_visual (gdk_rgb_get_visual());

   /* Handle command line options */
   while ((opt_flg = Dillo_get_opt(Options, argc, argv, &opt_argv, &idx))) {
     options_got |= opt_flg;
     switch (opt_flg) {
     case DILLO_CLI_VERSION:
        Dillo_print_version();
        return 0;
        break;
     case DILLO_CLI_HELP:
        Dillo_print_help(Options);
        return 0;
        break;
     case DILLO_CLI_XID:
        if (opt_argv[0][0] >= '0' && opt_argv[0][0] <= '9') {
           xid = strtol(opt_argv[0], NULL, 10);
        } else {
           g_printerr("Error: the XID must be an unsigned decimal numerical "
                      "value.\nThe offending value was: %s\n", opt_argv[0]);
           return -1;
        }
        break;
     case DILLO_CLI_FULLWINDOW:
     case DILLO_CLI_LOCAL:
        break;
     case DILLO_CLI_DBG_RENDERING:
        dillo_dbg_rendering = TRUE;
        break;
     case DILLO_CLI_GEOMETRY:
       if (a_Misc_parse_geometry(opt_argv[0], &xpos, &ypos, &width, &height))
          break;
     default:
        g_printerr("Error in command line options.\n");
        return -1;
        break;
     }
   }

   /* Send a delayed locale-related message */
   MSG("Setting locale to %s...\n", curr_locale);
   g_free(curr_locale);

   /* This lets threads in the file module end peacefully when aborted
    * todo: implement a cleaner mechanism (in file.c) */
   signal(SIGPIPE, SIG_IGN);

   /* This avoids making zombies when dpi-programs finish. */
   signal(SIGCHLD, Dillo_sigchld_handler);


   /* check that ~/.dillo exists, create it if it doesn't */
   dir = a_Misc_prepend_user_home(".dillo");
   Dillo_check_home_dir(dir);
   g_free(dir);

   a_Prefs_init();
   a_Dns_init();
   a_Http_init();
   a_Mime_init();
   a_Cache_init();
   a_Dicache_init();
   a_Interface_init();
   a_Dw_init();
   a_Cookies_init();

   /* -f overrides dillorc */
   if (options_got & DILLO_CLI_FULLWINDOW)
      prefs.fullwindow_start = TRUE;

   /* override dillorc geometry */
   if (options_got & DILLO_CLI_GEOMETRY) {
       prefs.width = width;
       prefs.height = height;
       prefs.xpos = xpos;
       prefs.ypos = ypos;
   }
   /* a_Nav_init() has been moved into this call because it needs to be
    * initialized with the new browser_window structure */
   bw = a_Interface_browser_window_new(prefs.width, prefs.height, xid);
   if ((prefs.xpos != D_GEOMETRY_DEFAULT_XPOS) ||
       (prefs.ypos != D_GEOMETRY_DEFAULT_YPOS))
      gtk_widget_set_uposition(bw->main_window, prefs.xpos, prefs.ypos);

   a_Bookmarks_init();

   /* Send dillo's startup screen */
   a_Nav_push(bw, prefs.start_page);

   for (i = idx; i < argc; i++) {
      /* If more than one URL/FILE, open in new window */
      if (i > idx)
         bw = a_Interface_browser_window_new(prefs.width, prefs.height, 0);

      start_url = Dillo_make_start_url(argv[i]);
      if (options_got & DILLO_CLI_LOCAL)
         a_Url_set_flags(start_url, URL_FLAGS(start_url) | URL_SpamSafe);
      a_Nav_push(bw, start_url);
      a_Url_free(start_url);
   }

   if (prefs.http_proxyuser && !a_Http_proxy_auth())
      a_Interface_proxy_passwd_dialog(bw);

   /* Start the GTK+ cycle */
   gtk_main();

   /*
    * Memory deallocating routines
    * (This can be left to the OS, but we'll do it, with a view to test
    *  and fix our memory management)
    */
   a_Cookies_freeall();
   a_Cache_freeall();
   a_Dicache_freeall();
   a_Http_freeall();
   a_Dns_freeall();
   a_Prefs_freeall();
   a_Dw_freeall();
   a_History_free();

   //a_Dpi_bye_dpid();
   MSG("Dillo: normal exit!\n");
   return 0;
}

/*
 * Check if '~/.dillo' directory exists.
 * If not, try to create it.
 */
static void Dillo_check_home_dir(char *dir)
{
   struct stat st;

   if ( stat(dir, &st) == -1 ) {
      if ( errno == ENOENT && mkdir(dir, 0700) < 0 ) {
         MSG("Dillo: error creating directory %s: %s\n",
             dir, g_strerror(errno));
      } else
         MSG("Dillo: error reading %s: %s\n", dir, g_strerror(errno));
   }
}


/*
 * Get next command line option.
 * Return value:
 *  opt_flg of the option
 *  0 if no more options found
 * -1 if an unrecognised option is found or if a mandatory option
 *    argument is not found.
 */
static gint
Dillo_get_opt(CLI_options *options, int argc,
              char **argv, char ***opt_argv, gint *cli_idx)
{
   gint i = 0, opt_flg = 0, n_arg, state;
   static int idx = 1;
   gint ret_val = 0;

   /* Allocate opt_argv */
   if (*opt_argv == NULL)
      *opt_argv = g_new0(gchar*, Dillo_get_max_opt(options) + 1);

   state = O_SEARCH;
   if (idx >= argc)
      state = O_DONE;

   while (1) {
      switch (state) {
      case O_SEARCH:
         for (i = 0; options[i].sh_opt; i++)
            if (strcmp(options[i].sh_opt, argv[idx]) == 0 ||
                strcmp(options[i].lg_opt, argv[idx]) == 0)
               break;
         state = (options[i].sh_opt) ? O_FOUND : O_NOTFOUND;
         break;
      case O_FOUND:
         ++idx;
         opt_flg  = options[i].opt_flg;
         n_arg = options[i].opt_argc;
         /* Find the required/optional arguments of the option */
         for (i = 0; idx < argc && i < abs(n_arg) && argv[idx][0] != '-'; i++)
            (*opt_argv)[i] = argv[idx++];
         (*opt_argv)[i] = NULL;

         /* Optional arguments have opt_argc < 0 */
         if (i < n_arg) {
            g_printerr("Option %s requires %d argument(s)\n",
                       argv[idx - i - 1], n_arg);
            state = O_ERROR;
         } else
            state = O_OK;
         break;
      case O_NOTFOUND:
         state = O_DONE;
         if (strcmp(argv[idx], "--") == 0)
            idx++;
         else if (argv[idx][0] == '-')
            state = O_ERROR;
         break;
      case O_OK:
         *cli_idx = idx;
         return opt_flg;
         break;
      case O_ERROR:
         ret_val = DILLO_CLI_ERROR;
         state = O_DONE;
         break;
      case O_DONE:
         g_free(*opt_argv);
         *cli_idx = idx;
         return ret_val;
         break;
      }
   }
}

/*
 * Print a short help text automatically generated from the options structure
 */
static void Dillo_print_help(CLI_options *options)
{
   g_print("\nUsage: dillo [OPTIONS] [URL|FILE]...\n"
              "Options:\n");

   for (; options && options->sh_opt; options++) {
      if (options->help)
         g_print("%s\n", options->help);
      else
         g_print("  %s, %s   *Undocumented*\n", options->sh_opt,
                 options->lg_opt);
   }
   g_print("  URL                    URL to browse.\n"
           "  FILE                   Local FILE to view.\n");
   g_print("\n");
}

/*
 * Print version (TODO: and maybe a copyright notice)
 */
static void Dillo_print_version(void)
{
   g_print("Dillo %s\n", VERSION);
}
