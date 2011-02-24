
#include "../config.h"
#include "../random/prng.h"
#include "../pixel.h"
#include "../util.h"

#define RANDOM8() (random8())

#ifdef ANIMATION_TESTS
void test1(){
	unsigned char x,y;
        
	for (y=0;y<NUM_ROWS;y++){
		for (x=0;x<NUM_COLS;x++){
			setpixel((pixel){x,y}, 3);
			wait(100);
		}
	}
}

void test_level1(){
	unsigned char x,y;
	for (y=0;y<NUM_ROWS;y++){
		for (x=0;x<NUM_COLS;x++){
			setpixel((pixel){x,y}, 1);
			wait(5);
		}
	}
	wait(2000);
}

void test_level2(){
	unsigned char x,y;
	for (y=0;y<NUM_ROWS;y++){
		for (x=0;x<NUM_COLS;x++){
			setpixel((pixel){x,y}, 2);
			wait(5);
		}
	}
	wait(2000);
}

void test_level3(){
	unsigned char x,y;
	for (y=0;y<NUM_ROWS;y++){
		for (x=0;x<NUM_COLS;x++){
			setpixel((pixel){x,y}, 3);
			wait(5);
		}
	}
	wait(2000);
}

void test_levels(){
	unsigned char x,y,b;
	for(b=1;b<4;b++){
		for (y=0;y<NUM_ROWS;y++){
			for (x=0;x<NUM_COLS;x++){
				setpixel((pixel){x,y}, b);
				wait(5);
			}
		}
	}
}

void test_palette(){
	unsigned char x,y,b;
	for (y=0;y<NUM_ROWS;y++){
		b=y%4;
		for (x=0;x<NUM_COLS;x++){
			setpixel((pixel){x,y}, b);
			// wait(1);
		}
	}
	wait(2000);
}

void test_palette2(){
	unsigned char x,y,b;
	for (x=0;x<NUM_COLS;x++){
		b=x%4;
		for (y=0;y<NUM_ROWS;y++){
			setpixel((pixel){x,y}, b);
			// wait(1);
		}
	}
        wait(1000);
        for (x=0;x<NUM_COLS;x++){
         // shift image right                        
        shift_pixmap_l();
        wait(30);
        }
        

}

#endif


#ifdef ANIMATION_OFF
void off()
{
	clear_screen(0);

	while(1)
		wait(100);

}
#endif

#ifdef ANIMATION_SPIRALE

static void walk(cursor* cur, unsigned char steps, unsigned int delay){
	unsigned char x;
	for(x=steps;x--;){
		set_cursor(cur, next_pixel(cur->pos, cur->dir));
		wait(delay);
	}
}

void spirale(unsigned int delay){
	clear_screen(0);

	cursor cur;
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

#ifdef ANIMATION_SCHACHBRETT
void schachbrett(unsigned char times){
	clear_screen(0);
	for (unsigned char i = times; i--;) {
		for (unsigned char row = 0; row < NUM_ROWS; ++row) {
			for (unsigned char col = 0; col < LINEBYTES; ++col) {
				pixmap[2][row][col] = ((i ^ row) & 0x01) ? 0x55 : 0xAA;
			}
		}
		wait(200);
	}
}
#endif


#ifdef ANIMATION_FEUER
#define FEUER_Y (NUM_ROWS + 3)

void feuer()
{
	unsigned char y, x;
	unsigned int  t;
	unsigned char world[NUM_COLS][FEUER_Y];   // double buffer
	

	for(t=0; t<800; t++) {
		// diffuse
		for(y=1; y<FEUER_Y; y++) {
			for(x=1; x<NUM_COLS-1; x++) {
				world[x][y-1] = (FEUER_N*world[x-1][y] + FEUER_S*world[x][y] + FEUER_N*world[x+1][y]) / FEUER_DIV;
			};

			world[0][y-1] = (FEUER_N*world[NUM_COLS-1][y] + FEUER_S*world[0][y] + FEUER_N*world[1][y]) / FEUER_DIV;
			world[NUM_COLS-1][y-1] = (FEUER_N*world[0][y] + FEUER_S*world[NUM_COLS-1][y] + FEUER_N*world[NUM_COLS-2][y]) / FEUER_DIV;
		};

		// update lowest line
		for(x=0; x<NUM_COLS; x++) {
			world[x][FEUER_Y-1] = RANDOM8();
		};
	
		// copy to screen
		for(y=0; y<NUM_ROWS; y++) {
			for(x=0; x<NUM_COLS; x++) {
				setpixel( (pixel){x,y}, (world[x][y] >> 5) );
			}		
		};

		wait(FEUER_DELAY);
	}
}
#endif

#ifdef ANIMATION_RANDOM_BRIGHT
/**
 * void random_bright(void)
 *  by Daniel Otte
 * 
 * 
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
