#include <stdint.h>
#include <string.h>
#include <gtk/gtk.h>
#include "votestuff.h"

/* reference object for lookup_widget() */
static GtkButton *btn_ref = NULL;

void vote_init (GtkButton *in_btn)
{
	btn_ref = in_btn;

	resetvote();
}

void update_txtfield (char *in_string)
{
	static char *tmp;
	GtkLabel *qfield;

	printf("L: %i\n", __LINE__);

	tmp = realloc (tmp, (sizeof(char) * strlen(in_string)) + 200);
	sprintf(tmp, "<span foreground=\"blue\" size=\"xx-large\">%s</span>", in_string);
	printf("L: %i\n", __LINE__);

	qfield = lookup_widget (GTK_WIDGET(btn_ref), "lbl_question");
	gtk_label_set_markup (qfield, tmp);
}

void setvote (uint8_t in_num, GtkButton *in_btn)
{
	GtkReliefStyle btnstyle = GTK_RELIEF_NORMAL;
	
	if (mymode == MODE_BLOCKED) return;
	if (votenum != NOT_VOTED && mymode != MODE_TEST) return;
	votenum = in_num;
	
	gtk_button_set_relief (in_btn, btnstyle);
}

uint8_t getvote (void)
{
	return votenum;
}

void countvote (void)
{
	if (getvote() == NOT_VOTED) return;

	votecounter[getvote()]++;

	printf("votecounter stats: %u %u %u %u\n",
		votecounter[0], votecounter[1], votecounter[2], votecounter[3]);
}

void resetvote ()
{
	GtkWidget *btns[4], *modebtn, *qfield;
	GtkReliefStyle btnstyle = GTK_RELIEF_NONE;
	
	votenum = NOT_VOTED;

	if (btn_ref == NULL) return;

	btns[0] = lookup_widget (GTK_WIDGET(btn_ref), "btn_c1");
	btns[1] = lookup_widget (GTK_WIDGET(btn_ref), "btn_c2");
	btns[2] = lookup_widget (GTK_WIDGET(btn_ref), "btn_c3");
	btns[3] = lookup_widget (GTK_WIDGET(btn_ref), "btn_c4");

	gtk_button_set_relief (btns[0], btnstyle);
	gtk_button_set_relief (btns[1], btnstyle);
	gtk_button_set_relief (btns[2], btnstyle);
	gtk_button_set_relief (btns[3], btnstyle);

	modebtn = lookup_widget (GTK_WIDGET(btn_ref), "btn_mode");
	mymode = MODE_TEST;
	gtk_button_set_label (modebtn, "Testmode");

	qfield = lookup_widget (GTK_WIDGET(btn_ref), "lbl_question");
	gtk_label_set_markup (qfield, "<span foreground=\"blue\" size=\"xx-large\">NAKKA!</span>");
}

void togglevotemode()
{
	GtkWidget *modebtn;

	modebtn = lookup_widget (GTK_WIDGET(btn_ref), "btn_mode");

	mymode++;
	if (mymode > MODE_BOTTOM) mymode = MODE_TEST;
	
	switch (mymode)
	{
		case MODE_TEST:
			gtk_button_set_label (modebtn, "Testmode");
		break;
		case MODE_RUN:
			gtk_button_set_label (modebtn, "Running...");
		break;
		case MODE_BLOCKED:
			gtk_button_set_label (modebtn, "BLOCKED");
		break;
		default:
			gtk_button_set_label (modebtn, "Mode unknown");
		break;
	}
}
