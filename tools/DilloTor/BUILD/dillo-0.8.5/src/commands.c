/*
 * File: commands.c
 *
 * Copyright (C) 1997 Raph Levien <raph@acm.org>
 * Copyright (C) 1999 Sammy Mannaert <nstalkie@tvd.be>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <gtk/gtk.h>
#include <stdio.h>              /* for sprintf */
#include <sys/time.h>           /* for gettimeofday (testing gorp only) */
#include <unistd.h>
#include <string.h>             /* for strcat() */

#include "msg.h"
#include "bookmark.h"
#include "interface.h"
#include "history.h"
#include "nav.h"
#include "misc.h"
#include "commands.h"
#include "prefs.h"
#include "menu.h"
#include "capi.h"
#include "selection.h"

/* FILE MENU */

/*
 * Create a new browser window
 */
void a_Commands_new_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *nbw, *bw = (BrowserWindow *)client_data;

   nbw = a_Interface_browser_window_new(bw->main_window->allocation.width,
                                  bw->main_window->allocation.height, 0);
   /* focus the location bar */
   gtk_widget_grab_focus(nbw->location);
}

/*
 * Create and show the "Open file" dialog
 */
void a_Commands_openfile_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;

   a_Interface_openfile_dialog(bw);
}

/*
 * Create and show the "Open Url" dialog window
 */
void a_Commands_openurl_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;
   a_Interface_open_dialog(widget, bw);
}

/*
 * ?
 */
void a_Commands_prefs_callback(GtkWidget *widget, gpointer client_data)
{
}

/*
 * Close browser window, and exit dillo if it's the last one.
 */
void a_Commands_close_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *)client_data;
   gtk_widget_destroy(bw->main_window);
}

/*
 * Free memory and quit dillo
 */
void a_Commands_exit_callback(GtkWidget *widget, gpointer client_data)
{
   a_Interface_quit_all();
}


/* PAGE MENU */

/*
 * Show current page's source code.
 */
void a_Commands_viewsource_callback (GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *)client_data;
   gchar *buf;
   gint buf_size;

   /* Get page source data */
   a_Capi_get_buf(a_History_get_url(NAV_TOP(bw)), &buf, &buf_size);
   /* Show it */
   a_Interface_text_window (&bw->viewsource_window,
                            "View Source", "view_source",
                            buf, buf_size,
                            530, 500);
}

/*
 * Show the detected HTML errors in current page.
 */
void a_Commands_view_page_bugs_callback (GtkWidget *widget,
                                         gpointer client_data)
{
   DilloHtmlLB *html_lb = client_data;

   a_Interface_text_window (&html_lb->bw->pagebugs_window,
                            "Detected HTML errors", "page_bugs",
                            html_lb->page_bugs->str,
                            html_lb->page_bugs->len,
                            530, 500);
}

/*
 * ?
 */
void a_Commands_selectall_callback(GtkWidget *widget, gpointer client_data)
{
}

/*
 * Create and show the "Find Text" dialog window
 */
void a_Commands_findtext_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;

   a_Interface_findtext_dialog( bw );
}

/*
 * Print the page!
 * ('cat page.html | html2ps | lpr -Pcool'   Why bother?  I think providing
 * such an option in a configurable way should cut it  --Jcid)
 */
void a_Commands_print_callback(GtkWidget *widget, gpointer client_data)
{
}


/* TOOLBAR MENU */

/*
 * Abort all active connections for this page
 * (Downloads MUST keep flowing)
 */
void a_Commands_stop_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = client_data;
   a_Nav_cancel_expect(bw);
   a_Interface_stop(bw);
   a_Interface_set_button_sens(bw);
   a_Interface_msg(bw, "Stopped");
}

/*
 *  Back to previous page
 */
void a_Commands_back_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;

   a_Nav_back(bw);
}

/*
 * Creates the menu for the forward navigation button.
 */
GtkMenu *a_Commands_forw_button_menu_creator_callback(GtkExtButton *button,
                                                      gpointer client_data)
{
   BrowserWindow *bw = client_data;

   if (bw->menu_popup.over_forw)
      gtk_widget_destroy(bw->menu_popup.over_forw);
   bw->menu_popup.over_forw = a_Menu_popup_history_new(bw, +1);
   return GTK_MENU(bw->menu_popup.over_forw);
}

/*
 * Creates the menu for the backward navigation button.
 */
GtkMenu *a_Commands_back_button_menu_creator_callback(GtkExtButton *button,
                                                      gpointer client_data)
{
   BrowserWindow *bw = client_data;

   if (bw->menu_popup.over_back)
      gtk_widget_destroy(bw->menu_popup.over_back);
   bw->menu_popup.over_back = a_Menu_popup_history_new(bw, -1);
   return GTK_MENU(bw->menu_popup.over_back);
}

/*
 * Handler for menu entries in the history menus. This one is called, when
 * the link should be opened in the same window.
 */
void a_Commands_history_callback_same_bw(GtkWidget *widget,
                                         gpointer client_data)
{
   BrowserWindow *bw = client_data;

   a_Nav_jump_callback(widget, bw, 0);
}

/*
 * Handler for menu entries in the history menus. This one is called, when
 * the link should be opened in a new window.
 */
void a_Commands_history_callback_new_bw(GtkWidget *widget,
                                        gpointer client_data)
{
   BrowserWindow *bw = client_data;

   a_Nav_jump_callback(widget, bw, 1);
}

/*
 * Go to the next page in the history buffer
 */
void a_Commands_forw_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;

   a_Nav_forw(bw);
}

/*
 * Start the reload process
 */
void a_Commands_reload_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;

   a_Nav_reload(bw);
}

/*
 * Go home!
 */
void a_Commands_home_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;

   a_Nav_home(bw);
}

/*
 * Bring up the save page dialog
 */
void a_Commands_save_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;

   a_Interface_save_dialog(widget, bw);
}

/*
 * Bring up the save link dialog
 */
void a_Commands_save_link_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;

   a_Interface_save_link_dialog(widget, bw);
}


/* BOOKMARKS MENU */

/*
 * Add a bookmark to the current bookmark widget.
 */
void a_Commands_addbm_callback(GtkWidget *widget, gpointer client_data)
{
   a_Bookmarks_add(widget, client_data);
}

/*
 * Show the bookmarks-file as rendered html
 */
void a_Commands_viewbm_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;

   a_Bookmarks_show(bw);
}


/* HELP MENU */

/*
 * This one was intended as a link to help-info on the web site, but
 * currently points to the home page  --Jcid
 */
void a_Commands_helphome_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *) client_data;
   DilloUrl *url = a_Url_new(DILLO_HOME, NULL, 0, 0, 0);

   a_Nav_push(bw, url);
   a_Url_free(url);
}


/* RIGHT BUTTON POP-UP MENU */

/*
 * Open link in browser-window
 */
void a_Commands_open_link_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *)client_data;

   a_Nav_push(bw, a_Menu_popup_get_url(bw));
}

/*
 * Open link in another browser-window
 */
void a_Commands_open_link_nw_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *)client_data;
   gint width, height;
   BrowserWindow *newbw;

   gdk_window_get_size (bw->main_window->window, &width, &height);
   newbw = a_Interface_browser_window_new(width, height, 0);
   a_Nav_push(newbw, a_Menu_popup_get_url(bw));
}

/*
 * Called when the user wants the popup's URL for pasting.
 */
void a_Commands_select_popup_url_callback(GtkWidget *widget, gpointer data)
{
   BrowserWindow *bw = (BrowserWindow *)data;
   a_Selection_set_selection(widget, URL_STR(a_Menu_popup_get_url(bw)));
}


/* BUG METER POP-UP MENU */

/*
 * Helper function for validation.
 */
static void Commands_ob_validate_page(BrowserWindow *bw, const gchar *fmt)
{
   GString *gstr = g_string_sized_new(128);
   gchar *o_url, *e_url, *n_url;
   DilloUrl *url;

   /* prepare the validation request URI */
   o_url = URL_STR(a_History_get_url(NAV_TOP(bw)));
   e_url = a_Misc_escape_chars(o_url, ":/?");
   n_url = e_url ? e_url : o_url;
   g_string_sprintf(gstr, fmt, n_url);
   if (n_url != o_url)
      g_free(n_url);
   url = a_Url_new(gstr->str, NULL, 0, 0, 0);
   g_string_free(gstr, TRUE);

   a_Nav_push(bw, url);
   a_Url_free(url);
}

/*
 * Validate current page with the W3C's validator.
 */
void a_Commands_ob_w3c_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *)client_data;
   const gchar *w3c_fmt="http://validator.w3.org/check?uri=%s";

   Commands_ob_validate_page(bw, w3c_fmt);
}

/*
 * Validate current page with the WDG's validator.
 */
void a_Commands_ob_wdg_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *)client_data;
   const gchar *wdg_fmt=
      "http://www.htmlhelp.org/cgi-bin/validate.cgi?url=%s&warnings=yes";

   Commands_ob_validate_page(bw, wdg_fmt);
}

/*
 * Request the info page for bug meter.
 */
void a_Commands_ob_info_callback(GtkWidget *widget, gpointer client_data)
{
   BrowserWindow *bw = (BrowserWindow *)client_data;
   const gchar *info_str = "http://www.dillo.org/help/bug_meter.html";
   DilloUrl *url = a_Url_new(info_str, NULL, 0, 0, 0);

   a_Nav_push(bw, url);
   a_Url_free(url);
}

