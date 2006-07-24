/*
 * File: web.c
 *
 * Copyright 2000 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>      /* for rint */

#include "msg.h"
#include "browser.h"
#include "nav.h"
#include "interface.h"
#include "IO/IO.h"
#include "IO/mime.h"

#include "dw_widget.h"
#include "dw_gtk_scrolled_window.h"
#include "dw_embed_gtk.h"
#include "prefs.h"
#include "web.h"

#define DEBUG_LEVEL 5
#include "debug.h"

/*
 * Local data
 */
static GSList *ValidWebs = NULL;  /* Active web structures list; it holds
                                   * pointers to DilloWeb structures. */

/*
 * Given the MIME content type, and a fd to read it from,
 * this function connects the proper MIME viewer to it.
 * Return value: 1 on success, -1 for unhandled MIME types
 */
gint a_Web_dispatch_by_type (const char *Type, DilloWeb *Web,
                             CA_Callback_t *Call, void **Data)
{
   DwWidget *dw = NULL;
   DwStyle style_attrs, *style;
   DwStyleFont font;

   DEBUG_MSG(1, "a_Web_dispatch_by_type\n");

   g_return_val_if_fail(Web->bw != NULL, -1);

   if (Web->flags & WEB_RootUrl) {
      /* We have RootUrl! */
      dw = a_Mime_set_viewer(Type, Web, Call, Data);
      if (dw == NULL)
         return -1;

      /* Set a style for the widget */
      font.name = prefs.vw_fontname; /* must be defined */
      font.size = rint(12.0 * prefs.font_factor);
      font.weight = 400;
      font.style = DW_STYLE_FONT_STYLE_NORMAL;

      a_Dw_style_init_values (&style_attrs, Web->bw->main_window->window);
      a_Dw_style_box_set_val (&style_attrs.margin, 5);
      style_attrs.font = a_Dw_style_font_new (&font);
      style_attrs.color =
         a_Dw_style_color_new (prefs.text_color, Web->bw->main_window->window);
      style_attrs.background_color =
         a_Dw_style_color_new (prefs.bg_color, Web->bw->main_window->window);
      style = a_Dw_style_new (&style_attrs, Web->bw->main_window->window);
      a_Dw_widget_set_style (dw, style);
      a_Dw_style_unref (style);

      a_Dw_gtk_scrolled_window_set_dw(
         GTK_DW_SCROLLED_WINDOW(Web->bw->docwin), dw);

      if (URL_POSX(Web->url) || URL_POSY(Web->url)) {
        a_Dw_gtk_scrolled_window_set_scrolling_position(
           GTK_DW_SCROLLED_WINDOW(Web->bw->docwin),
           URL_POSX(Web->url), URL_POSY(Web->url));
      } else {
        gchar *pf = a_Url_decode_hex_str(URL_FRAGMENT_(Web->url));
        a_Dw_gtk_scrolled_window_set_anchor(
           GTK_DW_SCROLLED_WINDOW(Web->bw->docwin), pf);
        g_free(pf);
      }

      /* Clear the title bar for pages without a <TITLE> tag */
      a_Interface_set_page_title(Web->bw, "");
      a_Interface_set_location_text(Web->bw, URL_STR(Web->url));
      a_Interface_reset_progress_bars(Web->bw);
      /* Reset the bug meter */
      a_Interface_bug_meter_update(Web->bw, 0);

      /* Let the Nav module know... */
      a_Nav_expect_done(Web->bw);

   } else {
      /* A non-RootUrl. At this moment we only handle image-children */
      if (!g_strncasecmp(Type, "image/", 6))
         dw = a_Mime_set_viewer(Type, Web, Call, Data);
   }

   if (!dw) {
      MSG_HTTP("unhandled MIME type: \"%s\"\n", Type);
   }
   return (dw ? 1 : -1);
}


/*
 * Allocate and set safe values for a DilloWeb structure
 */
DilloWeb* a_Web_new(const DilloUrl *url)
{
   DilloWeb *web= g_new(DilloWeb, 1);

   _MSG(" a_Web_new: ValidWebs ==> %d\n", g_slist_length(ValidWebs));
   web->url = a_Url_dup(url);
   web->bw = NULL;
   web->flags = 0;
   web->Image = NULL;
   web->stream  = NULL;
   web->SavedBytes = 0;

   ValidWebs = g_slist_append(ValidWebs, (gpointer)web);
   return web;
}

/*
 * Validate a DilloWeb pointer
 */
gint a_Web_valid(DilloWeb *web)
{
   return (g_slist_find(ValidWebs, web) != NULL);
}

/*
 * Deallocate a DilloWeb structure
 */
void a_Web_free(DilloWeb *web)
{
   if (!web) return;
   if (web->url)
      a_Url_free(web->url);
   if (web->Image)
      a_Image_unref(web->Image);
   ValidWebs = g_slist_remove(ValidWebs, (gpointer)web);
   g_free(web);
}

