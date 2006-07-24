/*
 * File: selection.c
 *
 * Copyright 2004 Sebastian Geerken <s.geerken@ping.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*
 * This Gtk+ widget is a variant of GtkButton, which adds two features:
 *
 *    1. the possibility to react on different mouse buttons (not only button
 *       1), and
 *    2. a clean way to attach menus.
 *
 * To archieve this use one of the following functions:
 *
 *    a_Gtk_ext_button_set_inactive,
 *    a_Gtk_ext_button_set_command,
 *    a_Gtk_ext_button_attach_menu, or
 *    a_Gtk_ext_button_attach_menu_creator.
 *
 * See comments there for more informations.
 *
 * About signals: For the command button mode, there are some new signals,
 * "clicked1", "clicked2", and "clicked3", which are emitted for the respective
 * mouse button. The signal "clicked" should be connected to as well, since
 * it is still used for non-mouse event handling, e.g. when then button is
 * activated by the <Enter> key.
 */

#include "gtk_ext_button.h"
#include <gtk/gtklabel.h>
#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>

static void Gtk_ext_button_class_init     (GtkExtButtonClass *klass);
static void Gtk_ext_button_init           (GtkExtButton *button);

static void Gtk_ext_button_destroy        (GtkObject *object);
static gint Gtk_ext_button_button_press   (GtkWidget *widget,
                                           GdkEventButton *event);
static gint Gtk_ext_button_button_release (GtkWidget *widget,
                                           GdkEventButton *event);
static gint Gtk_ext_button_enter_notify   (GtkWidget *widget,
                                           GdkEventCrossing *event);
static gint Gtk_ext_button_leave_notify   (GtkWidget *widget,
                                           GdkEventCrossing *event);
static void Gtk_ext_button_menu_hidden    (GtkExtButton *ext_button);

static gint clicked_signals[3];

/*
 * Create a new GtkExtButton with no child.
 */
GtkWidget *a_Gtk_ext_button_new ()
{
   return gtk_type_new (a_Gtk_ext_button_get_type ());
}

/*
 * Create a new GtkExtButton with a label.
 */
GtkWidget *a_Gtk_ext_button_new_with_label (const gchar *label)
{
   GtkWidget *button;
   GtkWidget *label_widget;
   
   button = a_Gtk_ext_button_new ();
   label_widget = gtk_label_new (label);
   gtk_misc_set_alignment (GTK_MISC (label_widget), 0.5, 0.5);
   
   gtk_container_add (GTK_CONTAINER (button), label_widget);
   gtk_widget_show (label_widget);
   
   return button;
}

/*
 * Standard Gtk+ function.
 */
guint a_Gtk_ext_button_get_type ()
{
   static gint type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "GtkExtButton",
         sizeof (GtkExtButton),
         sizeof (GtkExtButtonClass),
         (GtkClassInitFunc) Gtk_ext_button_class_init,
         (GtkObjectInitFunc) Gtk_ext_button_init,
         (GtkArgSetFunc)NULL,
         (GtkArgGetFunc)NULL,
         (GtkClassInitFunc)NULL
      };
      type = gtk_type_unique (gtk_button_get_type (), &info);
   }

   return type;
}

/*
 * Standard Gtk+ function.
 */
static void Gtk_ext_button_class_init(GtkExtButtonClass *klass)
{
   GtkObjectClass *object_class;
   GtkWidgetClass *widget_class;

   object_class = (GtkObjectClass*) klass;
   object_class->destroy = Gtk_ext_button_destroy;

   clicked_signals[0] =
      gtk_signal_new ("clicked1",
                      GTK_RUN_FIRST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (GtkExtButtonClass, clicked1),
                      gtk_marshal_NONE__NONE, GTK_TYPE_NONE, 0);
   clicked_signals[1] =
      gtk_signal_new ("clicked2",
                      GTK_RUN_FIRST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (GtkExtButtonClass, clicked2),
                      gtk_marshal_NONE__NONE, GTK_TYPE_NONE, 0);
   clicked_signals[2] =
      gtk_signal_new ("clicked3",
                      GTK_RUN_FIRST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (GtkExtButtonClass, clicked3),
                      gtk_marshal_NONE__NONE, GTK_TYPE_NONE, 0);
   gtk_object_class_add_signals (object_class, clicked_signals, 3);

   widget_class = (GtkWidgetClass*)klass;
   widget_class->button_press_event = Gtk_ext_button_button_press;
   widget_class->button_release_event = Gtk_ext_button_button_release;
   widget_class->enter_notify_event = Gtk_ext_button_enter_notify;
   widget_class->leave_notify_event = Gtk_ext_button_leave_notify;
}


/*
 * Standard Gtk+ function.
 */
static void Gtk_ext_button_init(GtkExtButton *button)
{
   int i;

   GTK_WIDGET_UNSET_FLAGS(button, GTK_CAN_FOCUS);

   button->pressed_button = 0;
   button->active_menu = NULL;
   button->menu_signal_id = -1;

   for(i = 0; i < 5; i++)
      button->action[i] = GTK_EXT_BUTTON_INACTIVE;
}


/*
 * Standard Gtk+ function.
 */
static void Gtk_ext_button_destroy (GtkObject *object)
{
   GtkExtButton *button;
   
   button = GTK_EXT_BUTTON (object);
   if (button->menu_signal_id != -1)
      gtk_signal_disconnect (GTK_OBJECT (button->active_menu),
                             button->menu_signal_id);
}


/*
 * This is used for gtk_menu_popup, to position the menu.
 */
static void Gtk_ext_button_position_menu (GtkMenu *menu,
                                          gint *x,
                                          gint *y,
                                          gpointer user_data)
{
   GtkWidget *widget;
   GtkRequisition requisition;

   widget = GTK_WIDGET (user_data);
   gdk_window_get_origin (widget->window, x, y);

   gtk_widget_size_request (GTK_WIDGET (menu), &requisition);
   if (*y + widget->allocation.height + requisition.height
       > gdk_screen_height())
      /* Show menu above button, since there is not enough space below. */
      *y -= requisition.height;
   else
      /* Show menu below button. */
      *y += widget->allocation.height;

   /* If the menu does not fit horizontilly, adjust position. */
   if (*x + requisition.width > gdk_screen_width ())
      *x = gdk_screen_width () - requisition.width;
}


/*
 * Standard Gtk+ function.
 */
static gint Gtk_ext_button_button_press (GtkWidget *widget,
                                         GdkEventButton *event)
{
   GtkButton *button;
   GtkExtButton *ext_button;
   GtkStateType new_state;
   GtkMenu *menu;
   
   button = GTK_BUTTON (widget);
   ext_button = GTK_EXT_BUTTON (widget);
   
   if (ext_button->pressed_button) {
      /* Already a button pressed. */
      return FALSE;
   } else if (event->button >= 1 && event->button <= 3 &&
              ext_button->action[event->button - 1]
              != GTK_EXT_BUTTON_INACTIVE) {
      ext_button->pressed_button = event->button;
      
      gtk_grab_add (widget);
      button->button_down = TRUE;
      new_state = (button->in_button ? GTK_STATE_ACTIVE : GTK_STATE_NORMAL);
      
      if (GTK_WIDGET_STATE (button) != new_state) {
         gtk_widget_set_state (GTK_WIDGET (button), new_state);
         gtk_widget_queue_draw (GTK_WIDGET (button));
      }

      menu = NULL;

      switch(ext_button->action[event->button - 1]) {
      case GTK_EXT_BUTTON_INACTIVE:
         g_assert_not_reached ();
         break;

      case GTK_EXT_BUTTON_COMMAND:
         /* Nothing to do anymore. */
         break;

      case GTK_EXT_BUTTON_MENU:
         menu = ext_button->action_data[event->button - 1].menu;
         break;
         
      case GTK_EXT_BUTTON_MENU_CREATOR:
         menu =
            ext_button->action_data[event->button - 1].creator.func (
               ext_button,
               ext_button->action_data[event->button - 1].creator.data);
         break;
      }
 
      if (menu) {
         ext_button->active_menu = menu;
         gtk_menu_popup (menu, NULL, widget, Gtk_ext_button_position_menu,
                         widget, event->button, event->time);
         ext_button->menu_signal_id =
            gtk_signal_connect_object (GTK_OBJECT (menu), "hide",
                                       GTK_SIGNAL_FUNC (
                                          Gtk_ext_button_menu_hidden),
                                       (gpointer) button);
      }
      
      return TRUE;
   } else
      return FALSE;
}


/*
 * Standard Gtk+ function.
 */
static gint Gtk_ext_button_button_release (GtkWidget *widget,
                                           GdkEventButton *event)
{
  GtkButton *button;
  GtkExtButton *ext_button;
  GtkStateType new_state;

  ext_button = GTK_EXT_BUTTON (widget);

  /* Only react on the button initially pressed. */
  if (event->button == ext_button->pressed_button) {
     button = GTK_BUTTON (widget);

     if (button->in_button)
        gtk_signal_emit (GTK_OBJECT (widget),
                         clicked_signals[event->button - 1]);

     gtk_grab_remove (widget);

     button->button_down = FALSE;
     ext_button->pressed_button = 0;
     new_state = (button->in_button ? GTK_STATE_PRELIGHT : GTK_STATE_NORMAL);

     if (GTK_WIDGET_STATE (button) != new_state) {
        gtk_widget_set_state (GTK_WIDGET (button), new_state);
        /* We _draw () instead of queue_draw so that if the operation
         * blocks, the label doesn't vanish.
         */
        gtk_widget_draw (GTK_WIDGET (button), NULL);
     }
  }

  return TRUE;
}


/*
 * Standard Gtk+ function.
 */
static gint Gtk_ext_button_enter_notify (GtkWidget *widget,
                                         GdkEventCrossing *event)
{
   GtkButton *button;
   GtkWidget *event_widget;
   GtkStateType new_state;
  
   button = GTK_BUTTON (widget);
   event_widget = gtk_get_event_widget ((GdkEvent*) event);
   
   if ((event_widget == widget) && (event->detail != GDK_NOTIFY_INFERIOR)) {
      button->in_button = TRUE;
      new_state =
         (button->button_down ? GTK_STATE_ACTIVE : GTK_STATE_PRELIGHT);

      if (GTK_WIDGET_STATE (button) != new_state) {
         gtk_widget_set_state (GTK_WIDGET (button), new_state);
         gtk_widget_queue_draw (GTK_WIDGET (button));
      }
   }

   return FALSE;
}


/*
 * Standard Gtk+ function.
 */
static gint Gtk_ext_button_leave_notify (GtkWidget *widget,
                                         GdkEventCrossing *event)
{
   GtkButton *button;
   GtkExtButton *ext_button;
   GtkWidget *event_widget;
   
   button = GTK_BUTTON (widget);
   event_widget = gtk_get_event_widget ((GdkEvent*) event);
 
   if ((event_widget == widget) &&
       (event->detail != GDK_NOTIFY_INFERIOR)) {
      button->in_button = FALSE;

      ext_button = GTK_EXT_BUTTON (widget);

      /* If a men is attached for this mouse button, we keep the button widget
       * inset. */
      if (ext_button->pressed_button == 0 ||
         ext_button->action[ext_button->pressed_button -1]
         == GTK_EXT_BUTTON_COMMAND) {
         /* Otherwise, normal behavior. */
         if (GTK_WIDGET_STATE (button) != GTK_STATE_NORMAL) {
            gtk_widget_set_state (GTK_WIDGET (button), GTK_STATE_NORMAL);
            gtk_widget_queue_draw (GTK_WIDGET (button));
         }
      }
   }

   return FALSE;
}

/*
 * This function is called, when a popped up menu is hidden again, to reset
 * the state of the button.
 */
static void Gtk_ext_button_menu_hidden (GtkExtButton *ext_button)
{
   GtkButton *button;

   g_return_if_fail (ext_button->menu_signal_id != -1);
   g_return_if_fail (ext_button->active_menu != NULL);
   gtk_signal_disconnect (GTK_OBJECT (ext_button->active_menu),
                          ext_button->menu_signal_id);
   ext_button->menu_signal_id = -1;
   ext_button->active_menu = NULL;

   gtk_grab_remove (GTK_WIDGET (ext_button));
   
   button = GTK_BUTTON (ext_button);
   button->in_button = FALSE;
   button->button_down = FALSE;
   ext_button->pressed_button = 0;
   
   gtk_widget_set_state (GTK_WIDGET (button), GTK_STATE_NORMAL);
   gtk_widget_queue_draw (GTK_WIDGET (button));
}


/*
 * Set no action for the specific mouse button. This is the default.
 */
void a_Gtk_ext_button_set_inactive (GtkExtButton *button,
                                    gint button_no)
{
   g_return_if_fail (button_no >= 1 && button_no <= 3);
   button->action[button_no - 1] = GTK_EXT_BUTTON_INACTIVE;
}


/*
 * Make the button behave like normal command button.
 */
void a_Gtk_ext_button_set_command (GtkExtButton *button,
                                   gint button_no)
{
   g_return_if_fail (button_no >= 1 && button_no <= 3);
   button->action[button_no - 1] = GTK_EXT_BUTTON_COMMAND;
}


/*
 * Attach a fixed menu to the button, which is popped up, when the user
 * presses the respective button.
 */
void a_Gtk_ext_button_attach_menu (GtkExtButton *button,
                                   gint button_no,
                                   GtkMenu *menu)
{
   g_return_if_fail (button_no >= 1 && button_no <= 3);
   button->action[button_no - 1] = GTK_EXT_BUTTON_MENU;
   button->action_data[button_no - 1].menu = menu;
}

/*
 * Attach a dynamically created menu to the button, which is popped up, when
 * the user presses the respective button.
 *
 * When the respective button has been pressed, the creator function is called
 * with the button, and the value of the argument data, the return value must
 * be the menu, which is then popped up.
 */
void a_Gtk_ext_button_attach_menu_creator (GtkExtButton *button,
                                           gint button_no,
                                           GtkExtButtonMenuCreator *creator,
                                           gpointer data)
{
   g_return_if_fail (button_no >= 1 && button_no <= 3);
   button->action[button_no - 1] = GTK_EXT_BUTTON_MENU_CREATOR;
   button->action_data[button_no - 1].creator.func = creator;
   button->action_data[button_no - 1].creator.data = data;
}
