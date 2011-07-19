/* mandelbrot.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2011 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdint.h>

static uint32_t max_iter = 32;
static double   max_sum = 4.0;

void mandelbrot_set_maxiter(uint32_t v){
	max_iter = v;
}

uint32_t mandelbrot_get_maxiter(void){
	return max_iter;
}


void mandelbrot_set_maxsum(double v){
	max_sum = v;
}

double mandelbrot_get_maxsum(void){
	return max_sum;
}


void mandelbrot_fillBuffer(double x, double y, uint32_t width, uint32_t height,
	 	                   double zoom, uint16_t* buffer){
	uint32_t wstart = width;
	double xstart=x;
	double x2, y2, xt=0, yt=0, xq=0, yq=0;
	uint32_t iter;
	zoom = 1.0/zoom;
	do{
		do{
			xq = yq = x2 = y2 = 0;
			iter = max_iter;
			do{
				xt = x2 - y2 + x;
				yt = 2 * xq * yq + y;
				xq = xt;
				yq = yt;
				x2 = xq * xq;
				y2 = yq * yq;
			}while(--iter&&((x2+y2)<max_sum));
			*buffer++ = iter;
			x += zoom;
		}while(--width);
		width = wstart;
		y += zoom;
		x = xstart;
	}while(--height);
}
