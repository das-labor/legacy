#include "messages.h"

void print_ballsleft (ball_t *in_b)
{
#ifdef SCROLLTEXT_SUPPORT
	uint8_t txt[20];
	snprintf (txt, sizeof(txt), "</#%u balls left", in_b->strength);
	scrolltext(txt);
#endif
}

void print_score ()
{
#ifdef SCROLLTEXT_SUPPORT
	uint8_t txt[32];
	snprintf (txt, sizeof(txt), "</#GAME OVER. Your score: %u", score_get());
	scrolltext(txt);
#endif

}
