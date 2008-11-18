#include <stdint.h>
#include <gtk/gtk.h>

#define NOT_VOTED 0x00

#define MODE_TEST    0x00
#define MODE_RUN     0x01
#define MODE_BLOCKED 0x02
#define MODE_BOTTOM MODE_BLOCKED

static uint8_t votenum = NOT_VOTED;
static uint8_t mymode = MODE_RUN;
static int votecounter[4] = {0, 0, 0, 0};


void setvote (uint8_t in_num, GtkButton *in_btn);
uint8_t getvote (void);
void countvote (void);
void resetvote ();
void togglevotemode();
void update_txtfield (char *in_string);
void vote_init (GtkButton *in_btn);

typedef struct
{
	int qlen, ilen, alen;
	char *question;
	char *image;
	char *answer;
	void *next;
} question_t;
