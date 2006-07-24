#ifndef __MENU_H__
#define __MENU_H__

#include <gtk/gtk.h>

GtkWidget *a_Menu_mainbar_new (BrowserWindow *bw, gint tiny);
GtkWidget *a_Menu_popup_op_new (BrowserWindow *bw);
GtkWidget *a_Menu_popup_ol_new (BrowserWindow *bw);
GtkWidget *a_Menu_popup_oi_new (BrowserWindow *bw);
GtkWidget *a_Menu_popup_ob_new (BrowserWindow *bw);
GtkWidget *a_Menu_popup_history_new(BrowserWindow *bw, gint direction);
void a_Menu_popup_ol_show_oi(BrowserWindow *bw, gboolean show);
void a_Menu_popup_set_url(BrowserWindow *bw, const DilloUrl *url);
void a_Menu_popup_set_url2(BrowserWindow *bw, const DilloUrl *url);
void a_Menu_popup_clear_url2(GtkWidget *menu_popup);

DilloUrl *a_Menu_popup_get_url(BrowserWindow *bw);

void a_Menu_pagemarks_new (BrowserWindow *bw);
void a_Menu_pagemarks_destroy (BrowserWindow *bw);
void a_Menu_pagemarks_add(BrowserWindow *bw, void *page, void *style,
                          gint level);
void a_Menu_pagemarks_set_text(BrowserWindow *bw, const char *str);

#endif /* MENU_H */
