#ifndef __DW_WIDGET_H__
#define __DW_WIDGET_H__

#include <gtk/gtkobject.h>
#include <gtk/gtkwidget.h>
#include <gdk/gdktypes.h>

#include "dw_style.h"
#include "dw.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DW_TYPE_WIDGET          (a_Dw_widget_get_type ())
#define DW_WIDGET(obj)          GTK_CHECK_CAST (obj, DW_TYPE_WIDGET, DwWidget)
#define DW_WIDGET_CLASS(klass)  GTK_CHECK_CLASS_CAST (klass, DW_TYPE_WIDGET, \
                                   DwWidgetClass)
#define DW_IS_WIDGET(obj)       GTK_CHECK_TYPE (obj, DW_TYPE_WIDGET)


#define DW_WIDGET_SET_FLAGS(wid, flag)    (DW_WIDGET(wid)->flags |= (flag))
#define DW_WIDGET_UNSET_FLAGS(wid, flag)  (DW_WIDGET(wid)->flags &= ~(flag))

#define DW_WIDGET_NEEDS_RESIZE(wid)      (DW_WIDGET(wid)->flags & \
                                          DW_NEEDS_RESIZE)
#define DW_WIDGET_NEEDS_ALLOCATE(wid)    (DW_WIDGET(wid)->flags & \
                                          DW_NEEDS_ALLOCATE)
#define DW_WIDGET_EXTREMES_CHANGED(wid)  (DW_WIDGET(wid)->flags & \
                                          DW_EXTREMES_CHANGED)

#define DW_WIDGET_USES_HINTS(wid)        (DW_WIDGET(wid)->flags & \
                                          DW_USES_HINTS)
#define DW_WIDGET_HAS_CONTENT(wid)       (DW_WIDGET(wid)->flags & \
                                          DW_HAS_CONTENT)

#define DW_WIDGET_HEIGHT(wid)         ((wid)->allocation.ascent + \
                                       (wid)->allocation.descent)
#define DW_WIDGET_CONTENT_HEIGHT(wid) \
   ((wid)->allocation.ascent + (wid)->allocation.descent - \
    p_Dw_style_box_diff_height((wid)->style))

#define DW_WIDGET_CONTENT_WIDTH(wid)  ((wid)->allocation.width - \
                                       p_Dw_style_box_diff_width((wid)->style))

typedef enum {
   DW_NEEDS_RESIZE     = 1 << 0,
   DW_NEEDS_ALLOCATE   = 1 << 1,
   DW_EXTREMES_CHANGED = 1 << 2,
   DW_USES_HINTS       = 1 << 3,
   DW_HAS_CONTENT      = 1 << 4,
} DwWidgetFlags;

typedef enum
{
   DW_HPOS_LEFT,
   DW_HPOS_CENTER,
   DW_HPOS_RIGHT,
   DW_HPOS_INTO_VIEW, /* scroll only, until the content in question comes
                       * into view */
   DW_HPOS_NO_CHANGE
} DwHPosition;

typedef enum
{
   DW_VPOS_TOP,
   DW_VPOS_CENTER,
   DW_VPOS_BOTTOM,
   DW_VPOS_INTO_VIEW, /* scroll only, until the content in question comes
                       * into view */
   DW_VPOS_NO_CHANGE
} DwVPosition;

/* content types for iterator data */
typedef enum
{
   DW_CONTENT_START  = 1 << 0,
   DW_CONTENT_END    = 1 << 1,
   DW_CONTENT_TEXT   = 1 << 2,
   DW_CONTENT_WIDGET = 1 << 3,
   DW_CONTENT_ANCHOR = 1 << 4,
   DW_CONTENT_BREAK  = 1 << 5,
   DW_CONTENT_ALL    = 0xff
} DwContentType;

/* Different "layers" may be highlighted in a widget. */
typedef enum
{
   DW_HIGHLIGHT_SELECTION,
   DW_HIGHLIGHT_FINDTEXT,
   DW_HIGHLIGHT_NUM_LAYERS
} DwHighlightLayer;

#define DW_WIDGET_WINDOW(widget) \
   ((widget)->clip_pixmap ? (widget)->clip_pixmap : \
      ((GtkDwViewport*)(widget)->viewport)->back_pixmap)

typedef struct _DwAllocation            DwAllocation;
typedef struct _DwRequisition           DwRequisition;
typedef struct _DwExtremes              DwExtremes;
typedef struct _DwRectPosition          DwRectPosition;
typedef struct _DwContent               DwContent;
typedef struct _DwIterator              DwIterator;
typedef struct _DwIteratorInt           DwIteratorInt;
typedef struct _DwIteratorText          DwIteratorText;

typedef struct _DwWidget                DwWidget;
typedef struct _DwWidgetClass           DwWidgetClass;

#define DW_PAINT_DEFAULT_BGND   0xd6d6c0


struct _DwAllocation
{
   gint32 x;
   gint32 y;
   gint32 width;
   gint32 ascent;
   gint32 descent;
};


struct _DwRequisition
{
   gint32 width;
   gint32 ascent;
   gint32 descent;
};


struct _DwExtremes
{
   gint32 min_width;
   gint32 max_width;
};


struct _DwRectPosition
{
   gint32 x, y, width, height;
   DwHPosition hpos;
   DwVPosition vpos;
};


struct _DwContent
{
   DwContentType type;
   gboolean space;
   union {
      char *text;
      DwWidget *widget;
      char *anchor;
      gint break_space;
   } data;
};

struct _DwIterator
{
   DwWidget *widget;
   gint mask;

   /* the current data, after first call of next */
   DwContent content;

   /* For simplicity, static stuff is put into the structure. */

   /*
    * Move iterator forward and store content it. Returns TRUE on
    * success.
    */
   gboolean    (*next)           (DwIterator*);

   /*
    * Move iterator backward and store content it. Returns TRUE on
    * success.
    */
   gboolean    (*prev)           (DwIterator*);

   /*
    * Highlight a part of the current content. Unhighlight the current
    * content by passing -1 as start. For text, start and end define the
    * characters, otherwise, the region is defined as [0, 1], i.e. for
    * highlighting a whole DwContent, pass 0 and >= 1.
    */
   void        (*highlight)      (DwIterator*,
                                  gint start,
                                  gint end,
                                  DwHighlightLayer layer);

   /*
    * Return the region, which a part of the item, the iterator points on,
    * allocates. The parameters start and end have the same meaning as in
    * DwIterator::highlight().
    */
   void        (*get_allocation) (DwIterator*,
                                  gint start,
                                  gint end,
                                  DwAllocation*);
   /*
    * Create an exact copy of the iterator, which then can be used
    * independantly of the original one.
    */
   DwIterator* (*clone)           (DwIterator*);

   /*
    * Return a value < 0, if first iterator is smaller, > 0, if it is
    * greater, or 0 for equal iterators. The implementation may assume
    * that both iterators belong to the same widget.
    */
   gint        (*compare)         (DwIterator*,
                                   DwIterator*);

   /*
    * Free memory of iterator.
    */
   void        (*free)            (DwIterator*);
};

/* This iterator type is quite commonly used. */
struct _DwIteratorInt
{
   DwIterator it;
   int pos;
};

struct _DwIteratorText
{
   DwIterator it;
   gchar *text;
};

struct _DwWidget
{
   GtkObject object;

   /* the parent widget, NULL for top-level widgets */
   DwWidget *parent;

   /* This value is defined by the parent widget, and used for incremential
    * resizing. See Dw.txt for an explanation. */
   gint parent_ref;

   /* the viewport in which the widget is shown */
   GtkWidget *viewport;

   /* see definition at the beginning */
   DwWidgetFlags flags;

   /* the current allocation: size and position, always relative to the
    * scrolled area! */
   DwAllocation allocation;

   /* a_Dw_widget_size_request stores the result of the last call of
    * Dw_xxx_size_request here, don't read this directly, but call
    * a_Dw_widget_size_request. */
   DwRequisition requisition;

   /* analogue to requisition */
   DwExtremes extremes;

   GdkCursor *cursor; /* todo: move this to style */
   DwStyle *style;

   /* See p_Dw_widget_set_bg_color(). */
   DwStyleColor *bg_color;

   /* See a_Dw_widget_set_button_sensitive(). */
   gboolean button_sensitive;
   gboolean button_sensitive_set;

   /* A temporary pixmap used for clipping. See p_Dw_widget_will_clip(). */
   GdkPixmap *clip_pixmap;
};


struct _DwWidgetClass
{
   GtkObjectClass parent_class;

   void (*size_request)         (DwWidget *widget,
                                 DwRequisition *requisition);
   void (*get_extremes)         (DwWidget *widget,
                                 DwExtremes *extremes);
   void (*size_allocate)        (DwWidget *widget,
                                 DwAllocation *allocation);
   void (*mark_size_change)     (DwWidget *widget,
                                 gint ref);
   void (*mark_extremes_change) (DwWidget *widget,
                                 gint ref);
   void (*set_width)            (DwWidget *widget,
                                 gint32 width);
   void (*set_ascent)           (DwWidget *widget,
                                 gint32 ascent);
   void (*set_descent)          (DwWidget *widget,
                                 gint32 descent);
   void (*draw)                 (DwWidget *widget,
                                 DwRectangle *area,
                                 GdkEventExpose *event);

   gboolean (*button_press_event)   (DwWidget *widget,
                                     gint32 x,
                                     gint32 y,
                                     GdkEventButton *event);
   gboolean (*button_release_event) (DwWidget *widget,
                                     gint32 x,
                                     gint32 y,
                                     GdkEventButton *event);
   gboolean (*motion_notify_event)  (DwWidget *widget,
                                     gint32 x,
                                     gint32 y,
                                     GdkEventMotion *event);
   gboolean (*enter_notify_event)   (DwWidget *widget,
                                     DwWidget *last_widget,
                                     GdkEventMotion *event);
   gboolean (*leave_notify_event)   (DwWidget *widget,
                                     DwWidget *next_widget,
                                     GdkEventMotion *event);

   DwIterator* (*iterator)       (DwWidget*,
                                  gint32 mask,
                                  gboolean at_end);
};


GtkType a_Dw_widget_get_type        (void);

void    p_Dw_widget_size_request    (DwWidget *widget,
                                     DwRequisition *requisition);
void    p_Dw_widget_get_extremes    (DwWidget *widget,
                                     DwExtremes *extremes);
void    p_Dw_widget_size_allocate   (DwWidget *widget,
                                     DwAllocation *allocation);
void    p_Dw_widget_set_width       (DwWidget *widget,
                                     gint32 width);
void    p_Dw_widget_set_ascent      (DwWidget *widget,
                                     gint32 ascent);
void    p_Dw_widget_set_descent     (DwWidget *widget,
                                     gint32 descent);
void    p_Dw_widget_draw            (DwWidget *widget,
                                     DwRectangle *area,
                                     GdkEventExpose *event);

void    a_Dw_widget_set_style       (DwWidget *widget,
                                     DwStyle *style);
void    a_Dw_widget_set_cursor      (DwWidget *widget,
                                     GdkCursor *cursor);

void    a_Dw_widget_set_button_sensitive (DwWidget *widget,
                                          gboolean button_sensitive);

DwWidget *a_Dw_widget_get_toplevel  (DwWidget *widget);

void    a_Dw_widget_scroll_to       (DwWidget *widget,
                                     DwHPosition hpos,
                                     DwVPosition vpos,
                                     gint32 x,
                                     gint32 y,
                                     gint32 width,
                                     gint32 height);

/* iterators */
DwIterator* a_Dw_widget_iterator   (DwWidget *widget,
                                    gint mask,
                                    gboolean at_end);
#define     a_Dw_iterator_next(it) ((it) ? (it)->next(it) : FALSE)
#define     a_Dw_iterator_prev(it) ((it) ? (it)->prev(it) : FALSE)
#define     a_Dw_iterator_highlight(it, s, e, l) ((it) ? \
                                      (it)->highlight(it, s, e, l) : (void)0)
#define     a_Dw_iterator_get_allocation(it, s, e, a) \
                                           ((it) ? \
                                            (it1)->get_allocation(it, s, e, a)\
                                            : (void)0)
#define     a_Dw_iterator_clone(it) ((it) ? (it)->clone(it) : NULL)
#define     a_Dw_iterator_compare(it1, it2) ((it1)->compare(it1, it2))
#define     a_Dw_iterator_free(it)  ((it) ? (it)->free(it) : (void)0)

/* for convenience */
#define     a_Dw_iterator_unhighlight(it, l) ((it) ? \
                                    (it)->highlight(it, -1, -1, l) : FALSE)
void        a_Dw_iterator_scroll_to  (DwIterator *it1,
                                      DwIterator *it2,
                                      gint start,
                                      gint end,
                                      DwHPosition hpos,
                                      DwVPosition vpos);

DwIterator* p_Dw_iterator_clone_std          (DwIterator *it);
void        p_Dw_iterator_free_std           (DwIterator *it);
void        p_Dw_iterator_highlight_std      (DwIterator *it,
                                              gint from,
                                              gint start,
                                              DwHighlightLayer layer);

void        p_Dw_iterator_get_allocation_std_only_widgets (DwIterator *it,
                                                           gint start,
                                                           gint end,
                                                           DwAllocation
                                                           *allocation);

DwIterator* p_Dw_iterator_clone_std_int      (DwIterator *it);
gint        p_Dw_iterator_compare_std_int    (DwIterator *it1,
                                              DwIterator *it2);

/* for debugging */
gchar*      a_Dw_content_text             (DwContent *content);
gchar*      a_Dw_content_html             (DwContent *content);
gchar*      a_Dw_iterator_text            (DwIterator *it);
void        a_Dw_widget_print_tree        (DwWidget *widget);

DwIterator* p_Dw_widget_text_iterator         (DwWidget* widget,
                                               gint32 mask,
                                               gboolean at_end,
                                               gchar *text);
gboolean    p_Dw_iterator_text_next           (DwIterator *it);
gboolean    p_Dw_iterator_text_prev           (DwIterator *it);
DwIterator* p_Dw_iterator_text_clone          (DwIterator *it);
gint        p_Dw_iterator_text_compare        (DwIterator *it1,
                                               DwIterator *it2);
void        p_Dw_iterator_text_get_allocation (DwIterator *it,
                                               gint start,
                                               gint end,
                                               DwAllocation *allocation);

/* Only for Dw module */
gint    p_Dw_widget_intersect         (DwWidget *widget,
                                       DwRectangle *area,
                                       DwRectangle *intersection);
void    p_Dw_widget_set_parent        (DwWidget *widget,
                                       DwWidget *parent);

gint32  p_Dw_widget_x_viewport_to_world (DwWidget *widget,
                                         gint16 viewport_x);
gint32  p_Dw_widget_y_viewport_to_world (DwWidget *widget,
                                         gint16 viewport_y);
gint16  p_Dw_widget_x_world_to_viewport (DwWidget *widget,
                                         gint32 world_x);
gint16  p_Dw_widget_y_world_to_viewport (DwWidget *widget,
                                         gint32 world_y);

gint    Dw_widget_mouse_event       (DwWidget *widget,
                                     GtkWidget *viewwidget,
                                     gint32 x,
                                     gint32 y,
                                     GdkEvent *event);
void    Dw_widget_update_cursor       (DwWidget *widget);
void    p_Dw_widget_queue_draw        (DwWidget *widget);
void    p_Dw_widget_queue_draw_area   (DwWidget *widget,
                                       gint32 x,
                                       gint32 y,
                                       gint32 width,
                                       gint32 height);
void    p_Dw_widget_queue_resize      (DwWidget *widget,
                                       gint ref,
                                       gboolean extremes_changed);
void    p_Dw_widget_will_clip         (DwWidget *widget);

void    p_Dw_widget_set_bg_color      (DwWidget *widget,
                                       DwStyleColor *color);
DwStyleColor* p_Dw_widget_get_bg_color(DwWidget *widget);

/* Wrappers for Dw_style_draw_box */
void    p_Dw_widget_draw_box          (DwWidget *widget,
                                       DwStyle *style,
                                       DwRectangle *area,
                                       gint32 x,
                                       gint32 y,
                                       gint32 width,
                                       gint32 height,
                                       gboolean inverse);
void    p_Dw_widget_draw_widget_box   (DwWidget *widget,
                                       DwRectangle *area,
                                       gboolean inverse);

void    p_Dw_widget_draw_selected     (DwWidget *widget,
                                       DwRectangle *area);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_WIDGET_H__ */
