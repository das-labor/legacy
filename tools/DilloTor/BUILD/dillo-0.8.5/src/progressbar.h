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
#ifndef __PROGRESSBAR_H__
#define __PROGRESSBAR_H__


#include <gdk/gdk.h>
#include <gtk/gtk.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PBAR_ISTR(s)  (s) ? "" : "Images: "
#define PBAR_PSTR(s)  (s) ? "" : "Page: "

GtkWidget* a_Progressbar_new(void);
void a_Progressbar_update(GtkWidget *pbar, const char *updatestr,
                                 gint op);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __PROGRESSBAR_H__ */

