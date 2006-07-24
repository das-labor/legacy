#ifndef __DW_GTK_SCROLLED_FRAME_H__
#define __DW_GTK_SCROLLED_FRAME_H__

#include <gdk/gdk.h>
#include <gtk/gtkbin.h>
#include <gtk/gtkadjustment.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTK_TYPE_DW_SCROLLED_FRAME        (a_Dw_gtk_scrolled_frame_get_type ())
#define GTK_DW_SCROLLED_FRAME(obj)         (GTK_CHECK_CAST (obj, \
                                              GTK_TYPE_DW_SCROLLED_FRAME, \
                                              GtkDwScrolledFrame))
#define GTK_DW_SCROLLED_FRAME_CLASS(klass) (GTK_CHECK_CLASS_CAST (klass, \
                                              GTK_DW_TYPE_SCROLLED_FRAME, \
                                              GtkDwScrolledFrameClass))
#define GTK_DW_IS_SCROLLED_FRAME(obj)      GTK_CHECK_TYPE (obj, \
                                              GTK_DW_TYPE_SCROLLED_FRAME)

typedef struct _GtkDwScrolledFrame      GtkDwScrolledFrame;
typedef struct _GtkDwScrolledFrameClass GtkDwScrolledFrameClass;


struct _GtkDwScrolledFrame
{
   GtkBin bin;

   GtkAdjustment *hadjustment;
   GtkAdjustment *vadjustment;

   /* used for button 2 dragging */
   gboolean button2_pressed;       /* TRUE if button 2 has been pressed in
                                      the GtkDwScrolledFrame */
   gfloat start_lmx, start_lmy;    /* the last mouse position */

   /* used for fast unblocking moving */
   gfloat moveto_x, moveto_y;      /* the position the idle function should
                                      move to */
   gboolean move_idle_id;

   GdkCursor *drag_cursor;
};


struct _GtkDwScrolledFrameClass
{
   GtkBinClass parent_class;

   void (*set_scroll_adjustments) (GtkDwScrolledFrame *scrolled_frame,
                                   GtkAdjustment  *hadjustment,
                                   GtkAdjustment  *vadjustment);
   void (*user_hchanged)          (GtkDwScrolledFrame *scrolled_frame);
   void (*user_vchanged)          (GtkDwScrolledFrame *scrolled_frame);
};


GtkType    a_Dw_gtk_scrolled_frame_get_type      (void);
GtkWidget* a_Dw_gtk_scrolled_frame_new           (GtkAdjustment *hadjustment,
                                                  GtkAdjustment *vadjustment);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DW_GTK_SCROLLED_FRAME_H__ */
