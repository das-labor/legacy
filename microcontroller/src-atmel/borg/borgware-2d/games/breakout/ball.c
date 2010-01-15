#include "common.h"

void ball_think (ball_t *b)
{
	uint8_t new_x, new_y;
	if (!b->strength)
		return;
	
	new_x = (b->x + b->dir_x) >> 8;
	new_y = (b->y + b->dir_y) >> 8;

	printf("B: %i %i, d: %i %i\n", new_x, new_y);
	
	/* ball fell out of the field */
//	if (new_y >= NUM_ROWS)
//		ball_die (b);

	/* bounce in x direction */
	if (check_bounce (new_x, b->y >> 8))
	{
		b->dir_x *= -1; /* invert x vector */
		new_x += b->dir_x;

#if BOUNCE_SLOWDOWN
		if (b->dir_x < 0)
		{
			b->dir_x += BOUNCE_SLOWDOWN;
		} else
		{
			b->dir_x -= BOUNCE_SLOWDOWN;
		}
#endif
	}

	/* bounce in y direction */
	if (check_bounce ((b->x >> 8), new_y))
	{
		b->dir_y *= -1; /* invert y vector */
		new_y += b->dir_y;

#if BOUNCE_SLOWDOWN
		if (b->dir_y < 0)
		{
			b->dir_y += BOUNCE_SLOWDOWN;
		} else
		{
			b->dir_y -= BOUNCE_SLOWDOWN;
		}
#endif
	}

	b->x = new_x;
	b->y = new_y;
}
	
void ball_die (ball_t *in_b)
{
	in_b->strength--;

	/* respawn ball with random direction */
	if (in_b->strength)
		ball_spawn (in_b, (NUM_COLS / 2) << 8, (NUM_ROWS-2) << 8, - random8(), random8(), START_LIFES);
}

void ball_draw (ball_t *b)
{
	pixel p;
	p.x = b->x;
	p.y = b->y;

	setpixel (p, 3);
}

void ball_spawn (ball_t *in_ball, uint16_t in_x, uint16_t in_y, int16_t in_dir_x, int16_t in_dir_y, uint8_t in_strength)
{
	in_ball->x = in_x;
	in_ball->y = in_y;
	in_ball->dir_x = in_dir_x;
	in_ball->dir_y = in_dir_y;
	in_ball->strength = in_strength;
}
