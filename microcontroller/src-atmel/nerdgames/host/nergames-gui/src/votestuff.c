#include <stdint.h>
#include <string.h>
#include <gtk/gtk.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "votestuff.h"

/* reference object for lookup_widget() */
static GtkButton *btn_ref = NULL;
static question_t *entryq, *currentq = NULL;

void vote_init (GtkButton *in_btn)
{
	GtkButton *modebtn;
	btn_ref = in_btn;
	
	modebtn = (GtkButton *) lookup_widget (GTK_WIDGET(btn_ref), "btn_mode");
	mymode = MODE_TEST;
	gtk_button_set_label (modebtn, "Testmode");

	resetvote();
	read_questions("questions.txt");
}

void update_txtfield (char *in_string)
{
	static char *tmp;
	GtkLabel *qfield;

	tmp = realloc (tmp, (sizeof(char) * strlen(in_string)) + 200);
	sprintf(tmp, "<span foreground=\"darkblue\" size=\"xx-large\">%s</span>", in_string);

	qfield = (GtkButton *) lookup_widget (GTK_WIDGET(btn_ref), "lbl_question");
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

	btns[0] = (GtkButton*) lookup_widget (GTK_WIDGET(btn_ref), "btn_c1");
	btns[1] = (GtkButton*) lookup_widget (GTK_WIDGET(btn_ref), "btn_c2");
	btns[2] = (GtkButton*) lookup_widget (GTK_WIDGET(btn_ref), "btn_c3");
	btns[3] = (GtkButton*) lookup_widget (GTK_WIDGET(btn_ref), "btn_c4");

	gtk_button_set_relief (btns[0], btnstyle);
	gtk_button_set_relief (btns[1], btnstyle);
	gtk_button_set_relief (btns[2], btnstyle);
	gtk_button_set_relief (btns[3], btnstyle);

	qfield = (GtkButton*) lookup_widget (GTK_WIDGET(btn_ref), "lbl_question");
}

void togglevotemode()
{
	GtkWidget *modebtn;

	modebtn = (GtkButton*) lookup_widget (GTK_WIDGET(btn_ref), "btn_mode");

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

question_t *nextquestion (void)
{
	if (currentq->next == NULL) return NULL; /* end of line */

	if (currentq == NULL)
	{
		currentq = entryq;
	} else
	{
		currentq = currentq->next;
	}

	return currentq;
}

int startquest (void)
{
	currentq = entryq;
}

int displayquestion (void)
{
	GtkImage *wimage = NULL;
	GtkLabel *qfield = NULL;

	if (currentq == NULL) return -1;

	/* set image */
	wimage = (GtkButton*) lookup_widget (GTK_WIDGET(btn_ref), "img_main");
	
	printf("image: %s\n", currentq->image);
	if (currentq->ilen > 0)
		gtk_image_set_from_file (wimage, currentq->image);
	else
		gtk_image_set_from_file (wimage, NULL);

	/* set text */
	qfield = (GtkButton*) lookup_widget (GTK_WIDGET(btn_ref), "lbl_question");
	
	if (currentq->qlen == 0)
	{
		gtk_label_set_text (qfield, NULL);
	} else
	{
		update_txtfield (currentq->question);
	}

	return 0;
}

int resolvequestion (void)
{
	if (currentq->alen == 0) return 0;

	update_txtfield (currentq->answer);
}

int add_question (size_t in_len, char* in_string)
{
	static uint_fast32_t qalen;
	static question_t *tmpq = NULL;

	if (in_len < 1) return -1;
	
	switch (in_string[0])
	{
		case 'Q':
		case 'q':
			if (entryq == NULL)
			{
				tmpq = malloc (sizeof(question_t));
				tmpq->next = NULL;
				entryq = tmpq;
				currentq = tmpq;
			} else
			{
				/* question in queue, save */
				currentq->next = tmpq;
				currentq = tmpq;
				tmpq = malloc (sizeof(question_t));
			}
			tmpq->next = NULL;
			tmpq->alen = 0;
			tmpq->ilen = 0;
			tmpq->qlen = in_len;
			tmpq->question = malloc (sizeof(char) * (in_len + 1));
			strncpy (tmpq->question, in_string +1, in_len);

		break;

		case 'A':
		case 'a':
			tmpq->alen = in_len;
			tmpq->answer = malloc(sizeof(char) * (in_len + 1));
			strncpy (tmpq->answer, (in_string + sizeof(char)), in_len - sizeof(char));
		break;

		case 'I':
		case 'i':
			tmpq->ilen = in_len;
			tmpq->image = malloc(sizeof(char) * (in_len + 1));
			strncpy (tmpq->image, (in_string + sizeof(char)), in_len - sizeof(char));
		break;
		
		case 'e':
		case 'E':
			printf ("end of file\n");
			currentq->next = tmpq;
			currentq = entryq;
			tmpq->next = entryq;
		break;
		default:
			printf ("ERROR: unknown command %c", in_string[0]);
	}
}

int read_questions (char *in_configfile)
{
	int fd, eno;
	uint32_t strpos = 0;
	char rdbuf[16000];
	const char delim = '\r';
	char *question, *qbuf, *tokptr, *saveptr;
	size_t qlen = 0, qbuflen = 0;
	uint_fast16_t i, l, q = 0;

	memset (rdbuf, '\0', sizeof(rdbuf));

	fd = open (in_configfile, O_RDONLY);
	eno = errno;

	if (eno)
	{
		printf("ERRROR: %s\n", strerror(eno));
		return (__LINE__ * -1);
	}
	
	i = read(fd, rdbuf, sizeof(rdbuf));
	qbuflen = i;
	qbuf = rdbuf;

	printf("%i bytes questions read\n%s\n", qbuflen, qbuf);

	tokptr = qbuf;


	for (i=0;i<qbuflen;i++)
	{
		if (qbuf[i] == '\n')
		{
			char *tmpq;

			tmpq = malloc ((i-strpos) + 1);
			strncpy (tmpq, qbuf + strpos, i-strpos);
			add_question (i-strpos, tmpq);
			strpos = i +1;
		}
	}

	close (fd);
	
	/* set question to first */
	currentq = entryq;
}
