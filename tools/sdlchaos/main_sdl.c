/* main_sdl.c */
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

#include <stdint.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "mandelbrot.h"

#define WIDTH  640
#define HEIGHT 480
#define MOVE_FACTOR 0.25

int main( int argc, char* args[] ) {
	SDL_Surface* screen = NULL;
	SDL_Surface* image = NULL;
	SDL_Event event;
	int quit=0,redraw=1;
	uint32_t comp_time;
	uint16_t mbuffer[WIDTH*HEIGHT];
	double x=-2.0, y=-1.0, zoom=100.0, max_sum;
	uint32_t max_iter;
	//Start SDL
	x = - (WIDTH/zoom)/2;
	y = - (HEIGHT/zoom)/2;
	max_iter = mandelbrot_get_maxiter();
	max_sum  = mandelbrot_get_maxsum();
	SDL_Init( SDL_INIT_EVERYTHING );
	screen = SDL_SetVideoMode( WIDTH, HEIGHT, 32, SDL_SWSURFACE );
	do{
		if(redraw){
			redraw = 0;
			comp_time = SDL_GetTicks();
			mandelbrot_fillBuffer(x, y, WIDTH, HEIGHT, zoom, mbuffer);
			comp_time = SDL_GetTicks() - comp_time;
			image = SDL_CreateRGBSurfaceFrom(mbuffer, WIDTH, HEIGHT, 16, 2*WIDTH,
					3<<4, 3<<2, 3, 0);
			SDL_BlitSurface( image, NULL, screen, NULL );
			//Update Screen
			SDL_Flip( screen );
			printf("info: needed %d milliseconds to compute\n", comp_time);
		}
		while(!SDL_PollEvent(&event))
			;
		if(event.type == SDL_QUIT){
			quit=1;
		}
		if(event.type == SDL_KEYDOWN){
			redraw = 1;
			switch(event.key.keysym.sym){
			case SDLK_q:        quit=1; break;
			case SDLK_PLUS:     zoom *= 2; x += (WIDTH/zoom)/2; y += (HEIGHT/zoom)/2; break;
			case SDLK_MINUS:    x -= (WIDTH/zoom)/2; y -= (HEIGHT/zoom)/2; zoom /= 2; break;
			case SDLK_UP:       y -= MOVE_FACTOR * HEIGHT / zoom ; break;
			case SDLK_DOWN:     y += MOVE_FACTOR * HEIGHT / zoom ; break;
			case SDLK_LEFT:     x -= MOVE_FACTOR * WIDTH  / zoom ; break;
			case SDLK_RIGHT:    x += MOVE_FACTOR * WIDTH  / zoom ; break;
			case SDLK_1:        max_iter -= 1; mandelbrot_set_maxiter(max_iter); break;
			case SDLK_2:        max_iter += 1; mandelbrot_set_maxiter(max_iter); break;
			case SDLK_3:        max_sum  -= 1; mandelbrot_set_maxsum(max_sum); break;
			case SDLK_4:        max_sum  += 1; mandelbrot_set_maxsum(max_sum); break;
			default: redraw = 0; break;
			}
		}
	}while(!quit);
	SDL_FreeSurface(image);
	//Quit SDL
	SDL_Quit();
	return 0;
}
