/*
 * Init and freeall, and some general functions.
 */

#include "dw.h"
#include "dw_style.h"
#include <gdk/gdk.h>
#include "debug.h"

GdkCursor *Dw_cursor_hand;

void a_Dw_init (void)
{
   DBG_OBJ_COLOR ("^Dw", "ffffe0");
   DBG_OBJ_COLOR ("^Gtk", "d0ffd0");

   a_Dw_style_init ();

   Dw_cursor_hand = gdk_cursor_new(GDK_HAND2);
}


void a_Dw_freeall (void)
{
   a_Dw_style_freeall ();

   gdk_cursor_destroy (Dw_cursor_hand);
}

/*
 * Return whether src1 and src2 intersect. If yes, return the intersection
 * rectangle in dest.
 *
 * The function has been copied from gdktrectangle.c
 */
gint p_Dw_rectangle_intersect (DwRectangle *src1,
                               DwRectangle *src2,
                               DwRectangle *dest)
{
   DwRectangle *temp;
   gint src1_x2, src1_y2;
   gint src2_x2, src2_y2;
   gint return_val;

   g_return_val_if_fail (src1 != NULL, FALSE);
   g_return_val_if_fail (src2 != NULL, FALSE);
   g_return_val_if_fail (dest != NULL, FALSE);

   return_val = FALSE;

   if (src2->x < src1->x) {
      temp = src1;
      src1 = src2;
      src2 = temp;
   }
   dest->x = src2->x;

   src1_x2 = src1->x + src1->width;
   src2_x2 = src2->x + src2->width;

   if (src2->x < src1_x2) {
      if (src1_x2 < src2_x2)
         dest->width = src1_x2 - dest->x;
      else
         dest->width = src2_x2 - dest->x;

      if (src2->y < src1->y) {
         temp = src1;
         src1 = src2;
         src2 = temp;
      }
      dest->y = src2->y;

      src1_y2 = src1->y + src1->height;
      src2_y2 = src2->y + src2->height;

      if (src2->y < src1_y2) {
         return_val = TRUE;

         if (src1_y2 < src2_y2)
            dest->height = src1_y2 - dest->y;
         else
            dest->height = src2_y2 - dest->y;

         if (dest->height == 0)
            return_val = FALSE;
         if (dest->width == 0)
            return_val = FALSE;
      }
   }

   return return_val;
}


/*
 * Return whether rect1 is a subset of rect2.
 */
gboolean p_Dw_rectangle_is_subset (DwRectangle *rect1,
                                   DwRectangle *rect2)
{
   return
      rect1->x >= rect2->x &&
      rect1->y >= rect2->y &&
      rect1->x + rect1->width <= rect2->x + rect2->width &&
      rect1->y + rect1->height <= rect2->y + rect2->height;
}
