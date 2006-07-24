#ifndef __DW_H__
#define __DW_H__

#include <gdk/gdktypes.h>

typedef struct _DwRectangle             DwRectangle;

struct _DwRectangle
{
   gint32 x;
   gint32 y;
   gint32 width;
   gint32 height;
};

void     a_Dw_init                (void);
void     a_Dw_freeall             (void);

gboolean p_Dw_rectangle_intersect (DwRectangle *src1,
                                   DwRectangle *src2,
                                   DwRectangle *dest);
gboolean p_Dw_rectangle_is_subset (DwRectangle *rect1,
                                   DwRectangle *rect2);

/* Needed at several points. */
extern GdkCursor *Dw_cursor_hand;

#endif /* __DW_H__ */
