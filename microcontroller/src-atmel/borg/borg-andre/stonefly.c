#include <stdlib.h>
#include <inttypes.h>
#include "pixel.h"
#include "prng.h"
#include "config.h"
#include "tetris/piece.h"

#define MAX_STONES 8
#define YSCALE 2		//y axis is scaled up, this allows for YSCALE different falling speeds

//this is hackish, i'll be drunk in a couple of minutes; no comments.

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t speed;
	tetris_piece_t piece;
} stone_t;

//forward decls (not drunk yet!)
void create_stone(stone_t *stone);
void draw_stone(stone_t *stone);


//create a stone at a random x pos
void create_stone(stone_t *stone)
{
	//create stone at top position
	stone->y = 0;

	//random x
	stone->x = random8() % NUM_COLS;	

	//random shape at random angle (untyped enums rock! yay!)
	stone->piece.shape = random8() % 7;
	stone->piece.angle = random8() % 4;

	//chose a random speed from 1-2
	stone->speed = (random8() % YSCALE) + 1;
}


//simply draw the stone at it's specified position
void draw_stone(stone_t *stone)
{
		uint8_t y, x;
		uint16_t nPieceMap = tetris_piece_getBitmap(&stone->piece);

		for(y = 0; y < 4; y++)
		{
			// clear all bits of the piece we are not interested in and
			// align the remaining row to LSB
			// -> translates to: get the correct row out of the bitmap and right-align
			nPieceMap = (nPieceMap & (0x000F << (y << 2))  ) >> (y << 2);

			// shift bitmap line to current x pos
			nPieceMap <<= stone->x;

			//DRUUUUUUUUUUUWWWWWWWWWWWWWWW!!!!!!!!!!! eerrr /U/A/s
			for (x = 0; x < 4; ++x)
			{
				if((nPieceMap & (1 << x)) && ((stone->x + x) < NUM_ROWS))
				{
					setpixel((pixel){ stone->x + x, (stone->y / YSCALE) + y}, 3);
				}
			}
		}	
}


//animation entry point
void stonefly(void)
{
	uint8_t i;
	stone_t stones[MAX_STONES];
	uint8_t stoneCount = 0;
	uint16_t counter = 500; /* run 500 cycles */

	//init all stones to zero
	for(i = 0; i < MAX_STONES; i++)
	{
		stones[i].speed = 0;		
	}

	//main loop
	while(counter--)
	{
		//clear scruuuun!
		clear_screen(0);

		//loop through all stones
		for(i = 0; i < stoneCount; i++)
		{
			//rapunzel!
			if((stones[i].y / YSCALE) >= NUM_ROWS)
			{
				//DIEEEE!!
				stones[i] = stones[stoneCount--];
				continue;
			}

			//draw stone!!
			draw_stone(&stones[i]);

			//advance!
			stones[i].y += stones[i].speed;
		}

		//if there are less than max_stones flying, there's a chance to spawn one
		if(stoneCount < MAX_STONES)	
		{
			if(random8() < 32)
			{
				create_stone(&stones[++stoneCount]);
			}
		}

		//loop delay
		wait(60);	
	}
}
