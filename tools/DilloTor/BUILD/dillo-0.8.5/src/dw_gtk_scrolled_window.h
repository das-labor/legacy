#ifndef __DW_GTK_SCROLLED_WINDOW_H__
#define __DW_GTK_SCROLLED_WINDOW_H__

#include <gtk/gtkscrolledwindow.h>
#include "dw_widget.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GTK_TYPE_DW_SCROLLED_WINDOW     (a_Dw_gtk_scrolled_window_get_type ())
#define GTK_DW_SCROLLED_WINDOW(obj)         (GTK_CHECK_CAST (obj, \
                                               GTK_TYPE_DW_SCROLLED_WINDOW, \
                                               GtkDwScrolledWindow))
#define GTK_DW_SCROLLED_WINDOW_CLASS(klass) (GTK_CHECK_CLASS_CAST (klass, \
                                                GTK_TYPE_DW_SCROLLED_WINDOW, \
                                                GtkDwScrolledWindowClass))
#define GTK_IS_DW_SCROLLED_WINDOW(obj)      GTK_CHECK_TYPE (obj, \
                                               GTK_TYPE_DW_SCROLLED_WINDOW)


typedef struct _GtkDwScrolledWindow      GtkDwScrolledWindow;
typedef struct _GtkDwScrolledWindowClass GtkDwScrolledWindowClass;


struct _GtkDwScrolledWindow
{
   GtkScrolledWindow scrolled_window;

   GtkWidget *gadget; /* This is a widget shown in the upper right
                       * corner, and in dillo used for the "full
                       * screen off" button. */
   GtkAdjustment *vadjustment;
   gfloat old_vadjustment_value;
};


struct _GtkDwScrolledWindowClass
{
   GtkScrolledWindowClass parent_class;
};


GtkType    a_Dw_gtk_scrolled_window_get_type (void);
GtkWidget* a_Dw_gtk_scrolled_window_new (void);
void       a_Dw_gtk_scrolled_window_add_gadget (
              GtkDwScrolledWindow *scrolled, GtkWidget *gadget);

void       a_Dw_gtk_scrolled_window_set_dw (
              GtkDwScrolledWindow *scrolled, DwWidget *widget);
DwWidget*  a_Dw_gtk_scrolled_window_get_dw (GtkDwScrolledWindow *scrolled);

void       a_Dw_gtk_scrolled_window_set_anchor (
              GtkDwScrolledWindow *scrolled, const gchar *anchor);
gint       a_Dw_gtk_scrolled_window_get_scrolling_position_x (
              GtkDwScrolledWindow *scrolled);
gint       a_Dw_gtk_scrolled_window_get_scrolling_position_y (
              GtkDwScrolledWindow *scrolled);
void       a_Dw_gtk_scrolled_window_set_scrolling_position (
              GtkDwScrolledWindow *scrolled, gint32 x, gint32 y);

gboolean   a_Dw_gtk_scrolled_window_search (
              GtkDwScrolledWindow *scrolled, gchar *string,
              gboolean case_sens);
void       a_Dw_gtk_scrolled_window_reset_search (
              GtkDwScrolledWindow *scrolled);

DwWidget*  a_Dw_gtk_scrolled_window_widget_at_viewport_point (
              GtkDwScrolledWindow *scrolled,
              gint32 vx,
              gint32 vy);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_GTK_SCROLLED_WINDOW_H__ */
