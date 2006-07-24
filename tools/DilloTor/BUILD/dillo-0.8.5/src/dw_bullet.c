/*
 * File: dw_bullet.c
 *
 * Copyright (C) 1997       Raph Levien <raph@acm.org>
 * Copyright (C) 1999       Luca Rota <drake@freemail.it>
 * Copyright (C) 2001-2003  Sebastian Geerken <s.geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * Bullets are drawn 1/5 of an x-height above the baseline, and are
 * 4/5 of an x-height wide and high.
 */

#include "dw_bullet.h"
#include "dw_gtk_viewport.h"

static void        Dw_bullet_init               (DwBullet *bullet);
static void        Dw_bullet_class_init         (DwBulletClass *klass);
static void        Dw_bullet_size_request       (DwWidget *widget,
                                                 DwRequisition *requisition);
static void        Dw_bullet_draw               (DwWidget *widget,
                                                 DwRectangle *area,
                                                 GdkEventExpose *event);
static DwIterator* Dw_bullet_iterator           (DwWidget *widget,
                                                gint32 mask,
                                                gboolean at_end);
static void        Dw_bullet_iterator_highlight (DwIterator *it,
                                                 gint start,
                                                 gint end,
                                                 DwHighlightLayer layer);



GtkType a_Dw_bullet_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "DwBullet",
         sizeof (DwBullet),
         sizeof (DwBulletClass),
         (GtkClassInitFunc) Dw_bullet_class_init,
         (GtkObjectInitFunc) Dw_bullet_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (DW_TYPE_WIDGET, &info);
   }

   return type;
}


DwWidget* a_Dw_bullet_new (void)
{
   return DW_WIDGET (gtk_object_new (DW_TYPE_BULLET, NULL));
}


static void Dw_bullet_init (DwBullet *bullet)
{
   int i;
   for (i = 0; i < DW_HIGHLIGHT_NUM_LAYERS; i++)
      bullet->selected[i] = FALSE;
}


static void Dw_bullet_class_init (DwBulletClass *klass)
{
   DwWidgetClass *widget_class;

   widget_class = (DwWidgetClass*)klass;
   widget_class->size_request = Dw_bullet_size_request;
   widget_class->draw = Dw_bullet_draw;
   widget_class->iterator = Dw_bullet_iterator;
}


static void Dw_bullet_size_request (DwWidget *widget,
                                    DwRequisition *requisition)
{
   requisition->width = MAX (widget->style->font->x_height * 4 / 5, 1);
   requisition->ascent = MAX (widget->style->font->x_height, 1);
   requisition->descent = 0;
}


static void Dw_bullet_draw (DwWidget *widget,
                            DwRectangle *area,
                            GdkEventExpose *event)
{
   gint32 x0, y0, x, y;
   GdkGC *gc;
   DwStyleColor *bg_color;
   GdkWindow *window;
   gint32 l;
   int i;
   gboolean selected = FALSE;

   for (i = 0; i < DW_HIGHLIGHT_NUM_LAYERS && !selected; i++)
      selected = DW_BULLET(widget)->selected[i];

   l = MIN (widget->allocation.width, widget->allocation.ascent);
   x = x0 = p_Dw_widget_x_world_to_viewport (widget, widget->allocation.x);
   y0 = p_Dw_widget_y_world_to_viewport (widget, widget->allocation.y);
   y = y0 + widget->allocation.ascent - widget->style->font->x_height;
   gc = selected ? widget->style->color->inverse_gc :
      widget->style->color->gc;
   window = DW_WIDGET_WINDOW (widget);

   if (selected) {
      bg_color = p_Dw_widget_get_bg_color (widget);
      gdk_draw_rectangle (window, bg_color->inverse_gc,
                          TRUE, x0, y0,
                          widget->allocation.width,
                          DW_WIDGET_HEIGHT(widget));
   }

   switch (widget->style->list_style_type) {
   case DW_STYLE_LIST_STYLE_TYPE_DISC:
      gdk_draw_arc (window, gc, TRUE, x, y, l, l, 0, 360 * 64);
      break;
   case DW_STYLE_LIST_STYLE_TYPE_CIRCLE:
      gdk_draw_arc (window, gc, FALSE, x, y, l, l, 0, 360 * 64);
      break;
   case DW_STYLE_LIST_STYLE_TYPE_SQUARE:
      gdk_draw_rectangle (window, gc, FALSE, x, y, l, l);
      break;
   default:
      break;
   }
}

static DwIterator *Dw_bullet_iterator (DwWidget *widget,
                                        gint32 mask,
                                        gboolean at_end)
{
   DwIterator *it;
   gchar *text;

   switch (widget->style->list_style_type) {
   case DW_STYLE_LIST_STYLE_TYPE_DISC:
      text = "*";
      break;
   case DW_STYLE_LIST_STYLE_TYPE_CIRCLE:
      text = "o";
      break;
   case DW_STYLE_LIST_STYLE_TYPE_SQUARE:
      text = "-";
      break;
   default:
      text = "?";
      break;
   }

   it = p_Dw_widget_text_iterator (widget, mask, at_end, text);
   if (it)
     it->highlight = Dw_bullet_iterator_highlight;
   return it;
}

static void Dw_bullet_iterator_highlight (DwIterator *it,
                                          gint start,
                                          gint end,
                                          DwHighlightLayer layer)
{
   if (it->content.type == DW_CONTENT_TEXT) {
      DW_BULLET(it->widget)->selected[layer] = (start == 0 && end >= 1);
      p_Dw_widget_queue_draw (it->widget);
   }
}
