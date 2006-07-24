/*
 * File: dw_gtk_scrolled_frame.c
 *
 * Copyright (C) 2001  Sebastian Geerken <S.Geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * GtkDwScrolledFrame is a container for the GtkDwViewport, it adds
 * a focus around the child and is responsible for focus, keys and the
 * button 2 dragging.
 */

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "dw_gtk_scrolled_frame.h"

static GtkBinClass *parent_class = NULL;

/* object/class initialisation */
static void Dw_gtk_scrolled_frame_init          (GtkDwScrolledFrame *frame);
static void Dw_gtk_scrolled_frame_class_init    (GtkDwScrolledFrameClass
                                                 *klass);

/* GtkObject methods */
static void Dw_gtk_scrolled_frame_destroy       (GtkObject *object);

/* GtkWidget methods */
static void Dw_gtk_scrolled_frame_realize       (GtkWidget *widget);
static void Dw_gtk_scrolled_frame_unrealize     (GtkWidget *widget);
static void Dw_gtk_scrolled_frame_size_request  (GtkWidget *widget,
                                                 GtkRequisition *requisition);
static void Dw_gtk_scrolled_frame_size_allocate (GtkWidget *widget,
                                                 GtkAllocation *allocation);
static void Dw_gtk_scrolled_frame_draw          (GtkWidget *widget,
                                                 GdkRectangle *area);
static void Dw_gtk_scrolled_frame_draw_focus    (GtkWidget *widget);
static gint Dw_gtk_scrolled_frame_expose        (GtkWidget *widget,
                                                 GdkEventExpose *event);
static gint Dw_gtk_scrolled_frame_key_press     (GtkWidget *widget,
                                                 GdkEventKey *event);
static gint Dw_gtk_scrolled_frame_focus_in      (GtkWidget *widget,
                                                 GdkEventFocus *event);
static gint Dw_gtk_scrolled_frame_focus_out     (GtkWidget *widget,
                                                 GdkEventFocus *event);
static gint Dw_gtk_scrolled_frame_button_press  (GtkWidget *widget,
                                                 GdkEventButton *event);
static gint Dw_gtk_scrolled_frame_button_release(GtkWidget *widget,
                                                 GdkEventButton *event);
static gint Dw_gtk_scrolled_frame_motion_notify (GtkWidget *widget,
                                                 GdkEventMotion *event);

/* GtkContainer methods */
static void Dw_gtk_scrolled_frame_add           (GtkContainer *container,
                                                 GtkWidget *widget);
static gint Dw_gtk_scrolled_frame_focus         (GtkContainer *container,
                                                 GtkDirectionType direction);

/* GtkDwScrolledFrame methods */
static void Dw_gtk_scrolled_frame_set_scroll_adjustments (GtkDwScrolledFrame
                                                          *frame,
                                                          GtkAdjustment
                                                          *hadjustment,
                                                          GtkAdjustment
                                                          *vadjustment);


static void Dw_gtk_scrolled_frame_move_x_to (GtkDwScrolledFrame *frame,
                                             gfloat x);
static void Dw_gtk_scrolled_frame_move_y_to (GtkDwScrolledFrame *frame,
                                             gfloat y);
static void Dw_gtk_scrolled_frame_move_by (GtkDwScrolledFrame *frame,
                                           gfloat dx,
                                           gfloat dy);

static gfloat Dw_gtk_scrolled_frame_correct_adj (GtkAdjustment *adj,
                                                 gfloat pos);



enum
{
   SET_SCROLL_ADJUSTMENTS,
   USER_HCHANGED,
   USER_VCHANGED,
   LAST_SIGNAL
};

static guint frame_signals[LAST_SIGNAL] = { 0 };

/*
 * Standard Gtk+ function
 */
GtkType a_Dw_gtk_scrolled_frame_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "GtkDwScrolledFrame",
         sizeof (GtkDwScrolledFrame),
         sizeof (GtkDwScrolledFrameClass),
         (GtkClassInitFunc) Dw_gtk_scrolled_frame_class_init,
         (GtkObjectInitFunc) Dw_gtk_scrolled_frame_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc) NULL
      };

      type = gtk_type_unique (GTK_TYPE_BIN, &info);
   }

   return type;
}


/*
 * Standard Gtk+ function
 */
GtkWidget* a_Dw_gtk_scrolled_frame_new (GtkAdjustment *hadjustment,
                                        GtkAdjustment *vadjustment)
{
   GtkWidget *widget;
   GtkDwScrolledFrame *frame;

   widget = gtk_widget_new (GTK_TYPE_DW_SCROLLED_FRAME, NULL);

   frame = GTK_DW_SCROLLED_FRAME (widget);
   frame->hadjustment = hadjustment;
   frame->vadjustment = vadjustment;

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
static void Dw_gtk_scrolled_frame_init (GtkDwScrolledFrame *frame)
{
   GTK_WIDGET_SET_FLAGS (frame, GTK_CAN_FOCUS);
   GTK_WIDGET_UNSET_FLAGS (frame, GTK_NO_WINDOW);

   frame->hadjustment = NULL;
   frame->vadjustment = NULL;
   frame->button2_pressed = FALSE;
   frame->move_idle_id = 0;
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_frame_class_init (GtkDwScrolledFrameClass *klass)
{
   GtkObjectClass *object_class;
   GtkWidgetClass *widget_class;
   GtkContainerClass *container_class;

   parent_class = gtk_type_class (gtk_bin_get_type ());

   object_class = (GtkObjectClass*) klass;
   widget_class = (GtkWidgetClass*) klass;
   container_class = (GtkContainerClass*) klass;

   frame_signals[SET_SCROLL_ADJUSTMENTS] =
      gtk_signal_new ("set_scroll_adjustments",
                      GTK_RUN_LAST,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (GtkDwScrolledFrameClass,
                                         set_scroll_adjustments),
                      gtk_marshal_NONE__POINTER_POINTER,
                      GTK_TYPE_NONE, 2, GTK_TYPE_ADJUSTMENT,
                      GTK_TYPE_ADJUSTMENT);
   widget_class->set_scroll_adjustments_signal =
      frame_signals[SET_SCROLL_ADJUSTMENTS];

   frame_signals[USER_HCHANGED] =
      gtk_signal_new ("user_hchanged",
                      GTK_RUN_LAST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (GtkDwScrolledFrameClass,
                                         user_hchanged),
                      gtk_marshal_NONE__NONE,
                      GTK_TYPE_NONE, 0);
   frame_signals[USER_VCHANGED] =
      gtk_signal_new ("user_vchanged",
                      GTK_RUN_LAST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (GtkDwScrolledFrameClass,
                                         user_vchanged),
                      gtk_marshal_NONE__NONE,
                      GTK_TYPE_NONE, 0);

   gtk_object_class_add_signals (object_class, frame_signals, LAST_SIGNAL);

   object_class->destroy = Dw_gtk_scrolled_frame_destroy;

   widget_class->realize = Dw_gtk_scrolled_frame_realize;
   widget_class->unrealize = Dw_gtk_scrolled_frame_unrealize;
   widget_class->size_request = Dw_gtk_scrolled_frame_size_request;
   widget_class->size_allocate = Dw_gtk_scrolled_frame_size_allocate;
   widget_class->draw_focus = Dw_gtk_scrolled_frame_draw_focus;
   widget_class->draw = Dw_gtk_scrolled_frame_draw;
   widget_class->expose_event = Dw_gtk_scrolled_frame_expose;
   widget_class->key_press_event = Dw_gtk_scrolled_frame_key_press;
   widget_class->focus_in_event = Dw_gtk_scrolled_frame_focus_in;
   widget_class->focus_out_event = Dw_gtk_scrolled_frame_focus_out;
   widget_class->button_press_event = Dw_gtk_scrolled_frame_button_press;
   widget_class->button_release_event = Dw_gtk_scrolled_frame_button_release;
   widget_class->motion_notify_event = Dw_gtk_scrolled_frame_motion_notify;

   container_class->add = Dw_gtk_scrolled_frame_add;
   container_class->focus = Dw_gtk_scrolled_frame_focus;

   klass->set_scroll_adjustments =
      Dw_gtk_scrolled_frame_set_scroll_adjustments;
   klass->user_hchanged = NULL;
   klass->user_vchanged = NULL;
}


/***********************
 *                     *
 *  GtkObject methods  *
 *                     *
 ***********************/

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_frame_destroy (GtkObject *object)
{
   GtkDwScrolledFrame *frame;

   frame = GTK_DW_SCROLLED_FRAME (object);
   if (frame->move_idle_id != 0)
      gtk_idle_remove (frame->move_idle_id);

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
static void Dw_gtk_scrolled_frame_realize (GtkWidget *widget)
{
   GdkWindowAttr attributes;

   GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED);

   attributes.window_type = GDK_WINDOW_CHILD;
   attributes.x = widget->allocation.x;
   attributes.y = widget->allocation.y;
   attributes.width = widget->allocation.width;
   attributes.height = widget->allocation.height;
   attributes.wclass = GDK_INPUT_OUTPUT;
   attributes.visual = gtk_widget_get_visual (widget);
   attributes.colormap = gtk_widget_get_colormap (widget);
   attributes.event_mask = (gtk_widget_get_events (widget)
                            | GDK_EXPOSURE_MASK
                            | GDK_KEY_PRESS_MASK
                            | GDK_BUTTON_PRESS_MASK);

   widget->window = gdk_window_new (gtk_widget_get_parent_window (widget),
                                    &attributes,
                                    GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL |
                                    GDK_WA_COLORMAP);
   gdk_window_set_user_data (widget->window, widget);
   widget->style = gtk_style_attach (widget->style, widget->window);
   gtk_style_set_background (widget->style, widget->window, GTK_STATE_NORMAL);

   GTK_DW_SCROLLED_FRAME(widget)->drag_cursor = gdk_cursor_new (GDK_FLEUR);
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_frame_unrealize (GtkWidget *widget)
{
   gdk_cursor_destroy (GTK_DW_SCROLLED_FRAME(widget)->drag_cursor);
   GTK_WIDGET_CLASS(parent_class)->unrealize (widget);
}

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_frame_size_allocate (GtkWidget *widget,
                                                 GtkAllocation *allocation)
{
   GtkBin *bin;
   gint border_width;
   GtkAllocation child_allocation;

   widget->allocation = *allocation;
   bin = GTK_BIN (widget);

   if (GTK_WIDGET_REALIZED (widget))
      gdk_window_move_resize (widget->window,
                              allocation->x, allocation->y,
                              allocation->width, allocation->height);

   if (bin->child) {
      border_width = GTK_CONTAINER(widget)->border_width;

      child_allocation.width =
         MAX (allocation->width - 2 * (widget->style->klass->xthickness +
                                       border_width),
              1);
      child_allocation.height =
         MAX (allocation->height - 2 * (widget->style->klass->ythickness +
                                        border_width),
              1);
      child_allocation.x = (allocation->width - child_allocation.width) / 2;
      child_allocation.y = (allocation->height - child_allocation.height) / 2;

      gtk_widget_size_allocate (bin->child, &child_allocation);
   }
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_frame_size_request (GtkWidget *widget,
                                                GtkRequisition *requisition)
{
   GtkBin *bin;
   gint border_width;
   GtkRequisition child_requisition;

   bin = GTK_BIN (widget);

   if (bin->child) {
      border_width = GTK_CONTAINER(widget)->border_width;
      gtk_widget_size_request (bin->child, &child_requisition);

      requisition->width =
         child_requisition.width + 2 * (widget->style->klass->xthickness +
                                        border_width);
      requisition->height =
         child_requisition.height + 2 * (widget->style->klass->ythickness +
                                         border_width);
   } else {
      requisition->width = 100;
      requisition->height = 100;
   }
}


/*
 * Draw the frame, eventually with a focus border
 */
static void Dw_gtk_scrolled_frame_paint_shadow (GtkWidget *widget,
                                             GdkRectangle *area)
{
   gint border_width;

   if (GTK_WIDGET_DRAWABLE (widget)) {
      border_width = GTK_CONTAINER(widget)->border_width;

      if (GTK_WIDGET_HAS_FOCUS (widget)) {
         gtk_draw_shadow (widget->style, widget->window,
                          GTK_STATE_NORMAL, GTK_SHADOW_IN,
                          border_width + 1, border_width + 1,
                          widget->allocation.width - 2 * (border_width + 1),
                          widget->allocation.height - 2 * (border_width + 1));

         /* "text" is probably next to what we need */
         gtk_paint_focus (widget->style, widget->window,
                          area, widget, "text",
                          border_width, border_width,
                          widget->allocation.width - 2 * border_width - 1,
                          widget->allocation.height - 2 * border_width - 1);
      } else {
         gtk_draw_shadow (widget->style, widget->window,
                          GTK_STATE_NORMAL, GTK_SHADOW_IN,
                          border_width, border_width,
                          widget->allocation.width - 2 * border_width,
                          widget->allocation.height - 2 * border_width);
      }
   }
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_frame_draw_focus (GtkWidget *widget)
{
   Dw_gtk_scrolled_frame_paint_shadow (widget, NULL);
}


/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_frame_draw (GtkWidget *widget,
                                        GdkRectangle *area)
{
   GTK_WIDGET_CLASS(parent_class)->draw (widget, area);
   Dw_gtk_scrolled_frame_paint_shadow (widget, area);
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_scrolled_frame_expose (GtkWidget *widget,
                                          GdkEventExpose *event)
{
   gint ret_val;

   ret_val = GTK_WIDGET_CLASS(parent_class)->expose_event (widget, event);
   Dw_gtk_scrolled_frame_paint_shadow (widget, &(event->area));

   return ret_val;
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_scrolled_frame_key_press (GtkWidget *widget,
                                             GdkEventKey *event)
{
   GtkContainer *container;
   GtkDwScrolledFrame *frame;

   container = GTK_CONTAINER (widget);
   frame = GTK_DW_SCROLLED_FRAME (widget);

   switch (event->keyval) {
   case GDK_Up:
   case GDK_KP_Up:
      Dw_gtk_scrolled_frame_move_by (frame, 0,
                                     - frame->vadjustment->step_increment);
      return TRUE;

   case GDK_Down:
   case GDK_KP_Down:
      Dw_gtk_scrolled_frame_move_by (frame, 0,
                                     + frame->vadjustment->step_increment);
      return TRUE;

   case GDK_Left:
   case GDK_KP_Left:
      Dw_gtk_scrolled_frame_move_by (frame,
                                     - frame->hadjustment->step_increment, 0);
      return TRUE;

   case GDK_Right:
   case GDK_KP_Right:
      Dw_gtk_scrolled_frame_move_by (frame,
                                     + frame->hadjustment->step_increment, 0);
      return TRUE;

   case GDK_Tab:
      if (event->state & GDK_SHIFT_MASK)
         return gtk_container_focus (container, GTK_DIR_TAB_BACKWARD);
      else
         return gtk_container_focus (container, GTK_DIR_TAB_FORWARD);

   case GDK_b: case GDK_B:
      if (event->state & GDK_CONTROL_MASK)
         return FALSE;
      /* continues */
   case GDK_Page_Up:
   case GDK_KP_Page_Up:
      if (event->state & GDK_CONTROL_MASK)
         Dw_gtk_scrolled_frame_move_by (frame,
                                        - frame->hadjustment->page_increment,
                                        0);
      else
         Dw_gtk_scrolled_frame_move_by (frame, 0,
                                        - frame->vadjustment->page_increment);
      return TRUE;

   case GDK_Page_Down:
   case GDK_KP_Page_Down:
   case GDK_space:
      if (event->state & GDK_CONTROL_MASK)
         Dw_gtk_scrolled_frame_move_by (frame,
                                        + frame->hadjustment->page_increment,
                                        0);
      else
         Dw_gtk_scrolled_frame_move_by (frame, 0,
                                        + frame->vadjustment->page_increment);
      return TRUE;

   case GDK_Home:
   case GDK_KP_Home:
      if (event->state & GDK_CONTROL_MASK)
         Dw_gtk_scrolled_frame_move_x_to (frame, 0);
      else
         Dw_gtk_scrolled_frame_move_y_to (frame, 0);
      return TRUE;

   case GDK_End:
   case GDK_KP_End:
      if (event->state & GDK_CONTROL_MASK)
         Dw_gtk_scrolled_frame_move_x_to (frame, G_MAXFLOAT);
      else
         Dw_gtk_scrolled_frame_move_y_to (frame, G_MAXFLOAT);
      return TRUE;

   default:
      return FALSE;
   }
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_scrolled_frame_focus_in (GtkWidget *widget,
                                            GdkEventFocus *event)
{
   GTK_WIDGET_SET_FLAGS (widget, GTK_HAS_FOCUS);
   gtk_widget_draw_focus (widget);
   return FALSE;
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_scrolled_frame_focus_out (GtkWidget *widget,
                                             GdkEventFocus *event)
{
   GTK_WIDGET_UNSET_FLAGS (widget, GTK_HAS_FOCUS);
   gtk_widget_draw_focus (widget);
   return FALSE;
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_scrolled_frame_button_press (GtkWidget *widget,
                                                GdkEventButton *event)
{
   GtkDwScrolledFrame *frame;

   if (!GTK_WIDGET_HAS_FOCUS (widget))
      gtk_widget_grab_focus (widget);

   if (event->button == 2) {
      frame = GTK_DW_SCROLLED_FRAME (widget);

      frame->button2_pressed = TRUE;
      frame->start_lmx = event->x;
      frame->start_lmy = event->y;

      gdk_pointer_grab (widget->window, FALSE,
                        GDK_BUTTON_RELEASE_MASK | GDK_BUTTON2_MOTION_MASK,
                        NULL, frame->drag_cursor, event->time);

   }

   return TRUE;
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_scrolled_frame_button_release (GtkWidget *widget,
                                                  GdkEventButton *event)
{
   GTK_DW_SCROLLED_FRAME(widget)->button2_pressed = FALSE;
   gdk_pointer_ungrab (event->time);
   return TRUE;
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_scrolled_frame_motion_notify (GtkWidget *widget,
                                                 GdkEventMotion *event)
{
   GtkDwScrolledFrame *frame = GTK_DW_SCROLLED_FRAME (widget);

   /* frame->button2_pressed makes sure that the button was pressed
      in the GtkDwScrolledFrame */
   if ((event->state & GDK_BUTTON2_MASK) && frame->button2_pressed) {
      Dw_gtk_scrolled_frame_move_by (frame,
                                     frame->start_lmx - event->x,
                                     frame->start_lmy - event->y);

      frame->start_lmx = event->x;
      frame->start_lmy = event->y;
   }

   return TRUE;
}


/**************************
 *                        *
 *  GtkContainer methods  *
 *                        *
 **************************/

/*
 * Standard Gtk+ function
 */
static void Dw_gtk_scrolled_frame_add (GtkContainer *container,
                                       GtkWidget *widget)
{
   GtkDwScrolledFrame *frame;

   GTK_CONTAINER_CLASS(parent_class)->add (container, widget);

   frame = GTK_DW_SCROLLED_FRAME (container);
   if ( !gtk_widget_set_scroll_adjustments (
           GTK_WIDGET (container), frame->hadjustment, frame->vadjustment) )
      g_warning ("Dw_gtk_scrolled_frame_set_scroll_adjustments(): "
                 "child is non scrollable");
}


/*
 * Standard Gtk+ function
 */
static gint Dw_gtk_scrolled_frame_focus (GtkContainer *container,
                                         GtkDirectionType direction)
{
   /* todo:
    * This crashed if the child is not a container (what will never happen
    * in dillo, where GtkDwScrolledFrame always contains a GtkDwViewport).
    */
   GtkDwScrolledFrame *frame;

   frame = GTK_DW_SCROLLED_FRAME (container);

   if (!GTK_WIDGET_HAS_FOCUS (container) && container->focus_child == NULL){
      // no focus on the GtkDwScrolledFrame or any child widget => grab focus!
      gtk_widget_grab_focus (GTK_WIDGET (container));
      return TRUE;
   }

   switch (direction) {
   case GTK_DIR_TAB_FORWARD:
      /* deliver to child */
      if (!gtk_container_focus (GTK_CONTAINER (GTK_BIN(container)->child),
                                direction)) {
         /* todo: it would be nice to keep focus on this last child widget,
          * instead of the container... Anyway this may change with GTK2 */
         gtk_widget_grab_focus (GTK_WIDGET (container));
      }
      return TRUE;

   case GTK_DIR_TAB_BACKWARD:
      if (GTK_WIDGET_HAS_FOCUS (container)) {
         /* will focus the widget "before" */
         return FALSE;
      } else {
         if (!gtk_container_focus (GTK_CONTAINER (GTK_BIN(container)->child),
                                   GTK_DIR_TAB_BACKWARD))
            /* first child of the child unfocussed */
            gtk_widget_grab_focus (GTK_WIDGET (container));

         return TRUE;
      }
      break;

   /*
   case GTK_DIR_LEFT:
   case GTK_DIR_RIGHT:
   case GTK_DIR_UP:
   case GTK_DIR_DOWN:
   */
   default:
      /* focus the GtkDwScrolledFrame */
      gtk_widget_grab_focus (GTK_WIDGET (container));
      return TRUE;
   }

   /* make compiler happy */
   return FALSE;
}


/********************************
 *                              *
 *  GtkDwScrolledFrame methods  *
 *                              *
 ********************************/

/*
 * Standard Gtk+ function
 */
static void
Dw_gtk_scrolled_frame_set_scroll_adjustments (GtkDwScrolledFrame *frame,
                                              GtkAdjustment *hadjustment,
                                              GtkAdjustment *vadjustment)
{
   GtkBin *bin;

   frame->hadjustment = hadjustment;
   frame->vadjustment = vadjustment;

   bin = GTK_BIN (frame);
   if (bin->child) {
      if (!gtk_widget_set_scroll_adjustments (bin->child,
                                              hadjustment,
                                              vadjustment))
         g_warning ("Dw_gtk_scrolled_frame_set_scroll_adjustments(): "
                    "child is non scrollable");
   }
}


/*
 * Functions for moving the viewport. This is queued and done in an
 * idle function, to prevent locks by too frequent keypresses, or too
 * fast mouse movements. (How to reproduce: start dillo on a slow,
 * overloaded system with nice priority.)
 */

/*
 * The idle function.
 */
static gint Dw_gtk_scrolled_frame_move_idle (gpointer data)
{
   GtkDwScrolledFrame *frame;

   frame = GTK_DW_SCROLLED_FRAME (data);

   g_return_val_if_fail (frame->hadjustment != NULL, FALSE);
   g_return_val_if_fail (frame->vadjustment != NULL, FALSE);

   if (frame->moveto_x != frame->hadjustment->value) {
      gtk_adjustment_set_value (frame->hadjustment, frame->moveto_x);
      gtk_signal_emit (GTK_OBJECT (frame), frame_signals[USER_HCHANGED]);
   }

   if (frame->moveto_y != frame->vadjustment->value) {
      gtk_adjustment_set_value (frame->vadjustment, frame->moveto_y);
      gtk_signal_emit (GTK_OBJECT (frame), frame_signals[USER_VCHANGED]);
   }

   frame->move_idle_id = 0;
   return FALSE;
}


/*
 * Change horizontall position to x.
 */
static void Dw_gtk_scrolled_frame_move_x_to (GtkDwScrolledFrame *frame,
                                             gfloat x)
{
   frame->moveto_x =
      Dw_gtk_scrolled_frame_correct_adj (frame->hadjustment, x);
   if (frame->move_idle_id == 0)
      frame->move_idle_id = gtk_idle_add (Dw_gtk_scrolled_frame_move_idle,
                                          (gpointer) frame);
}


/*
 * Change vertical position to y.
 */
static void Dw_gtk_scrolled_frame_move_y_to (GtkDwScrolledFrame *frame,
                                             gfloat y)
{
   frame->moveto_y =
      Dw_gtk_scrolled_frame_correct_adj (frame->vadjustment, y);
   if (frame->move_idle_id == 0)
      frame->move_idle_id = gtk_idle_add (Dw_gtk_scrolled_frame_move_idle,
                                          (gpointer) frame);
}


/*
 * Move viewport by dx and dy.
 */
static void Dw_gtk_scrolled_frame_move_by (GtkDwScrolledFrame *frame,
                                           gfloat dx,
                                           gfloat dy)
{
   if (frame->move_idle_id == 0) {
      /*
       * Initialization: if the idle function is not active, set
       * moveto_x and moveto_y to adjustments values, which may
       * already have been changed by the scrollbars. This code does
       * not work when a user uses scrollbars and keys at the same
       * time. ;-)
       */
      frame->moveto_x = frame->hadjustment->value;
      frame->moveto_y = frame->vadjustment->value;
   }

   frame->moveto_x =
      Dw_gtk_scrolled_frame_correct_adj (frame->hadjustment,
                                         frame->moveto_x + dx);
   frame->moveto_y =
      Dw_gtk_scrolled_frame_correct_adj (frame->vadjustment,
                                         frame->moveto_y + dy);

   if (frame->move_idle_id == 0)
      frame->move_idle_id = gtk_idle_add (Dw_gtk_scrolled_frame_move_idle,
                                          (gpointer) frame);

}


/*
 * Helper function: Corrects pos to fit into the boundaries of adj.
 */
static gfloat Dw_gtk_scrolled_frame_correct_adj (GtkAdjustment *adj,
                                                 gfloat pos)
{
   if (pos < adj->lower)
      return adj->lower;
   else if (pos > adj->upper - adj->page_size)
      return adj->upper - adj->page_size;
   else
      return pos;
}
