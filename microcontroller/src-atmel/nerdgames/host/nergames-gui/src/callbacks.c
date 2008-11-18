#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "votestuff.h"


gboolean
key_press_handler                      (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data)
{

  return FALSE;
}



void
on_btn_c1_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{
	setvote (1, button);
}


void
on_btn_c2_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{
	setvote (2, button);
}


void
on_btn_c3_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{

	setvote (3, button);
}


void
on_btn_c4_clicked                      (GtkButton       *button,
                                        gpointer         user_data)
{

	setvote (4, button);
}


void
on_btn_backwards_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_btn_forward_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	static uint8_t mystate = 1;
	static uint_fast8_t init_done = 0;

	countvote();
	resetvote();
	
	init_done = (!init_done) ? 1 : 1;

	if (mystate == 1)
	{
		nextquestion();
		displayquestion();
		mystate = 0;
		gtk_button_set_label (button, "Solve");
	}
	else
	{
		gtk_button_set_label (button, "Next");
		resolvequestion();
		mystate = 1;
	}
}


void
on_btn_mode_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	togglevotemode();
}


void
on_btn_reset_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
	resetvote();

}

