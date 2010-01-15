#include "common.h"

uint8_t brick_damage (uint8_t in_x, uint8_t in_y)
{
	game_field_t newtype;

	switch (playfiled[in_x][in_y])
	{
		case brick_1:
			playfield[in_x][in_y] = freespace;
			break;

		case brick_2:
			playfield[in_x][in_y] = brick_1;
			break;

		case brick_3:
			playfield[in_x][in_y] = brick_2;
			break;
			
		default:
			return;
	}
	score_add (1);
}

uint8_t check_bounce (uint8_t in_x, uint8_t in_y)
{
	/* overflow check */
	if (in_x >= NUM_ROWS)
		return 1;
	
	if (in_y >= NUM_COLS)
		return 1;
	
	/* collisions with real objects */
	switch (playfield[in_x][in_y])
	{
		case freespace:
		case ball:
			return 0;
		
		case brick_2:
		case brick_3:
		case brick_1:
			brick_damage (in_x, in_y);
		/* intentional fallthrough */

		case brick_solid:
		case bouncer:
			return 1;		
	}
}

/* this is the actual draw function for a single field
 */
static inline void draw_single_field (game_field_t in_f)
{
	switch (in_f)
	{
		case b1:
			setPixel ();
		return;
		case rb:
		case b2:
			
		return;

		case b3:
		case bl:
		case bs:
			
		return;

		default: /* this includes freespace */

		return;
		
	}
}

void playfield_draw ()
{
	uint8_t x,y;

	for (x=0;x<NUM_ROWS;x++)
	{
		for (y=0;y<NUM_COLS;y++)
		{
			
		}
	}
}
