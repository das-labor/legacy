/*
 * Preferences for dillo
 *
 * Copyright (C) 2000 Luca Rota <lrota@cclinf.polito.it>
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

#include <glib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>       /* for strchr */
#include <fcntl.h>
#include <unistd.h>
#include <locale.h>       /* for setlocale */
#include "prefs.h"
#include "colors.h"
#include "misc.h"

/* symbol array */
static const struct {
   gchar *name;
   guint  token;
} symbols[] = {
   { "geometry", DRC_TOKEN_GEOMETRY },
   { "http_proxy", DRC_TOKEN_PROXY },
   { "http_proxyuser", DRC_TOKEN_PROXYUSER },
   { "no_proxy", DRC_TOKEN_NOPROXY },
   { "link_color", DRC_TOKEN_LINK_COLOR },
   { "visited_color", DRC_TOKEN_VISITED_COLOR, },
   { "bg_color", DRC_TOKEN_BG_COLOR },
   { "allow_white_bg", DRC_TOKEN_ALLOW_WHITE_BG },
   { "force_my_colors", DRC_TOKEN_FORCE_MY_COLORS },
   { "contrast_visited_color", DRC_TOKEN_CONTRAST_VISITED_COLOR },
   { "text_color", DRC_TOKEN_TEXT_COLOR },
   { "use_oblique", DRC_TOKEN_USE_OBLIQUE },
   { "start_page", DRC_TOKEN_START_PAGE },
   { "home", DRC_TOKEN_HOME },
   { "show_tooltip", DRC_TOKEN_SHOW_TOOLTIP },
   { "panel_size", DRC_TOKEN_PANEL_SIZE },
   { "small_icons", DRC_TOKEN_SMALL_ICONS },
   { "limit_text_width", DRC_TOKEN_LIMIT_TEXT_WIDTH },
   { "w3c_plus_heuristics", DRC_TOKEN_W3C_PLUS_HEURISTICS },
   { "font_factor", DRC_TOKEN_FONT_FACTOR },
   { "use_dicache", DRC_TOKEN_USE_DICACHE },
   { "show_back", DRC_TOKEN_SHOW_BACK },
   { "show_forw", DRC_TOKEN_SHOW_FORW },
   { "show_home", DRC_TOKEN_SHOW_HOME },
   { "show_reload", DRC_TOKEN_SHOW_RELOAD },
   { "show_save", DRC_TOKEN_SHOW_SAVE },
   { "show_stop", DRC_TOKEN_SHOW_STOP },
   { "show_bookmarks", DRC_TOKEN_SHOW_BOOKMARKS },
   { "show_menubar", DRC_TOKEN_SHOW_MENUBAR },
   { "show_clear_url", DRC_TOKEN_SHOW_CLEAR_URL },
   { "show_url", DRC_TOKEN_SHOW_URL },
   { "show_search", DRC_TOKEN_SHOW_SEARCH },
   { "show_progress_box", DRC_TOKEN_SHOW_PROGRESS_BOX },
   { "fullwindow_start", DRC_TOKEN_FULLWINDOW_START },
   { "transient_dialogs", DRC_TOKEN_TRANSIENT_DIALOGS },
   { "vw_fontname", DRC_TOKEN_VW_FONT },
   { "fw_fontname", DRC_TOKEN_FW_FONT },
   { "generate_submit", DRC_TOKEN_GENERATE_SUBMIT },
   { "enterpress_forces_submit", DRC_TOKEN_ENTERPRESS_FORCES_SUBMIT },
   { "search_url", DRC_TOKEN_SEARCH_URL },
   { "show_msg", DRC_TOKEN_SHOW_MSG },
   { "show_extra_warnings", DRC_TOKEN_SHOW_EXTRA_WARNINGS }
};

static const guint n_symbols = sizeof (symbols) / sizeof (symbols[0]);

/*
 * Read tokens from dillorc and set values in the prefs structure.
 * (Although this function can be called several times, and not leak,
 *  preferences aren't yet enabled for on-the-fly changes).
 */
static guint Prefs_parser(GScanner *scanner)
{
   gint st;
   guint symbol;

   /* expect a valid symbol */
   g_scanner_get_next_token(scanner);
   symbol = scanner->token;
   if (scanner->token == G_TOKEN_EQUAL_SIGN) {
      g_scanner_get_next_token (scanner);
      return G_TOKEN_NONE;
   } else if (symbol < DRC_TOKEN_FIRST || symbol > DRC_TOKEN_LAST )
      return G_TOKEN_SYMBOL;

   /* expect '=' */
   g_scanner_get_next_token(scanner);
   if (scanner->token != G_TOKEN_EQUAL_SIGN)
      return G_TOKEN_EQUAL_SIGN;

   /* expect a string */
   g_scanner_get_next_token(scanner);
   if (scanner->token != G_TOKEN_STRING)
      return G_TOKEN_STRING;

   /* assign value and exit successfully */
   switch (symbol) {
   case DRC_TOKEN_GEOMETRY:
      a_Misc_parse_geometry(scanner->value.v_string, &prefs.xpos, &prefs.ypos,
                            &prefs.width, &prefs.height);
      break;
   case DRC_TOKEN_PROXY:
      a_Url_free(prefs.http_proxy);
      prefs.http_proxy = a_Url_new(scanner->value.v_string, NULL, 0, 0, 0);
      break;
   case DRC_TOKEN_PROXYUSER:
      g_free(prefs.http_proxyuser);
      prefs.http_proxyuser = g_strdup(scanner->value.v_string);
      break;
   case DRC_TOKEN_NOPROXY:
      g_free(prefs.no_proxy);
      prefs.no_proxy = g_strdup(scanner->value.v_string);
      prefs.no_proxy_vec = g_strsplit(prefs.no_proxy, " ", 0);
      break;
   case DRC_TOKEN_LINK_COLOR:
      prefs.link_color = a_Color_parse(scanner->value.v_string,
                                       prefs.link_color, &st);
      break;
   case DRC_TOKEN_VISITED_COLOR:
      prefs.visited_color = a_Color_parse(scanner->value.v_string,
                                          prefs.visited_color, &st);
      break;
   case DRC_TOKEN_TEXT_COLOR:
      prefs.text_color = a_Color_parse(scanner->value.v_string,
                                       prefs.text_color, &st);
      break;
   case DRC_TOKEN_BG_COLOR:
      prefs.bg_color = a_Color_parse(scanner->value.v_string,
                                     prefs.bg_color, &st);
      break;
   case DRC_TOKEN_ALLOW_WHITE_BG:
      prefs.allow_white_bg = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_FORCE_MY_COLORS:
      prefs.force_my_colors = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_CONTRAST_VISITED_COLOR:
      prefs.contrast_visited_color =
         (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_USE_OBLIQUE:
      prefs.use_oblique = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_PANEL_SIZE:
      if (!g_strcasecmp(scanner->value.v_string, "tiny"))
         prefs.panel_size = 1;
      else if (!g_strcasecmp(scanner->value.v_string, "small"))
         prefs.panel_size = 2;
      else if (!g_strcasecmp(scanner->value.v_string, "medium"))
         prefs.panel_size = 3;
      else /* default to "large" */
         prefs.panel_size = 4;
      break;
   case DRC_TOKEN_SMALL_ICONS:
      prefs.small_icons = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_START_PAGE:
      a_Url_free(prefs.start_page);
      prefs.start_page = a_Url_new(scanner->value.v_string, NULL, 0, 0, 0);
      break;
   case DRC_TOKEN_HOME:
      a_Url_free(prefs.home);
      prefs.home = a_Url_new(scanner->value.v_string, NULL, 0, 0, 0);
      break;
   case DRC_TOKEN_SHOW_TOOLTIP:
      prefs.show_tooltip = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_FONT_FACTOR:
      prefs.font_factor = strtod(scanner->value.v_string, NULL);
      break;
   case DRC_TOKEN_LIMIT_TEXT_WIDTH:
      prefs.limit_text_width = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_W3C_PLUS_HEURISTICS:
      prefs.w3c_plus_heuristics = (strcmp(scanner->value.v_string,"YES") == 0);
      break;
   case DRC_TOKEN_USE_DICACHE:
      prefs.use_dicache = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_BACK:
      prefs.show_back = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_FORW:
      prefs.show_forw = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_HOME:
      prefs.show_home = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_RELOAD:
      prefs.show_reload = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_SAVE:
      prefs.show_save = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_STOP:
      prefs.show_stop = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_BOOKMARKS:
      prefs.show_bookmarks = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_MENUBAR:
      prefs.show_menubar = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_CLEAR_URL:
      prefs.show_clear_url = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_URL:
      prefs.show_url = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_SEARCH:
      prefs.show_search = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_PROGRESS_BOX:
      prefs.show_progress_box = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_FULLWINDOW_START:
      prefs.fullwindow_start = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_TRANSIENT_DIALOGS:
      prefs.transient_dialogs = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_FW_FONT:
      g_free(prefs.fw_fontname);
      prefs.fw_fontname = g_strdup(scanner->value.v_string);
      break;
   case DRC_TOKEN_VW_FONT:
      g_free(prefs.vw_fontname);
      prefs.vw_fontname = g_strdup(scanner->value.v_string);
      break;
   case DRC_TOKEN_GENERATE_SUBMIT:
      prefs.generate_submit = (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_ENTERPRESS_FORCES_SUBMIT:
      prefs.enterpress_forces_submit =
         (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SEARCH_URL:
      g_free(prefs.search_url);
      prefs.search_url = g_strdup(scanner->value.v_string);
      break;
   case DRC_TOKEN_SHOW_MSG:
      prefs.show_msg =
         (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   case DRC_TOKEN_SHOW_EXTRA_WARNINGS:
      prefs.show_extra_warnings =
         (strcmp(scanner->value.v_string, "YES") == 0);
      break;
   default:
      break;   /* Not reached */
   }
   return G_TOKEN_NONE;
}

static gint Prefs_load(void)
{
   GScanner *scanner;
   gint fd;
   guint i, expected_token;
   gchar *file;

   /* Here we load and set options from dillorc */
   file = a_Misc_prepend_user_home(".dillo/dillorc");
   fd = open(file, O_RDONLY);
   g_free(file);
   if (fd < 0 && (fd = open(DILLORC_SYS, O_RDONLY)) < 0)
      return FILE_NOT_FOUND;

   fcntl(fd, F_SETFD, FD_CLOEXEC | fcntl(fd, F_GETFD));

   scanner = g_scanner_new(NULL);

   /* Adjust lexing behaviour to suit our needs */
   /* Specifies the chars which can be used in identifiers */
   scanner->config->cset_identifier_nth = (
      G_CSET_a_2_z
      "~+-_:&%#/.0123456789"
      G_CSET_A_2_Z
      G_CSET_LATINS   /*??? I don't know if we need these two */
      G_CSET_LATINC   /*??? */
   );
   /* Specifies the chars which can start identifiers */
   scanner->config->cset_identifier_first = (
      G_CSET_a_2_z
      G_CSET_A_2_Z
      "_0123456789"
   );
   /* Don't return G_TOKEN_SYMBOL, but the symbol's value */
   scanner->config->symbol_2_token = TRUE;
   /* Don't return G_TOKEN_IDENTIFIER, but convert it to string */
   scanner->config->identifier_2_string = TRUE;

   /* load symbols into the scanner */
   g_scanner_freeze_symbol_table(scanner);
   for (i = 0; i < n_symbols; i++)
      g_scanner_add_symbol(scanner, symbols[i].name,
                           GINT_TO_POINTER (symbols[i].token));
   g_scanner_thaw_symbol_table(scanner);

   /* feed in the text */
   g_scanner_input_file(scanner, fd);

   /* give the error handler an idea on how the input is named */
   scanner->input_name = "dillorc";

   /*
    * Scanning loop, we parse the input until it's end is reached,
    * the scanner encountered a lexing error, or our sub routine came
    * across invalid syntax
    */
   do {
      expected_token = Prefs_parser(scanner);

      /* Give an error message upon syntax errors */
      if (expected_token == G_TOKEN_SYMBOL)
         g_scanner_unexp_token (scanner, expected_token, NULL, "symbol", NULL,
                                NULL, FALSE);
      else if (expected_token == G_TOKEN_STRING)
         g_scanner_unexp_token (scanner, expected_token, NULL, "string", NULL,
                                NULL, FALSE);
      else if (expected_token == G_TOKEN_EQUAL_SIGN)
         g_scanner_unexp_token (scanner, expected_token, NULL, "=", NULL,
                                NULL, FALSE);
      g_scanner_peek_next_token (scanner);
   } while ( /* expected_token == G_TOKEN_NONE && */
            scanner->next_token != G_TOKEN_EOF &&
            scanner->next_token != G_TOKEN_ERROR);

   /* give an error message upon syntax errors */
   if (expected_token != G_TOKEN_NONE)
      g_scanner_unexp_token(scanner, expected_token, NULL, "symbol", NULL,
                            NULL, TRUE);

   /* finish parsing */
   g_scanner_destroy(scanner);
   close(fd);
   return PARSE_OK;
}

void a_Prefs_init(void)
{
   gchar *old_locale;

   prefs.width = D_GEOMETRY_DEFAULT_WIDTH;
   prefs.height = D_GEOMETRY_DEFAULT_HEIGHT;
   prefs.xpos = D_GEOMETRY_DEFAULT_XPOS;
   prefs.ypos = D_GEOMETRY_DEFAULT_YPOS;
   prefs.http_proxy = a_Url_new(HTTP_PROXY, NULL, 0, 0, 0);
   //prefs.http_proxy = NULL;
   prefs.http_proxyuser = NULL;
   prefs.no_proxy = NULL;
   prefs.no_proxy_vec = NULL;
   prefs.link_color = DW_COLOR_DEFAULT_BLUE;
   prefs.visited_color = DW_COLOR_DEFAULT_PURPLE;
   prefs.bg_color = DW_COLOR_DEFAULT_BGND;
   prefs.text_color = DW_COLOR_DEFAULT_BLACK;
   prefs.use_oblique = FALSE;
   prefs.start_page = a_Url_new(DILLO_START_PAGE, NULL, 0, 0, 0);
   prefs.home = a_Url_new(DILLO_HOME, NULL, 0, 0, 0);
   prefs.allow_white_bg = TRUE;
   prefs.force_my_colors = FALSE;
   prefs.contrast_visited_color = FALSE;
   prefs.show_tooltip = FALSE;
   prefs.panel_size = 1;
   prefs.small_icons = FALSE;
   prefs.limit_text_width = FALSE;
   prefs.w3c_plus_heuristics = TRUE;
   prefs.font_factor = 1.0;
   prefs.use_dicache = FALSE;
   prefs.show_back=TRUE;
   prefs.show_forw=TRUE;
   prefs.show_home=TRUE;
   prefs.show_reload=TRUE;
   prefs.show_save=TRUE;
   prefs.show_stop=TRUE;
   prefs.show_bookmarks=TRUE;
   prefs.show_menubar=TRUE;
   prefs.show_clear_url=TRUE;
   prefs.show_url=TRUE;
   prefs.show_search=TRUE;
   prefs.show_progress_box=TRUE;
   prefs.fullwindow_start=FALSE;
   prefs.transient_dialogs=FALSE;
   prefs.vw_fontname = g_strdup("helvetica");
   prefs.fw_fontname = g_strdup("courier");
   prefs.generate_submit = FALSE;
   prefs.enterpress_forces_submit = FALSE;
   prefs.search_url = g_strdup("http://www.google.com/search?q=%s");
   prefs.show_msg = TRUE;
   prefs.show_extra_warnings = FALSE;

   /* this locale stuff is to avoid parsing problems with float numbers */
   old_locale = g_strdup (setlocale (LC_NUMERIC, NULL));
   setlocale (LC_NUMERIC, "C");

   Prefs_load();

   setlocale (LC_NUMERIC, old_locale);
   g_free (old_locale);
}

/*
 *  Preferences memory-deallocation
 *  (Call this one at exit time)
 */
void a_Prefs_freeall(void)
{
   g_free(prefs.http_proxyuser);
   g_free(prefs.no_proxy);
   if (prefs.no_proxy_vec)
      g_strfreev(prefs.no_proxy_vec);
   a_Url_free(prefs.http_proxy);
   g_free(prefs.fw_fontname);
   g_free(prefs.vw_fontname);
   a_Url_free(prefs.start_page);
   a_Url_free(prefs.home);
   g_free(prefs.search_url);
}
