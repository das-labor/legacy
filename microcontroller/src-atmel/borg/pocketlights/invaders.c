#include "invaders.h"

#include "pixel.h"
#include "buttons.h"
#include "util.h"
#include "config.h"

#define NUM_BULLETS 5

static int tank_posx=0;
static int invader_posx=0;

static int game_won=0;

static bullet_t bullets[NUM_BULLETS];
static int bullets_fired=0;

void reset_game(){
	tank_posx=NUM_COLS/2-1;
	
	int i;
	for(i=0; i<NUM_BULLETS; i++){
		bullets[i].pos=(pixel){0,1};
		bullets_fired=0;
	}
}

void move_tank (direction dir){
	switch(dir){
		case left:	tank_posx++; 	break;
		case right:	tank_posx--;	break;
		default:					break;
	}
	
	// Make the tank stay on the playing field
	if(tank_posx>=(NUM_COLS-1))
		tank_posx=(NUM_COLS-1);
	      
	if(tank_posx<=0+2)
		tank_posx=0+2;
}

void move_bullets(){
	int i;
	for(i=0; i<bullets_fired; i++){
		if(bullets[i].pos.y <= 0){
			bullets[i].pos=(pixel){0,1};
			bullets_fired--;
		}
		if(bullets[i].pos.x != 0)
			bullets[i].pos=next_pixel(bullets[i].pos,up);
	}	
}

void draw_bullets() {
	int i;
	for(i=0; i<NUM_BULLETS; i++){
		if(bullets[i].pos.x != 0)
			setpixel(bullets[i].pos,3);
	}		
}

void draw_tank() {
	setpixel((pixel){tank_posx, 	NUM_ROWS-1},3);	/*		*/
	setpixel((pixel){tank_posx-1, 	NUM_ROWS-1},3); /*	 #	*/
	setpixel((pixel){tank_posx-2, 	NUM_ROWS-1},3);	/*	###	*/
	setpixel((pixel){tank_posx-1, 	NUM_ROWS-2},3);	/*		*/
}

void fire_bullet(){
	if(bullets_fired<NUM_BULLETS){
		bullets[bullets_fired++].pos=(pixel){tank_posx-1, NUM_ROWS-2};
	}
}



void react_input_invaders(){
	switch(get_button()){
		case BUTTON_LEFT: 		move_tank(left); 	break;
		case BUTTON_RIGHT: 		move_tank(right); 	break;
		case BUTTON_B:			fire_bullet(); 		break;
		case BUTTON_A: 			fire_bullet(); 	 	break;
		//DEBUG:
		case BUTTON_START:		reset_game();		break;
		default: break;			
	}
}

void play_invaders(){
	game_won=0;
	reset_game();
	
	while(!game_won){
		clear_screen(0);
		react_input_invaders();
		move_bullets();
		draw_tank();
		draw_bullets();
		wait(100);
	}	
}