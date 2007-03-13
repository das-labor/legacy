#include "pong.h"

#include "pixel.h"
#include "buttons.h"
#include "util.h"
#include "config.h"
#include "scrolltext.h"

static ball_t ball;

static int paddle1_posy=0;
static int paddle2_posy=0;

static int player1_score;
static int player2_score;

static int game_won=0;

void reset_ball(){
	ball.pos = (pixel){NUM_COLS/2,NUM_ROWS/2};
	ball.speed = 1;
	ball.dir = random()%4;	
}

void draw_paddles(){
	// Draw the Paddles
	int i;
	for (i=0; i<PADDLE_LENGTH; i++){
		setpixel((pixel){0,paddle2_posy+i},3);
	}
	for (i=0; i<PADDLE_LENGTH; i++){
		setpixel((pixel){NUM_COLS-1,paddle1_posy+i},3);
	}
}

void draw_ball(){
	setpixel(ball.pos,3);
}

void move_paddle(int player, direction dir){
	switch(player){
		case 1: 	if(dir==up)
					paddle1_posy--;
				if(dir==down)
					paddle1_posy++;
				break;
		case 2:	if(dir==up)
					paddle2_posy--;
				if(dir==down)
					paddle2_posy++;
				break;
		default: break;
	}	
	
	// Make the Paddles stay on the playing field
	if(paddle1_posy>=(NUM_ROWS-PADDLE_LENGTH))
		paddle1_posy=(NUM_ROWS-PADDLE_LENGTH);
	      
	if(paddle1_posy<=0)
		paddle1_posy=0;
	
	if(paddle2_posy>=(NUM_ROWS-PADDLE_LENGTH))
		paddle2_posy=(NUM_ROWS-PADDLE_LENGTH);
	
	if(paddle2_posy<=0)
		paddle2_posy=0;

}

void move_ball(){
	ball.pos=next_diagonal_pixel(ball.pos,ball.dir);
	
	// Let the Ball bounce off the top and bottom of the screen
	if(ball.pos.y>=NUM_ROWS-1){
		switch(ball.dir){
			case left_down: 	ball.dir=left_up; 	break;
			case right_down: 	ball.dir=right_up; 	break;
			default:							break;
		}
	}
	if(ball.pos.y<=0){
		switch(ball.dir){
			case left_up: 		ball.dir=left_down; 	break;
			case right_up: 	ball.dir=right_down; break;
			default:							break;
		}		
	}
	
	//// Bounce the Ball off the paddles and/or count points
	
	//ball direction is left, check left paddle for collision
	if((ball.dir==left_up)||(ball.dir==left_down)){
		if(ball.pos.x==NUM_COLS-2 || ball.pos.x==NUM_COLS-1){
			if((ball.pos.y>=paddle1_posy)&&(ball.pos.y<=(paddle1_posy+PADDLE_LENGTH))){ //paddle is placed right, ball is hit.
				switch(ball.dir){
					case left_up:		ball.dir=right_up;		break;
					case left_down:	ball.dir=right_down;	break;
					default:								break;
				}
			} else {	// Ooops. Player 1 has missed the ball....
				draw_paddles();
				draw_ball();
				pixel tmp_pos = ball.pos;
				ball.pos=next_diagonal_pixel(ball.pos,ball.dir);
				setpixel(tmp_pos,0);
				draw_ball();
				player2_score++;
				//TODO: display scores...
				
				setpixel(ball.pos,0);
				wait(100);
				setpixel(ball.pos,3);
				wait(100);
				setpixel(ball.pos,0);
				wait(100);
				setpixel(ball.pos,3);
				wait(700);
				reset_ball();
			}
		}
	}
	
	//ball direction is right, check right paddle for collision
	if((ball.dir==right_up)||(ball.dir==right_down)){
		if(ball.pos.x==1 || ball.pos.x==0){
			if((ball.pos.y>=paddle2_posy)&&(ball.pos.y<=paddle2_posy+PADDLE_LENGTH)){ //paddle is placed right, ball is hit.
				switch(ball.dir){
					case right_up:		ball.dir=left_up;	break;
					case right_down:	ball.dir=left_down;	break;
					default:							break;
				}
			} else {	// Ooops. Player 2 has missed the ball....
				
				/* TODO:
				
				handle the miss correctly: 	- set a variable "missed" true
										- let the normal cycle draw the ball, paddles
										- handle the miss AFTER the ball has travelled to the screen border
										- blink the ball, show the score
										- reset "missed variable", reset_ball()...
				*/ 
				
				
				draw_paddles();
				draw_ball();
				pixel tmp_pos = ball.pos;
				ball.pos=next_diagonal_pixel(ball.pos,ball.dir);
				setpixel(tmp_pos,0);
				draw_ball();
				player1_score++;
				//TODO: display scores...
				
				setpixel(ball.pos,0);
				wait(100);
				setpixel(ball.pos,3);
				wait(100);
				setpixel(ball.pos,0);
				wait(100);
				setpixel(ball.pos,3);
				wait(700);
				reset_ball();

			}
		}
	}
	//TODO: introduce the speed factor...
}

void react_input(){
	switch(get_button()){
		case BUTTON_UP: 			move_paddle(1,up); 	break;
		case BUTTON_DOWN: 		move_paddle(1,down); 	break;
		case BUTTON_B:			move_paddle(2,down); 	break;
		case BUTTON_A: 			move_paddle(2,up); 	break;
		//DEBUG:
		case BUTTON_START:		reset_ball();			break;
		default: break;			
	}
}

void play_pong(){
	game_won=0;
	player1_score=0;
	player2_score=0;
	reset_ball();
	
	while(!game_won){
		clear_screen(0);
		react_input();
		draw_paddles();
		move_ball();
		draw_ball();
		wait(100);
	}	
	scrolltext("</#Game Over...");
}

