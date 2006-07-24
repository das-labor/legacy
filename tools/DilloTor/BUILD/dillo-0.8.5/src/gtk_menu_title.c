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
 * This is a widget with a very special purpose, for displaying titles within
 * menues. It is set to inactive (so it cannot be focussed), and the text is
 * centered horizontally.
 *
 * BTW, it is simple to change the look of menu titles, by putting the
 * following into your ~/.gtkrc file:
 *
 *    style "menu-title" = "default" {
 *      font = "-*-helvetica-medium-o-*-*-10-*-*-*-*-*-*-*"
 *    }
 *
 *    widget "*GtkMenuTitle" style "menu-title"
 */
#include "gtk_menu_title.h"

#define BORDER_SPACING  3

static void Gtk_menu_title_class_init   (GtkMenuTitleClass *klass);
static void Gtk_menu_title_init         (GtkMenuTitle *menu_title);

static void Gtk_menu_title_destroy      (GtkObject *object);
static void Gtk_menu_title_size_request (GtkWidget *widget,
                                         GtkRequisition *requisition);
static void Gtk_menu_title_draw         (GtkWidget *widget,
                                         GdkRectangle *area);
static gint Gtk_menu_title_expose_event (GtkWidget *widget,
                                         GdkEventExpose *event);

static GtkMenuItemClass *parent_class = NULL;


/*
 * Standard Gtk+ function.
 */
GtkType a_Gtk_menu_title_get_type (void)
{
  static GtkType type = 0;

  if (!type) {
    GtkTypeInfo info = {
      "GtkMenuTitle",
      sizeof (GtkMenuTitle),
      sizeof (GtkMenuTitleClass),
      (GtkClassInitFunc) Gtk_menu_title_class_init,
      (GtkObjectInitFunc) Gtk_menu_title_init,
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
static void Gtk_menu_title_class_init (GtkMenuTitleClass *klass)
{
   GtkWidgetClass *widget_class;
   GtkObjectClass *object_class;

   parent_class = (GtkMenuItemClass*) klass;

   object_class = (GtkObjectClass*) klass;
   object_class->destroy = Gtk_menu_title_destroy;

   widget_class = (GtkWidgetClass*) klass;
   widget_class->size_request = Gtk_menu_title_size_request;
   widget_class->draw = Gtk_menu_title_draw;
   widget_class->expose_event = Gtk_menu_title_expose_event;
}


/*
 * Standard Gtk+ function.
 */
static void Gtk_menu_title_init (GtkMenuTitle *menu_title)
{
   menu_title->label = NULL;
}


/*
 * Return a new GtkMenuTitle.
 */
GtkWidget* a_Gtk_menu_title_new (const char *label)
{
   GtkWidget *widget;

   widget = gtk_type_new (a_Gtk_menu_title_get_type ());
   GTK_MENU_TITLE(widget)->label = label ? g_strdup (label) : NULL;
   gtk_widget_set_sensitive (widget, FALSE);
   return widget;
}


/*
 * Standard Gtk+ function.
 */
static void Gtk_menu_title_destroy (GtkObject *object)
{
   GtkMenuTitle *menu_title;

   menu_title = GTK_MENU_TITLE (object);
   if (menu_title->label)
      g_free (menu_title->label);
}


/*
 * Standard Gtk+ function.
 */
static void Gtk_menu_title_size_request (GtkWidget *widget,
                                         GtkRequisition *requisition)
{
   GtkMenuTitle *menu_title;

   menu_title = GTK_MENU_TITLE (widget);
   requisition->width = 2 * (GTK_CONTAINER (widget)->border_width +
                             widget->style->klass->xthickness +
                             BORDER_SPACING);
   requisition->height = 2 * (GTK_CONTAINER (widget)->border_width +
                              widget->style->klass->ythickness);
   
   if (menu_title->label) {
      requisition->width +=
         gdk_string_width (widget->style->font, GTK_MENU_TITLE(widget)->label);
      requisition->height +=
         widget->style->font->ascent + widget->style->font->descent;
   }
}

/*
 * Standard Gtk+ function.
 */
static void Gtk_menu_title_paint (GtkWidget *widget,
                                  GdkRectangle *area)
{
   GtkMenuTitle *menu_title;
   gint x, y;

   menu_title = GTK_MENU_TITLE (widget);
   if (menu_title->label) {
      x =
         (widget->allocation.width - gdk_string_width (widget->style->font,
                                                       menu_title->label)) / 2;
      y =
         GTK_CONTAINER (widget)->border_width +
         widget->style->klass->ythickness
         + widget->style->font->ascent;

      /* We do not use widget->state, but instead GTK_STATE_NORMAL, since
       * otherwise, the text would be rendered gray. */
      gdk_draw_string (widget->window, widget->style->font,
                       widget->style->fg_gc[GTK_STATE_NORMAL], x, y,
                       menu_title->label);
   }
}

/*
 * Standard Gtk+ function.
 */
static void Gtk_menu_title_draw (GtkWidget *widget,
                                 GdkRectangle *area)
{
   if (GTK_WIDGET_DRAWABLE (widget))
      Gtk_menu_title_paint (widget, area);
}


/*
 * Standard Gtk+ function.
 */
static gint Gtk_menu_title_expose_event (GtkWidget *widget,
                                         GdkEventExpose *event)
{
   if (GTK_WIDGET_DRAWABLE (widget))
      Gtk_menu_title_paint (widget, &event->area);
   return TRUE;
}
