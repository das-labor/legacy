#include "score.h"
static uint16_t score = 0;
void score_add (uint8_t in_score)
{
	score += in_score;
}

uint16_t score_get()
{
	return score;
}
