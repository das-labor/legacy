/* maze_game.cc */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010 Daniel Otte (daniel.otte@rub.de)

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
#include <SDL/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "sdl_maze.hh"
#include "sdl_helper.h"

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH  640
#define SCREEN_BBP     32
#define SCREEN_METHOD  SDL_HWSURFACE



SDL_Surface *init_screen(){
	SDL_Surface *screen=NULL;
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
		fprintf(stderr, "Error: could not initialize SDL: %s\n", SDL_GetError());
		return NULL;
	}
	//Set up screen
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BBP, SCREEN_METHOD);
	SDL_WM_SetCaption("SDL testing application", NULL);
	if(TTF_Init()==-1){
		fprintf(stderr, "Error: could not initialize SDL_TTF\n");
	}

	return screen;
}

void clean_up(void){
	TTF_Quit();
	SDL_Quit();
}

void display_start_msg(SDL_Surface* sf){
	const char *msg_a[] =
	{	"Move the green dot through the maze",
		"to the red target area.",
		"Press [Enter] when ready.",
		""
	};
	const char **msg = msg_a;
	TTF_Font *font=NULL;
	SDL_Color msgcolor = {0xFF, 0xFF, 0xFF};
	SDL_Surface *msg_sf;
	SDL_Rect rect;
	SDL_Event event;

	/* blank screen */
	SDL_FillRect(sf, NULL, 0);
	/* render text */
	font = TTF_OpenFont("FreeSans.ttf", 32);
	if(!font){
		fprintf(stderr, "Error: could not load font: %s\n", SDL_GetError());
	}
	/* display text */
	rect.y = sf->h/4 ;
	while(**msg){
		msg_sf = TTF_RenderText_Solid(font, *msg++, msgcolor);
		rect.x = (sf->w - msg_sf->w)/2;
		rect.w = msg_sf->w;
		rect.h = msg_sf->h;
		SDL_BlitSurface(msg_sf, NULL, sf, &rect);
		rect.y += msg_sf->h+16;
		SDL_FreeSurface(msg_sf);
	}
	SDL_Flip(sf);
	/* wait for enter */
	for(;;){
		SDL_PollEvent(&event);
		if(event.type == SDL_KEYDOWN){
			if(event.key.keysym.sym == SDLK_RETURN){
				break;
			}
		}
	}
	TTF_CloseFont(font);
}

void display_end_msg(SDL_Surface* sf, uint32_t time){
	const char *msg_a[] = {
			"Congratulations!",
			"You mastered the maze in",
			"",
			""
	};
	char time_str[35];
	const char **msg = msg_a;
	TTF_Font *font=NULL;
	SDL_Color msgcolor = {0xFF, 0xFF, 0xFF};
	SDL_Surface *msg_sf;
	SDL_Rect rect;
	SDL_Event event;

	msg_a[2]=time_str;
	if(time>=60*1000){
		sprintf(time_str,"%d min and %.2f seconds", time/(60*1000), (time%(60*1000))/1000.0);
	}else{
		sprintf(time_str,"%.2f seconds", time/1000.0);
	}
	/* blank screen */
	SDL_FillRect(sf, NULL, 0);
	/* render text */
	font = TTF_OpenFont("FreeSans.ttf", 32);
	if(!font){
		fprintf(stderr, "Error: could not load font: %s\n", SDL_GetError());
	}
	/* display text */
	rect.y = sf->h/4 ;
	while(**msg){
		msg_sf = TTF_RenderText_Solid(font, *msg++, msgcolor);
		rect.x = (sf->w - msg_sf->w)/2;
		rect.w = msg_sf->w;
		rect.h = msg_sf->h;
		SDL_BlitSurface(msg_sf, NULL, sf, &rect);
		rect.y += msg_sf->h+16;
		SDL_FreeSurface(msg_sf);
	}
	SDL_Flip(sf);
	/* wait for enter */
	for(;;){
		SDL_PollEvent(&event);
		if(event.type == SDL_KEYDOWN){
			if(event.key.keysym.sym == SDLK_RETURN){
				break;
			}
		}
	}
	TTF_CloseFont(font);
}


int main(int argc, char** argv){
	SDL_Surface *screen = NULL;
	SDL_Event event;
	bool change=true;
	bool quit=false;
	uint32_t start_time;
	uint32_t time_elapsed;
	SDL_Rect maze_pos;
	screen = init_screen();

	display_start_msg(screen);

	sdl_maze playmaze(20,20, NULL, 20, 2);
	SDL_Surface* maze_surface=NULL;

	maze_surface = playmaze.getSurface();
	maze_pos.x = (screen->w - maze_surface->w)/2;
	maze_pos.y = (screen->h - maze_surface->h)/2;
	maze_pos.w = maze_surface->w;
	maze_pos.h = maze_surface->h;
	playmaze.setRandom();
	playmaze.print(stdout);
	playmaze.set_player_pos(0, -1);
	playmaze.set_target_pos(19, -1);
	playmaze.load_player_image_from_file("player20.png");
	playmaze.load_target_image_from_file("target20.png");
	playmaze.render_maze();
	SDL_FillRect(screen, NULL, 0);

	start_time=SDL_GetTicks();
	while(quit==false){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				quit=true;
			}
			if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					case SDLK_UP:    change=true;
					                 playmaze.move(north_ot);
					                 break;
					case SDLK_DOWN:  change=true;
					                 playmaze.move(south_ot);
					                 break;
					case SDLK_LEFT:  change=true;
					                 playmaze.move(west_ot);
					                 break;
					case SDLK_RIGHT: change=true;
					                 playmaze.move(east_ot);
					                 break;
					case SDLK_q:	 quit=true;
					                 break;
					default : break;
				}
			}
		}
		if(change){
			playmaze.render_game();
			SDL_BlitSurface(maze_surface, NULL, screen, &maze_pos);
			SDL_Flip(screen);
			change=false;
			quit=playmaze.game_finished();
		}

	}
	time_elapsed = SDL_GetTicks() - start_time;

	display_end_msg(screen, time_elapsed);

	clean_up();

	return 0;
}
