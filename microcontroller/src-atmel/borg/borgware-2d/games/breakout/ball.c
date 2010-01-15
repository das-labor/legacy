#include "common.h"

void ball_think (ball_t *b)
{
	uint8_t new_x, new_y;
	if (!b->strength)
		return;
	
	new_x = (b->x + b->dir_x) >> 8;
	new_y = (b->y + b->dir_y) >> 8;
	
	/* ball fell out of the field */
	if (new_y >= NUM_ROWS)
		ball_die (b);

	/* bounce in x direction */
	if (check_bounce (new_x, b->y >> 8))
	{
		b->dir_x *= -1; /* invert x vector */

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
	if (check_bounce (b->x >> 8), new_y)
	{
		b->dir_y *= -1; /* invert y vector */

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
}
	
void ball_die (ball_t *in_b)
{
	in_b->strength--;

	/* respawn ball with random direction */
	if (in_b->strength)
		ball_spawn (in_b, (NUM_COLS / 2) << 8, (NUM_ROWS-2) << 8, - random8(), random8(), START_LIFES);
}
