/*
 * Bookmarks server (chat version).
 *
 * NOTE: this code illustrates how to make a dpi-program.
 *
 * Copyright 2002-2004 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

/* Todo: this server is not assembling the received packets.
 * This means it currently expects dillo to send full dpi tags
 * within the socket; if that fails, everything stops.
 * This is not hard to fix, mainly is a matter of expecting the
 * final '>' of a tag.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include "dpiutil.h"

#include <glib.h>

/* This one os tricky, some sources state it should include the byte
 * for the terminating NULL, and others say it shouldn't. */
# define D_SUN_LEN(ptr) ((size_t) (((struct sockaddr_un *) 0)->sun_path) \
                        + strlen ((ptr)->sun_path))

typedef struct {
   gint key;
   gint section;
   gchar *url;
   gchar *title;
} BmRec;

typedef struct {
   gint section;
   gchar *title;

   gint o_sec;   /* private, for normalization */
} BmSec;


/*
 * Local data
 */
static char *Header = "Content-type: text/html\n\n";
static char *BmFile = NULL;
static time_t BmFileTimeStamp = 0;
static GSList *B_bms = NULL;
static gint bm_key = 0;

static GSList *B_secs = NULL;
static gint sec_key = 0;

static int MODIFY_PAGE_NUM = 1;


/*
 * Forward declarations
 */


/* -- HTML templates ------------------------------------------------------- */

char *mainpage_header =
"<html>\n"
"<head>\n"
"<title>Bookmarks</title>\n"
"</head>\n"
"<body bgcolor='#778899' link='black' vlink='brown'>\n"
"<table border='1' cellpadding='0' width='100%'>\n"
" <tr><td>\n"
"  <table width='100%' bgcolor='#b4b4b4'>\n"
"   <tr>\n"
"    <td>&nbsp;Bookmarks::</td>\n"
"    <td width='100%' align='right'>\n"
"     [<a href='dpi:/bm/modify'>modify</a>]\n"
"    </td></tr>\n"
"  </table></td></tr>\n"
"</table>\n"
"<br>\n";

char *modifypage_header =
"<html>\n"
"<head>\n"
"<title>Bookmarks</title>\n"
"</head>\n"
"<body bgcolor='#778899' link='black' vlink='brown'>\n"
"<table border='1' cellpadding='0' width='100%'>\n"
" <tr><td>\n"
"  <table width='100%' bgcolor='#b4b4b4'>\n"
"   <tr>\n"
"    <td>&nbsp;Bookmarks :: modify</td></tr>\n"
"  </table></td></tr>                            \n"
"</table>                                        \n"
"\n"
"<form>\n"
"<table width='100%' border='1' cellpadding='0'>\n"
" <tr><td>\n"
"  <table width='100%' bgcolor='teal'>\n"
"   <tr>\n"
"    <td><b>Select&nbsp;an&nbsp;operation&nbsp;</b></td>\n"
"    <td><select name='operation'>\n"
"     <option value='none' selected>--\n"
"     <option value='delete'>Delete\n"
"     <option value='move'>Move\n"
"     <option value='modify'>Modify\n"
"     <option value='add_sec'>Add Section\n"
"     <option value='add_url'>Add URL\n"
"     </select></td>\n"
"    <td><b>,&nbsp;mark&nbsp;its&nbsp;operands,&nbsp;and&nbsp;</b></td>\n"
"    <td><input type='submit' name='submit' value='submit.'></td>\n"
"    <td width='100%'></td>\n"
"    </tr>\n"
"  </table></td></tr>\n"
"</table>\n";

char *mainpage_sections_header =
"<table border='1' cellpadding='0' cellspacing='20' width='100%'>\n"
" <tr valign='top'>\n"
"  <td>\n"
"   <table bgcolor='#b4b4b4' border='2' cellpadding='4' cellspacing='1'>\n"
"    <tr><td>\n"
"     <table width='100%' bgcolor='#b4b4b4'>\n"
"      <tr><td><small>Sections:</small></td></tr></table></td></tr>\n";

char *modifypage_sections_header =
"<table border='1' cellpadding='0' cellspacing='20' width='100%'>\n"
" <tr valign='top'>\n"
"  <td>\n"
"   <table bgcolor='#b4b4b4' border='1'>\n"
"    <tr><td>\n"
"     <table width='100%' bgcolor='#b4b4b4'>\n"
"      <tr><td><small>Sections:</small></td></tr></table></td></tr>\n";

char *mainpage_sections_item =
"    <tr><td align='center'>\n"
"      <a href='#s%d'>%s</a></td></tr>\n";

char *modifypage_sections_item =
"    <tr><td>\n"
"     <table width='100%%' bgcolor='#b4b4b4'cellspacing='0' cellpadding='0'>\n"
"      <tr align='center'>"
"       <td width='1%%'><input type='checkbox' name='s%d'></td>\n"
"       <td><a href='#s%d'>%s</a></td></tr></table></td></tr>\n";

char *mainpage_sections_footer =
"   </table>\n";

char *modifypage_sections_footer =
"   </table>\n";

char *mainpage_middle1 =
"  </td>\n"
"  <td width='100%'>\n";

char *modifypage_middle1 =
"  </td>\n"
"  <td width='100%'>\n";

char *mainpage_section_card_header =
"   <a name='s%d'></a>\n"
"   <table bgcolor='#bfbfbf' width='100%%' cellspacing='2'>\n"
"    <tr>\n"
"     <td bgcolor='#bf0c0c'><font color='white'><b>\n"
"      &nbsp;&nbsp;&nbsp;%s&nbsp;&nbsp;&nbsp;</b></font></td>\n"
"     <td bgcolor='white' width='100%%'>&nbsp;</td></tr>\n";

char *modifypage_section_card_header =
"   <a name='s%d'></a>\n"
"   <table bgcolor='#bfbfbf' width='100%%' cellspacing='2'>\n"
"    <tr>\n"
"     <td bgcolor='#bf0c0c'><font color='white'><b>\n"
"      &nbsp;&nbsp;&nbsp;%s&nbsp;&nbsp;&nbsp;</b></font></td>\n"
"     <td bgcolor='white' width='100%%'>&nbsp;</td></tr>\n";

char *mainpage_section_card_item =
"    <tr><td colspan='2'>\n"
"      <a href='%s'>%s</a> </td></tr>\n";

char *modifypage_section_card_item =
"    <tr>\n"
"     <td colspan='2'><input type='checkbox' name='url%d'>\n"
"      <a href='%s'>%s</a></td></tr>\n";

char *mainpage_section_card_footer =
"   </table>\n"
"   <hr>\n";

char *modifypage_section_card_footer =
"   </table>\n"
"   <hr>\n";

char *mainpage_footer =
"  </td>\n"
" </tr>\n"
"</table>\n"
"</body>\n"
"</html>\n";

char *modifypage_footer =
"  </td>\n"
" </tr>\n"
"</table>\n"
"</form>\n"
"</body>\n"
"</html>\n";

/* ------------------------------------------------------------------------- */
char *modifypage_add_section_page =
"<html>\n"
"<head>\n"
"<title>Bookmarks</title>\n"
"</head>\n"
"<body bgcolor='#778899' link='black' vlink='brown'>\n"
"<table border='1' cellpadding='0' width='100%'>\n"
" <tr><td colspan='2'>\n"
"  <table bgcolor='#b4b4b4' width='100%'>\n"
"   <tr><td bgcolor='#b4b4b4'>&nbsp;Modify bookmarks:: add section\n"
"   </td></tr></table></td></tr>\n"
"</table>\n"
"<br>\n"
"<form>\n"
" <input type='hidden' name='operation' value='add_section'>\n"
"<table border='1' width='100%'>\n"
" <tr>\n"
"  <td bgcolor='olive'><b>New&nbsp;section:</b></td>\n"
"  <td bgcolor='white' width='100%'></td></tr>\n"
"</table>\n"
"<table width='100%' cellpadding='10'>\n"
"<tr><td>\n"
" <table width='100%' bgcolor='teal'>\n"
"  <tr>\n"
"   <td>Title:</td>\n"
"   <td><input type='text' name='title' size='64'></td></tr>\n"
" </table>\n"
" </td></tr>\n"
"</table>\n"
"<table width='100%' cellpadding='4' border='0'>\n"
"<tr><td bgcolor='#a0a0a0'>\n"
" <input type='submit' name='submit' value='submit.'></td></tr>\n"
"</table>\n"
"</form>\n"
"</body>\n"
"</html>\n"
"\n";

/* ------------------------------------------------------------------------- */
char *modifypage_update_header =
"<html>\n"
"<head>\n"
"<title>Bookmarks</title>\n"
"</head>\n"
"<body bgcolor='#778899' link='black' vlink='brown'>\n"
"<table border='1' cellpadding='0' width='100%'>\n"
" <tr><td colspan='2'>\n"
"  <table bgcolor='#b4b4b4' width='100%'>\n"
"   <tr><td bgcolor='#b4b4b4'>&nbsp;Modify bookmarks:: update\n"
"   </td></tr></table></td></tr>\n"
"</table>\n"
"<br>\n"
"<form>\n"
"<input type='hidden' name='operation' value='modify2'>\n";

char *modifypage_update_title =
"<table border='1' width='100%%'>\n"
" <tr>\n"
"  <td bgcolor='olive'><b>%s</b></td>\n"
"  <td bgcolor='white' width='100%%'></td></tr>\n"
"</table>\n";

char *modifypage_update_item_header =
"<table width='100%' cellpadding='10'>\n";

char *modifypage_update_item =
"<tr><td>\n"
" <table width='100%%' bgcolor='teal'>\n"
"  <tr>\n"
"   <td>Title:</td>\n"
"   <td><input type='text' name='title%d' size='64'\n"
"        value='%s'></td></tr>\n"
"  <tr>\n"
"   <td>URL:</td>\n"
"   <td>%s</td></tr>\n"
" </table>\n"
" </td></tr>\n";

char *modifypage_update_item2 =
"<tr><td>\n"
" <table width='100%%' bgcolor='teal'>\n"
"  <tr>\n"
"   <td>Title:</td>\n"
"   <td><input type='text' name='s%d' size='64'\n"
"        value='%s'></td></tr>\n"
" </table>\n"
" </td></tr>\n";

char *modifypage_update_item_footer =
"</table>\n";

char *modifypage_update_footer =
"<table width='100%' cellpadding='4' border='0'>\n"
"<tr><td bgcolor='#a0a0a0'>\n"
" <input type='submit' name='submit' value='submit.'></td></tr>\n"
"</table>\n"
"</form>\n"
"</body>\n"
"</html>\n";

/* ------------------------------------------------------------------------- */
char *modifypage_add_url =
"<html>\n"
"<head>\n"
"<title>Bookmarks</title>\n"
"</head>\n"
"<body bgcolor='#778899' link='black' vlink='brown'>\n"
"<table border='1' cellpadding='0' width='100%'>\n"
" <tr><td colspan='2'>\n"
"  <table bgcolor='#b4b4b4' width='100%'>\n"
"   <tr><td bgcolor='#b4b4b4'>&nbsp;Modify bookmarks:: add url\n"
"   </td></tr></table></td></tr>\n"
"</table>\n"
"<br>\n"
"<form>\n"
"<input type='hidden' name='operation' value='add_url2'>\n"
"<table border='1' width='100%'>\n"
" <tr>\n"
"  <td bgcolor='olive'><b>Add&nbsp;url:</b></td>\n"
"  <td bgcolor='white' width='100%'></td></tr>\n"
"</table>\n"
"<table width='100%' cellpadding='10'>\n"
"<tr><td>\n"
" <table width='100%' bgcolor='teal'>\n"
"  <tr>\n"
"   <td>Title:</td>\n"
"   <td><input type='text' name='title' size='64'></td></tr>\n"
"  <tr>\n"
"   <td>URL:</td>\n"
"   <td><input type='text' name='url' size='64'></td></tr>\n"
" </table>\n"
" </td></tr>\n"
"</table>\n"
"<table width='100%' cellpadding='4' border='0'>\n"
"<tr><td bgcolor='#a0a0a0'>\n"
" <input type='submit' name='submit' value='submit.'></td></tr>\n"
"</table>\n"
"</form>\n"
"</body>\n"
"</html>\n";


/* ------------------------------------------------------------------------- */

/*
 * Return a new string with spaces changed with &nbsp;
 */
static char *make_one_line_str(char *str)
{
   char *new_str;
   int i, j, n;

   for (i = 0, n = 0; str[i]; ++i)
      if (str[i] == ' ')
         ++n;

   new_str = g_new(char, strlen(str) + 6*n + 1);
   new_str[0] = 0;

   for (i = 0, j = 0; str[i]; ++i) {
      if (str[i] == ' ') {
         strcpy(new_str + j, "&nbsp;");
         j += 6;
      } else {
         new_str[j] = str[i];
         new_str[++j] = 0;
      }
   }

   return new_str;
}

/*
 * Use character stuffing for characters given in 'esc_set'.
 * Return value: New stuffed string.
 */
static gchar *Stuff_chars(const gchar *str, const gchar *esc_set)
{
   gint i;
   gchar *p, *sstr = NULL;

   if ((p = strpbrk(str, esc_set))) {
      /* start stuffing */
      GString *gstr = g_string_new(str);
      for (i = p - str; gstr->str[i]; ++i)
         if (strchr(esc_set, gstr->str[i])) {
            g_string_insert_c(gstr, i, gstr->str[i]);
            ++i;
         }
      sstr = gstr->str;
      g_string_free(gstr, FALSE);
   }
   return (sstr ? sstr : g_strdup(str));
}

/*
 * Given an urlencoded string, return it to the original version.
 */
static void Unencode_str(char *e_str)
{
   char *p, *e;

   for (p = e = e_str; *e; e++, p++) {
      if (*e == '+') {
         *p = ' ';
      } else if (*e == '%') {
         if (g_strncasecmp(e, "%0D%0A", 6) == 0) {
            *p = '\n';
            e += 5;
         } else {
            *p = (isdigit(e[1]) ? (e[1] - '0') : (e[1] - 'A' + 10)) * 16 +
                 (isdigit(e[2]) ? (e[2] - '0') : (e[2] - 'A' + 10));
            e += 2;
         }
      } else {
         *p = *e;
      }
   }
   *p = 0;
}

/*
 * Read a line of text up to the newline character, store it into a newly
 * allocated string and return it.
 */
static char *Get_line(FILE *stream)
{
   guint i, size = 64;
   int ch;
   char *buf;

   buf = g_new(char, size);

   for (i = 0; (ch = fgetc(stream)) != EOF; ++i) {
      if (i + 1 == size) {
         size *= 2;
         buf = g_realloc(buf, size);
      }
      if ((buf[i] = ch) == '\n' && ++i)
         break;
   }
   buf[i] = 0;

   if (i > 0) {
      buf = g_realloc(buf, i + 1);
   } else {
      g_free(buf);
      buf = NULL;
   }
   return buf;
}

/*
 * Send a short message to dillo's status bar.
 */
static int Bmsrv_dpi_send_status_msg(SockHandler *sh, char *str)
{
   int st;
   GString *gstr = g_string_new("");
   gchar *sstr = Stuff_chars(str, "'\"");

   g_string_sprintf(gstr, "<dpi cmd='send_status_message' msg='%s'>", sstr);
   st = sock_handler_write_str(sh, gstr->str, 1);
   g_string_free(gstr, TRUE);
   g_free(sstr);
   return st;
}

/* -- ADT for bookmarks ---------------------------------------------------- */
/*
 * Compare function for searching a bookmark by its key
 */
static gint Bms_key_cmp(gconstpointer node, gconstpointer key)
{
   return ( GPOINTER_TO_INT(key) != ((BmRec *)node)->key );
}

/*
 * Compare function for searching a bookmark by section
 */
static gint Bms_bysec_cmp(gconstpointer node, gconstpointer key)
{
   return ( GPOINTER_TO_INT(key) != ((BmRec *)node)->section );
}

/*
 * Compare function for searching a section by its number
 */
static gint Bms_sec_cmp(gconstpointer node, gconstpointer key)
{
   return ( GPOINTER_TO_INT(key) != ((BmSec *)node)->section );
}

/*
 * Return the Bm record by key
 */
static BmRec *Bms_get(gint key)
{
   GSList *list;

   list = g_slist_find_custom(B_bms, GINT_TO_POINTER(key),
                              Bms_key_cmp);
   return (list) ? list->data : NULL;
}

/*
 * Return the Section record by key
 */
static BmSec *Bms_get_sec(gint key)
{
   GSList *list;

   list = g_slist_find_custom(B_secs, GINT_TO_POINTER(key),
                              Bms_sec_cmp);
   return (list) ? list->data : NULL;
}

/*
 * Add a bookmark
 */
static void Bms_add(gint section, char *url, char *title)
{
   BmRec *node;

   node = g_new(BmRec, 1);
   node->key = ++bm_key;
   node->section = section;
   node->url = g_strdup(url);
   node->title = g_strdup(title);
   B_bms = g_slist_append(B_bms, node);
}

/*
 * Add a section
 */
static void Bms_sec_add(char *title)
{
   BmSec *node;

   node = g_new(BmSec, 1);
   node->section = sec_key++;
   node->title = g_strdup(title);
   B_secs = g_slist_append(B_secs, node);
}

/*
 * Delete a bookmark by its key
 */
static void Bms_del(gint key)
{
   GSList *list;
   BmRec *node;

   list = g_slist_find_custom(B_bms, GINT_TO_POINTER(key),
                              Bms_key_cmp);
   if (list) {
      node = list->data;
      g_free(node->title);
      g_free(node->url);
      g_free(list->data);
      B_bms = g_slist_remove(B_bms, list->data);
   }
   if (B_bms == NULL)
      bm_key = 0;
}

/*
 * Delete a section and its bookmarks by section number
 */
static void Bms_sec_del(gint section)
{
   GSList *list;
   BmSec *sec_node;
   BmRec *bm_node;

   list = g_slist_find_custom(B_secs, GINT_TO_POINTER(section),
                              Bms_sec_cmp);
   if (list) {
      sec_node = list->data;
      g_free(sec_node->title);
      g_free(list->data);
      B_secs = g_slist_remove(B_secs, list->data);

      /* iterate B_bms and remove those that match the section */
      while ((list = g_slist_find_custom(B_bms, GINT_TO_POINTER(section),
                                         Bms_bysec_cmp))) {
         bm_node = list->data;
         Bms_del(bm_node->key);
      }
   }
   if (B_secs == NULL)
      sec_key = 0;
}

/*
 * Move a bookmark to another section
 */
static void Bms_move(gint key, gint target_section)
{
   GSList *list;

   list = g_slist_find_custom(B_bms, GINT_TO_POINTER(key), Bms_key_cmp);
   if (list) {
      BmRec *node = list->data;
      node->section = target_section;
   }
}

/*
 * Update a bookmark title by key
 */
static void Bms_update_title(gint key, gchar *n_title)
{
   GSList *list;

   list = g_slist_find_custom(B_bms, GINT_TO_POINTER(key), Bms_key_cmp);
   if (list) {
      BmRec *node = list->data;
      if (strcmp(node->title, n_title)) {
         g_free(node->title);
         node->title = g_strdup(n_title);
      }
   }
}

/*
 * Update a section title by key
 */
static void Bms_update_sec_title(gint key, gchar *n_title)
{
   GSList *list;

   list = g_slist_find_custom(B_secs, GINT_TO_POINTER(key),
                              Bms_sec_cmp);
   if (list) {
      BmSec *node = list->data;
      if (strcmp(node->title, n_title)) {
         g_free(node->title);
         node->title = g_strdup(n_title);
      }
   }
}

/*
 * Free all the bookmarks data (bookmarks and sections)
 */
static void Bms_free(void)
{
   BmRec *bm_node;
   BmSec *sec_node;

   /* free B_bms */
   while (B_bms) {
      bm_node = B_bms->data;
      Bms_del(bm_node->key);
   }
   /* free B_secs */
   while (B_secs) {
      sec_node = B_secs->data;
      Bms_sec_del(sec_node->section);
   }
}

/*
 * Enforce increasing correlative section numbers with no jumps.
 */
static void Bms_normalize(void)
{
   BmRec *bm_node;
   BmSec *sec_node;
   GSList *list1, *list2;
   gint n;

   /* we need at least one section */
   if (!B_secs)
      Bms_sec_add("Unclassified");

   /* make correlative section numbers */
   n = 0;
   for (list1 = B_secs; list1; list1 = list1->next) {
      sec_node = list1->data;
      sec_node->o_sec = sec_node->section;
      sec_node->section = n++;
   }

   /* iterate B_secs and make the changes in B_bms */
   for (list1 = B_secs; list1; list1 = list1->next) {
      sec_node = list1->data;
      if (sec_node->section != sec_node->o_sec) {
         /* update section numbers */
         for (list2 = B_bms; list2; list2 = list2->next) {
            bm_node = list2->data;
            if (bm_node->section == sec_node->o_sec)
               bm_node->section = sec_node->section;
         }
      }
   }
}

/* -- Load bookmarks file -------------------------------------------------- */

/*
 * If there's no "bm.txt", create one from "bookmarks.html".
 */
static void Bms_check_import(void)
{
   gchar *OldBmFile;
   char *cmd1 =
      "echo \":s0: Unclassified\" > %s";
   char *cmd2 =
      "grep -i \"href\" %s | "
      "sed -e 's/<li><A HREF=\"/s0 /' -e 's/\">/ /' -e 's/<.*$//' >> %s";
   GString *gstr = g_string_new("");


   if (access(BmFile, F_OK) != 0) {
      OldBmFile = g_strconcat(g_get_home_dir(),
                          "/", ".dillo/bookmarks.html", NULL);
      if (access(OldBmFile, F_OK) == 0) {
         g_string_sprintf(gstr, cmd1, BmFile);
         system(gstr->str);
         g_string_sprintf(gstr, cmd2, OldBmFile, BmFile);
         system(gstr->str);
         g_string_free(gstr, TRUE);
         g_free(OldBmFile);
      }
   }
}

/*
 * Load bookmarks data from a file
 */
static int Bms_load(void)
{
   FILE *BmTxt;
   char *buf, *p, *url, *title;
   int section;
   struct stat TimeStamp;

   /* clear current bookmarks */
   Bms_free();

   /* open bm file */
   if (!(BmTxt = fopen(BmFile, "r"))) {
      perror("[fopen]");
      return 1;
   }

   /* load bm file into memory */
   while ((buf = Get_line(BmTxt)) != NULL) {
      if (buf[0] == 's') {
         /* get section, url and title */
         section = strtol(buf + 1, NULL, 10);
         p = strchr(buf, ' '); *p = 0;
         url = ++p;
         p = strchr(p, ' '); *p = 0;
         title = ++p;
         p = strchr(p, '\n'); *p = 0;
         Bms_add(section, url, title);
         //g_print("Section: %d Url: %s Title: %s\n", section, url, title);

      } else if (buf[0] == ':' && buf[1] == 's') {
         // section = strtol(buf + 2, NULL, 10);
         p = strchr(buf + 2, ' ');
         title = ++p;
         p = strchr(p, '\n'); *p = 0;
         Bms_sec_add(title);

      } else {
         g_print("Syntax error in bookmarks file:\n %s", buf);
      }
      g_free(buf);
   }
   fclose(BmTxt);

   /* keep track of the timestamp */
   stat(BmFile, &TimeStamp);
   BmFileTimeStamp = TimeStamp.st_mtime;

   return 0;
}

/*
 * Load bookmarks data if:
 *   - file timestamp is newer than ours  or
 *   - we haven't loaded anything yet :)
 */
static int Bms_cond_load(void)
{
   int st = 0;
   struct stat TimeStamp;

   if (stat(BmFile, &TimeStamp) != 0) {
      /* try to import... */
      Bms_check_import();
      if (stat(BmFile, &TimeStamp) != 0)
         TimeStamp.st_mtime = 0;
   }

   if (!BmFileTimeStamp || !B_bms || !B_secs ||
       BmFileTimeStamp < TimeStamp.st_mtime) {
      Bms_load();
      st = 1;
   }
   //g_print("\n >>> %s\n\n", st ? "LOADED" : "KEPT");
   return st;
}

/* -- Save bookmarks file -------------------------------------------------- */

/*
 * Update the bookmarks file from memory contents
 * Return code: { 0:OK, 1:Abort }
 */
static int Bms_save(void)
{
   FILE *BmTxt;
   BmRec *bm_node;
   BmSec *sec_node;
   GSList *list, *list2;
   GString *gstr = g_string_new("");
   struct stat BmStat;

   /* make a safety backup */
   if (stat(BmFile, &BmStat) == 0 && BmStat.st_size > 256) {
      gchar *BmFileBak = g_strconcat(BmFile, ".bak", NULL);
      rename(BmFile, BmFileBak);
      g_free(BmFileBak);
   }

   /* open bm file */
   if (!(BmTxt = fopen(BmFile, "w"))) {
      perror("[fopen]");
      return 1;
   }

   /* normalize */
   Bms_normalize();

   /* save sections */
   for (list = B_secs; list; list = list->next) {
      sec_node = list->data;
      g_string_sprintf(gstr, ":s%d: %s\n", sec_node->section, sec_node->title);
      fwrite(gstr->str, (size_t)gstr->len, 1, BmTxt);
   }

   /* save bookmarks  (section url title) */
   for (list = B_secs; list; list = list->next) {
      sec_node = list->data;
      for (list2 = B_bms; list2; list2 = list2->next) {
         bm_node = list2->data;
         if (bm_node->section == sec_node->section) {
            g_string_sprintf(gstr, "s%d %s %s\n",
                             bm_node->section, bm_node->url, bm_node->title);
            fwrite(gstr->str, (size_t)gstr->len, 1, BmTxt);
         }
      }
   }

   g_string_free(gstr, TRUE);
   fclose(BmTxt);

   /* keep track of the timestamp */
   stat(BmFile, &BmStat);
   BmFileTimeStamp = BmStat.st_mtime;

   return 0;
}

/* -- Add bookmark --------------------------------------------------------- */

/*
 * Add a new bookmark to DB :)
 */
static int Bmsrv_add_bm(SockHandler *sh, char *url, char *title)
{
   char *msg="Added bookmark!";
   int section = 0;

   //g_print("Adding:\n  %s\n  %s\n", title, url);

   /* Add in memory */
   Bms_add(section, url, title);
   /* Write to file */
   Bms_save();

   if (Bmsrv_dpi_send_status_msg(sh, msg))
      return 1;

   return 0;
}

/* -- Modify --------------------------------------------------------------- */

/*
 * Count how many sections and urls were marked in a request
 */
static void Bmsrv_count_urls_and_sections(char *url, gint *n_sec, gint *n_url)
{
   char *p, *q;
   int i;

   /* Check marked urls and sections */
   *n_sec = *n_url = 0;
   if ((p = strchr(url, '?'))) {
      for (q = p; (q = strstr(q, "&url")); ++q) {
         for (i = 0; isdigit(q[4+i]); ++i);
         *n_url += (q[4+i] == '=') ? 1 : 0;
      }
      for (q = p; (q = strstr(q, "&s")); ++q) {
         for (i = 0; isdigit(q[2+i]); ++i);
         *n_sec += (q[2+i] == '=') ? 1 : 0;
      }
   }
}

/*
 * Send a dpi reload request
 * Return code: { 0:OK, 1:Abort, 2:Close }
 */
static int Bmsrv_send_reload_request(SockHandler *sh, char *url)
{
   gint st = 0;
   GString *gstr = g_string_new("");

   g_string_sprintf(gstr, "<dpi cmd='reload_request' url='%s'>", url);
   if (sock_handler_write_str(sh, gstr->str, 1))
      st = 1;
   g_string_free(gstr, TRUE);

   return st;
}

/*
 * Send the HTML for the modify page
 * Return code: { 0:OK, 1:Abort, 2:Close }
 */
static int Bmsrv_send_modify_page(SockHandler *sh)
{
   static GString *gstr = NULL;
   gchar *l_title;
   GSList *list1, *list2;
   BmSec *sec_node;
   BmRec *bm_node;

   //g_print("  sending page body...\n");

   if (!gstr)
      gstr = g_string_new("");

   /* send modify page header */
   if (sock_handler_write_str(sh, modifypage_header, 0))
      return 1;

   /* write sections header */
   if (sock_handler_write_str(sh, modifypage_sections_header, 0))
      return 1;
   /* write sections */
   for (list1 = B_secs; list1; list1 = list1->next) {
      sec_node = list1->data;
      g_string_sprintf(gstr, modifypage_sections_item,
                       sec_node->section, sec_node->section, sec_node->title);
      if (sock_handler_write_str(sh, gstr->str, 0))
         return 1;
   }
   /* write sections footer */
   if (sock_handler_write_str(sh, modifypage_sections_footer, 0))
      return 1;

   /* send page middle */
   if (sock_handler_write_str(sh, modifypage_middle1, 0))
      return 1;

   /* send bookmark cards */
   for (list1 = B_secs; list1; list1 = list1->next) {
      sec_node = list1->data;

      /* send card header */
      l_title = make_one_line_str(sec_node->title);
      g_string_sprintf(gstr, modifypage_section_card_header,
                       sec_node->section, l_title);
      g_free(l_title);
      if (sock_handler_write_str(sh, gstr->str, 0))
         return 1;

      /* send section's bookmarks */
      for (list2 = B_bms; list2; list2 = list2->next) {
         bm_node = list2->data;
         if (bm_node->section == sec_node->section) {
            g_string_sprintf(gstr, modifypage_section_card_item,
                             bm_node->key, bm_node->url, bm_node->title);
            if (sock_handler_write_str(sh, gstr->str, 0))
               return 1;
         }
      }

      /* send card footer */
      if (sock_handler_write_str(sh, modifypage_section_card_footer, 0))
         return 1;
   }

   /* finish page */
   if (sock_handler_write_str(sh, modifypage_footer, 1))
      return 1;

   //g_print("  page body sent.\n");
   return 2;
}

/*
 * Send the HTML for the modify page for "add section"
 * Return code: { 0:OK, 1:Abort, 2:Close }
 */
static int Bmsrv_send_modify_page_add_section(SockHandler *sh)
{
   //g_print("  sending page body...\n");

   /* send modify page2 */
   if (sock_handler_write_str(sh, modifypage_add_section_page, 1))
      return 1;

   //g_print("  page body sent.\n");
   return 2;
}

/*
 * Send the HTML for the modify page for "add url"
 * Return code: { 0:OK, 1:Abort, 2:Close }
 */
static int Bmsrv_send_modify_page_add_url(SockHandler *sh)
{
   if (sock_handler_write_str(sh, modifypage_add_url, 1))
      return 1;
   return 2;
}

/*
 * Parse a modify urls request and either:
 *   - make a local copy of the url
 *     or
 *   - send the modify page for the marked urls and sections
 * Return code: { 0:OK, 1:Abort, 2:Close }
 */
static int Bmsrv_send_modify_update(SockHandler *sh, char *url)
{
   static char *url1 = NULL;
   static GString *gstr = NULL;
   char *p, *q;
   int i, key, n_sec, n_url;
   BmRec *bm_node;
   BmSec *sec_node;

   /* bookmarks were loaded before */

   if (!gstr)
      gstr = g_string_new("");

   if (sh == NULL) {
      /* just copy url */
      g_free(url1);
      url1 = g_strdup(url);
      return 0;
   }

   /* send HTML here */
   if (sock_handler_write_str(sh, modifypage_update_header, 0))
      return 1;

   /* Count number of marked urls and sections */
   Bmsrv_count_urls_and_sections(url1, &n_sec, &n_url);

   if (n_sec) {
      g_string_sprintf(gstr, modifypage_update_title, "Update&nbsp;sections:");
      sock_handler_write_str(sh, gstr->str, 0);
      sock_handler_write_str(sh, modifypage_update_item_header, 0);
      /* send items here */
      p = strchr(url1, '?');
      for (q = p; (q = strstr(q, "&s")); ++q) {
         for (i = 0; isdigit(q[2+i]); ++i);
         if (q[2+i] == '=') {
            key = strtol(q + 2, NULL, 10);
            if ((sec_node = Bms_get_sec(key))) {
               g_string_sprintf(gstr, modifypage_update_item2,
                                sec_node->section, sec_node->title);
               sock_handler_write_str(sh, gstr->str, 0);
            }
         }
      }
      sock_handler_write_str(sh, modifypage_update_item_footer, 0);
   }

   if (n_url) {
      g_string_sprintf(gstr, modifypage_update_title, "Update&nbsp;titles:");
      sock_handler_write_str(sh, gstr->str, 0);
      sock_handler_write_str(sh, modifypage_update_item_header, 0);
      /* send items here */
      p = strchr(url1, '?');
      for (q = p; (q = strstr(q, "&url")); ++q) {
         for (i = 0; isdigit(q[4+i]); ++i);
         if (q[4+i] == '=') {
            key = strtol(q + 4, NULL, 10);
            bm_node = Bms_get(key);
            g_string_sprintf(gstr, modifypage_update_item,
                             bm_node->key, bm_node->title, bm_node->url);
            sock_handler_write_str(sh, gstr->str, 0);
         }
      }
      sock_handler_write_str(sh, modifypage_update_item_footer, 0);
   }

   sock_handler_write_str(sh, modifypage_update_footer, 1);

   return 2;
}

/*
 * Make the modify-page and send it back
 * Return code: { 0:OK, 1:Abort, 2:Close }
 */
static int Bmsrv_send_modify_answer(SockHandler *sh, char *url)
{
   GString *gstr = g_string_new("");

   g_string_sprintf(gstr, "<dpi cmd='start_send_page' url='%s'>", url);
   //g_print("  cmd_buf=%s\n", gstr->str);
   /* Send dpi command */
   if (sock_handler_write_str(sh, gstr->str, 1) != 0) {
      g_string_free(gstr, TRUE);
      return 1;
   }
   g_string_free(gstr, TRUE);

   /* Send HTTP header */
   //g_print("  sending Header...\n");
   if (sock_handler_write(sh, Header, strlen(Header), 0) != 0) {
      return 1;
   }

   if (MODIFY_PAGE_NUM == 2) {
      MODIFY_PAGE_NUM = 1;
      return Bmsrv_send_modify_page_add_section(sh);
   } else if (MODIFY_PAGE_NUM == 3) {
      MODIFY_PAGE_NUM = 1;
      return Bmsrv_send_modify_update(sh, NULL);
   } else if (MODIFY_PAGE_NUM == 4) {
      MODIFY_PAGE_NUM = 1;
      return Bmsrv_send_modify_page_add_url(sh);
   } else {
      return Bmsrv_send_modify_page(sh);
   }
}


/* Operations */

/*
 * Parse a delete bms request, delete them, and update bm file.
 * Return code: { 0:OK, 1:Abort }
 */
static int Bmsrv_modify_delete(SockHandler *sh, char *url)
{
   gchar *p;
   gint nb, ns, key;

   /* bookmarks were loaded before */

   /* Remove marked sections */
   p = strchr(url, '?');
   for (ns = 0; (p = strstr(p, "&s")); ++p) {
      if (isdigit(p[2])) {
         key = strtol(p + 2, NULL, 10);
         Bms_sec_del(key);
         ++ns;
      }
   }

   /* Remove marked urls */
   p = strchr(url, '?');
   for (nb = 0; (p = strstr(p, "&url")); ++p) {
      if (isdigit(p[4])) {
         key = strtol(p + 4, NULL, 10);
         Bms_del(key);
         ++nb;
      }
   }

/* -- This doesn't work because dillo erases the message upon the
 *    receipt of the first data stream.
 *
   sprintf(msg, "Deleted %d bookmark%s!>", n, (n > 1) ? "s" : "");
   if (Bmsrv_dpi_send_status_msg(sh, msg))
      return 1;
*/

   /* Write new bookmarks file */
   if (nb || ns)
      Bms_save();

   return 0;
}

/*
 * Parse a move urls request, move and update bm file.
 * Return code: { 0:OK, 1:Abort }
 */
static int Bmsrv_modify_move(SockHandler *sh, char *url)
{
   char *p;
   int n, section = 0, key;

   /* bookmarks were loaded before */

   /* get target section */
   for (p = url; (p = strstr(p, "&s")); ++p) {
      if (isdigit(p[2])) {
         section = strtol(p + 2, NULL, 10);
         break;
      }
   }
   if (!p)
      return 1;

   /* move marked urls */
   p = strchr(url, '?');
   for (n = 0; (p = strstr(p, "&url")); ++p) {
      if (isdigit(p[4])) {
         key = strtol(p + 4, NULL, 10);
         Bms_move(key, section);
         ++n;
      }
   }

   /* Write new bookmarks file */
   if (n) {
      Bms_save();
   }

   return 0;
}

/*
 * Parse a modify request: update urls and sections, then save.
 * Return code: { 0:OK, 1:Abort }
 */
static int Bmsrv_modify_update(SockHandler *sh, char *url)
{
   char *p, *q, *title;
   int i, key;

   /* bookmarks were loaded before */

   p = strchr(url, '?');
   for (  ; (p = strstr(p, "s")); ++p) {
      if (p[-1] == '&' || p[-1] == '?' ) {
         for (i = 0; isdigit(p[1 + i]); ++i);
         if (i && p[1 + i] == '=') {
            /* we have a title/key to change */
            key = strtol(p + 1, NULL, 10);
            if ((q = strchr(p + 1, '&')))
               title = g_strndup(p + 2 + i, (guint)(q - (p + 2 + i)));
            else
               title = g_strdup(p + 2 + i);

            Unencode_str(title);
            Bms_update_sec_title(key, title);
            g_free(title);
         }
      }
   }

   p = strchr(url, '?');
   for (  ; (p = strstr(p, "title")); ++p) {
      if (p[-1] == '&' || p[-1] == '?' ) {
         for (i = 0; isdigit(p[5 + i]); ++i);
         if (i && p[5 + i] == '=') {
            /* we have a title/key to change */
            key = strtol(p + 5, NULL, 10);
            if ((q = strchr(p + 5, '&')))
               title = g_strndup(p + 6 + i, (guint)(q - (p + 6 + i)));
            else
               title = g_strdup(p + 6 + i);

            Unencode_str(title);
            Bms_update_title(key, title);
            g_free(title);
         }
      }
   }

   /* Write new bookmarks file */
   Bms_save();

   return 0;
}

/*
 * Parse an "add section" request, and update the bm file.
 * Return code: { 0:OK, 1:Abort }
 */
static int Bmsrv_modify_add_section(SockHandler *sh, char *url)
{
   char *p, *title = NULL;

   /* bookmarks were loaded before */

   /* get new section's title */
   if ((p = strstr(url, "&title="))) {
      title = g_strdup (p + 7);
      if ((p = strchr(title, '&')))
         *p = 0;
      Unencode_str(title);
   } else
      return 1;

   Bms_sec_add(title);
   g_free(title);

   /* Write new bookmarks file */
   Bms_save();

   return 0;
}

/*
 * Parse an "add url" request, and update the bm file.
 * Return code: { 0:OK, 1:Abort }
 */
static int Bmsrv_modify_add_url(SockHandler *sh, char *s_url)
{
   char *p, *q, *title, *url;
   int i;
   static gint section = 0;

   /* bookmarks were loaded before */

   if (sh == NULL) {
      /* look for section */
      for (q = s_url; (q = strstr(q, "&s")); ++q) {
         for (i = 0; isdigit(q[2+i]); ++i);
         if (q[2+i] == '=')
            section = strtol(q + 2, NULL, 10);
      }
      return 1;
   }

   if (!(p = strstr(s_url, "&title=")) ||
       !(q = strstr(s_url, "&url=")))
      return 1;

   title = g_strdup (p + 7);
   if ((p = strchr(title, '&')))
      *p = 0;
   url = g_strdup (q + 5);
   if ((p = strchr(url, '&')))
      *p = 0;
   if (strlen(title) && strlen(url)) {
      Unencode_str(title);
      Unencode_str(url);
      Bms_add(section, url, title);
   }
   g_free(title);
   g_free(url);
   section = 0;

   /* todo: we should send an "Bookmark added" message, but the
      msg-after-HTML functionallity is still pending, not hard though. */

   /* Write new bookmarks file */
   Bms_save();

   return 0;
}

/*
 * Check the parameters of a modify request, and return an error message
 * when it's wrong.
 * Return code: { 0:OK, 2:Close }
 */
static int Bmsrv_check_modify_request(SockHandler *sh, char *url)
{
   char *p, *msg;
   int n_sec, n_url;

   /* Count number of marked urls and sections */
   Bmsrv_count_urls_and_sections(url, &n_sec, &n_url);

   p = strchr(url, '?');
   if (strstr(p, "operation=delete&")) {
      if (n_url || n_sec)
         return 0;
      msg = "Delete: you must mark what to delete!";

   } else if (strstr(url, "operation=move&")) {
      if (n_url && n_sec)
         return 0;
      else if (n_url)
         msg = "Move: you must mark a target section!";
      else if (n_sec)
         msg = "Move: can not move a section (yet).";
      else
         msg = "Move: you must mark some urls, and a target section!";

   } else if (strstr(url, "operation=modify&")) {
      if (n_url || n_sec)
         return 0;
      msg = "Modify: you must mark what to update!";

   } else if (strstr(url, "operation=modify2&")) {
      /* nothing to check here */
      return 0;

   } else if (strstr(url, "operation=add_sec&")) {
      /* nothing to check here */
      return 0;

   } else if (strstr(url, "operation=add_section&")) {
      /* nothing to check here */
      return 0;

   } else if (strstr(url, "operation=add_url&")) {
      if (n_sec <= 1)
         return 0;
      msg = "Add url: only one target section is allowed!";

   } else if (strstr(url, "operation=add_url2&")) {
      /* nothing to check here */
      return 0;

   } else if (strstr(url, "operation=none&")) {
      msg = "No operation, just do nothing!";

   } else {
      msg = "Sorry, not implemented yet.";
   }

   Bmsrv_dpi_send_status_msg(sh, msg);
   return 2;
}

/*
 * Parse a and process a modify request.
 * Return code: { 0:OK, 1:Abort, 2:Close }
 */
static int Bmsrv_process_modify_request(SockHandler *sh, char *url)
{
   /* check the provided parameters */
   if (Bmsrv_check_modify_request(sh, url) != 0)
      return 2;

   if (strstr(url, "operation=delete&")) {
      if (Bmsrv_modify_delete(sh, url) == 1)
         return 1;
      if (Bmsrv_send_reload_request(sh, "dpi:/bm/modify") == 1)
         return 1;

   } else if (strstr(url, "operation=move&")) {
      if (Bmsrv_modify_move(sh, url) == 1)
         return 1;
      if (Bmsrv_send_reload_request(sh, "dpi:/bm/modify") == 1)
         return 1;

   } else if (strstr(url, "operation=modify&")) {
      /* make a local copy of 'url' */
      Bmsrv_send_modify_update(NULL, url);
      MODIFY_PAGE_NUM = 3;
      if (Bmsrv_send_reload_request(sh, "dpi:/bm/modify") == 1)
         return 1;

   } else if (strstr(url, "operation=modify2&")) {
      if (Bmsrv_modify_update(sh, url) == 1)
         return 1;
      if (Bmsrv_send_reload_request(sh, "dpi:/bm/modify") == 1)
         return 1;

   } else if (strstr(url, "operation=add_sec&")) {
      /* this global variable tells which page to send  (--hackish...) */
      MODIFY_PAGE_NUM = 2;
      if (Bmsrv_send_reload_request(sh, "dpi:/bm/modify") == 1)
         return 1;

   } else if (strstr(url, "operation=add_section&")) {
      if (Bmsrv_modify_add_section(sh, url) == 1)
         return 1;
      if (Bmsrv_send_reload_request(sh, "dpi:/bm/modify") == 1)
         return 1;

   } else if (strstr(url, "operation=add_url&")) {
      /* this global variable tells which page to send  (--hackish...) */
      MODIFY_PAGE_NUM = 4;
      /* parse section if present */
      Bmsrv_modify_add_url(NULL, url);
      if (Bmsrv_send_reload_request(sh, "dpi:/bm/modify") == 1)
         return 1;

   } else if (strstr(url, "operation=add_url2&")) {
      if (Bmsrv_modify_add_url(sh, url) == 1)
         return 1;
      if (Bmsrv_send_reload_request(sh, "dpi:/bm/modify") == 1)
         return 1;
   }

   return 2;
}

/* -- Bookmarks ------------------------------------------------------------ */

/*
 * Send the current bookmarks page (in HTML)
 */
static int send_bm_page(SockHandler *sh)
{
   static GString *gstr = NULL;
   gchar *l_title;
   GSList *list1, *list2;
   BmSec *sec_node;
   BmRec *bm_node;

   if (!gstr)
      gstr = g_string_new("");

   if (sock_handler_write_str(sh, mainpage_header, 0))
      return 1;

   /* write sections header */
   if (sock_handler_write_str(sh, mainpage_sections_header, 0))
      return 1;
   /* write sections */
   for (list1 = B_secs; list1; list1 = list1->next) {
      sec_node = list1->data;
      g_string_sprintf(gstr, mainpage_sections_item,
                       sec_node->section, sec_node->title);
      if (sock_handler_write_str(sh, gstr->str, 0))
         return 1;
   }
   /* write sections footer */
   if (sock_handler_write_str(sh, mainpage_sections_footer, 0))
      return 1;

   /* send page middle */
   if (sock_handler_write_str(sh, mainpage_middle1, 0))
      return 1;

   /* send bookmark cards */
   for (list1 = B_secs; list1; list1 = list1->next) {
      sec_node = list1->data;

      /* send card header */
      l_title = make_one_line_str(sec_node->title);
      g_string_sprintf(gstr, mainpage_section_card_header,
                       sec_node->section, l_title);
      g_free(l_title);
      if (sock_handler_write_str(sh, gstr->str, 0))
         return 1;

      /* send section's bookmarks */
      for (list2 = B_bms; list2; list2 = list2->next) {
         bm_node = list2->data;
         if (bm_node->section == sec_node->section) {
            g_string_sprintf(gstr, mainpage_section_card_item,
                             bm_node->url, bm_node->title);
            if (sock_handler_write_str(sh, gstr->str, 0))
               return 1;
         }
      }

      /* send card footer */
      if (sock_handler_write_str(sh, mainpage_section_card_footer, 0))
         return 1;
   }

   /* finish page */
   if (sock_handler_write_str(sh, mainpage_footer, 1))
      return 1;

   return 0;
}


/* -- Dpi parser ----------------------------------------------------------- */

/*
 * Parse a data stream (dpi protocol)
 * Note: Buf is a zero terminated string
 * Return code: { 0:OK, 1:Abort, 2:Close }
 */
static int Bmsrv_parse_buf(SockHandler *sh, char *Buf, size_t BufSize)
{
   char *msg1="<dpi cmd='chat' msg='Hi browser'>";
   char *msg2="<dpi cmd='chat' msg='Is it worth?'>";
   char *msg3="<dpi cmd='chat' msg='Ok, send it'>";

   char *p, *cmd, *url, *title, *msg, cmd_buf[16*1024];
   int st;

   if (!(p = strchr(Buf, '>'))) {
      /* Haven't got a full tag */
      g_print("Haven't got a full tag!\n");
      return 1;
   }

   cmd = Get_attr_value(Buf, BufSize, "cmd");
   //g_print("cmd: %s\n", cmd ? cmd : "NULL");

   if (cmd && strcmp(cmd, "chat") == 0) {
      g_free(cmd);
      msg = Get_attr_value(Buf, BufSize, "msg");
      if (*msg == 'H') {
         /* "Hi server" */
         if (sock_handler_write_str(sh, msg1, 1))
            return 1;
      } else if (*msg == 'I') {
         /* "I want to set abookmark" */
         if (sock_handler_write_str(sh, msg2, 1))
            return 1;
      } else if (*msg == 'S') {
         /* "Sure" */
         if (sock_handler_write_str(sh, msg3, 1))
            return 1;
      }
      g_free(msg);
      return 0;
   }

   /* sync with the bookmarks file */
   Bms_cond_load();

   if (cmd && strcmp(cmd, "DpiBye") == 0) {
      g_print("bookmarks dpi (pid %d): Got DpiBye.\n", (gint)getpid());
      exit(0);

   } else if (cmd && strcmp(cmd, "add_bookmark") == 0) {
      g_free(cmd);
      url = Get_attr_value(Buf, BufSize, "url");
      title = Get_attr_value(Buf, BufSize, "title");
      if (strlen(title) == 0) {
         g_free(title);
         title = g_strdup("(Untitled)");
      }
      if (url && title)
         Bmsrv_add_bm(sh, url, title);
      g_free(url);
      g_free(title);
      return 2;

   } else if (cmd && strcmp(cmd, "open_url") == 0) {
      g_free(cmd);
      url = Get_attr_value(Buf, BufSize, "url");

      if (strcmp(url, "dpi:/bm/modify") == 0) {
         st = Bmsrv_send_modify_answer(sh, url);
         return st;

      } else if (strncmp(url, "dpi:/bm/modify?", 15) == 0) {
         /* process request */
         st = Bmsrv_process_modify_request(sh, url);
         return st;
      }

      sprintf(cmd_buf, "<dpi cmd='start_send_page' url='%s'>", url);
      //g_print("  cmd_buf=%s\n", cmd_buf);
      /* Send dpi command */
      if (sock_handler_write(sh, cmd_buf, strlen(cmd_buf), 1) != 0) {
         return 1;
      }

      /* Send HTTP header */
      //g_print("  sending HTTP header...\n");
      if (sock_handler_write(sh, Header, strlen(Header), 1) != 0) {
         return 1;
      }

      //g_print("  sending page body...\n");
      st = send_bm_page(sh);
      if (st != 0) {
         char *err =
            "<HTML><body> Error on the bookmarks server...</body></html>";
         if (sock_handler_write(sh, err, strlen(err), 1) != 0) {
            return 1;
         }
      }
      //g_print("  page body sent.\n");
      return 2;
   }

   return 0;
}

/* --  Termination handlers ----------------------------------------------- */
/*
 * (was to delete the local namespace socket),
 *  but this is handled by 'dpid' now.
 */
static void cleanup(void)
{
  /* no cleanup required */
}

/*
 * Perform any necessary cleanups upon abnormal termination
 */
static void termination_handler(int signum)
{
  exit(signum);
}


/*
 * -- MAIN -------------------------------------------------------------------
 */
int main (void) {
   struct sockaddr_un spun;
   int temp_sock_descriptor;
   int address_size;
   char *buf;
   int code;
   SockHandler *sh;

   /* Arrange the cleanup function for terminations via exit() */
   atexit(cleanup);

   /* Arrange the cleanup function for abnormal terminations */
   if (signal (SIGINT, termination_handler) == SIG_IGN)
     signal (SIGINT, SIG_IGN);
   if (signal (SIGHUP, termination_handler) == SIG_IGN)
     signal (SIGHUP, SIG_IGN);
   if (signal (SIGTERM, termination_handler) == SIG_IGN)
     signal (SIGTERM, SIG_IGN);

   BmFile = g_strconcat(g_get_home_dir(), "/", ".dillo/bm.txt", NULL);

   g_print("bookmarks.dpi (v.13): accepting connections...\n");

   /* some OSes may need this... */
   address_size = sizeof(struct sockaddr_un);

   while (1) {
      temp_sock_descriptor =
         accept(STDIN_FILENO, (struct sockaddr *)&spun, &address_size);
      if (temp_sock_descriptor == -1) {
         perror("[accept]");
         exit(1);
      }

      /* create the SockHandler structure */
      sh = sock_handler_new(temp_sock_descriptor,temp_sock_descriptor,8*1024);

      while (1) {
         code = 1;
         if ((buf = sock_handler_read(sh)) != NULL) {
            /* Let's see what we fished... */
            code = Bmsrv_parse_buf(sh, buf, strlen(buf));
         }
         if (code == 1)
            exit(1);
         else if (code == 2)
            break;
      }

      //g_print("Closing SockHandler\n");
      sock_handler_close(sh);
      sock_handler_free(sh);

   }/*while*/
}
