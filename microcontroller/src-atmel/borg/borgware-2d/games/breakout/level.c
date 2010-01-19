#include "level.h"

/* real level definition */
enum game_field_t level_field (uint8_t in_x, uint8_t in_y, uint8_t in_lvl);
enum game_field_t level_field (uint8_t in_x, uint8_t in_y, uint8_t in_lvl)
{
	switch (in_lvl)
	{
		case 1:
			/* space for the lower half of the level */
			if (in_y > (NUM_ROWS / 2))
				return sp;
			
			/* type 2 bricks for 1/4th of the field */
			if (in_y <= (NUM_ROWS / 4))
				return b2;
			
			/* fill the rest with type 1 */
			return b1;
		break;
		
		case 3:
			/* add a row of solid bricks right in the middle of the field */
			if (in_y == (NUM_ROWS / 2) &&
				(in_x > (NUM_COLS / 4)) && (in_x < (NUM_COLS - (NUM_COLS / 4))))
				return bs;

			/* intentional fallthrough: the rest of level 3 is like level 2 */

		case 2:
			/* space for the lower third of the level */
			if (in_y > (NUM_ROWS / 3))
				return sp;
			
			/* type 3 bricks for 1/8th of the field */
			if (in_y <= (NUM_ROWS / 8))
				return b3;
			
			/* type 2 bricks for 1/4th of the field */
			if (in_y <= (NUM_ROWS / 4))
				return b2;
			
			/* fill the rest with type 1 */
			return b1;

		default: /* random level generation */
			/* space for the lower half of the level */
			if (in_y > (NUM_ROWS / 2))
				return sp;
			
			return random8() & 0x03; /* fill field with random bricks (and spaces) */
		break;
	}
}

void level_init (uint8_t in_levelnum)
{
	uint8_t x,y;

	for (x=0;x<NUM_COLS;x++)
	{
		for (y=0;y<NUM_ROWS;y++)
		{
			playfield_set (x,y, level_field (x, y, in_levelnum));
		}
	}
}

