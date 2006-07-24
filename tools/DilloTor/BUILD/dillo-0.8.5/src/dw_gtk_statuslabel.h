/* GTK - The GIMP Toolkit
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
#ifndef __GTK_STATUSLABEL_H__
#define __GTK_STATUSLABEL_H__


#include <gdk/gdk.h>
#include <gtk/gtkmisc.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GTK_STATUSLABEL(obj) \
   GTK_CHECK_CAST (obj, Dw_gtk_statuslabel_get_type (), GtkLabel)

#define GTK_STATUSLABEL_CLASS(klass) \
   GTK_CHECK_CLASS_CAST (klass, Dw_gtk_statuslabel_get_type (), GtkLabelClass)

#define GTK_IS_STATUSLABEL(obj) \
   GTK_CHECK_TYPE (obj, Dw_gtk_statuslabel_get_type ())


typedef struct _GtkStatusLabel       GtkStatusLabel;
typedef struct _GtkStatusLabelClass  GtkStatusLabelClass;

struct _GtkStatusLabel
{
  GtkLabel label;
};

struct _GtkStatusLabelClass
{
  GtkMiscClass parent_class;
};


guint      Dw_gtk_statuslabel_get_type (void);
GtkWidget* a_Dw_gtk_statuslabel_new      (char      *str);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_STATUSLABEL_H__ */
