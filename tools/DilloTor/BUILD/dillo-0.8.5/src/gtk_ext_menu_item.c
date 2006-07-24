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
 * See comments at the beginning of "gtk_ext_menu.c".
 */

#include "gtk_ext_menu_item.h"
#include <gtk/gtksignal.h>
#include <gtk/gtkaccellabel.h>

static void Gtk_ext_menu_item_class_init (GtkExtMenuItemClass *klass);
static void Gtk_ext_menu_item_init       (GtkExtMenuItem *item);

static gint std_activate_signal;
static gint activate_signals[3];

/*
 * Standard Gtk+ function.
 */
GtkType a_Gtk_ext_menu_item_get_type (void)
{
  static GtkType type = 0;

  if (!type) {
    GtkTypeInfo info = {
      "GtkExtMenuItem",
      sizeof (GtkExtMenuItem),
      sizeof (GtkExtMenuItemClass),
      (GtkClassInitFunc) Gtk_ext_menu_item_class_init,
      (GtkObjectInitFunc) Gtk_ext_menu_item_init,
      (GtkArgSetFunc) NULL,
      (GtkArgGetFunc) NULL,
      (GtkClassInitFunc)NULL
    };

    type = gtk_type_unique (gtk_menu_item_get_type (), &info);
  }

  return type;
}

/*
 * Standard Gtk+ function.
 */
static void Gtk_ext_menu_item_class_init (GtkExtMenuItemClass *klass)
{
   GtkObjectClass *object_class;

   object_class = (GtkObjectClass*) klass;
   activate_signals[0] =
      gtk_signal_new ("activate1",
                      GTK_RUN_FIRST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (GtkExtMenuItemClass, activate1),
                      gtk_marshal_NONE__NONE, GTK_TYPE_NONE, 0);
   activate_signals[1] =
      gtk_signal_new ("activate2",
                      GTK_RUN_FIRST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (GtkExtMenuItemClass, activate2),
                      gtk_marshal_NONE__NONE, GTK_TYPE_NONE, 0);
   activate_signals[2] =
      gtk_signal_new ("activate3",
                      GTK_RUN_FIRST | GTK_RUN_ACTION,
                      object_class->type,
                      GTK_SIGNAL_OFFSET (GtkExtMenuItemClass, activate3),
                      gtk_marshal_NONE__NONE, GTK_TYPE_NONE, 0);
   gtk_object_class_add_signals (object_class, activate_signals, 3);

   klass->activate1 = NULL;
   klass->activate2 = NULL;
   klass->activate3 = NULL;

   std_activate_signal =
      GTK_WIDGET_CLASS(gtk_type_class (gtk_menu_item_get_type ()))
      ->activate_signal;
}


/*
 * Standard Gtk+ function.
 */
static void Gtk_ext_menu_item_init (GtkExtMenuItem *item)
{
}

/*
 * Create a new, empty GtkExtMenuItem.
 */
GtkWidget *a_Gtk_ext_menu_item_new (void)
{
   return gtk_type_new (a_Gtk_ext_menu_item_get_type ());
}

/*
 * Create a GtkExtMenuItem with a label as child.
 */
GtkWidget *a_Gtk_ext_menu_item_new_with_label (const gchar *label)
{
  GtkWidget *menu_item;
  GtkWidget *accel_label;

  menu_item = a_Gtk_ext_menu_item_new ();
  accel_label = gtk_accel_label_new (label);
  gtk_misc_set_alignment (GTK_MISC (accel_label), 0.0, 0.5);

  gtk_container_add (GTK_CONTAINER (menu_item), accel_label);
  gtk_accel_label_set_accel_widget (GTK_ACCEL_LABEL (accel_label), menu_item);
  gtk_widget_show (accel_label);

  return menu_item;
}

/*
 * This method is called by GtkExtMenu, before handling a button release event.
 * See comments there.
 */
void p_Gtk_ext_menu_item_prepare_button_release (GtkExtMenuItem *item,
                                                 GdkEventButton *event)
{
   if (event->button >= 1 && event->button <= 3)
      GTK_WIDGET_CLASS(GTK_OBJECT(item)->klass)->activate_signal =
         activate_signals[event->button - 1];
}

/*
 * This method is called by GtkExtMenu, after handling a button release event.
 * See comments there.
 */
void p_Gtk_ext_menu_item_finish_button_release (GtkExtMenuItem *item,
                                              GdkEventButton *event)
{
   GTK_WIDGET_CLASS(GTK_OBJECT(item)->klass)->activate_signal =
      std_activate_signal;
}
