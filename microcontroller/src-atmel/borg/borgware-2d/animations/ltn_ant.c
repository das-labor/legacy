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

#include <limits.h>
#include "../config.h"
#include "../pixel.h"
#include "../util.h"
#include "../random/prng.h"

#if (UNUMCOLS <= (UCHAR_MAX / 2)) && (UNUMROWS <= (UCHAR_MAX / 2))
	typedef unsigned char coord_t;
#else
	typedef unsigned int coord_t;
#endif

#define P (1u)
#define NX (UNUM_COLS - 1u)
#define NY (UNUM_ROWS - 1u)

#if UNUM_ROWS == UNUM_COLS
	static coord_t const dcomp[] = {0, P, NX};
	#define xdcomp dcomp
	#define ydcomp dcomp
#else
	static coord_t const xdcomp[] = {0, P, NX};
	static coord_t const ydcomp[] = {0, P, NY};
#endif



void ltn_ant() {
	clear_screen(0);

	struct {
		coord_t  x,  y;
		coord_t ox, oy; /* Used to set old pixels to brightness 2 */
		coord_t dx, dy; /* Vector can only be (0,1),(1,0),(0,-1),(-1,0) */
	} ant; 
		                    
	unsigned char temp;
	unsigned int cycles = 500;

	/* Random start position and direction */
	ant.x = random8() % UNUM_COLS;
	ant.y = random8() % UNUM_ROWS;

	/* Make sure we do have a valid vector */
	ant.dx = xdcomp[random8() % 3];
	do {
		ant.dy = ydcomp[random8() % 3];
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
		ant.x = (coord_t)(ant.x + ant.dx) % UNUM_COLS;
		ant.y = (coord_t)(ant.y + ant.dy) % UNUM_ROWS;

		cycles--;
	}
	wait(300);
}
