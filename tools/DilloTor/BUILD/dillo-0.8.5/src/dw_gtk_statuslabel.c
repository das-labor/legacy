/*
 * GTK - The GIMP Toolkit
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <string.h>
#include <gtk/gtklabel.h>

#include "msg.h"
#include "dw_gtk_statuslabel.h"


static void Dw_gtk_statuslabel_class_init(GtkLabelClass * klass);
static void Dw_gtk_statuslabel_size_request(GtkWidget * widget,
                                            GtkRequisition * requisition);
static gint Dw_gtk_statuslabel_expose(GtkWidget * widget,
                                      GdkEventExpose * event);


static GtkLabelClass *parent_class = NULL;

/* todo: provide a Dw_gtk_statuslabel_set function that doesn't do a
 * check_resize. */

/*
 * ?
 */
guint Dw_gtk_statuslabel_get_type()
{
   static guint statuslabel_type = 0;

   if (!statuslabel_type) {
      GtkTypeInfo statuslabel_info =
      {
         "GtkStatusLabel",
         sizeof(GtkStatusLabel),
         sizeof(GtkStatusLabelClass),
         (GtkClassInitFunc) Dw_gtk_statuslabel_class_init,
         (GtkObjectInitFunc) NULL,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      statuslabel_type = gtk_type_unique(gtk_label_get_type(),
                                         &statuslabel_info);
   }
   return statuslabel_type;
}

/*
 * ?
 */
static void Dw_gtk_statuslabel_class_init(GtkLabelClass * class)
{
   GtkWidgetClass *widget_class;

   widget_class = (GtkWidgetClass *) class;

   parent_class = gtk_type_class(gtk_label_get_type());

   widget_class->size_request = Dw_gtk_statuslabel_size_request;
   widget_class->expose_event = Dw_gtk_statuslabel_expose;
}

/*
 * ?
 */
GtkWidget *a_Dw_gtk_statuslabel_new(char *str)
{
   GtkLabel *label;

   g_return_val_if_fail(str != NULL, NULL);

   label = gtk_type_new(Dw_gtk_statuslabel_get_type());

   gtk_label_set(label, str);

   return GTK_WIDGET(label);
}

/*
 * ?
 */
static void Dw_gtk_statuslabel_size_request(GtkWidget * widget,
                                            GtkRequisition * requisition)
{
   GtkLabel *label;

   g_return_if_fail(widget != NULL);
   g_return_if_fail(GTK_IS_LABEL(widget));
   g_return_if_fail(requisition != NULL);

   label = GTK_LABEL(widget);

   requisition->width = (100 +
                         label->misc.xpad * 2);
   requisition->height = (GTK_WIDGET(label)->style->font->ascent +
                          GTK_WIDGET(label)->style->font->descent +
                          label->misc.ypad * 2 + 2);
}

/*
 * ?
 */
static void Dw_gtk_statuslabel_expose_string(GtkWidget * widget,
                                             gint state,
                                             char *string,
                                             gint width)
{
   gint x, y;
   GtkMisc *misc;

   misc = GTK_MISC(widget);
   x = (widget->allocation.x * (1.0 - misc->xalign) +
        (widget->allocation.x + widget->allocation.width - (width -
                                                       misc->xpad * 2)) *
        misc->xalign) + 0.5;
   y = (widget->allocation.y * (1.0 - misc->yalign) +
        (widget->allocation.y + widget->allocation.height -
         (widget->requisition.height - misc->ypad * 2)) * misc->yalign +
        widget->style->font->ascent) + 1.5;

   if (state == GTK_STATE_INSENSITIVE)
      gdk_draw_string(widget->window,
                      widget->style->font,
                      widget->style->white_gc,
                      x + 1, y + 1, string);

   /*
    * gdk_draw_rectangle (widget->window,
    * widget->style->bg_gc[GTK_STATE_SELECTED], FALSE,
    * widget->allocation.x, widget->allocation.y,
    * widget->allocation.width - 1, widget->allocation.height - 1);
    */

   gdk_draw_string(widget->window,
                   widget->style->font,
                   widget->style->fg_gc[state],
                   x, y, string);
}

/*
 * ?
 */
static gint Dw_gtk_statuslabel_expose(GtkWidget * widget,
                                      GdkEventExpose * event)
{
   GtkLabel *label;
   GtkMisc *misc;
   gint state;
   gint width;
   char *scratch_string;
   gint len, num_elide, num_prefix;

   g_return_val_if_fail(widget != NULL, FALSE);
   g_return_val_if_fail(GTK_IS_LABEL(widget), FALSE);
   g_return_val_if_fail(event != NULL, FALSE);

   if (GTK_WIDGET_VISIBLE(widget) && GTK_WIDGET_MAPPED(widget)) {
      label = GTK_LABEL(widget);
      misc = GTK_MISC(widget);

      state = widget->state;
      if (!GTK_WIDGET_IS_SENSITIVE(widget))
         state = GTK_STATE_INSENSITIVE;

      width = gdk_string_width(widget->style->font,
                               label->label) +
          misc->xpad * 2;
      /* If the label fits in the space allocated, we draw it. If not,
       * we (somehow) truncate. */
      if ((widget->allocation.width >= width) &&
          (widget->allocation.height >= widget->requisition.height)) {
         Dw_gtk_statuslabel_expose_string(widget, state, label->label, width);
      } else if (widget->allocation.height >= widget->requisition.height) {
         /* enough height, but not enough width - truncate */
         /* I know this algorithm is a bit inefficient, but I seriously
          * doubt it matters. */
         len = strlen(label->label);
         scratch_string = g_new(char, len + 3);

         scratch_string[0] = '\0';
         for (num_elide = 1; num_elide < len; num_elide++) {
            num_prefix = (len - num_elide) / 2;
            memcpy(scratch_string, label->label, num_prefix);
            scratch_string[num_prefix] = '.';
            scratch_string[num_prefix + 1] = '.';
            scratch_string[num_prefix + 2] = '.';
            memcpy(scratch_string + num_prefix + 3,
                   label->label + num_elide + num_prefix,
                   len + 1 - (num_elide + num_prefix));
            width = gdk_string_width(widget->style->font, scratch_string) +
                misc->xpad * 2;
            if (widget->allocation.width >= width)
               break;
         }
         Dw_gtk_statuslabel_expose_string(widget, state, scratch_string,width);
         g_free(scratch_string);
      } else {
         _MSG("Dw_gtk_statuslabel_expose:"
              " allocation too small: %d %d ( %d %d )\n",
               widget->allocation.width, widget->allocation.height,
               widget->requisition.width, widget->requisition.height);
      }
   }
   return TRUE;
}
