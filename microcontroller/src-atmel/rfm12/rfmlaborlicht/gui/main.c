#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "usbstuff.h"

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

void on_gui_exit()
{
	gtk_main_quit ();
	exit(1);
}


int main (int argc, char *argv[])
{
	GtkBuilder      *builder; 
	GtkWidget       *window;

	gtk_init (&argc, &argv);
	usbstuff_init();

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "gui.glade", NULL);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "win_main"));
	gtk_builder_connect_signals (builder, NULL);

	g_object_unref (G_OBJECT (builder));

	gtk_widget_show (window);                
	gtk_main ();
	return 1;
}
