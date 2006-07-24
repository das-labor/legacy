/*
 * File: cookies.c
 *
 * Copyright 2001 Lars Clausen   <lrclause@cs.uiuc.edu>
 *                Jörgen Viksell <jorgen.viksell@telia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/* Handling of cookies takes place here.
 * This implementation aims to follow RFC 2965:
 * http://www.cis.ohio-state.edu/cs/Services/rfc/rfc-text/rfc2965.txt
 */

#ifndef DISABLE_COOKIES

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>       /* for time() and time_t */
#include <ctype.h>

#include "msg.h"
#include "IO/Url.h"
#include "misc.h"
#include "list.h"
#include "cookies.h"

#define DEBUG_LEVEL 8
#include "debug.h"

/* The maximum length of a line in the cookie file */
#define LINE_MAXLEN 4096

typedef enum {
   COOKIE_ACCEPT,
   COOKIE_ACCEPT_SESSION,
   COOKIE_DENY
} CookieControlAction;

typedef struct {
   CookieControlAction action;
   char *domain;
} CookieControl;

typedef struct {
   char *name;
   char *value;
   char *domain;
   char *path;
   time_t expires_at;
   guint version;
   char *comment;
   char *comment_url;
   gboolean secure;
   gboolean session_only;
   GList *ports;
} CookieData_t;

/* Hashtable indexed by domain, each value is a set of cookies for
 * that domain. */
static GHashTable *cookies;

/* Variables for access control */
static CookieControl *ccontrol = NULL;
static int num_ccontrol = 0;
static int num_ccontrol_max = 1;
static CookieControlAction default_action = COOKIE_DENY;

static gboolean disabled;
static FILE *file_stream;

static FILE *Cookies_fopen(const char *file, gchar *init_str);
static CookieControlAction Cookies_control_check(const DilloUrl *url);
static CookieControlAction Cookies_control_check_domain(const char *domain);
static int Cookie_control_init(void);
static void Cookies_parse_ports(const DilloUrl *set_url, CookieData_t *cookie,
                                const char *port_str);
static char *Cookies_build_ports_str(CookieData_t *cookie);
static char *Cookies_strip_path(const char *path);
static void Cookies_add_cookie(CookieData_t *cookie);
static void Cookies_remove_cookie(CookieData_t *cookie);
static gint Cookies_equals(gconstpointer a, gconstpointer b);

/*
 * Return a file pointer. If the file doesn't exist, try to create it,
 * with the optional 'init_str' as its content.
 */
static FILE *Cookies_fopen(const char *filename, gchar *init_str)
{
   FILE *F_in;
   int fd;

   if ((F_in = fopen(filename, "r+")) == NULL) {
      /* Create the file */
      fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
      if (fd != -1) {
         if (init_str)
            write(fd, init_str, strlen(init_str));
         close(fd);

         DEBUG_MSG(10, "Cookies: Created file: %s\n", filename);
         F_in = Cookies_fopen(filename, NULL);
      } else {
         DEBUG_MSG(10, "Cookies: Could not create file: %s!\n", filename);
      }
   }

   /* set close on exec */
   fcntl(fileno(F_in), F_SETFD, FD_CLOEXEC | fcntl(fileno(F_in), F_GETFD));

   return F_in;
}

static void Cookies_free_cookie(CookieData_t *cookie)
{
   g_free(cookie->name);
   g_free(cookie->value);
   g_free(cookie->domain);
   g_free(cookie->path);
   g_free(cookie->comment);
   g_free(cookie->comment_url);
   g_list_free(cookie->ports);
   g_free(cookie);
}

/*
 * Initialize the cookies module
 * (The 'disabled' variable is writable only within a_Cookies_init)
 */
void a_Cookies_init()
{
   CookieData_t *cookie;
   char *filename;
   char line[LINE_MAXLEN];
#ifndef HAVE_LOCKF
   struct flock lck;
#endif

   /* Default setting */
   disabled = TRUE;

   /* Read and parse the cookie control file (cookiesrc) */
   if (Cookie_control_init() != 0) {
      DEBUG_MSG(10, "Disabling cookies.\n");
      return;
   }

   /* Get a stream for the cookies file */
   filename = a_Misc_prepend_user_home(".dillo/cookies");
   file_stream = Cookies_fopen(filename, NULL);
   g_free(filename);

   if (!file_stream) {
      DEBUG_MSG(10, "ERROR: Can't open ~/.dillo/cookies, disabling cookies\n");
      return;
   }

   /* Try to get a lock from the file descriptor */
#ifdef HAVE_LOCKF
   disabled = (lockf(fileno(file_stream), F_TLOCK, 0) == -1);
#else /* POSIX lock */
   lck.l_start = 0; /* start at beginning of file */
   lck.l_len = 0;  /* lock entire file */
   lck.l_type = F_WRLCK;
   lck.l_whence = SEEK_SET;  /* absolute offset */

   disabled = (fcntl(fileno(file_stream), F_SETLK, &lck) == -1);
#endif
   if (disabled) {
      DEBUG_MSG(10, "The cookies file has a file lock:\n"
                    "  disabling cookies in this dillo!\n");
      fclose(file_stream);
      return;
   }

   DEBUG_MSG(10, "Enabling cookies as from cookiesrc...\n");

   cookies = g_hash_table_new(g_str_hash, g_str_equal);

   /* Get all lines in the file */
   while (!feof(file_stream)) {
      line[0] = '\0';
      fgets(line, LINE_MAXLEN, file_stream);

      /* Remove leading and trailing whitespaces */
      g_strstrip(line);

      if (line[0] != '\0') {
         /* Would use g_strsplit, but it doesn't give empty trailing pieces.
          */
         /* Split the row into pieces using a tab as the delimiter.
          * pieces[0] The version this cookie was set as (0 / 1)
          * pieces[1] The domain name
          * pieces[2] A comma separated list of accepted ports
          * pieces[3] The path
          * pieces[4] Is the cookie unsecure or secure (0 / 1)
          * pieces[5] Timestamp of expire date
          * pieces[6] Name of the cookie
          * pieces[7] Value of the cookie
          * pieces[8] Comment
          * pieces[9] Comment url
          */
         CookieControlAction action;
         char *piece;
         char *line_marker = line;

         cookie = g_new0(CookieData_t, 1);

         cookie->session_only = FALSE;
         piece = d_strsep(&line_marker, "\t");
         if (piece != NULL)
            cookie->version = strtol(piece, NULL, 10);
         cookie->domain  = g_strdup(d_strsep(&line_marker, "\t"));
         Cookies_parse_ports(NULL, cookie, d_strsep(&line_marker, "\t"));
         cookie->path = g_strdup(d_strsep(&line_marker, "\t"));
         piece = d_strsep(&line_marker, "\t");
         if (piece != NULL && piece[0] == '1')
            cookie->secure = TRUE;
         piece = d_strsep(&line_marker, "\t");
         if (piece != NULL)
            cookie->expires_at = (time_t) strtol(piece, NULL, 10);
         cookie->name = g_strdup(d_strsep(&line_marker, "\t"));
         cookie->value = g_strdup(d_strsep(&line_marker, "\t"));
         cookie->comment = g_strdup(d_strsep(&line_marker, "\t"));
         cookie->comment_url = g_strdup(d_strsep(&line_marker, "\t"));

         if (!cookie->domain || cookie->domain[0] == '\0' ||
             !cookie->path || cookie->path[0] != '/' ||
             !cookie->name || cookie->name[0] == '\0' ||
             !cookie->value) {
            DEBUG_MSG(8, "Malformed line in cookies file!\n");
            Cookies_free_cookie(cookie);
            continue;
         }

         action = Cookies_control_check_domain(cookie->domain);
         if (action == COOKIE_DENY) {
            Cookies_free_cookie(cookie);
            continue;
         } else if (action == COOKIE_ACCEPT_SESSION) {
            cookie->session_only = TRUE;
         }

         /* Save cookie in memory */
         Cookies_add_cookie(cookie);
      }
   }
}

/*
 * Save the cookies and remove them from the hash table
 */
static gboolean Cookies_freeall_cb(gpointer key,
                                   gpointer value,
                                   gpointer data)
{
   CookieData_t *cookie;
   GList *domain_cookies = value;
   char *ports_str;

   for (; domain_cookies; domain_cookies = g_list_next(domain_cookies)) {
      cookie = domain_cookies->data;

      if (!cookie->session_only) {
         ports_str = Cookies_build_ports_str(cookie);
         fprintf(file_stream, "%d\t%s\t%s\t%s\t%d\t%ld\t%s\t%s\t%s\t%s\n",
                 cookie->version,
                 cookie->domain,
                 ports_str,
                 cookie->path,
                 cookie->secure ? 1 : 0,
                 (long)cookie->expires_at,
                 cookie->name,
                 cookie->value,
                 (cookie->comment) ? cookie->comment : "",
                 (cookie->comment_url) ? cookie->comment_url : "");
         g_free(ports_str);
      }

      Cookies_free_cookie(cookie);
   }
   g_list_free(value);
   g_free(key);

   /* Return TRUE to tell GLIB to free this key from the hash table */
   return TRUE;
}

/*
 * Flush cookies to disk and free all the memory allocated.
 */
void a_Cookies_freeall()
{
   int fd;
#ifndef HAVE_LOCKF
   struct flock lck;
#endif

   if (disabled)
      return;

   rewind(file_stream);
   fd = fileno(file_stream);
   ftruncate(fd, 0);

   g_hash_table_foreach_remove(cookies, Cookies_freeall_cb, NULL);

#ifdef HAVE_LOCKF
   lockf(fd, F_ULOCK, 0);
#else  /* POSIX file lock */
   lck.l_start = 0; /* start at beginning of file */
   lck.l_len = 0;  /* lock entire file */
   lck.l_type = F_UNLCK;
   lck.l_whence = SEEK_SET;  /* absolute offset */

   fcntl(fileno(file_stream), F_SETLKW, &lck);
#endif
   fclose(file_stream);
}

static char *months[] =
{ "",
  "Jan", "Feb", "Mar",
  "Apr", "May", "Jun",
  "Jul", "Aug", "Sep",
  "Oct", "Nov", "Dec"
};

/*
 * Take a months name and return a number between 1-12.
 * E.g. 'April' -> 4
 */
static int Cookies_get_month(const char *month_name)
{
   int i;

   for (i = 1; i <= 12; i++) {
      if (!g_strncasecmp(months[i], month_name, 3))
         return i;
   }
   return 0;
}

/*
 * Return a local timestamp from a GMT date string
 * Accept: RFC-1123 | RFC-850 | ANSI asctime | Old Netscape format.
 *
 *   Wdy, DD-Mon-YY HH:MM:SS GMT
 *   Wdy, DD-Mon-YYYY HH:MM:SS GMT
 *   Weekday, DD-Mon-YY HH:MM:SS GMT
 *   Weekday, DD-Mon-YYYY HH:MM:SS GMT
 *   Tue May 21 13:46:22 1991\n
 *   Tue May 21 13:46:22 1991
 *
 * (return 0 on malformed date string syntax)
 */
static time_t Cookies_create_timestamp(const char *expires)
{
   time_t ret;
   int day, month, year, hour, minutes, seconds;
   gchar *cp;
   gchar *E_msg =
      "Cookies: Expire date is malformed!\n"
      " (should be RFC-1123 | RFC-850 | ANSI asctime)\n"
      " Ignoring cookie: [%s]\n";

   cp = strchr(expires, ',');
   if (!cp && (strlen(expires) == 24 || strlen(expires) == 25)) {
      /* Looks like ANSI asctime format... */
      cp = (gchar *)expires;
      day = strtol(cp + 8, NULL, 10);       /* day */
      month = Cookies_get_month(cp + 4);    /* month */
      year = strtol(cp + 20, NULL, 10);     /* year */
      hour = strtol(cp + 11, NULL, 10);     /* hour */
      minutes = strtol(cp + 14, NULL, 10);  /* minutes */
      seconds = strtol(cp + 17, NULL, 10);  /* seconds */

   } else if (cp && (cp - expires == 3 || cp - expires > 5) &&
                    (strlen(cp) == 24 || strlen(cp) == 26)) {
      /* RFC-1123 | RFC-850 format | Old Netscape format */
      day = strtol(cp + 2, NULL, 10);
      month = Cookies_get_month(cp + 5);
      year = strtol(cp + 9, &cp, 10);
      /* todo: tricky, because two digits for year IS ambiguous! */
      year += (year < 70) ? 2000 : ((year < 100) ? 1900 : 0);
      hour = strtol(cp + 1, NULL, 10);
      minutes = strtol(cp + 4, NULL, 10);
      seconds = strtol(cp + 7, NULL, 10);

   } else {
      DEBUG_MSG(5, E_msg, expires);
      return (time_t) 0;
   }

   /* Error checks  --this may be overkill */
   if (!(day > 0 && day < 32 && month > 0 && month < 13 && year > 1970 &&
         hour >= 0 && hour < 24 && minutes >= 0 && minutes < 60 &&
         seconds >= 0 && seconds < 60)) {
      DEBUG_MSG(5, E_msg, expires);
      return (time_t) 0;
   }

   /* Calculate local timestamp.
    * [stolen from Lynx... (http://lynx.browser.org)] */
   month -= 3;
   if (month < 0) {
      month += 12;
      year--;
   }

   day += (year - 1968) * 1461 / 4;
   day += ((((month * 153) + 2) / 5) - 672);
   ret = (time_t)((day * 60 * 60 * 24) +
                  (hour * 60 * 60) +
                  (minutes * 60) +
                  seconds);

   DEBUG_MSG(4, "\tExpires in %ld seconds, at %s",
             (long)ret - time(NULL),
             ctime(&ret));

   return ret;
}

/*
 * Parse a string containing a list of port numbers.
 */
static void Cookies_parse_ports(const DilloUrl *set_url, CookieData_t *cookie,
                                const char *port_str)
{
   if ((!port_str || !port_str[0]) && set_url) {
      /* There was no list, so only the calling urls port should be allowed. */
      cookie->ports = g_list_append(cookie->ports,
                                    GINT_TO_POINTER(URL_PORT(set_url)));
   } else if (port_str[0] == '"' && port_str[1] != '"') {
      char **tokens, **i;
      int port;

      tokens = g_strsplit(port_str + 1, ",", -1);
      for (i = tokens; *i; ++i) {
         port = strtol(*i, NULL, 10);
         if (port > 0) {
            cookie->ports = g_list_append(cookie->ports,
                                          GINT_TO_POINTER(port));
         }
      }
      g_strfreev(tokens);
   }
}

/*
 * Build a string of the ports in 'cookie'.
 */
static char *Cookies_build_ports_str(CookieData_t *cookie)
{
   GString *gstr;
   GList *list;
   char *ret;

   gstr = g_string_new("\"");
   for (list = cookie->ports; list; list = g_list_next(list))
      g_string_sprintfa(gstr, "%d,", GPOINTER_TO_INT(list->data));

   /* Remove any trailing comma */
   if (gstr->len > 1)
      g_string_erase(gstr, gstr->len - 1, 1);

   g_string_append(gstr, "\"");

   ret = gstr->str;
   g_string_free(gstr, FALSE);

   return ret;
}

/*
 * Used by g_list_insert_sorted() to sort the cookies by most specific path
 */
static gint Cookies_compare(gconstpointer a, gconstpointer b)
{
   const CookieData_t *ca = a, *cb = b;

   return strcmp(ca->path, cb->path);
}

static void Cookies_add_cookie(CookieData_t *cookie)
{
   GList *domain_cookies, *tmp;
   char *domain_str;

   /* Don't add an expired cookie */
   if (!cookie->session_only && cookie->expires_at < time(NULL)) {
      Cookies_free_cookie(cookie);
      return;
   }

   domain_cookies = g_hash_table_lookup(cookies, cookie->domain);

   if (domain_cookies) {
      /* Respect the limit of 20 cookies per domain */
      if (g_list_length(domain_cookies) > 20) {
         DEBUG_MSG(8, "There are too many cookies for this domain (%s)\n",
                   cookie->domain);
         Cookies_free_cookie(cookie);
         return;
      }

      /* Remove any cookies with the same name and path */
      while ((tmp = g_list_find_custom(domain_cookies, cookie,
                                       Cookies_equals))) {
         Cookies_remove_cookie(tmp->data);
         domain_cookies = g_hash_table_lookup(cookies, cookie->domain);
      }
   }

   /* Allocate string key when no domain_cookies are left
    * (because remove_cookie has then killed the key, when it was there) */
   domain_str = domain_cookies ? cookie->domain : g_strdup(cookie->domain);

   DEBUG_MSG(5, "Adding cookie '%s' for %s\n", cookie->name, cookie->domain);
   domain_cookies = g_list_insert_sorted(domain_cookies, cookie,
                                         Cookies_compare);
   g_hash_table_insert(cookies, domain_str, domain_cookies);
}

/*
 * Remove the cookie from the domain list.
 * If the domain list is empty, free the hash table entry.
 * Free the cookie.
 */
static void Cookies_remove_cookie(CookieData_t *cookie)
{
   GList *list;
   gpointer orig_key;
   gpointer orig_val;

   if (g_hash_table_lookup_extended(cookies, cookie->domain,
                                    &orig_key, &orig_val)) {
      list = g_list_remove(orig_val, cookie);

      if (list) {
         /* Make sure that we have the correct start of the list stored */
         g_hash_table_insert(cookies, cookie->domain, list);
      } else {
         g_hash_table_remove(cookies, cookie->domain);
         g_free(orig_key);
      }
   } else {
      DEBUG_MSG(5, "Attempting to remove a cookie that doesn't exist!\n");
   }

   Cookies_free_cookie(cookie);
}

/*
 * Return the attribute that is present at *cookie_str. This function
 * will also attempt to advance cookie_str past any equal-sign.
 */
static char *Cookies_parse_attr(char **cookie_str)
{
   char *str = *cookie_str;
   guint i, end = 0;
   gboolean got_attr = FALSE;

   for (i = 0; ; i++) {
      switch (str[i]) {
      case ' ':
      case '\t':
      case '=':
      case ';':
         got_attr = TRUE;
         if (end == 0)
            end = i;
         break;
      case ',':
         *cookie_str = str + i;
         return g_strndup(str, i);
         break;
      case '\0':
         if (!got_attr) {
            end = i;
            got_attr = TRUE;
         }
         /* fall through! */
      default:
         if (got_attr) {
            *cookie_str = str + i;
            return g_strndup(str, end);
         }
         break;
      }
   }

   return NULL;
}

/*
 * Get the value starting at *cookie_str.
 * broken_syntax: watch out for stupid syntax (comma in unquoted string...)
 */
static char *Cookies_parse_value(char **cookie_str,
                                 gboolean broken_syntax,
                                 gboolean keep_quotes)
{
   guint i, end;
   char *str = *cookie_str;

   for (i = end = 0; !end; ++i) {
      switch (str[i]) {
      case ' ':
      case '\t':
         if (!broken_syntax && str[0] != '\'' && str[0] != '"') {
            *cookie_str = str + i + 1;
            end = 1;
         }
         break;
      case '\'':
      case '"':
         if (i != 0 && str[i] == str[0]) {
            char *tmp = str + i;

            while (*tmp != '\0' && *tmp != ';' && *tmp != ',')
               tmp++;

            *cookie_str = (*tmp == ';') ? tmp + 1 : tmp;

            if (keep_quotes)
               i++;
            end = 1;
         }
         break;
      case '\0':
         *cookie_str = str + i;
         end = 1;
         break;
      case ',':
         if (str[0] != '\'' && str[0] != '"' && !broken_syntax) {
            /* A new cookie starts here! */
            *cookie_str = str + i;
            end = 1;
         }
         break;
      case ';':
         if (str[0] != '\'' && str[0] != '"') {
            *cookie_str = str + i + 1;
            end = 1;
         }
         break;
      default:
         break;
      }
   }
   /* keep i as an index to the last char */
   --i;

   if ((str[0] == '\'' || str[0] == '"') && !keep_quotes) {
      return i > 1 ? g_strndup(str + 1, i - 1) : NULL;
   } else {
      return g_strndup(str, i);
   }
}

/*
 * Parse one cookie...
 */
static CookieData_t *Cookies_parse_one(const DilloUrl *set_url,
                                       char **cookie_str)
{
   CookieData_t *cookie;
   char *str = *cookie_str;
   char *attr;
   char *value;
   int num_attr = 0;
   gboolean max_age = FALSE;
   gboolean discard = FALSE;

   cookie = g_new0(CookieData_t, 1);
   cookie->session_only = TRUE;

   /* Iterate until there is nothing left of the string OR we come
    * across a comma representing the start of another cookie */
   while (*str != '\0' && *str != ',') {
      /* Skip whitespace */
      while (isspace(*str))
         str++;

      /* Get attribute */
      attr = Cookies_parse_attr(&str);
      if (!attr) {
         DEBUG_MSG(8, "Failed to parse cookie attribute!\n");
         Cookies_free_cookie(cookie);
         return NULL;
      }

      /* Get the value for the attribute and store it */
      if (num_attr == 0) {
         /* The first attr, which always is the user supplied attr, may
          * have the same name as an ordinary attr. Hence this workaround. */
         cookie->name = g_strdup(attr);
         cookie->value = Cookies_parse_value(&str, FALSE, TRUE);
      } else if (g_strcasecmp(attr, "Path") == 0) {
         value = Cookies_parse_value(&str, FALSE, FALSE);
         cookie->path = value;
      } else if (g_strcasecmp(attr, "Domain") == 0) {
         value = Cookies_parse_value(&str, FALSE, FALSE);
         cookie->domain = value;
      } else if (g_strcasecmp(attr, "Discard") == 0) {
         cookie->session_only = TRUE;
         discard = TRUE;
      } else if (g_strcasecmp(attr, "Max-Age") == 0) {
         if (!discard) {
            value = Cookies_parse_value(&str, FALSE, FALSE);

            if (value) {
               cookie->expires_at = time(NULL) + strtol(value, NULL, 10);
               cookie->session_only = FALSE;
               max_age = TRUE;
               g_free(value);
            } else {
               DEBUG_MSG(8, "Failed to parse cookie value!\n");
               Cookies_free_cookie(cookie);
               return NULL;
            }
         }
      } else if (g_strcasecmp(attr, "Expires") == 0) {
         if (!max_age && !discard) {
            DEBUG_MSG(5, "Old netscape-style cookie...\n");
            value = Cookies_parse_value(&str, TRUE, FALSE);
            if (value) {
               cookie->expires_at = Cookies_create_timestamp(value);
               cookie->session_only = FALSE;
               g_free(value);
            } else {
               DEBUG_MSG(8, "Failed to parse cookie value!\n");
               Cookies_free_cookie(cookie);
               return NULL;
            }
         }
      } else if (g_strcasecmp(attr, "Port") == 0) {
         value = Cookies_parse_value(&str, FALSE, TRUE);
         Cookies_parse_ports(set_url, cookie, value);
         g_free(value);
      } else if (g_strcasecmp(attr, "Comment") == 0) {
         value = Cookies_parse_value(&str, FALSE, FALSE);
         cookie->comment = value;
      } else if (g_strcasecmp(attr, "CommentURL") == 0) {
         value = Cookies_parse_value(&str, FALSE, FALSE);
         cookie->comment_url = value;
      } else if (g_strcasecmp(attr, "Version") == 0) {
         value = Cookies_parse_value(&str, FALSE, FALSE);

         if (value) {
            cookie->version = strtol(value, NULL, 10);
            g_free(value);
         } else {
            DEBUG_MSG(8, "Failed to parse cookie value!\n");
            Cookies_free_cookie(cookie);
            return NULL;
         }
      } else if (g_strcasecmp(attr, "Secure") == 0) {
         cookie->secure = TRUE;
      } else {
         /* Oops! this can't be good... */
         g_free(attr);
         Cookies_free_cookie(cookie);
         DEBUG_MSG(8, "Cookie contains illegal attribute!\n");
         return NULL;
      }

      g_free(attr);
      num_attr++;
   }

   *cookie_str = (*str == ',') ? str + 1 : str;

   if (cookie->name && cookie->value) {
      return cookie;
   } else {
      DEBUG_MSG(8, "Cookie missing name and/or value!\n");
      Cookies_free_cookie(cookie);
      return NULL;
   }
}

/*
 * Iterate the cookie string until we catch all cookies.
 * Return Value: a list with all the cookies! (or NULL upon error)
 */
static GSList *Cookies_parse_string(const DilloUrl *set_url,
                                    char *cookie_string)
{
   CookieData_t *cookie;
   GSList *ret = NULL;
   char *str = cookie_string;

   /* The string may contain several cookies separated by comma.
    * We'll iterate until we've catched them all */
   while (*str) {
      cookie = Cookies_parse_one(set_url, &str);

      if (cookie) {
         ret = g_slist_append(ret, cookie);
      } else {
         DEBUG_MSG(8, "Malformed cookie field, ignoring cookie: %s\n",
                   cookie_string);
         return NULL;
      }
   }

   return ret;
}

/*
 * Compare cookies by name and path (return 0 if equal)
 */
static gint Cookies_equals(gconstpointer a, gconstpointer b)
{
   const CookieData_t *ca = a, *cb = b;

   return (strcmp(ca->name, cb->name) || strcmp(ca->path, cb->path));
}

/*
 * Validate cookies domain against some security checks.
 */
static gboolean Cookies_validate_domain(CookieData_t *cookie,
                                        const DilloUrl *set_url)
{
   const char *host;
   int dots, diff, i;
   gboolean is_ip;

   /* Make sure that the path is set to something */
   if (!cookie->path || cookie->path[0] != '/') {
      g_free(cookie->path);
      cookie->path = Cookies_strip_path(URL_PATH_(set_url));
   }

   /* If the server never set a domain, or set one without a leading
    * dot (which isn't allowed), we use the calling URL's hostname. */
   if (cookie->domain == NULL || cookie->domain[0] != '.') {
      g_free(cookie->domain);
      cookie->domain = g_strdup(URL_HOST(set_url));
      return TRUE;
   }

   /* Count the number of dots and also find out if it is an IP-address */
   is_ip = TRUE;
   for (i = 0, dots = 0; cookie->domain[i] != '\0'; i++) {
      if (cookie->domain[i] == '.')
         dots++;
      else if (!isdigit(cookie->domain[i]))
         is_ip = FALSE;
   }

   /* A valid domain must have at least two dots in it */
   /* NOTE: this breaks cookies on localhost... */
   if (dots < 2) {
      return FALSE;
   }

   /* Now see if the url matches the domain */
   host = URL_HOST(set_url);
   diff = strlen(host) - i;
   if (diff > 0) {
      if (g_strcasecmp(host + diff, cookie->domain))
         return FALSE;

      if (!is_ip) {
         /* "x.y.test.com" is not allowed to set cookies for ".test.com";
          *  only an url of the form "y.test.com" would be. */
         while ( diff-- )
            if (host[diff] == '.')
               return FALSE;
      }
   }

   return TRUE;
}

/*
 * Strip of the filename from a full path
 */
static char *Cookies_strip_path(const char *path)
{
   char *ret;
   guint len;

   if (path) {
      len = strlen(path);

      while (len && path[len] != '/')
         len--;
      ret = g_strndup(path, len + 1);
   } else {
      ret = g_strdup("/");
   }

   return ret;
}

/*
 * Set the value corresponding to the cookie string
 */
void a_Cookies_set(GList *cookie_strings, const DilloUrl *set_url)
{
   CookieControlAction action;
   GSList *list;

   if (disabled)
      return;

   action = Cookies_control_check(set_url);
   if (action == COOKIE_DENY) {
      DEBUG_MSG(5, "Cookies: denied SET for %s\n", URL_HOST_(set_url));
      return;
   }

   while (cookie_strings != NULL ) {
      char *cookie_string = cookie_strings->data;
      list = Cookies_parse_string(set_url, cookie_string);

      while (list) {
         CookieData_t *cookie = list->data;

         if (Cookies_validate_domain(cookie, set_url)) {
            if (action == COOKIE_ACCEPT_SESSION)
               cookie->session_only = TRUE;

            Cookies_add_cookie(cookie);
         } else {
            DEBUG_MSG(5, "Rejecting cookie for %s from %s:\n",
                      cookie->domain, URL_STR_(set_url));
            Cookies_free_cookie(cookie);
         }

         list = g_slist_remove(list, list->data);
      }

      cookie_strings = g_list_next(cookie_strings);
   }
}

/*
 * Compare the cookie with the supplied data to see if it matches
 */
static gboolean Cookies_match(CookieData_t *cookie, const DilloUrl *url,
                              const char *path, gboolean is_ssl)
{
   /* Insecure cookies matches both secure and insecure urls, secure
      cookies matches only secure urls */
   if (cookie->secure && !is_ssl)
      return FALSE;

   /* Check that the cookie path is a subpath of the current path */
   if (strncmp(cookie->path, path, strlen(cookie->path)) != 0)
      return FALSE;

   /* Check if the port of the request URL matches any
    * of those set in the cookie */
   if (cookie->ports) {
      GList *list;
      int port = URL_PORT(url);

      for (list = cookie->ports; list; list = g_list_next(list)) {
         if (GPOINTER_TO_INT(list->data) == port)
            return TRUE;
      }

      return FALSE;
   }

   /* It's a match */
   return TRUE;
}

/*
 * Return a string that contains all relevant cookies as headers.
 */
char *a_Cookies_get(const DilloUrl *request_url)
{
   char *domain_string, *q, *str, *path;
   CookieControlAction action;
   CookieData_t *cookie;
   GList *matching_cookies = NULL;
   GList *domain_cookie;
   gboolean is_ssl;
   GString *cookie_gstring;

   if (disabled)
      return g_strdup("");

   action = Cookies_control_check(request_url);
   if (action == COOKIE_DENY) {
      DEBUG_MSG(5, "Cookies: denied GET for %s\n", URL_HOST_(request_url));
      return g_strdup("");
   }

   path = Cookies_strip_path(URL_PATH_(request_url));

   /* Check if the protocol is secure or not */
   is_ssl = (!g_strcasecmp(URL_SCHEME(request_url), "https"));

   for (domain_string = (char *) URL_HOST(request_url);
        domain_string != NULL && *domain_string;
        domain_string = strchr(domain_string+1, '.')) {
      domain_cookie = g_hash_table_lookup(cookies, domain_string);

      while (domain_cookie) {
         cookie = domain_cookie->data;
         domain_cookie = g_list_next(domain_cookie);

         /* Remove expired cookie. */
         if (!cookie->session_only && cookie->expires_at < time(NULL)) {
            Cookies_remove_cookie(cookie);
            continue;
         }

         /* Check if the cookie matches the requesting URL */
         if (Cookies_match(cookie, request_url, path, is_ssl))
            matching_cookies = g_list_append(matching_cookies, cookie);
      }
   }

   /* Found the cookies, now make the string */
   cookie_gstring = g_string_new("");
   if (matching_cookies != NULL) {
      CookieData_t *first_cookie = matching_cookies->data;

      g_string_sprintfa(cookie_gstring, "Cookie: ");

      if (first_cookie->version != 0)
         g_string_sprintfa(cookie_gstring, "$Version=\"%d\"; ",
                           first_cookie->version);

      while (matching_cookies) {
         cookie = matching_cookies->data;
         q = (cookie->version == 0 ? "" : "\"");

         g_string_sprintfa(cookie_gstring,
                           "%s=%s; $Path=%s%s%s; $Domain=%s%s%s",
                           cookie->name, cookie->value,
                           q, cookie->path, q, q, cookie->domain, q);

         if (cookie->ports) {
            char *ports_str = Cookies_build_ports_str(cookie);
            g_string_sprintfa(cookie_gstring, "; $Port=%s", ports_str);
            g_free(ports_str);
         }

         matching_cookies = g_list_next(matching_cookies);
         g_string_append(cookie_gstring, matching_cookies ? "; " : "\r\n");
      }

      DEBUG_MSG(4, "Final cookie string: %s", cookie_gstring->str);
   }

   g_free(path);
   str = cookie_gstring->str;
   g_string_free(cookie_gstring, FALSE);
   return str;
}

/* -------------------------------------------------------------
 *                    Access control routines
 * ------------------------------------------------------------- */


/*
 * Get the cookie control rules (from cookiesrc).
 * Return value:
 *   0 = Parsed OK, with cookies enabled
 *   1 = Parsed OK, with cookies disabled
 *   2 = Can't open the control file
 */
static int Cookie_control_init(void)
{
   CookieControl cc;
   FILE *stream;
   char *filename;
   char line[LINE_MAXLEN];
   char domain[LINE_MAXLEN];
   char rule[LINE_MAXLEN];
   int i, j;
   gboolean enabled = FALSE;

   /* Get a file pointer */
   filename = a_Misc_prepend_user_home(".dillo/cookiesrc");
   stream = Cookies_fopen(filename, "DEFAULT DENY\n");
   g_free(filename);

   if (!stream)
      return 2;

   /* Get all lines in the file */
   while (!feof(stream)) {
      line[0] = '\0';
      fgets(line, LINE_MAXLEN, stream);

      /* Remove leading and trailing whitespaces */
      g_strstrip(line);

      if (line[0] != '\0' && line[0] != '#') {
         i = 0;
         j = 0;

         /* Get the domain */
         while (!isspace(line[i]))
            domain[j++] = line[i++];
         domain[j] = '\0';

         /* Skip past whitespaces */
         i++;
         while (isspace(line[i]))
            i++;

         /* Get the rule */
         j = 0;
         while (line[i] != '\0' && !isspace(line[i]))
            rule[j++] = line[i++];
         rule[j] = '\0';

         if (g_strcasecmp(rule, "ACCEPT") == 0)
            cc.action = COOKIE_ACCEPT;
         else if (g_strcasecmp(rule, "ACCEPT_SESSION") == 0)
            cc.action = COOKIE_ACCEPT_SESSION;
         else if (g_strcasecmp(rule, "DENY") == 0)
            cc.action = COOKIE_DENY;
         else {
            MSG("Cookies: rule '%s' for domain '%s' is not recognised.\n",
                rule, domain);
            continue;
         }

         cc.domain = g_strdup(domain);
         if (g_strcasecmp(cc.domain, "DEFAULT") == 0) {
            /* Set the default action */
            default_action = cc.action;
            g_free(cc.domain);
         } else {
            a_List_add(ccontrol, num_ccontrol, num_ccontrol_max);
            ccontrol[num_ccontrol++] = cc;
         }

         if (cc.action != COOKIE_DENY)
            enabled = TRUE;
      }
   }

   fclose(stream);

   return (enabled ? 0 : 1);
}

/*
 * Check the rules for an appropriate action for this domain
 */
static CookieControlAction Cookies_control_check_domain(const char *domain)
{
   int i, diff;

   for (i = 0; i < num_ccontrol; i++) {
      if (ccontrol[i].domain[0] == '.') {
         diff = strlen(domain) - strlen(ccontrol[i].domain);
         if (diff >= 0) {
            if (g_strcasecmp(domain + diff, ccontrol[i].domain) != 0)
               continue;
         } else {
            continue;
         }
      } else {
         if (g_strcasecmp(domain, ccontrol[i].domain) != 0)
            continue;
      }

      /* If we got here we have a match */
      return( ccontrol[i].action );
   }

   return default_action;
}

/*
 * Same as the above except it takes an URL
 */
static CookieControlAction Cookies_control_check(const DilloUrl *url)
{
   return Cookies_control_check_domain(URL_HOST(url));
}

#endif /* !DISABLE_COOKIES */
