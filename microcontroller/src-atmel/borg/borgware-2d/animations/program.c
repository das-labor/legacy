
#include "../config.h"
#include "../compat/pgmspace.h"
#include "../random/prng.h"
#include "../pixel.h"
#include "../util.h"

#define RANDOM8() (random8())


#ifdef ANIMATION_TESTS
void test_level(unsigned char level){
	for (unsigned char y=NUM_ROWS;y--;){
		for (unsigned char x=NUM_COLS;x--;){
			setpixel((pixel){x,y}, level);
			wait(5);
		}
	}
	wait(2000);
}

void test_palette(){
	for (unsigned char y=NUM_ROWS;y--;){
		for (unsigned char x=NUM_COLS;x--;){
			setpixel((pixel){x,y}, y%4);
			// wait(1);
		}
	}
	wait(2000);
}

void test_palette2(){
	for (unsigned char x=NUM_COLS;x--;){
		for (unsigned char y=NUM_ROWS;y--;){
			setpixel((pixel){x,y}, x%4);
			// wait(1);
		}
	}
	wait(1000);
	for (unsigned char x=NUM_COLS;x--;){
		// shift image right
		shift_pixmap_l();
		wait(30);
	}
}
#endif


#ifdef ANIMATION_SPIRAL
static void walk(cursor_t* cur, unsigned char steps, unsigned int delay){
	unsigned char x;
	for(x=steps;x--;){
		set_cursor(cur, next_pixel(cur->pos, cur->dir));
		wait(delay);
	}
}

void spiral(unsigned int delay){
	clear_screen(0);

	cursor_t cur;
	cur.dir = right;
	cur.mode = set;
	set_cursor (&cur, (pixel){NUM_COLS-1,0});

	unsigned char clearbit=0;
	while(clearbit == 0){

		clearbit = 1;
		while (!get_next_pixel(cur.pos, cur.dir)){
			clearbit = 0;
			walk(&cur, 1, delay);
		}
		cur.dir = direction_r(cur.dir);
	}

	cur.mode = clear;
	set_cursor(&cur, (pixel){(NUM_COLS/2)-1,(NUM_ROWS/2)-1});

	for(clearbit=0;clearbit==0;){
		if( get_next_pixel(cur.pos, direction_r(cur.dir)) ){
			cur.dir = direction_r(cur.dir);
		}
		if( get_next_pixel(cur.pos, cur.dir) == 1 ){
			walk(&cur , 1, delay);
		}else{
			clearbit = 1;
		}
	}
}
#endif


#ifdef ANIMATION_JOERN1
void joern1(){
unsigned char i, j, x;
	unsigned char rolr=0x01 , rol;
	clear_screen(3);
	for(i = 0; i< 80;i++){
		rol = rolr;
		for(j = 0 ;j < NUM_ROWS; j++){
			for(x=0;x<LINEBYTES;x++)
				pixmap[2][j][x] = rol;
			if((rol<<=1)==0)rol = 0x01;
		}
		if((rolr<<=1) == 0) rolr = 1;
		wait(100);
	}
}
#endif


#ifdef ANIMATION_CHESSBOARD
void chessboard(unsigned char times){
	while (times--) {
		for (unsigned char row = 0; row < NUM_ROWS; ++row) {
			for (unsigned char col = 0; col < NUM_COLS; ++col) {
				setpixel((pixel){col, row}, (times ^ row ^ col) & 0x01 ? 0 : 3);
			}
		}
		wait(200);
	}
}
#endif


#ifdef ANIMATION_FIRE
#ifndef FIRE_CYCLES
	#define FIRE_CYCLES 800
#endif
#define FIRE_Y (NUM_ROWS + 3)
void fire()
{
	unsigned char y, x;
	unsigned int  t;
	unsigned char world[NUM_COLS][FIRE_Y];   // double buffer

	for (t = 0; t < FIRE_CYCLES; t++) {
		// diffuse
		for (y = 1; y < FIRE_Y; y++) {
			for (x = 1; x < NUM_COLS - 1; x++) {
				world[x][y - 1] =
						(FIRE_N * world[x - 1][y] +
						 FIRE_S * world[    x][y] +
						 FIRE_N * world[x + 1][y]) / FIRE_DIV;
			};

			world[0][y - 1] =
					(FIRE_N * world[NUM_COLS - 1][y] +
					 FIRE_S * world[           0][y] +
					 FIRE_N * world[           1][y]) / FIRE_DIV;

			world[NUM_COLS - 1][y - 1] =
					(FIRE_N * world[0           ][y] +
					 FIRE_S * world[NUM_COLS - 1][y] +
					 FIRE_N * world[NUM_COLS - 2][y]) / FIRE_DIV;
		}

		// update lowest line
		for (x = 0; x < NUM_COLS; x++) {
			world[x][FIRE_Y - 1] = RANDOM8();
		}

		// copy to screen
		for (y = 0; y < NUM_ROWS; y++) {
			for (x = 0; x < NUM_COLS; x++) {
				setpixel((pixel) {x,y}, world[x][y] >> 5);
			}
		}

		wait(FIRE_DELAY);
	}
}
#endif


#ifdef ANIMATION_RANDOM_BRIGHT
/**
 * void random_bright(void)
 *  by Daniel Otte
 */
void random_bright(unsigned cycles){
	uint8_t t,x,y;
	while(cycles--){
		for(y=0; y<NUM_ROWS; ++y)
			for(x=0; x<NUM_COLS/4; ++x){
				t=random8();
				setpixel((pixel){x*4+0, y}, 0x3&(t>>0));
				setpixel((pixel){x*4+1, y}, 0x3&(t>>2));
				setpixel((pixel){x*4+2, y}, 0x3&(t>>4));
				setpixel((pixel){x*4+3, y}, 0x3&(t>>6));
			}
		wait(200);
	}
}
#endif
