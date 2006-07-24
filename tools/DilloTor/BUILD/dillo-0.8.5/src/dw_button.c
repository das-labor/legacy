/*
 * File: dw_button.c
 *
 * Copyright (C) 2002 Sebastian Geerken <S.Geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * This widget imitates the look and behavior of GtkButton.
 *
 * NOTE: Unlike in X, we do not get release events, if the pointer has
 * been moved out of the widget with mouse button down (implicit
 * pointer grab, will perhaps be added to Dw). For this reason, the
 * behavior of DwButton is a bit differernt from GtkButton: If the
 * user presses the mouse button within the button, then leaves it
 * (with mouse button pressed), and enters it again, a GtkButton will
 * still be in the state "pressed", while DwButton resets this state
 * when the user leaves the button. This is to avoid problems in the
 * situation when the user leaves the button with mouse button down,
 * and releases it outside of the button.
 *
 * BUG (reminder): If the user releases the button, it gets somehow a
 * "leave_notify" event. This bug is in the base code, not here,
 * somehow it is assumed that the mouse pointer is out of the viewport
 * (Dw_widget_mouse_event is called with widget == NULL). The effect
 * is that after releasing the mouse button, the DwButton switches
 * from "active" to "normal", not to "highlighted", as it should.
 * Should be fixed at the next opportunity.
 */

#include "dw_button.h"
#include "dw_gtk_viewport.h"
#include "debug.h"
#include <gtk/gtk.h>

static void Dw_button_init          (DwButton *button);
static void Dw_button_class_init    (DwButtonClass *klass);

static void Dw_button_size_request      (DwWidget *widget,
                                         DwRequisition *requisition);
static void Dw_button_get_extremes      (DwWidget *widget,
                                         DwExtremes *extremes);
static void Dw_button_size_allocate     (DwWidget *widget,
                                         DwAllocation *allocation);
static void Dw_button_set_width         (DwWidget *widget,
                                         gint32 width);
static void Dw_button_set_ascent        (DwWidget *widget,
                                         gint32 ascent);
static void Dw_button_set_descent       (DwWidget *widget,
                                         gint32 descent);
static void Dw_button_draw              (DwWidget *widget,
                                         DwRectangle *area,
                                         GdkEventExpose *event);
static gboolean Dw_button_button_press      (DwWidget *widget,
                                             gint32 x,
                                             gint32 y,
                                             GdkEventButton *event);
static gboolean Dw_button_button_release    (DwWidget *widget,
                                             gint32 x,
                                             gint32 y,
                                             GdkEventButton *event);
static gboolean Dw_button_enter_notify      (DwWidget *widget,
                                             DwWidget *last_widget,
                                             GdkEventMotion *event);
static gboolean Dw_button_leave_notify      (DwWidget *widget,
                                             DwWidget *next_widget,
                                             GdkEventMotion *event);


static void Dw_button_add               (DwContainer *container,
                                         DwWidget *widget);
static void Dw_button_remove            (DwContainer *container,
                                         DwWidget *widget);
static void Dw_button_forall            (DwContainer *container,
                                         DwCallback callback,
                                         gpointer callback_data);

static DwIterator* Dw_button_iterator          (DwWidget *widget,
                                                gint mask,
                                                gboolean at_end);
static gboolean    Dw_button_iterator_next     (DwIterator *it);
static gboolean    Dw_button_iterator_prev     (DwIterator *it);
static gint        Dw_button_iterator_compare  (DwIterator *it1,
                                                DwIterator *it2);


enum
{
   CLICKED,
   CLICKED_AT,
   LAST_SIGNAL
};

static DwContainerClass *parent_class;
static guint button_signals[LAST_SIGNAL] = { 0 };


/*
 * Return the type of DwButton
 */
GtkType a_Dw_button_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "DwButton",
         sizeof (DwButton),
         sizeof (DwButtonClass),
         (GtkClassInitFunc) Dw_button_class_init,
         (GtkObjectInitFunc) Dw_button_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (DW_TYPE_CONTAINER, &info);
   }

   return type;
}


/*
 * Standard Gtk+ function.
 *
 * - "flags" is a mask for the window flags, it should be the mask the
 *   child uses (e.g. DW_USES_HINT for DwPage, 0 for DwImage). (Will
 *   hopefully be removed soon.)
 * - When passing FALSE for "relief", the button will neither have a
 *   border, nor paint a background.
 */
DwWidget* a_Dw_button_new (gint flags,
                           gboolean relief)
{
   DwButton *button;

   button = DW_BUTTON (gtk_object_new (DW_TYPE_BUTTON, NULL));
   DBG_OBJ_CREATE (button, "DwButton");
   DW_WIDGET_SET_FLAGS (button, flags);
   button->relief = relief;

   return DW_WIDGET (button);
}


/*
 * Standard Gtk+ function.
 */
static void Dw_button_init (DwButton *button)
{
   button->child = NULL;
   button->in_button = FALSE;
   button->pressed = FALSE;
   button->sensitive = FALSE;
}

/*
 * Standard Gtk+ function.
 */
static void Dw_button_class_init (DwButtonClass *klass)
{
   GtkObjectClass *object_class;
   DwWidgetClass *widget_class;
   DwContainerClass *container_class;

   object_class = (GtkObjectClass*) klass;
   widget_class = (DwWidgetClass*) klass;
   container_class = (DwContainerClass*) klass;
   parent_class = gtk_type_class (DW_TYPE_CONTAINER);

   button_signals[CLICKED] =
      gtk_signal_new ("clicked",
                      GTK_RUN_FIRST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (DwButtonClass, clicked),
                      gtk_marshal_NONE__NONE,
                      GTK_TYPE_NONE, 0);
   button_signals[CLICKED_AT] =
      gtk_signal_new ("clicked_at",
                      GTK_RUN_FIRST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (DwButtonClass, clicked_at),
                      gtk_marshal_NONE__INT_INT,
                      GTK_TYPE_NONE,
                      2, GTK_TYPE_INT, GTK_TYPE_INT);
   gtk_object_class_add_signals (object_class, button_signals, LAST_SIGNAL);

   widget_class->size_request = Dw_button_size_request;
   widget_class->get_extremes = Dw_button_get_extremes;
   widget_class->size_allocate = Dw_button_size_allocate;
   widget_class->set_width = Dw_button_set_width;
   widget_class->set_ascent = Dw_button_set_ascent;
   widget_class->set_descent = Dw_button_set_descent;
   widget_class->draw = Dw_button_draw;
   widget_class->button_press_event = Dw_button_button_press;
   widget_class->button_release_event = Dw_button_button_release;
   widget_class->enter_notify_event = Dw_button_enter_notify;
   widget_class->leave_notify_event = Dw_button_leave_notify;
   widget_class->iterator = Dw_button_iterator;

   container_class->add = Dw_button_add;
   container_class->remove = Dw_button_remove;
   container_class->forall = Dw_button_forall;

   klass->clicked = NULL;
   klass->clicked_at = NULL;
}


/*
 * Standard Dw function.
 */
static void Dw_button_size_request (DwWidget *widget,
                                    DwRequisition *requisition)
{
   DwButton *button = DW_BUTTON (widget);
   DwRequisition child_requisition;
   GtkStyleClass *styleclass = widget->viewport->style->klass;

   if (button->child) {
      p_Dw_widget_size_request (button->child, &child_requisition);
      *requisition = child_requisition;
   } else {
      requisition->width = 0;
      requisition->ascent = 0;
      requisition->descent = 0;
   }

   if (button->relief) {
      requisition->width += 2 * styleclass->xthickness;
      requisition->ascent += styleclass->ythickness;
      requisition->descent += styleclass->ythickness;
   }
}


/*
 * Standard Dw function.
 */
static void Dw_button_get_extremes (DwWidget *widget,
                                    DwExtremes *extremes)
{
   DwButton *button = DW_BUTTON (widget);
   DwExtremes child_extremes;
   GtkStyleClass *styleclass = widget->viewport->style->klass;

   if (button->child) {
      p_Dw_widget_get_extremes (button->child, &child_extremes);
      *extremes = child_extremes;
   } else {
      extremes->min_width = 0;
      extremes->max_width = 0;
   }

   if (button->relief) {
      extremes->min_width += 2 * styleclass->xthickness;
      extremes->max_width += 2 * styleclass->xthickness;
   }
}


/*
 * Standard Dw function.
 */
static void Dw_button_size_allocate (DwWidget *widget,
                                     DwAllocation *allocation)
{
   DwButton *button = DW_BUTTON (widget);
   DwAllocation child_allocation;
   GtkStyleClass *styleclass = widget->viewport->style->klass;

   if (button->child) {
      child_allocation = *allocation;

      if (button->relief) {
         child_allocation. x += styleclass->xthickness;
         child_allocation. y += styleclass->ythickness;
         child_allocation.width -= 2 * styleclass->xthickness;
         child_allocation.ascent -= styleclass->ythickness;
         child_allocation.descent -= styleclass->ythickness;
      }

      p_Dw_widget_size_allocate (button->child, &child_allocation);
   }
}


/*
 * Standard Dw function.
 */
static void Dw_button_set_width (DwWidget *widget,
                                 gint32 width)
{
   DwButton *button = DW_BUTTON (widget);

   if (button->child)
      p_Dw_widget_set_width
         (button->child,
          width - (button->relief ?
                   (2 * widget->viewport->style->klass->xthickness) : 0));
}


/*
 * Standard Dw function.
 */
static void Dw_button_set_ascent (DwWidget *widget,
                                  gint32 ascent)
{
   DwButton *button = DW_BUTTON (widget);

   if (button->child)
      p_Dw_widget_set_ascent
         (button->child,
          ascent  - (button->relief ?
                     widget->viewport->style->klass->ythickness : 0));
}


/*
 * Standard Dw function.
 */
static void Dw_button_set_descent (DwWidget *widget,
                                   gint32 descent)
{
   DwButton *button = DW_BUTTON (widget);

   if (button->child)
      p_Dw_widget_set_descent
         (button->child,
          descent - (button->relief ?
                     widget->viewport->style->klass->ythickness : 0));
}


/*
 * Standard Dw function.
 */
static void Dw_button_draw (DwWidget *widget,
                            DwRectangle *area,
                            GdkEventExpose *event)
{
   DwButton *button = DW_BUTTON (widget);
   GtkStateType state;
   GtkShadowType shadow;
   GdkRectangle gdk_area;
   DwRectangle child_area;

   if (button->relief) {
      if (button->sensitive) {
         if (button->in_button) {
            if (button->pressed) {
               state = GTK_STATE_ACTIVE;
               shadow = GTK_SHADOW_IN;
            } else {
               state = GTK_STATE_PRELIGHT;
               shadow = GTK_SHADOW_OUT;
            }
         } else {
            state = GTK_STATE_NORMAL;
            shadow = GTK_SHADOW_OUT;
         }
      } else {
         state = GTK_STATE_INSENSITIVE;
         shadow = GTK_SHADOW_OUT;
      }

      gdk_area.x =
         p_Dw_widget_x_world_to_viewport (widget,
                                          area->x + widget->allocation.x);
      gdk_area.y =
         p_Dw_widget_y_world_to_viewport (widget,
                                          area->y + widget->allocation.y);
      gdk_area.width = area->width;
      gdk_area.height = area->height;

      gtk_paint_box (widget->viewport->style, DW_WIDGET_WINDOW (widget),
                     state, shadow, &gdk_area, widget->viewport,
                     "buttondefault",
                     p_Dw_widget_x_world_to_viewport (widget,
                                                      widget->allocation.x),
                     p_Dw_widget_y_world_to_viewport (widget,
                                                      widget->allocation.y),
                     widget->allocation.width,
                     DW_WIDGET_HEIGHT(widget));
   }

   if (button->child &&
       p_Dw_widget_intersect (button->child, area, &child_area))
      p_Dw_widget_draw (button->child, &child_area, event);
}


/*
 * Standard Dw function.
 */
static gboolean Dw_button_button_press (DwWidget *widget,
                                        gint32 x,
                                        gint32 y,
                                        GdkEventButton *event)
{
   DwButton *button = DW_BUTTON (widget);

   /* assert it was the left mouse button */
   if (event->button != 1)
      return FALSE;

   button->pressed = TRUE;
   if (button->relief)
      p_Dw_widget_queue_draw (widget);
   return TRUE;
}


/*
 * Standard Dw function.
 */
static gboolean Dw_button_button_release (DwWidget *widget,
                                          gint32 x,
                                          gint32 y,
                                          GdkEventButton *event)
{
   DwButton *button = DW_BUTTON (widget);
   gint32 cx, cy;

   /* Notice that button->pressed may have been set to FALSE in
    * Dw_button_leave_notify. */
   if (button->pressed && button->in_button && button->sensitive) {
      /* simple "click" */
      gtk_signal_emit (GTK_OBJECT (widget), button_signals[CLICKED]);

      /* "clicked_at": like "clicked", but position is passed, relative
       * to the child widget. */
      if (button->child) {
         cx = x + widget->allocation.x - button->child->allocation.x;
         cy = y + widget->allocation.y - button->child->allocation.y;

         /* If position is outside of the child, clip at the child
            allocation */
         if (cx < 0)
            cx = 0;
         else if (cx > button->child->allocation.width - 1)
            cx = button->child->allocation.width - 1;

         if (cy < 0)
            cy = 0;
         else if (cy > (DW_WIDGET_HEIGHT(button->child) - 1))
            cy = DW_WIDGET_HEIGHT(button->child) - 1;

         gtk_signal_emit (GTK_OBJECT (widget), button_signals[CLICKED_AT],
                          cx, cy);
      }
   }

   button->pressed = FALSE;
   if (button->relief)
      p_Dw_widget_queue_draw (widget);

   return TRUE;
}


/*
   Standard Dw function.

 */
static gboolean Dw_button_enter_notify (DwWidget *widget,
                                        DwWidget *last_widget,
                                        GdkEventMotion *event)
{
   DwButton *button = DW_BUTTON (widget);

   button->in_button = TRUE;
   if (button->relief)
      p_Dw_widget_queue_draw (widget);
   return TRUE;
}


/*
 * Standard Dw function.
 */
static gboolean Dw_button_leave_notify (DwWidget *widget,
                                        DwWidget *next_widget,
                                        GdkEventMotion *event)
{
   DwButton *button = DW_BUTTON (widget);

   if (button->child == NULL || next_widget != button->child) {
      button->in_button = FALSE;

      /* See comment at the beginning. */
      button->pressed = FALSE;

      if (button->relief)
         p_Dw_widget_queue_draw (widget);
   }
   return TRUE;
}


/*
 * Standard Dw function.
 */
static void Dw_button_add (DwContainer *container,
                           DwWidget *widget)
{
   DwButton *button = DW_BUTTON (container);

   g_return_if_fail (button->child == NULL);
   button->child = widget;
   p_Dw_widget_set_parent (widget, DW_WIDGET (container));
}


/*
 * Standard Dw function.
 */
static void Dw_button_remove (DwContainer *container,
                              DwWidget *widget)
{
   DwButton *button = DW_BUTTON (container);

   g_return_if_fail (button->child != NULL);
   g_return_if_fail (widget == button->child);
   button->child = NULL;
}


/*
 * Standard Dw function.
 */
static void Dw_button_forall (DwContainer *container,
                              DwCallback callback,
                              gpointer callback_data)
{
   DwButton *button = DW_BUTTON (container);

   if (button->child)
      callback (button->child, callback_data);
}



/*
 * An insensitive button does not respond on user interaction. Used
 * in HTML forms.
 */
void a_Dw_button_set_sensitive (DwButton *button,
                                gboolean sensitive)
{
   button->sensitive = sensitive;
   if (button->relief)
      p_Dw_widget_queue_draw (DW_WIDGET (button));
}


static DwIterator *Dw_button_iterator (DwWidget *widget,
                                       gint mask,
                                       gboolean at_end)
{
   DwIterator *it = g_new (DwIterator, 1);
   it->widget = widget;
   it->mask = mask;
   it->content.type = (at_end ? DW_CONTENT_END : DW_CONTENT_START);
   it->next = Dw_button_iterator_next;
   it->prev = Dw_button_iterator_prev;
   it->clone = p_Dw_iterator_clone_std;
   it->compare = Dw_button_iterator_compare;
   it->free = p_Dw_iterator_free_std;
   it->highlight = p_Dw_iterator_highlight_std;
   it->get_allocation = p_Dw_iterator_get_allocation_std_only_widgets;
   return it;
}


static gboolean Dw_button_iterator_next (DwIterator *it)
{
   if (it->content.type == DW_CONTENT_START &&
       (it->mask & DW_CONTENT_WIDGET) &&
       DW_BUTTON(it->widget)->child) {
      it->content.type = DW_CONTENT_WIDGET;
      it->content.data.widget = DW_BUTTON(it->widget)->child;
      return TRUE;
   } else {
      it->content.type = DW_CONTENT_END;
      return FALSE;
   }
}

static gboolean Dw_button_iterator_prev (DwIterator *it)
{
   if (it->content.type == DW_CONTENT_END &&
       (it->mask & DW_CONTENT_WIDGET) &&
       DW_BUTTON(it->widget)->child) {
      it->content.type = DW_CONTENT_WIDGET;
      it->content.data.widget = DW_BUTTON(it->widget)->child;
      return TRUE;
   } else {
      it->content.type = DW_CONTENT_START;
      return FALSE;
   }
}



static gint Dw_button_iterator_compare (DwIterator *it1,
                                        DwIterator *it2)
{
   if (it1->content.type == it2->content.type)
      return 0;

   switch (it1->content.type) {
   case DW_CONTENT_START:
      return -1;
   case DW_CONTENT_TEXT:
      if (it2->content.type == DW_CONTENT_START)
         return +1;
      else
         return -1;
   case DW_CONTENT_END:
      return +1;
   default:
      return 0;
   }
}
