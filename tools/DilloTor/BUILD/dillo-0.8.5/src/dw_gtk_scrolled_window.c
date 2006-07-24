/*
 * File: dw_gtk_scrolled_window.c
 *
 * Copyright (C) 2001  Sebastian Geerken <S.Geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stddef.h>

#include "dw_gtk_scrolled_window.h"
#include "dw_gtk_scrolled_frame.h"
#include "dw_gtk_viewport.h"
#include "findtext.h"
#include <gtk/gtk.h>

#include "debug.h"

static GtkScrolledWindowClass *parent_class = NULL;

static void Dw_gtk_scrolled_window_init      (GtkDwScrolledWindow *scrolled);
static void Dw_gtk_scrolled_window_class_init(GtkDwScrolledWindowClass *klass);

static void Dw_gtk_scrolled_window_destroy       (GtkObject *object);
static void Dw_gtk_scrolled_window_map           (GtkWidget *widget);
static void Dw_gtk_scrolled_window_unmap         (GtkWidget *widget);
static void Dw_gtk_scrolled_window_draw          (GtkWidget *widget,
                                                  GdkRectangle *area);
static void Dw_gtk_scrolled_window_size_allocate (GtkWidget *widget,
                                                  GtkAllocation *allocation);

static void Dw_gtk_scrolled_window_remove         (GtkContainer *container,
                                                   GtkWidget *widget);
static void Dw_gtk_scrolled_window_forall        (GtkContainer *container,
                                                  gboolean include_internals,
                                                  GtkCallback callback,
                                                  gpointer callback_data);

static void Dw_gtk_scrolled_window_changed1  (GtkDwScrolledWindow *scrolled);
static void Dw_gtk_scrolled_window_changed2  (GtkDwScrolledWindow *scrolled);


/*
 * Standard Gtk+ function
 */
GtkType a_Dw_gtk_scrolled_window_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "GtkDwScrolledWindow",
         sizeof (GtkDwScrolledWindow),
         sizeof (GtkDwScrolledWindowClass),
         (GtkClassInitFunc) Dw_gtk_scrolled_window_class_init,
         (GtkObjectInitFunc) Dw_gtk_scrolled_window_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (GTK_TYPE_SCROLLED_WINDOW, &info);
   }

   return type;
}


/*
 * Standard Gtk+ function
 */
GtkWidget* a_Dw_gtk_scrolled_window_new (void)
{
   return gtk_widget_new (GTK_TYPE_DW_SCROLLED_WINDOW, NULL);
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_window_init (GtkDwScrolledWindow *scrolled)
{
   GtkAdjustment *hadjustment, *vadjustment;
   GtkWidget *frame, *viewport;
   size_t i;
   char *signals[] = {
      "button_press_event",
      "button_release_event",
      "motion_notify_event",
      "key_press_event"         /* although the scrollbars are not focused */
   };

   hadjustment =
      GTK_ADJUSTMENT (gtk_adjustment_new (0.0, 0.0, 0.0, 10.0, 0.0, 0.0));
   vadjustment =
      GTK_ADJUSTMENT (gtk_adjustment_new (0.0, 0.0, 0.0, 10.0, 0.0, 0.0));

   gtk_scrolled_window_set_hadjustment (GTK_SCROLLED_WINDOW (scrolled),
                                        hadjustment);
   gtk_scrolled_window_set_vadjustment (GTK_SCROLLED_WINDOW (scrolled),
                                        vadjustment);

   frame = a_Dw_gtk_scrolled_frame_new (hadjustment, vadjustment);
   gtk_container_add (GTK_CONTAINER (scrolled), frame);
   gtk_widget_show (frame);

   viewport = a_Dw_gtk_viewport_new (hadjustment, vadjustment);
   gtk_container_add (GTK_CONTAINER (frame), viewport);
   gtk_widget_show (viewport);

   scrolled->vadjustment = vadjustment;
   scrolled->old_vadjustment_value = vadjustment->value;
   scrolled->gadget = NULL;

   /*
    * For anchors, we need to recognize when the *user* changes the
    * viewport and distiguish them from changes caused by the program.
    * Instead of using the "change" signal and checking where it came
    * from, the following code connects all possible events by which
    * users could change the scrollbar adjustments. ...
    */
   for (i = 0; i < sizeof (signals) / sizeof (signals[0]); i++) {
      gtk_signal_connect_object
         (GTK_OBJECT (GTK_SCROLLED_WINDOW(scrolled)->vscrollbar),
          signals[i],  GTK_SIGNAL_FUNC (Dw_gtk_scrolled_window_changed1),
          GTK_OBJECT (scrolled));
      gtk_signal_connect_object_after
         (GTK_OBJECT (GTK_SCROLLED_WINDOW(scrolled)->vscrollbar),
          signals[i],  GTK_SIGNAL_FUNC (Dw_gtk_scrolled_window_changed2),
          GTK_OBJECT (scrolled));
   }

   /* ... The GtkDwScrolledFrame has a signal for this. */
   gtk_signal_connect_object (GTK_OBJECT (frame), "user_vchanged",
                              GTK_SIGNAL_FUNC (Dw_gtk_viewport_remove_anchor),
                              GTK_OBJECT (viewport));
#if 0
   /* This does not seem to work for GtkLayout's (see also dw_embed_gtk.c): */
   gtk_container_set_focus_hadjustment (GTK_CONTAINER (viewport),
                                        hadjustment);
   gtk_container_set_focus_vadjustment (GTK_CONTAINER (viewport),
                                        vadjustment);
#endif
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_window_class_init (GtkDwScrolledWindowClass *klass)
{
   GtkObjectClass *object_class;
   GtkWidgetClass *widget_class;
   GtkContainerClass *container_class;

   object_class = (GtkObjectClass*) klass;
   widget_class = (GtkWidgetClass*) klass;
   container_class = (GtkContainerClass*) klass;
   parent_class = gtk_type_class (GTK_TYPE_SCROLLED_WINDOW);

   object_class->destroy = Dw_gtk_scrolled_window_destroy;

   widget_class->map = Dw_gtk_scrolled_window_map;
   widget_class->unmap = Dw_gtk_scrolled_window_unmap;
   widget_class->draw = Dw_gtk_scrolled_window_draw;
   widget_class->size_allocate = Dw_gtk_scrolled_window_size_allocate;

   container_class->remove = Dw_gtk_scrolled_window_remove;
   container_class->forall = Dw_gtk_scrolled_window_forall;
}

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_window_destroy (GtkObject *object)
{
   GtkDwScrolledWindow *scrolled = GTK_DW_SCROLLED_WINDOW (object);

   if (scrolled->gadget)
      gtk_widget_destroy (scrolled->gadget);

   GTK_OBJECT_CLASS(parent_class)->destroy (object);
}

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_window_map (GtkWidget *widget)
{
   GtkDwScrolledWindow *scrolled = GTK_DW_SCROLLED_WINDOW (widget);

   GTK_WIDGET_CLASS(parent_class)->map (widget);

   if (scrolled->gadget && GTK_WIDGET_VISIBLE (scrolled->gadget) &&
       !GTK_WIDGET_MAPPED (scrolled->gadget))
      gtk_widget_map (scrolled->gadget);
}

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_window_unmap (GtkWidget *widget)
{
   GtkDwScrolledWindow *scrolled = GTK_DW_SCROLLED_WINDOW (widget);

   GTK_WIDGET_CLASS(parent_class)->unmap (widget);

   if (scrolled->gadget && GTK_WIDGET_MAPPED (scrolled->gadget))
      gtk_widget_unmap (scrolled->gadget);
}

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_window_draw (GtkWidget *widget,
                                         GdkRectangle *area)
{
   GtkDwScrolledWindow *scrolled = GTK_DW_SCROLLED_WINDOW (widget);
   GdkRectangle child_area;

   GTK_WIDGET_CLASS(parent_class)->draw (widget, area);

   if (scrolled->gadget && GTK_WIDGET_VISIBLE (scrolled->gadget) &&
       gtk_widget_intersect (scrolled->gadget, area, &child_area))
      gtk_widget_draw (scrolled->gadget, &child_area);
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_window_size_allocate (GtkWidget *widget,
                                                  GtkAllocation *allocation)
{
   GtkDwViewport *viewport;
   GtkAllocation old_allocation = widget->allocation, child_allocation;
   GtkRequisition child_requisition;
   GtkDwScrolledWindow *scrolled;
   GtkWidget *hscrollbar, *vscrollbar;
   gint gx, gy;

   GTK_WIDGET_CLASS(parent_class)->size_allocate (widget, allocation);
   widget->allocation = *allocation;

   DEBUG_MSG (2, "Dw_gtk_scrolled_window_size_allocate: %d x %d\n",
              allocation->width, allocation->height);
   if (old_allocation.width != allocation->width ||
       old_allocation.height != allocation->height) {
      viewport =  GTK_DW_VIEWPORT (GTK_BIN(GTK_BIN(widget)->child)->child);

      /* It may be that scrollbars are not needed anymore. See
         Dw_gtk_viewport_calc_size for more details. */
      if (allocation->width > old_allocation.width)
         viewport->hscrollbar_used = FALSE;
      if (allocation->height > old_allocation.height)
         viewport->vscrollbar_used = FALSE;

      Dw_gtk_viewport_calc_size (viewport);
   }

   scrolled = GTK_DW_SCROLLED_WINDOW (widget);
   if (scrolled->gadget && GTK_WIDGET_VISIBLE (scrolled->gadget)) {
      /* Raise the gagdet, otherwise it may be hidden. */
      if (GTK_WIDGET_REALIZED (scrolled->gadget))
          gdk_window_raise (scrolled->gadget->window);

      /* Allocate the gadget. */
      gtk_widget_size_request (scrolled->gadget, &child_requisition);
      gx = allocation->x + allocation->width - child_requisition.width;
      gy = allocation->y + allocation->height - child_requisition.height;
      child_allocation.x = MAX (gx, 1);
      child_allocation.y = MAX (gy, 1);
      child_allocation.width = child_requisition.width;
      child_allocation.height = child_requisition.height;
      gtk_widget_size_allocate (scrolled->gadget, &child_allocation);

      /* Re-allocate the scrollbars, when necessary */
      hscrollbar = GTK_SCROLLED_WINDOW(scrolled)->hscrollbar;
      if (GTK_WIDGET_VISIBLE (hscrollbar) &&
          hscrollbar->allocation.x + hscrollbar->allocation.width > gx) {
         child_allocation = hscrollbar->allocation;
         child_allocation.width = MAX (gx - hscrollbar->allocation.x, 1);
         gtk_widget_size_allocate (hscrollbar, &child_allocation);
      }

      vscrollbar = GTK_SCROLLED_WINDOW(scrolled)->vscrollbar;
      if (GTK_WIDGET_VISIBLE (vscrollbar) &&
          vscrollbar->allocation.y + vscrollbar->allocation.height > gy) {
         child_allocation = vscrollbar->allocation;
         child_allocation.height = MAX (gy - vscrollbar->allocation.y, 1);
         gtk_widget_size_allocate (vscrollbar, &child_allocation);
      }
   }
}

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_window_remove  (GtkContainer *container,
                                            GtkWidget *widget)
{
   GtkDwScrolledWindow *scrolled = GTK_DW_SCROLLED_WINDOW (container);
   gboolean widget_was_visible;

   if (widget == scrolled->gadget) {
       widget_was_visible = GTK_WIDGET_VISIBLE (widget);
       gtk_widget_unparent (widget);
       scrolled->gadget = NULL;
   } else
      GTK_CONTAINER_CLASS(parent_class)->remove (container, widget);
}

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_window_forall (GtkContainer *container,
                                           gboolean include_internals,
                                           GtkCallback callback,
                                           gpointer callback_data)
{
   GtkDwScrolledWindow *scrolled = GTK_DW_SCROLLED_WINDOW (container);

   GTK_CONTAINER_CLASS(parent_class)->forall (container, include_internals,
                                              callback, callback_data);
   if (scrolled->gadget)
      callback (scrolled->gadget, callback_data);
}


/*
 * Adds the gadget widget.
 */
void a_Dw_gtk_scrolled_window_add_gadget (GtkDwScrolledWindow *scrolled,
                                          GtkWidget *gadget)
{
   g_return_if_fail (scrolled->gadget == NULL);
   scrolled->gadget = gadget;
   gtk_widget_set_parent (gadget, GTK_WIDGET (scrolled));
   gtk_widget_queue_resize (GTK_WIDGET (scrolled));
}


/*
 * Sets the top-level DwWidget. The old top-level DwWidget is destroyed.
 */
void a_Dw_gtk_scrolled_window_set_dw (GtkDwScrolledWindow *scrolled,
                                      DwWidget *widget)
{
   GtkWidget *viewport;
   DwWidget *old_child;

   viewport = GTK_BIN(GTK_BIN(scrolled)->child)->child;

   if ((old_child = GTK_DW_VIEWPORT (viewport)->child))
      gtk_object_destroy (GTK_OBJECT (old_child));

   a_Dw_gtk_viewport_add_dw (GTK_DW_VIEWPORT (viewport), widget);
}

/*
 * Gets the top-level DwWidget.
 */
DwWidget* a_Dw_gtk_scrolled_window_get_dw (GtkDwScrolledWindow *scrolled)
{
   GtkWidget *viewport;

   viewport = GTK_BIN(GTK_BIN(scrolled)->child)->child;
   return GTK_DW_VIEWPORT(viewport)->child;
}

/*
 * See a_Dw_gtk_viewport_set_anchor.
 */
void a_Dw_gtk_scrolled_window_set_anchor (GtkDwScrolledWindow *scrolled,
                                          const gchar *anchor)
{
   GtkWidget *viewport;

   viewport = GTK_BIN(GTK_BIN(scrolled)->child)->child;
   a_Dw_gtk_viewport_set_anchor (GTK_DW_VIEWPORT (viewport), anchor);
}

/*
 * The scrolling position is remembered setting this value in the history-URL
 */
gint a_Dw_gtk_scrolled_window_get_scrolling_position_x (GtkDwScrolledWindow
                                                        *scrolled)
{
   GtkLayout *viewport = GTK_LAYOUT(GTK_BIN(GTK_BIN(scrolled)->child)->child);

   return ((int) viewport->hadjustment->value);
}

/*
 * The scrolling position is remembered setting this value in the history-URL
 */
gint a_Dw_gtk_scrolled_window_get_scrolling_position_y (GtkDwScrolledWindow
                                                        *scrolled)
{
   GtkLayout *viewport = GTK_LAYOUT(GTK_BIN(GTK_BIN(scrolled)->child)->child);

   return ((int) viewport->vadjustment->value);
}

/*
 * See a_Dw_gtk_viewport_set_scrolling_position.
 */
void a_Dw_gtk_scrolled_window_set_scrolling_position (GtkDwScrolledWindow
                                                      *scrolled,
                                                      gint32 x,
                                                      gint32 y)
{
   GtkWidget *viewport;

   viewport = GTK_BIN(GTK_BIN(scrolled)->child)->child;
   a_Dw_gtk_viewport_set_scrolling_position (GTK_DW_VIEWPORT (viewport), x, y);
}


/*
 * See also Dw_gtk_scrolled_window_init.
 * Called before possible change, save the old value.
 */
static void Dw_gtk_scrolled_window_changed1 (GtkDwScrolledWindow *scrolled)
{
   scrolled->old_vadjustment_value = scrolled->vadjustment->value;
}


/*
 * See also Dw_gtk_scrolled_window_init.
 * Called after possible change, compare old and new values.
 */
static void Dw_gtk_scrolled_window_changed2 (GtkDwScrolledWindow *scrolled)
{
   GtkWidget *viewport;

   if (scrolled->old_vadjustment_value != scrolled->vadjustment->value) {
      viewport = GTK_BIN(GTK_BIN(scrolled)->child)->child;
      Dw_gtk_viewport_remove_anchor (GTK_DW_VIEWPORT (viewport));
   }
}

/*
 * Convenience function. See a_Findtext_search.
 */
gboolean a_Dw_gtk_scrolled_window_search (GtkDwScrolledWindow *scrolled,
                                          gchar *string,
                                          gboolean case_sens)
{
   GtkWidget *viewport;

   viewport = GTK_BIN(GTK_BIN(scrolled)->child)->child;
   return
      a_Findtext_search (GTK_DW_VIEWPORT(viewport)->findtext_state, string,
                         case_sens);
}

/*
 * Convenience function. See a_Findtext_reset_search.
 */
void a_Dw_gtk_scrolled_window_reset_search (GtkDwScrolledWindow *scrolled)
{
   GtkWidget *viewport;

   viewport = GTK_BIN(GTK_BIN(scrolled)->child)->child;
   a_Findtext_reset_search (GTK_DW_VIEWPORT(viewport)->findtext_state);
}

/*
 * Convenience function. See a_Dw_gtk_viewport_widget_at_viewport_point.
 */
DwWidget*  a_Dw_gtk_scrolled_window_widget_at_viewport_point (
              GtkDwScrolledWindow *scrolled,
              gint32 vx,
              gint32 vy)
{
   GtkWidget *viewport;

   viewport = GTK_BIN(GTK_BIN(scrolled)->child)->child;
   return
      a_Dw_gtk_viewport_widget_at_viewport_point (GTK_DW_VIEWPORT(viewport),
                                                  vx, vy);
}
