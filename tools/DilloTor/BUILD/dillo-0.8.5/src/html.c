/*
 * File: html.c
 *
 * Copyright (C) 1997 Raph Levien <raph@acm.org>
 * Copyright (C) 1999 James McCollough <jamesm@gtwn.net>
 * Copyright (C) 2000-2004 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Dillo HTML parsing routines
 */

/* Undefine if you want to unroll tables. For instance for PDAs */
#define USE_TABLES

/* Define to 1 to ignore white space immediately after an open tag,
 * and immediately before a close tag. */
#define SGML_SPCDEL 0


#include <ctype.h>      /* for isspace and tolower */
#include <string.h>     /* for memcpy and memmove */
#include <stdlib.h>
#include <stdio.h>      /* for sprintf */
#include <math.h>      /* for rint */
#include <errno.h>

#include <gtk/gtk.h>

#include "msg.h"
#include "list.h"
#include "binaryconst.h"
#include "colors.h"
#include "dillo.h"
#include "history.h"
#include "nav.h"
#include "menu.h"
#include "commands.h"
#include "dw.h"         /* for Dw_cursor_hand */

#include "dw_gtk_viewport.h"
#include "dw_gtk_scrolled_window.h"
#include "dw_widget.h"
#include "dw_page.h"
#include "dw_bullet.h"
#include "dw_button.h"
#include "dw_hruler.h"
#include "dw_embed_gtk.h"
#include "dw_table.h"
#include "dw_table_cell.h"
#include "dw_list_item.h"
#include "dw_style.h"
#include "interface.h"
#include "progressbar.h"
#include "prefs.h"
#include "misc.h"
#include "capi.h"
#include "html.h"

#define DEBUG_LEVEL 10
#include "debug.h"

typedef void (*TagOpenFunct) (DilloHtml *Html, char *Tag, gint Tagsize);
typedef void (*TagCloseFunct) (DilloHtml *Html, gint TagIdx);

#define TAB_SIZE 8

/*
 * Forward declarations
 */
static const char *Html_get_attr(DilloHtml *html,
                                 const char *tag,
                                 gint tagsize,
                                 const char *attrname);
static const char *Html_get_attr2(DilloHtml *html,
                                  const char *tag,
                                  gint tagsize,
                                  const char *attrname,
                                  DilloHtmlTagParsingFlags flags);
static char *Html_get_attr_wdef(DilloHtml *html,
                                const char *tag,
                                gint tagsize,
                                const char *attrname,
                                const char *def);
static void Html_add_widget(DilloHtml *html, DwWidget *widget,
                            char *width_str, char *height_str,
                            DwStyle *style_attrs);
static gint Html_write_raw(DilloHtml *html, char *buf, gint bufsize, gint Eof);
static void Html_write(DilloHtml *html, char *Buf, gint BufSize, gint Eof);
static void Html_close(DilloHtml *html, gint ClientKey);
static void Html_callback(int Op, CacheClient_t *Client);
static DilloHtml *Html_new(BrowserWindow *bw, const DilloUrl *url);
static void Html_tag_open_input(DilloHtml *html, char *tag, gint tagsize);
static void Html_add_input(DilloHtmlForm *form,
                           DilloHtmlInputType type,
                           GtkWidget *widget,
                           const char *name,
                           const char *init_str,
                           DilloHtmlSelect *select,
                           gboolean init_val);
static void Html_submit_form(GtkWidget *submit, DilloHtmlLB *html_lb,
                             gint click_x, gint click_y);
static void Html_reset_form(GtkWidget *reset, DilloHtmlLB *html_lb);
static gint Html_tag_index(char *tag);

/* exported function */
DwWidget *a_Html_text(const char *Type, void *P, CA_Callback_t *Call,
                      void **Data);


/*
 * Local Data
 */

/* The following array of font sizes has to be _strictly_ crescent */
static const gint FontSizes[] = {8, 10, 12, 14, 18, 24};
static const gint FontSizesNum = 6;
static const gint FontSizesBase = 2;

/* Parsing table structure */
typedef struct {
   gchar *name;           /* element name */
   unsigned char Flags;   /* flags (explained near the table data) */
   gchar EndTag;          /* Is it Required, Optional or Forbidden */
   guchar TagLevel;       /* Used to heuristically parse bad HTML  */
   TagOpenFunct open;     /* Open function */
   TagCloseFunct close;   /* Close function */
} TagInfo;
static const TagInfo Tags[];

/*
 * Return the line number of the tag being processed by the parser.
 */
static gint Html_get_line_number(DilloHtml *html)
{
   gint i, ofs, line;
   const char *p = html->Start_Buf;

   g_return_val_if_fail(p != NULL, -1);

   ofs = html->CurrTagOfs;
   line = html->OldTagLine;
   for (i = html->OldTagOfs; i < ofs; ++i)
      if (p[i] == '\n')
         ++line;
   html->OldTagOfs = html->CurrTagOfs;
   html->OldTagLine = line;
   return line;
}

/*
 * Collect HTML error strings inside the linkblock.
 */
static void Html_msg(DilloHtml *html, const char *format, ... )
{
   va_list argp;
   gchar buf[512];

   g_snprintf(buf, 512, "HTML warning: line %d, ",
              Html_get_line_number(html));
   g_string_append(html->linkblock->page_bugs, buf);
   va_start(argp, format);
   g_vsnprintf(buf, 512, format, argp);
   va_end(argp);
   g_string_append(html->linkblock->page_bugs, buf);
   a_Interface_bug_meter_update(html->bw,
                                ++html->linkblock->num_page_bugs);
}

/*
 * Wrapper for a_Url_new that adds an error detection message.
 * (if use_base_url is TRUE, html->linkblock->base_url is used)
 */
static DilloUrl *Html_url_new(DilloHtml *html,
                              const gchar *url_str, const gchar *base_url,
                              gint flags, gint32 posx, gint32 posy,
                              gint use_base_url)
{
   DilloUrl *url;
   gint n_ic;

   url = a_Url_new(
            url_str,
            (use_base_url) ? base_url : URL_STR_(html->linkblock->base_url),
            flags, posx, posy);
   if ((n_ic = URL_ILLEGAL_CHARS(url)) != 0)
      MSG_HTML("URL has %d illegal character%s [00-1F, 7F or space]\n",
               n_ic, (n_ic) > 1 ? "s" : "");
   return url;
}

/*
 * Set callback function and callback data for "html/text" MIME type.
 */
DwWidget *a_Html_text(const char *Type, void *P, CA_Callback_t *Call,
                      void **Data)
{
   DilloWeb *web = P;
   DilloHtml *html = Html_new(web->bw, web->url);

   *Data = (void *) html;
   *Call = (CA_Callback_t) Html_callback;

   return html->dw;
}

/*
 * We'll make the linkblock first to get it out of the way.
 */
static DilloHtmlLB *Html_lb_new(BrowserWindow *bw, const DilloUrl *url)
{
   DilloHtmlLB *html_lb = g_new(DilloHtmlLB, 1);

   html_lb->bw = bw;
   html_lb->base_url = a_Url_dup(url);
   html_lb->num_forms_max = 1;
   html_lb->num_forms = 0;
   html_lb->forms = NULL;

   html_lb->num_links_max = 1;
   html_lb->num_links = 0;
   html_lb->links = NULL;
   a_Dw_image_map_list_init(&html_lb->maps);

   html_lb->link_color = prefs.link_color;
   html_lb->visited_color = prefs.visited_color;

   html_lb->num_page_bugs = 0;
   html_lb->page_bugs = g_string_new("");

   return html_lb;
}

/*
 * Free the memory used by the linkblock
 */
static void Html_lb_free(void *lb)
{
   gint i, j, k;
   DilloHtmlForm *form;
   DilloHtmlLB *html_lb = lb;

   DEBUG_MSG(3, "Html_lb_free\n");

   a_Url_free(html_lb->base_url);

   for (i = 0; i < html_lb->num_forms; i++) {
      form = &html_lb->forms[i];
      a_Url_free(form->action);
      for (j = 0; j < form->num_inputs; j++) {
         g_free(form->inputs[j].name);
         g_free(form->inputs[j].init_str);

         if (form->inputs[j].type == DILLO_HTML_INPUT_SELECT ||
            form->inputs[j].type == DILLO_HTML_INPUT_SEL_LIST) {
            for (k = 0; k < form->inputs[j].select->num_options; k++) {
               g_free(form->inputs[j].select->options[k].value);
            }
            g_free(form->inputs[j].select->options);
            g_free(form->inputs[j].select);
         }
      }
      g_free(form->inputs);
   }
   g_free(html_lb->forms);

   for (i = 0; i < html_lb->num_links; i++)
      if (html_lb->links[i])
         a_Url_free(html_lb->links[i]);
   g_free(html_lb->links);

   a_Dw_image_map_list_free(&html_lb->maps);

   g_string_free(html_lb->page_bugs, TRUE);

   g_free(html_lb);
}


/*
 * Set the URL data for image maps.
 */
static void Html_set_link_coordinates(DilloHtmlLB *lb,
                                      gint link, gint x, gint y)
{
   gchar data[64];

   if (x != -1) {
      g_snprintf(data, 64, "?%d,%d", x, y);
      a_Url_set_ismap_coords(lb->links[link], data);
   }
}

/*
 * Handle the status function generated by the dw scroller,
 * and show the url in the browser status-bar.
 */
static void Html_handle_status(DwWidget *widget, gint link, gint x, gint y,
                               DilloHtmlLB *lb)
{
   DilloUrl *url;

   url = (link == -1) ? NULL : lb->links[link];
   if (url) {
      Html_set_link_coordinates(lb, link, x, y);
      a_Interface_msg(lb->bw, "%s",
                         URL_ALT_(url) ? URL_ALT_(url) : URL_STR_(url));
      a_Dw_widget_set_cursor (widget, Dw_cursor_hand);
      lb->bw->status_is_link = 1;

   } else {
      if (lb->bw->status_is_link)
         a_Interface_msg(lb->bw, "");
      a_Dw_widget_set_cursor (widget, NULL);
   }
}

/*
 * Popup the link menu ("link_pressed" callback of the page)
 */
static gboolean Html_link_menu(DwWidget *widget, gint link, gint x, gint y,
                               GdkEventButton *event, DilloHtmlLB *lb)
{
   DwWidget *widget_at_cursor;
   gboolean show_oi = FALSE;

   if (event->button == 3) {
      Html_set_link_coordinates(lb, link, x, y);
      a_Menu_popup_set_url(lb->bw, lb->links[link]);

      /* if we've got an image, prepare the image popup */
      widget_at_cursor =
         a_Dw_gtk_scrolled_window_widget_at_viewport_point(
            GTK_DW_SCROLLED_WINDOW (lb->bw->docwin), event->x, event->y);
      if (widget_at_cursor && DW_IS_IMAGE (widget_at_cursor)) {
         DwImage *image = DW_IMAGE (widget_at_cursor);
         /* test image->url (it may have not started to arrive yet!) */
         if (image->url) {
            /* use the second URL for this popup */
            gtk_object_set_data(GTK_OBJECT (lb->bw->menu_popup.over_image),
                                "url2", GINT_TO_POINTER(2));
            a_Menu_popup_set_url2(lb->bw, image->url);
            show_oi = TRUE;
         }
      }
      a_Menu_popup_ol_show_oi(lb->bw, show_oi);

      gtk_menu_popup(GTK_MENU(lb->bw->menu_popup.over_link), NULL, NULL,
                     NULL, NULL, event->button, event->time);
      return TRUE;
   }

   return FALSE;
}


/*
 * Activate a link ("link_clicked" callback of the page)
 */
static gboolean Html_link_clicked(DwWidget *widget, gint link, gint x, gint y,
                                  GdkEventButton *event, DilloHtmlLB *lb)
{
   Html_set_link_coordinates(lb, link, x, y);
   if (event->button == 1)
      a_Nav_push(lb->bw, lb->links[link]);
   else if (event->button == 2) {
      a_Nav_push_nw(lb->bw, lb->links[link]);
   } else {
      return FALSE;
   }

   if (DW_IS_PAGE (widget))
      a_Dw_page_change_link_color (DW_PAGE (widget), link, lb->visited_color);

   return TRUE;
}

/*
 * Popup the image menu ("button_press_event" callback of image)
 */
static gboolean Html_image_menu(DwWidget *widget,
                                gint32 x, gint32 y, GdkEventButton *event,
                                BrowserWindow *bw)
{
   DwImage *image = DW_IMAGE (widget);
   if (event->button == 3 && image->url) {
      a_Menu_popup_set_url(bw, image->url);
      a_Menu_popup_clear_url2(bw->menu_popup.over_image);

      gtk_menu_popup(GTK_MENU(bw->menu_popup.over_image), NULL, NULL,
                     NULL, NULL, event->button, event->time);
      return TRUE;
   }

   return FALSE;
}

/*
 * Popup the page menu ("button_press_event" callback of the viewport)
 */
static int Html_page_menu(GtkWidget *viewport, GdkEventButton *event,
                          BrowserWindow *bw)
{
   gpointer bug_pix;

   if (event->button == 3) {
      /* set the working URL */
      a_Menu_popup_set_url(bw, a_History_get_url(NAV_TOP(bw)));
      /* set "View page Bugs" sensitivity */
      bug_pix = gtk_object_get_data(GTK_OBJECT(bw->status_bug_meter), "bug");
      gtk_widget_set_sensitive(bw->viewbugs_menuitem,
                               GTK_WIDGET_VISIBLE(GTK_WIDGET(bug_pix)));
      gtk_menu_popup(GTK_MENU(bw->menu_popup.over_page), NULL, NULL,
                     NULL, NULL, event->button, event->time);
      return TRUE;
   } else
      return FALSE;
}

/*
 * Connect all signals of a page or an image.
 */
static void Html_connect_signals(DilloHtml *html, GtkObject *widget)
{
   gtk_signal_connect (widget, "link_entered",
                       GTK_SIGNAL_FUNC(Html_handle_status),
                       (gpointer)html->linkblock);
   gtk_signal_connect (widget, "link_pressed", GTK_SIGNAL_FUNC(Html_link_menu),
                       (gpointer)html->linkblock);
   gtk_signal_connect (widget, "link_clicked",
                       GTK_SIGNAL_FUNC(Html_link_clicked),
                       (gpointer)html->linkblock);
}


/*
 * Create a new link in the linkblock, set it as the url's parent
 * and return the index.
 */
static gint Html_set_new_link(DilloHtml *html, DilloUrl **url)
{
   gint nl;

   nl = html->linkblock->num_links;
   a_List_add(html->linkblock->links, nl, html->linkblock->num_links_max);
   html->linkblock->links[nl] = (*url) ? *url : NULL;
   return html->linkblock->num_links++;
}


/*
 * Check an integer value to be inside a range.
 * Return: 'n' if valid, 'def' if not.
 */
static int Html_check_int(int n, int min, int max, int def)
{
   return (n >= min && n <= max) ? n : def;
}

/*
 * Allocate and insert form information into the Html linkblock
 */
static gint Html_form_new(DilloHtmlLB *html_lb,
                          DilloHtmlMethod method,
                          const DilloUrl *action,
                          DilloHtmlEnc enc)
{
   gint nf;

   a_List_add(html_lb->forms, html_lb->num_forms, html_lb->num_forms_max);

   nf = html_lb->num_forms;
   html_lb->forms[nf].method = method;
   html_lb->forms[nf].action = a_Url_dup(action);
   html_lb->forms[nf].enc = enc;
   html_lb->forms[nf].num_inputs = 0;
   html_lb->forms[nf].num_inputs_max = 4;
   html_lb->forms[nf].inputs = NULL;
   html_lb->forms[nf].num_entry_fields = 0;
   html_lb->forms[nf].num_submit_buttons = 0;
   html_lb->num_forms++;

   _MSG("Html_form_new: action=%s nform=%d\n", action, nf);
   return nf;
}


/*
 * Change one toplevel attribute. var should be an identifier. val is
 * only evaluated once, so you can safely use a function call for it.
 */
#define HTML_SET_TOP_ATTR(html, var, val) \
   do { \
      DwStyle style_attrs, *old_style; \
       \
      old_style = (html)->stack[(html)->stack_top].style; \
      style_attrs = *old_style; \
      style_attrs.var = (val); \
      (html)->stack[(html)->stack_top].style = \
         a_Dw_style_new (&style_attrs, (html)->bw->main_window->window); \
      a_Dw_style_unref (old_style); \
   } while (FALSE)

/*
 * Set the font at the top of the stack. BImask specifies which
 * attributes in BI should be changed.
 */
static void Html_set_top_font(DilloHtml *html, gchar *name, gint size,
                              gint BI, gint BImask)
{
   DwStyleFont font_attrs;

   font_attrs = *html->stack[(html)->stack_top].style->font;
   if ( name )
      font_attrs.name = name;
   if ( size )
      font_attrs.size = size;
   if ( BImask & 1 )
      font_attrs.weight   = (BI & 1) ? 700 : 400;
   if ( BImask & 2 )
      font_attrs.style = (BI & 2) ?
         (prefs.use_oblique ?
          DW_STYLE_FONT_STYLE_OBLIQUE : DW_STYLE_FONT_STYLE_ITALIC) :
         DW_STYLE_FONT_STYLE_NORMAL;

   HTML_SET_TOP_ATTR (html, font, a_Dw_style_font_new (&font_attrs));
}

/*
 * Evaluates the ALIGN attribute (left|center|right|justify) and
 * sets the style at the top of the stack.
 */
static void Html_tag_set_align_attr(DilloHtml *html, char *tag, gint tagsize)
{
   const char *align, *charattr;

   if ((align = Html_get_attr(html, tag, tagsize, "align"))) {
      if (g_strcasecmp (align, "left") == 0)
         HTML_SET_TOP_ATTR (html, text_align, DW_STYLE_TEXT_ALIGN_LEFT);
      else if (g_strcasecmp (align, "right") == 0)
         HTML_SET_TOP_ATTR (html, text_align, DW_STYLE_TEXT_ALIGN_RIGHT);
      else if (g_strcasecmp (align, "center") == 0)
         HTML_SET_TOP_ATTR (html, text_align, DW_STYLE_TEXT_ALIGN_CENTER);
      else if (g_strcasecmp (align, "justify") == 0)
         HTML_SET_TOP_ATTR (html, text_align, DW_STYLE_TEXT_ALIGN_JUSTIFY);
      else if (g_strcasecmp (align, "char") == 0) {
         /* todo: Actually not supported for <p> etc. */
         HTML_SET_TOP_ATTR (html, text_align, DW_STYLE_TEXT_ALIGN_STRING);
         if ((charattr = Html_get_attr(html, tag, tagsize, "char"))) {
            if (charattr[0] == 0)
               /* todo: ALIGN=" ", and even ALIGN="&32;" will reult in
                * an empty string (don't know whether the latter is
                * correct, has to be clarified with the specs), so
                * that for empty strings, " " is assumed. */
               HTML_SET_TOP_ATTR (html, text_align_char, ' ');
            else
               HTML_SET_TOP_ATTR (html, text_align_char, charattr[0]);
         } else
            /* todo: Examine LANG attr of <html>. */
            HTML_SET_TOP_ATTR (html, text_align_char, '.');
      }
   }
}

/*
 * Evaluates the VALIGN attribute (top|bottom|middle|baseline) and
 * sets the style in style_attrs. Returns TRUE when set.
 */
static gboolean Html_tag_set_valign_attr(DilloHtml *html, char *tag,
                                         gint tagsize, DwStyle *style_attrs)
{
   const char *attr;

   if ((attr = Html_get_attr(html, tag, tagsize, "valign"))) {
      if (g_strcasecmp (attr, "top") == 0)
         style_attrs->valign = DW_STYLE_VALIGN_TOP;
      else if (g_strcasecmp (attr, "bottom") == 0)
         style_attrs->valign = DW_STYLE_VALIGN_BOTTOM;
      else if (g_strcasecmp (attr, "baseline") == 0)
         style_attrs->valign = DW_STYLE_VALIGN_BASELINE;
      else
         style_attrs->valign = DW_STYLE_VALIGN_MIDDLE;
      return TRUE;
   } else
      return FALSE;
}


/*
 * Add a new DwPage into the current DwPage, for indentation.
 * left and right are the horizontal indentation amounts, space is the
 * vertical space around the block.
 */
static void Html_add_indented_widget(DilloHtml *html, DwWidget *page,
                                     int left, int right, int space)
{
   DwStyle style_attrs, *style;

   style_attrs = *html->stack[html->stack_top].style;

   a_Dw_style_box_set_val(&style_attrs.margin, 0);
   a_Dw_style_box_set_val(&style_attrs.border_width, 0);
   a_Dw_style_box_set_val(&style_attrs.padding, 0);

   /* Activate this for debugging */
#if 0
   a_Dw_style_box_set_val(&style_attrs.border_width, 1);
   a_Dw_style_box_set_border_color
      (&style_attrs,
       a_Dw_style_shaded_color_new(style_attrs.color->color_val,
                                   html->bw->main_window->window));
   a_Dw_style_box_set_border_style(&style_attrs, DW_STYLE_BORDER_DASHED);
#endif

   style_attrs.margin.left = left;
   style_attrs.margin.right = right;
   style = a_Dw_style_new (&style_attrs, html->bw->main_window->window);

   a_Dw_page_add_parbreak (DW_PAGE (html->dw), space, style);
   a_Dw_page_add_widget (DW_PAGE (html->dw), page, style);
   a_Dw_page_add_parbreak (DW_PAGE (html->dw), space, style);
   html->stack[html->stack_top].page = html->dw = page;
   html->stack[html->stack_top].hand_over_break = TRUE;
   a_Dw_style_unref (style);

   /* Handle it when the user clicks on a link */
   Html_connect_signals(html, GTK_OBJECT(page));
}

/*
 * Create and add a new indented DwPage to the current DwPage
 */
static void Html_add_indented(DilloHtml *html, int left, int right, int space)
{
   DwWidget *page = a_Dw_page_new ();
   Html_add_indented_widget (html, page, left, right, space);
}

/*
 * Given a font_size, this will return the correct 'level'.
 * (or the closest, if the exact level isn't found).
 */
static gint Html_fontsize_to_level(gint fontsize)
{
   gint i, level;
   gdouble normalized_size = fontsize / prefs.font_factor,
           approximation   = FontSizes[FontSizesNum-1] + 1;

   for (i = level = 0; i < FontSizesNum; i++)
      if (approximation >= fabs(normalized_size - FontSizes[i])) {
         approximation = fabs(normalized_size - FontSizes[i]);
         level = i;
      } else {
         break;
      }

   return level;
}

/*
 * Given a level of a font, this will return the correct 'size'.
 */
static gint Html_level_to_fontsize(gint level)
{
   level = MAX(0, level);
   level = MIN(FontSizesNum - 1, level);

   return rint(FontSizes[level]*prefs.font_factor);
}

/*
 * Miscelaneous initializations for a DwPage
 */
static void Html_set_dwpage(DilloHtml *html)
{
   DwWidget *widget;
   DwPage *page;
   DwStyle style_attrs;
   DwStyleFont font;

   g_return_if_fail (html->dw == NULL);

   widget = a_Dw_page_new ();
   page = DW_PAGE (widget);
   html->dw = html->stack[0].page = widget;

   /* Create a dummy font, attribute, and tag for the bottom of the stack. */
   font.name = prefs.vw_fontname; /* Helvetica */
   font.size = Html_level_to_fontsize(FontSizesBase);
   font.weight = 400;
   font.style = DW_STYLE_FONT_STYLE_NORMAL;

   a_Dw_style_init_values (&style_attrs, html->bw->main_window->window);
   style_attrs.font = a_Dw_style_font_new (&font);
   style_attrs.color = a_Dw_style_color_new (prefs.text_color,
                                             html->bw->main_window->window);
   html->stack[0].style = a_Dw_style_new (&style_attrs,
                                          html->bw->main_window->window);

   html->stack[0].table_cell_style = NULL;

   /* Handle it when the user clicks on a link */
   Html_connect_signals(html, GTK_OBJECT(widget));

   gtk_signal_connect_while_alive (
      GTK_OBJECT(GTK_BIN(html->bw->docwin)->child), "button_press_event",
      GTK_SIGNAL_FUNC(Html_page_menu), (gpointer)html->bw, GTK_OBJECT (page));

   /* Connect the "bug meter" button-press to the linkblock */
   gtk_signal_connect_while_alive(
      GTK_OBJECT (html->bw->status_bug_meter), "clicked",
      GTK_SIGNAL_FUNC (a_Commands_view_page_bugs_callback),
      (gpointer)html->linkblock, GTK_OBJECT (page));
   gtk_signal_connect_while_alive(
      GTK_OBJECT (html->bw->status_bug_meter), "clicked1",
      GTK_SIGNAL_FUNC (a_Commands_view_page_bugs_callback),
      (gpointer)html->linkblock, GTK_OBJECT (page));
   /* also connect with the "View page Bugs" menuitem */
   gtk_signal_connect_while_alive(
      GTK_OBJECT (html->bw->viewbugs_menuitem), "activate",
      GTK_SIGNAL_FUNC (a_Commands_view_page_bugs_callback),
      (gpointer)html->linkblock, GTK_OBJECT (page));

   /* Destroy the linkblock when the DwPage is destroyed */
   gtk_signal_connect_object(GTK_OBJECT(page), "destroy",
                             GTK_SIGNAL_FUNC(Html_lb_free),
                             (gpointer)html->linkblock);
}

/*
 * Create and initialize a new DilloHtml structure
 */
static DilloHtml *Html_new(BrowserWindow *bw, const DilloUrl *url)
{
   DilloHtml *html;

   html = g_new(DilloHtml, 1);

   html->Start_Buf = NULL;
   html->Start_Ofs = 0;
   html->CurrTagOfs = 0;
   html->OldTagOfs = 0;
   html->OldTagLine = 1;

   html->dw = NULL;
   html->bw = bw;
   html->linkblock = Html_lb_new(bw, url);

   html->stack_max = 16;
   html->stack_top = 0;
   html->stack = g_new(DilloHtmlState, html->stack_max);
   html->stack[0].tag_name = g_strdup("none");
   html->stack[0].style = NULL;
   html->stack[0].table_cell_style = NULL;
   html->stack[0].parse_mode = DILLO_HTML_PARSE_MODE_INIT;
   html->stack[0].table_mode = DILLO_HTML_TABLE_MODE_NONE;
   html->stack[0].cell_text_align_set = FALSE;
   html->stack[0].list_type = HTML_LIST_NONE; /* no <ul> or <ol> open */
   html->stack[0].list_number = 0;
   html->stack[0].tag_idx = -1;               /* MUST not be used */
   html->stack[0].page = NULL;
   html->stack[0].table = NULL;
   html->stack[0].ref_list_item = NULL;
   html->stack[0].current_bg_color = prefs.bg_color;
   html->stack[0].hand_over_break = FALSE;

   html->Stash = g_string_new("");
   html->StashSpace = FALSE;

   html->SPCBuf = NULL;

   html->pre_column = 0;
   html->PreFirstChar = FALSE;
   html->PrevWasCR = FALSE;
   html->PrevWasOpenTag = FALSE;
   html->SPCPending = FALSE;
   html->InVisitedLink = FALSE;
   html->ReqTagClose = FALSE;
   html->CloseOneTag = FALSE;
   html->TagSoup = TRUE;
   html->NameVal = NULL;

   html->Num_HTML = html->Num_HEAD = html->Num_BODY = html->Num_TITLE = 0;

   html->InFlags = 0;

   html->attr_data = g_string_sized_new(1024);

   Html_set_dwpage(html);

   return html;
}

/*
 * Initialize the stash buffer
 */
static void Html_stash_init(DilloHtml *html)
{
   html->stack[html->stack_top].parse_mode = DILLO_HTML_PARSE_MODE_STASH;
   html->StashSpace = FALSE;
   g_string_truncate(html->Stash, 0);
}

/* Entities list from the HTML 4.01 DTD */
typedef struct {
   char *entity;
   int isocode;
} Ent_t;

#define NumEnt 252
static const Ent_t Entities[NumEnt] = {
   {"AElig",0306}, {"Aacute",0301}, {"Acirc",0302},  {"Agrave",0300},
   {"Alpha",01621},{"Aring",0305},  {"Atilde",0303}, {"Auml",0304},
   {"Beta",01622}, {"Ccedil",0307}, {"Chi",01647},   {"Dagger",020041},
   {"Delta",01624},{"ETH",0320},    {"Eacute",0311}, {"Ecirc",0312},
   {"Egrave",0310},{"Epsilon",01625},{"Eta",01627},  {"Euml",0313},
   {"Gamma",01623},{"Iacute",0315}, {"Icirc",0316},  {"Igrave",0314},
   {"Iota",01631}, {"Iuml",0317},   {"Kappa",01632}, {"Lambda",01633},
   {"Mu",01634},   {"Ntilde",0321}, {"Nu",01635},    {"OElig",0522},
   {"Oacute",0323},{"Ocirc",0324},  {"Ograve",0322}, {"Omega",01651},
   {"Omicron",01637},{"Oslash",0330},{"Otilde",0325},{"Ouml",0326},
   {"Phi",01646},  {"Pi",01640},    {"Prime",020063},{"Psi",01650},
   {"Rho",01641},  {"Scaron",0540}, {"Sigma",01643}, {"THORN",0336},
   {"Tau",01644},  {"Theta",01630}, {"Uacute",0332}, {"Ucirc",0333},
   {"Ugrave",0331},{"Upsilon",01645},{"Uuml",0334},  {"Xi",01636},
   {"Yacute",0335},{"Yuml",0570},   {"Zeta",01626},  {"aacute",0341},
   {"acirc",0342}, {"acute",0264},  {"aelig",0346},  {"agrave",0340},
   {"alefsym",020465},{"alpha",01661},{"amp",38},    {"and",021047},
   {"ang",021040}, {"aring",0345},  {"asymp",021110},{"atilde",0343},
   {"auml",0344},  {"bdquo",020036},{"beta",01662},  {"brvbar",0246},
   {"bull",020042},{"cap",021051},  {"ccedil",0347}, {"cedil",0270},
   {"cent",0242},  {"chi",01707},   {"circ",01306},  {"clubs",023143},
   {"cong",021105},{"copy",0251},   {"crarr",020665},{"cup",021052},
   {"curren",0244},{"dArr",020723}, {"dagger",020040},{"darr",020623},
   {"deg",0260},   {"delta",01664}, {"diams",023146},{"divide",0367},
   {"eacute",0351},{"ecirc",0352},  {"egrave",0350}, {"empty",021005},
   {"emsp",020003},{"ensp",020002}, {"epsilon",01665},{"equiv",021141},
   {"eta",01667},  {"eth",0360},    {"euml",0353},   {"euro",020254},
   {"exist",021003},{"fnof",0622},  {"forall",021000},{"frac12",0275},
   {"frac14",0274},{"frac34",0276}, {"frasl",020104},{"gamma",01663},
   {"ge",021145},  {"gt",62},       {"hArr",020724}, {"harr",020624},
   {"hearts",023145},{"hellip",020046},{"iacute",0355},{"icirc",0356},
   {"iexcl",0241}, {"igrave",0354}, {"image",020421},{"infin",021036},
   {"int",021053}, {"iota",01671},  {"iquest",0277}, {"isin",021010},
   {"iuml",0357},  {"kappa",01672}, {"lArr",020720}, {"lambda",01673},
   {"lang",021451},{"laquo",0253},  {"larr",020620}, {"lceil",021410},
   {"ldquo",020034},{"le",021144},  {"lfloor",021412},{"lowast",021027},
   {"loz",022712}, {"lrm",020016},  {"lsaquo",020071},{"lsquo",020030},
   {"lt",60},      {"macr",0257},   {"mdash",020024},{"micro",0265},
   {"middot",0267},{"minus",021022},{"mu",01674},    {"nabla",021007},
   {"nbsp",32},    {"ndash",020023},{"ne",021140},   {"ni",021013},
   {"not",0254},   {"notin",021011},{"nsub",021204}, {"ntilde",0361},
   {"nu",01675},   {"oacute",0363}, {"ocirc",0364},  {"oelig",0523},
   {"ograve",0362},{"oline",020076},{"omega",01711}, {"omicron",01677},
   {"oplus",021225},{"or",021050},  {"ordf",0252},   {"ordm",0272},
   {"oslash",0370},{"otilde",0365}, {"otimes",021227},{"ouml",0366},
   {"para",0266},  {"part",021002}, {"permil",020060},{"perp",021245},
   {"phi",01706},  {"pi",01700},    {"piv",01726},   {"plusmn",0261},
   {"pound",0243}, {"prime",020062},{"prod",021017}, {"prop",021035},
   {"psi",01710},  {"quot",34},     {"rArr",020722}, {"radic",021032},
   {"rang",021452},{"raquo",0273},  {"rarr",020622}, {"rceil",021411},
   {"rdquo",020035},{"real",020434},{"reg",0256},    {"rfloor",021413},
   {"rho",01701},  {"rlm",020017},  {"rsaquo",020072},{"rsquo",020031},
   {"sbquo",020032},{"scaron",0541},{"sdot",021305}, {"sect",0247},
   {"shy",0255},   {"sigma",01703}, {"sigmaf",01702},{"sim",021074},
   {"spades",023140},{"sub",021202},{"sube",021206}, {"sum",021021},
   {"sup",021203}, {"sup1",0271},   {"sup2",0262},   {"sup3",0263},
   {"supe",021207},{"szlig",0337},  {"tau",01704},   {"there4",021064},
   {"theta",01670},{"thetasym",01721},{"thinsp",020011},{"thorn",0376},
   {"tilde",01334},{"times",0327},  {"trade",020442},{"uArr",020721},
   {"uacute",0372},{"uarr",020621}, {"ucirc",0373},  {"ugrave",0371},
   {"uml",0250},   {"upsih",01722}, {"upsilon",01705},{"uuml",0374},
   {"weierp",020430},{"xi",01676},  {"yacute",0375}, {"yen",0245},
   {"yuml",0377},  {"zeta",01666},  {"zwj",020015},  {"zwnj",020014}
};


/*
 * Comparison function for binary search
 */
static int Html_entity_comp(const void *a, const void *b)
{
   return strcmp(((Ent_t *)a)->entity, ((Ent_t *)b)->entity);
}

/*
 * Binary search of 'key' in entity list
 */
static int Html_entity_search(char *key)
{
   Ent_t *res, EntKey;

   EntKey.entity = key;
   res = bsearch(&EntKey, Entities, NumEnt, sizeof(Ent_t), Html_entity_comp);
   if ( res )
     return (res - Entities);
   return -1;
}

/*
 * Switch a few UCS encodings to latin1.
 */
static gint Html_try_ucs2latin1(gint isocode)
{
   gint ret;
   switch (isocode) {
      case 0x2018:
      case 0x2019: ret = '\''; break;
      case 0x201c:
      case 0x201d: ret = '"'; break;
      case 0x2013:
      case 0x2014: ret = '-'; break;
      case 0x2039: ret = '<'; break;
      case 0x203a: ret = '>'; break;
      case 0x2022: ret = 176; break;
      default:     ret = -1;  break;
   }
   return ret;
}

/*
 * Switch a few 'undefined for HTML' ASCII encodings to latin1.
 */
static gint Html_try_ascii2latin1(gint isocode)
{
   gint ret;
   switch (isocode) {
      case 145:
      case 146: ret = '\''; break;
      case 147:
      case 148: ret = '"'; break;
      case 149: ret = 176; break;
      case 150:
      case 151: ret = '-'; break;
      default:  ret = isocode; break;
   }
   return ret;
}

/*
 * Given an entity, return the ISO-Latin1 character code.
 * Returns a negative value (error code) if not a valid entity.
 *
 * The first character *token is assumed to be == '&'
 *
 * For valid entities, *entsize is set to the length of the parsed entity.
 */
static gint Html_parse_entity(DilloHtml *html, const gchar *token,
                              gint toksize, gint *entsize)
{
   gint isocode, i;
   gchar *tok, *s, c;

   token++;
   tok = s = toksize ? g_strndup(token, (guint)toksize) : g_strdup(token);

   isocode = -1;

   if (*s == '#') {
      /* numeric character reference */
      errno = 0;
      if (*++s == 'x' || *s == 'X') {
         if (isxdigit(*++s)) {
            /* strtol with base 16 accepts leading "0x" - we don't */
            if (*s == '0' && s[1] == 'x') {
               s++;
               isocode = 0; 
            } else {
               isocode = strtol(s, &s, 16);
            }
         }
      } else if (isdigit(*s)) {
         isocode = strtol(s, &s, 10);
      }

      if (!isocode || errno || isocode > 0x7fffffffL) {
         /* this catches null bytes, errors and codes >=2^31 */
         MSG_HTML("numeric character reference out of range\n");
         isocode = -2;
      }

      if (isocode != -1) {
         if (*s == ';')
            s++;
         else if (prefs.show_extra_warnings)
            MSG_HTML("numeric character reference without trailing ';'\n");
      }

   } else if (isalpha(*s)) {
      /* character entity reference */
      while (isalnum(*++s) || strchr(":_.-", *s));
      c = *s;
      *s = 0;

      if ((i = Html_entity_search(tok)) == -1) {
         MSG_HTML("undefined character entity '%s'\n", tok);
         isocode = -3;
      } else
         isocode = Entities[i].isocode;

      if (c == ';')
         s++;
      else if (prefs.show_extra_warnings)
         MSG_HTML("character entity reference without trailing ';'\n");
   }

   *entsize = s-tok+1;
   g_free(tok);

   if (isocode >= 128 && isocode <= 159) {
      MSG_HTML("code positions 128-159 are not defined for ISO Latin-1\n");
      isocode = Html_try_ascii2latin1(isocode);
   } else if (isocode  > 255)
      /* Try a few UCS translations to Latin1 */
      isocode = Html_try_ucs2latin1(isocode);
   else if (isocode == -1 && prefs.show_extra_warnings)
      MSG_HTML("literal '&'\n");

   return isocode;
}

/*
 * Convert all the entities in a token to plain ISO character codes. Takes
 * a token and its length, and returns a newly allocated string.
 */
static char *
 Html_parse_entities(DilloHtml *html, gchar *token, gint toksize)
{
   gchar *esc_set = "&\xE2\xC2";
   gchar *new_str;
   gint i, j, isocode, entsize;

   new_str = g_strndup(token, toksize);
   if (new_str[strcspn(new_str, esc_set)] == 0)
      return new_str;

   for (i = j = 0; i < toksize; i++) {
      if (token[i] == '&' &&
          (isocode = Html_parse_entity(html, token+i,
                                       toksize-i, &entsize)) >= 0) {
         new_str[j++] = (gchar) isocode;
         i += entsize-1;

      } else if (token[i] == '\xE2' && token[i+1] == '\x80' && i+2 < toksize){
         /* Hack: for parsing some UTF-8 characters into latin1 */
         switch (token[i+2]) {
         case '\x94':
            new_str[j++] = '-';
            new_str[j++] = '-';
            break;
         case '\x98':
         case '\x99':
            new_str[j++] = '\'';
            break;
         case '\x9C':
         case '\x9D':
            new_str[j++] = '"';
            break;
         case '\xA2':
            new_str[j++] = '*';
            new_str[j++] = ' ';
            break;
         default: /* unhandled */
            new_str[j++] = '\xE2';
            break;
         }
         i += 2;

      } else if (token[i] == '\xC2' && token[i+1] == '\xA0') {
         /* Hack: for parsing some UTF-8 characters into latin1 */
         new_str[j++] = ' ';
         ++i;

      } else {
         new_str[j++] = token[i];
      }
   }
   new_str[j] = '\0';
   return new_str;
}

/*
 * Parse spaces
 *
 */
static void Html_process_space(DilloHtml *html, char *space, gint spacesize)
{
   gint i, offset;
   DilloHtmlParseMode parse_mode = html->stack[html->stack_top].parse_mode;

   if ( parse_mode == DILLO_HTML_PARSE_MODE_STASH ) {
      html->StashSpace = (html->Stash->len > 0);
      html->SPCPending = FALSE;

   } else if ( parse_mode == DILLO_HTML_PARSE_MODE_VERBATIM ) {
      char *Pword = g_strndup(space, spacesize);
      g_string_append(html->Stash, Pword);
      g_free(Pword);
      html->SPCPending = FALSE;

   } else if ( parse_mode == DILLO_HTML_PARSE_MODE_PRE ) {
      /* re-scan the string for characters that cause line breaks */
      for (i = 0; i < spacesize; i++) {
         /* Support for "\r", "\n" and "\r\n" line breaks (skips the first) */
         if (!html->PreFirstChar &&
             (space[i] == '\r' || (space[i] == '\n' && !html->PrevWasCR))) {
            a_Dw_page_add_linebreak(DW_PAGE (html->dw),
                                    html->stack[(html)->stack_top].style);
            html->pre_column = 0;
         }
         html->PreFirstChar = FALSE;

         /* cr and lf should not be rendered -- they appear as a break */
         switch (space[i]) {
         case '\r':
         case '\n':
            break;
         case '\t':
            if (prefs.show_extra_warnings)
               MSG_HTML("TAB character inside <PRE>\n");
            offset = TAB_SIZE - html->pre_column % TAB_SIZE;
            a_Dw_page_add_text(DW_PAGE (html->dw),
                               g_strnfill(offset, ' '),
                               html->stack[html->stack_top].style);
            html->pre_column += offset;
            break;
         default:
            a_Dw_page_add_text(DW_PAGE (html->dw),
                               g_strndup(space + i, 1),
                               html->stack[html->stack_top].style);
            html->pre_column++;
            break;
         }

         html->PrevWasCR = (space[i] == '\r');
      }
      html->SPCPending = FALSE;

   } else {
      if (SGML_SPCDEL && html->PrevWasOpenTag) {
         /* SGML_SPCDEL ignores white space inmediately after an open tag */
         html->SPCPending = FALSE;
      } else {
         g_free(html->SPCBuf);
         html->SPCBuf = g_strndup(space, spacesize);
         html->SPCPending = TRUE;
      }

      if ( parse_mode == DILLO_HTML_PARSE_MODE_STASH_AND_BODY )
         html->StashSpace = (html->Stash->len > 0);
   }
}

/*
 * Handles putting the word into its proper place
 *  > STASH and VERBATIM --> html->Stash
 *  > otherwise it goes through a_Dw_page_add_text()
 *
 * Entities are parsed (or not) according to parse_mode.
 */
static void Html_process_word(DilloHtml *html, char *word, gint size)
{
   gint i, start;
   gchar *Pword;
   DilloHtmlParseMode parse_mode = html->stack[html->stack_top].parse_mode;

   if ( parse_mode == DILLO_HTML_PARSE_MODE_STASH ||
        parse_mode == DILLO_HTML_PARSE_MODE_STASH_AND_BODY ) {
      if ( html->StashSpace ) {
         g_string_append_c(html->Stash, ' ');
         html->StashSpace = FALSE;
      }
      Pword = Html_parse_entities(html, word, size);
      g_string_append(html->Stash, Pword);
      g_free(Pword);

   } else if ( parse_mode == DILLO_HTML_PARSE_MODE_VERBATIM ) {
      /* word goes in untouched, it is not processed here. */
      Pword = g_strndup(word, size);
      g_string_append(html->Stash, Pword);
      g_free(Pword);
   }

   if ( parse_mode == DILLO_HTML_PARSE_MODE_STASH  ||
        parse_mode == DILLO_HTML_PARSE_MODE_VERBATIM ) {
      /* skip until the closing instructions */

   } else if ( parse_mode == DILLO_HTML_PARSE_MODE_PRE ) {
      /* all this overhead is to catch white-space entities */
      Pword = Html_parse_entities(html, word, size);
      for (start = i = 0; Pword[i]; start = i)
         if (isspace(Pword[i])) {
            while (Pword[++i] && isspace(Pword[i]));
            Html_process_space(html, Pword + start, i - start);
         } else {
            while (Pword[++i] && !isspace(Pword[i]));
            a_Dw_page_add_text(DW_PAGE (html->dw),
                               g_strndup(Pword + start, i - start),
                               html->stack[html->stack_top].style);
            html->pre_column += i - start;
            html->PreFirstChar = FALSE;
         }
      g_free(Pword);

   } else {
      /* add pending space if present */
      if (html->SPCPending && (!SGML_SPCDEL || !html->PrevWasOpenTag))
         /* SGML_SPCDEL ignores space after an open tag */
         a_Dw_page_add_space(DW_PAGE (html->dw),
                             html->stack[html->stack_top].style);

      /* actually white-space entities inside the word could be
       * collapsed (except &nbsp;), but that's too much overhead
       * for a very rare case of ill-formed HTML  --Jcid */

      Pword = Html_parse_entities(html, word, size);
      g_strdelimit(Pword, "\t\f\n\r", ' ');
      a_Dw_page_add_text(DW_PAGE (html->dw),
                         Pword,
                         html->stack[html->stack_top].style);
   }

   html->PrevWasOpenTag = FALSE;
   html->SPCPending = FALSE;
}

/*
 * Does the tag in tagstr (e.g. "p") match the tag in the tag, tagsize
 * structure, with the initial < skipped over (e.g. "P align=center>")
 */
static gboolean Html_match_tag(const char *tagstr, char *tag, gint tagsize)
{
   gint i;

   for (i = 0; i < tagsize && tagstr[i] != '\0'; i++) {
      if (tolower(tagstr[i]) != tolower(tag[i]))
         return FALSE;
   }
   /* The test for '/' is for xml compatibility: "empty/>" will be matched. */
   if (i < tagsize && (isspace(tag[i]) || tag[i] == '>' || tag[i] == '/'))
      return TRUE;
   return FALSE;
}

/*
 * This function is called after popping the stack, to
 * handle nested DwPage widgets.
 */
static void Html_eventually_pop_dw(DilloHtml *html)
{
   /* This function is called after popping from the stack, so the
    * relevant hand_over_break is at html->stack_top + 1. */
   if (html->dw != html->stack[html->stack_top].page) {
      if (html->stack[html->stack_top + 1].hand_over_break)
         a_Dw_page_hand_over_break(DW_PAGE(html->dw),
                                   html->stack[(html)->stack_top].style);
      a_Dw_page_flush(DW_PAGE(html->dw));
      html->dw = html->stack[html->stack_top].page;
   }
}

/*
 * Push the tag (copying attributes from the top of the stack)
 */
static void Html_push_tag(DilloHtml *html, gint tag_idx)
{
   char *tagstr;
   gint n_items;

   /* Save the element's name (no parameters) into tagstr. */
   tagstr = g_strdup(Tags[tag_idx].name);

   n_items = html->stack_top + 1;
   a_List_add(html->stack, n_items, html->stack_max);
   /* We'll copy the former stack item and just change the tag and its index
    * instead of copying all fields except for tag.  --Jcid */
   html->stack[n_items] = html->stack[n_items - 1];
   html->stack[n_items].tag_name = tagstr;
   html->stack[n_items].tag_idx = tag_idx;
   html->stack_top = n_items;
   /* proper memory management, may be unref'd later */
   a_Dw_style_ref (html->stack[html->stack_top].style);
   if (html->stack[html->stack_top].table_cell_style)
      a_Dw_style_ref (html->stack[html->stack_top].table_cell_style);
   html->dw = html->stack[html->stack_top].page;
}

/*
 * Push the tag (used to force en element with optional open into the stack)
 * Note: now it's the same as Html_push_tag(), but things may change...
 */
static void Html_force_push_tag(DilloHtml *html, gint tag_idx)
{
   Html_push_tag(html, tag_idx);
}

/*
 * Pop the top tag in the stack
 */
static void Html_real_pop_tag(DilloHtml *html)
{
   a_Dw_style_unref (html->stack[html->stack_top].style);
   if (html->stack[html->stack_top].table_cell_style)
      a_Dw_style_unref (html->stack[html->stack_top].table_cell_style);
   g_free(html->stack[html->stack_top--].tag_name);
   Html_eventually_pop_dw(html);
}

/*
 * Default close function for tags.
 * (conditional cleanup of the stack)
 * There're several ways of doing it. Considering the HTML 4.01 spec
 * which defines optional close tags, and the will to deliver useful diagnose
 * messages for bad-formed HTML, it'll go as follows:
 *   1.- Search the stack for the first tag that requires a close tag.
 *   2.- If it matches, clean all the optional-close tags in between.
 *   3.- Cleanup the matching tag. (on error, give a warning message)
 *
 * If 'w3c_mode' is NOT enabled:
 *   1.- Search the stack for a matching tag based on tag level.
 *   2.- If it exists, clean all the tags in between.
 *   3.- Cleanup the matching tag. (on error, give a warning message)
 */
static void Html_tag_cleanup_at_close(DilloHtml *html, gint TagIdx)
{
   gint w3c_mode = !prefs.w3c_plus_heuristics;
   gint stack_idx, cmp = 1;
   gint new_idx = TagIdx;

   if (html->CloseOneTag) {
      Html_real_pop_tag(html);
      html->CloseOneTag = FALSE;
      return;
   }

   /* Look for the candidate tag to close */
   stack_idx = html->stack_top;
   while (stack_idx &&
          (cmp = (new_idx != html->stack[stack_idx].tag_idx)) &&
          ((w3c_mode &&
            Tags[html->stack[stack_idx].tag_idx].EndTag == 'O') ||
           (!w3c_mode &&
            Tags[html->stack[stack_idx].tag_idx].TagLevel <
            Tags[new_idx].TagLevel))) {
      --stack_idx;
   }

   /* clean, up to the matching tag */
   if (cmp == 0 && stack_idx > 0) {
      /* There's a valid matching tag in the stack */
      while (html->stack_top >= stack_idx) {
         gint toptag_idx = html->stack[html->stack_top].tag_idx;
         /* Warn when we decide to close an open tag (for !w3c_mode) */
         if (html->stack_top > stack_idx &&
             Tags[toptag_idx].EndTag != 'O')
            MSG_HTML("  - forcing close of open tag: <%s>\n",
                     Tags[toptag_idx].name);

         /* Close this and only this tag */
         html->CloseOneTag = TRUE;
         Tags[toptag_idx].close (html, toptag_idx);
      }

   } else {
      MSG_HTML("unexpected closing tag: </%s>. -- expected </%s>\n",
               Tags[new_idx].name, html->stack[stack_idx].tag_name);
   }
}

/*
 * Cleanup (conditional), and Pop the tag (if it matches)
 */
static void Html_pop_tag(DilloHtml *html, gint TagIdx)
{
   Html_tag_cleanup_at_close(html, TagIdx);
}

/*
 * Some parsing routines.
 */

/*
 * Used by Html_parse_length
 */
static DwStyleLength Html_parse_length_or_multi_length (const gchar *attr,
                                                        gchar **endptr)
{
   DwStyleLength l;
   double v;
   gchar *end;

   v = strtod (attr, &end);
   switch (*end) {
   case '%':
      end++;
      l = DW_STYLE_CREATE_PER_LENGTH (v / 100);
      break;

   case '*':
      end++;
      l = DW_STYLE_CREATE_REL_LENGTH (v);
      break;
/*
   The "px" suffix seems not allowed by HTML4.01 SPEC.
   case 'p':
      if (end[1] == 'x')
         end += 2;
*/
   default:
      l = DW_STYLE_CREATE_ABS_LENGTH ((gint)v);
      break;
   }

   if (endptr)
      *endptr = end;
   return l;
}


/*
 * Returns a length or a percentage, or DW_STYLE_UNDEF_LENGTH in case
 * of an error, or if attr is NULL.
 */
static DwStyleLength Html_parse_length (DilloHtml *html, const gchar *attr)
{
   DwStyleLength l;
   gchar *end;

   l = Html_parse_length_or_multi_length (attr, &end);
   if (DW_STYLE_IS_REL_LENGTH (l))
      /* not allowed as &Length; */
      return DW_STYLE_LENGTH_AUTO;
   else {
      /* allow only whitespaces */
      if (*end && !isspace (*end)) {
         MSG_HTML("Garbage after length: %s\n", attr);
         return DW_STYLE_LENGTH_AUTO;
      }
   }

   return l;
}

/*
 * Parse a color attribute.
 * Return value: parsed color, or default_color (+ error msg) on error.
 */
static gint32
 Html_color_parse(DilloHtml *html, const char *subtag, gint32 default_color)
{
   gint err = 1;
   gint32 color = a_Color_parse(subtag, default_color, &err);

   if (err) {
      MSG_HTML("color is not in \"#RRGGBB\" format\n");
   }
   return color;
}

/*
 * Check that 'val' is composed of characters inside [A-Za-z0-9:_.-]
 * Note: ID can't have entities, but this check is enough (no '&').
 * Return value: 1 if OK, 0 otherwise.
 */
static gint
 Html_check_name_val(DilloHtml *html, const char *val, const char *attrname)
{
   gint i;

   for (i = 0; val[i]; ++i)
      if (!(isalnum(val[i]) || strchr(":_.-", val[i])))
         break;

   if (val[i] || !isalpha(val[0]))
      MSG_HTML("'%s' value is not of the form "
               "[A-Za-z][A-Za-z0-9:_.-]*\n", attrname);

   return !(val[i]);
}

/*
 * Handle open HTML element
 */
static void Html_tag_open_html(DilloHtml *html, char *tag, gint tagsize)
{
   if (!(html->InFlags & IN_HTML))
      html->InFlags |= IN_HTML;
   ++html->Num_HTML;

   if (html->Num_HTML > 1) {
      MSG_HTML("HTML element was already open\n");
   }
}

/*
 * Handle close HTML element
 */
static void Html_tag_close_html(DilloHtml *html, gint TagIdx)
{
   /* todo: may add some checks here */
   html->InFlags &= ~IN_HTML;
   Html_pop_tag(html, TagIdx);
}

/*
 * Handle open HEAD element
 */
static void Html_tag_open_head(DilloHtml *html, char *tag, gint tagsize)
{
   if (html->InFlags & IN_BODY) {
      MSG_HTML("HEAD element must go before the BODY section\n");
      html->ReqTagClose = TRUE;
      return;
   }

   if (!(html->InFlags & IN_HEAD))
      html->InFlags |= IN_HEAD;
   ++html->Num_HEAD;

   if (html->Num_HEAD > 1) {
      MSG_HTML("HEAD element was already open\n");
   }
}

/*
 * Handle close HEAD element
 * Note: as a side effect of Html_test_section() this function is called
 *       twice when the head element is closed implicitly.
 */
static void Html_tag_close_head(DilloHtml *html, gint TagIdx)
{
   if (html->InFlags & IN_HEAD) {
      if (html->Num_TITLE == 0)
         MSG_HTML("HEAD section lacks the TITLE element\n");
   
      html->InFlags &= ~IN_HEAD;
   }
   Html_pop_tag(html, TagIdx);
}

/*
 * Handle open TITLE
 * calls stash init, where the title string will be stored
 */
static void Html_tag_open_title(DilloHtml *html, char *tag, gint tagsize)
{
   ++html->Num_TITLE;
   Html_stash_init(html);
}

/*
 * Handle close TITLE
 * set page-title in the browser window and in the history.
 */
static void Html_tag_close_title(DilloHtml *html, gint TagIdx)
{
   if (html->InFlags & IN_HEAD) {
      /* title is only valid inside HEAD */
      a_Interface_set_page_title(html->linkblock->bw, html->Stash->str);
      a_History_set_title(NAV_TOP(html->linkblock->bw), html->Stash->str);
   } else {
      MSG_HTML("the TITLE element must be inside the HEAD section\n");
   }
   Html_pop_tag(html, TagIdx);
}

/*
 * Handle open SCRIPT
 * initializes stash, where the embedded code will be stored.
 * MODE_VERBATIM is used because MODE_STASH catches entities.
 */
static void Html_tag_open_script(DilloHtml *html, char *tag, gint tagsize)
{
   Html_stash_init(html);
   html->stack[html->stack_top].parse_mode = DILLO_HTML_PARSE_MODE_VERBATIM;
}

/*
 * Handle close SCRIPT
 */
static void Html_tag_close_script(DilloHtml *html, gint TagIdx)
{
   /* eventually the stash will be sent to an interpreter for parsing */
   Html_pop_tag(html, TagIdx);
}

/*
 * Handle open STYLE
 * store the contents to the stash where (in the future) the style
 * sheet interpreter can get it.
 */
static void Html_tag_open_style(DilloHtml *html, char *tag, gint tagsize)
{
   Html_stash_init(html);
   html->stack[html->stack_top].parse_mode = DILLO_HTML_PARSE_MODE_VERBATIM;
}

/*
 * Handle close STYLE
 */
static void Html_tag_close_style(DilloHtml *html, gint TagIdx)
{
   /* eventually the stash will be sent to an interpreter for parsing */
   Html_pop_tag(html, TagIdx);
}

/*
 * <BODY>
 */
static void Html_tag_open_body(DilloHtml *html, char *tag, gint tagsize)
{
   const char *attrbuf;
   DwPage *page;
   DwStyle style_attrs, *style;
   gint32 color;

   if (!(html->InFlags & IN_BODY))
      html->InFlags |= IN_BODY;
   ++html->Num_BODY;

   if (html->Num_BODY > 1) {
      MSG_HTML("BODY element was already open\n");
   }
   if (html->InFlags & IN_HEAD) {
      /* if we're here, it's bad XHTML, no need to recover */
      MSG_HTML("unclosed HEAD element\n");
   }

   page = DW_PAGE (html->dw);

   if (!prefs.force_my_colors) {
      if ((attrbuf = Html_get_attr(html, tag, tagsize, "bgcolor"))) {
         color = Html_color_parse(html, attrbuf, prefs.bg_color);
         if ( (color == 0xffffff && !prefs.allow_white_bg) ||
              prefs.force_my_colors )
            color = prefs.bg_color;

         style_attrs = *html->dw->style;
         style_attrs.background_color =
            a_Dw_style_color_new (color, html->bw->main_window->window);
         style = a_Dw_style_new (&style_attrs, html->bw->main_window->window);
         a_Dw_widget_set_style (html->dw, style);
         a_Dw_style_unref (style);
         html->stack[html->stack_top].current_bg_color = color;
      }

      if ((attrbuf = Html_get_attr(html, tag, tagsize, "text"))) {
         color = Html_color_parse(html, attrbuf, prefs.text_color);
         HTML_SET_TOP_ATTR
            (html, color,
             a_Dw_style_color_new (color, html->bw->main_window->window));
      }

      if ((attrbuf = Html_get_attr(html, tag, tagsize, "link")))
         html->linkblock->link_color = Html_color_parse(html, attrbuf,
                                                        prefs.link_color);

      if ((attrbuf = Html_get_attr(html, tag, tagsize, "vlink")))
         html->linkblock->visited_color =
            Html_color_parse(html, attrbuf, prefs.visited_color);

      if (prefs.contrast_visited_color) {
         /* get a color that has a "safe distance" from text, link and bg */
         html->linkblock->visited_color =
            a_Color_vc(html->linkblock->visited_color,
                       html->stack[html->stack_top].style->color->color_val,
                       html->linkblock->link_color,
                       html->stack[html->stack_top].current_bg_color);
      }
   }

   html->stack[html->stack_top].parse_mode = DILLO_HTML_PARSE_MODE_BODY;
}

/*
 * BODY
 */
static void Html_tag_close_body(DilloHtml *html, gint TagIdx)
{
   html->InFlags &= ~IN_BODY;
   Html_pop_tag(html, TagIdx);
}

/*
 * <P>
 * todo: what's the point between adding the parbreak before and
 *       after the push?
 */
static void Html_tag_open_p(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_tag_set_align_attr (html, tag, tagsize);
}

/*
 * <TABLE>
 */
static void Html_tag_open_table(DilloHtml *html, char *tag, gint tagsize)
{
#ifdef USE_TABLES
   DwWidget *table;
   DwStyle style_attrs, *tstyle, *old_style;
   const char *attrbuf;
   gint32 border = 0, cellspacing = 1, cellpadding = 2, bgcolor;
#endif

   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 0,
                          html->stack[(html)->stack_top].style);

#ifdef USE_TABLES
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "border")))
      border = *attrbuf ? Html_check_int(strtol(attrbuf,NULL,10), 0,100,1) : 1;
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "cellspacing")))
      cellspacing = Html_check_int(strtol(attrbuf, NULL, 10), 0, 100, 1);
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "cellpadding")))
      cellpadding = Html_check_int(strtol(attrbuf, NULL, 10), 0, 100, 2);

   /* The style for the table */
   style_attrs = *html->stack[html->stack_top].style;

   /* When dillo was started with the --debug-rendering option, there
    * is always a border around the table. */
   if (dillo_dbg_rendering)
      a_Dw_style_box_set_val (&style_attrs.border_width, MIN (border, 1));
   else
      a_Dw_style_box_set_val (&style_attrs.border_width, border);

   a_Dw_style_box_set_border_color
      (&style_attrs,
       a_Dw_style_shaded_color_new (
          html->stack[html->stack_top].current_bg_color,
          html->bw->main_window->window));
   a_Dw_style_box_set_border_style (&style_attrs, DW_STYLE_BORDER_OUTSET);
   style_attrs.border_spacing = cellspacing;

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "width"))) {
      int dw_len = Html_parse_length (html, attrbuf);
      int len = strtol(attrbuf, NULL, 10);
      if ((DW_STYLE_IS_PER_LENGTH(dw_len) &&
           Html_check_int(len, 0, 100, -1) != -1)  ||
          (DW_STYLE_IS_ABS_LENGTH(dw_len) &&
           Html_check_int(len, 0, 5000, -1) != -1) ||
          (DW_STYLE_IS_REL_LENGTH(dw_len) &&
           Html_check_int(len, 0, 100, -1) != -1)) {
         style_attrs.width = dw_len;
      }
   }

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "align"))) {
      if (g_strcasecmp (attrbuf, "left") == 0)
         style_attrs.text_align = DW_STYLE_TEXT_ALIGN_LEFT;
      else if (g_strcasecmp (attrbuf, "right") == 0)
         style_attrs.text_align = DW_STYLE_TEXT_ALIGN_RIGHT;
      else if (g_strcasecmp (attrbuf, "center") == 0)
         style_attrs.text_align = DW_STYLE_TEXT_ALIGN_CENTER;
   }

   if (!prefs.force_my_colors &&
       (attrbuf = Html_get_attr(html, tag, tagsize, "bgcolor"))) {
      bgcolor = Html_color_parse(html, attrbuf, -1);
      if (bgcolor != -1) {
         if (bgcolor == 0xffffff && !prefs.allow_white_bg)
            bgcolor = prefs.bg_color;
         html->stack[html->stack_top].current_bg_color = bgcolor;
         style_attrs.background_color =
            a_Dw_style_color_new (bgcolor, html->bw->main_window->window);
      }
   }

   tstyle = a_Dw_style_new (&style_attrs, html->bw->main_window->window);

   /* The style for the cells */
   style_attrs = *html->stack[html->stack_top].style;
   /* When dillo was started with the --debug-rendering option, there
    * is always a border around the cells. */
   if (dillo_dbg_rendering)
      a_Dw_style_box_set_val (&style_attrs.border_width, 1);
   else
      a_Dw_style_box_set_val (&style_attrs.border_width, border ? 1 : 0);

   a_Dw_style_box_set_val (&style_attrs.padding, cellpadding);
   a_Dw_style_box_set_border_color (&style_attrs, tstyle->border_color.top);
   a_Dw_style_box_set_border_style (&style_attrs, DW_STYLE_BORDER_INSET);

   old_style = html->stack[html->stack_top].table_cell_style;
   html->stack[html->stack_top].table_cell_style =
      a_Dw_style_new (&style_attrs, html->bw->main_window->window);
   if (old_style)
      a_Dw_style_unref (old_style);

   table = a_Dw_table_new ();
   a_Dw_page_add_widget (DW_PAGE (html->dw), table, tstyle);
   a_Dw_style_unref (tstyle);

   html->stack[html->stack_top].table_mode = DILLO_HTML_TABLE_MODE_TOP;
   html->stack[html->stack_top].cell_text_align_set = FALSE;
   html->stack[html->stack_top].table = table;
#endif
}


/*
 * used by <TD> and <TH>
 */
static void Html_tag_open_table_cell(DilloHtml *html, char *tag, gint tagsize,
                                     DwStyleTextAlignType text_align)
{
#ifdef USE_TABLES
   DwWidget *col_page;
   gint colspan = 1, rowspan = 1;
   const char *attrbuf;
   DwStyle style_attrs, *style, *old_style;
   gint32 bgcolor;
   gboolean new_style;

   switch (html->stack[html->stack_top].table_mode) {
   case DILLO_HTML_TABLE_MODE_NONE:
      MSG_HTML("<td> or <th> outside <table>\n");
      return;

   case DILLO_HTML_TABLE_MODE_TOP:
      MSG_HTML("<td> or <th> outside <tr>\n");
      /* a_Dw_table_add_cell takes care that dillo does not crash. */
      /* continues */
   case DILLO_HTML_TABLE_MODE_TR:
   case DILLO_HTML_TABLE_MODE_TD:
      /* todo: check errors? */
      if ((attrbuf = Html_get_attr(html, tag, tagsize, "colspan")))
         colspan = Html_check_int(strtol(attrbuf, NULL, 10), 0, 1000, 1);
      if ((attrbuf = Html_get_attr(html, tag, tagsize, "rowspan")))
         rowspan = Html_check_int(strtol(attrbuf, NULL, 10), 0, 1000, 1);

      /* text style */
      old_style = html->stack[html->stack_top].style;
      style_attrs = *old_style;
      if (!html->stack[html->stack_top].cell_text_align_set)
         style_attrs.text_align = text_align;
      if (Html_get_attr(html, tag, tagsize, "nowrap"))
         style_attrs.white_space = DW_STYLE_WHITE_SPACE_NOWRAP;
      else
         style_attrs.white_space = DW_STYLE_WHITE_SPACE_NORMAL;

      html->stack[html->stack_top].style =
         a_Dw_style_new (&style_attrs, html->bw->main_window->window);
      a_Dw_style_unref (old_style);
      Html_tag_set_align_attr (html, tag, tagsize);

      /* cell style */
      style_attrs = *html->stack[html->stack_top].table_cell_style;
      new_style = FALSE;

      if ((attrbuf = Html_get_attr(html, tag, tagsize, "width"))) {
         style_attrs.width = Html_parse_length (html, attrbuf);
         new_style = TRUE;
      }

      if (Html_tag_set_valign_attr (html, tag, tagsize, &style_attrs))
         new_style = TRUE;

      if (!prefs.force_my_colors &&
          (attrbuf = Html_get_attr(html, tag, tagsize, "bgcolor"))) {
         bgcolor = Html_color_parse(html, attrbuf, -1);
         if (bgcolor != -1) {
            if (bgcolor == 0xffffff && !prefs.allow_white_bg)
               bgcolor = prefs.bg_color;

            new_style = TRUE;
            style_attrs.background_color =
               a_Dw_style_color_new (bgcolor, html->bw->main_window->window);
            html->stack[html->stack_top].current_bg_color = bgcolor;
         }
      }

      if (html->stack[html->stack_top].style->text_align
          == DW_STYLE_TEXT_ALIGN_STRING)
         col_page = a_Dw_table_cell_new
            (a_Dw_table_get_cell_ref
             (DW_TABLE (html->stack[html->stack_top].table)));
      else
         col_page = a_Dw_page_new ();

      if (new_style) {
         style = a_Dw_style_new (&style_attrs, html->bw->main_window->window);
         a_Dw_widget_set_style (col_page, style);
         a_Dw_style_unref (style);
      } else
         a_Dw_widget_set_style (col_page,
                                html->stack[html->stack_top].table_cell_style);

      a_Dw_table_add_cell (DW_TABLE (html->stack[html->stack_top].table),
                           col_page, colspan, rowspan);
      html->stack[html->stack_top].page = html->dw = col_page;

      /* Handle it when the user clicks on a link */
      Html_connect_signals(html, GTK_OBJECT(col_page));
      break;

   default:
      /* compiler happiness */
      break;
   }

   html->stack[html->stack_top].table_mode = DILLO_HTML_TABLE_MODE_TD;
#endif
}


/*
 * <TD>
 */
static void Html_tag_open_td(DilloHtml *html, char *tag, gint tagsize)
{
   Html_tag_open_table_cell (html, tag, tagsize, DW_STYLE_TEXT_ALIGN_LEFT);
}


/*
 * <TH>
 */
static void Html_tag_open_th(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, NULL, 0, 1, 1);
   Html_tag_open_table_cell (html, tag, tagsize, DW_STYLE_TEXT_ALIGN_CENTER);
}


/*
 * <TR>
 */
static void Html_tag_open_tr(DilloHtml *html, char *tag, gint tagsize)
{
   const char *attrbuf;
   DwStyle style_attrs, *style, *old_style;
   gint32 bgcolor;

#ifdef USE_TABLES
   switch (html->stack[html->stack_top].table_mode) {
   case DILLO_HTML_TABLE_MODE_NONE:
      _MSG("Invalid HTML syntax: <tr> outside <table>\n");
      return;

   case DILLO_HTML_TABLE_MODE_TOP:
   case DILLO_HTML_TABLE_MODE_TR:
   case DILLO_HTML_TABLE_MODE_TD:
      style = NULL;

      if (!prefs.force_my_colors &&
          (attrbuf = Html_get_attr(html, tag, tagsize, "bgcolor"))) {
         bgcolor = Html_color_parse(html, attrbuf, -1);
         if (bgcolor != -1) {
            if (bgcolor == 0xffffff && !prefs.allow_white_bg)
               bgcolor = prefs.bg_color;

            style_attrs = *html->stack[html->stack_top].style;
            style_attrs.background_color =
               a_Dw_style_color_new (bgcolor, html->bw->main_window->window);
            style =
               a_Dw_style_new (&style_attrs, html->bw->main_window->window);
            html->stack[html->stack_top].current_bg_color = bgcolor;
         }
      }

      a_Dw_table_add_row (DW_TABLE (html->stack[html->stack_top].table),
                          style);
      if (style)
         a_Dw_style_unref (style);

      if (Html_get_attr (html, tag, tagsize, "align")) {
         html->stack[html->stack_top].cell_text_align_set = TRUE;
         Html_tag_set_align_attr (html, tag, tagsize);
      }

      style_attrs = *html->stack[html->stack_top].table_cell_style;
      if (Html_tag_set_valign_attr (html, tag, tagsize, &style_attrs)) {
         old_style = html->stack[html->stack_top].table_cell_style;
         html->stack[html->stack_top].table_cell_style =
            a_Dw_style_new (&style_attrs, html->bw->main_window->window);
         a_Dw_style_unref (old_style);
      } else

      break;

   default:
      break;
   }

   html->stack[html->stack_top].table_mode = DILLO_HTML_TABLE_MODE_TR;
#else
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 0,
                          html->stack[(html)->stack_top].style);
#endif
}

/*
 * <FRAME>, <IFRAME>
 * todo: This is just a temporary fix while real frame support
 *       isn't finished. Imitates lynx/w3m's frames.
 */
static void Html_tag_open_frame (DilloHtml *html, gchar *tag, gint tagsize)
{
   const char *attrbuf;
   gchar *src, *buf;
   DilloUrl *url;
   DwPage *page;
   DwStyle style_attrs, *link_style;
   DwWidget *bullet;
   gint buf_size;

   page = DW_PAGE(html->dw);

   if ( !(attrbuf = Html_get_attr(html, tag, tagsize, "src")) )
      return;

   if (!(url = Html_url_new(html, attrbuf, NULL, 0, 0, 0, 0)))
      return;

   src = g_strdup(attrbuf);

   style_attrs = *(html->stack[html->stack_top].style);

   if (a_Capi_get_buf(url, &buf, &buf_size))  /* visited frame */
      style_attrs.color = a_Dw_style_color_new
         (html->linkblock->visited_color, html->bw->main_window->window);
   else                                /* unvisited frame */
      style_attrs.color = a_Dw_style_color_new
         (html->linkblock->link_color,  html->bw->main_window->window);

   style_attrs.text_decoration |= DW_STYLE_TEXT_DECORATION_UNDERLINE;
   style_attrs.x_link = Html_set_new_link(html, &url);
   link_style = a_Dw_style_new (&style_attrs,
                                html->bw->main_window->window);

   a_Dw_page_add_parbreak(page, 5, html->stack[(html)->stack_top].style);

   /* The bullet will be assigned the current list style, which should
    * be "disc" by default, but may in very weird pages be different.
    * Anyway, there should be no harm. */
   bullet = a_Dw_bullet_new();
   a_Dw_page_add_widget(page, bullet, html->stack[html->stack_top].style);
   a_Dw_page_add_space(page, html->stack[html->stack_top].style);

   if (tolower(tag[1]) == 'i') {
      /* IFRAME usually comes with very long advertising/spying URLS,
       * to not break rendering we will force name="IFRAME" */
      a_Dw_page_add_text(page, g_strdup("IFRAME"), link_style);

   } else {
      /* FRAME:
       * If 'name' tag is present use it, if not use 'src' value */
      if ( !(attrbuf = Html_get_attr(html, tag, tagsize, "name")) ) {
         a_Dw_page_add_text(page, g_strdup(src), link_style);
      } else {
         a_Dw_page_add_text(page, g_strdup(attrbuf), link_style);
      }
   }

   a_Dw_page_add_parbreak(page, 5, html->stack[(html)->stack_top].style);

   a_Dw_style_unref(link_style);
   g_free(src);
}

/*
 * <FRAMESET>
 * todo: This is just a temporary fix while real frame support
 *       isn't finished. Imitates lynx/w3m's frames.
 */
static void Html_tag_open_frameset (DilloHtml *html, gchar *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   a_Dw_page_add_text(DW_PAGE(html->dw), g_strdup("--FRAME--"),
                      html->stack[html->stack_top].style);
   Html_add_indented(html, 40, 0, 5);
}

/*
 * <H1> | <H2> | <H3> | <H4> | <H5> | <H6>
 */
static void Html_tag_open_h(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);

   /* todo: combining these two would be slightly faster */
   Html_set_top_font(html, prefs.vw_fontname,
                     Html_level_to_fontsize(FontSizesNum - (tag[2] - '0')),
                     1, 3);
   Html_tag_set_align_attr (html, tag, tagsize);

   /* First finalize unclosed H tags (we test if already named anyway) */
   a_Menu_pagemarks_set_text(html->bw, html->Stash->str);
   a_Menu_pagemarks_add(html->bw, DW_PAGE (html->dw),
                        html->stack[html->stack_top].style, (tag[2] - '0'));
   Html_stash_init(html);
   html->stack[html->stack_top].parse_mode =
      DILLO_HTML_PARSE_MODE_STASH_AND_BODY;
}

/*
 * Handle close: <H1> | <H2> | <H3> | <H4> | <H5> | <H6>
 */
static void Html_tag_close_h(DilloHtml *html, gint TagIdx)
{
   a_Menu_pagemarks_set_text(html->bw, html->Stash->str);
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_pop_tag(html, TagIdx);
}

/*
 * <BIG> | <SMALL>
 */
static void Html_tag_open_big_small(DilloHtml *html, char *tag, gint tagsize)
{
   gint level;

   level =
      Html_fontsize_to_level(html->stack[html->stack_top].style->font->size) +
      ((g_strncasecmp(tag+1, "big", 3)) ? -1 : 1);
   Html_set_top_font(html, NULL, Html_level_to_fontsize(level), 0, 0);
}

/*
 * <BR>
 */
static void Html_tag_open_br(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_linebreak(DW_PAGE (html->dw),
                           html->stack[(html)->stack_top].style);
}

/*
 * <BUTTON>
 */
static void Html_tag_open_button(DilloHtml *html, char *tag, gint tagsize)
{
   /*
    * Buttons are rendered on one line, this is (at several levels) a
    * bit simpler. May be changed in the future.
    */
   DwStyle style_attrs, *style;
   DwWidget *button, *page;
   DilloHtmlForm *form;
   DilloHtmlLB *html_lb;
   DilloHtmlInputType inp_type;
   gchar *name, *value, *type;

   /* Render the button */
   style_attrs = *html->stack[html->stack_top].style;

   a_Dw_style_box_set_val(&style_attrs.margin, 0);
   a_Dw_style_box_set_val(&style_attrs.border_width, 0);
   a_Dw_style_box_set_val(&style_attrs.padding, 0);
   style = a_Dw_style_new (&style_attrs, html->bw->main_window->window);
   button = a_Dw_button_new (DW_USES_HINTS, TRUE);

   /* The new button is not set button-insensitive, since nested buttons
    * (if they are anyway allowed, todo: search in spec) should all be
    * activatable. */
   a_Dw_widget_set_button_sensitive (button, TRUE);

   a_Dw_page_add_parbreak (DW_PAGE (html->dw), 5, style);
   a_Dw_page_add_widget (DW_PAGE (html->dw), button, style);
   a_Dw_page_add_parbreak (DW_PAGE (html->dw), 5, style);
   a_Dw_style_unref (style);

   a_Dw_style_box_set_val(&style_attrs.margin, 5);
   style = a_Dw_style_new (&style_attrs, html->bw->main_window->window);
   page = a_Dw_page_new ();
   a_Dw_widget_set_style (page, style);
   a_Dw_style_unref (style);
   a_Dw_container_add (DW_CONTAINER (button), page);
   a_Dw_widget_set_button_sensitive (DW_WIDGET (page), FALSE);
   a_Dw_style_box_set_val(&style_attrs.margin, 0);

   a_Dw_button_set_sensitive (DW_BUTTON (button), FALSE);

   html->stack[html->stack_top].page = html->dw = page;

   /* Handle it when the user clicks on a link */
   Html_connect_signals(html, GTK_OBJECT(page));

   /* Connect it to the form */
   html_lb = html->linkblock;
   form = &(html_lb->forms[html_lb->num_forms - 1]);

   type = Html_get_attr_wdef(html, tag, tagsize, "type", "");

   if (strcmp(type, "submit") == 0) {
      inp_type = DILLO_HTML_INPUT_BUTTON_SUBMIT;
      gtk_signal_connect(GTK_OBJECT(button), "clicked",
                         GTK_SIGNAL_FUNC(Html_submit_form), html_lb);
   } else if (strcmp(type, "reset") == 0) {
      inp_type = DILLO_HTML_INPUT_BUTTON_RESET;
      gtk_signal_connect(GTK_OBJECT(button), "clicked",
                         GTK_SIGNAL_FUNC(Html_reset_form), html_lb);
   } else
      return;

   value = Html_get_attr_wdef(html, tag, tagsize, "value", NULL);
   name = Html_get_attr_wdef(html, tag, tagsize, "name", NULL);

   Html_add_input(form, inp_type, (GtkWidget*)button, name, value,
                  NULL, FALSE);

   g_free(type);
   g_free(name);
   g_free(value);
}


/*
 * <FONT>
 */
static void Html_tag_open_font(DilloHtml *html, char *tag, gint tagsize)
{
#if 1
   DwStyle style_attrs, *old_style;
   /*DwStyleFont font;*/
   const char *attrbuf;
   gint32 color;

   if (!prefs.force_my_colors) {
      old_style = html->stack[html->stack_top].style;
      style_attrs = *old_style;

      if ((attrbuf = Html_get_attr(html, tag, tagsize, "color"))) {
         if (prefs.contrast_visited_color && html->InVisitedLink) {
            color = html->linkblock->visited_color;
         } else { 
            /* use the tag-specified color */
            color = Html_color_parse(
                       html, attrbuf, style_attrs.color->color_val);
            style_attrs.color = a_Dw_style_color_new
               (color, html->bw->main_window->window);
         }
      }

#if 0
      if ((attrbuf = Html_get_attr(html, tag, tagsize, "face"))) {
         font = *( style_attrs.font );
         font.name = attrbuf;
         style_attrs.font = a_Dw_style_font_new_from_list (&font);
      }
#endif

      html->stack[html->stack_top].style =
         a_Dw_style_new (&style_attrs, html->bw->main_window->window);
      a_Dw_style_unref (old_style);
   }

#endif
}

/*
 * <ABBR>
 */
static void Html_tag_open_abbr(DilloHtml *html, char *tag, gint tagsize)
{
   DwTooltip *tooltip;
   const char *attrbuf;

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "title"))) {
      tooltip = a_Dw_tooltip_new_no_ref(attrbuf);
      HTML_SET_TOP_ATTR(html, x_tooltip, tooltip);
   }
}

/*
 * <B>
 */
static void Html_tag_open_b(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, NULL, 0, 1, 1);
}

/*
 * <STRONG>
 */
static void Html_tag_open_strong(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, NULL, 0, 1, 1);
}

/*
 * <I>
 */
static void Html_tag_open_i(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, NULL, 0, 2, 2);
}

/*
 * <EM>
 */
static void Html_tag_open_em(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, NULL, 0, 2, 2);
}

/*
 * <CITE>
 */
static void Html_tag_open_cite(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, NULL, 0, 2, 2);
}

/*
 * <CENTER>
 */
static void Html_tag_open_center(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 0,
                          html->stack[(html)->stack_top].style);
   HTML_SET_TOP_ATTR(html, text_align, DW_STYLE_TEXT_ALIGN_CENTER);
}

/*
 * <ADDRESS>
 */
static void Html_tag_open_address(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_set_top_font(html, NULL, 0, 2, 2);
}

/*
 * <TT>
 */
static void Html_tag_open_tt(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, prefs.fw_fontname, 0, 0, 0);
}

/*
 * Read image-associated tag attributes,
 * create new image and add it to the html page (if add is TRUE).
 */
static DilloImage *Html_add_new_image(DilloHtml *html, char *tag,
                                      gint tagsize, DwStyle *style_attrs,
                                      gboolean add)
{
   DilloImage *Image;
   char *width_ptr, *height_ptr, *alt_ptr;
   const char *attrbuf;
   int space;

   if (prefs.show_tooltip &&
       (attrbuf = Html_get_attr(html, tag, tagsize, "title")))
      style_attrs->x_tooltip = a_Dw_tooltip_new_no_ref(attrbuf);

   alt_ptr = Html_get_attr_wdef(html, tag, tagsize, "alt", NULL);
   width_ptr = Html_get_attr_wdef(html, tag, tagsize, "width", NULL);
   height_ptr = Html_get_attr_wdef(html, tag, tagsize, "height", NULL);

   /* todo: we should scale the image respecting its ratio.
    *       As the image size is not known at this time, maybe a flag
    *       can be set to scale it later.
   if ((width_ptr && !height_ptr) || (height_ptr && !width_ptr))
      [...]
   */

   /* Spacing to the left and right */
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "hspace"))) {
      space = strtol(attrbuf, NULL, 10);

      if (space > 0)
         style_attrs->margin.left = style_attrs->margin.right = space;
   }

   /* Spacing at the top and bottom */
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "vspace"))) {
      space = strtol(attrbuf, NULL, 10);

      if (space > 0)
         style_attrs->margin.top = style_attrs->margin.bottom = space;
   }

   /* Add a new image widget to this page */
   if ((Image = a_Image_new(0, 0, alt_ptr,
                            html->stack[html->stack_top].current_bg_color)))
      if (add)
         Html_add_widget(html, DW_WIDGET(Image->dw), width_ptr, height_ptr,
                         style_attrs);

   g_free(width_ptr);
   g_free(height_ptr);
   g_free(alt_ptr);
   return Image;
}

/*
 * Tell cache to retrieve image
 */
static void Html_load_image(DilloHtml *html, DilloUrl *url, DilloImage *Image)
{
   DilloWeb *Web;
   gint ClientKey;
   /* Fill a Web structure for the cache query */
   Web = a_Web_new(url);
   Web->bw = html->bw;
   Web->Image = Image;
   Web->flags |= WEB_Image;
   /* Request image data from the cache */
   if ((ClientKey = a_Capi_open_url(Web, NULL, NULL)) != 0) {
      a_Interface_add_client(html->bw, ClientKey, 0);
      a_Interface_add_url(html->bw, url, WEB_Image);
   }
}

/*
 * Create a new Image struct and request the image-url to the cache
 * (If it either hits or misses, is not relevant here; that's up to the
 *  cache functions)
 */
static void Html_tag_open_img(DilloHtml *html, char *tag, gint tagsize)
{
   DilloImage *Image;
   DilloUrl *url, *usemap_url;
   DwPage *page;
   DwStyle style_attrs;
   const char *attrbuf;
   gint border;

   /* This avoids loading images. Useful for viewing suspicious HTML email. */
   if (URL_FLAGS(html->linkblock->base_url) & URL_SpamSafe)
      return;

   if (!(attrbuf = Html_get_attr(html, tag, tagsize, "src")) ||
       !(url = Html_url_new(html, attrbuf, NULL, 0, 0, 0, 0)))
      return;

   page = DW_PAGE (html->dw);


   usemap_url = NULL;
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "usemap")))
      /* todo: usemap URLs outside of the document are not used. */
      usemap_url = Html_url_new(html, attrbuf, NULL, 0, 0, 0, 0);

   style_attrs = *html->stack[html->stack_top].style;

   if (html->stack[html->stack_top].style->x_link != -1 ||
       usemap_url != NULL) {
      /* Images within links */
      border = 1;
      if ((attrbuf = Html_get_attr(html, tag, tagsize, "border")))
         border = strtol (attrbuf, NULL, 10);

      if (html->stack[html->stack_top].style->x_link != -1)
         /* In this case we can use the text color */
         a_Dw_style_box_set_border_color
            (&style_attrs,
             a_Dw_style_shaded_color_new (style_attrs.color->color_val,
                                          html->bw->main_window->window));
      else
         a_Dw_style_box_set_border_color
            (&style_attrs,
             a_Dw_style_shaded_color_new (html->linkblock->link_color,
                                          html->bw->main_window->window));

      a_Dw_style_box_set_border_style (&style_attrs, DW_STYLE_BORDER_SOLID);
      a_Dw_style_box_set_val (&style_attrs.border_width, border);
   }

   Image = Html_add_new_image(html, tag, tagsize, &style_attrs, TRUE);
   Html_connect_signals(html, GTK_OBJECT(Image->dw));
   gtk_signal_connect_after(GTK_OBJECT(Image->dw), "button_press_event",
                            GTK_SIGNAL_FUNC(Html_image_menu), html->bw);

   /* Image maps */
   if (Html_get_attr(html, tag, tagsize, "ismap")) {
      /* BUG: if several ISMAP images follow each other without
       * being separated with a word, only the first one is ISMAPed
       */
      a_Dw_image_set_ismap (Image->dw);
      _MSG("  Html_tag_open_img: server-side map (ISMAP)\n");
   } else if (html->stack[html->stack_top].style->x_link != -1 &&
              usemap_url == NULL)
      /* For simple links, we have to suppress the "image_pressed" signal.
       * This is overridden for USEMAP images. */
      a_Dw_widget_set_button_sensitive (DW_WIDGET (Image->dw), FALSE);

   if (usemap_url) {
      a_Dw_image_set_usemap (Image->dw, &html->linkblock->maps, usemap_url);
      a_Url_free (usemap_url);
   }

   Html_load_image(html, url, Image);
   a_Url_free(url);
}

/*
 * <map>
 */
static void Html_tag_open_map(DilloHtml *html, char *tag, gint tagsize)
{
   char *hash_name;
   const char *attrbuf;
   DilloUrl *url;

   if (html->InFlags & IN_MAP) {
      MSG_HTML("nested <map>\n");
   } else {
      if ((attrbuf = Html_get_attr(html, tag, tagsize, "name"))) {
         hash_name = g_strdup_printf("#%s", attrbuf);
         url = Html_url_new(html, hash_name, NULL, 0, 0, 0, 0);
         a_Dw_image_map_list_add_map (&html->linkblock->maps, url);
         a_Url_free (url);
         g_free(hash_name);
      }
      html->InFlags |= IN_MAP;
   }
}

/*
 * Handle close <MAP>
 */
static void Html_tag_close_map(DilloHtml *html, gint TagIdx)
{
   html->InFlags &= ~IN_MAP;
   Html_pop_tag(html, TagIdx);
}

/*
 * Read coords in a string and fill a GdkPoint array
 */
static int Html_read_coords(DilloHtml *html, const char *str, GdkPoint *array)
{
   gint i, toggle, pending, coord;
   const char *tail = str;
   char *newtail = NULL;

   i = 0;
   toggle = 0;
   pending = 1;
   while ( pending ) {
      coord = strtol(tail, &newtail, 10);
      if (toggle) {
        array[i].y = coord;
        array[++i].x = 0;
        toggle = 0;
      } else {
        array[i].x = coord;
        array[i].y = -1;
        toggle = 1;
      }
      if (!*newtail || (coord == 0 && newtail == tail)) {
         pending = 0;
      } else {
         if (*newtail != ',') {
            MSG_HTML("usemap coords MUST be separated with ','\n");
         }
         tail = newtail + 1;
      }
   }

   return i;
}

/*
 * <AREA>
 */
static void Html_tag_open_area(DilloHtml *html, char *tag, gint tagsize)
{
   /* todo: point must be a dynamic array */
   GdkPoint point[1024];
   DilloUrl* url;
   const char *attrbuf;
   gint type = DW_IMAGE_MAP_SHAPE_RECT;
   gint nbpoints, link = -1;

   if ( (attrbuf = Html_get_attr(html, tag, tagsize, "shape")) ) {
      if ( g_strcasecmp(attrbuf, "rect") == 0 )
         type = DW_IMAGE_MAP_SHAPE_RECT;
      else if ( g_strcasecmp(attrbuf, "circle") == 0 )
         type = DW_IMAGE_MAP_SHAPE_CIRCLE;
      else if ( g_strncasecmp(attrbuf, "poly", 4) == 0 )
         type = DW_IMAGE_MAP_SHAPE_POLY;
      else
         type = DW_IMAGE_MAP_SHAPE_RECT;
   }
   /* todo: add support for coords in % */
   if ( (attrbuf = Html_get_attr(html, tag, tagsize, "coords")) ) {
      /* Is this a valid poly ?
       * rect = x0,y0,x1,y1               => 2
       * circle = x,y,r                   => 2
       * poly = x0,y0,x1,y1,x2,y2 minimum => 3 */
      nbpoints = Html_read_coords(html, attrbuf, point);
   } else
      return;

   if ( Html_get_attr(html, tag, tagsize, "nohref") ) {
      link = -1;
      _MSG("nohref");
   }

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "href"))) {
      url = Html_url_new(html, attrbuf, NULL, 0, 0, 0, 0);
      g_return_if_fail ( url != NULL );
      if ((attrbuf = Html_get_attr(html, tag, tagsize, "alt")))
         a_Url_set_alt(url, attrbuf);

      link = Html_set_new_link(html, &url);
   }

   a_Dw_image_map_list_add_shape(&html->linkblock->maps, type, link,
                                 point, nbpoints);
}


/*
 * Test and extract the link from a javascript instruction.
 */
static const char* Html_get_javascript_link(DilloHtml *html)
{
   size_t i;
   char ch, *p1, *p2;
   GString *Buf = html->attr_data;

   if (g_strncasecmp("javascript", Buf->str, 10) == 0) {
      i = strcspn(Buf->str, "'\"");
      ch = Buf->str[i];
      if ((ch == '"' || ch == '\'') &&
          (p2 = strchr(Buf->str + i + 1 , ch))) {
         p1 = Buf->str + i;
         MSG_HTML("link depends on javascript()\n");
         g_string_truncate(Buf, p2 - Buf->str);
         g_string_erase(Buf, 0, p1 - Buf->str + 1);
      }
   }
   return Buf->str;
}

/*
 * Register an anchor for this page.
 */
static void Html_add_anchor(DilloHtml *html, const char *name)
{
   _MSG("Registering ANCHOR: %s\n", name);
   if (!a_Dw_page_add_anchor(DW_PAGE(html->dw), name,
                             html->stack[html->stack_top].style))
      MSG_HTML("Anchor names must be unique within the document\n");
   /* According to Sec. 12.2.1 of the HTML 4.01 spec, "anchor names that
    * differ only in case may not appear in the same document", but
    * "comparisons between fragment identifiers and anchor names must be
    * done by exact (case-sensitive) match." We ignore the case issue and
    * always test for exact matches. Moreover, what does uppercase mean
    * for Unicode characters outside the ASCII range? */
}

/*
 * <A>
 */
static void Html_tag_open_a(DilloHtml *html, char *tag, gint tagsize)
{
   DwStyle style_attrs, *old_style;
   DilloUrl *url;
   const char *attrbuf;
   gchar *buf;
   gint buf_size;

   /* todo: add support for MAP with A HREF */
   Html_tag_open_area(html, tag, tagsize);

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "href"))) {
      /* if it's a javascript link, extract the reference. */
      if (tolower(attrbuf[0]) == 'j')
         attrbuf = Html_get_javascript_link(html);

      url = Html_url_new(html, attrbuf, NULL, 0, 0, 0, 0);
      g_return_if_fail ( url != NULL );

      old_style = html->stack[html->stack_top].style;
      style_attrs = *old_style;

      if (a_Capi_get_buf(url, &buf, &buf_size)) {
         html->InVisitedLink = TRUE;
         style_attrs.color = a_Dw_style_color_new(
            html->linkblock->visited_color,
/*
            a_Color_vc(html->linkblock->visited_color,
                       html->stack[html->stack_top].style->color->color_val,
                       html->linkblock->link_color,
                       html->stack[html->stack_top].current_bg_color),
*/
            html->bw->main_window->window);
      } else {
         style_attrs.color = a_Dw_style_color_new
            (html->linkblock->link_color,  html->bw->main_window->window);
      }

      if ((attrbuf = Html_get_attr(html, tag, tagsize, "title")))
         style_attrs.x_tooltip = a_Dw_tooltip_new_no_ref(attrbuf);

      style_attrs.text_decoration |= DW_STYLE_TEXT_DECORATION_UNDERLINE;
      style_attrs.x_link = Html_set_new_link(html, &url);

      html->stack[html->stack_top].style =
         a_Dw_style_new (&style_attrs, html->bw->main_window->window);
      a_Dw_style_unref (old_style);
   }

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "name"))) {
      if (prefs.show_extra_warnings)
         Html_check_name_val(html, attrbuf, "name");
      /* html->NameVal is freed in Html_process_tag */
      html->NameVal = a_Url_decode_hex_str(attrbuf);
      Html_add_anchor(html, html->NameVal);
   }
}

/*
 * <A> close function
 */
static void Html_tag_close_a(DilloHtml *html, gint TagIdx)
{
   html->InVisitedLink = FALSE;
   Html_pop_tag(html, TagIdx);
}

/*
 * Insert underlined text in the page.
 */
static void Html_tag_open_u(DilloHtml *html, char *tag, gint tagsize)
{
   DwStyle *style;
   DwStyle style_attrs;

   style = html->stack[html->stack_top].style;
   style_attrs = *style;
   style_attrs.text_decoration |= DW_STYLE_TEXT_DECORATION_UNDERLINE;
   html->stack[html->stack_top].style =
      a_Dw_style_new(&style_attrs, html->bw->main_window->window);
   a_Dw_style_unref(style);
}

/*
 * Insert strike-through text. Used by <S>, <STRIKE> and <DEL>.
 */
static void Html_tag_open_strike(DilloHtml *html, char *tag, gint tagsize)
{
   DwStyle *style;
   DwStyle style_attrs;

   style = html->stack[html->stack_top].style;
   style_attrs = *style;
   style_attrs.text_decoration |= DW_STYLE_TEXT_DECORATION_LINE_THROUGH;
   html->stack[html->stack_top].style =
      a_Dw_style_new(&style_attrs, html->bw->main_window->window);
   a_Dw_style_unref(style);
}

/*
 * <BLOCKQUOTE>
 */
static void Html_tag_open_blockquote(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_add_indented(html, 40, 40, 9);
}

/*
 * Handle the <UL> tag.
 */
static void Html_tag_open_ul(DilloHtml *html, char *tag, gint tagsize)
{
   const char *attrbuf;
   DwStyleListStyleType list_style_type;

   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_add_indented(html, 40, 0, 9);

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "type"))) {
      /* list_style_type explicitly defined */
      if (g_strncasecmp(attrbuf, "disc", 4) == 0)
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_DISC;
      else if (g_strncasecmp(attrbuf, "circle", 6) == 0)
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_CIRCLE;
      else if (g_strncasecmp(attrbuf, "square", 6) == 0)
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_SQUARE;
      else
         /* invalid value */
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_DISC;
   } else {
      if (html->stack[html->stack_top].list_type == HTML_LIST_UNORDERED) {
         /* Nested <UL>'s. */
         /* --EG :: I changed the behavior here : types are cycling instead of
          * being forced to square. It's easier for mixed lists level counting.
          */
         switch (html->stack[html->stack_top].style->list_style_type) {
         case DW_STYLE_LIST_STYLE_TYPE_DISC:
            list_style_type = DW_STYLE_LIST_STYLE_TYPE_CIRCLE;
            break;
         case DW_STYLE_LIST_STYLE_TYPE_CIRCLE:
            list_style_type = DW_STYLE_LIST_STYLE_TYPE_SQUARE;
            break;
         case DW_STYLE_LIST_STYLE_TYPE_SQUARE:
         default: /* this is actually a bug */
            list_style_type = DW_STYLE_LIST_STYLE_TYPE_DISC;
            break;
         }
      } else {
         /* Either first <UL>, or a <OL> before. */
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_DISC;
      }
   }

   HTML_SET_TOP_ATTR(html, list_style_type, list_style_type);
   html->stack[html->stack_top].list_type = HTML_LIST_UNORDERED;

   html->stack[html->stack_top].list_number = 0;
   html->stack[html->stack_top].ref_list_item = NULL;
}

/*
 * Handle the <MENU> tag.
 * (Deprecated and almost the same as <UL>)
 */
static void Html_tag_open_menu(DilloHtml *html, char *tag, gint tagsize)
{
   DwStyleListStyleType list_style_type = DW_STYLE_LIST_STYLE_TYPE_DISC;

   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_add_indented(html, 40, 0, 9);
   HTML_SET_TOP_ATTR(html, list_style_type, list_style_type);
   html->stack[html->stack_top].list_type = HTML_LIST_UNORDERED;
   html->stack[html->stack_top].list_number = 0;
   html->stack[html->stack_top].ref_list_item = NULL;

   if (prefs.show_extra_warnings)
      MSG_HTML("it is strongly recommended using <UL> instead of <MENU>\n");
}

/*
 * Handle the <OL> tag.
 */
static void Html_tag_open_ol(DilloHtml *html, char *tag, gint tagsize)
{
   const char *attrbuf;
   DwStyleListStyleType list_style_type;

   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_add_indented(html, 40, 0, 9);

   list_style_type = DW_STYLE_LIST_STYLE_TYPE_DECIMAL;

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "type"))) {
      if (*attrbuf == '1')
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_DECIMAL;
      else if (*attrbuf == 'a')
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_LOWER_ALPHA;
      else if (*attrbuf == 'A')
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_UPPER_ALPHA;
      else if (*attrbuf == 'i')
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_LOWER_ROMAN;
      else if (*attrbuf == 'I')
         list_style_type = DW_STYLE_LIST_STYLE_TYPE_UPPER_ROMAN;
   }

   HTML_SET_TOP_ATTR(html, list_style_type, list_style_type);
   html->stack[html->stack_top].list_type = HTML_LIST_ORDERED;

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "start")))
      html->stack[html->stack_top].list_number = strtol(attrbuf, NULL, 10);
   else
      html->stack[html->stack_top].list_number = 1;
   html->stack[html->stack_top].ref_list_item = NULL;
}

/*
 * Handle the <LI> tag.
 */
static void Html_tag_open_li(DilloHtml *html, char *tag, gint tagsize)
{
   DwWidget *bullet, *list_item, **ref_list_item;
   char str[64];
   const char *attrbuf;
   gint *list_number;

   /* Get our parent tag's variables (used as state storage) */
   list_number = &html->stack[html->stack_top - 1].list_number;
   ref_list_item = &html->stack[html->stack_top - 1].ref_list_item;

   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 0,
                          html->stack[(html)->stack_top].style);

   switch (html->stack[html->stack_top].list_type) {
   case HTML_LIST_NONE:
      MSG_HTML("<li> outside <ul> or <ol>\n");
      list_item = a_Dw_list_item_new(NULL);
      Html_add_indented_widget(html, list_item, 0, 0, 0 /* or 1 */);
      bullet = a_Dw_bullet_new();
      a_Dw_list_item_init_with_widget(DW_LIST_ITEM(html->dw), bullet,
                                      html->stack[html->stack_top].style);
      break;

   default:
      list_item = a_Dw_list_item_new((DwListItem*)*ref_list_item);
      Html_add_indented_widget(html, list_item, 0, 0, 0 /* or 1 */);
      *ref_list_item = list_item;

      switch (html->stack[html->stack_top].list_type) {
      case HTML_LIST_UNORDERED:
         bullet = a_Dw_bullet_new();
         a_Dw_list_item_init_with_widget(DW_LIST_ITEM(html->dw), bullet,
                                         html->stack[html->stack_top].style);
         break;

      case HTML_LIST_ORDERED:
         if ((attrbuf = Html_get_attr(html, tag, tagsize, "value")))
            *list_number = strtol(attrbuf, NULL, 10);
         a_Dw_style_numtostr
            (*list_number, str, 64,
             html->stack[html->stack_top].style->list_style_type);
         (*list_number)++;
         a_Dw_list_item_init_with_text(DW_LIST_ITEM (html->dw), g_strdup(str),
                                       html->stack[html->stack_top].style);

      case HTML_LIST_NONE:
         /* make compiler happy */
         break;
      }
   }
}

/*
 * <HR>
 */
static void Html_tag_open_hr(DilloHtml *html, char *tag, gint tagsize)
{
   DwWidget *hruler;
   DwStyle style_attrs;
   char *width_ptr;
   const char *attrbuf;
   gint32 size = 0;

   width_ptr = Html_get_attr_wdef(html, tag, tagsize, "width", "100%");

   style_attrs = *html->stack[html->stack_top].style;

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "size")))
      size = strtol(attrbuf, NULL, 10);

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "align"))) {
      if (g_strcasecmp (attrbuf, "left") == 0)
         style_attrs.text_align = DW_STYLE_TEXT_ALIGN_LEFT;
      else if (g_strcasecmp (attrbuf, "right") == 0)
         style_attrs.text_align = DW_STYLE_TEXT_ALIGN_RIGHT;
      else if (g_strcasecmp (attrbuf, "center") == 0)
         style_attrs.text_align = DW_STYLE_TEXT_ALIGN_CENTER;
   }

   /* todo: evaluate attribute */
   if (Html_get_attr(html, tag, tagsize, "noshade")) {
      a_Dw_style_box_set_border_style (&style_attrs, DW_STYLE_BORDER_SOLID);
      a_Dw_style_box_set_border_color
         (&style_attrs,
          a_Dw_style_shaded_color_new (style_attrs.color->color_val,
                                       html->bw->main_window->window));
      if (size < 1)
         size = 1;
   } else {
      a_Dw_style_box_set_border_style (&style_attrs, DW_STYLE_BORDER_INSET);
      a_Dw_style_box_set_border_color
         (&style_attrs,
          a_Dw_style_shaded_color_new
             (html->stack[html->stack_top].current_bg_color,
              html->bw->main_window->window));
      if (size < 2)
         size = 2;
   }

   style_attrs.border_width.top =
      style_attrs.border_width.left = (size + 1) / 2;
   style_attrs.border_width.bottom =
      style_attrs.border_width.right = size / 2;

   a_Dw_page_add_parbreak (DW_PAGE (html->dw), 5,
                           html->stack[(html)->stack_top].style);
   hruler = a_Dw_hruler_new ();
   Html_add_widget(html, hruler, width_ptr, NULL, &style_attrs);
   a_Dw_page_add_parbreak (DW_PAGE (html->dw), 5,
                           html->stack[(html)->stack_top].style);
   g_free(width_ptr);
}

/*
 * <DL>
 */
static void Html_tag_open_dl(DilloHtml *html, char *tag, gint tagsize)
{
   /* may want to actually do some stuff here. */
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
}

/*
 * <DT>
 */
static void Html_tag_open_dt(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_set_top_font(html, NULL, 0, 1, 1);
}

/*
 * <DD>
 */
static void Html_tag_open_dd(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_add_indented(html, 40, 40, 9);
}

/*
 * <PRE>
 */
static void Html_tag_open_pre(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_set_top_font(html, prefs.fw_fontname, 0, 0, 0);

   /* Is the placement of this statement right? */
   html->stack[html->stack_top].parse_mode = DILLO_HTML_PARSE_MODE_PRE;
   HTML_SET_TOP_ATTR (html, white_space, DW_STYLE_WHITE_SPACE_PRE);
   html->pre_column = 0;
   html->PreFirstChar = TRUE;
   html->InFlags |= IN_PRE;
}

/*
 * Custom close for <PRE>
 */
static void Html_tag_close_pre(DilloHtml *html, gint TagIdx)
{
   html->InFlags &= ~IN_PRE;
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_pop_tag(html, TagIdx);
}

/*
 * Check whether a tag is in the "excluding" element set for PRE
 * Excl. Set = {IMG, OBJECT, APPLET, BIG, SMALL, SUB, SUP, FONT, BASEFONT}
 */
static gint Html_tag_pre_excludes(gint tag_idx)
{
   char *es_set[] = {"img", "object", "applet", "big", "small", "sub", "sup",
                     "font", "basefont", NULL};
   static gint ei_set[10], i;

   /* initialize array */
   if (!ei_set[0])
      for (i = 0; es_set[i]; ++i)
         ei_set[i] = Html_tag_index(es_set[i]);

   for (i = 0; ei_set[i]; ++i)
      if (tag_idx == ei_set[i])
         return 1;
   return 0;
}

/*
 * Handle <FORM> tag
 */
static void Html_tag_open_form(DilloHtml *html, char *tag, gint tagsize)
{
   DilloUrl *action;
   DilloHtmlMethod method;
   DilloHtmlEnc enc;
   const char *attrbuf;

   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);

   if (html->InFlags & IN_FORM) {
      MSG_HTML("nested forms\n");
      return;
   }
   html->InFlags |= IN_FORM;

   method = DILLO_HTML_METHOD_GET;
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "method"))) {
      if (!g_strcasecmp(attrbuf, "post"))
         method = DILLO_HTML_METHOD_POST;
      /* todo: maybe deal with unknown methods? */
   }
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "action")))
      action = Html_url_new(html, attrbuf, NULL, 0, 0, 0, 0);
   else
      action = a_Url_dup(html->linkblock->base_url);
   enc = DILLO_HTML_ENC_URLENCODING;
   if ( (attrbuf = Html_get_attr(html, tag, tagsize, "encoding")) ) {
      /* todo: maybe deal with unknown encodings? */
   }
   Html_form_new(html->linkblock, method, action, enc);
   a_Url_free(action);
}

static void Html_tag_close_form(DilloHtml *html, gint TagIdx)
{
   static gchar *SubmitTag =
      "<input type='submit' value='?Submit?' alt='dillo-generated-button'>";
   DilloHtmlForm *form;
   gint i;

   if (html->InFlags & IN_FORM) {
      form = &(html->linkblock->forms[html->linkblock->num_forms - 1]);

      /* If we don't have a submit button and the user desires one,
         let's add a custom one */
      if (form->num_submit_buttons == 0) {
         if (prefs.show_extra_warnings || form->num_entry_fields != 1)
            MSG_HTML("FORM lacks a Submit button\n");
         if (prefs.generate_submit) {
            MSG_HTML(" (added a submit button internally)\n");
            Html_tag_open_input(html, SubmitTag, strlen(SubmitTag));
            form->num_submit_buttons = 0;
         }
      }

      /* Make buttons sensitive again */
      for (i = 0; i < form->num_inputs; i++) {
         /* Check for tricky HTML (e.g. <input type=image>) */
         if (!form->inputs[i].widget)
            continue;
         if (form->inputs[i].type == DILLO_HTML_INPUT_SUBMIT ||
             form->inputs[i].type == DILLO_HTML_INPUT_RESET) {
            gtk_widget_set_sensitive(form->inputs[i].widget, TRUE);
         } else if (form->inputs[i].type == DILLO_HTML_INPUT_IMAGE ||
                    form->inputs[i].type == DILLO_HTML_INPUT_BUTTON_SUBMIT ||
                    form->inputs[i].type == DILLO_HTML_INPUT_BUTTON_RESET) {
            a_Dw_button_set_sensitive(DW_BUTTON(form->inputs[i].widget), TRUE);
         }
      }
   }
   html->InFlags &= ~IN_FORM;
   html->InFlags &= ~IN_SELECT;
   html->InFlags &= ~IN_TEXTAREA;
   Html_pop_tag(html, TagIdx);
}

/*
 * Handle <META>
 * We do not support http-equiv=refresh because it's non standard,
 * (the HTML 4.01 SPEC recommends explicitily to avoid it), and it
 * can be easily abused!
 *
 * More info at:
 *   http://lists.w3.org/Archives/Public/www-html/2000Feb/thread.html#232
 *
 * todo: Note that we're sending custom HTML while still IN_HEAD. This
 * is a hackish way to put the message. A much cleaner approach is to
 * build a custom widget for it.
 */
static void Html_tag_open_meta(DilloHtml *html, char *tag, gint tagsize)
{
   const gchar *meta_template =
"<table width='100%%'><tr><td bgcolor='#ee0000'>Warning:</td>\n"
" <td bgcolor='#8899aa' width='100%%'>\n"
" This page uses the NON-STANDARD meta refresh tag.<br> The HTML 4.01 SPEC\n"
" (sec 7.4.4) recommends explicitly to avoid it.</td></tr>\n"
" <tr><td bgcolor='#a0a0a0' colspan='2'>The author wanted you to go\n"
" <a href='%s'>here</a>%s</td></tr></table><br>\n";

   const gchar *equiv, *content;
   gchar *html_msg, delay_str[64];
   gint delay;

   /* only valid inside HEAD */
   if (!(html->InFlags & IN_HEAD)) {
      MSG_HTML("META elements must be inside the HEAD section\n");
      return;
   }

   if ((equiv = Html_get_attr(html, tag, tagsize, "http-equiv")) &&
       !g_strcasecmp(equiv, "refresh") &&
       (content = Html_get_attr(html, tag, tagsize, "content"))) {

      /* Get delay, if present, and make a message with it */
      if ((delay = strtol(content, NULL, 0)))
         g_snprintf(delay_str, 64, " after %d second%s.",
                    delay, (delay > 1) ? "s" : "");
      else
         sprintf(delay_str, ".");

      /* Skip to anything after "URL=" */
      while (*content && *(content++) != '=');

      /* Send a custom HTML message
       * todo: this is a hairy hack, It'd be much better to build a widget. */
      html_msg = g_strdup_printf(meta_template, content, delay_str);
      {
         DilloHtmlProcessingState SaveFlags = html->InFlags;
         html->InFlags = IN_BODY;
         html->TagSoup = FALSE;
         Html_write_raw(html, html_msg, strlen(html_msg), 0);
         html->TagSoup = TRUE;
         html->InFlags = SaveFlags;
      }
      g_free(html_msg);
   }
}

/*
 * Set the history of the menu to be consistent with the active menuitem.
 */
static void Html_select_set_history(DilloHtmlInput *input)
{
   gint i;

   for (i = 0; i < input->select->num_options; i++) {
      if (GTK_CHECK_MENU_ITEM(input->select->options[i].menuitem)->active) {
         gtk_option_menu_set_history(GTK_OPTION_MENU(input->widget), i);
         break;
      }
   }
}

/*
 * Reset the input widget to the initial value.
 */
static void Html_reset_input(DilloHtmlInput *input)
{
   gint i;

   switch (input->type) {
   case DILLO_HTML_INPUT_TEXT:
   case DILLO_HTML_INPUT_PASSWORD:
      gtk_entry_set_text(GTK_ENTRY(input->widget), input->init_str);
      break;
   case DILLO_HTML_INPUT_CHECKBOX:
      gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(input->widget),
                                  input->init_val);
      break;
   case DILLO_HTML_INPUT_RADIO:
      gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(input->widget),
                                  input->init_val);
      break;
   case DILLO_HTML_INPUT_SELECT:
      if (input->select != NULL) {
         /* this is in reverse order so that, in case more than one was
          * selected, we get the last one, which is consistent with handling
          * of multiple selected options in the layout code. */
         for (i = input->select->num_options - 1; i >= 0; i--) {
            if (input->select->options[i].init_val) {
               gtk_menu_item_activate(GTK_MENU_ITEM
                                      (input->select->options[i].menuitem));
               Html_select_set_history(input);
               break;
            }
         }
      }
      break;
   case DILLO_HTML_INPUT_SEL_LIST:
      if (!input->select)
         break;
      for (i = 0; i < input->select->num_options; i++) {
         if (input->select->options[i].init_val) {
            if (input->select->options[i].menuitem->state == GTK_STATE_NORMAL)
               gtk_list_select_child(GTK_LIST(input->select->menu),
                                     input->select->options[i].menuitem);
         } else {
            if (input->select->options[i].menuitem->state==GTK_STATE_SELECTED)
               gtk_list_unselect_child(GTK_LIST(input->select->menu),
                                       input->select->options[i].menuitem);
         }
      }
      break;
   case DILLO_HTML_INPUT_TEXTAREA:
      if (input->init_str != NULL) {
         int pos = 0;
         gtk_editable_delete_text(GTK_EDITABLE(input->widget), 0, -1);
         gtk_editable_insert_text(GTK_EDITABLE(input->widget), input->init_str,
                                  strlen(input->init_str), &pos);
      }
      break;
   default:
      break;
   }
}


/*
 * Add a new input to the form data structure, setting the initial
 * values.
 */
static void Html_add_input(DilloHtmlForm *form,
                           DilloHtmlInputType type,
                           GtkWidget *widget,
                           const char *name,
                           const char *init_str,
                           DilloHtmlSelect *select,
                           gboolean init_val)
{
   DilloHtmlInput *input;

   _MSG("name=[%s] init_str=[%s] init_val=[%d]\n",
        name, init_str, init_val);
   a_List_add(form->inputs, form->num_inputs, form->num_inputs_max);
   input = &(form->inputs[form->num_inputs]);
   input->type = type;
   input->widget = widget;
   input->name = (name) ? g_strdup(name) : NULL;
   input->init_str = (init_str) ? g_strdup(init_str) : NULL;
   input->select = select;
   input->init_val = init_val;
   Html_reset_input(input);

   /* some stats */
   if (type == DILLO_HTML_INPUT_PASSWORD ||
       type == DILLO_HTML_INPUT_TEXT ||
       type == DILLO_HTML_INPUT_TEXTAREA) {
      form->num_entry_fields++;
   } else if (type == DILLO_HTML_INPUT_SUBMIT ||
              type == DILLO_HTML_INPUT_BUTTON_SUBMIT ||
              type == DILLO_HTML_INPUT_IMAGE) {
      form->num_submit_buttons++;
   }
   form->num_inputs++;
}


/*
 * Given a GtkWidget, find the form that contains it.
 * Return value: form_index if successful, -1 otherwise.
 */
static int Html_find_form(GtkWidget *reset, DilloHtmlLB *html_lb)
{
   gint form_index;
   gint input_index;
   DilloHtmlForm *form;

   for (form_index = 0; form_index < html_lb->num_forms; form_index++) {
      form = &(html_lb->forms[form_index]);
      for (input_index = 0; input_index < form->num_inputs; input_index++) {
         if (form->inputs[input_index].widget == reset) {
            return form_index;
         }
      }
   }
   return -1;
}

/*
 * Reset all inputs in the form containing reset to their initial values.
 * In general, reset is the reset button for the form.
 */
static void Html_reset_form(GtkWidget *reset, DilloHtmlLB *html_lb)
{
   gint i, j;
   DilloHtmlForm *form;

   if ( (i = Html_find_form(reset, html_lb)) != -1 ){
      form = &html_lb->forms[i];
      for ( j = 0; j < form->num_inputs; j++)
         Html_reset_input(&(form->inputs[j]));
   }
}

/*
 * Urlencode 'val' and append it to 'str'
 */
static void Html_urlencode_append(GString *str, const char *val)
{
   gchar *enc_val = a_Url_encode_hex_str(val);
   g_string_append(str, enc_val);
   g_free(enc_val);
}

/*
 * Append a name-value pair to an existing url.
 * (name and value are urlencoded before appending them)
 */
static void
 Html_append_input(GString *url, const char *name, const char *value)
{
   if (name != NULL) {
      Html_urlencode_append(url, name);
      g_string_append_c(url, '=');
      Html_urlencode_append(url, value);
      g_string_append_c(url, '&');
   }
}

/*
 * Append a image button click position to an existing url.
 */
static void Html_append_clickpos(GString *url, const char *name, int x, int y)
{
   if (name) {
      Html_urlencode_append(url, name);
      g_string_sprintfa(url, ".x=%d&", x);
      Html_urlencode_append(url, name);
      g_string_sprintfa(url, ".y=%d&", y);
   } else
      g_string_sprintfa(url, "x=%d&y=%d&", x, y);
}

/*
 * Submit the form containing the submit input by making a new query URL
 * and sending it with a_Nav_push.
 * (Called by GTK+)
 * click_x and click_y are used only by input images and are set only when
 * called by Html_image_clicked. GTK+ does NOT give these arguments.
 */
static void Html_submit_form(GtkWidget *submit, DilloHtmlLB *html_lb,
                             gint click_x, gint click_y)
{
   gint i, input_index;
   DilloHtmlForm *form;
   DilloHtmlInput *input;
   DilloUrl *new_url;
   gchar *url_str, *action_str, *p, *text;

   /* Search the form that generated the submit event */
   if ( (i = Html_find_form(submit, html_lb)) == -1 )
      return;

   form = &html_lb->forms[i];
   if ((form->method == DILLO_HTML_METHOD_GET) ||
       (form->method == DILLO_HTML_METHOD_POST)) {
      GString *DataStr = g_string_sized_new(4096);

      DEBUG_MSG(3,"Html_submit_form form->action=%s\n",URL_STR_(form->action));

      for (input_index = 0; input_index < form->num_inputs; input_index++) {
         input = &(form->inputs[input_index]);
         switch (input->type) {
         case DILLO_HTML_INPUT_TEXT:
         case DILLO_HTML_INPUT_PASSWORD:
            Html_append_input(DataStr, input->name,
                              gtk_entry_get_text(GTK_ENTRY(input->widget)));
            break;
         case DILLO_HTML_INPUT_CHECKBOX:
         case DILLO_HTML_INPUT_RADIO:
            if (GTK_TOGGLE_BUTTON(input->widget)->active &&
                input->name != NULL && input->init_str != NULL) {
               Html_append_input(DataStr, input->name, input->init_str);
            }
            break;
         case DILLO_HTML_INPUT_HIDDEN:
            Html_append_input(DataStr, input->name, input->init_str);
            break;
         case DILLO_HTML_INPUT_SELECT:
            for (i = 0; i < input->select->num_options; i++) {
               if (GTK_CHECK_MENU_ITEM(input->select->options[i].menuitem)->
                   active) {
                  Html_append_input(DataStr, input->name,
                                    input->select->options[i].value);
                  break;
               }
            }
            break;
         case DILLO_HTML_INPUT_SEL_LIST:
            for (i = 0; i < input->select->num_options; i++) {
               if (input->select->options[i].menuitem->state ==
                   GTK_STATE_SELECTED) {
                  Html_append_input(DataStr, input->name,
                                    input->select->options[i].value);
               }
            }
            break;
         case DILLO_HTML_INPUT_TEXTAREA:
            text = gtk_editable_get_chars(GTK_EDITABLE (input->widget),0,-1);
            Html_append_input(DataStr, input->name, text);
            g_free(text);
            break;
         case DILLO_HTML_INPUT_INDEX:
            Html_urlencode_append(DataStr,
               gtk_entry_get_text(GTK_ENTRY(input->widget)));
            break;
         case DILLO_HTML_INPUT_IMAGE:
            if (input->widget == submit) {
               Html_append_input(DataStr, input->name, input->init_str);
               Html_append_clickpos(DataStr, input->name, click_x, click_y);
            }
            break;
         case DILLO_HTML_INPUT_SUBMIT:
         case DILLO_HTML_INPUT_BUTTON_SUBMIT:
            /* Only the button that triggered the submit. */
            if (input->widget == submit && form->num_submit_buttons > 0)
               Html_append_input(DataStr, input->name, input->init_str);
            break;
         default:
            break;
         } /* switch */
      } /* for (inputs) */

      if ( DataStr->str[DataStr->len - 1] == '&' )
         g_string_truncate(DataStr, DataStr->len - 1);

      /* form->action was previously resolved against base URL */
      action_str = g_strdup(URL_STR(form->action));

      if (form->method == DILLO_HTML_METHOD_POST) {
         new_url = a_Url_new(action_str, NULL, 0, 0, 0);
         a_Url_set_data(new_url, DataStr->str);
         a_Url_set_flags(new_url, URL_FLAGS(new_url) | URL_Post);
      } else {
         /* remove <fragment> and <query> sections if present */
         if ((p = strchr(action_str, '#')))
            *p = 0;
         if ((p = strchr(action_str, '?')))
            *p = 0;

         url_str = g_strconcat(action_str, "?", DataStr->str, NULL);
         new_url = a_Url_new(url_str, NULL, 0, 0, 0);
         a_Url_set_flags(new_url, URL_FLAGS(new_url) | URL_Get);
         g_free(url_str);
      }

      a_Nav_push(html_lb->bw, new_url);
      g_free(action_str);
      g_string_free(DataStr, TRUE);
      a_Url_free(new_url);
   } else {
      MSG("Html_submit_form: Method unknown\n");
   }

   /* now, make the rendered area have its focus back */
   gtk_widget_grab_focus(GTK_BIN(html_lb->bw->docwin)->child);
}


/*
 * Submit form if it has no submit button.
 * (Called by GTK+ when the user presses enter in a text entry within a form)
 */
static void Html_enter_submit_form(GtkWidget *submit, DilloHtmlLB *html_lb)
{
   gint i;

   /* Search the form that generated the submit event */
   if ( (i = Html_find_form(submit, html_lb)) == -1 )
      return;

   /* Submit on enterpress when there's a single text-entry only,
    * or if the user set enter to always submit */
   if ((html_lb->forms[i].num_entry_fields == 1) ||
       prefs.enterpress_forces_submit)
      Html_submit_form(submit, html_lb, 1, 1);
}

/*
 * Call submit form, when input image has been clicked
 */
static void Html_image_clicked(DwWidget *widget, gint x, gint y,
                               DilloHtmlLB *lb)
{
   _MSG("Hallo! (%d, %d, %p)\n", x, y, lb);
   Html_submit_form((GtkWidget*) widget, lb, x, y);
}

/*
 * Create input image for the form
 */
static DwWidget *Html_input_image(DilloHtml *html, char *tag, gint tagsize,
                                  DilloHtmlLB *html_lb, DilloUrl *action)
{
   DilloImage *Image;
   DwWidget *button;
   DilloUrl *url = NULL;
   DwStyle style_attrs;
   const char *attrbuf;

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "src")) &&
       (url = Html_url_new(html, attrbuf, NULL, 0, 0, 0, 0))) {
      button = a_Dw_button_new (0, FALSE);
      a_Dw_page_add_widget (DW_PAGE (html->dw), button,
                            html->stack[html->stack_top].style);
      gtk_signal_connect(GTK_OBJECT(button), "clicked_at",
                         GTK_SIGNAL_FUNC(Html_image_clicked), html_lb);
      a_Dw_button_set_sensitive(DW_BUTTON(button), FALSE);

      /* create new image and add it to the button */
      if ((Image = Html_add_new_image(html, tag, tagsize, &style_attrs,
                                      FALSE))) {
         /* By suppressing the "image_pressed" signal, the events are sent
          * to the parent DwButton */
         a_Dw_widget_set_button_sensitive (DW_WIDGET (Image->dw), FALSE);
         a_Dw_widget_set_style(DW_WIDGET(Image->dw),
                               html->stack[html->stack_top].style);
         a_Dw_container_add(DW_CONTAINER(button), DW_WIDGET(Image->dw));
         a_Dw_widget_set_cursor(DW_WIDGET(Image->dw), Dw_cursor_hand);
         Html_load_image(html, url, Image);
         a_Url_free(url);
         return button;
      }
   }

   DEBUG_MSG(10, "Html_input_image: unable to create image submit.\n");
   a_Url_free(url);
   return NULL;
}

/*
 * Add a new input to current form
 */
static void Html_tag_open_input(DilloHtml *html, char *tag, gint tagsize)
{
   DilloHtmlForm *form;
   DilloHtmlInputType inp_type;
   DilloHtmlLB *html_lb;
   DwWidget *embed_gtk;
   GtkWidget *widget = NULL;
   GSList *group;
   char *value, *name, *type, *init_str;
   const char *attrbuf, *label;
   gboolean init_val = FALSE;
   gint input_index;

   if (!(html->InFlags & IN_FORM)) {
      MSG_HTML("input camp outside <form>\n");
      return;
   }

   html_lb = html->linkblock;
   form = &(html_lb->forms[html_lb->num_forms - 1]);

   /* Get 'value', 'name' and 'type' */
   value = Html_get_attr_wdef(html, tag, tagsize, "value", NULL);
   name = Html_get_attr_wdef(html, tag, tagsize, "name", NULL);
   type = Html_get_attr_wdef(html, tag, tagsize, "type", "");

   init_str = NULL;
   if (!g_strcasecmp(type, "password")) {
      inp_type = DILLO_HTML_INPUT_PASSWORD;
      widget = gtk_entry_new();
      gtk_entry_set_visibility(GTK_ENTRY(widget), FALSE);
      if (value)
         init_str = g_strdup(Html_get_attr(html, tag, tagsize, "value"));
   } else if (!g_strcasecmp(type, "checkbox")) {
      inp_type = DILLO_HTML_INPUT_CHECKBOX;
      widget = gtk_check_button_new();
      init_val = (Html_get_attr(html, tag, tagsize, "checked") != NULL);
      init_str = (value) ? value : g_strdup("on");
   } else if (!g_strcasecmp(type, "radio")) {
      inp_type = DILLO_HTML_INPUT_RADIO;
      group = NULL;
      for (input_index = 0; input_index < form->num_inputs; input_index++) {
         if (form->inputs[input_index].type == DILLO_HTML_INPUT_RADIO &&
             (form->inputs[input_index].name &&
              !g_strcasecmp(form->inputs[input_index].name, name)) ){
            group = gtk_radio_button_group(GTK_RADIO_BUTTON
                                           (form->inputs[input_index].widget));
            form->inputs[input_index].init_val = TRUE;
            break;
         }
      }
      widget = gtk_radio_button_new(group);

      init_val = (Html_get_attr(html, tag, tagsize, "checked") != NULL);
      init_str = (value) ? value : NULL;
   } else if (!g_strcasecmp(type, "hidden")) {
      inp_type = DILLO_HTML_INPUT_HIDDEN;
      if (value)
         init_str = g_strdup(Html_get_attr(html, tag, tagsize, "value"));
   } else if (!g_strcasecmp(type, "submit")) {
      inp_type = DILLO_HTML_INPUT_SUBMIT;
      init_str = (value) ? value : g_strdup("submit");
      widget = gtk_button_new_with_label(init_str);
      gtk_widget_set_sensitive(widget, FALSE); /* Until end of FORM! */
      gtk_signal_connect(GTK_OBJECT(widget), "clicked",
                         GTK_SIGNAL_FUNC(Html_submit_form), html_lb);
   } else if (!g_strcasecmp(type, "reset")) {
      inp_type = DILLO_HTML_INPUT_RESET;
      init_str = (value) ? value : g_strdup("Reset");
      widget = gtk_button_new_with_label(init_str);
      gtk_widget_set_sensitive(widget, FALSE); /* Until end of FORM! */
      gtk_signal_connect(GTK_OBJECT(widget), "clicked",
                         GTK_SIGNAL_FUNC(Html_reset_form), html_lb);
   } else if (!g_strcasecmp(type, "image")) {
      if (URL_FLAGS(html->linkblock->base_url) & URL_SpamSafe) {
         /* Don't request the image, make a text submit button instead */
         inp_type = DILLO_HTML_INPUT_SUBMIT;
         attrbuf = Html_get_attr(html, tag, tagsize, "alt");
         label = attrbuf ? attrbuf : value ? value : name ? name : "Submit";
         init_str = g_strdup(label);
         widget = gtk_button_new_with_label(init_str);
         gtk_widget_set_sensitive(widget, FALSE); /* Until end of FORM! */
         gtk_signal_connect(GTK_OBJECT(widget), "clicked",
                            GTK_SIGNAL_FUNC(Html_submit_form), html_lb);
      } else {
         inp_type = DILLO_HTML_INPUT_IMAGE;
         /* use a dw_image widget */
         widget = (GtkWidget*) Html_input_image(html, tag, tagsize,
                                                html_lb, form->action);
         init_str = value;
      }
   } else if (!g_strcasecmp(type, "file")) {
      /* todo: implement it! */
      inp_type = DILLO_HTML_INPUT_FILE;
      init_str = (value) ? value : NULL;
      MSG("An input of the type \"file\" wasn't rendered!\n");
   } else if (!g_strcasecmp(type, "button")) {
      inp_type = DILLO_HTML_INPUT_BUTTON;
      if (value) {
         init_str = value;
         widget = gtk_button_new_with_label(init_str);
      }
   } else {
      /* Text input, which also is the default */
      inp_type = DILLO_HTML_INPUT_TEXT;
      widget = gtk_entry_new();

      init_str = (value) ? value : NULL;
      gtk_signal_connect(GTK_OBJECT(widget), "activate",
                         GTK_SIGNAL_FUNC(Html_enter_submit_form),
                         html_lb);
   }

   Html_add_input(form, inp_type, widget, name,
                  (init_str) ? init_str : "", NULL, init_val);

   if (widget != NULL && inp_type != DILLO_HTML_INPUT_IMAGE) {
      if (inp_type == DILLO_HTML_INPUT_TEXT ||
          inp_type == DILLO_HTML_INPUT_PASSWORD) {
         /*
          * The following is necessary, because gtk_entry_button_press
          * returns FALSE, so the event would be delivered to the
          * GtkDwScrolledFrame, which then would be focused, instead of
          * the entry.
          */
         gtk_signal_connect_after(GTK_OBJECT(widget), "button_press_event",
                                  GTK_SIGNAL_FUNC(gtk_true), NULL);

         /* Readonly or not? */
         gtk_entry_set_editable(
            GTK_ENTRY(widget),
            !(Html_get_attr(html, tag, tagsize, "readonly")));

         /* Set width of the entry */
         if ((attrbuf = Html_get_attr(html, tag, tagsize, "size")))
            gtk_widget_set_usize(widget, (strtol(attrbuf, NULL, 10) + 1) *
                                 gdk_char_width(widget->style->font, '0'), 0);

         /* Maximum length of the text in the entry */
         if ((attrbuf = Html_get_attr(html, tag, tagsize, "maxlength")))
            gtk_entry_set_max_length(GTK_ENTRY(widget),
                                     strtol(attrbuf, NULL, 10));
      }
      gtk_widget_show(widget);

      embed_gtk = a_Dw_embed_gtk_new();
      a_Dw_embed_gtk_add_gtk (DW_EMBED_GTK (embed_gtk), widget);
      a_Dw_page_add_widget(DW_PAGE (html->dw), embed_gtk,
                           html->stack[html->stack_top].style);
   }

   g_free(type);
   g_free(name);
   if (init_str != value)
      g_free(init_str);
   g_free(value);
}

/*
 * The ISINDEX tag is just a deprecated form of <INPUT type=text> with
 * implied FORM, afaics.
 */
static void Html_tag_open_isindex(DilloHtml *html, char *tag, gint tagsize)
{
   DilloHtmlForm *form;
   DilloHtmlLB *html_lb;
   DilloUrl *action;
   GtkWidget *widget;
   DwWidget *embed_gtk;
   const char *attrbuf;

   html_lb = html->linkblock;

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "action")))
      action = Html_url_new(html, attrbuf, NULL, 0, 0, 0, 0);
   else
      action = a_Url_dup(html->linkblock->base_url);

   Html_form_new(html->linkblock, DILLO_HTML_METHOD_GET, action,
                 DILLO_HTML_ENC_URLENCODING);

   form = &(html_lb->forms[html_lb->num_forms - 1]);

   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);

   if ((attrbuf = Html_get_attr(html, tag, tagsize, "prompt")))
      a_Dw_page_add_text(DW_PAGE (html->dw), g_strdup(attrbuf),
                         html->stack[html->stack_top].style);

   widget = gtk_entry_new();
   Html_add_input(form, DILLO_HTML_INPUT_INDEX,
                  widget, NULL, NULL, NULL, FALSE);
   gtk_signal_connect(GTK_OBJECT(widget), "activate",
                      GTK_SIGNAL_FUNC(Html_enter_submit_form),
                      html_lb);
   gtk_widget_show(widget);
   /* compare <input type=text> */
   gtk_signal_connect_after(GTK_OBJECT(widget), "button_press_event",
                            GTK_SIGNAL_FUNC(gtk_true),
                            NULL);

   embed_gtk = a_Dw_embed_gtk_new();
   a_Dw_embed_gtk_add_gtk(DW_EMBED_GTK(embed_gtk), widget);
   a_Dw_page_add_widget(DW_PAGE (html->dw), embed_gtk,
                        html->stack[html->stack_top].style);

   a_Url_free(action);
}

/*
 * Close  textarea
 * (TEXTAREA is parsed in VERBATIM mode, and entities are handled here)
 */
static void Html_tag_close_textarea(DilloHtml *html, gint TagIdx)
{
   DilloHtmlLB *html_lb = html->linkblock;
   char *str;
   DilloHtmlForm *form;
   gint i;

   if (html->InFlags & IN_FORM && html->InFlags & IN_TEXTAREA) {
      /* Remove the line ending that follows the opening tag */
      if (html->Stash->str[0] == '\r')
         html->Stash = g_string_erase(html->Stash, 0, 1);
      if (html->Stash->str[0] == '\n')
         html->Stash = g_string_erase(html->Stash, 0, 1);
   
      /* As the spec recommends to canonicalize line endings, it is safe
       * to replace '\r' with '\n'. It will be canonicalized anyway! */
      for (i = 0; i < html->Stash->len; ++i) {
         if (html->Stash->str[i] == '\r') {
            if (html->Stash->str[i + 1] == '\n')
               g_string_erase(html->Stash, i, 1);
            else
               html->Stash->str[i] = '\n';
         }
      }
   
      /* The HTML3.2 spec says it can have "text and character entities". */
      str = Html_parse_entities(html, html->Stash->str, html->Stash->len);
   
      form = &(html_lb->forms[html_lb->num_forms - 1]);
      form->inputs[form->num_inputs - 1].init_str = str;
      gtk_text_insert(GTK_TEXT(form->inputs[form->num_inputs - 1].widget),
                      NULL, NULL, NULL, str, -1);
   
      html->InFlags &= ~IN_TEXTAREA;
   }
   Html_pop_tag(html, TagIdx);
}

/*
 * The textarea tag
 * (todo: It doesn't support wrapping).
 */
static void Html_tag_open_textarea(DilloHtml *html, char *tag, gint tagsize)
{
   DilloHtmlLB *html_lb;
   DilloHtmlForm *form;
   GtkWidget *widget;
   GtkWidget *scroll;
   DwWidget *embed_gtk;
   char *name;
   const char *attrbuf;
   int cols, rows;

   /* We can't push a new <FORM> because the 'action' URL is unknown */
   if (!(html->InFlags & IN_FORM)) {
      MSG_HTML("<textarea> outside <form>\n");
      html->ReqTagClose = TRUE;
      return;
   }
   if (html->InFlags & IN_TEXTAREA) {
      MSG_HTML("nested <textarea>\n");
      html->ReqTagClose = TRUE;
      return;
   }

   html->InFlags |= IN_TEXTAREA;
   html_lb = html->linkblock;
   form = &(html_lb->forms[html_lb->num_forms - 1]);
   Html_stash_init(html);
   html->stack[html->stack_top].parse_mode = DILLO_HTML_PARSE_MODE_VERBATIM;

   cols = 20;
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "cols")))
      cols = strtol(attrbuf, NULL, 10);
   rows = 10;
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "rows")))
      rows = strtol(attrbuf, NULL, 10);
   name = NULL;
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "name")))
      name = g_strdup(attrbuf);

   widget = gtk_text_new(NULL, NULL);
   /* compare <input type=text> */
   gtk_signal_connect_after(GTK_OBJECT(widget), "button_press_event",
                            GTK_SIGNAL_FUNC(gtk_true),
                            NULL);

   /* Calculate the width and height based on the cols and rows
    * todo: Get it right... Get the metrics from the font that will be used.
    */
   gtk_widget_set_usize(widget, 6 * cols, 16 * rows);

   /* If the attribute readonly isn't specified we make the textarea
    * editable. If readonly is set we don't have to do anything.
    */
   if (!Html_get_attr(html, tag, tagsize, "readonly"))
      gtk_text_set_editable(GTK_TEXT(widget), TRUE);

   scroll = gtk_scrolled_window_new(NULL, NULL);
   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);
   gtk_container_add(GTK_CONTAINER(scroll), widget);
   gtk_widget_show(widget);
   gtk_widget_show(scroll);

   Html_add_input(form, DILLO_HTML_INPUT_TEXTAREA,
                  widget, name, NULL, NULL, FALSE);
   g_free(name);

   embed_gtk = a_Dw_embed_gtk_new ();
   a_Dw_embed_gtk_add_gtk (DW_EMBED_GTK (embed_gtk), scroll);
   a_Dw_page_add_widget(DW_PAGE (html->dw), embed_gtk,
                        html->stack[html->stack_top].style);
}

/*
 * <SELECT>
 */
/* The select tag is quite tricky, because of gorpy html syntax. */
static void Html_tag_open_select(DilloHtml *html, char *tag, gint tagsize)
{
   DilloHtmlForm *form;
   DilloHtmlSelect *Select;
   DilloHtmlLB *html_lb;
   GtkWidget *widget, *menu;
   char *name;
   const char *attrbuf;
   gint size, type, multi;

   if (!(html->InFlags & IN_FORM)) {
      MSG_HTML("<select> outside <form>\n");
      return;
   }
   if (html->InFlags & IN_SELECT) {
      MSG_HTML("nested <select>\n");
      return;
   }
   html->InFlags |= IN_SELECT;

   html_lb = html->linkblock;

   form = &(html_lb->forms[html_lb->num_forms - 1]);

   name = Html_get_attr_wdef(html, tag, tagsize, "name", NULL);

   size = 0;
   if ((attrbuf = Html_get_attr(html, tag, tagsize, "size")))
      size = strtol(attrbuf, NULL, 10);

   multi = (Html_get_attr(html, tag, tagsize, "multiple")) ? 1 : 0;
   if (size < 1)
      size = multi ? 10 : 1;

   if (size == 1) {
      menu = gtk_menu_new();
      widget = gtk_option_menu_new();
      type = DILLO_HTML_INPUT_SELECT;
   } else {
      menu = gtk_list_new();
      widget = menu;
      if (multi)
         gtk_list_set_selection_mode(GTK_LIST(menu), GTK_SELECTION_MULTIPLE);
      type = DILLO_HTML_INPUT_SEL_LIST;
   }

   Select = g_new(DilloHtmlSelect, 1);
   Select->menu = menu;
   Select->size = size;
   Select->num_options = 0;
   Select->num_options_max = 8;
   Select->options = g_new(DilloHtmlOption, Select->num_options_max);

   Html_add_input(form, type, widget, name, NULL, Select, FALSE);
   Html_stash_init(html);
   g_free(name);
}

/*
 * ?
 */
static void Html_option_finish(DilloHtml *html)
{
   DilloHtmlForm *form;
   DilloHtmlInput *input;
   GtkWidget *menuitem;
   GSList *group;
   DilloHtmlSelect *select;

   if (!(html->InFlags & IN_FORM))
      return;

   form = &(html->linkblock->forms[html->linkblock->num_forms - 1]);
   input = &(form->inputs[form->num_inputs - 1]);
   if ( input->select->num_options <= 0)
      return;

   select = input->select;
   if (input->type == DILLO_HTML_INPUT_SELECT ) {
      if ( select->num_options == 1)
         group = NULL;
      else
         group = gtk_radio_menu_item_group(GTK_RADIO_MENU_ITEM
                                   (select->options[0].menuitem));
      menuitem = gtk_radio_menu_item_new_with_label(group, html->Stash->str);
      select->options[select->num_options - 1].menuitem = menuitem;
      if ( select->options[select->num_options - 1].value == NULL )
         select->options[select->num_options - 1].value =
             g_strdup(html->Stash->str);
      gtk_menu_append(GTK_MENU(select->menu), menuitem);
      if ( select->options[select->num_options - 1].init_val )
         gtk_menu_item_activate(GTK_MENU_ITEM(menuitem));
      gtk_widget_show(menuitem);
      gtk_signal_connect (GTK_OBJECT (menuitem), "select",
                          GTK_SIGNAL_FUNC (a_Interface_scroll_popup),
                          NULL);
   } else if ( input->type == DILLO_HTML_INPUT_SEL_LIST ) {
      menuitem = gtk_list_item_new_with_label(html->Stash->str);
      select->options[select->num_options - 1].menuitem = menuitem;
      if (select->options[select->num_options - 1].value == NULL)
         select->options[select->num_options - 1].value =
             g_strdup(html->Stash->str);
      gtk_container_add(GTK_CONTAINER(select->menu), menuitem);
      if ( select->options[select->num_options - 1].init_val )
         gtk_list_select_child(GTK_LIST(select->menu), menuitem);
      gtk_widget_show(menuitem);
   }
}

/*
 * <OPTION>
 */
static void Html_tag_open_option(DilloHtml *html, char *tag, gint tagsize)
{
   DilloHtmlForm *form;
   DilloHtmlInput *input;
   DilloHtmlLB *html_lb;
   gint no;

   if (!(html->InFlags & IN_SELECT))
      return;

   html_lb = html->linkblock;

   form = &(html_lb->forms[html_lb->num_forms - 1]);
   input = &(form->inputs[form->num_inputs - 1]);
   if (input->type == DILLO_HTML_INPUT_SELECT ||
       input->type == DILLO_HTML_INPUT_SEL_LIST) {
      Html_option_finish(html);
      no = input->select->num_options;
      a_List_add(input->select->options, no, input->select->num_options_max);
      input->select->options[no].menuitem = NULL;
      input->select->options[no].value = Html_get_attr_wdef(html, tag, tagsize,
                                                           "value", NULL);
      input->select->options[no].init_val =
         (Html_get_attr(html, tag, tagsize, "selected") != NULL);
      input->select->num_options++;
   }
   Html_stash_init(html);
}

/*
 * ?
 */
static void Html_tag_close_select(DilloHtml *html, gint TagIdx)
{
   DilloHtmlForm *form;
   DilloHtmlInput *input;
   GtkWidget *scrolledwindow;
   DilloHtmlLB *html_lb;
   DwWidget *embed_gtk;
   GtkRequisition req;
   gint height;

   if (html->InFlags & IN_SELECT) {
      html->InFlags &= ~IN_SELECT;
   
      html_lb = html->linkblock;
   
      form = &(html_lb->forms[html_lb->num_forms - 1]);
      input = &(form->inputs[form->num_inputs - 1]);
      if (input->type == DILLO_HTML_INPUT_SELECT) {
         Html_option_finish(html);
   
         gtk_option_menu_set_menu(GTK_OPTION_MENU(input->widget),
                                  input->select->menu);
         Html_select_set_history(input);

         // gtk_option_menu_set_history(GTK_OPTION_MENU(input->widget), 1);

         gtk_widget_show(input->widget);
   
         embed_gtk = a_Dw_embed_gtk_new ();
         a_Dw_embed_gtk_add_gtk (DW_EMBED_GTK (embed_gtk), input->widget);
         a_Dw_page_add_widget(DW_PAGE (html->dw), embed_gtk,
                              html->stack[html->stack_top].style);
      } else if (input->type == DILLO_HTML_INPUT_SEL_LIST) {
         Html_option_finish(html);
   
         if (input->select->size < input->select->num_options) {
            scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
            gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
                                           GTK_POLICY_NEVER,
                                           GTK_POLICY_AUTOMATIC);
            gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW
                                                  (scrolledwindow),
                                                  input->widget);
   
            gtk_container_set_focus_vadjustment
               (GTK_CONTAINER (input->widget),
                gtk_scrolled_window_get_vadjustment
                (GTK_SCROLLED_WINDOW(scrolledwindow)));
   
            /* Calculate the height of the scrolled window */
            gtk_widget_size_request(input->select->options[0].menuitem, &req);
            height = input->select->size * req.height +
               2 * scrolledwindow->style->klass->ythickness;
            gtk_widget_set_usize(scrolledwindow, -1, height);
   
            gtk_widget_show(input->widget);
            input->widget = scrolledwindow;
         }
         gtk_widget_show(input->widget);
   
         /* note: In this next call, scrolledwindows get a g_warning from
          * gdkwindow.c:422. I'm not really going to sweat it now - the
          * embedded widget stuff is going to get massively redone anyway. */
         embed_gtk = a_Dw_embed_gtk_new ();
         a_Dw_embed_gtk_add_gtk (DW_EMBED_GTK (embed_gtk), input->widget);
         a_Dw_page_add_widget(DW_PAGE (html->dw), embed_gtk,
                              html->stack[html->stack_top].style);
      }
   }
   Html_pop_tag(html, TagIdx);
}

/*
 * Set the Document Base URI
 */
static void Html_tag_open_base(DilloHtml *html, char *tag, gint tagsize)
{
   const char *attrbuf;
   DilloUrl *BaseUrl;

   if (html->InFlags & IN_HEAD) {
      if ((attrbuf = Html_get_attr(html, tag, tagsize, "href"))) {
         BaseUrl = Html_url_new(html, attrbuf, "", 0, 0, 0, 1);
         if (URL_SCHEME_(BaseUrl)) {
            /* Pass the URL_SpamSafe flag to the new base url */
            a_Url_set_flags(
               BaseUrl, URL_FLAGS(html->linkblock->base_url) & URL_SpamSafe);
            a_Url_free(html->linkblock->base_url);
            html->linkblock->base_url = BaseUrl;
         } else {
            MSG_HTML("base URI is relative (it MUST be absolute)\n");
            a_Url_free(BaseUrl);
         }
      }
   } else {
      MSG_HTML("the BASE element must appear in the HEAD section\n");
   }
}

/*
 * <CODE>
 */
static void Html_tag_open_code(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, prefs.fw_fontname, 0, 0, 0);
}

/*
 * <DFN>
 */
static void Html_tag_open_dfn(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, NULL, 0, 2, 3);
}

/*
 * <KBD>
 */
static void Html_tag_open_kbd(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, prefs.fw_fontname, 0, 0, 0);
}

/*
 * <SAMP>
 */
static void Html_tag_open_samp(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, prefs.fw_fontname, 0, 0, 0);
}

/*
 * <VAR>
 */
static void Html_tag_open_var(DilloHtml *html, char *tag, gint tagsize)
{
   Html_set_top_font(html, NULL, 0, 2, 2);
}

/*
 * <SUB>
 */
static void Html_tag_open_sub(DilloHtml *html, char *tag, gint tagsize)
{
   HTML_SET_TOP_ATTR (html, valign, DW_STYLE_VALIGN_SUB);
}

/*
 * <SUP>
 */
static void Html_tag_open_sup(DilloHtml *html, char *tag, gint tagsize)
{
   HTML_SET_TOP_ATTR (html, valign, DW_STYLE_VALIGN_SUPER);
}

/*
 * <DIV> (todo: make a complete implementation)
 */
static void Html_tag_open_div(DilloHtml *html, char *tag, gint tagsize)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 0,
                          html->stack[(html)->stack_top].style);
   Html_tag_set_align_attr (html, tag, tagsize);
}

/*
 * </DIV>, also used for </CENTER>
 */
static void Html_tag_close_div(DilloHtml *html, gint TagIdx)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 0,
                          html->stack[(html)->stack_top].style);
   Html_pop_tag(html, TagIdx);
}

/*
 * Default close for most tags - just pop the stack.
 */
static void Html_tag_close_default(DilloHtml *html, gint TagIdx)
{
   Html_pop_tag(html, TagIdx);
}

/*
 * Default close for paragraph tags - pop the stack and break.
 */
static void Html_tag_close_par(DilloHtml *html, gint TagIdx)
{
   a_Dw_page_add_parbreak(DW_PAGE (html->dw), 9,
                          html->stack[(html)->stack_top].style);
   Html_pop_tag(html, TagIdx);
}


/*
 * Function index for the open and close functions for each tag
 * (Alphabetically sorted for a binary search)
 *
 * Explanation for the 'Flags' camp:
 *
 *   {"address", B8(010110), ...}
 *                  |||||`- inline element
 *                  ||||`-- block element
 *                  |||`--- inline container
 *                  ||`---- block container
 *                  |`----- body element
 *                  `------ head element
 *
 *   Notes:
 *     - The upper two bits are not used yet.
 *     - Empty elements have both inline and block container clear.
 *       (flow have both set)
 */
struct _TagInfo{
   gchar *name;
   unsigned char Flags;
   gchar EndTag;
   guchar TagLevel;
   TagOpenFunct open;
   TagCloseFunct close;
};


static const TagInfo Tags[] = {
 {"a", B8(010101),'R',2, Html_tag_open_a, Html_tag_close_a},
 {"abbr", B8(010101),'R',2, Html_tag_open_abbr, Html_tag_close_default},
 /* acronym 010101 */
 {"address", B8(010110),'R',2, Html_tag_open_address, Html_tag_close_par},
 {"area", B8(010001),'F',0, Html_tag_open_area, Html_tag_close_default},
 {"b", B8(010101),'R',2, Html_tag_open_b, Html_tag_close_default},
 {"base", B8(100001),'F',0, Html_tag_open_base, Html_tag_close_default},
 /* basefont 010001 */
 /* bdo 010101 */
 {"big", B8(010101),'R',2, Html_tag_open_big_small, Html_tag_close_default},
 {"blockquote", B8(011110),'R',2,Html_tag_open_blockquote,Html_tag_close_par},
 {"body", B8(011110),'O',7, Html_tag_open_body, Html_tag_close_body},
 {"br", B8(010001),'F',0, Html_tag_open_br, Html_tag_close_default},
 {"button", B8(011101),'R',2, Html_tag_open_button, Html_tag_close_default},
 /* caption */
 {"center", B8(011110),'R',2, Html_tag_open_center, Html_tag_close_div},
 {"cite", B8(010101),'R',2, Html_tag_open_cite, Html_tag_close_default},
 {"code", B8(010101),'R',2, Html_tag_open_code, Html_tag_close_default},
 /* col 010010 'F' */
 /* colgroup */
 {"dd", B8(011110),'O',1, Html_tag_open_dd, Html_tag_close_par},
 {"del", B8(011101),'R',2, Html_tag_open_strike, Html_tag_close_default},
 {"dfn", B8(010101),'R',2, Html_tag_open_dfn, Html_tag_close_default},
 /* dir 011010 */
 /* todo: complete <div> support! */
 {"div", B8(011110),'R',2, Html_tag_open_div, Html_tag_close_div},
 {"dl", B8(011010),'R',2, Html_tag_open_dl, Html_tag_close_par},
 {"dt", B8(010110),'O',1, Html_tag_open_dt, Html_tag_close_par},
 {"em", B8(010101),'R',2, Html_tag_open_em, Html_tag_close_default},
 /* fieldset */
 {"font", B8(010101),'R',2, Html_tag_open_font, Html_tag_close_default},
 {"form", B8(011110),'R',2, Html_tag_open_form, Html_tag_close_form},
 {"frame", B8(010010),'F',0, Html_tag_open_frame, Html_tag_close_default},
 {"frameset", B8(011110),'R',2,Html_tag_open_frameset, Html_tag_close_default},
 {"h1", B8(010110),'R',2, Html_tag_open_h, Html_tag_close_h},
 {"h2", B8(010110),'R',2, Html_tag_open_h, Html_tag_close_h},
 {"h3", B8(010110),'R',2, Html_tag_open_h, Html_tag_close_h},
 {"h4", B8(010110),'R',2, Html_tag_open_h, Html_tag_close_h},
 {"h5", B8(010110),'R',2, Html_tag_open_h, Html_tag_close_h},
 {"h6", B8(010110),'R',2, Html_tag_open_h, Html_tag_close_h},
 {"head", B8(101101),'O',1, Html_tag_open_head, Html_tag_close_head},
 {"hr", B8(010010),'F',0, Html_tag_open_hr, Html_tag_close_default},
 {"html", B8(001110),'O',8, Html_tag_open_html, Html_tag_close_html},
 {"i", B8(010101),'R',2, Html_tag_open_i, Html_tag_close_default},
 {"iframe", B8(011110),'R',2, Html_tag_open_frame, Html_tag_close_default},
 {"img", B8(010001),'F',0, Html_tag_open_img, Html_tag_close_default},
 {"input", B8(010001),'F',0, Html_tag_open_input, Html_tag_close_default},
 /* ins */
 {"isindex", B8(110001),'F',0, Html_tag_open_isindex, Html_tag_close_default},
 {"kbd", B8(010101),'R',2, Html_tag_open_kbd, Html_tag_close_default},
 /* label 010101 */
 /* legend 01?? */
 {"li", B8(011110),'O',1, Html_tag_open_li, Html_tag_close_default},
 /* link 100000 'F' */
 {"map", B8(011001),'R',2, Html_tag_open_map, Html_tag_close_map},
 /* menu 1010 -- todo: not exactly 1010, it can contain LI and inline */
 {"menu", B8(011010),'R',2, Html_tag_open_menu, Html_tag_close_par},
 {"meta", B8(100001),'F',0, Html_tag_open_meta, Html_tag_close_default},
 /* noframes 1011 */
 /* noscript 1011 */
 /* object 11xxxx */
 {"ol", B8(011010),'R',2, Html_tag_open_ol, Html_tag_close_par},
 /* optgroup */
 {"option", B8(010001),'O',1, Html_tag_open_option, Html_tag_close_default},
 {"p", B8(010110),'O',1, Html_tag_open_p, Html_tag_close_par},
 /* param 010001 'F' */
 {"pre", B8(010110),'R',2, Html_tag_open_pre, Html_tag_close_pre},
 /* q 010101 */
 {"s", B8(010101),'R',2, Html_tag_open_strike, Html_tag_close_default},
 {"samp", B8(010101),'R',2, Html_tag_open_samp, Html_tag_close_default},
 {"script", B8(111001),'R',2, Html_tag_open_script, Html_tag_close_script},
 {"select", B8(011001),'R',2, Html_tag_open_select, Html_tag_close_select},
 {"small", B8(010101),'R',2, Html_tag_open_big_small, Html_tag_close_default},
 /* span 0101 */
 {"strike", B8(010101),'R',2, Html_tag_open_strike, Html_tag_close_default},
 {"strong", B8(010101),'R',2, Html_tag_open_strong, Html_tag_close_default},
 {"style", B8(100101),'R',2, Html_tag_open_style, Html_tag_close_style},
 {"sub", B8(010101),'R',2, Html_tag_open_sub, Html_tag_close_default},
 {"sup", B8(010101),'R',2, Html_tag_open_sup, Html_tag_close_default},
 {"table", B8(011010),'R',5, Html_tag_open_table, Html_tag_close_div},
 /* tbody */
 {"td", B8(011110),'O',3, Html_tag_open_td, Html_tag_close_default},
 {"textarea", B8(010101),'R',2,Html_tag_open_textarea,Html_tag_close_textarea},
 /* tfoot */
 {"th", B8(011110),'O',1, Html_tag_open_th, Html_tag_close_default},
 /* thead */
 {"title", B8(100101),'R',2, Html_tag_open_title, Html_tag_close_title},
 {"tr", B8(011010),'O',4, Html_tag_open_tr, Html_tag_close_default},
 {"tt", B8(010101),'R',2, Html_tag_open_tt, Html_tag_close_default},
 {"u", B8(010101),'R',2, Html_tag_open_u, Html_tag_close_default},
 {"ul", B8(011010),'R',2, Html_tag_open_ul, Html_tag_close_par},
 {"var", B8(010101),'R',2, Html_tag_open_var, Html_tag_close_default}

};
#define NTAGS (sizeof(Tags)/sizeof(Tags[0]))


/*
 * Compares tag from buffer ('/' or '>' or space-ended string) [p1]
 * with tag from taglist (lowercase, zero ended string) [p2]
 * Return value: as strcmp()
 */
static gint Html_tag_compare(char *p1, char *p2)
{
   while ( *p2 ) {
      if ( tolower(*p1) != *p2 )
         return(tolower(*p1) - *p2);
      ++p1;
      ++p2;
   }
   return !strchr(" >/\n\r\t", *p1);
}

/*
 * Get 'tag' index
 * return -1 if tag is not handled yet
 */
static gint Html_tag_index(char *tag)
{
   gint low, high, mid, cond;

   /* Binary search */
   low = 0;
   high = NTAGS - 1;          /* Last tag index */
   while (low <= high) {
      mid = (low + high) / 2;
      if ((cond = Html_tag_compare(tag, Tags[mid].name)) < 0 )
         high = mid - 1;
      else if (cond > 0)
         low = mid + 1;
      else
         return mid;
   }
   return -1;
}

/*
 * For elements with optional close, check whether is time to close.
 * Return value: (1: Close, 0: Don't close)
 * --tuned for speed.
 */
static gint Html_needs_optional_close(gint old_idx, gint cur_idx)
{
   static gint i_P = -1, i_LI, i_TD, i_TR, i_TH, i_DD, i_DT, i_OPTION;
               // i_THEAD, i_TFOOT, i_COLGROUP;

   if (i_P == -1) {
    /* initialize the indexes of elements with optional close */
    i_P  = Html_tag_index("p"),
    i_LI = Html_tag_index("li"),
    i_TD = Html_tag_index("td"),
    i_TR = Html_tag_index("tr"),
    i_TH = Html_tag_index("th"),
    i_DD = Html_tag_index("dd"),
    i_DT = Html_tag_index("dt"),
    i_OPTION = Html_tag_index("option");
    // i_THEAD = Html_tag_index("thead");
    // i_TFOOT = Html_tag_index("tfoot");
    // i_COLGROUP = Html_tag_index("colgroup");
   }

   if (old_idx == i_P || old_idx == i_DT) {
      /* P and DT are closed by block elements */
      return (Tags[cur_idx].Flags & 2);
   } else if (old_idx == i_LI) {
      /* LI closes LI */
      return (cur_idx == i_LI);
   } else if (old_idx == i_TD || old_idx == i_TH) {
      /* TD and TH are closed by TD, TH and TR */
      return (cur_idx == i_TD || cur_idx == i_TH || cur_idx == i_TR);
   } else if (old_idx == i_TR) {
      /* TR closes TR */
      return (cur_idx == i_TR);
   } else if (old_idx ==  i_DD) {
      /* DD is closed by DD and DT */
      return (cur_idx == i_DD || cur_idx == i_DT);
   } else if (old_idx ==  i_OPTION) {
      return 1;  // OPTION always needs close
   }

   /* HTML, HEAD, BODY are handled by Html_test_section(), not here. */
   /* todo: TBODY is pending */
   return 0;
}


/*
 * Conditional cleanup of the stack (at open time).
 * - This helps catching block elements inside inline containers (a BUG).
 * - It also closes elements with "optional" close tag.
 *
 * This function is called when opening a block element or <OPTION>.
 *
 * It searches the stack closing open inline containers, and closing
 * elements with optional close tag when necessary.
 *
 * Note: OPTION is the only non-block element with an optional close.
 */
static void Html_stack_cleanup_at_open(DilloHtml *html, gint new_idx)
{
   /* We know that the element we're about to push is a block element.
    * (except for OPTION, which is an empty inline, so is closed anyway)
    * Notes:
    *   Its 'tag' is not yet pushed into the stack,
    *   'new_idx' is its index inside Tags[].
    */

   if (!html->TagSoup)
      return;

   while (html->stack_top) {
      gint oldtag_idx = html->stack[html->stack_top].tag_idx;

      if (Tags[oldtag_idx].EndTag == 'O') {    // Element with optional close
         if (!Html_needs_optional_close(oldtag_idx, new_idx))
            break;
      } else if (Tags[oldtag_idx].Flags & 8) { // Block container
         break;
      }

      /* we have an inline (or empty) container... */
      if (Tags[oldtag_idx].EndTag == 'R') {
         MSG_HTML("<%s> is not allowed to contain <%s>. -- closing <%s>\n",
                  Tags[oldtag_idx].name, Tags[new_idx].name,
                  Tags[oldtag_idx].name);
      }

      /* Workaround for Apache and its bad HTML directory listings... */
      if ((html->InFlags & IN_PRE) &&
          strcmp(Tags[new_idx].name, "hr") == 0)
         break;

      /* This call closes the top tag only. */
      Html_tag_cleanup_at_close(html, oldtag_idx);
   }
}

/*
 * HTML, HEAD and BODY elements have optional open and close tags.
 * Handle this "magic" here.
 */
static void Html_test_section(DilloHtml *html, gint new_idx, gint IsCloseTag)
{
   gchar *tag;
   gint tag_idx;

   if (!(html->InFlags & IN_HTML)) {
      tag = "<html>";
      tag_idx = Html_tag_index(tag + 1);
      if (tag_idx != new_idx || IsCloseTag) {
         /* implicit open */
         Html_force_push_tag(html, tag_idx);
         Tags[tag_idx].open (html, tag, strlen(tag));
      }
   }

   if (Tags[new_idx].Flags & 32) {
      /* head element */
      if (!(html->InFlags & IN_HEAD)) {
         tag = "<head>";
         tag_idx = Html_tag_index(tag + 1);
         if (tag_idx != new_idx || IsCloseTag) {
            /* implicit open of the head element */
            Html_force_push_tag(html, tag_idx);
            Tags[tag_idx].open (html, tag, strlen(tag));
         }
      }

   } else if (Tags[new_idx].Flags & 16) {
      /* body element */
      if (html->InFlags & IN_HEAD) {
         tag = "</head>";
         tag_idx = Html_tag_index(tag + 2);
         Tags[tag_idx].close (html, tag_idx);
      }
      tag = "<body>";
      tag_idx = Html_tag_index(tag + 1);
      if (tag_idx != new_idx || IsCloseTag) {
         /* implicit open */
         Html_force_push_tag(html, tag_idx);
         Tags[tag_idx].open (html, tag, strlen(tag));
      }
   }
}

/*
 * Process a tag, given as 'tag' and 'tagsize'. -- tagsize is [1 based]
 * ('tag' must include the enclosing angle brackets)
 * This function calls the right open or close function for the tag.
 */
static void Html_process_tag(DilloHtml *html, char *tag, gint tagsize)
{
   gint ci, ni;           /* current and new tag indexes */
   const char *attrbuf;
   char *start = tag + 1; /* discard the '<' */
   gint IsCloseTag = (*start == '/');

   /* Handle HTML, HEAD and BODY. Elements with optional open and close */
   ni = Html_tag_index(start + IsCloseTag);
   if (ni != -1 && !(html->InFlags & IN_BODY) /* && parsing HTML */)
      Html_test_section(html, ni, IsCloseTag);

   /* White space handling */
   if (html->SPCPending && (!SGML_SPCDEL || !IsCloseTag))
      /* SGML_SPCDEL requires space pending and open tag */
      a_Dw_page_add_space(DW_PAGE (html->dw),
                          html->stack[html->stack_top].style);
   html->SPCPending = FALSE;

   /* Tag processing */
   ci = html->stack[html->stack_top].tag_idx;
   if (ni != -1) {

      if (!IsCloseTag) {
         /* Open function */

         /* Cleanup when opening a block element, or
          * when openning over an element with optional close */
         if (Tags[ni].Flags & 2 || (ci != -1 && Tags[ci].EndTag == 'O'))
            Html_stack_cleanup_at_open(html, ni);

         /* todo: this is only raising a warning, take some defined action.
          * Note: apache uses IMG inside PRE (we could use its "alt"). */
         if ((html->InFlags & IN_PRE) && Html_tag_pre_excludes(ni))
            MSG_HTML("<pre> is not allowed to contain <%s>\n", Tags[ni].name);

         /* Push the tag into the stack */
         Html_push_tag(html, ni);

         /* Call the open function for this tag */
         Tags[ni].open (html, tag, tagsize);

         /* Now parse attributes that can appear on any tag */
         if (tagsize >= 8 &&        /* length of "<t id=i>" */
             (attrbuf = Html_get_attr2(html, tag, tagsize, "id",
                                       HTML_LeftTrim | HTML_RightTrim))) {
            /* According to the SGML declaration of HTML 4, all NAME values
             * occuring outside entities must be converted to uppercase
             * (this is what "NAMECASE GENERAL YES" says). But the HTML 4
             * spec states in Sec. 7.5.2 that anchor ids are case-sensitive.
             * So we don't do it and hope for better specs in the future ...
             */
            Html_check_name_val(html, attrbuf, "id");
            /* We compare the "id" value with the url-decoded "name" value */
            if (!html->NameVal || strcmp(html->NameVal, attrbuf)) {
               if (html->NameVal)
                  MSG_HTML("'id' and 'name' attribute of <a> tag differ\n");
               Html_add_anchor(html, attrbuf);
            }
         }

         /* Reset NameVal */
         if (html->NameVal) {
            g_free(html->NameVal);
            html->NameVal = NULL;
         }

         /* let the parser know this was an open tag */
         html->PrevWasOpenTag = TRUE;

         /* Request inmediate close for elements with forbidden close tag. */
         /* todo: XHTML always requires close tags. A simple implementation
          * of the commented clause below will make it work. */
         if  (/* parsing HTML && */ Tags[ni].EndTag == 'F')
            html->ReqTagClose = TRUE;
      }

      /* Close function: test for </x>, ReqTagClose, <x /> and <x/> */
      if (*start == '/' ||                                      /* </x>    */
          html->ReqTagClose ||                                  /* request */
          (tag[tagsize - 2] == '/' &&                           /* XML:    */
           (isspace(tag[tagsize - 3]) ||                        /*  <x />  */
            (size_t)tagsize == strlen(Tags[ni].name) + 3))) {   /*  <x/>   */

         Tags[ni].close (html, ni);
         /* This was a close tag */
         html->PrevWasOpenTag = FALSE;
         html->ReqTagClose = FALSE;
      }

   } else {
      /* tag not working - just ignore it */
   }
}

/*
 * Get attribute value for 'attrname' and return it.
 *  Tags start with '<' and end with a '>' (Ex: "<P align=center>")
 *  tagsize = strlen(tag) from '<' to '>', inclusive.
 *
 * Returns one of the following:
 *    * The value of the attribute.
 *    * An empty string if the attribute exists but has no value.
 *    * NULL if the attribute doesn't exist.
 */
static const char *Html_get_attr2(DilloHtml *html,
                                  const char *tag,
                                  gint tagsize,
                                  const char *attrname,
                                  DilloHtmlTagParsingFlags flags)
{
   gint i, isocode, entsize, Found = 0, delimiter = 0, attr_pos = 0;
   GString *Buf = html->attr_data;
   DilloHtmlTagParsingState state = SEEK_ATTR_START;

   g_return_val_if_fail(*attrname, NULL);

   g_string_truncate(Buf, 0);

   for (i = 1; i < tagsize; ++i) {
      switch (state) {
      case SEEK_ATTR_START:
         if (isspace(tag[i]))
            state = SEEK_TOKEN_START;
         else if (tag[i] == '=')
            state = SEEK_VALUE_START;
         break;

      case MATCH_ATTR_NAME:
         if ((Found = (!(attrname[attr_pos]) &&
                       (tag[i] == '=' || isspace(tag[i]) || tag[i] == '>')))) {
            state = SEEK_TOKEN_START;
            --i;
         } else if (tolower(tag[i]) != tolower(attrname[attr_pos++]))
            state = SEEK_ATTR_START;
         break;

      case SEEK_TOKEN_START:
         if (tag[i] == '=') {
            state = SEEK_VALUE_START;
         } else if (!isspace(tag[i])) {
            attr_pos = 0;
            state = (Found) ? FINISHED : MATCH_ATTR_NAME;
            --i;
         }
         break;
      case SEEK_VALUE_START:
         if (!isspace(tag[i])) {
            delimiter = (tag[i] == '"' || tag[i] == '\'') ? tag[i] : ' ';
            i -= (delimiter == ' ');
            state = (Found) ? GET_VALUE : SKIP_VALUE;
         }
         break;

      case SKIP_VALUE:
         if ((delimiter == ' ' && isspace(tag[i])) || tag[i] == delimiter)
            state = SEEK_TOKEN_START;
         break;
      case GET_VALUE:
         if ((delimiter == ' ' && (isspace(tag[i]) || tag[i] == '>')) ||
             tag[i] == delimiter) {
            state = FINISHED;
         } else if (tag[i] == '&' && (flags & HTML_ParseEntities)) {
            if ((isocode = Html_parse_entity(html, tag+i,
                                             tagsize-i, &entsize)) >= 0) {
               g_string_append_c(Buf, (gchar) isocode);
               i += entsize-1;
            } else {
               g_string_append_c(Buf, tag[i]);
            }
         } else if (tag[i] == '\r' || tag[i] == '\t') {
            g_string_append_c(Buf, ' ');
         } else if (tag[i] == '\n') {
            /* ignore */
         } else {
            g_string_append_c(Buf, tag[i]);
         }
         break;

      case FINISHED:
         i = tagsize;
         break;
      }
   }

   if (flags & HTML_LeftTrim)
      while (isspace(Buf->str[0]))
         g_string_erase(Buf, 0, 1);
   if (flags & HTML_RightTrim)
      while (Buf->len && isspace(Buf->str[Buf->len - 1]))
         g_string_truncate(Buf, Buf->len - 1);

   return (Found) ? Buf->str : NULL;
}

/*
 * Call Html_get_attr2 telling it to parse entities and strip the result
 */
static const char *Html_get_attr(DilloHtml *html,
                                 const char *tag,
                                 gint tagsize,
                                 const char *attrname)
{
   return Html_get_attr2(html, tag, tagsize, attrname,
                         HTML_LeftTrim | HTML_RightTrim | HTML_ParseEntities);
}

/*
 * "Html_get_attr with default"
 * Call Html_get_attr() and strdup() the returned string.
 * If the attribute isn't found a copy of 'def' is returned.
 */
static char *Html_get_attr_wdef(DilloHtml *html,
                               const char *tag,
                               gint tagsize,
                               const char *attrname,
                               const char *def)
{
   const char *attrbuf = Html_get_attr(html, tag, tagsize, attrname);

   return attrbuf ? g_strdup(attrbuf) : g_strdup(def);
}

/*
 * Add a widget to the page.
 */
static void Html_add_widget(DilloHtml *html,
                            DwWidget *widget,
                            char *width_str,
                            char *height_str,
                            DwStyle *style_attrs)
{
   DwStyle new_style_attrs, *style;

   new_style_attrs = *style_attrs;
   new_style_attrs.width = width_str ?
      Html_parse_length (html, width_str) : DW_STYLE_LENGTH_AUTO;
   new_style_attrs.height = height_str ?
      Html_parse_length (html, height_str) : DW_STYLE_LENGTH_AUTO;
   style = a_Dw_style_new (&new_style_attrs, (html)->bw->main_window->window);
   a_Dw_page_add_widget(DW_PAGE (html->dw), widget, style);
   a_Dw_style_unref (style);
}


/*
 * Dispatch the apropriate function for 'Op'
 * This function is a Cache client and gets called whenever new data arrives
 *  Op      : operation to perform.
 *  CbData  : a pointer to a DilloHtml structure
 *  Buf     : a pointer to new data
 *  BufSize : new data size (in bytes)
 */
static void Html_callback(int Op, CacheClient_t *Client)
{
   if ( Op ) {
      Html_write(Client->CbData, Client->Buf, Client->BufSize, 1);
      Html_close(Client->CbData, Client->Key);
   } else
      Html_write(Client->CbData, Client->Buf, Client->BufSize, 0);
}

/*
 * Here's where we parse the html and put it into the page structure.
 * Return value: number of bytes parsed
 */
static gint Html_write_raw(DilloHtml *html, char *buf, gint bufsize, gint Eof)
{
   char ch = 0, *p, *text;
   DwPage *page;
   gint token_start, buf_index;

   g_return_val_if_fail ((page = DW_PAGE (html->dw)) != NULL, 0);

   buf = g_strndup(buf, bufsize);

   /* Now, 'buf' and 'bufsize' define a buffer aligned to start at a token
    * boundary. Iterate through tokens until end of buffer is reached. */
   buf_index = 0;
   token_start = buf_index;
   while (buf_index < bufsize) {
      /* invariant: buf_index == bufsize || token_start == buf_index */

      if (html->stack[html->stack_top].parse_mode ==
          DILLO_HTML_PARSE_MODE_VERBATIM) {
         /* Non HTML code here, let's skip until closing tag */
         do {
            char *tag = html->stack[html->stack_top].tag_name;
            buf_index += strcspn(buf + buf_index, "<");
            if (buf_index + (gint)strlen(tag) + 3 > bufsize) {
               buf_index = bufsize;
            } else if (strncmp(buf + buf_index, "</", 2) == 0 &&
                       Html_match_tag(tag, buf+buf_index+2, strlen(tag)+1)) {
               /* copy VERBATIM text into the stash buffer */
               text = g_strndup(buf + token_start, buf_index - token_start);
               g_string_append(html->Stash, text);
               g_free(text);
               token_start = buf_index;
               break;
            } else
               ++buf_index;
         } while (buf_index < bufsize);
      }

      if ( isspace(buf[buf_index]) ) {
         /* whitespace: group all available whitespace */
         while (++buf_index < bufsize && isspace(buf[buf_index]));
         Html_process_space(html, buf + token_start, buf_index - token_start);
         token_start = buf_index;

      } else if (buf[buf_index] == '<' && (ch = buf[buf_index + 1]) &&
                 (isalpha(ch) || strchr("/!?", ch)) ) {
         /* Tag */
         if (buf_index + 3 < bufsize && !strncmp(buf + buf_index, "<!--", 4)) {
            /* Comment: search for close of comment, skipping over
             * everything except a matching "-->" tag. */
            while ( (p = memchr(buf + buf_index, '>', bufsize - buf_index)) ){
               buf_index = p - buf + 1;
               if ( p[-1] == '-' && p[-2] == '-' ) break;
            }
            if ( p ) {
               /* Got the whole comment. Let's throw it away! :) */
               token_start = buf_index;
            } else
               buf_index = bufsize;
         } else {
            /* Tag: search end of tag (skipping over quoted strings) */
            html->CurrTagOfs = html->Start_Ofs + token_start;

            while ( buf_index < bufsize ) {
               buf_index++;
               buf_index += strcspn(buf + buf_index, ">\"'<");
               if ( (ch = buf[buf_index]) == '>' ) {
                  break;
               } else if ( ch == '"' || ch == '\'' ) {
                  /* Skip over quoted string */
                  buf_index++;
                  buf_index += strcspn(buf + buf_index,
                                       (ch == '"') ? "\">" : "'>");
                  if ( buf[buf_index] == '>' ) {
                     /* Unterminated string value? Let's look ahead and test:
                      * (<: unterminated, closing-quote: terminated) */
                     gint offset = buf_index + 1;
                     offset += strcspn(buf + offset,
                                       (ch == '"') ? "\"<" : "'<");
                     if (buf[offset] == ch || !buf[offset]) {
                        buf_index = offset;
                     } else {
                        MSG_HTML("attribute lacks closing quote\n");
                        break;
                     }
                  }
               } else if ( ch == '<' ) {
                  /* unterminated tag detected */
                  p = g_strndup(buf+token_start+1,
                                strcspn(buf+token_start+1, " <"));
                  MSG_HTML("<%s> element lacks its closing '>'\n", p);
                  g_free(p);
                  --buf_index;
                  break;
               }
            }
            if (buf_index < bufsize) {
               buf_index++;
               Html_process_tag(html, buf + token_start,
                                buf_index - token_start);
               token_start = buf_index;
            }
         }
      } else {
         /* A Word: search for whitespace or tag open */
         while (++buf_index < bufsize) {
            buf_index += strcspn(buf + buf_index, " <\n\r\t\f\v");
            if ( buf[buf_index] == '<' && (ch = buf[buf_index + 1]) &&
                 !isalpha(ch) && !strchr("/!?", ch))
               continue;
            break;
         }
         if (buf_index < bufsize || Eof) {
            /* successfully found end of token */
            Html_process_word(html, buf + token_start,
                              buf_index - token_start);
            token_start = buf_index;
         }
      }
   }/*while*/

   a_Dw_page_flush(page);
   g_free(buf);

   return token_start;
}

/*
 * Process the newly arrived html and put it into the page structure.
 * (This function is called by Html_callback whenever there's new data)
 */
static void Html_write(DilloHtml *html, char *Buf, gint BufSize, gint Eof)
{
   DwPage *page;
   char completestr[32];
   gint token_start;
   char *buf = Buf + html->Start_Ofs;
   gint bufsize = BufSize - html->Start_Ofs;

   g_return_if_fail ( (page = DW_PAGE (html->dw)) != NULL );

   html->Start_Buf = Buf;
   token_start = Html_write_raw(html, buf, bufsize, Eof);
   html->Start_Ofs += token_start;

   if ( html->bw ) {
      g_snprintf(
         completestr, 32, "%s%.1f Kb",
         PBAR_PSTR(prefs.panel_size == 1),
         (float)html->Start_Ofs/1024);
      a_Progressbar_update(html->bw->progress, completestr, 1);
   }
}

/*
 * Finish parsing a HTML page
 * (Free html struct, close the client and update progressbar).
 */
static void Html_close(DilloHtml *html, gint ClientKey)
{
   gint si;

   //#if defined (DEBUG_LEVEL) && DEBUG_LEVEL >= 1
   //a_Dw_widget_print_tree (GTK_DW_VIEWPORT(html->dw->viewport)->child);
   //#endif

   /* force the close of elements left open (todo: not for XHTML) */
   while ((si = html->stack_top)) {
      if (html->stack[si].tag_idx != -1) {
         Html_tag_cleanup_at_close(html, html->stack[si].tag_idx);
      }
   }
   g_free(html->stack[0].tag_name); /* "none" */
   a_Dw_style_unref(html->stack[0].style); /* template style */

   g_free(html->stack);

   g_string_free(html->Stash, TRUE);
   g_free(html->SPCBuf);
   g_string_free(html->attr_data, TRUE);

   /* Remove this client from our active list */
   a_Interface_close_client(html->bw, ClientKey);

   /* Set progress bar insensitive */
   a_Progressbar_update(html->bw->progress, NULL, 0);

   g_free(html);
}


