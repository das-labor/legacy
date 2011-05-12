#include <inttypes.h>
#include "../config.h"
#include "../random/prng.h"
#include "../pixel.h"
#include "../util.h"

//dots flying from left to right
void flyingdots()
{
	uint8_t rowbuffer[NUM_ROWS], i, j;
	pixel p;

	clear_screen(0);
	
	//set the pixels to the leftmost columns
	p.x = NUM_COLS-1;
	
	//clear rowbuffer
	for(i = 0; i < NUM_ROWS; i++)
	{
		rowbuffer[i] = 0;
	}

	//produce 200 dots
	for (i = 0; i < 200; i++)
	{	
		//the idea is to use a buffer for one row
		//first all pixels that are in the rowbuffer
		//will be reduced in brightness
		//(as you can assume that they were
		// drawn in the last round and need to be dimmed
		// to produce the trail effect)
		//and then a new one will be added
		//finally the rowbuffer is drawn
		
		//loop through all pixels in our rowbuffer
		for(j = 0; j < NUM_ROWS; j++)
		{			
			//add a trail or remove the pixel, if it was bright enough
			if(rowbuffer[j] > 0)
			{
				rowbuffer[j]--;
			}
		}
				
		//choose row and brightness
		p.y = random8() % NUM_ROWS;
		rowbuffer[p.y] += (random8() % (NUMPLANE)) + 1;	
		
		//cap max brightness
		if(rowbuffer[p.y] > NUMPLANE)
		{
			rowbuffer[p.y] = NUMPLANE;
		}
		
		//draw all pixels in our rowbuffer
		for(j = 0; j < NUM_ROWS; j++)
		{
			if(rowbuffer[j] > 0)
			{
				p.y = j;
				setpixel(p, rowbuffer[j]);
			}
		}
		
		//shift the picture right
		//(yes, shift_pixmap_l shifts the picture right)
		shift_pixmap_l();
		
		//wait a bit
		wait(100);		
	}  
}
