/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Author & Copyright (C) 2010: Soeren Heisrath (forename@surename.org)
 *
 */

#include "ball.h"

/* internal functions */

static void ball_spawn (ball_t *in_ball, int16_t in_x, int16_t in_y, int16_t in_dir_x, int16_t in_dir_y)
{
	in_ball->x = in_x;
	in_ball->y = in_y;
	in_ball->dir_x = in_dir_x;
	in_ball->dir_y = in_dir_y;
}

static void ball_die (ball_t *in_b)
{
	in_b->strength--;

	/* respawn ball with random direction */
	if (in_b->strength)
	{
		print_ballsleft(in_b);
		ball_spawn_default (in_b);
	}
}

/* modify a vecotor according to given type of bouncing */
static void bounce_rand_vector (ball_t *in_b, int8_t in_bouncetype)
{
	uint8_t rval = random8();
	
	switch (in_bouncetype)
	{
		case BOUNCE_NONE: /* don't touch the vector since nothing changed */
			return;

		case BOUNCE_BRICK:
			in_b->dir_x ^= (rval & 0x07);
			in_b->dir_y ^= (rval & 0x07);
			break;

		case BOUNCE_REBOUND: /* the rebound is rather percise */
			in_b->dir_x ^= (rval & 0x03);
			in_b->dir_y ^= (rval & 0x03);
			
			if (JOYISRIGHT || JOYISLEFT)
			{
				/* a moving rebond accelerates the ball 12,5% */
				in_b->dir_y += (in_b->dir_y / 8);
				in_b->dir_x += (in_b->dir_x / 8);
			}
			break;

		default: /* walls */
			in_b->dir_x ^= (rval & 0x01);
			in_b->dir_y ^= (rval & 0x01);
	}
	if (!in_b->dir_x)
		in_b->dir_x = 213;
	
	if (!in_b->dir_y)
		in_b->dir_y = 217;
}


/* interface functions */

void ball_think (ball_t *b)
{
	int8_t proj_x, proj_y, bounce, tmp;
	
	/* projection of the new coordinates */
	proj_x = (b->x + (b->dir_x)) / 256;
	proj_y = (b->y + (b->dir_y)) / 256;
	
	/* falling out of the field */
	if (proj_y >= NUM_ROWS)
		ball_die (b);

	bounce = check_bounce (proj_x, b->y / 256);

	/* bouncing on bricks needs special handling */
	if (bounce & (BOUNCE_BRICK))
		bounce |= BOUNCE_X;

	tmp = check_bounce (b->x / 256, proj_y);
	if (tmp & (BOUNCE_BRICK))
		bounce |= BOUNCE_Y;
	bounce |= tmp;
	
	tmp = check_bounce (proj_x, proj_y);
	if (tmp & (BOUNCE_BRICK))
		bounce |= BOUNCE_X | BOUNCE_Y;
	bounce |= tmp;

	bounce_rand_vector (b, bounce);

	/* bounce in x direction */
	if (bounce & BOUNCE_X)
	{
		b->dir_x *= -1; /* invert x vector */
	}

	/* bounce in y direction */
	if (bounce & BOUNCE_Y)
	{
		b->dir_y *= -1; /* invert y vector */
	}
	
#if BOUNCE_SLOWDOWN
	if (bounce & BOUNCE_BRICK)
	{
		if (b->dir_y < - BALL_MINSPEED)
		{
			b->dir_y += BOUNCE_SLOWDOWN;
		} else if (b->dir_y > BALL_MINSPEED)
		{
			b->dir_y -= BOUNCE_SLOWDOWN;
		}

		if (b->dir_x < - BALL_MINSPEED)
		{
			b->dir_x += BOUNCE_SLOWDOWN;
		} else if (b->dir_y > BALL_MINSPEED)
		{
			b->dir_x -= BOUNCE_SLOWDOWN;
		}
	}
#endif

	if (bounce & BOUNCE_REBOUND)
	{
		rebound_reflect(b, proj_x);
	}

	if (b->dir_x > BALL_MAXSPEED)
		b->dir_x = BALL_MAXSPEED;

	if (b->dir_x < -BALL_MAXSPEED)
		b->dir_x = -BALL_MAXSPEED;

	if (b->dir_y > BALL_MAXSPEED)
		b->dir_y = BALL_MAXSPEED;

	if (b->dir_y < -BALL_MAXSPEED)
		b->dir_y = -BALL_MAXSPEED;


	b->y += b->dir_y;
	b->x += b->dir_x;
}

void ball_draw (ball_t *b)
{
	pixel p;
	p.x = (uint8_t) abs(b->x / 256);
	p.y = (uint8_t) abs(b->y / 256);

	setpixel (p, 3);
}

void ball_spawn_default (ball_t *in_b)
{
	int16_t xdir;

	xdir = 128 + (random8() & 0x3F);
	if (random8() & 0x01)
		xdir *= -1;
	
	ball_spawn (in_b, rebound_getpos() * 256, (NUM_ROWS - 2) * 256, xdir, -131);
}
