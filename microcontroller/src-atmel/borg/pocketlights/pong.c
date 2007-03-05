#include "pong.h"

#include "pixel.h"
#include "buttons.h"

ball ball;

int paddle1_posy=0;
int paddle2_posy=0;

int player1_score;
int player2_score;

int game_won=0;

void play_pong(){
	ball.pos = (pixel){9,3};
	ball.speed = 1;
	ball.dir = random()%4;
	
	while(!game_won){
		move_ball();
		react_input();
	}	
}

void move_paddle(int player, direction dir){
	
	
}