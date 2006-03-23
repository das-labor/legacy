#include "pixel.h"
#include "util.h"
#include "scrolltext.h"
#include <avr/pgmspace.h>

/*

	IT'S A TERRIBLE HACK!!! ;-)

*/


//#define setPixel(_X, _Y, _V) setpixel( (pixel){15 - _X, _Y}, _V)
#define setPixel(_X, _Y, _V) if(((unsigned char)(_Y)) < 16 ) setpixel( (pixel){15 - _X, _Y}, _V)

unsigned char random();

//extern unsigned char rhein[16][16] PROGMEM;

unsigned char rhein[16][16] PROGMEM= { 
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0 },
  { 3, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 3, 0, 0, 0, 0, 0, 3, 2, 3, 0, 0, 0, 3, 0, 0, 0 },
  { 3, 3, 3, 0, 0, 0, 3, 2, 3, 0, 0, 3, 0, 0, 0, 0 },
  { 3, 2, 2, 3, 3, 3, 2, 2, 2, 3, 3, 2, 3, 0, 0, 0 },
  { 0, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 0 },
  { 0, 0, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 },
  { 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 2, 3 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 3, 3, 2, 2, 3 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 2, 2, 2, 2, 3 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 2, 2, 3, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0 },
  };
  
  
 void rhein_fire_logo(int ms)
 {
 	clear_screen(0);
 	
 	unsigned char x, y;
 
 	for(x = 0; x < 16; x++) {
 		for(y = 0; y < 16; y++) {
 			setPixel(x, y, pgm_read_byte(&rhein[y][x]));
 		}
 	}
 
 	wait(ms);
 }


unsigned char numbers[6][5][3] PROGMEM =
{
	{	{3,3,3},
		{3,0,3},
		{3,0,3},
		{3,0,3},
		{3,3,3}
	},

	{	{0,3,3},
		{3,0,3},
		{0,0,3},
		{0,0,3},
		{0,0,3}
	},


	{	{3,3,3},
		{0,0,3},
		{0,3,0},
		{3,0,0},
		{3,3,3}
	},


	{	{3,3,3},
		{0,0,3},
		{0,3,3},
		{0,0,3},
		{3,3,3}
	},

	{	{3,0,3},
		{3,0,3},
		{3,3,3},
		{0,0,3},
		{0,0,3}
	},

	{	{3,3,3},
		{3,0,0},
		{3,3,3},
		{0,0,3},
		{3,3,3}
	}
};
 

unsigned char player[4][5][3] PROGMEM = 
{
	{{0,1,0},
	{0,2,2},
	{0,3,0},
	{0,2,0},
	{1,0,0}},

	{{0,1,0},
	{0,2,0},
	{0,3,0},
	{2,2,0},
	{0,1,0}},

/*
	{{0,1,0},
	{0,2,0},
	{0,3,0},
	{0,2,0},
	{0,1,0}},
*/

	{{1,0,0},
	{0,2,0},
	{0,3,0},
	{0,2,2},
	{0,1,0}},

	{{0,1,0},
	{2,2,0},
	{0,3,0},
	{0,2,0},
	{0,1,0}},

};


void drawNumber(unsigned char num, signed char ax, signed char ay) {
	if(num > 5) return;
	//if(pos.y > 15) return;
	unsigned char x, y;
	for(x = 0; x < 5; ++x){
		for(y = 0; y < 3; ++y){
				setPixel(x+ax, y+ay,  pgm_read_byte(&numbers[num][4-x][y]));
		}
	}	

}


void drawLine(unsigned char y) {
	unsigned char x;
	if(y > 15) return;
	

	for(x = 0; x < 16; ++x) {
		setPixel(x, y, 3);
	}
}

void drawRow(unsigned char y) {
	unsigned char x;
	if(y > 15) return;
	

	for(x = 0; x < 16; ++x) {
		setPixel(y, x, 3);
	}
}

void test() {
	unsigned char x;
	for(x = 0; x < 16; ++x) {
		clear_screen(0);
		drawLine(x);
		wait(800);
	}

	for(x = 0; x < 16; ++x) {
		clear_screen(0);
		drawRow(x);
		wait(800);
	}
	
}


#define LINESPEED 20

void running() {
	
	
	pixel plPos = {7,7};
	unsigned char x,y,t, py, n;
	t=0;
	n = 5;
	while(t < 255) {
		clear_screen(0);
		py = t%LINESPEED;
		
		drawLine(py);
		drawLine(py+8);
		drawNumber(n, 15, py-4);
		//drawNumber(0, 15, ((py+4)%LINESPEED)-2);
		if(py ==0) n--;
		for(x = 0; x < 5; ++x){
			for(y = 0; y < 3; ++y){
				if(pgm_read_byte(&player[t%4][x][y])) {
					setPixel(x+plPos.x, y+plPos.y,  pgm_read_byte(&player[t%4][x][y]));
				}
			}
		}	
		t++;
		if(n > 5) break;
		wait(150);
	}
}



void showScores(unsigned char home, unsigned char guest) {
	char* hometxt = "</#FIRE";
	
	scrolltext(hometxt);
	char* tmp = "xp10#xx";
	tmp[5]= (home / 10)+0x30;
	tmp[6] = (home % 10)+0x30;
	scrolltext(tmp);

	char* guesttxt = ">/#GALAXY";
	scrolltext(guesttxt);
	tmp[5] = (guest / 10)+0x30;
	tmp[6] = (guest % 10)+0x30;
	scrolltext(tmp);

}









#define FEUER_Y (NUM_ROWS + 3)
//#define FEUER_Y (NUM_ROWS +3 ) -9
//#define FEUER_S 30
//#define FEUER_N 5
#undef FEUER_DIV 
#define FEUER_DIV 41

 void rhein_fire()
 {
 	unsigned char y, x;
	unsigned int  t;
	unsigned char world[NUM_COLS][FEUER_Y];   // double buffer
	
	clear_screen(0);
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
			world[x][FEUER_Y-1] = random();
		};
	
		

		// copy to screen
		for(y=0; y<NUM_ROWS; y++) {
			for(x=0; x<NUM_COLS; x++) {
				if(pgm_read_byte(&rhein[y][x]) != 0) {
					setPixel(x, y, (world[x][y] >> 5) );
				}
			}		
		};

		wait(FEUER_DELAY);
	}
 }


typedef struct{
	pixel start;
	unsigned char len;
	unsigned char decay;
	unsigned char index;
	unsigned char speed;
} streamer;

void rhein_matrix() {
	unsigned int counter = 500;//run 500 cycles
	streamer streamers[STREAMER_NUM];
	unsigned char matrix_bright[NUM_COLS][NUM_ROWS];
	unsigned char x, y;
	unsigned char index = 0;

	unsigned char draw;

	unsigned char streamer_num = 0;
	while(counter--){
		unsigned char i, j;
		for(x=0;x<NUM_COLS;x++)
			for(y=0;y<NUM_ROWS;y++)
				matrix_bright[x][y]=0;
		
		for(i=0;i<streamer_num;i++){
			streamer str = streamers[i];
			
			unsigned char bright = 255; draw = 0;
			for(j=(str.len>>3);j!=0xFF;j--){ //Draw streamer
				if(j+str.start.y<NUM_ROWS){
					if(bright>>6) draw = 1;
					if(bright > matrix_bright[str.start.x][str.start.y+j]){
						//setpixel((pixel){str.start.x, str.start.y+j}, bright>>6);
						matrix_bright[str.start.x][str.start.y+j] = bright;
					}
				}
				bright-=((bright>>5)*str.decay);
			}
			str.len+=str.speed>>1;
			streamers[i] = str;
			if(!draw){
				for(j=i;j<streamer_num-1;j++){
					streamers[j] = streamers[j+1];
				}
				streamer_num--;
				i--;
			};
						
		}
		
		//clear_screen(0);
		for(y=0;y<NUM_ROWS;y++) {
			for(x=0;x<NUM_COLS;x++) {
				if(pgm_read_byte(&rhein[y][x]) != 0) {
					//if((matrix_bright[x][y]>>6) == 0) {
						setPixel(x, y, pgm_read_byte(&rhein[y][x]));
					//} else {
					//	setPixel(x, y, (matrix_bright[x][y]>>6)-2 );
					//}
				} else {
					setPixel(x, y, (matrix_bright[x][y]>>6));
				}
			}
		}
		unsigned char nsc;
		for(nsc=0;nsc<6;nsc++){
			if(streamer_num<STREAMER_NUM){
				unsigned char sy = random()%(2*NUM_ROWS);
				if (sy>NUM_ROWS-1) sy=0;
				streamers[streamer_num] = (streamer){{random()%NUM_COLS, sy}, 0, (random()%8)+12, index++,(random()%16)+3};
				streamer_num++;	
			}
		}
		wait(80);	
		
	
	
	}

}
 
