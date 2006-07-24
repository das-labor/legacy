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
 * This widget, which is a variant of GtkMenu, works together with
 * GtkExtMenuItem. Together, they add the possibility to react on different
 * mouse buttons, for activating the menu items.
 *
 * If an item is activated by the mouse, the signals "activate1", "activate2",
 * and "activate3" are emitted. The signal "activate" should still be regarded,
 * since it is emitted (by the base classes), when an item was activated by
 * other input devices, e.g. the keyboard.
 *
 * Some notes about the implementation: Normally, mouse events are handled
 * by GtkMenu, not by GtkMenuItem. GtkMenu will call gtk_widget_activate(),
 * which emits the activate signal. For GtkMenuItem, this is "activate".
 *
 * GtkExtMenu changes dealing with release events a bit: if the affected
 * menu item is a GtkExtMenuItem, it is told to handle this event specially.
 * The GtkExtMenuItem will simply change its activate signal temporally. 
 *
 * (This is a bit ugly, but still cleaner than listening to the low-level
 * events, like "button-release-event".)
 */

#include "gtk_ext_menu.h"
#include "gtk_ext_menu_item.h"
#include <gtk/gtkmenuitem.h>
#include <gtk/gtkmain.h>
#include <gtk/gtksignal.h>

static void Gtk_ext_menu_class_init       (GtkExtMenuClass *klass);
static void Gtk_ext_menu_init             (GtkExtMenu *menu);

static gint Gtk_ext_menu_button_release   (GtkWidget *widget,
                                           GdkEventButton *event);

static GtkMenuClass *parent_class = NULL;


/*
 * Standard Gtk+ function.
 */
GtkType a_Gtk_ext_menu_get_type (void)
{
   static GtkType type = 0;

   if (!type) {
      GtkTypeInfo info = {
         "GtkExtMenu",
         sizeof (GtkExtMenu),
         sizeof (GtkExtMenuClass),
         (GtkClassInitFunc) Gtk_ext_menu_class_init,
         (GtkObjectInitFunc) Gtk_ext_menu_init,
         (GtkArgSetFunc) NULL,
         (GtkArgGetFunc) NULL,
         (GtkClassInitFunc)NULL
      };
      
      type = gtk_type_unique (gtk_menu_get_type (), &info);
   }
   
   return type;
}


/*
 * Standard Gtk+ function.
 */
static void Gtk_ext_menu_class_init (GtkExtMenuClass *klass)
{
   GtkWidgetClass *gtk_widget_class;
   
   parent_class = gtk_type_class (gtk_menu_get_type ());
   
   gtk_widget_class = (GtkWidgetClass*) klass;
   gtk_widget_class->button_release_event = Gtk_ext_menu_button_release;
}


/*
 * Standard Gtk+ function.
 */
static void Gtk_ext_menu_init (GtkExtMenu *menu)
{
}


/*
 * Create a new GtkExtMenu.
 */
GtkWidget* a_Gtk_ext_menu_new (void)
{
   return gtk_type_new (a_Gtk_ext_menu_get_type ());
}


/*
 * Copied from Gtk+ 1.2.10, file "menu_shell.c", function
 * gtk_menu_shell_is_item.
 */
static gint Gtk_ext_menu_is_item (GtkMenuShell *menu_shell,
                                  GtkWidget *child)
{
   GtkWidget *parent;

   g_return_val_if_fail (menu_shell != NULL, FALSE);
   g_return_val_if_fail (GTK_IS_MENU_SHELL (menu_shell), FALSE);
   g_return_val_if_fail (child != NULL, FALSE);
   
   parent = child->parent;
   while (parent && GTK_IS_MENU_SHELL (parent)) {
      if (parent == (GtkWidget*) menu_shell)
         return TRUE;
      parent = GTK_MENU_SHELL (parent)->parent_menu_shell;
   }

   return FALSE;
}

/*
 * Copied from Gtk+ 1.2.10, file "menu_shell.c", function
 * gtk_menu_shell_get_item.
 */
static GtkWidget* Gtk_ext_menu_get_item (GtkMenuShell *menu_shell,
                                         GdkEvent *event)
{
   GtkWidget *menu_item;
   
  menu_item = gtk_get_event_widget ((GdkEvent*) event);
  
  while (menu_item && !GTK_IS_MENU_ITEM (menu_item))
     menu_item = menu_item->parent;
  
  if (menu_item && Gtk_ext_menu_is_item (menu_shell, menu_item))
     return menu_item;
  else
     return NULL;
}


/*
 * Standard Gtk+ function.
 */
static gint Gtk_ext_menu_button_release (GtkWidget *widget,
                                         GdkEventButton *event)
{
   GtkWidget *menu_item;
   gint return_value;

   menu_item = Gtk_ext_menu_get_item (GTK_MENU_SHELL (widget),
                                      (GdkEvent*) event);
   if (menu_item != NULL && GTK_IS_EXT_MENU_ITEM (menu_item))
      p_Gtk_ext_menu_item_prepare_button_release (GTK_EXT_MENU_ITEM(menu_item),
                                                  event);
   return_value =
      GTK_WIDGET_CLASS(parent_class)->button_release_event (widget, event);
   if (menu_item != NULL && GTK_IS_EXT_MENU_ITEM (menu_item))
      p_Gtk_ext_menu_item_finish_button_release (GTK_EXT_MENU_ITEM (menu_item),
                                                 event);
   return return_value;
}
