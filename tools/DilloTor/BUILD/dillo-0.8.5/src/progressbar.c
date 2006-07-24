/*
 * File: progressbar.c
 *
 * Copyright (C) 2004 Jorge Arellano Cid <jcid@dillo.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "progressbar.h"


/*
 * The progressbar is basically a GtkFrame with a text label.
 */
GtkWidget* a_Progressbar_new(void)
{
   GtkWidget *p_bar, *label;

   p_bar = gtk_frame_new(NULL);
   label = gtk_label_new("");
   gtk_frame_set_shadow_type(GTK_FRAME(p_bar), GTK_SHADOW_IN);
   gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.4);
   gtk_container_add(GTK_CONTAINER (p_bar), label);
   gtk_widget_show(label);
   return p_bar;
}

/*
 * Update the specified progress bar.
 *  updatestr : String to display within the bar (NULL is ignored)
 *  sens      : sensitivity (0 = set insensitive, 1 = set sensitive)
 */
void a_Progressbar_update(GtkWidget *p_bar, const char *updatestr, gint sens)
{
   gtk_widget_set_sensitive(p_bar, (sens == 0) ? FALSE : TRUE);

   if ( updatestr != NULL )
      gtk_label_set_text(GTK_LABEL(GTK_BIN(p_bar)->child), updatestr);
}

