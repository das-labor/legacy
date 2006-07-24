/*
 * File: file.c :)
 *
 * Copyright (C) 2000 - 2004 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Directory scanning is no longer streamed, but it gets sorted instead!
 * Directory entries on top, files next.
 * With new HTML layout.
 */

#include <pthread.h>

#include <ctype.h>           /* for tolower */
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/un.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>           /* for errno */
#include <glib.h>

#include "dpiutil.h"

#define MAXNAMESIZE 30
#define HIDE_DOTFILES TRUE

#define _MSG(fmt...)
#define MSG(fmt...)  g_print("[file dpi]: " fmt)

enum {
   FILE_OK,
   FILE_NOT_FOUND,
   FILE_NO_ACCESS
};

typedef struct {
   char *full_path;
   const char *filename;
   off_t size;
   mode_t mode;
   time_t mtime;
} FileInfo;

typedef struct {
   char *dirname;
   GList *flist;       /* List of files and subdirectories (for sorting) */
} DilloDir;

typedef struct {
   SockHandler *sh;
   gint status;
   gint old_style;
   pthread_t thrID;
   gint done;
} ClientInfo;

/*
 * Forward references
 */
static const char *File_content_type(const char *filename);
static gint File_get_file(ClientInfo *Client,
                          const gchar *filename,
                          struct stat *sb,
                          const char *orig_url);
static gint File_get_dir(ClientInfo *Client,
                         const gchar *DirName,
                         const char *orig_url);
static gchar *File_html_escape(const gchar *str);

/*
 * Global variables
 */
static volatile gint DPIBYE = 0;
static volatile gint ThreadRunning = 0;
static gint OLD_STYLE = 0;
/* A list for the clients we are serving */
static GList *Clients = NULL;
/* a mutex for operations on clients */
static pthread_mutex_t ClMut;

/*
 * Close a file descriptor, but handling EINTR
 */
static void File_close(int fd)
{
   while (close(fd) < 0 && errno == EINTR)
      ;
}

/*
 * Detects 'Content-Type' when the server does not supply one.
 * It uses the magic(5) logic from file(1). Currently, it
 * only checks the few mime types that Dillo supports.
 *
 * 'Data' is a pointer to the first bytes of the raw data.
 * (this is a copy of a_Misc_get_content_type_from_data())
 */
static const gchar *File_get_content_type_from_data(void *Data, size_t Size)
{
   static const gchar *Types[] = {
      "application/octet-stream",
      "text/html", "text/plain",
      "image/gif", "image/png", "image/jpeg",
   };
   gint Type = 0;
   gchar *p = Data;
   size_t i;

   /* HTML try */
   for (i = 0; i < Size && isspace(p[i]); ++i);
   if ((Size - i >= 5  && !g_strncasecmp(p+i, "<html", 5)) ||
       (Size - i >= 5  && !g_strncasecmp(p+i, "<head", 5)) ||
       (Size - i >= 6  && !g_strncasecmp(p+i, "<title", 6)) ||
       (Size - i >= 14 && !g_strncasecmp(p+i, "<!doctype html", 14)) ||
       /* this line is workaround for FTP through the Squid proxy */
       (Size - i >= 17 && !g_strncasecmp(p+i, "<!-- HTML listing", 17))) {

      Type = 1;

   /* Images */
   } else if (Size >= 4 && !g_strncasecmp(p, "GIF8", 4)) {
      Type = 3;
   } else if (Size >= 4 && !g_strncasecmp(p, "\x89PNG", 4)) {
      Type = 4;
   } else if (Size >= 2 && !g_strncasecmp(p, "\xff\xd8", 2)) {
      /* JPEG has the first 2 bytes set to 0xffd8 in BigEndian - looking
       * at the character representation should be machine independent. */
      Type = 5;

   /* Text */
   } else {
      /* We'll assume ASCII if chars are below 128 (after all, this
       * is a last resort when the server doesn't send Content-Type) */
      Size = MIN (Size, 256);
      for (i = 0; i < Size; i++)
         if ((guchar) p[i] > 127)
            break;
      Type = (i < 12 || i < Size) ? 0 : 2;                      
   }

   return (Types[Type]);
}

/*
 * Escape characters as %XX sequences.
 * Return value: New string, or NULL if there's no need to escape.
 * (this is a copy of a_Misc_escape_chars())
 */
static gchar *File_uri_escape(const gchar *str, gchar *esc_set)
{
   static const char *hex = "0123456789ABCDEF";
   gchar *p = NULL;
   GString *gstr;
   gint i;

   for (i = 0; str[i]; ++i)
      if (str[i] <= 0x1F || str[i] == 0x7F || strchr(esc_set, str[i]))
         break;

   if (str[i]) {
      /* needs escaping */
      gstr = g_string_sized_new(64);
      for (i = 0; str[i]; ++i) {
         if (str[i] <= 0x1F || str[i] == 0x7F || strchr(esc_set, str[i])) {
            g_string_append_c(gstr, '%');
            g_string_append_c(gstr, hex[(str[i] >> 4) & 15]);
            g_string_append_c(gstr, hex[str[i] & 15]);
         } else {
            g_string_append_c(gstr, str[i]);
         }
      }
      p = gstr->str;
      g_string_free(gstr, FALSE);
   }
   return p;
}

/*
 * Escape unsafe characters as html entities.
 * Return value: NULL if there's no need to escape, New string otherwise.
 */
static gchar *File_html_escape(const gchar *str)
{
   static const char *unsafe_chars = "&<>\"'";
   static const char *unsafe_rep[] =
     { "&amp;", "&lt;", "&gt;", "&quot;", "&#39;" };

   gchar *p;
   GString *gstr;
   gint i;

   if ((p = strpbrk(str, unsafe_chars))) {
      gstr = g_string_sized_new(64);

      for (i = 0; str[i]; ++i) {
         if ((p = strchr(unsafe_chars, str[i])))
            g_string_append(gstr, unsafe_rep[p - unsafe_chars]);
         else
            g_string_append_c(gstr, str[i]);
      }
      p = gstr->str;
      g_string_free(gstr, FALSE);
   }
   return p;
}

/*
 * Compare two FileInfo pointers
 * This function is used for sorting directories
 */
static gint File_comp(gconstpointer a, gconstpointer b)
{
   FileInfo *f1 = (FileInfo *) a;
   FileInfo *f2 = (FileInfo *) b;

   if (S_ISDIR(f1->mode)) {
      if (S_ISDIR(f2->mode)) {
         return strcmp(f1->filename, f2->filename);
      } else {
         return -1;
      }
   } else {
      if (S_ISDIR(f2->mode)) {
         return 1;
      } else {
         return strcmp(f1->filename, f2->filename);
      }
   }
}

/*
 * Allocate a DilloDir structure, set safe values in it and sort the entries.
 */
static DilloDir *File_dillodir_new(char *dirname)
{
   struct stat sb;
   struct dirent *de;
   DIR *dir;
   DilloDir *Ddir;
   FileInfo *finfo;
   char *fname;
   int dirname_len;

   if ( !(dir = opendir(dirname)))
      return NULL;

   Ddir = g_new(DilloDir, 1);
   Ddir->dirname = g_strdup(dirname);
   Ddir->flist = NULL;

   dirname_len = strlen(Ddir->dirname);

   /* Scan every name and sort them */
   while ((de = readdir(dir)) != 0) {
      if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
         continue;              /* skip "." and ".." */

      if (HIDE_DOTFILES) {
         /* Don't add hidden files or backup files to the list */
         if (de->d_name[0] == '.' ||
             de->d_name[0] == '#' ||
             (de->d_name[0] != '\0' &&
              de->d_name[strlen(de->d_name) - 1] == '~'))
            continue;
      }

      fname = g_strdup_printf("%s/%s", Ddir->dirname, de->d_name);

      if (stat(fname, &sb) == -1) {
         g_free(fname);
         continue;              /* ignore files we can't stat */
      }

      finfo = g_new(FileInfo, 1);
      finfo->full_path = fname;
      finfo->filename = fname + dirname_len + 1;
      finfo->size = sb.st_size;
      finfo->mode = sb.st_mode;
      finfo->mtime = sb.st_mtime;

      Ddir->flist = g_list_prepend(Ddir->flist, finfo);
   }

   closedir(dir);

   /* sort the entries */
   Ddir->flist = g_list_sort(Ddir->flist, File_comp);

   return Ddir;
}

/*
 * Deallocate a DilloDir structure.
 */
static void File_dillodir_free(DilloDir *Ddir)
{
   GList *list;

   for (list = Ddir->flist; list; list = g_list_next(list)) {
      FileInfo *finfo = list->data;
      g_free(finfo->full_path);
      g_free(finfo);
   }

   g_list_free(Ddir->flist);
   g_free(Ddir->dirname);
   g_free(Ddir);
}

/*
 * Output the string for parent directory
 */
static void File_print_parent_dir(ClientInfo *Client, const char *dirname)
{
   if (strcmp(dirname, "/") != 0) {        /* Not the root dir */
      char *parent = g_strdup(dirname);
      char *p, *HCparent, *Cparent, *s1, *s2;

      /* cut trailing '/' */
      parent[strlen(parent) - 1] = '\0';
      /* make 'parent' have the parent dir path */
      if ( (p = strrchr(parent, '/')) )
         *(p + 1) = '\0';

      Cparent = (s1 = File_uri_escape(parent, "%#:' ")) ? s1: parent;
      HCparent = (s2 = File_html_escape(Cparent)) ? s2 : Cparent;
      sock_handler_printf(Client->sh, 0,
         "<a href='file:%s'>Parent directory</a>", HCparent);
      g_free(s2);
      g_free(s1);
      g_free(parent);
   }
}

/*
 * Given a timestamp, output an HTML-formatted date string.
 */
static void File_print_mtime(ClientInfo *Client, time_t mtime)
{
   char *ds = ctime(&mtime);

   /* Month, day and {hour or year} */
   if (Client->old_style) {
      sock_handler_printf(Client->sh, 0, " %.3s %.2s", ds + 4, ds + 8);
      if (time(NULL) - mtime > 15811200) {
         sock_handler_printf(Client->sh, 0, "  %.4s", ds + 20);
      } else {
         sock_handler_printf(Client->sh, 0, " %.5s", ds + 11);
      }
   } else {
      sock_handler_printf(Client->sh, 0,
         "<td>%.3s&nbsp;%.2s&nbsp;%.5s", ds + 4, ds + 8,
         // (more than 6 months old) ? year : hour;
         (time(NULL) - mtime > 15811200) ? ds + 20 : ds + 11);
   }
}

/*
 * Return a HTML-line from file info.
 */
static void File_info2html(ClientInfo *Client,
                           FileInfo *finfo, const char *dirname, gint n)
{
   gint size;
   char *sizeunits;
   char namebuf[MAXNAMESIZE + 1];
   const char *ref, *Cref, *HCref, *Hname;
   const gchar *cont, *filecont, *name = finfo->filename;
   gchar *s1, *s2, *s3;

   if (finfo->size <= 9999) {
      size = finfo->size;
      sizeunits = "bytes";
   } else if (finfo->size / 1024 <= 9999) {
      size = finfo->size / 1024 + (finfo->size % 1024 >= 1024 / 2);
      sizeunits = "Kb";
   } else {
      size = finfo->size / 1048576 + (finfo->size % 1048576 >= 1048576 / 2);
      sizeunits = "Mb";
   }

   /* we could note if it's a symlink... */
   if S_ISDIR (finfo->mode) {
      cont = "application/directory";
      filecont = "Directory";
   } else if (finfo->mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
      cont = "application/executable";
      filecont = "Executable";
   } else {
      filecont = cont = File_content_type(finfo->full_path);
      if (!filecont || !strcmp(filecont, "application/octet-stream"))
         filecont = "unknown";
   }

   ref = name;

   if (strlen(name) > MAXNAMESIZE) {
      memcpy(namebuf, name, MAXNAMESIZE - 3);
      strcpy(namebuf + (MAXNAMESIZE - 3), "...");
      name = namebuf;
   }

   /* escape problematic filenames */
   Cref = (s1 = File_uri_escape(ref, "%#:' ")) ? s1 : ref;
   HCref = (s2 = File_html_escape(Cref)) ? s2 : Cref;
   Hname = (s3 = File_html_escape(name)) ? s3 : name;

   if (Client->old_style) {
      char *dots = ".. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..";
      gint ndots = MAXNAMESIZE - strlen(name);
      sock_handler_printf(Client->sh, 0,
         "%s<a href='%s'>%s</a>"
         " %s"
         " %-11s%4d %-5s",
         S_ISDIR (finfo->mode) ? ">" : " ", HCref, Hname,
         dots + 50 - (ndots > 0 ? ndots : 0),
         filecont, size, sizeunits);

   } else {
      sock_handler_printf(Client->sh, 0,
         "<tr align=center %s><td>%s<td align=left><a href='%s'>%s</a>"
         "<td>%s<td>%d&nbsp;%s",
         (n & 1) ? "bgcolor=#dcdcdc" : "",
         S_ISDIR (finfo->mode) ? ">" : " ", HCref, Hname,
         filecont, size, sizeunits);
   }
   File_print_mtime(Client, finfo->mtime);
   sock_handler_printf(Client->sh, 0, "\n");

   g_free(s3);
   g_free(s2);
   g_free(s1);
}

/*
 * Read a local directory and translate it to html.
 */
static void File_transfer_dir(ClientInfo *Client,
                              DilloDir *Ddir, const char *orig_url)
{
   gint n;
   GList *list;
   char *s1, *s2, *s3, *Hdirname, *Cdirname, *HCdirname;

   /* Send DPI header */
   sock_handler_printf(Client->sh, 1,
      "<dpi cmd='start_send_page' url='%s'>\n", orig_url);

   /* Send page title */
   Cdirname =
      (s1 = File_uri_escape(Ddir->dirname, "%#:' ")) ? s1 : Ddir->dirname;
   HCdirname = (s2 = File_html_escape(Cdirname)) ? s2 : Cdirname;
   Hdirname = (s3 = File_html_escape(Ddir->dirname)) ? s3 : Ddir->dirname;

   sock_handler_printf(Client->sh, 0,
      "Content-Type: text/html\n\n"
      "<HTML>\n<HEAD>\n <BASE href='file:%s'>\n"
      " <TITLE>file:%s</TITLE>\n</HEAD>\n"
      "<BODY><H1>Directory listing of %s</H1>\n",
      HCdirname, Hdirname, Hdirname);
   g_free(s3);
   g_free(s2);
   g_free(s1);

   if (Client->old_style) {
      sock_handler_printf(Client->sh, 0, "<pre>\n");
   }

   /* Output the parent directory */
   File_print_parent_dir(Client, Ddir->dirname);

   /* HTML style toggle */
   sock_handler_printf(Client->sh, 0,
      "&nbsp;&nbsp;<a href='dpi:/file/toggle'>%%</a>\n");

   if (Ddir->flist) {
      if (Client->old_style) {
         sock_handler_printf(Client->sh, 0, "\n\n");
      } else {
         sock_handler_printf(Client->sh, 0,
            "<br><br>\n"
            "<table border=0 cellpadding=1 cellspacing=0"
            " bgcolor=#E0E0E0 width=100%%>\n"
            "<tr align=center>\n"
            "<td>\n"
            "<td width=60%%><b>Filename</b>"
            "<td><b>Type</b>"
            "<td><b>Size</b>"
            "<td><b>Modified&nbsp;at</b>\n");
      }
   } else {
      sock_handler_printf(Client->sh, 0, "<br><br>Directory is empty...");
   }

   /* Output entries */
   for (n = 0, list = Ddir->flist; list; list = g_list_next(list)) {
      File_info2html(Client, list->data, Ddir->dirname, ++n);
   }

   if (Ddir->flist) {
      if (Client->old_style) {
         sock_handler_printf(Client->sh, 0, "</pre>\n");
      } else {
         sock_handler_printf(Client->sh, 0, "</table>\n");
      }
   }

   sock_handler_printf(Client->sh, 0, "</BODY></HTML>\n");
}

/*
 * Return a content type based on the extension of the filename.
 */
static const char *File_ext(const char *filename)
{
   char *e;

   if ( !(e = strrchr(filename, '.')) )
      return NULL;

   e++;

   if (!strcasecmp(e, "gif")) {
      return "image/gif";
   } else if (!strcasecmp(e, "jpg") ||
              !strcasecmp(e, "jpeg")) {
      return "image/jpeg";
   } else if (!strcasecmp(e, "png")) {
      return "image/png";
   } else if (!strcasecmp(e, "html") ||
              !strcasecmp(e, "htm") ||
              !strcasecmp(e, "shtml")) {
      return "text/html";
   } else {
      return NULL;
   }
}

/*
 * Based on the extension, return the content_type for the file.
 * (if there's no extension, analyze the data and try to figure it out)
 */
static const char *File_content_type(const char *filename)
{
   gint fd;
   const gchar *ct;

   if (!(ct = File_ext(filename))) {
      /* everything failed, let's analyze the data... */
      if ((fd = open(filename, O_RDONLY | O_NONBLOCK)) != -1) {
         gchar buf[256];
         ssize_t buf_size;
         if ((buf_size = read(fd, buf, 256)) > 12 ) {
            ct = File_get_content_type_from_data(buf, (size_t)buf_size);
         }
         File_close(fd);
      }
   }

   return ct;
}

/*
 * Try to stat the file and determine if it's readable.
 */
static void File_get(ClientInfo *Client, const char *filename, 
                     const char *orig_url)
{
   int res;
   struct stat sb;

   if (stat(filename, &sb) != 0) {
      /* stat failed, prepare a file-not-found error. */
      res = FILE_NOT_FOUND;
   } else if (S_ISDIR(sb.st_mode)) {
      /* set up for reading directory */
      res = File_get_dir(Client, filename, orig_url);
   } else {
      /* set up for reading a file */
      res = File_get_file(Client, filename, &sb, orig_url);
   }

   if (res == FILE_NOT_FOUND) {
      sock_handler_printf(Client->sh, 1,
         "<dpi cmd='send_status_message' msg='%s Not Found: %s'>",
         S_ISDIR(sb.st_mode) ? "Directory" : "File", filename);
   } else if (res == FILE_NO_ACCESS) {
      sock_handler_printf(Client->sh, 1,
         "<dpi cmd='send_status_message' msg='"
         "Failed to open the %s %s'>",
         S_ISDIR(sb.st_mode) ? "directory" : "file", filename);
   }
}

/*
 *
 */
static gint File_get_dir(ClientInfo *Client,
                         const gchar *DirName, const char *orig_url)
{
   GString *g_dirname;
   DilloDir *Ddir;

   /* Let's make sure this directory url has a trailing slash */
   g_dirname = g_string_new(DirName);
   if ( g_dirname->str[g_dirname->len - 1] != '/' )
      g_string_append(g_dirname, "/");

   /* Let's get a structure ready for transfer */
   Ddir = File_dillodir_new(g_dirname->str);
   g_string_free(g_dirname, TRUE);
   if ( Ddir ) {
      File_transfer_dir(Client, Ddir, orig_url);
      File_dillodir_free(Ddir);
      return FILE_OK;
   } else
      return FILE_NO_ACCESS;
}

/*
 * Send the MIME content/type and then send the file itself.
 */
static gint File_get_file(ClientInfo *Client,
                          const gchar *filename,
                          struct stat *sb,
                          const char *orig_url)
{
#define LBUF 16*1024

   const gchar *ct;
   char buf[LBUF];
   gint fd, st;

   if ( (fd = open(filename, O_RDONLY | O_NONBLOCK)) < 0)
      return FILE_NO_ACCESS;

   /* Content type info: as we may misdetect a lot of files,
    * every unknown type is rendered as "text/plain".
    * todo: a better approach could be to detect&reject those types we know
    * for sure we don't handle (as gzip, bzip, ELF, etc)
    */
   ct = File_content_type(filename);
   if (!ct || !strcmp(ct, "application/octet-stream"))
      ct = "text/plain";


   /* Send DPI command */
   sock_handler_printf(Client->sh, 1,
      "<dpi cmd='start_send_page' url='%s'>\n", orig_url);

   /* Send HTTP stream */
   sock_handler_printf(Client->sh, 0,
      "Content-Type: %s\n"
      "Content-length: %ld\n\n",
      ct, sb->st_size);

   /* Send raw file contents */
   do {
      if ((st = read(fd, buf, LBUF)) > 0) {
         if (sock_handler_write(Client->sh, buf, (size_t)st, 0) != 0)
            break;
      } else if (st < 0) {
         perror("[read]");
         if (errno == EINTR || errno == EAGAIN)
            continue;
      }
   } while (st > 0);

   /* todo: It may be better to send an error report to dillo instead of
    * calling abort from g_error() */
   if (st == -1)
      g_error("ERROR while reading from file \"%s\", error was \"%s\"\n",
              filename, strerror(errno));

   File_close(fd);
   return FILE_OK;
}

/*
 * Given an hex octet (e3, 2F, 20), return the corresponding
 * character if the octet is valid, and -1 otherwise
 */
static int File_parse_hex_octet(const gchar *s)
{
   gint hex_value;
   gchar *tail, hex[3];

   if ( (hex[0] = s[0]) && (hex[1] = s[1]) ) {
      hex[2] = 0;
      hex_value = strtol(hex, &tail, 16);
      if (tail - hex == 2)
        return hex_value;
   }

   return -1;
}

static void File_parse_hex_octets(char **str)
{
   gchar *dest, *tmp, *orig = *str;
   int i, val;

   if (strchr(orig, '%')) {
      dest = tmp = g_new(gchar, strlen(orig) + 1);

      for (i = 0; orig[i]; i++) {
         if (orig[i] == '%' &&
             (val = File_parse_hex_octet(orig + i + 1)) >= 0) {
            *dest++ = val;
            i += 2;
         } else {
            *dest++ = orig[i];
         }
      }
      *dest = '\0';

      g_free(orig);
      *str = tmp;
   }
}

/*
 * Make a file URL into a human (and machine) readable path.
 * The idea is to always have a path that starts with only one slash.
 * Embedded slashes are ignored.
 */
static char *File_normalize_path(const char *orig)
{
   char *str = (char *) orig, *basename = NULL, *ret, *p;

   /* Make sure the string starts with file: (it should, but...) */
   if (strncmp(str, "file:", 5) != 0)
      return NULL;

   str += 5;

   /* Skip slashes */
   while (str[0] == '/' && str[1] != '\0' && str[1] == '/')
      str++;

   if (*str == '\0') {
      /* Give CWD if the string is only "file:" */
      basename = g_get_current_dir();
   } else if (*str == '~') {
      if (str[1] == '\0' || str[1] == '/') {
         /* Expand 'tilde' to user's home */
         basename = g_strdup(g_get_home_dir());
         str++;
      }
   } else if (*str == '.') {
      if (str[1] == '\0' || str[1] == '/') {
         /* User wants the CWD */
         basename = g_get_current_dir();
         str++; 
      } else if (str[1] == '.') {
         /* One level down from the CWD */
         char *tmp1 = g_get_current_dir();
         char *tmp2 = strrchr(tmp1, '/');

         if (tmp2) {
            basename = g_strndup(tmp1, (guint)(tmp2 - tmp1));
         }
         str += 2;
         g_free(tmp1);
      }
   } else if (*str != '/') {
      return NULL;
   }

   ret = g_strdup_printf("%s%s%s",
                         basename ? basename : "",
                         basename ? "/" : "",
                         str);
   g_free(basename);

   /* remove the fragment if present */
   if ((p = strrchr(ret, '#')) != NULL)
      *p = 0;
   /* Parse possible hexadecimal octets in the URI path */
   File_parse_hex_octets(&ret);

   return ret;
}

/*
 * Set the style flag and ask for a reload, so it shows inmediatly.
 */
static void File_toggle_html_style(ClientInfo *Client)
{
   OLD_STYLE = !OLD_STYLE;
   sock_handler_printf(Client->sh, 0, "<dpi cmd='reload_request'>");
}

/*
 * Perform any necessary cleanups upon abnormal termination
 */
static void termination_handler(int signum)
{
  exit(signum);
}


/* Client handling ----------------------------------------------------------*/

/*
 * Add a new client to the list.
 */
static ClientInfo *File_add_client(gint sock_fd)
{
   ClientInfo *NewClient;

   NewClient = g_new(ClientInfo, 1);
   NewClient->sh = sock_handler_new(sock_fd, sock_fd, 8*1024);
   NewClient->status = 0;
   NewClient->done = 0;
   NewClient->old_style = OLD_STYLE;
  pthread_mutex_lock(&ClMut);
   Clients = g_list_append(Clients, NewClient);
  pthread_mutex_unlock(&ClMut);
   return NewClient;
}

/*
 * Get client record by number
 */
static void *File_get_client_n(guint n)
{
   void *client;

  pthread_mutex_lock(&ClMut);
   client = g_list_nth_data(Clients, n);
  pthread_mutex_unlock(&ClMut);

   return client;
}

/*
 * Remove a client from the list.
 */
static void File_remove_client_n(guint n)
{
   ClientInfo *Client;

  pthread_mutex_lock(&ClMut);
   Client = g_list_nth_data(Clients, n);
   Clients = g_list_remove(Clients, (void *)Client);
  pthread_mutex_unlock(&ClMut);

   _MSG("Closing Socket Handler\n");
   sock_handler_close(Client->sh);
   sock_handler_free(Client->sh);
   g_free(Client);
}

/*
 * Return the number of clients.
 */
static gint File_num_clients(void)
{
   guint n;

  pthread_mutex_lock(&ClMut);
   n = g_list_length(Clients);
  pthread_mutex_unlock(&ClMut);

   return n;
}

/*
 * Serve this client.
 * (this function runs on its own thread)
 */
static void *File_serve_client(void *data)
{
   char *dpip_tag, *cmd = NULL, *url = NULL, *path;
   ClientInfo *Client = data;

   /* Read the dpi command */
   dpip_tag = sock_handler_read(Client->sh);
   MSG("dpip_tag = '%s'\n", dpip_tag);

   if (dpip_tag) {
      cmd = Get_attr_value(dpip_tag, strlen(dpip_tag), "cmd");
      if (cmd) {
         if (strcmp(cmd, "DpiBye") == 0) {
            DPIBYE = 1;
         } else {
            url = Get_attr_value(dpip_tag, strlen(dpip_tag), "url");
            if (!url)
               MSG("file.dpi:: Failed to parse 'url'\n");
         }
      }
   }
   g_free(cmd);
   g_free(dpip_tag);

   if (!DPIBYE && url) {
      _MSG("url = '%s'\n", url);

      path = File_normalize_path(url);
      if (path) {
         _MSG("path = '%s'\n", path);
         File_get(Client, path, url);
      } else if (strcmp(url, "dpi:/file/toggle") == 0) {
         File_toggle_html_style(Client);
      } else {
         MSG("ERROR: URL path was %s\n", url);
      }
      g_free(path);
   }
   g_free(url);

   /* flag the the transfer finished */
   Client->done = 1;

   return NULL;
}

/*
 * Serve the client queue.
 * (this function runs on its own thread)
 */
static void *File_serve_clients(void *client)
{
   /* switch to detached state */
   pthread_detach(pthread_self());

   while (File_num_clients()) {
      client = File_get_client_n((guint)0);
      File_serve_client(client);
      File_remove_client_n((guint)0);
   }
   ThreadRunning = 0;

   return NULL;
}

/* --------------------------------------------------------------------------*/

/*
 * Check a fd for activity, with a max timeout.
 * return value: 0 if timeout, 1 if input available, -1 if error.
 */
int File_check_fd(int filedes, unsigned int seconds)
{
  int st;
  fd_set set;
  struct timeval timeout;

  /* Initialize the file descriptor set. */
  FD_ZERO (&set);
  FD_SET (filedes, &set);

  /* Initialize the timeout data structure. */
  timeout.tv_sec = seconds;
  timeout.tv_usec = 0;

  do {
     st = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
  } while (st == -1 && errno == EINTR);

  return st;
}


int main(void)
{
   ClientInfo *NewClient;
   struct sockaddr_un spun;
   gint temp_sock_descriptor;
   gint address_size, c_st, st = 1;
   guint i;

   /* Arrange the cleanup function for abnormal terminations */
   if (signal (SIGINT, termination_handler) == SIG_IGN)
     signal (SIGINT, SIG_IGN);  
   if (signal (SIGHUP, termination_handler) == SIG_IGN)
     signal (SIGHUP, SIG_IGN);  
   if (signal (SIGTERM, termination_handler) == SIG_IGN)
     signal (SIGTERM, SIG_IGN); 

   MSG("(v.1) accepting connections...\n");

   /* initialize mutex */
   pthread_mutex_init(&ClMut, NULL);

   /* some OSes may need this... */
   address_size = sizeof(struct sockaddr_un);

   /* start the service loop */
   while (!DPIBYE) {
      /* wait for a connection */
      do {
         c_st = File_check_fd(STDIN_FILENO, 1);
      } while (c_st == 0 && !DPIBYE);
      if (c_st < 0) {
         perror("[select]");
         break;
      }
      if (DPIBYE)
         break;

      temp_sock_descriptor =
         accept(STDIN_FILENO, (struct sockaddr *)&spun, &address_size);

      if (temp_sock_descriptor == -1) {
         perror("[accept]");    
         break;
      }

      /* Create and initialize a new client */
      NewClient = File_add_client(temp_sock_descriptor);

      if (!ThreadRunning) {
         ThreadRunning = 1;
         /* Serve the client from a thread (avoids deadlocks) */
         if (pthread_create(&NewClient->thrID, NULL,
                            File_serve_clients, NewClient) != 0) {
            perror("[pthread_create]");
            ThreadRunning = 0;
            break;
         }
      }
   }

   /* todo: handle a running thread better. */
   for (i = 0; i < 5 && ThreadRunning; ++i) {
      MSG("sleep i=%u", i);
      sleep(i);
   }

   if (DPIBYE)
      st = 0;
   return st;
}

