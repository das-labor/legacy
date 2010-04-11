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
		char x ,  y;
		char dx, dy; /* Vector can only be: (0,1),(1,0),(0,-1),(-1,0) */
		char ox, oy; /* Used to set old pixels so brightness 2 */
	} ant; 
		                    
	char temp, i = 0;

	/* Random startposition and direction */
	ant.x  = random8() % NUM_COLS;
	ant.y  = random8() % NUM_ROWS;

	/* Make sure we do have a valid vector */	
	do {	
		ant.dx = random8() % 2;
		ant.dy = random8() % 2;
		/* We need negative directions, too */
		if(ant.dx == 2)
			ant.dx = -1;
		if(ant.dy == 2)
			ant.dy = -1;
	} while(ant.dx == ant.dy);

	do { 
		if(get_pixel((pixel) {ant.x, ant.y}) == 0) { /* If the pixel is not set turn it on */
			setpixel((pixel) {ant.x, ant.y}, 3);
			
			temp   = ant.dx;
			ant.dx = ant.dy;
			ant.dy =  -temp; /* Turn 90 degrees to the right */
			
			/* Lets the last pixel be darker than the latest */
			if(i == 1) 
				setpixel((pixel) {ant.ox, ant.oy}, 2);
			i = 1;
			ant.ox = ant.x;
			ant.oy = ant.y;

		} else {
			setpixel((pixel) {ant.x, ant.y}, 0);
			
			temp   = ant.dy;
			ant.dy = ant.dx;
			ant.dx =  -temp; /* Turn 90 degrees to the left */
		}

		wait(100);

		ant.x += ant.dx;
		ant.y += ant.dy;

	} while(ant.x >= 0       && 
		ant.x < NUM_COLS && 
		ant.y >= 0       && 
		ant.y < NUM_ROWS); /* Make sure we won't leave the matrix. (Mr. Smith hates it outside the matrix, you know?) */
				    
	wait(300);

}
