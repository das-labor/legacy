#ifndef __DW_GTK_VIEWPORT_H__
#define __DW_GTK_VIEWPORT_H__

#include <gtk/gtklayout.h>
#include "dw_widget.h"
#include "findtext.h"
#include "selection.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_TYPE_DW_VIEWPORT            (a_Dw_gtk_viewport_get_type ())
#define GTK_DW_VIEWPORT(obj)            (GTK_CHECK_CAST (obj, \
                                           GTK_TYPE_DW_VIEWPORT,GtkDwViewport))
#define GTK_DW_VIEWPORT_CLASS(klass)    (GTK_CHECK_CLASS_CAST (klass, \
                                          GTK_TYPE_DW_VIEWPORT, \
                                          GtkDwViewportClass)
#define GTK_IS_DW_VIEWPORT(obj)         GTK_CHECK_TYPE (obj, \
                                           GTK_TYPE_DW_VIEWPORT)

typedef enum {
   DW_GTK_VIEWPORT_DRAW,
   DW_GTK_VIEWPORT_RESIZE
} DwGtkViewportDrawResizeAction;

typedef struct _GtkDwViewport       GtkDwViewport;
typedef struct _GtkDwViewportClass  GtkDwViewportClass;


struct _GtkDwViewport
{
   GtkLayout layout;

   GdkPixmap *back_pixmap;              /* backing pixmap for buffering */
   gint back_width, back_height;
   gint depth;

   DwWidget *child;
   DwWidget *last_entered;
   gboolean hscrollbar_used, vscrollbar_used, calc_size_blocked;

   /* updated by Dw_gtk_viewport_motion_notify */
   gdouble mouse_x, mouse_y;

   gchar *anchor;
   DwRectPosition anchor_pos;
   gint anchor_idle_id;

   FindtextState *findtext_state;
   Selection *selection;

   /* Anchors of the widget tree.
    * Key: gchar*, has to be stored elsewhere
    * Value: an instance of GtkDwViewportAnchor (in .c file) */
   GHashTable *anchors_table;

   /* Queue of draw and resize requests. */
   gint draw_resize_idle_id;
   DwGtkViewportDrawResizeAction draw_resize_action;

   /* What has to be redrawn. DwRectangle's are in world coordinates. */
   DwRectangle *draw_areas;
   gint num_draw_areas;
   gint num_draw_areas_max; /* number allocated */
};


struct _GtkDwViewportClass
{
   GtkLayoutClass parent_class;
};


GtkType        a_Dw_gtk_viewport_get_type        (void);
GtkWidget*     a_Dw_gtk_viewport_new             (GtkAdjustment *hadjustment,
                                                  GtkAdjustment *vadjustment);
void           a_Dw_gtk_viewport_add_dw          (GtkDwViewport *viewport,
                                                  DwWidget *widget);

void           a_Dw_gtk_viewport_set_anchor      (GtkDwViewport *viewport,
                                                  const gchar *anchor);
void           a_Dw_gtk_viewport_set_scrolling_position (GtkDwViewport
                                                         *viewport,
                                                         gint32 x,
                                                         gint32 y);

DwWidget*      a_Dw_gtk_viewport_widget_at_viewport_point (GtkDwViewport
                                                           *viewport,
                                                           gint32 vx,
                                                           gint32 vy);

gchar*         p_Dw_gtk_viewport_add_anchor      (DwWidget *widget,
                                                  const gchar* name,
                                                  gint32 y);
void           p_Dw_gtk_viewport_change_anchor   (DwWidget *widget,
                                                  gchar *name,
                                                  gint32 y);
void           p_Dw_gtk_viewport_remove_anchor   (DwWidget *widget,
                                                  gchar *name);

void           Dw_gtk_viewport_remove_dw         (GtkDwViewport *viewport);
void           Dw_gtk_viewport_calc_size         (GtkDwViewport *viewport);

DwWidget*      Dw_gtk_viewport_widget_at_point (GtkDwViewport *viewport,
                                                gint32 x,
                                                gint32 y);

void           Dw_gtk_viewport_update_anchor    (GtkDwViewport *viewport);
void           Dw_gtk_viewport_scroll_to        (GtkDwViewport *viewport,
                                                 DwHPosition hpos,
                                                 DwVPosition vpos,
                                                 gint32 x,
                                                 gint32 y,
                                                 gint32 width,
                                                 gint32 height);
void           Dw_gtk_viewport_remove_anchor    (GtkDwViewport *viewport);
void           Dw_gtk_viewport_queue_draw       (GtkDwViewport *viewport,
                                                 gint32 x,
                                                 gint32 y,
                                                 gint32 width,
                                                 gint32 height);
void           Dw_gtk_viewport_queue_resize     (GtkDwViewport *viewport);

void            Dw_gtk_viewport_update_background (GtkDwViewport *viewport);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_GTK_VIEWPORT_H__ */
