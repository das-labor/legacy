#include "common.h"


void ball_think (ball_t *b)
{
	int8_t proj_x, proj_y, bounce;
	if (!b->strength)
		return;
	
	/* projection of the new coordinates */
	proj_x = (b->x + (b->dir_x)) / 256;
	proj_y = (b->y + (b->dir_y)) / 256;
	
	/* ball fell out of the field */
	if (proj_y >= NUM_ROWS)
		ball_die (b);
	

	bounce = check_bounce (proj_x, b->y / 256);
	if (bounce & BOUNCE_UNDEF)
		bounce = (BOUNCE_X | bounce) & (BOUNCE_X | BOUNCE_Y);

	bounce |= check_bounce (b->x / 256, proj_y);
	if (bounce & BOUNCE_UNDEF)
		bounce = (BOUNCE_Y | bounce) & (BOUNCE_X | BOUNCE_Y);

	bounce |= check_bounce (proj_x, proj_y);
	if (bounce & BOUNCE_UNDEF)
		bounce = BOUNCE_X | BOUNCE_Y;

	

	/* bounce in x direction */
	if (bounce & 0x01)
	{
		b->dir_x *= -1; /* invert x vector */
		b->dir_x ^= random8() & 0x0F; /* randomize bouncing */


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
	if (bounce & 0x02)
	{
		b->dir_y *= -1; /* invert y vector */
		b->dir_y ^= random8() & 0x0F;
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

	b->y += b->dir_y;
	b->x += b->dir_x;
}
	
void ball_die (ball_t *in_b)
{
	in_b->strength--;

	/* respawn ball with random direction */
	if (in_b->strength)
	{
		print_ballsleft(in_b);
		ball_spawn (in_b, (uint16_t) (rebound_getpos() * 256), (uint16_t) (NUM_ROWS-2) * 256, -120, 150, in_b->strength);
	}
}

void ball_draw (ball_t *b)
{
	pixel p;
	p.x = (uint8_t) abs(b->x / 256);
	p.y = (uint8_t) abs(b->y / 256);

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

void ball_spawn_default (ball_t *in_b)
{
	ball_spawn (in_b, (uint16_t) (NUM_COLS / 2) * 256, (uint16_t) (NUM_ROWS-2) * 256, -120, 150, START_LIFES);
}
