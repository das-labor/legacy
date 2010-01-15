#include <stdint.h>

#ifndef SCORE_H
#define SCORE_H

static uint16_t score = 0;

void score_add(uint8_t);
score_add (uint8_t in_score)
{
	score += in_score;
}
#endif
