#include <stdlib.h>
#include <inttypes.h>
#include "../config.h"
#include "../random/prng.h"
#include "../pixel.h"
#include "../games/tetris/piece.h"
#include "../util.h"

#define MAX_STONES 8
#define YSCALE 2		//y axis is scaled up, this allows for YSCALE different falling speeds

//this is hackish, i'll be drunk in a couple of minutes; no comments.

/* space invader
001000100 0x0044
101111101 0x017D
110111011 0x01BB
011111110 0x00FE
010000010 0x0082
*/

uint16_t invader[] = {0x0044, 0x017D, 0x01BB, 0x00FE, 0x0082};

#define INV_HEIGHT (sizeof(invader) / sizeof(uint16_t))

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t speed;
	uint8_t color;
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
	//4 is piece width
	stone->x = random8() % (NUM_COLS - 4);

	//random shape at random angle (untyped enums rock! yay!)
	stone->piece.shape = random8() % 7;
	stone->piece.angle = random8() % 4;

	//chose a random speed from 1-2
	stone->speed = (random8() % YSCALE) + 1;

	//chose a random color
	stone->color = (random8() % NUMPLANE) + 1;
}


//simply draw the stone at it's specified position
void draw_stone(stone_t *stone)
{
		uint8_t y, x, ydraw;
		uint16_t nPieceMap = tetris_piece_getBitmap(&stone->piece);
		uint16_t pieceLala;

		for(y = 0; y < 4; y++)
		{
			// clear all bits of the piece we are not interested in and
			// align the remaining row to LSB
			// -> translates to: get the correct row out of the bitmap and right-align
			pieceLala = (nPieceMap & (0x000F << (y << 2))  ) >> (y << 2);

			// shift bitmap line to current x pos (this can be done faster... BUT I WANT IT SO!)
			pieceLala <<= stone->x;

			//DRUUUUUUUUUUUWWWWWWWWWWWWWWW!!!!!!!!!!! eerrr /U/A/s
			ydraw = (stone->y / YSCALE) + y;
			if((ydraw < (NUM_ROWS + 4)) && (ydraw > 3)) //drawing begins @ 3, to make piece of height 4 scroll in
			{
				for (x = 0; x < 16; ++x)
				{
					if(pieceLala & (1 << x))
					{
						setpixel((pixel){ x, ydraw - 4 }, stone->color);
					}
				}
			}
		}
}


void draw_invader(uint8_t ypos, uint8_t xpos)
{
		uint8_t y, x, ydraw;
		uint16_t pieceLala;

		for(y = 0; y < INV_HEIGHT; y++)
		{
			pieceLala = invader[y];

			// shift bitmap line to current x pos (this is nonsense overhead... BUT I LIKE IT!)
			pieceLala <<= xpos;

			//DRUUUUUUUUUUUWWWWWWWWWWWWWWW!!!!!!!!!!! eerrr /U/A/s
			ydraw = (ypos  / 2) + y;
			if((ydraw < (NUM_ROWS + INV_HEIGHT)) && (ydraw > (INV_HEIGHT - 1))) //drawing begins @ 5, to make invader of height 6 scroll in
			{
				for (x = 0; x < 16; ++x)
				{
					if(pieceLala & (1 << x))
					{
						setpixel((pixel){ x, ydraw - INV_HEIGHT }, 1);
					}
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
	uint16_t counter = 622; /* run 622 cycles */
	uint8_t invax = 0, invay = 0, invasion = 0;
	uint8_t draw;

	//init all stones to zero
	for(i = 0; i < MAX_STONES; i++)
	{
		stones[i].speed = 0;
	}

	//main loop
	while(counter || stoneCount || invasion)
	{
		if(counter > 0)
		{
			counter--;
		}

		//see if invasion is done!
		if((invay / 2) >= (NUM_ROWS + INV_HEIGHT))
		{
			invasion = 0;
		}

		//clear scruuuun!
		clear_screen(0);

		//invade screen!!!
		if(invasion)
		{
			draw_invader(invay, invax);

			//advancarrrr
			invay++;
		}

		//loop through all stones
		for(i = 0; i < stoneCount; i++)
		{
			draw = 1;

			//rapunzel!
			while(((stones[i].y / YSCALE) >= (NUM_ROWS + 4)) && (stoneCount > 0))
			{
				//DIEEEE!!
				if(--stoneCount == i)
				{
					draw = 0;
					break;
				}
				else
					stones[i] = stones[stoneCount];
			}

			if(draw)
			{
				//draw stone!!
				draw_stone(&stones[i]);

				//advance!
				stones[i].y += stones[i].speed;
			}
		}

		//if there are less than max_stones flying, there's a chance to spawn one
		if((stoneCount < MAX_STONES) && (counter > 0))
		{
			if(random8() < 48)
			{
				create_stone(&stones[stoneCount++]);
			}

			//invasion time!!!
			if((random8() < 8) && (invasion == 0))
			{
				//9 is invader width
				invax = random8() % (NUM_COLS - 9);
				invay = 0;
				invasion = 1;
			}
		}

		//loop delay
		wait(60);
	}
}
