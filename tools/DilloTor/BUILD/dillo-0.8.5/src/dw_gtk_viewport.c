/*
 * File: dw_gtk_viewport.c
 *
 * Copyright (C) 2001-2003  Sebastian Geerken <S.Geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <gtk/gtk.h>
#include "msg.h"
#include "dw_gtk_viewport.h"
#include "dw_container.h"
#include "list.h"

/*#define DEBUG_LEVEL 1*/
#include "debug.h"
#include "msg.h"

typedef struct
{
   gchar *name;
   DwWidget *widget;
   gint32 y;
} GtkDwViewportAnchor;

static GtkLayoutClass *parent_class = NULL;

/* object/class initialisation */
static void Dw_gtk_viewport_init          (GtkDwViewport *viewport);
static void Dw_gtk_viewport_class_init    (GtkDwViewportClass *klass);

/* GtkObject methods */
static void Dw_gtk_viewport_destroy       (GtkObject *object);

/* GtkWidget methods */
static void Dw_gtk_viewport_size_allocate (GtkWidget *widget,
                                           GtkAllocation *allocation);
static void Dw_gtk_viewport_realize       (GtkWidget *widget);
static void Dw_gtk_viewport_draw          (GtkWidget *widget,
                                           GdkRectangle *area);
static gint Dw_gtk_viewport_expose        (GtkWidget *widget,
                                           GdkEventExpose *event);
static gint Dw_gtk_viewport_button_press  (GtkWidget *widget,
                                           GdkEventButton *event);
static gint Dw_gtk_viewport_button_release(GtkWidget *widget,
                                           GdkEventButton *event);
static gint Dw_gtk_viewport_motion_notify (GtkWidget *widget,
                                           GdkEventMotion *event);
static gint Dw_gtk_viewport_enter_notify  (GtkWidget *widget,
                                           GdkEventCrossing *event);
static gint Dw_gtk_viewport_leave_notify  (GtkWidget *widget,
                                           GdkEventCrossing *event);
static void Dw_gtk_viewport_adj_changed   (GtkAdjustment *adj,
                                           GtkDwViewport *viewport);

/*
 * Standard Gtk+ function
 */
GtkType a_Dw_gtk_viewport_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "GtkDwViewport",
         sizeof (GtkDwViewport),
         sizeof (GtkDwViewportClass),
         (GtkClassInitFunc) Dw_gtk_viewport_class_init,
         (GtkObjectInitFunc) Dw_gtk_viewport_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (GTK_TYPE_LAYOUT, &info);
   }

   return type;
}


/*
 * Standard Gtk+ function
 */
GtkWidget* a_Dw_gtk_viewport_new (GtkAdjustment *hadjustment,
                                  GtkAdjustment *vadjustment)
{
   GtkWidget *widget;

   widget = gtk_widget_new (GTK_TYPE_DW_VIEWPORT, NULL);
   gtk_layout_set_hadjustment (GTK_LAYOUT (widget), hadjustment);
   gtk_layout_set_vadjustment (GTK_LAYOUT (widget), vadjustment);

   /* Following two statements expect that the adjustments are passed as
    * arguments (!= NULL), and don't change. This is the case in dillo,
    * however, for more general perposes, the signal function
    * "set_scroll_adjustments" had to be redefined.
    */
   gtk_signal_connect (GTK_OBJECT (hadjustment), "value_changed",
                       GTK_SIGNAL_FUNC (Dw_gtk_viewport_adj_changed),
                       (gpointer) widget);
   gtk_signal_connect (GTK_OBJECT (vadjustment), "value_changed",
                       GTK_SIGNAL_FUNC (Dw_gtk_viewport_adj_changed),
                       (gpointer) widget);

   return widget;
}

/*********************************
 *                               *
 *  object/class initialisation  *
 *                               *
 *********************************/

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_viewport_init (GtkDwViewport *viewport)
{
   DBG_OBJ_CREATE (viewport, "GtkDwViewport");

   GTK_WIDGET_UNSET_FLAGS (viewport, GTK_NO_WINDOW);
   GTK_WIDGET_UNSET_FLAGS (viewport, GTK_CAN_FOCUS);

   /* Without this, gtk_layout_{draw|expose} will clear the window.
      Look at gtklayout.c */
   GTK_WIDGET_SET_FLAGS (viewport, GTK_APP_PAINTABLE);

   viewport->back_pixmap = NULL;
   viewport->child = NULL;
   viewport->last_entered = NULL;
   viewport->draw_resize_idle_id = 0;
   viewport->anchor = NULL;
   viewport->anchor_idle_id = 0;
   viewport->findtext_state = a_Findtext_state_new ();
   viewport->selection = a_Selection_new ();
   viewport->anchors_table = g_hash_table_new (g_str_hash, g_str_equal);
   viewport->draw_areas = NULL;
   viewport->num_draw_areas = 0;
   viewport->num_draw_areas_max = 4;

   DBG_OBJ_ASSOC (viewport->findtext_state, viewport);
   DBG_OBJ_ASSOC (viewport->selection, viewport);
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_viewport_class_init (GtkDwViewportClass *klass)
{
   GtkObjectClass *object_class;
   GtkWidgetClass *widget_class;

   parent_class = gtk_type_class (gtk_layout_get_type ());

   object_class = (GtkObjectClass*) klass;
   widget_class = (GtkWidgetClass*) klass;

   object_class->destroy = Dw_gtk_viewport_destroy;

   widget_class->size_allocate = Dw_gtk_viewport_size_allocate;
   widget_class->realize = Dw_gtk_viewport_realize;
   widget_class->draw = Dw_gtk_viewport_draw;
   widget_class->expose_event = Dw_gtk_viewport_expose;
   widget_class->button_press_event = Dw_gtk_viewport_button_press;
   widget_class->button_release_event = Dw_gtk_viewport_button_release;
   widget_class->motion_notify_event = Dw_gtk_viewport_motion_notify;
   widget_class->enter_notify_event = Dw_gtk_viewport_enter_notify;
   widget_class->leave_notify_event = Dw_gtk_viewport_leave_notify;
}


/***********************
 *                     *
 *  GtkObject methods  *
 *                     *
 ***********************/

static gboolean Dw_gtk_viewport_destroy_anchor (gpointer key,
                                                gpointer value,
                                                gpointer user_data)
{
   g_free (value);
   return TRUE;
}

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_viewport_destroy (GtkObject *object)
{
   GtkDwViewport *viewport;

   viewport = GTK_DW_VIEWPORT (object);

   if (viewport->back_pixmap)
      gdk_pixmap_unref (viewport->back_pixmap);
   if (viewport->child)
      gtk_object_destroy (GTK_OBJECT (viewport->child));
   if (viewport->draw_resize_idle_id != 0)
      gtk_idle_remove (viewport->draw_resize_idle_id);
   if (viewport->anchor_idle_id != 0)
      gtk_idle_remove (viewport->anchor_idle_id);
   g_free (viewport->anchor);

   g_hash_table_foreach_remove (viewport->anchors_table,
                                Dw_gtk_viewport_destroy_anchor, NULL);
   g_hash_table_destroy (viewport->anchors_table);

   g_free (viewport->draw_areas);

   a_Findtext_state_destroy (viewport->findtext_state);
   a_Selection_free (viewport->selection);

   GTK_OBJECT_CLASS(parent_class)->destroy (object);
}


/***********************
 *                     *
 *  GtkWidget methods  *
 *                     *
 ***********************/

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_viewport_size_allocate (GtkWidget *widget,
                                           GtkAllocation *allocation)
{
   GTK_WIDGET_CLASS(parent_class)->size_allocate (widget, allocation);

   /* gtk_layout_size_allocate() sets them to different values. */
   GTK_LAYOUT(widget)->hadjustment->page_increment = allocation->width;
   GTK_LAYOUT(widget)->vadjustment->page_increment = 0.96 * allocation->height;
}

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_viewport_realize (GtkWidget *widget)
{
   GtkDwViewport *viewport;

   GTK_WIDGET_CLASS(parent_class)->realize (widget);

   gdk_window_set_events (widget->window,
                          gdk_window_get_events (widget->window)
                          | GDK_BUTTON_PRESS_MASK
                          | GDK_BUTTON_RELEASE_MASK
                          | GDK_POINTER_MOTION_MASK
                          | GDK_ENTER_NOTIFY_MASK
                          | GDK_LEAVE_NOTIFY_MASK);

   viewport = GTK_DW_VIEWPORT (widget);
   gdk_window_get_geometry (widget->window, NULL, NULL, NULL, NULL,
                            &viewport->depth);

   Dw_gtk_viewport_update_background (viewport);
   if (viewport->child)
      Dw_widget_update_cursor (viewport->child);
}


/*
 * (Nearly) standard Gtk+ function
 */
static void Dw_gtk_viewport_paint (GtkWidget *widget,
                                   GdkRectangle *area,
                                   GdkEventExpose *event)
{
   GtkLayout *layout;
   DwRectangle parent_area, child_area, intersection;
   GtkDwViewport *viewport;
   gboolean new_back_pixmap;

   if (GTK_WIDGET_DRAWABLE (widget)) {
      layout = GTK_LAYOUT (widget);
      viewport = GTK_DW_VIEWPORT (widget);

      DEBUG_MSG (2, "Drawing (%d, %d), %d x %d\n",
                 area->x, area->y, area->width, area->height);

      /* Make sure the backing pixmap is large enough. */
      if (viewport->child) {
         if (viewport->back_pixmap)
            new_back_pixmap =
               (widget->allocation.width > viewport->back_width ||
                widget->allocation.height > viewport->back_height);
         else
            new_back_pixmap = TRUE;

         if (new_back_pixmap) {
            if (viewport->back_pixmap)
               gdk_pixmap_unref (viewport->back_pixmap);
            viewport->back_pixmap = gdk_pixmap_new (widget->window,
                                                    widget->allocation.width,
                                                    widget->allocation.height,
                                                    viewport->depth);
            viewport->back_width = widget->allocation.width;
            viewport->back_height = widget->allocation.height;
            DEBUG_MSG (1, "   Creating new pixmap, size = %d x %d\n",
                       widget->allocation.width, widget->allocation.height);
         }

         /* Draw top-level Dw widget. */
         parent_area.x =
            p_Dw_widget_x_viewport_to_world (viewport->child, area->x);
         parent_area.y =
            p_Dw_widget_y_viewport_to_world (viewport->child, area->y);
         parent_area.width = area->width;
         parent_area.height = area->height;

         child_area.x = viewport->child->allocation.x;
         child_area.y = viewport->child->allocation.y;
         child_area.width = viewport->child->allocation.width;
         child_area.height = DW_WIDGET_HEIGHT(viewport->child);

         if (p_Dw_rectangle_intersect (&parent_area, &child_area,
                                       &intersection)) {
            intersection.x -= viewport->child->allocation.x;
            intersection.y -= viewport->child->allocation.y;

            /* "Clear" backing pixmap. */
            gdk_draw_rectangle (viewport->back_pixmap,
                                viewport->child->style->background_color->gc,
                                TRUE, area->x, area->y,
                                area->width, area->height);
            /* Widgets draw in backing pixmap. */
            p_Dw_widget_draw (viewport->child, &intersection, event);
            /* Copy backing pixmap into window. */
            gdk_draw_pixmap (layout->bin_window, widget->style->black_gc,
                             viewport->back_pixmap, area->x, area->y,
                             area->x, area->y, area->width, area->height);
         }
      } else
         gdk_window_clear_area (layout->bin_window,
                                area->x, area->y, area->width, area->height);
   }
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_viewport_draw (GtkWidget *widget,
                                  GdkRectangle *area)
{
   Dw_gtk_viewport_paint (widget, area, NULL);
   GTK_WIDGET_CLASS(parent_class)->draw (widget, area);
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_viewport_expose (GtkWidget *widget,
                                    GdkEventExpose *event)
{
   Dw_gtk_viewport_paint (widget, &(event->area), event);
   return GTK_WIDGET_CLASS(parent_class)->expose_event (widget, event);
}


/*
 * Handle the mouse event and deliver it to the Dw widget.
 * Most is done in a_Dw_widget_mouse_event.
 */
static gint Dw_gtk_viewport_mouse_event (GtkWidget *widget,
                                         gint32 x,
                                         gint32 y,
                                         GdkEvent *event)
{
   GtkDwViewport *viewport;
   DwWidget *dw_widget;
   gint32 world_x, world_y;

   if (event == NULL || event->any.window == widget->window) {
      viewport = GTK_DW_VIEWPORT (widget);
      if (viewport->child) {
         world_x = x + gtk_layout_get_hadjustment(GTK_LAYOUT(viewport))->value;
         world_y = y + gtk_layout_get_vadjustment(GTK_LAYOUT(viewport))->value;
         dw_widget =
            Dw_gtk_viewport_widget_at_point (viewport, world_x, world_y);

         return Dw_widget_mouse_event (dw_widget, widget,
                                       world_x, world_y, event);
      }
   }

   return FALSE;
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_viewport_button_press (GtkWidget *widget,
                                          GdkEventButton *event)
{
   /* We focus always the viewport window (i.e., more precisely, the
    * GktDwScrolledFrame). */
   if (widget->parent)
      gtk_widget_grab_focus(widget->parent);

   return Dw_gtk_viewport_mouse_event (widget, event->x, event->y,
                                       (GdkEvent*) event);
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_viewport_button_release (GtkWidget *widget,
                                            GdkEventButton *event)
{
   return Dw_gtk_viewport_mouse_event (widget, event->x, event->y,
                                       (GdkEvent*) event);
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_viewport_motion_notify (GtkWidget *widget,
                                           GdkEventMotion *event)
{
   GtkDwViewport *viewport = GTK_DW_VIEWPORT (widget);

   viewport->mouse_x = event->x;
   viewport->mouse_y = event->y;
   return Dw_gtk_viewport_mouse_event (widget, event->x, event->y,
                                       (GdkEvent*) event);
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_viewport_enter_notify (GtkWidget *widget,
                                          GdkEventCrossing *event)
{
   return Dw_gtk_viewport_mouse_event (widget, event->x, event->y, NULL);
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_viewport_leave_notify (GtkWidget *widget,
                                          GdkEventCrossing *event)
{
   /* There will anyway be no Dw widget, thus this simple call */
   return Dw_widget_mouse_event (NULL, widget, 0, 0, NULL);
}


/*
 * This function is called when the viewport changes, and causes
 * motion_notify events to be simulated.
 */
static void Dw_gtk_viewport_adj_changed (GtkAdjustment *adj,
                                         GtkDwViewport *viewport)
{
   Dw_gtk_viewport_mouse_event (GTK_WIDGET (viewport),
                                viewport->mouse_x, viewport->mouse_y, NULL);
}

/*
 * This function sets the background and the viewport.
 */
void Dw_gtk_viewport_update_background (GtkDwViewport *viewport)
{
   /* The toplevel widget should always have a defined background color,
    * except at the beginning. Searching a defined background is not
    * necessary. */
   if (viewport->child && viewport->child->style && 
       viewport->child->style->background_color)
      gdk_window_set_background (GTK_LAYOUT(viewport)->bin_window,
                                 &viewport->child->style->background_color
                                 ->color);
}

/**********************
 *                    *
 *  public functions  *
 *                    *
 **********************/

/*
 * Set the top-level Dw widget.
 * If there is already one, you must destroy it before, otherwise the
 * function will fail.
 */
void a_Dw_gtk_viewport_add_dw (GtkDwViewport *viewport,
                               DwWidget *widget)
{
   g_return_if_fail (viewport->child == NULL);

   viewport->child = widget;
   DBG_OBJ_ASSOC(widget, viewport);

   widget->parent = NULL;
   widget->viewport = GTK_WIDGET (viewport);

   Dw_gtk_viewport_update_background (viewport);
   Dw_widget_update_cursor (viewport->child);

   Dw_gtk_viewport_calc_size (viewport);
   Dw_gtk_viewport_remove_anchor (viewport);

   a_Findtext_state_set_widget (viewport->findtext_state, widget);
   a_Selection_reset (viewport->selection);
}

/**************************************************
 *                                                *
 *  Functions used by GtkDwViewport and DwWidget  *
 *                                                *
 **************************************************/

/*
 * This function only *recognizes* that the top-level Dw widget has
 * been destroyed, and only removes it from the viewport. It is called
 * by Dw_widget_destroy. Don't use this function directly!
 *
 * N.b.: a widget, which stores anchors, should already have removed
 * them from the widget, by calling a_Dw_gtk_viewport_remove_anchor().
 * A good place for this is the implementation of GtkObject::destroy,
 * the call to the parent destroy method should then be done at the
 * end of the function, since Dw_widget_destroy calls this function.
 */
void Dw_gtk_viewport_remove_dw (GtkDwViewport *viewport)
{
   /* Test, that all anchors have been removed properly. */
   gint num_anchors_left = g_hash_table_size (viewport->anchors_table);
   /* g_assert (num_anchors_left == 0); */
   if (num_anchors_left != 0)
      g_warning ("%d anchors left", num_anchors_left);

   a_Findtext_state_set_widget (viewport->findtext_state, NULL);
   viewport->child = NULL;
   Dw_gtk_viewport_remove_anchor (viewport);
   Dw_gtk_viewport_calc_size (viewport);
}


/*
 * Used by Dw_gtk_viewport_calc_size.
 */
static void Dw_gtk_viewport_calc_child_size (GtkDwViewport *viewport,
                                             gint32 child_width,
                                             gint32 child_height,
                                             DwRequisition *child_requisition)
{
   if (child_width < 0) child_width = 0;
   if (child_height < 0) child_height = 0;

   DEBUG_MSG (2, "   width = %d, height = %d ...\n",
              child_width, child_height);

   p_Dw_widget_set_width (viewport->child, child_width);
   p_Dw_widget_set_ascent (viewport->child, child_height);
   p_Dw_widget_set_descent (viewport->child, 0);

   p_Dw_widget_size_request (viewport->child, child_requisition);
}


/*
 * Calculate the size of the scrolled area and allocate the top-level
 * widget. This function is called when the top-level Dw widget has
 * changed its size etc.
 */
void Dw_gtk_viewport_calc_size (GtkDwViewport *viewport)
{
   GtkWidget *widget;
   GtkScrolledWindow *scrolled;

   DwRequisition child_requisition;
   DwAllocation child_allocation;
   gint border_width,  space;

   GtkRequisition bar_requisition;
   gint max_width, max_height, bar_width_diff, bar_height_diff, child_height;

   if (viewport->calc_size_blocked)
      return;

   viewport->calc_size_blocked = TRUE;

   if (viewport->child) {
      /*
       * Determine the size hints for the Dw widget. This is a bit
       * tricky, because you must know if scrollbars are visible or
       * not, which depends on the size of the Dw widget, which then
       * depends on the hints. The idea is to test several
       * configurations, there are four of them, from combining the
       * cases horizontal/vertical scrollbar visible/invisible.
       *
       * For optimization, the horizontal scrollbar is currently not
       * regarded, the height hint is always the same, as if the
       * scrollbar was allways visible. In future, this may be
       * implemented correctly, by using the minimal width to optimize
       * most cases. (Minimal widths will also be used by tables.)
       *
       * Furthermore, the last result (vertical scrollbar visible or
       * not) is stored in the viewport, and tested first. This will
       * make a second test only necessary when the visibility
       * switches, which normally happens only once when filling the
       * page with text. (Actually, this assumes that the page size is
       * always *growing*, but this is nevertheless true in dillo.)
       */

      widget = GTK_WIDGET (viewport);
      scrolled = GTK_SCROLLED_WINDOW (widget->parent->parent);
      space = GTK_SCROLLED_WINDOW_CLASS(GTK_OBJECT(scrolled)->klass)
         ->scrollbar_spacing;
      border_width = GTK_CONTAINER(viewport)->border_width;

      gtk_widget_size_request (scrolled->vscrollbar, &bar_requisition);
      bar_width_diff = bar_requisition.width + space;
      max_width = widget->allocation.width - 2 * border_width;
      if (scrolled->vscrollbar_visible)
         max_width += bar_width_diff;

      gtk_widget_size_request (scrolled->hscrollbar, &bar_requisition);
      bar_height_diff = bar_requisition.height + space;
      max_height = widget->allocation.height - 2 * border_width;
      if (scrolled->hscrollbar_visible)
         max_height += bar_height_diff;

      DEBUG_MSG (2, "------------------------------------------------->\n");
      DEBUG_MSG (2, "Dw_gtk_viewport_calc_size: %d x %d (%c/%c) -> %d x %d\n",
                 widget->allocation.width, widget->allocation.height,
                 scrolled->vscrollbar_visible ? 't' : 'f',
                 scrolled->hscrollbar_visible ? 't' : 'f',
                 max_width, max_height);

      if (scrolled->vscrollbar_policy == GTK_POLICY_NEVER)
         child_height = max_height;
      else
         child_height = max_height - bar_height_diff;

      switch (scrolled->vscrollbar_policy) {
      case GTK_POLICY_ALWAYS:
         Dw_gtk_viewport_calc_child_size (viewport, max_width - bar_width_diff,
                                          child_height,
                                          &child_requisition);
         break;

      case GTK_POLICY_AUTOMATIC:
         if (viewport->vscrollbar_used) {
            DEBUG_MSG (2, "Testing with vertical scrollbar ...\n");
            Dw_gtk_viewport_calc_child_size (viewport,
                                             max_width - bar_width_diff,
                                             child_height,
                                             &child_requisition);

            if (child_requisition.ascent
                + child_requisition.descent <= child_height) {
               DEBUG_MSG (2, "   failed!\n");
               Dw_gtk_viewport_calc_child_size (viewport, max_width,
                                                child_height,
                                                &child_requisition);
               viewport->vscrollbar_used = TRUE;
            }

         } else {
            DEBUG_MSG (2, "Testing without vertical scrollbar ...\n");
            Dw_gtk_viewport_calc_child_size (viewport, max_width,
                                             child_height,
                                             &child_requisition);

            /* todo: see above */
            if (child_requisition.ascent
                + child_requisition.descent > child_height) {
               DEBUG_MSG (2, "   failed!\n");
               Dw_gtk_viewport_calc_child_size (viewport,
                                                max_width - bar_width_diff,
                                                child_height,
                                                &child_requisition);
               viewport->vscrollbar_used = TRUE;
            }
         }
         break;

      case GTK_POLICY_NEVER:
         Dw_gtk_viewport_calc_child_size (viewport, max_width,
                                          child_height,
                                          &child_requisition);
      }

      child_allocation.x = border_width;
      child_allocation.y = border_width;
      child_allocation.width = child_requisition.width;
      child_allocation.ascent = child_requisition.ascent;
      child_allocation.descent = child_requisition.descent;
      p_Dw_widget_size_allocate (viewport->child, &child_allocation);

      gtk_layout_set_size (GTK_LAYOUT (viewport),
                           child_requisition.width + 2 * border_width,
                           child_requisition.ascent + child_requisition.descent
                           + 2 * border_width);

      DEBUG_MSG (1, "Setting size to %d x %d\n",
                 child_requisition.width + 2 * border_width,
                 child_requisition.ascent + child_requisition.descent
                 + 2 * border_width);

      DEBUG_MSG (2, "<-------------------------------------------------\n");
   } else {
      gtk_layout_set_size (GTK_LAYOUT (viewport), 1, 1);
      viewport->hscrollbar_used = FALSE;
      viewport->vscrollbar_used = FALSE;
   }

   Dw_gtk_viewport_update_anchor (viewport);
   gtk_widget_queue_draw (GTK_WIDGET (viewport));

   viewport->calc_size_blocked = FALSE;
}


/* used by Dw_gtk_viewport_widget_at_point */
typedef struct
{
   gint32 x;
   gint32 y;
   DwWidget *widget;
} WidgetAtPointData;

/* used by Dw_gtk_viewport_widget_at_point */
static void Dw_gtk_viewport_widget_at_point_callback (DwWidget *widget,
                                                      gpointer data)
{
   WidgetAtPointData *callback_data;

   callback_data = (WidgetAtPointData*) data;
   DEBUG_MSG (1, "  Checking %p ...\n", widget);

   if (/* As a special exception, for the top-level widget, not the
        * allocation is regarded, but the whole viewport. This makes
        * selections more useful, since so the user can start the
        * selection outside of the allocation. */
       widget->parent == NULL ||
       /* Otherwise, check whether pointer is in the allocation. */
       (callback_data->x >= widget->allocation.x &&
        callback_data->y >= widget->allocation.y &&
        callback_data->x < widget->allocation.x + widget->allocation.width &&
        callback_data->y < widget->allocation.y + DW_WIDGET_HEIGHT(widget))) {
      DEBUG_MSG (1, "    yes\n");
      if (DW_IS_CONTAINER (widget))
         a_Dw_container_forall (DW_CONTAINER (widget),
                                Dw_gtk_viewport_widget_at_point_callback,
                                data);

      if (callback_data->widget == NULL)
         callback_data->widget = widget;
   }
}

/*
 * Return the widget at point (x, y) (world coordinates).
 */
DwWidget* Dw_gtk_viewport_widget_at_point (GtkDwViewport *viewport,
                                           gint32 x,
                                           gint32 y)
{
   WidgetAtPointData callback_data;

   callback_data.x = x;
   callback_data.y = y;
   callback_data.widget = NULL;

   if (viewport->child)
      Dw_gtk_viewport_widget_at_point_callback (viewport->child,
                                                &callback_data);

   return callback_data.widget;
}


/*************
 *           *
 *  Anchors  *
 *           *
 *************/

/*
 * todo: Currently, no horizontal scrolling is done. This is generally
 * possible, DW_HPOS_INTO_VIEW should be used for this, but it is
 * rather complicated to determine the width of an anchor. This would
 * be the lenght of the region between <a> and </a>, the page widget
 * would have to have two kinds of content types (opening and closing
 * anchor), and some changes in the HTML parser are necessary.
 */

/*
 * Add an anchor, and assign a position for it. For all widgets
 * directly or indirectly assigned to a viewports, anchors must be
 * unique, this is tested. "name" is copied, so no strdup is
 * neccessary for the caller.
 *
 * Return the copy on success, or NULL, when this anchor had already
 * been added to the widget tree.
 *
 * The viewport gets the responsibility to free "name".
 */
gchar* p_Dw_gtk_viewport_add_anchor (DwWidget *widget,
                                     const gchar *name,
                                     gint32 y)
{
   GtkDwViewport *viewport;
   GtkDwViewportAnchor *anchor;

   _MSG("new anchor %p/'%s' -> %d\n", widget, name, y);

   g_return_val_if_fail (widget->viewport != NULL, NULL);
   viewport = GTK_DW_VIEWPORT (widget->viewport);

   if (g_hash_table_lookup_extended (viewport->anchors_table, name, NULL,NULL))
      /* Anchor does already exist. */
      return NULL;
   else {
      anchor = g_new (GtkDwViewportAnchor, 1);
      anchor->name = g_strdup (name);
      anchor->widget = widget;
      anchor->y = y;
      
      g_hash_table_insert (viewport->anchors_table, anchor->name, anchor);
      Dw_gtk_viewport_update_anchor (viewport);
      
      return anchor->name;
   }
}


/*
 * Assign a position for an already existing anchor.
 */
void p_Dw_gtk_viewport_change_anchor (DwWidget *widget,
                                      gchar *name,
                                      gint32 y)
{
   GtkDwViewport *viewport;
   GtkDwViewportAnchor *anchor;
   gpointer tmp_anchor;
   gboolean exists;

   _MSG("changing anchor %p/'%s' -> %d\n", widget, name, y);

   g_return_if_fail (widget->viewport != NULL);
   viewport = GTK_DW_VIEWPORT (widget->viewport);

   exists =
      g_hash_table_lookup_extended (viewport->anchors_table, name, NULL,
                                    &tmp_anchor);
   g_return_if_fail(exists);

   anchor = tmp_anchor;
   g_return_if_fail(anchor->widget == widget);

   anchor->y = y;

   Dw_gtk_viewport_update_anchor (viewport);
}


/*
 * Remove an anchor from the table in the viewport. Notice that "name"
 * is freed here.
 */
void p_Dw_gtk_viewport_remove_anchor (DwWidget *widget,
                                      gchar *name)
{
   GtkDwViewport *viewport;
   GtkDwViewportAnchor *anchor;
   gpointer tmp_anchor;
   gboolean exists;

   _MSG("removing anchor %p/'%s'\n", widget, name);

   g_return_if_fail (widget->viewport != NULL);
   viewport = GTK_DW_VIEWPORT (widget->viewport);

   exists = 
      g_hash_table_lookup_extended (viewport->anchors_table, name, NULL,
                                    &tmp_anchor);
   g_return_if_fail(exists);

   anchor = tmp_anchor;
   g_return_if_fail(anchor->widget == widget);
   
   g_hash_table_remove (viewport->anchors_table, name);
   g_free (anchor->name);
   g_free (anchor);
}


/*
 * Used by Dw_gtk_viewport_update_anchor_idle.
 */
static gboolean Dw_gtk_viewport_calc_into (gint32 requested_value,
                                           gint32 requested_size,
                                           gint32 current_value,
                                           gint32 size,
                                           gint32 *return_value)
{
   if (requested_size > size) {
      /* The viewport size is smaller than the size of the region which will
       * be shown. If the region is already visible, do not change the
       * position. Otherwise, show the left/upper border, this is most likely
       * what is needed. */
      if (current_value >= requested_value &&
          current_value + size < requested_value + requested_size)
         return FALSE;
      else
         requested_size = size;
   }

   if (requested_value < current_value) {
      *return_value = requested_value;
      return TRUE;
   } else if (requested_value + requested_size > current_value + size) {
      *return_value = requested_value - size + requested_size;
      return TRUE;
   } else
      return FALSE;
}

/*
 * See Dw_gtk_viewport_scroll_to.
 */
static gint Dw_gtk_viewport_update_anchor_idle (gpointer data)
{
   gint32 vp_width, vp_height, x = 0, y = 0;
   GtkScrolledWindow *scrolled;
   GtkDwViewport *viewport;
   GtkWidget *vwidget;
   GtkAdjustment *vadj, *hadj;
   gboolean change_x, change_y;

   DBG_MSG (data, "scrolling", 0, "Dw_gtk_viewport_update_anchor_idle");
   DBG_MSG_START (data);

   vwidget = GTK_WIDGET (data);
   viewport = GTK_DW_VIEWPORT (vwidget);
   scrolled = GTK_SCROLLED_WINDOW (vwidget->parent->parent);
   hadj = GTK_LAYOUT(viewport)->hadjustment;
   vadj = GTK_LAYOUT(viewport)->vadjustment;

   vp_width =
      vwidget->allocation.width - GTK_CONTAINER(viewport)->border_width;
   vp_height =
      vwidget->allocation.height - GTK_CONTAINER(viewport)->border_width;
   DBG_MSGF (viewport, "scrolling", 0, "vp_width = %d", vp_width);
   DBG_MSGF (viewport, "scrolling", 0, "vp_height = %d", vp_height);

   change_x = TRUE;
   switch (viewport->anchor_pos.hpos) {
   case DW_HPOS_LEFT:
      DBG_MSG (viewport, "scrolling", 0, "DW_HPOS_LEFT");
      x = viewport->anchor_pos.x;
      break;
   case DW_HPOS_CENTER:
      DBG_MSG (viewport, "scrolling", 0, "DW_HPOS_CENTER");
      x = viewport->anchor_pos.x - (vp_width - viewport->anchor_pos.width) / 2;
      break;
   case DW_HPOS_RIGHT:
      DBG_MSG (viewport, "scrolling", 0, "DW_HPOS_RIGHT");
      x = viewport->anchor_pos.x - (vp_width - viewport->anchor_pos.width);
      break;
   case DW_HPOS_INTO_VIEW:
      DBG_MSG (viewport, "scrolling", 0, "DW_HPOS_INTO_VIEW");
      change_x = Dw_gtk_viewport_calc_into (viewport->anchor_pos.x,
                                            viewport->anchor_pos.width,
                                            hadj->value, vp_width, &x);
      break;
   case DW_HPOS_NO_CHANGE:
      DBG_MSG (viewport, "scrolling", 0, "DW_HPOS_NO_CHANGE");
      change_x = FALSE;
      break;
   }

   change_y = TRUE;
   switch (viewport->anchor_pos.vpos) {
   case DW_VPOS_TOP:
      DBG_MSG (viewport, "scrolling", 0, "DW_VPOS_TOP");
      y = viewport->anchor_pos.y;
      break;
   case DW_VPOS_CENTER:
      DBG_MSG (viewport, "scrolling", 0, "DW_VPOS_CENTER");
      y = viewport->anchor_pos.y -
         (vp_height - viewport->anchor_pos.height) / 2;
      break;
   case DW_VPOS_BOTTOM:
      DBG_MSG (viewport, "scrolling", 0, "DW_VPOS_BOTTOM");
      y = viewport->anchor_pos.y - (vp_height - viewport->anchor_pos.height);
      break;
   case DW_VPOS_INTO_VIEW:
      DBG_MSG (viewport, "scrolling", 0, "DW_VPOS_INTO_VIEW");
      change_y = Dw_gtk_viewport_calc_into (viewport->anchor_pos.y,
                                            viewport->anchor_pos.height,
                                            vadj->value, vp_height, &y);
   case DW_VPOS_NO_CHANGE:
      DBG_MSG (viewport, "scrolling", 0, "DW_VPOS_NO_CHANGE");
      change_y = FALSE;
      break;
   }

   DBG_MSGF (viewport, "scrolling", 0, "scrolling to (%d, %d)\n", x, y);
   DBG_MSGF (viewport, "scrolling", 0,
             "hadj->upper = %g, hadj->page_size = %g",
             hadj->upper, hadj->page_size);
   DBG_MSGF (viewport, "scrolling", 0,
             "vadj->upper = %g, vadj->page_size = %g",
             vadj->upper, vadj->page_size);

   if (change_x) {
      if (x > hadj->upper - hadj->page_size)
         gtk_adjustment_set_value (hadj, hadj->upper - hadj->page_size);
      else
         gtk_adjustment_set_value (hadj, x);
   }

   if (change_y) {
      if (y > vadj->upper - vadj->page_size)
         gtk_adjustment_set_value (vadj, vadj->upper - vadj->page_size);
      else
         gtk_adjustment_set_value (vadj, y);
   }

   viewport->anchor_idle_id = 0;

   DBG_MSG_END (viewport);
   return FALSE;
}


/*
 * Called when possibly the scroll position has to be changed because
 * of anchors.
 */
void Dw_gtk_viewport_update_anchor (GtkDwViewport *viewport)
{
   GtkDwViewportAnchor *anchor;
   gpointer tmp_anchor;

   if (viewport->anchor &&
       g_hash_table_lookup_extended (viewport->anchors_table, viewport->anchor,
                                     NULL, &tmp_anchor)) {
      anchor = tmp_anchor;
      Dw_gtk_viewport_scroll_to (viewport, DW_HPOS_NO_CHANGE, DW_VPOS_TOP,
                                 0, anchor->y + anchor->widget->allocation.y,
                                 0, 0);
   }
}


/*
 * Sets the anchor to scroll to.
 */
void a_Dw_gtk_viewport_set_anchor (GtkDwViewport *viewport,
                                   const gchar *anchor)
{
   Dw_gtk_viewport_remove_anchor (viewport);

   if (anchor) {
      viewport->anchor = g_strdup (anchor);
      Dw_gtk_viewport_update_anchor (viewport);
   } else {
      viewport->anchor = NULL;
      gtk_adjustment_set_value (GTK_LAYOUT(viewport)->vadjustment, 0);
   }
}


/*
 * Sets the position to scroll to. The current anchor will be removed.
 */
void a_Dw_gtk_viewport_set_scrolling_position (GtkDwViewport *viewport,
                                               gint32 x,
                                               gint32 y)
{
   Dw_gtk_viewport_remove_anchor (viewport);
   Dw_gtk_viewport_scroll_to (viewport, DW_HPOS_LEFT, DW_VPOS_TOP, x, y, 0, 0);
}

/*
 * Scrolls the viewport, so that the region [x, y, width, height] (world
 * coordinates) is seen, according to hpos and vpos.
 *
 * The actual scrolling is done in an idle function.
 */
void Dw_gtk_viewport_scroll_to (GtkDwViewport *viewport,
                                DwHPosition hpos,
                                DwVPosition vpos,
                                gint32 x,
                                gint32 y,
                                gint32 width,
                                gint32 height)
{
   viewport->anchor_pos.hpos = hpos;
   viewport->anchor_pos.vpos = vpos;
   viewport->anchor_pos.x = x;
   viewport->anchor_pos.y = y;
   viewport->anchor_pos.width = width;
   viewport->anchor_pos.height = height;

   DBG_OBJ_SET_NUM (viewport, "anchor_pos.hpos", viewport->anchor_pos.hpos);
   DBG_OBJ_SET_NUM (viewport, "anchor_pos.vpos", viewport->anchor_pos.vpos);
   DBG_OBJ_SET_NUM (viewport, "anchor_pos.x", viewport->anchor_pos.x);
   DBG_OBJ_SET_NUM (viewport, "anchor_pos.y", viewport->anchor_pos.y);
   DBG_OBJ_SET_NUM (viewport, "anchor_pos.width", viewport->anchor_pos.width);
   DBG_OBJ_SET_NUM (viewport, "anchor_pos.height",
                    viewport->anchor_pos.height);

   if (viewport->anchor_idle_id == 0)
      viewport->anchor_idle_id = gtk_idle_add
         (Dw_gtk_viewport_update_anchor_idle, (gpointer)viewport);
}


/*
 * Remove anchor and idle function.
 */
void Dw_gtk_viewport_remove_anchor (GtkDwViewport *viewport)
{
   if (viewport->anchor) {
      g_free (viewport->anchor);
      viewport->anchor = NULL;
   }

   if (viewport->anchor_idle_id != 0) {
      gtk_idle_remove (viewport->anchor_idle_id);
      viewport->anchor_idle_id = 0;
   }
}

/*
 * Drawing and resizing is done in this idle function.
 */
static gint Dw_gtk_viewport_draw_resize_idle (gpointer data)
{
   GtkDwViewport *viewport;
   GtkLayout *layout;
   GtkWidget *widget;
   DwRectangle viewport_area, world_area;
   GdkRectangle gtk_area;
   int i;

   viewport = GTK_DW_VIEWPORT (data);

   switch (viewport->draw_resize_action) {
   case DW_GTK_VIEWPORT_DRAW:
      for (i = 0; i < viewport->num_draw_areas; i++) {
         widget = GTK_WIDGET (viewport);
         layout = GTK_LAYOUT (viewport);

         viewport_area.x = gtk_layout_get_hadjustment(layout)->value;
         viewport_area.y = gtk_layout_get_vadjustment(layout)->value;;
         viewport_area.width = widget->allocation.width;
         viewport_area.height = widget->allocation.height;

         if (p_Dw_rectangle_intersect (&viewport->draw_areas[i],
                                       &viewport_area, &world_area)) {
            gtk_area.x = world_area.x - viewport_area.x;
            gtk_area.y = world_area.y - viewport_area.y;
            gtk_area.width = world_area.width;
            gtk_area.height = world_area.height;
            gtk_widget_draw (widget, &gtk_area);
         }
      }

      /* No more areas to be drawn. */
      viewport->num_draw_areas = 0;
      break;

   case DW_GTK_VIEWPORT_RESIZE:
      Dw_gtk_viewport_calc_size (viewport);
      break;
   }

   viewport->draw_resize_idle_id = 0;
   return FALSE;
}

/*
 * Queue an area for drawing. This function is called by
 * p_Dw_widget_queue_draw_area. x and y are passed in world coordinates.
 */
void Dw_gtk_viewport_queue_draw (GtkDwViewport *viewport,
                                 gint32 x,
                                 gint32 y,
                                 gint32 width,
                                 gint32 height)
{
   DwRectangle area;
   int i;

   if (viewport->draw_resize_idle_id == 0) {
      viewport->draw_resize_action = DW_GTK_VIEWPORT_DRAW;
      viewport->draw_resize_idle_id =
         gtk_idle_add (Dw_gtk_viewport_draw_resize_idle, (gpointer)viewport);
   } else if (viewport->draw_resize_action == DW_GTK_VIEWPORT_RESIZE)
      /* Drawing is always overridden by resizing. */
      return;

   area.x = x;
   area.y = y;
   area.width = width;
   area.height = height;

   /* First, try to keep the list as clean as possible. Check whether other
    * rectangles interfer with this one in some way. */
   /* An idea for optimization: The list could be sorted, and so the part of
    * the list we have to consider here, may be reduced, the start may be
    * found via linear search. However, this probably makes balanced binary
    * trees necessary, since moving elements within the array may be quite
    * time-consuming.
    */
   _MSG("  num_draw_areas = %d\n", viewport->num_draw_areas);
   for (i = 0; i < viewport->num_draw_areas; i++) {
      if (p_Dw_rectangle_is_subset (&area, &viewport->draw_areas[i]))
         /* First case: area is a subset of an already queued rectangle
          * -> nothing to do. */
         return;
      else if (p_Dw_rectangle_is_subset (&viewport->draw_areas[i], &area)) {
         /* Second case: area is a subset of an already queued rectangle
          * -> replace the other one with area. */
         viewport->draw_areas[i] = area;
         return;
      }
      /* Maybe some more tests: if both areas may exactly be combined to a
       * rectangle? Very unlikely case ... */
   }

   /* No interference: add  the new area to the list. */
   viewport->num_draw_areas++;
   a_List_add (viewport->draw_areas, viewport->num_draw_areas,
               viewport->num_draw_areas_max);
   viewport->draw_areas[viewport->num_draw_areas - 1] = area;
}

/*
 * Start the resizing idle. This function is called by
 * p_Dw_widget_queue_resize, after the appropriate attributes have been set in
 * the widgets, where necessary.
 */
void Dw_gtk_viewport_queue_resize (GtkDwViewport *viewport)
{
   /* Resizing always overrides drawing. */
   viewport->draw_resize_action = DW_GTK_VIEWPORT_RESIZE;
   viewport->num_draw_areas = 0;

   if (viewport->draw_resize_idle_id == 0)
      viewport->draw_resize_idle_id =
         gtk_idle_add (Dw_gtk_viewport_draw_resize_idle, (gpointer)viewport);
}


/*
 * Return the DwWidget which is at position (vx, vy) in viewport coordinates.
 */
DwWidget* a_Dw_gtk_viewport_widget_at_viewport_point (GtkDwViewport *viewport,
                                                      gint32 vx,
                                                      gint32 vy)
{
   gint32 world_x, world_y;

   if (viewport->child) {
      world_x = vx + gtk_layout_get_hadjustment(GTK_LAYOUT(viewport))->value;
      world_y = vy + gtk_layout_get_vadjustment(GTK_LAYOUT(viewport))->value;
      return Dw_gtk_viewport_widget_at_point (viewport, world_x, world_y);
   } else
      return NULL;
}
