/* sdl_helper.c */
/*
    This file is part of the PsyBench.
    Copyright (C) 2010-2010 Daniel Otte (daniel.otte@rub.de)

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

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include "sdl_helper.h"

SDL_Surface *load_image(const char* fname){
	SDL_Surface *image=NULL, *opt_image=NULL;
	image = IMG_Load(fname);
	if(!image){
		fprintf(stderr, "Error: could not load image: %s\n", SDL_GetError());
		return NULL;
	}
	opt_image = SDL_DisplayFormatAlpha(image);
	SDL_FreeSurface(image);
	return opt_image;
}
