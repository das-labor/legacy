
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


#ifdef ANIMATION_SPIRALE
void spirale(int delay) {
	clear_screen(0);
	static signed char const delta[5] PROGMEM = {0, -1, 0, 1, 0};
	unsigned char length[2] = {NUM_ROWS, NUM_COLS - 1};
	unsigned char x = NUM_COLS - 1, y = NUM_ROWS;
	unsigned char i = 0;

	while (length[i & 0x01] != 0) {
		for (unsigned char j = 0; j < length[i & 0x01]; ++j) {
			x += pgm_read_byte(&delta[i]);
			y += pgm_read_byte(&delta[i + 1]);
			setpixel((pixel){x, y}, 3);
			wait(delay);
		}
		length[i++ & 0x01]--;
		i %= 4;
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
