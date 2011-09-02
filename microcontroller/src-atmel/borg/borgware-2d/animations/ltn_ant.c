/* Copyright (c) 2010 Jan Lieven
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */


#include "../config.h"
#include "../pixel.h"
#include "../util.h"
#include "../random/prng.h"


void ltn_ant() {
	clear_screen(0);

	struct {
		unsigned char  x,  y;
		unsigned char ox, oy; /* Used to set old pixels so brightness 2 */
		signed char   dx, dy; /* Vector can only be (0,1),(1,0),(0,-1),(-1,0) */
	} ant; 
		                    
	signed char temp;
	unsigned int cycles = 500;

	/* Random start position and direction */
	ant.x = random8() % NUM_COLS;
	ant.y = random8() % NUM_ROWS;

	/* Make sure we do have a valid vector */
	ant.dx = (random8() % 3) - 1;
	do {
		ant.dy = (random8() % 3) - 1;
	} while(ant.dx == ant.dy);

	ant.ox = ant.x;
	ant.oy = ant.y;

	while(cycles != 0) {
		/* If the pixel is not set turn it on */
		if(get_pixel((pixel) {ant.x, ant.y}) == 0) {
			setpixel((pixel) {ant.x, ant.y}, 3);
			
			temp   = ant.dx;
			ant.dx = ant.dy;
			ant.dy =  -temp; /* Turn 90 degrees to the right */
			
			/* Lets the last pixel be darker than the latest */
			if((ant.ox != ant.x) || (ant.oy != ant.y))
				setpixel((pixel) {ant.ox, ant.oy}, 2);

			ant.ox = ant.x;
			ant.oy = ant.y;

		} else {
			setpixel((pixel) {ant.x, ant.y}, 0);
			
			temp   = ant.dy;
			ant.dy = ant.dx;
			ant.dx =  -temp; /* Turn 90 degrees to the left */
		}

		wait(100);

		/* Playing field is modeled after a torus */
		if(ant.x == 0 && ant.dx < 0)
			ant.x = NUM_COLS - 1;
		else
			ant.x = (ant.x + ant.dx) % UNUM_COLS;

		if(ant.y == 0 && ant.dy < 0)
			ant.y = NUM_ROWS - 1;
		else
			ant.y = (ant.y + ant.dy) % UNUM_ROWS;

		cycles--;
	}
	wait(300);
}
