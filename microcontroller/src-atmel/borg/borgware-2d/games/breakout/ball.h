#ifndef BALL_H
#define BALL_H

#include <stdint.h>
#include "common.h"


typedef struct
{
	uint16_t x;
	uint16_t y;
	int16_t dir_x; /* direction vector */
	int16_t dir_y;
	uint8_t strength;
} ball_t;

void ball_spawn (ball_t *in_ball, uint16_t in_x, uint16_t in_y, int16_t in_dir_x, int16_t in_dir_y, uint8_t in_strength)
{
	in_ball->x = in_x;
	in_ball->y = in_y;
	in_ball->dir_x = in_dir_x;
	in_ball->dir_y = in_dir_y;
	in_ball->strength = in_strength;
}

/* @description Called once per game tick. Move the ball further along it's vector.
 */
void ball_think (ball_t *in_ball);

/* @description Change the ball's moving vector according to bounce and collision type
 */
void ball_bounce (ball_t *in_ball, enum collision_t in_coltype);

void ball_die (ball_t *in_b)
#endif /* BALL_H */
