#ifndef BALL_H
#define BALL_H

#include <stdint.h>
#include "common.h"


typedef struct
{
	int16_t x;
	int16_t y;
	int16_t dir_x; /* direction vector */
	int16_t dir_y;
	uint8_t strength;
} ball_t;

void ball_spawn (ball_t *in_ball, uint16_t in_x, uint16_t in_y, int16_t in_dir_x, int16_t in_dir_y, uint8_t in_strength);

/* @description Called once per game tick. Move the ball further along it's vector.
 */
void ball_think (ball_t *in_ball);


void ball_die (ball_t *in_b);

void ball_draw (ball_t *);
#endif /* BALL_H */
