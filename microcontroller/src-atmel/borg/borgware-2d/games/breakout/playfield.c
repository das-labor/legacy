#include "common.h"
static enum game_field_t playfield[NUM_COLS][NUM_ROWS];

void playfield_set (uint8_t in_x, uint8_t in_y, enum game_field_t in_field)
{
	playfield[in_x][in_y] = in_field;
}

void brick_damage (uint8_t in_x, uint8_t in_y)
{
	enum game_field_t newtype;

	if (playfield[in_x][in_y] > bs || playfield[in_x][in_y] == 0)
		return;
	
	playfield[in_x][in_y]--;
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
		case sp:
		case bl:
			return 0;
		
		case b2:
		case b3:
		case b1:
			brick_damage (in_x, in_y);
		/* intentional fallthrough */

		case bs:
			return 1;		
		
		/* bouncing on the rebound needs special care */
		case rb:
			return 2;
	}
}

/* this is the actual draw function for a single field
 */
static inline void draw_single_field (uint8_t in_x, uint8_t in_y, enum game_field_t in_f)
{
	pixel tmp;
	uint8_t b;
	switch (in_f)
	{
		case b1:
			b = 1;
		break;

		case rb:
		case b2:
			b = 2;
		break;

		case b3:
		case bl:
		case bs:
			b = 3;
		break;

		default: /* this includes freespace */
			b = 0;
		break;
		
	}
	tmp.x = in_x;
	tmp.y = in_y;
	setpixel (tmp, b);
}

void playfield_draw ()
{
	uint8_t x,y;

	for (x=0;x<NUM_ROWS;x++)
	{
		for (y=0;y<NUM_COLS;y++)
		{
			draw_single_field (x,y, playfield[x][y]);
		}
	}
}
