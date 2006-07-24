/*
 * File: interface.c
 *
 * Copyright (C) 1997 Raph Levien <raph@acm.org>
 * Copyright (C) 1999 Sammy Mannaert <nstalkie@tvd.be>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <config.h>

#include <stdio.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#include "msg.h"
#include "list.h"
#include "misc.h"
#include "dillo.h"
#include "history.h"
#include "nav.h"
#include "IO/Url.h"
#include "IO/IO.h"
#include "interface.h"
#include "commands.h"
#include "menu.h"
#include "bookmark.h"
#include "prefs.h"
#include "url.h"
#include "capi.h"
#include "gtk_ext_button.h"

#include "dw_widget.h"
#include "dw_gtk_scrolled_window.h"
#include "dw_gtk_viewport.h"
#include "dw_gtk_statuslabel.h"
#include "dw_container.h"
#include "progressbar.h"

#include "pixmaps.h"
#include <gdk/gdkkeysyms.h>

#define DEBUG_LEVEL 0
#include "debug.h"


/*
 * Local Data
 */
/* BrowserWindow holds all the widgets (and perhaps more)
 * for each new_browser.*/
static BrowserWindow **browser_window;
static gint num_bw, num_bw_max;

/* We need only one of them. */
static GtkTooltips *tooltips = NULL;

/* open dialog last dir */
static gchar *open_dialog_last_dirname = NULL;

/* save dialog last dir */
static gchar *save_dialog_last_dirname = NULL;


/*
 * Initialize global data
 */
void a_Interface_init(void)
{
   num_bw = 0;
   num_bw_max = 16;
   browser_window = NULL;
   tooltips = gtk_tooltips_new ();
   open_dialog_last_dirname = NULL;
   save_dialog_last_dirname = NULL;
}

/*
 * Stop all active connections in the browser window (except downloads)
 */
void a_Interface_stop(BrowserWindow *bw)
{
   DEBUG_MSG(3, "a_Interface_stop: hi!\n");

   /* Remove root clients */
   while ( bw->NumRootClients ) {
      a_Cache_stop_client(bw->RootClients[0]);
      a_List_remove(bw->RootClients, 0, bw->NumRootClients);
   }
   /* Remove image clients */
   while ( bw->NumImageClients ) {
      a_Cache_stop_client(bw->ImageClients[0]);
      a_List_remove(bw->ImageClients, 0, bw->NumImageClients);
   }
}

/*
 * Empty RootClients, ImageClients and PageUrls lists and
 * reset progress bar data.
 */
void a_Interface_clean(BrowserWindow *bw)
{
   g_return_if_fail ( bw != NULL );

   while ( bw->NumRootClients )
      a_List_remove(bw->RootClients, 0, bw->NumRootClients);

   while ( bw->NumImageClients )
      a_List_remove(bw->ImageClients, 0, bw->NumImageClients);

   while ( bw->NumPageUrls ) {
      a_Url_free(bw->PageUrls[0].Url);
      a_List_remove(bw->PageUrls, 0, bw->NumPageUrls);
   }

   /* Zero image-progressbar data */
   bw->NumImages = 0;
   bw->NumImagesGot = 0;
}

/*=== Browser Window Interface Updating =====================================*/
/*
 * Remove the cache-client from the bw list
 * (client can be a image or a html page)
 */
void a_Interface_remove_client(BrowserWindow *bw, gint ClientKey)
{
   gint i;
   gboolean Found = FALSE;

   for ( i = 0; !Found && i < bw->NumRootClients; ++i)
      if ( bw->RootClients[i] == ClientKey ) {
         a_List_remove(bw->RootClients, i, bw->NumRootClients);
         Found = TRUE;
      }

   for ( i = 0; !Found && i < bw->NumImageClients; ++i)
      if ( bw->ImageClients[i] == ClientKey ) {
         a_List_remove(bw->ImageClients, i, bw->NumImageClients);
         bw->NumImagesGot++;
         Found = TRUE;
      }

   a_Interface_set_button_sens(bw);
}

/*
 * Remove the cache-client from the bw list
 * (client can be a image or a html page)
 */
void a_Interface_close_client(BrowserWindow *bw, gint ClientKey)
{
   gchar numstr[32];

   a_Interface_remove_client(bw, ClientKey);

   /* --Progress bars stuff-- */
   g_snprintf(numstr, 32, "%s%d of %d", PBAR_ISTR(prefs.panel_size == 1),
              bw->NumImagesGot, bw->NumImages);
   a_Progressbar_update(bw->imgprogress, numstr,
                        (bw->NumImagesGot == bw->NumImages) ? 0 : 1 );
}

/*
 * Set the sensitivity on back/forw buttons and menu entries.
 */
static gint Interface_sens_idle_func(BrowserWindow *bw)
{
   gboolean back_sensitive, forw_sensitive, stop_sensitive;

   /* Stop button */
   stop_sensitive = (bw->NumRootClients > 0);
   gtk_widget_set_sensitive(bw->stop_button, stop_sensitive);

   /* Back and Forward buttons */
   back_sensitive = a_Nav_stack_ptr(bw) > 0;
   gtk_widget_set_sensitive(bw->back_button, back_sensitive);
   forw_sensitive = (a_Nav_stack_ptr(bw) < a_Nav_stack_size(bw) - 1 &&
                     !bw->nav_expecting);
   gtk_widget_set_sensitive(bw->forw_button, forw_sensitive);

   bw->sens_idle_id = 0;
   return FALSE;
}

/*
 * Set the sensitivity on back/forw buttons and menu entries.
 */
void a_Interface_set_button_sens(BrowserWindow *bw)
{
   if (bw->sens_idle_id == 0)
      bw->sens_idle_id = gtk_idle_add(
                             (GtkFunction)Interface_sens_idle_func, bw);
}

/*
 * Add a reference to the cache-client in the browser window's list.
 * This helps us keep track of which are active in the window so that it's
 * possible to abort them.
 * (Root: Flag, whether a Root URL or not)
 */
void a_Interface_add_client(BrowserWindow *bw, gint Key, gint Root)
{
   gint nc;
   char numstr[32];

   g_return_if_fail ( bw != NULL );

   if ( Root ) {
      nc = bw->NumRootClients;
      a_List_add(bw->RootClients, nc, bw->MaxRootClients);
      bw->RootClients[nc] = Key;
      bw->NumRootClients++;
      a_Interface_set_button_sens(bw);
   } else {
      nc = bw->NumImageClients;
      a_List_add(bw->ImageClients, nc, bw->MaxImageClients);
      bw->ImageClients[nc] = Key;
      bw->NumImageClients++;
      bw->NumImages++;
      a_Interface_set_button_sens(bw);

      /* --Progress bar stuff-- */
      g_snprintf(numstr, 32, "%s%d of %d", PBAR_ISTR(prefs.panel_size == 1),
                 bw->NumImagesGot, bw->NumImages);
      a_Progressbar_update(bw->imgprogress, numstr, 1);
   }
}

/*
 * Add an URL to the browser window's list.
 * This helps us keep track of page requested URLs so that it's
 * possible to stop, abort and reload them.)
 *   Flags: Chosen from {BW_Root, BW_Image, BW_Download}
 */
void a_Interface_add_url(BrowserWindow *bw, const DilloUrl *Url, gint Flags)
{
   gint nu, i;
   gboolean found = FALSE;

   g_return_if_fail ( bw != NULL && Url != NULL );

   nu = bw->NumPageUrls;
   for ( i = 0; i < nu; i++ ) {
      if ( !a_Url_cmp(Url, bw->PageUrls[i].Url) ) {
         found = TRUE;
         break;
      }
   }
   if ( !found ) {
      a_List_add(bw->PageUrls, nu, bw->MaxPageUrls);
      bw->PageUrls[nu].Url = a_Url_dup(Url);
      bw->PageUrls[nu].Flags = Flags;
      bw->NumPageUrls++;
   }

   /* test:
   MSG("Urls:\n");
   for (i = 0; i < bw->NumPageUrls; i++)
      MSG("%s\n", bw->PageUrls[i].Url);
   MSG("---\n");
   */
}

/*
 * Remove a single browser window. This includes all its open childs,
 * freeing all resources associated with them, and exiting gtk
 * if no browser windows are left.
 */
static gboolean Interface_quit(GtkWidget *widget, BrowserWindow *bw)
{
   gint i;

   /* stop/abort open connections. */
   a_Interface_stop(bw);

   g_slist_free(bw->PanelHandles);

   if (bw->open_dialog_window != NULL)
      gtk_widget_destroy(bw->open_dialog_window);
   if (bw->openfile_dialog_window != NULL)
      gtk_widget_destroy(bw->openfile_dialog_window);
   if (bw->quit_dialog_window != NULL)
      gtk_widget_destroy(bw->quit_dialog_window);
   if (bw->findtext_dialog_window != NULL)
      gtk_widget_destroy(bw->findtext_dialog_window);
   if (bw->search_dialog_window != NULL)
      gtk_widget_destroy(bw->search_dialog_window);
   if (bw->proxy_passwd_dialog_window != NULL)
      gtk_widget_destroy(bw->proxy_passwd_dialog_window);
   if (bw->question_dialog_window != NULL)
      gtk_widget_destroy(bw->question_dialog_window);

   if (bw->menu_popup.over_page)
      gtk_widget_destroy(bw->menu_popup.over_page);
   if (bw->menu_popup.over_link)
      /* this also destroys menu_popup.over_image */
      gtk_widget_destroy(bw->menu_popup.over_link);
   if (bw->menu_popup.over_back)
      gtk_widget_destroy(bw->menu_popup.over_back);
   if (bw->menu_popup.over_forw)
      gtk_widget_destroy(bw->menu_popup.over_forw);
   if (bw->menu_popup.over_bug)
      gtk_widget_destroy(bw->menu_popup.over_bug);
   if (bw->menu_popup.url)
      a_Url_free(bw->menu_popup.url);
   if (bw->menu_popup.url2)
      a_Url_free(bw->menu_popup.url2);

   if (bw->sens_idle_id)
      gtk_idle_remove(bw->sens_idle_id);

   for (i = 0; i < num_bw; i++)
      if (browser_window[i] == bw) {
         browser_window[i] = browser_window[--num_bw];
         break;
      }

   /* free nav_stack and nav_expect stuff */
   a_Nav_free(bw);

   g_free(bw->RootClients);
   g_free(bw->ImageClients);

   for (i = 0; i < bw->NumPageUrls; i++)
      a_Url_free(bw->PageUrls[i].Url);
   g_free(bw->PageUrls);
   g_free(bw);

   if (num_bw == 0)
      gtk_main_quit();

   return FALSE;
}


/*=== Browser Window Interface Construction =================================*/

/*
 * Clear a text entry
 */
static void Interface_entry_clear(GtkEntry *entry)
{
   gtk_entry_set_text(entry, "");
   gtk_widget_grab_focus(GTK_WIDGET(entry));
}

/*
 * Get the selection into the clear url button.
 * (cub = clear url button)
 */
static void Interface_cub_get_selection(GtkWidget *widget, gpointer data)
{
  /* Request the the primary selection as a string */
  gtk_selection_convert (widget, GDK_SELECTION_PRIMARY,
                         GDK_SELECTION_TYPE_STRING,
                         GDK_CURRENT_TIME);
}

/*
 * Receive the selection (from a paste event)
 * (cub = clear url button)
 */
static void Interface_cub_selection_received(GtkWidget *widget,
                                             GtkSelectionData *selection_data,
                                             guint32 time,
                                             gpointer data)
{
   BrowserWindow *bw = data;
   gchar *damn_string;

   _MSG("Interface_cub_selection_received:\n");

   if (selection_data->length < 0) {
       DEBUG_MSG (1, "Selection retrieval failed\n");
       return;
   }
   
   damn_string = g_strndup((gchar *)selection_data->data,
                           selection_data->length);
   gtk_entry_set_text(GTK_ENTRY(bw->location), damn_string);
   gtk_widget_activate(GTK_WIDGET(bw->location));
   g_free(damn_string);

   return;
}

/*
 * Create a pixmap and return it.
 */
static GtkWidget *Interface_pixmap_new(GtkWidget *parent, gchar **data)
{
   GtkWidget *pixmapwid;
   GdkPixmap *pixmap;
   GdkBitmap *mask;
   GtkStyle *style;

   style = gtk_widget_get_style(parent);

   pixmap = gdk_pixmap_create_from_xpm_d(parent->window, &mask,
                                         &style->bg[GTK_STATE_NORMAL], data);

   pixmapwid = gtk_pixmap_new(pixmap, mask);

   return (pixmapwid);
}

/*
 * Create an extended button for the toolbar. "label_text" may be NULL.
 * "icon_ret" and "label_ret " may be used to return the child widgets, but
 * may also be 0.
 */
static GtkWidget *Interface_toolbox_ext_button_new(GtkWidget *parent,
                                                   gchar *label_text,
                                                   gchar **image_data,
                                                   GtkWidget **icon_ret,
                                                   GtkWidget **label_ret)
{
   GtkWidget *button, *pixmap, *vbox, *label;

   button = a_Gtk_ext_button_new ();
   GTK_WIDGET_UNSET_FLAGS (button, GTK_CAN_FOCUS);
   gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
   pixmap = Interface_pixmap_new (parent, image_data);

   if (label_text) {
      vbox = gtk_vbox_new(FALSE, 0);
      gtk_container_add(GTK_CONTAINER (button), vbox);
      gtk_box_pack_start(GTK_BOX(vbox), pixmap, FALSE, FALSE, 0);
      label = gtk_label_new(label_text);
      gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
   } else {
      gtk_container_add(GTK_CONTAINER (button), pixmap);
      label = NULL;
   }

   gtk_widget_show_all (button);

   if (icon_ret)
      *icon_ret = pixmap;
   if (label_ret)
      *label_ret = label;

   return button;
}

/*
 * Set the bw's cursor type
 */
void a_Interface_set_cursor(BrowserWindow *bw, GdkCursorType CursorType)
{
   GdkCursor *cursor;

   if ( bw->CursorType != CursorType ) {
      cursor = gdk_cursor_new(CursorType);
      gdk_window_set_cursor(bw->docwin->window, cursor);
      gdk_cursor_destroy(cursor);
      bw->CursorType = CursorType;
   }
}

/*
 * Connect button's "clicked" event with (key, key_mod) pair.
 */
static void Interface_set_button_accel(GtkButton *button,
                                       gint key,
                                       gint key_mod,
                                       GtkAccelGroup *accel_group)
{
   gtk_widget_add_accelerator(GTK_WIDGET(button), "clicked", accel_group,
                              key, key_mod, GTK_ACCEL_LOCKED);
}

/*
 * Create the "NEW" button, its location-entry and the search button.
 */
static GtkWidget *Interface_locbar_new(BrowserWindow *bw)
{
   GtkWidget *hbox, *toolbar_l, *toolbar_r;

   hbox = gtk_hbox_new(FALSE, 0);

   /* location entry */
   bw->location = gtk_entry_new();
   gtk_signal_connect(GTK_OBJECT(bw->location), "activate",
                      (GtkSignalFunc) a_Interface_entry_open_url, bw);
   gtk_widget_add_accelerator(GTK_WIDGET(bw->location), "grab_focus",
                              bw->accel_group, GDK_u,
                              GDK_CONTROL_MASK, GTK_ACCEL_LOCKED);

   /* left toolbar (Clear url) */
   toolbar_l = gtk_toolbar_new(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH);
   gtk_toolbar_set_button_relief(GTK_TOOLBAR(toolbar_l), GTK_RELIEF_NONE);
   GTK_WIDGET_UNSET_FLAGS (toolbar_l, GTK_CAN_FOCUS);

   bw->clear_url_button =
      Interface_toolbox_ext_button_new(bw->main_window, NULL, s_new_xpm,
                                       NULL, NULL);
   a_Gtk_ext_button_set_command(GTK_EXT_BUTTON(bw->clear_url_button), 1);
   a_Gtk_ext_button_set_command(GTK_EXT_BUTTON(bw->clear_url_button), 2);

   gtk_toolbar_append_widget(GTK_TOOLBAR(toolbar_l), bw->clear_url_button,
                             "Clear the URL box (middle-click to paste an "
                             "URL).", "Toolbar/Clear");
   gtk_signal_connect_object(GTK_OBJECT(bw->clear_url_button), "clicked",
                             GTK_SIGNAL_FUNC (Interface_entry_clear),
                             GTK_OBJECT(bw->location));
   gtk_signal_connect_object(GTK_OBJECT(bw->clear_url_button), "clicked1",
                             GTK_SIGNAL_FUNC (Interface_entry_clear),
                             GTK_OBJECT(bw->location));
   gtk_signal_connect(GTK_OBJECT(bw->clear_url_button), "clicked2",
                      GTK_SIGNAL_FUNC (Interface_cub_get_selection),
                      NULL);
   gtk_signal_connect(GTK_OBJECT(bw->clear_url_button), "selection_received",
                      GTK_SIGNAL_FUNC (Interface_cub_selection_received),
                      bw);

   /* right toolbar (Search) */
   toolbar_r = gtk_toolbar_new(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH);
   gtk_toolbar_set_button_relief(GTK_TOOLBAR(toolbar_r), GTK_RELIEF_NONE);
   GTK_WIDGET_UNSET_FLAGS (toolbar_r, GTK_CAN_FOCUS);

   bw->search_button = gtk_toolbar_append_item(
                          GTK_TOOLBAR(toolbar_r),
                          NULL, "Search the Web", "Toolbar/New",
                          Interface_pixmap_new(bw->main_window, search_xpm),
                          GTK_SIGNAL_FUNC(a_Interface_search_dialog),
                          bw);
   gtk_widget_add_accelerator(GTK_WIDGET(bw->search_button), "clicked",
                              bw->accel_group, GDK_s,
                              GDK_CONTROL_MASK, GTK_ACCEL_LOCKED);
   gtk_box_pack_start(GTK_BOX(hbox), toolbar_l, FALSE, FALSE, 0);
   gtk_widget_show(toolbar_l);
   gtk_box_pack_start(GTK_BOX(hbox), bw->location, TRUE, TRUE, 0);
   gtk_widget_show(bw->location);
   gtk_box_pack_start(GTK_BOX(hbox), toolbar_r, FALSE, FALSE, 0);
   gtk_widget_show(toolbar_r);
   gtk_widget_show(hbox);

   return (hbox);
}

/*
 * Create a new toolbar (Back, Forward, Home, Reload, Save and Stop buttons)
 */
static GtkWidget *Interface_toolbar_new(BrowserWindow *bw, gint label)
{
   GtkWidget *toolbar, *label_widget, *icon_widget;
   GtkToolbarChild *toolbar_child;
   gboolean s = prefs.small_icons;

   toolbar = gtk_toolbar_new(GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_BOTH);
   gtk_toolbar_set_button_relief(GTK_TOOLBAR(toolbar), GTK_RELIEF_NONE);

   /* back button */
   bw->back_button =
      Interface_toolbox_ext_button_new(bw->main_window, label ? "Back" : NULL,
                                       s ? s_left_xpm : left_xpm,
                                       &label_widget, &icon_widget);
   gtk_toolbar_append_widget(GTK_TOOLBAR(toolbar), bw->back_button,
                             "Go to previous page (right-click for menu).",
                             "Toolbar/Back");
   gtk_widget_set_sensitive(bw->back_button, FALSE);
   Interface_set_button_accel(GTK_BUTTON(bw->back_button), GDK_comma,
                              0, bw->accel_group);
   a_Gtk_ext_button_set_command(GTK_EXT_BUTTON(bw->back_button), 1);
   a_Gtk_ext_button_attach_menu_creator(
      GTK_EXT_BUTTON(bw->back_button), 3,
      a_Commands_back_button_menu_creator_callback, (gpointer)bw);
   gtk_signal_connect(GTK_OBJECT(bw->back_button), "clicked",
                      GTK_SIGNAL_FUNC(a_Commands_back_callback), (gpointer)bw);
   gtk_signal_connect(GTK_OBJECT(bw->back_button), "clicked1",
                      GTK_SIGNAL_FUNC(a_Commands_back_callback), (gpointer)bw);

   /*
    * HACK: We have added GtkExtButton's as widgets
    * (gtk_toolbar_append_widget), but they should behave like buttons etc.
    * (gtk_toolbar_append_item), especially, they should have the same size.
    * So we change some parts of the internal representation.
    */
   toolbar_child = ((GtkToolbarChild*)GTK_TOOLBAR(toolbar)->children->data);
   toolbar_child->type = GTK_TOOLBAR_CHILD_BUTTON;
   toolbar_child->widget = bw->back_button;
   toolbar_child->icon = icon_widget;
   toolbar_child->label = label_widget;

   /* forward button */
   bw->forw_button =
      Interface_toolbox_ext_button_new(bw->main_window,
                                       label ? "Forward" : NULL,
                                       s ? s_right_xpm : right_xpm,
                                       &label_widget, &icon_widget);
   gtk_toolbar_append_widget(GTK_TOOLBAR(toolbar), bw->forw_button,
                             "Go to next page (right-click for menu).",
                             "Toolbar/Forward");
   gtk_widget_set_sensitive(bw->forw_button, FALSE);
   Interface_set_button_accel(GTK_BUTTON(bw->forw_button), GDK_period,
                              0, bw->accel_group);
   a_Gtk_ext_button_set_command(GTK_EXT_BUTTON(bw->forw_button), 1);
   a_Gtk_ext_button_attach_menu_creator(
      GTK_EXT_BUTTON(bw->forw_button), 3,
      a_Commands_forw_button_menu_creator_callback, (gpointer)bw);
   gtk_signal_connect(GTK_OBJECT(bw->forw_button), "clicked",
                      GTK_SIGNAL_FUNC(a_Commands_forw_callback), (gpointer)bw);
   gtk_signal_connect(GTK_OBJECT(bw->forw_button), "clicked1",
                      GTK_SIGNAL_FUNC(a_Commands_forw_callback), (gpointer)bw);

   /*
    * HACK: See above.
    */
   toolbar_child =
      ((GtkToolbarChild*)GTK_TOOLBAR(toolbar)->children->next->data);
   toolbar_child->type = GTK_TOOLBAR_CHILD_BUTTON;
   toolbar_child->widget = bw->forw_button;
   toolbar_child->icon = icon_widget;
   toolbar_child->label = label_widget;

   /* home button */
   bw->home_button = gtk_toolbar_append_item(
                        GTK_TOOLBAR(toolbar),
                        label ? "Home" : NULL,
                        "Go to the Home page", "Toolbar/Home",
                        Interface_pixmap_new(bw->main_window,
                                             s ? s_home_xpm : home_xpm),
                        (GtkSignalFunc) a_Commands_home_callback, bw);
   /*
    * SG: This had never any effect, was it there for future extensions?
    * gtk_signal_connect(GTK_OBJECT(bw->home_button), "button-press-event",
    *                    GTK_SIGNAL_FUNC(a_Commands_navpress_callback), bw);
    */

   /* reload button */
   bw->reload_button = gtk_toolbar_append_item(
                          GTK_TOOLBAR(toolbar),
                          label ? "Reload" : NULL,
                          "Reload this page", "Toolbar/Reload",
                          Interface_pixmap_new(bw->main_window,
                                               s ? s_reload_xpm : reload_xpm),
                          (GtkSignalFunc) a_Commands_reload_callback, bw);
   Interface_set_button_accel(GTK_BUTTON(bw->reload_button), GDK_r,
                              GDK_CONTROL_MASK, bw->accel_group);

   /* save button */
   bw->save_button = gtk_toolbar_append_item(
                        GTK_TOOLBAR(toolbar),
                        label ? "Save" : NULL,
                        "Save this page", "Toolbar/Save",
                        Interface_pixmap_new(bw->main_window,
                                             s ? s_save_xpm : save_xpm),
                        (GtkSignalFunc) a_Commands_save_callback, bw);
   /* stop button */
   bw->stop_button = gtk_toolbar_append_item(
                        GTK_TOOLBAR(toolbar),
                        label ? "Stop" : NULL,
                        "Stop the current transfer", "Toolbar/Stop",
                        Interface_pixmap_new(bw->main_window,
                                             s ? s_stop_xpm : stop_xpm),
                        (GtkSignalFunc) a_Commands_stop_callback, bw);
   gtk_widget_set_sensitive(bw->stop_button, FALSE);

   /* bookmarks button */
   bw->bookmarks_button = gtk_toolbar_append_item(
                             GTK_TOOLBAR(toolbar),
                             label ? "Book" : NULL,
                             "View bookmarks", "Toolbar/Bookmarks",
                             Interface_pixmap_new(bw->main_window,
                                                  s ? s_bm_xpm : bm_xpm),
                             (GtkSignalFunc) a_Commands_viewbm_callback, bw);
   gtk_widget_set_sensitive(bw->bookmarks_button, TRUE);
   Interface_set_button_accel(GTK_BUTTON(bw->bookmarks_button), GDK_semicolon,
                              0, bw->accel_group);
   Interface_set_button_accel(GTK_BUTTON(bw->bookmarks_button), GDK_b,
                              GDK_CONTROL_MASK, bw->accel_group);
   /*
    * SG: This had never any effect, was it there for future extensions?
    * gtk_signal_connect(GTK_OBJECT(bw->bookmarks_button),
    *                    "button-press-event",
    *                    GTK_SIGNAL_FUNC(a_Commands_navpress_callback), bw);
    */

   gtk_widget_show(toolbar);
   return toolbar;
}

/*
 * Create the progressbar's box
 */
static GtkWidget *Interface_progressbox_new(BrowserWindow *bw, gint vertical)
{
   GtkWidget *progbox;

   progbox = vertical ? gtk_vbox_new(FALSE, 0) : gtk_hbox_new(FALSE, 0);
   bw->progress_box = progbox;
   bw->imgprogress = a_Progressbar_new();
   bw->progress = a_Progressbar_new();
   gtk_box_pack_start(GTK_BOX(progbox), bw->imgprogress, TRUE, TRUE, 0);
   gtk_widget_show(bw->imgprogress);
   gtk_box_pack_start(GTK_BOX(progbox), bw->progress, TRUE, TRUE, 0);
   gtk_widget_show(bw->progress);
   return (progbox);
}

/*
 * Hide/Unhide this bw's control panels.
 * toggle: Flag [toggle or set].
 */
static void Interface_toggle_panel(BrowserWindow *bw, gint toggle)
{
   if (toggle)
      bw->fullwindow = !bw->fullwindow;

   if (bw->fullwindow) {
      g_slist_foreach(bw->PanelHandles, (GFunc)gtk_widget_hide, NULL);
      gtk_widget_hide(bw->status_box);
      gtk_widget_show (bw->full_screen_off_button);
      gtk_widget_grab_focus(GTK_BIN(bw->docwin)->child);
   } else {
      g_slist_foreach(bw->PanelHandles, (GFunc)gtk_widget_show, NULL);
      gtk_widget_show(bw->status_box);
      gtk_widget_hide (bw->full_screen_off_button);
   }
}

/*
 * Customize the appearance of the bw.
 */
static void Interface_browser_window_customize(BrowserWindow *bw)
{
   if ( !prefs.show_back )
      gtk_widget_hide(bw->back_button);
   if ( !prefs.show_forw )
      gtk_widget_hide(bw->forw_button);
   if ( !prefs.show_home )
      gtk_widget_hide(bw->home_button);
   if ( !prefs.show_reload )
      gtk_widget_hide(bw->reload_button);
   if ( !prefs.show_save )
      gtk_widget_hide(bw->save_button);
   if ( !prefs.show_stop )
      gtk_widget_hide(bw->stop_button);
   if ( !prefs.show_bookmarks )
      gtk_widget_hide(bw->bookmarks_button);
   if ( !prefs.show_menubar )
      gtk_widget_hide(bw->menubar);
   if ( !prefs.show_clear_url)
      gtk_widget_hide(bw->clear_url_button);
   if ( !prefs.show_url )
      gtk_widget_hide(bw->location);
   if ( !prefs.show_search )
      gtk_widget_hide(bw->search_button);
   if ( !prefs.show_progress_box )
      gtk_widget_hide(bw->progress_box);

   bw->fullwindow = prefs.fullwindow_start;
   Interface_toggle_panel(bw, FALSE);
}

static void Interface_full_screen_callback (BrowserWindow *bw)
{
   Interface_toggle_panel(bw, TRUE);
}

/*
 * Handler for double-mouse-clicks that don't belong to the viewport.
 */
static gint Interface_click_callback(BrowserWindow *bw, GdkEventButton *event)
{
   if (event->type == GDK_2BUTTON_PRESS && event->button == 1)
      Interface_toggle_panel(bw, TRUE);
   return TRUE;
}

/*
 * Handler for key presses that don't belong to the viewport.
 * (Used to customize the interface a bit)
 */
static void Interface_key_press_handler(GtkWidget *widget,
                                        GdkEventKey *event,
                                        gpointer client_data)
{
   BrowserWindow *bw = client_data;

   switch (event->keyval) {
   case GDK_BackSpace:
      /* This key is handled here because GTK accel group ignores it */
      if (event->state & GDK_SHIFT_MASK)
         a_Commands_forw_callback(NULL, bw);
      else
         a_Commands_back_callback(NULL, bw);
      break;
   case GDK_slash:
      /* This key is handled here because GTK accel group ignores it */
      a_Commands_findtext_callback(NULL, bw);
      break;
   default:
      _MSG(">> Key pressed!\n");
      break;
   }
}

/*
 * Add the button for switching into fullscreen mode
 */
static void Interface_add_full_screen_button (BrowserWindow *bw, GtkBox *box)
{
   /* The button is put into a vbox, so that it is not enlarged vertically. */
   GtkWidget *vbox, *dummy, *button, *pixmap;

   vbox = gtk_vbox_new (FALSE, 0);
   gtk_box_pack_start (GTK_BOX (box), vbox, FALSE, FALSE, 0);
   gtk_widget_show (vbox);

   /* The dummy will make the button align at the bottom.
    * (Important only when using large text fonts.) */
   dummy = gtk_vbox_new (FALSE, 0);
   gtk_box_pack_start (GTK_BOX (vbox), dummy, TRUE, TRUE, 0);
   gtk_widget_show (dummy);

   button = gtk_button_new ();
   gtk_tooltips_set_tip (tooltips, button, "Hide Controls", "Show Controls");
   GTK_WIDGET_UNSET_FLAGS (button, GTK_CAN_FOCUS);
   gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
   gtk_widget_show (button);

   pixmap = Interface_pixmap_new(bw->main_window, full_screen_on_xpm);
   gtk_container_add (GTK_CONTAINER (button), pixmap);
   gtk_widget_show (pixmap);

   gtk_signal_connect_object
      (GTK_OBJECT (button), "clicked",
       GTK_SIGNAL_FUNC (Interface_full_screen_callback), (gpointer)bw);
}

/*
 * Create a new browser window and return it.
 * (the new window is stored in browser_window[])
 */
BrowserWindow *
a_Interface_browser_window_new(gint width, gint height, guint32 xid)
{
   GtkWidget *box1, *hbox, *button, *label,
             *progbox, *toolbar, *handlebox, *menubar, *locbox, *pixmap;
   BrowserWindow *bw;
   char buf[64];

   /* We use g_new0() to zero the memory */
   bw = g_new0(BrowserWindow, 1);
   a_List_add(browser_window, num_bw, num_bw_max);
   browser_window[num_bw++] = bw;

   /* initialize nav_stack struct in browser_window struct */
   a_Nav_init(bw);

   if (!xid)
       bw->main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   else
       bw->main_window = gtk_plug_new(xid);

   gtk_window_set_policy(GTK_WINDOW(bw->main_window), TRUE, TRUE, FALSE);
   gtk_signal_connect(GTK_OBJECT(bw->main_window), "delete_event",
                      GTK_SIGNAL_FUNC(gtk_object_destroy), bw);
   gtk_signal_connect(GTK_OBJECT(bw->main_window), "destroy",
                      GTK_SIGNAL_FUNC(Interface_quit), bw);
   gtk_container_border_width(GTK_CONTAINER(bw->main_window), 0);

   gtk_window_set_wmclass(GTK_WINDOW(bw->main_window), "dillo", "Dillo");

   /* -RL :: I must realize the window to see it correctly */
   gtk_widget_realize(bw->main_window);

   /* Create and attach an accel group to the main window */
   bw->accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(bw->main_window), bw->accel_group);

   /* set window title */
   g_snprintf(buf, 64, "Version %s", VERSION);
   a_Interface_set_page_title(bw, buf);

   box1 = gtk_vbox_new(FALSE, 0);

   /* setup the control panel */
   if (prefs.panel_size == 1) {
      handlebox = gtk_handle_box_new();
      bw->PanelHandles = g_slist_append(bw->PanelHandles, handlebox);
      hbox = gtk_hbox_new(FALSE, 0);
      /* Control Buttons */
      toolbar = Interface_toolbar_new(bw, 0);
      /* Menus */
      menubar = a_Menu_mainbar_new(bw, 1);
      /* Location entry */
      locbox = Interface_locbar_new(bw);
      /* progress bars */
      progbox = Interface_progressbox_new(bw, 0);

      gtk_box_pack_start(GTK_BOX(hbox), toolbar, FALSE, FALSE, 0);
      gtk_widget_show(toolbar);
      gtk_box_pack_start(GTK_BOX(hbox), menubar, FALSE, FALSE, 0);
      gtk_widget_show(menubar);
      gtk_box_pack_start(GTK_BOX(hbox), locbox, TRUE, TRUE, 0);
      gtk_widget_show(locbox);
      gtk_box_pack_start(GTK_BOX(hbox), progbox, FALSE, FALSE, 0);
      gtk_widget_show(progbox);
      gtk_container_add(GTK_CONTAINER(handlebox), hbox);
      gtk_widget_show(hbox);
      gtk_box_pack_start(GTK_BOX(box1), handlebox, FALSE, FALSE, 0);
      gtk_widget_show(handlebox);

   } else if (prefs.panel_size == 2 || prefs.panel_size == 3) {
      handlebox = gtk_handle_box_new();
      bw->PanelHandles = g_slist_append(bw->PanelHandles, handlebox);
      hbox = gtk_hbox_new(FALSE, 0);
      menubar = a_Menu_mainbar_new(bw, 0);
      locbox = Interface_locbar_new(bw);
      gtk_box_pack_start(GTK_BOX(hbox), menubar, FALSE, FALSE, 0);
      gtk_widget_show(menubar);
      gtk_box_pack_start(GTK_BOX(hbox), locbox, TRUE, TRUE, 0);
      gtk_widget_show(locbox);
      gtk_container_add(GTK_CONTAINER(handlebox), hbox);
      gtk_widget_show(hbox);
      gtk_box_pack_start(GTK_BOX(box1), handlebox, FALSE, FALSE, 0);
      gtk_widget_show(handlebox);

      handlebox = gtk_handle_box_new();
      bw->PanelHandles = g_slist_append(bw->PanelHandles, handlebox);
      gtk_container_border_width(GTK_CONTAINER(handlebox), 4);
      hbox = gtk_hbox_new(FALSE, 0);
      toolbar = Interface_toolbar_new(bw, (prefs.panel_size == 3));
      progbox = Interface_progressbox_new(bw, (prefs.panel_size == 3));
      gtk_box_pack_start(GTK_BOX(hbox), toolbar, TRUE, TRUE, 0);
      gtk_widget_show(toolbar);
      gtk_box_pack_start(GTK_BOX(hbox), progbox, FALSE, FALSE, 0);
      gtk_widget_show(progbox);
      gtk_container_add(GTK_CONTAINER(handlebox), hbox);
      gtk_widget_show(hbox);
      gtk_box_pack_start(GTK_BOX(box1), handlebox, FALSE, FALSE, 0);
      gtk_widget_show(handlebox);

   } else {
      handlebox = gtk_handle_box_new();
      bw->PanelHandles = g_slist_append(bw->PanelHandles, handlebox);
      menubar = a_Menu_mainbar_new(bw, 0);
      gtk_container_add(GTK_CONTAINER(handlebox), menubar);
      gtk_widget_show(menubar);
      gtk_box_pack_start(GTK_BOX(box1), handlebox, FALSE, FALSE, 0);
      gtk_widget_show(handlebox);

      handlebox = gtk_handle_box_new();
      bw->PanelHandles = g_slist_append(bw->PanelHandles, handlebox);
      gtk_container_border_width(GTK_CONTAINER(handlebox), 4);
      hbox = gtk_hbox_new(FALSE, 0);
      toolbar = Interface_toolbar_new(bw, 1);
      progbox = Interface_progressbox_new(bw, 1);
      gtk_box_pack_start(GTK_BOX(hbox), toolbar, TRUE, TRUE, 0);
      gtk_widget_show(toolbar);
      gtk_box_pack_start(GTK_BOX(hbox), progbox, FALSE, FALSE, 0);
      gtk_widget_show(progbox);
      gtk_container_add(GTK_CONTAINER(handlebox), hbox);
      gtk_widget_show(hbox);
      gtk_box_pack_start(GTK_BOX(box1), handlebox, FALSE, FALSE, 0);
      gtk_widget_show(handlebox);

      handlebox = gtk_handle_box_new();
      bw->PanelHandles = g_slist_append(bw->PanelHandles, handlebox);
      locbox = Interface_locbar_new(bw);
      gtk_container_add(GTK_CONTAINER(handlebox), locbox);
      gtk_widget_show(locbox);
      gtk_box_pack_start(GTK_BOX(box1), handlebox, FALSE, FALSE, 0);
      gtk_widget_show(handlebox);
   }

   /* Add box1 */
   gtk_container_add(GTK_CONTAINER(bw->main_window), box1);

   /* Now the main document window */
   bw->docwin = a_Dw_gtk_scrolled_window_new();
   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(bw->docwin),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   gtk_box_pack_start(GTK_BOX(box1), bw->docwin, TRUE, TRUE, 0);
   gtk_widget_show(bw->docwin);

   /* todo (GTK2): this call is a bit of a workaround to missing focus
    * between the location box and the docwin. It's fixed in GTK2:
    * http://mail.gnome.org/archives/gtk-list/2003-June/msg00307.html  */
   gtk_widget_grab_focus(GTK_BIN(bw->docwin)->child);

   /* Full screen mode via double click is done in two ways: First,
    * a feature of the selection is used, since in complex pages,
    * getting events back to the viewport is quite difficult. Second,
    * a simple callback, called e.g. when viewing image resources. */
   a_Selection_set_dclick_callback(
      GTK_DW_VIEWPORT(GTK_BIN(GTK_BIN(bw->docwin)->child)->child)->selection,
      (void(*)(gpointer))Interface_full_screen_callback, bw);
   /* Selection requires an owner widget */
   a_Selection_set_owner(
      GTK_DW_VIEWPORT(GTK_BIN(GTK_BIN(bw->docwin)->child)->child)->selection,
      GTK_BIN(bw->docwin)->child);

   gtk_signal_connect_object_after(GTK_OBJECT(GTK_BIN(bw->docwin)->child),
                                   "button_press_event",
                                   GTK_SIGNAL_FUNC(Interface_click_callback),
                                   (gpointer)bw);

   /* full screen button: actually there're two buttons, one in the scrolled
    * window (fs off) and one in the status bar (fs on). They look as one. */
   bw->full_screen_off_button = gtk_button_new ();
   gtk_tooltips_set_tip (tooltips, bw->full_screen_off_button,
                         "Show Controls", "Hide Controls");
   GTK_WIDGET_UNSET_FLAGS (bw->full_screen_off_button, GTK_CAN_FOCUS);
   a_Dw_gtk_scrolled_window_add_gadget (GTK_DW_SCROLLED_WINDOW (bw->docwin),
                                        bw->full_screen_off_button);
   pixmap = Interface_pixmap_new(bw->main_window, full_screen_off_xpm);
   gtk_container_add (GTK_CONTAINER (bw->full_screen_off_button), pixmap);
   gtk_widget_show (pixmap);
   gtk_signal_connect_object
      (GTK_OBJECT (bw->full_screen_off_button), "clicked",
       GTK_SIGNAL_FUNC (Interface_full_screen_callback), (gpointer)bw);
   Interface_set_button_accel(GTK_BUTTON(bw->full_screen_off_button), GDK_h,
                              GDK_CONTROL_MASK, bw->accel_group);

   /* Catch key_press event */
   gtk_signal_connect(GTK_OBJECT(GTK_BIN(bw->docwin)->child),
                      "key_press_event",
                      GTK_SIGNAL_FUNC(Interface_key_press_handler), bw);

   gtk_widget_set_usize(bw->main_window, width, height);

   /* status widget */
   /* create the over-bug-meter menu */
   bw->menu_popup.over_bug = a_Menu_popup_ob_new(bw);

   bw->status = a_Dw_gtk_statuslabel_new("");
   gtk_misc_set_alignment(GTK_MISC(bw->status), 0.0, 0.5);
   /*  status widget for HTML errors.
    *  Note: the "clicked" signal is hooked with the linkblock later */
   button = bw->status_bug_meter = a_Gtk_ext_button_new();
   GTK_WIDGET_UNSET_FLAGS (button, GTK_CAN_FOCUS);
   a_Gtk_ext_button_set_command (GTK_EXT_BUTTON (button), 1);
   a_Gtk_ext_button_attach_menu (GTK_EXT_BUTTON (button), 3,
                                 GTK_MENU(bw->menu_popup.over_bug));

   hbox = gtk_hbox_new(FALSE, 0);
   pixmap = Interface_pixmap_new(bw->main_window, mini_bug_xpm);
   gtk_object_set_data(GTK_OBJECT (button), "bug", pixmap);
   gtk_box_pack_start(GTK_BOX(hbox), pixmap, FALSE, FALSE, 0);
   pixmap = Interface_pixmap_new(bw->main_window, mini_ok_xpm);
   gtk_object_set_data(GTK_OBJECT (button), "ok", pixmap);
   gtk_box_pack_start(GTK_BOX(hbox), pixmap, FALSE, FALSE, 0);
   gtk_widget_show (pixmap);
   label = gtk_label_new("");
   gtk_object_set_data(GTK_OBJECT (button), "label", label);
   gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
   gtk_container_add (GTK_CONTAINER (button), hbox);
   gtk_widget_show(hbox);
   gtk_tooltips_set_tip (tooltips, button,
                         "Show HTML bugs (right-click for menu).", "");

   bw->status_box = gtk_hbox_new(FALSE, 0);
   gtk_box_pack_start(GTK_BOX(bw->status_box), bw->status, TRUE, TRUE, 2);
   gtk_widget_show(bw->status);
   gtk_box_pack_start(GTK_BOX(bw->status_box), button, FALSE, FALSE, 0);
   gtk_widget_show(button);

   Interface_add_full_screen_button (bw, GTK_BOX (bw->status_box));

   gtk_box_pack_start(GTK_BOX(box1), bw->status_box, FALSE, FALSE, 0);
   gtk_widget_show(bw->status_box);

   gtk_widget_show(bw->main_window);
   gtk_widget_show(box1);

   /* initialize the rest of the bw's data. */
   bw->pagemarks_menuitem = NULL;
   bw->pagemarks_menu = NULL;
   bw->pagemarks_last = NULL;
   bw->viewbugs_menuitem = NULL;

   /* the image menu is created first because it is used by the link menu */
   bw->menu_popup.over_image = a_Menu_popup_oi_new(bw);
   bw->menu_popup.over_link = a_Menu_popup_ol_new(bw);
   bw->menu_popup.over_page = a_Menu_popup_op_new(bw);
   bw->menu_popup.over_back = NULL;
   bw->menu_popup.over_forw = NULL;
   bw->menu_popup.url = NULL;
   bw->menu_popup.url2 = NULL;

   bw->redirect_level = 0;
   bw->sens_idle_id = 0;

   bw->CursorType = -1;

   bw->RootClients = NULL;
   bw->NumRootClients = 0;
   bw->MaxRootClients = 8;

   bw->ImageClients = NULL;
   bw->NumImageClients = 0;
   bw->MaxImageClients = 8;
   bw->NumImages = 0;
   bw->NumImagesGot = 0;

   bw->PageUrls = NULL;
   bw->NumPageUrls = 0;
   bw->MaxPageUrls = 8;

   bw->open_dialog_window = NULL;
   bw->open_dialog_entry = NULL;
   bw->openfile_dialog_window = NULL;
   bw->quit_dialog_window = NULL;
   bw->save_dialog_window = NULL;
   bw->save_link_dialog_window = NULL;
   bw->findtext_dialog_window = NULL;
   bw->findtext_dialog_check = NULL;
   bw->findtext_dialog_entry = NULL;
   bw->search_dialog_window = NULL;
   bw->search_dialog_entry = NULL;
   bw->proxy_passwd_dialog_window = NULL;
   bw->proxy_passwd_dialog_entry = NULL;
   bw->question_dialog_window = NULL;
   bw->question_dialog_data = NULL;
   bw->viewsource_window = NULL;
   bw->pagebugs_window = NULL;

   /* now that the bw is made, let's customize it.. */
   Interface_browser_window_customize(bw);

   return bw;
}

/*
 * Set the title of the browser window to start with "Dillo: "
 * prepended to it.
 */
void a_Interface_set_page_title(BrowserWindow *bw, char *title)
{
   GString *buf;

   g_return_if_fail (bw != NULL && title != NULL);

   buf = g_string_new("");
   g_string_sprintfa(buf, "Dillo: %s", title);
   gtk_window_set_title(GTK_WINDOW(bw->main_window), buf->str);
   g_string_free(buf, TRUE);
}

/*
 * Set location entry's text
 */
void a_Interface_set_location_text(BrowserWindow *bw, char *text)
{
   gtk_entry_set_text(GTK_ENTRY(bw->location), text);
}

/*
 * Get location entry's text
 */
gchar *a_Interface_get_location_text(BrowserWindow *bw)
{
   return gtk_entry_get_text(GTK_ENTRY(bw->location));
}

/*
 * Reset images and text progress bars
 */
void a_Interface_reset_progress_bars(BrowserWindow *bw)
{
   a_Progressbar_update(bw->progress, "", 0);
   a_Progressbar_update(bw->imgprogress, "", 0);
}

/*
 * Set the status string on the bottom of the dillo window.
 */
void a_Interface_msg(BrowserWindow *bw, const char *format, ... )
{
   static char msg[1024];
   va_list argp;

   if ( bw ) {
      va_start(argp, format);
      g_vsnprintf(msg, 1024, format, argp);
      va_end(argp);
      gtk_label_set_text(GTK_LABEL(bw->status), msg);
      bw->status_is_link = 0;
   }
}

/*
 * Update the bug-meter button for detected page errors.
 */
void a_Interface_bug_meter_update(BrowserWindow *bw, gint num_err)
{
   static char msg[64];
   gpointer label, bug, ok;

   if ( bw ) {
      label = gtk_object_get_data(GTK_OBJECT(bw->status_bug_meter), "label");
      if (num_err < 2) {
         bug = gtk_object_get_data(GTK_OBJECT(bw->status_bug_meter), "bug");
         ok = gtk_object_get_data(GTK_OBJECT(bw->status_bug_meter), "ok");
         if (num_err == 0) {
            gtk_widget_hide(bug);
            gtk_widget_hide(label);
            gtk_widget_show(ok);
         } else {
            gtk_widget_show(bug);
            gtk_widget_show(label);
            gtk_widget_hide(ok);
         }
         gtk_widget_queue_resize (GTK_WIDGET (bw->status_bug_meter));
      }
      g_snprintf(msg, 64, " %d", num_err);
      gtk_label_set_text(GTK_LABEL(label), msg);
   }
}

/*
 * Called from `destroy' callback in Interface_make_*_dialog
 */
static void Interface_destroy_window(GtkWidget *widget, GtkWidget **window)
{
   /* todo: sometimes this function is called twice with dialog windows */
   _MSG("Interface_destroy_window %p\n", *window);
   if (*window) {
      gtk_widget_destroy(*window);
      *window = NULL;
   }
}


/*
 * Close and free every single browser_window (called at exit time)
 */
void a_Interface_quit_all(void)
{
   BrowserWindow **bws;
   gint i, n_bw;

   n_bw = num_bw;
   bws = g_malloc(sizeof(BrowserWindow *) * n_bw);

   /* we copy into a new list because destroying the main window can
    * modify the browser_window array. */
   for (i = 0; i < n_bw; i++)
      bws[i] = browser_window[i];

   for (i = 0; i < n_bw; i++)
      gtk_widget_destroy(bws[i]->main_window);

   g_free(bws);
   
   g_free(open_dialog_last_dirname);
   g_free(save_dialog_last_dirname);
}

/*
 * Make a dialog for choosing files (by calling
 * gtk_file_selection_*() calls)
 * This can be used for saving, opening, or whatever,
 * just set the correct callbacks
 */
static void
 Interface_make_choose_file_dialog(GtkWidget **DialogWindow,
                                   char *WmName, char *WmClass, char *WTitle,
                                   GtkSignalFunc B1CallBack, void *B1CbData)
{
   GtkAccelGroup *accel_group;

   *DialogWindow = gtk_file_selection_new(WTitle);
   gtk_window_set_modal(GTK_WINDOW(*DialogWindow), FALSE);
   gtk_window_set_wmclass(GTK_WINDOW(*DialogWindow), WmName, WmClass);

   gtk_file_selection_hide_fileop_buttons(GTK_FILE_SELECTION(*DialogWindow));
   gtk_signal_connect(
      GTK_OBJECT(*DialogWindow),
      "destroy", (GtkSignalFunc) Interface_destroy_window, DialogWindow);
   gtk_signal_connect(
      GTK_OBJECT(GTK_FILE_SELECTION(*DialogWindow)->ok_button),
      "clicked", (GtkSignalFunc) B1CallBack, B1CbData);
   gtk_signal_connect(
      GTK_OBJECT(GTK_FILE_SELECTION (*DialogWindow)->cancel_button),
      "clicked", (GtkSignalFunc) Interface_destroy_window, DialogWindow);

   /* Make GDK_Escape close the dialog */
   accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(*DialogWindow), accel_group);
   gtk_widget_add_accelerator(
      GTK_FILE_SELECTION(*DialogWindow)->cancel_button, "clicked",
      accel_group, GDK_Escape, 0, GTK_ACCEL_LOCKED);
}

/*
 * Get the file URL from the widget and push it to the browser window.
 */
static void
 Interface_openfile_ok_callback(GtkWidget *widget, BrowserWindow *bw)
{
   char *fn, *Cfn, *s;
   DilloUrl *url;
   GString *UrlStr = g_string_sized_new(1024);

   fn = gtk_file_selection_get_filename(
           GTK_FILE_SELECTION(bw->openfile_dialog_window));

   Cfn = (s = a_Misc_escape_chars(fn, "% ")) ? s : fn;
   g_string_sprintf(UrlStr, "file:%s", Cfn);
   url = a_Url_new(UrlStr->str, NULL, 0, 0, 0);
   a_Nav_push(bw, url);
   a_Url_free(url);
   g_string_free(UrlStr, TRUE);
   g_free(s);

   g_free(open_dialog_last_dirname);
   open_dialog_last_dirname = g_strdup(fn);

   gtk_widget_destroy(bw->openfile_dialog_window);
}

/*
 * Returns a newly allocated string holding a search url generated from
 * a string of keywords (separarated by blanks) and prefs.search_url.
 * The search string is urlencoded.
 */
static gchar *Interface_make_search_url(const gchar *str)
{
   gchar *keys = a_Url_encode_hex_str(str), *c = prefs.search_url;
   GString *newstr = g_string_sized_new(strlen(c)+strlen(keys));
   gchar *search_url;

   for (; *c; c++)
      if (*c == '%')
         switch(*++c) {
         case 's':
            g_string_append(newstr, keys); break;;
         case '%':
            g_string_append_c(newstr, '%'); break;;
         case 0:
            MSG("Warning: search_url ends with '%%'\n"); c--; break;;
         default:
            MSG("Warning: illegal specifier '%%%c' in search_url\n", *c);
         }
      else
         g_string_append_c(newstr, *c);

   g_free(keys);

   search_url = newstr->str;
   g_string_free(newstr, FALSE);
   return search_url;
}

/*
 * Open an url string.
 * The URL is not sent "as is", illegal chars are ripped out,
 * then it's fully parsed by a_Url_new().
 */
static void Interface_open_url_string(gchar *text, BrowserWindow *bw)
{
   gchar *new_text;
   DilloUrl *url;

   if (text && *text) {
      /* Filter URL string */
      new_text = a_Url_string_strip_delimiters(text);

      url = a_Url_new(new_text, NULL, 0, 0, 0);
      if (url) {
         a_Nav_push(bw, url);
         a_Url_free(url);
      }
      g_free(new_text);
   }

   /* let the rendered area have focus */
   gtk_widget_grab_focus(GTK_BIN(bw->docwin)->child);
}

/*
 * Open an URL specified in the location entry, or in the open URL dialog.
 */
void a_Interface_entry_open_url(GtkWidget *widget, BrowserWindow *bw)
{
   gchar *text;
   GtkEntry *entry;

   /* entry = { bw->location | bw->open_dialog_entry } */
   entry = GTK_ENTRY(widget == bw->location ? widget : bw->open_dialog_entry);
   text = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
   DEBUG_MSG(1, "entry_open_url %s\n", text);
   Interface_open_url_string(text, bw);
   g_free(text);

   if (bw->open_dialog_window != NULL)
      gtk_widget_hide(bw->open_dialog_window);
}

/*
 * Create and show the "Open File" dialog
 */
void a_Interface_openfile_dialog(BrowserWindow *bw)
{
   if (!bw->openfile_dialog_window) {
      Interface_make_choose_file_dialog(
         &(bw->openfile_dialog_window),
         "openfile_dialog", "Dillo", "Dillo: Open File",
         (GtkSignalFunc) Interface_openfile_ok_callback, (void *)bw);
   }

   if (open_dialog_last_dirname)
      gtk_file_selection_set_filename(
         GTK_FILE_SELECTION(bw->openfile_dialog_window),
         open_dialog_last_dirname);

   if (!GTK_WIDGET_VISIBLE(bw->openfile_dialog_window))
      gtk_widget_show(bw->openfile_dialog_window);
   else
      gdk_window_raise(bw->openfile_dialog_window->window);
}

/*
 * Make a dialog interface with three buttons, a text entry, and an optional
 * check button.
 */
static void
 Interface_make_dialog(GtkWidget **DialogWindow, char *WmName, char *WmClass,
   char *WTitle, GtkWidget **DialogEntry, char *EntryStr, gint VisibleEntry,
   GtkWidget **CheckButton, char *CheckButtonText,
   char *B1Label, GtkSignalFunc B1CallBack, void *B1CbData)
{
   GtkWidget *button, *box1, *box2, *entry;
   GtkAccelGroup *accel_group;

   *DialogWindow = gtk_window_new(GTK_WINDOW_DIALOG);
   gtk_window_set_wmclass(GTK_WINDOW(*DialogWindow), WmName, WmClass);
   gtk_window_set_position(GTK_WINDOW(*DialogWindow), GTK_WIN_POS_CENTER);
   gtk_window_set_title(GTK_WINDOW(*DialogWindow), WTitle);
   gtk_signal_connect(GTK_OBJECT(*DialogWindow), "destroy",
                      (GtkSignalFunc) Interface_destroy_window, DialogWindow);

   /* Create and attach an accel group to the dialog window */
   accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(*DialogWindow), accel_group);

   gtk_container_border_width(GTK_CONTAINER(*DialogWindow), 5);

   box1 = gtk_vbox_new(FALSE, 5);
   gtk_container_add(GTK_CONTAINER(*DialogWindow), box1);
   gtk_widget_show(box1);

   entry = gtk_entry_new();
   GTK_WIDGET_SET_FLAGS(entry, GTK_HAS_FOCUS);
   gtk_widget_set_usize(entry, 250, 0);
   gtk_entry_set_text(GTK_ENTRY(entry), EntryStr);
   gtk_box_pack_start(GTK_BOX(box1), entry, FALSE, FALSE, 0);
   *DialogEntry = GTK_WIDGET(entry);
   gtk_entry_set_visibility(GTK_ENTRY(entry), VisibleEntry ? TRUE : FALSE);
   gtk_widget_show(entry);

   if (CheckButton && CheckButtonText) {
      *CheckButton = gtk_check_button_new_with_label(CheckButtonText);
      gtk_box_pack_start(GTK_BOX(box1), *CheckButton, FALSE, FALSE, 0);
      gtk_widget_show(*CheckButton);
   }

   gtk_signal_connect(GTK_OBJECT(entry), "activate", B1CallBack, B1CbData);

   box2 = gtk_hbox_new(TRUE, 5);
   gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, FALSE, 0);
   gtk_widget_show(box2);

   button = gtk_button_new_with_label(B1Label);
   gtk_signal_connect(GTK_OBJECT(button), "clicked", B1CallBack, B1CbData);
   GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
   gtk_box_pack_start(GTK_BOX(box2), button, FALSE, TRUE, 0);
   gtk_widget_grab_default(button);
   gtk_widget_show(button);
   gtk_signal_connect_object(GTK_OBJECT(entry), "focus_in_event",
                             (GtkSignalFunc) gtk_widget_grab_default,
                             GTK_OBJECT(button));

   button = gtk_button_new_with_label("Clear");
   gtk_signal_connect_object(GTK_OBJECT(button), "clicked",
                             (GtkSignalFunc) Interface_entry_clear,
                             GTK_OBJECT(entry));
   GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
   gtk_box_pack_start(GTK_BOX(box2), button, FALSE, TRUE, 0);
   gtk_widget_show(button);

   button = gtk_button_new_with_label("Cancel");
   gtk_signal_connect_object(GTK_OBJECT(button), "clicked",
                             (GtkSignalFunc) gtk_widget_destroy,
                             GTK_OBJECT(*DialogWindow));
   gtk_widget_add_accelerator(button, "clicked", accel_group,
                              GDK_Escape, 0, GTK_ACCEL_LOCKED);
   GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
   gtk_box_pack_start(GTK_BOX(box2), button, FALSE, TRUE, 0);
   gtk_widget_show(button);

   gtk_widget_grab_focus(entry);
}

/*
 * Set bw->question_dialog_answer
 */
static void Interface_question_dialog_set_answer_cb(DialogAnswer *answer)
{
   _MSG("Interface_question_dialog_set_answer_cb\n");
   answer->bw->question_dialog_answer = answer;
}

/*
 * Make a question-dialog with a question and some alternatives.
 * The selected choices are left in bw->question_dialog_answer in a structure.
 * A generic callback function can decide afterwards based on that information.
 * (0 means the window was cancelled, and 1 to 5 the respective alternatives)
 */
static void Interface_make_question_dialog(
        BrowserWindow *bw,
        GtkWidget **DialogWindow, char *WmName, char *WmClass,
        char *WTitle, char *Question, gint modal_flag,
        char *alt1, char *alt2, char *alt3, char *alt4, char *alt5,
        GtkSignalFunc AnswerCallback)
{
   GtkWidget *frame, *label, *button = NULL, *box1, *box2;
   DialogAnswer *answer;
   int i;

   *DialogWindow = gtk_window_new(GTK_WINDOW_DIALOG);
   gtk_window_set_wmclass(GTK_WINDOW(*DialogWindow), WmName, WmClass);
   gtk_window_set_title(GTK_WINDOW(*DialogWindow), WTitle);
   gtk_window_set_position(GTK_WINDOW(*DialogWindow), GTK_WIN_POS_CENTER);
   gtk_container_border_width(GTK_CONTAINER(*DialogWindow), 10);
   gtk_window_set_modal(GTK_WINDOW(*DialogWindow), modal_flag);
   /* attach AnswerCallback to "destroy" so appropriate actions can be taken */
   gtk_signal_connect_object(GTK_OBJECT(*DialogWindow), "destroy",
      (GtkSignalFunc) AnswerCallback, (gpointer)bw);
   gtk_signal_connect(GTK_OBJECT(*DialogWindow), "destroy",
      (GtkSignalFunc) Interface_destroy_window, DialogWindow);

   box1 = gtk_vbox_new(FALSE, 5);
   frame = gtk_frame_new(NULL);
   gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
   label = gtk_label_new(Question);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_FILL);
   gtk_misc_set_padding(GTK_MISC(label), 20, 20);
   gtk_container_add(GTK_CONTAINER(frame), label);
   gtk_widget_show(label);
   gtk_widget_show(frame);
   gtk_box_pack_start(GTK_BOX(box1), frame, TRUE, TRUE, 0);

   /* This struct will save us some code and lots of callback functions.
    * (it looks clumsy, but provides a handy interface for the caller) */
   answer = g_new(DialogAnswer, 6);
   for (i = 0; i < 6; ++i) {
      answer[i].bw = bw;
      answer[i].alt_num = i;
      answer[i].this = answer;
   }
   answer[0].alt_str = NULL; answer[1].alt_str = alt1;
   answer[2].alt_str = alt2; answer[3].alt_str = alt3;
   answer[4].alt_str = alt4; answer[5].alt_str = alt5;
   /* Set the default answer */
   bw->question_dialog_answer = &answer[0];

   /* pack the alternatives */
   box2 = gtk_hbox_new(TRUE, 5);
   for (i = 1; i < 6 && answer[i].alt_str; ++i) {
      button = gtk_button_new_with_label(answer[i].alt_str);
      gtk_signal_connect_object(GTK_OBJECT(button), "clicked",
         (GtkSignalFunc) Interface_question_dialog_set_answer_cb,
         (gpointer)&answer[i]);
      gtk_signal_connect(GTK_OBJECT(button), "clicked",
         (GtkSignalFunc) Interface_destroy_window, DialogWindow);
      gtk_widget_show(button);
      gtk_box_pack_start(GTK_BOX(box2), button, FALSE, TRUE, 0);
   }

   gtk_box_pack_start(GTK_BOX(box1), box2, FALSE, FALSE, 0);
   gtk_container_add(GTK_CONTAINER(*DialogWindow), box1);

   gtk_widget_show(box2);
   gtk_widget_show(box1);
   gtk_widget_grab_focus(button);
   gtk_widget_show(*DialogWindow);
}

/*
 * Create and show an [OK|Cancel] question dialog
 */
void a_Interface_question_dialog(
        BrowserWindow *bw, gchar *QuestionTxt, gint modal_flag,
        char *alt1, char *alt2, char *alt3, char *alt4, char *alt5,
        GtkSignalFunc AnswerCallback)
{
   if (!bw->question_dialog_window) {
      Interface_make_question_dialog(
         bw,
         &(bw->question_dialog_window), "question_dialog", "Dillo",
         "Dillo: Question", QuestionTxt, modal_flag,
         alt1, alt2, alt3, alt4, alt5,
         AnswerCallback);
   } else {
      /* should not reach here */
      gtk_widget_destroy(bw->question_dialog_window);
   }
}

/*
 * Create and show the open URL dialog
 */
void a_Interface_open_dialog(GtkWidget *widget, BrowserWindow *bw)
{
   if (!bw->open_dialog_window) {
      Interface_make_dialog(&(bw->open_dialog_window),
         "open_dialog", "Dillo", "Dillo: Open URL",
         &(bw->open_dialog_entry), "", 1, NULL, NULL,
         "OK", (GtkSignalFunc) a_Interface_entry_open_url, (void *)bw);
      if (prefs.transient_dialogs)
         gtk_window_set_transient_for(GTK_WINDOW(bw->open_dialog_window),
                                      GTK_WINDOW(bw->main_window));
   }

   if (!GTK_WIDGET_VISIBLE(bw->open_dialog_window))
      gtk_widget_show(bw->open_dialog_window);
   else
      gdk_window_raise(bw->open_dialog_window->window);
}

/*
 * Receive data from the cache and save it to a local file
 */
static void Interface_save_callback(int Op, CacheClient_t *Client)
{
   DilloWeb *Web = Client->Web;
   gint Bytes;

   if ( Op ){
      struct stat st;

      fflush(Web->stream);
      fstat(fileno(Web->stream), &st);
      fclose(Web->stream);
      a_Interface_msg(Web->bw, "File saved (%d Bytes)", st.st_size);
   } else {
      if ( (Bytes = Client->BufSize - Web->SavedBytes) > 0 ) {
         Bytes = fwrite(Client->Buf + Web->SavedBytes, 1, Bytes, Web->stream);
         Web->SavedBytes += Bytes;
      }
   }
}

/*
 * Save current page to a local file
 */
static void Interface_file_save_url(GtkWidget *widget, BrowserWindow *bw)
{
   const char *name;
   GtkFileSelection *choosefile;
   GtkEntry *entry_url;
   DilloUrl *url;
   FILE *out;

   choosefile = GTK_FILE_SELECTION(bw->save_dialog_window);
   entry_url = GTK_ENTRY(bw->location);
   name = gtk_file_selection_get_filename(choosefile);
   url = a_Url_dup(a_History_get_url(NAV_TOP(bw)));

   if ( strlen(name) && (out = fopen(name, "w")) != NULL ) {
      DilloWeb *Web = a_Web_new(url);
      Web->bw = bw;
      Web->stream = out;
      Web->flags |= WEB_Download;
      /* todo: keep track of this client */
      a_Capi_open_url(Web, Interface_save_callback, Web);

      g_free(save_dialog_last_dirname);
      save_dialog_last_dirname = g_strdup(name);
   }
   a_Url_free(url);

   gtk_widget_destroy(bw->save_dialog_window);
}

/*
 * Save the link-URL to a local file
 */
static void Interface_file_save_link(GtkWidget *widget, BrowserWindow *bw)
{
   const gchar *name;
   const DilloUrl *url;
   gchar *cmd, *buf, *s_url, *s_name;
   FILE *out;
   gint buf_size;

   name = gtk_file_selection_get_filename(
             GTK_FILE_SELECTION(bw->save_link_dialog_window));
   url  = a_Menu_popup_get_url(bw);

   g_free(save_dialog_last_dirname);
   save_dialog_last_dirname = g_strdup(name);

   if (!a_Capi_get_buf(url, &buf, &buf_size)) {
      /* Not cached, ask the downloads server to get it */
      s_url = a_Misc_stuff_chars(URL_STR(url), "'\"");
      s_name = a_Misc_stuff_chars(name, "'\"");
      cmd = g_strdup_printf(
               "<dpi cmd='download' url='%s' destination='%s'>\n",
               s_url, s_name);
      a_Capi_dpi_send_cmd(NULL, bw, cmd, "downloads", 1);
      g_free(cmd);
      g_free(s_name);
      g_free(s_url);

   } else {
      /* Cached! Save from the cache */
      if ( strlen(name) && (out = fopen(name, "w")) != NULL ) {
         DilloWeb *Web = a_Web_new(url);
         Web->bw = bw;
         Web->stream = out;
         Web->flags |= WEB_Download;
         /* todo: keep track of this client */
         a_Capi_open_url(Web, Interface_save_callback, Web);
      } else
         g_printerr("Error trying to save: %s\n", name);
   }

   gtk_widget_destroy(bw->save_link_dialog_window);
}

/*
 * Scan Url and return a local-filename suggestion for saving
 */
static char *Interface_make_save_name(const DilloUrl *url)
{
   gchar *FileName, *FilenameWithDir;

   if ((FileName = strrchr(URL_PATH(url), '/')))
      FileName = g_strndup(FileName + 1, MIN(strlen(FileName + 1), 80));
   else
      FileName = g_strdup("");

   if (save_dialog_last_dirname) {
      if (*FileName) {
        gchar *dirpart = g_dirname(save_dialog_last_dirname);
        FilenameWithDir =
           g_strconcat(dirpart, G_DIR_SEPARATOR_S, FileName, NULL);
        g_free(dirpart);
      } else {
        FilenameWithDir = g_strdup(save_dialog_last_dirname);
      }
      g_free(FileName);
      return FilenameWithDir;
   } else
      return FileName;
}

/*
 * Show the dialog interface for saving an URL
 */
void a_Interface_save_dialog(GtkWidget *widget, BrowserWindow *bw)
{
   gchar *SuggestedName;   /* Suggested save name */
   DilloUrl* url;

   if (!bw->save_dialog_window) {
      Interface_make_choose_file_dialog(
         &bw->save_dialog_window,
         "save_dialog", "Dillo", "Dillo: Save URL as File...",
         (GtkSignalFunc) Interface_file_save_url, (void *)bw );
   }
   url = a_Url_new(a_Interface_get_location_text(bw), NULL, 0, 0, 0);
   SuggestedName = Interface_make_save_name(url);
   gtk_file_selection_set_filename(
      GTK_FILE_SELECTION(bw->save_dialog_window), SuggestedName);
   g_free(SuggestedName);
   a_Url_free(url);

   if (!GTK_WIDGET_VISIBLE(bw->save_dialog_window))
      gtk_widget_show(bw->save_dialog_window);
   else
      gdk_window_raise(bw->save_dialog_window->window);
}

/*
 * Show the dialog interface for saving a link
 */
void a_Interface_save_link_dialog(GtkWidget *widget, BrowserWindow *bw)
{
   char *SuggestedName;   /* Suggested save name */

   if (!bw->save_link_dialog_window) {
      Interface_make_choose_file_dialog(
         &bw->save_link_dialog_window,
         "save_link_dialog", "Dillo",
         "Dillo: Save link as File...",
         (GtkSignalFunc) Interface_file_save_link,
         (void *)bw);
   }
   SuggestedName = Interface_make_save_name(a_Menu_popup_get_url(bw));
   gtk_file_selection_set_filename(
      GTK_FILE_SELECTION(bw->save_link_dialog_window), SuggestedName);
   g_free(SuggestedName);

   if (!GTK_WIDGET_VISIBLE(bw->save_link_dialog_window))
      gtk_widget_show(bw->save_link_dialog_window);
   else
      gdk_window_raise(bw->save_link_dialog_window->window);
}

/*
 * Offer the "Save Link As..." dialog for an unhandled MIME type URL.
 */
void a_Interface_offer_link_download(BrowserWindow *bw, const DilloUrl *url)
{
   a_Menu_popup_set_url(bw, url);
   a_Interface_save_link_dialog(NULL, bw);
}

/*
 * Scroll to an occurence of a string in the open page
 */
static void Interface_entry_search(GtkWidget *widget, BrowserWindow* bw)
{
   char *string;
   gboolean case_sens;

   string = gtk_editable_get_chars(GTK_EDITABLE(bw->findtext_dialog_entry),
                                   0, -1);
   case_sens = gtk_toggle_button_get_active
      (GTK_TOGGLE_BUTTON(bw->findtext_dialog_check));
   switch(a_Dw_gtk_scrolled_window_search(GTK_DW_SCROLLED_WINDOW(bw->docwin),
                                          string, case_sens)) {
   case FINDTEXT_RESTART:
      a_Interface_message_window("Dillo: Find text",
                                 "No further occurence of \"%s\". "
                                 "Restarting from the beginning.", string);
      break;
   case FINDTEXT_NOT_FOUND:
      a_Interface_message_window("Dillo: Find text",
                                 "Cannot find \"%s\".", string);
      break;
   }
   g_free(string);
}

/*
 * Show the dialog interface for finding text in a page
 */
void a_Interface_findtext_dialog(BrowserWindow *bw)
{
   if (!bw->findtext_dialog_window) {
      Interface_make_dialog(&(bw->findtext_dialog_window),
         "findtext_dialog", "Dillo", "Dillo: Find text in page",
         &(bw->findtext_dialog_entry), "", 1,
         &(bw->findtext_dialog_check), "Case sensitive",
         "Find", (GtkSignalFunc) Interface_entry_search, (void *)bw);
      if (prefs.transient_dialogs)
         gtk_window_set_transient_for(GTK_WINDOW(bw->findtext_dialog_window),
                                      GTK_WINDOW(bw->main_window));
      gtk_signal_connect_object
         (GTK_OBJECT(bw->findtext_dialog_window), "destroy",
          (GtkSignalFunc) a_Dw_gtk_scrolled_window_reset_search,
          (void*)bw->docwin);
      gtk_window_set_position(GTK_WINDOW(bw->findtext_dialog_window),
                              GTK_WIN_POS_NONE);
   }

   a_Interface_set_nice_window_pos(bw->findtext_dialog_window,
                                   bw->main_window);

   if (!GTK_WIDGET_VISIBLE(bw->findtext_dialog_window))
      gtk_widget_show(bw->findtext_dialog_window);
   else
      gdk_window_raise(bw->findtext_dialog_window->window);
}

/*
 * Use the search dialog's entry to feed a web search engine.
 */
static void Interface_search_callback(GtkWidget *widget, BrowserWindow *bw)
{
   gchar *keyw, *url_str;

   keyw = gtk_editable_get_chars(GTK_EDITABLE(bw->search_dialog_entry), 0, -1);
   if (keyw) {
      url_str = Interface_make_search_url(keyw);
      Interface_open_url_string(url_str, bw);
      g_free(url_str);
      g_free(keyw);
   }
   if (bw->search_dialog_window != NULL)
      gtk_widget_hide(bw->search_dialog_window);
}

/*
 * Show the dialog interface for web search engine.
 */
void a_Interface_search_dialog(GtkWidget *widget, BrowserWindow *bw)
{
   if (!bw->search_dialog_window) {
      Interface_make_dialog(&(bw->search_dialog_window),
         "search_dialog", "Dillo", "Dillo: Search the Web",
         &(bw->search_dialog_entry), "", 1, NULL, NULL,
         "Search", (GtkSignalFunc)Interface_search_callback, (void *)bw);
      if (prefs.transient_dialogs)
         gtk_window_set_transient_for(GTK_WINDOW(bw->search_dialog_window),
                                      GTK_WINDOW(bw->main_window));
   }

   if (!GTK_WIDGET_VISIBLE(bw->search_dialog_window))
      gtk_widget_show(bw->search_dialog_window);
   else
      gdk_window_raise(bw->search_dialog_window->window);
}

/*
 * Get and activate a proxy password.
 */
static void Interface_entry_proxy_passwd(GtkWidget *widget, BrowserWindow *bw)
{
   gchar *text;

   text = gtk_editable_get_chars(GTK_EDITABLE(bw->proxy_passwd_dialog_entry),
                                 0, -1);
   a_Http_set_proxy_passwd(text);
   g_free(text);

   if (bw->proxy_passwd_dialog_window != NULL)
      gtk_widget_destroy(bw->proxy_passwd_dialog_window);
}

/*
 * Show the dialog interface for asking proxy password.
 */
void a_Interface_proxy_passwd_dialog(BrowserWindow *bw)
{
   GString *buf = g_string_new("");

   g_string_sprintf(buf, "Dillo: Enter proxy password for '%s'",
                    prefs.http_proxyuser);

   if (!bw->proxy_passwd_dialog_window) {
      Interface_make_dialog(&(bw->proxy_passwd_dialog_window),
         "proxy_passwd_dialog", "Dillo", buf->str,
         &(bw->proxy_passwd_dialog_entry), "", 0, NULL, NULL,
         "OK", (GtkSignalFunc)Interface_entry_proxy_passwd, (void *)bw);
      if (prefs.transient_dialogs)
         gtk_window_set_transient_for(
            GTK_WINDOW(bw->proxy_passwd_dialog_window),
            GTK_WINDOW(bw->main_window));
      gtk_window_set_modal(GTK_WINDOW(bw->proxy_passwd_dialog_window), TRUE);
   }

   if (!GTK_WIDGET_VISIBLE(bw->proxy_passwd_dialog_window))
      gtk_widget_show(bw->proxy_passwd_dialog_window);

   g_string_free(buf, TRUE);
}

/*
 * This signal callback adjusts the position of a menu.
 * It's useful for very long menus.
 */
void a_Interface_scroll_popup(GtkWidget *widget)
{
   /*
    * todo:
    *   1) Scrolling menues should rather be the task of Gtk+. This is
    *      a hack, and I don't know if it does not break anything.
    *   2) It could be improved, e.g. a timeout could be added for
    *      better mouse navigation.
    */
   int y, h, mx, my, sh;

   y = widget->allocation.y;
   h = widget->allocation.height;
   gdk_window_get_geometry (widget->parent->parent->window,
                            &mx, &my, NULL, NULL, NULL);
   sh = gdk_screen_height ();

   if (y + my < 0)
      gdk_window_move (widget->parent->parent->window, mx, - y + 1);
   else if (y + my > sh - h)
      gdk_window_move (widget->parent->parent->window, mx, sh - h - y - 1);
}

/*
 * A general purpose message window.
 */
void a_Interface_message_window(const char *title, const char *format, ... )
{
   GtkAccelGroup *accel_group;
   GtkWidget *window, *frame, *label, *button, *box;
   static char msg[1024];
   va_list argp;

   va_start(argp, format);
   g_vsnprintf(msg, 1024, format, argp);
   va_end(argp);

   window = gtk_window_new(GTK_WINDOW_DIALOG);
   gtk_window_set_wmclass(GTK_WINDOW(window), "question_dialog", "dillo");
   gtk_window_set_title(GTK_WINDOW(window), title);
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_border_width(GTK_CONTAINER(window), 10);
   gtk_signal_connect_object(GTK_OBJECT(window), "delete_event",
                             (GtkSignalFunc)gtk_widget_destroy, (void*)window);

   /* accel_group for the message window */
   accel_group = gtk_accel_group_new();
   gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

   box = gtk_vbox_new(FALSE, 5);
   frame = gtk_frame_new(NULL);
   gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
   label = gtk_label_new(msg);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_FILL);
   gtk_misc_set_padding(GTK_MISC(label), 20, 20);
   gtk_container_add(GTK_CONTAINER(frame), label);
   gtk_widget_show(label);
   gtk_widget_show(frame);
   gtk_box_pack_start(GTK_BOX(box), frame, TRUE, TRUE, 0);

   button = gtk_button_new_with_label("Close");
   gtk_signal_connect_object(GTK_OBJECT(button), "clicked",
                             (GtkSignalFunc)gtk_widget_destroy, (void*)window);
   gtk_widget_add_accelerator(button, "clicked",
                              accel_group, GDK_Escape, 0, GTK_ACCEL_LOCKED);
   gtk_widget_show(button);
   gtk_box_pack_start(GTK_BOX(box), button, FALSE, TRUE, 0);
   gtk_widget_show(button);
   gtk_container_add(GTK_CONTAINER(window), box);

   gtk_widget_show(box);
   gtk_widget_grab_focus(button);
   gtk_widget_show(window);
}

/*
 * A general purpose window for long text display.
 */
void a_Interface_text_window (GtkWidget **text_widget,
                              gchar *title, gchar *wm_class,
                              gchar *buf, gint buf_size,
                              gint xsize_max, gint ysize_max)
{
   gint xsize, ysize;
   GtkWidget *window, *box1, *button, *scrolled_window, *text;

   if (*text_widget)
      gtk_widget_destroy (*text_widget);

   /* -RL :: This code is adapted from testgtk. */
   if ( !*text_widget ) {
      window = *text_widget = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      gtk_window_set_wmclass(GTK_WINDOW(window), wm_class, "Dillo");
      gtk_widget_set_name (window, "text window");
      xsize = (prefs.width < xsize_max) ? prefs.width : xsize_max;
      ysize = (prefs.height < ysize_max) ? prefs.height : ysize_max;
      gtk_widget_set_usize (window, xsize, ysize);
      gtk_window_set_policy (GTK_WINDOW(window), TRUE, TRUE, FALSE);

      gtk_signal_connect (GTK_OBJECT (window), "destroy",
                          GTK_SIGNAL_FUNC(gtk_widget_destroyed),
                          text_widget);

      gtk_window_set_title (GTK_WINDOW (window), title);
      gtk_container_border_width (GTK_CONTAINER (window), 0);

      box1 = gtk_vbox_new (FALSE, 0);
      gtk_container_add (GTK_CONTAINER (window), box1);
      gtk_widget_show (box1);

      scrolled_window = gtk_scrolled_window_new (NULL, NULL);
      gtk_box_pack_start (GTK_BOX (box1), scrolled_window, TRUE, TRUE, 0);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                      GTK_POLICY_NEVER,
                                      GTK_POLICY_ALWAYS);
      gtk_widget_show (scrolled_window);

      text = gtk_text_new (NULL, NULL);
      gtk_text_set_editable (GTK_TEXT (text), FALSE);
      gtk_container_add (GTK_CONTAINER (scrolled_window), text);
      gtk_widget_show (text);

      gtk_text_freeze (GTK_TEXT (text));
      gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL, buf, buf_size);
      gtk_text_thaw (GTK_TEXT (text));

      button = gtk_button_new_with_label ("close");
      gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
                                 GTK_SIGNAL_FUNC(gtk_widget_destroy),
                                 GTK_OBJECT (window));
      gtk_box_pack_start (GTK_BOX (box1), button, FALSE, FALSE, 0);
      GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
      gtk_widget_grab_default (button);
      gtk_widget_show (button);
   }

   if (!GTK_WIDGET_VISIBLE (*text_widget))
      gtk_widget_show (*text_widget);
}

/*
 * Places win1 in a way that it does not, or as less as possible, cover win2.
 */
void a_Interface_set_nice_window_pos(GtkWidget *win1, GtkWidget *win2)
{
   gint w1, h1, x2, y2, w2, h2, sw, sh, sr, sl, sb, st, max;

   gtk_widget_realize(win1);
   gdk_window_get_geometry(win1->window, NULL, NULL, &w1, &h1, NULL);
   gdk_window_get_origin(win2->window, &x2, &y2);
   gdk_window_get_geometry(win2->window, NULL, NULL, &w2, &h2, NULL);
   sw = gdk_screen_width();
   sh = gdk_screen_height();

   /* space (excluding win1 space) at right, left, bottom and top */
   sr = sw - (x2 + w2 + w1);
   sl = x2 - w1;
   sb = sh - (y2 + h2 + h1);
   st = y2 - h1;

   /* First, we test, whether win1 can be placed so that it does not
    * covor win2. */
   if (sr >= 0)
      gtk_widget_set_uposition(win1, x2 + w2, (sh - h1) / 2);
   else if (sl >= 0)
      gtk_widget_set_uposition(win1, x2 - w1, (sh - h1) / 2);
   else if (sb >= 0)
      gtk_widget_set_uposition(win1, (sh - h1) / 2, y2 + h2);
   else if (st >= 0)
      gtk_widget_set_uposition(win1, (sh - h1) / 2, y2 - h1);
   else {
      /* Second, we search for the position where the covered space
       * is (more or less) minimized. */
      max = MAX(MAX(sr, sl), MAX(sb, st));
      if (sr == max)
         gtk_widget_set_uposition(win1, sw - w1, (sh - h1) / 2);
      else if (sl == max)
         gtk_widget_set_uposition(win1, 0, (sh - h1) / 2);
      else if (sb == max)
         gtk_widget_set_uposition(win1, (sh - h1) / 2, sh - h1);
      else
         gtk_widget_set_uposition(win1, (sh - h1) / 2, 0);
   }
}
