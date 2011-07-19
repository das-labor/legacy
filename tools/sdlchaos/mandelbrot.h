/* mandelbrot.h */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2011 Daniel Otte (daniel.otte@rub.de)

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

#ifndef MANDELBROT_H_
#define MANDELBROT_H_

#include <stdint.h>

void mandelbrot_set_maxiter(uint32_t v);
uint32_t mandelbrot_get_maxiter(void);

void mandelbrot_set_maxsum(double v);
double mandelbrot_get_maxsum(void);

void mandelbrot_fillBuffer(double x, double y, uint32_t width, uint32_t height,
	 	                   double zoom, uint16_t* buffer);

#endif /* MANDELBROT_H_ */
