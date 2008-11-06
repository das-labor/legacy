#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <unistd.h>
#include <stdint.h>

#include <gtk/gtk.h>
#include <stdio.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "usbstuff.h"


void
on_addrfield_changed                   (GtkEditable     *editable,
                                        gpointer         user_data)
{

}


void
on_colorselection_color_changed        (GtkColorSelection *colorselection,
                                        gpointer         user_data)
{
	GdkColor *mycolor;

	mycolor = malloc(sizeof(GdkColor));

	gtk_color_selection_get_current_color (colorselection, mycolor);
	printf("Color: %02X%02X%02X \r\n", (uint8_t) (mycolor->red >> 8), (uint8_t) (mycolor->green >> 8), (uint8_t) (mycolor->blue >> 8));

	rlight_setcolor ((uint8_t) (mycolor->red >> 8), (uint8_t) (mycolor->green >> 8),
		(uint8_t) (mycolor->blue >> 8));
}


void
on_btn_commit_released                 (GtkButton       *button,
                                        gpointer         user_data)
{

}


gboolean
llctrl_exit                            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
	exit (1);
	return FALSE;
}


void
asjdjasd                               (GtkButton       *button,
                                        gpointer         user_data)
{

}

