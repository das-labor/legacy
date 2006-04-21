#include "pixel.h"
#include "util.h"
#include "scrolltext.h"
#include <avr/pgmspace.h>
#include "rhein.h"

/*

	IT'S A TERRIBLE HACK!!! ;-)

*/

#define FRAMESPEED 100
#define LINESPEED 10
//#define setPixel(_X, _Y, _V) if((((unsigned char)(_X)) < 16) && (((unsigned char)(_Y)) < 16 )) setpixel( (pixel){15 - _X, _Y}, _V)
#define setPixel(_X, _Y, _V) if(((unsigned char)(_Y)) < 16 ) setpixel( (pixel){15 - _X, _Y}, _V)
//#define setPixel(_X, _Y, _V) setpixel( (pixel){15 - _X, _Y}, _V)

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

unsigned char Smile1[8][8] PROGMEM= { 
  { 0, 0, 2, 2, 2, 2, 0, 0, },
  { 0, 2, 0, 0, 0, 0, 2, 0, },
  { 2, 0, 3, 0, 0, 3, 0, 2, },
  { 2, 0, 0, 0, 0, 0, 0, 2, },
  { 2, 0, 3, 0, 0, 3, 0, 2, },
  { 2, 0, 0, 3, 3, 0, 0, 2, },
  { 0, 2, 0, 0, 0, 0, 2, 0, },
  { 0, 0, 2, 2, 2, 2, 0, 0, }
  };

unsigned char Smile2[8][8] PROGMEM= { 
  { 0, 0, 2, 2, 2, 2, 0, 0, },
  { 0, 2, 0, 0, 0, 0, 2, 0, },
  { 2, 0, 3, 0, 0, 3, 0, 2, },
  { 2, 0, 0, 0, 0, 0, 0, 2, },
  { 2, 0, 0, 3, 3, 0, 0, 2, },
  { 2, 0, 3, 0, 0, 3, 0, 2, },
  { 0, 2, 0, 0, 0, 0, 2, 0, },
  { 0, 0, 2, 2, 2, 2, 0, 0, }
  };

void drawSmiley(signed char off_x, signed char off_y, unsigned char smil[8][8] ) {
	unsigned char x,y;

	clear_screen(0);
	for(x = 0; x < 8; x++) {
 		for(y = 0; y < 8; y++) {
 			setPixel(x + off_x, y+ off_y, pgm_read_byte(&smil[y][x]));
 		}
 	}
 

}

void dropSmiley(unsigned char smil)
{
	unsigned char *smiley = &Smile1;
	switch(smil) {
		case SMILING: 
				smiley = &Smile1;
				break;
		case SAD:
				smiley = &Smile2;
				break;
	}

	signed char i = -8;

	for(i = -8; i < 23; i++) {
		drawSmiley( 12, i, smiley);
		wait(80);		
	}

}

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

// Kleine Nummern an den 10 Yard Linien
unsigned char numbers[7][5][3] PROGMEM =
{
	// 0 
	{	{3,3,3},
		{3,0,3},
		{3,0,3},
		{3,0,3},
		{3,3,3}
	},

	// 1 
	{	{0,3,3},
		{3,0,3},
		{0,0,3},
		{0,0,3},
		{0,0,3}
	},

	// 2 
	{	{3,3,3},
		{0,0,3},
		{0,3,0},
		{3,0,0},
		{3,3,3}
	},

	// 3 
	{	{3,3,3},
		{0,0,3},
		{0,3,3},
		{0,0,3},
		{3,3,3}
	},

	// 4 
	{	{3,0,3},
		{3,0,3},
		{3,3,3},
		{0,0,3},
		{0,0,3}
	},

	// 5 
	{	{3,3,3},
		{3,0,0},
		{3,3,3},
		{0,0,3},
		{3,3,3}
	},

	// G 
	{	{2,3,2},
		{3,0,0},
		{3,2,3},
		{3,0,3},
		{2,3,2}
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

//	{{0,1,0},
//	{0,2,0},
//	{0,3,0},
//	{0,2,0},
//	{0,1,0}},

	{{1,0,0},
	{0,2,0},
	{0,3,0},
	{0,2,2},
	{0,1,0}},

	{{0,1,0},
	{2,2,0},
	{0,3,0},
	{0,2,0},
	{0,1,0}}

};

unsigned char player2[2][3][4] PROGMEM = 
{
	{{0,3,0,0},
	{3,3,3,0},
	{0,0,3,3}},

	{{0,0,3,3},
	{3,3,3,0},
	{0,3,0,0}}
};

void drawNumber(unsigned char num, signed char ax, signed char ay) {
	if(num > 6) return;
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


// Zeichnet das Spielfeld, jeweils mit dem aktuellen Yardstand in Zeile 0
// Halfyard 0-240 sein: 
//   0 = Tochdownzone Ende
//  20 = Tochdownzone Anfang
// 120 = 50 Yard linie
void drawField(unsigned char halfyard) {
	unsigned char x;
	unsigned char y;
	unsigned char n;

	// Alle 5 Yard eine Linie
	for(y = 0; y < 16; ++y){
		if ((( halfyard + 20 + y ) % 10 ) == 0 )
			if ((( halfyard + y) / 10 ) !=0)
				drawLine(y);
	}	
	// Die Null unter jeder 10er Linie	
	for(y = 0; y < 19; ++y){
		if ((( halfyard + 20 + y - 4 ) % 20 ) == 0 ){
			if ((( halfyard + 20 + y - 4 ) / 20 ) > 2)
				drawNumber(0, 15, y - 2);
			if ((( halfyard + 20 + y - 4 ) / 20 ) == 2)
				drawNumber(6, 15, y - 2);
		}
	}	

	// Die Zahl ber jeder 10er Linie	
	for(y = 0; y < 19; ++y){
		if ((( halfyard + 20 + y - 18 ) % 20 ) == 0 ){
			n = (( halfyard + 20 + y - 18 ) / 20 ) - 1;
			if (n > 0)
				drawNumber(n, 15, y - 2);
		}
	}	
	// Endzone
	if ( halfyard < 20 ){
		for(y = 0; y < 16; ++y){
			if ((( halfyard + y) < 20) && (( halfyard + y) > 10)){
				for(x = ((( halfyard + y ) % 2 ) * 2); x < 16; x+=4){
					setPixel(x, y, 1);		
				}
			}
		}
	}
}
/*		for(y = 0; y < 16; ++y){
			if (( halfyard + y) < 20){
				if ((( halfyard + y) % 4 ) == 0){
					setPixel(1, ( halfyard + y), 1);
					setPixel(5, ( halfyard + y), 1);
					setPixel(9, ( halfyard + y), 1);
					setPixel(13, ( halfyard + y), 1);
				} 
				if (((( halfyard + y) % 4 ) +2 ) == 0){
					setPixel(3, ( halfyard + y), 1);
					setPixel(7, ( halfyard + y), 1);
					setPixel(11, ( halfyard + y), 1);
					setPixel(15, ( halfyard + y), 1);
				}

			}
		}
*/

void running() {

	// Deklarationen
	pixel         player_pos;	
	unsigned char yard;
	unsigned char x;
	unsigned char y;
	unsigned char t;

	// ***************************************************************
	// *** Zuerst dern Player von unten in das Bild laufen lassen  ***
	// ***************************************************************
	// Hier l?t der Player von der 60 zur 55 Yard linie und in den letzten 
	// 5 Schritten, sollte die Linie zusammen mit der Null von oben reinscrollen.
	player_pos.x = 7;
	player_pos.y = 19;
	t=0;
	
	while(player_pos.y > 7){
		// Matrix lschen
		clear_screen(0);

		drawField(109);

		// Stepcounter
		t++;

		// Y-Position vom Player von unten bis hin zur Mitte
		player_pos.y--;	

		// Animation vom laufenden Player 
		// 4 verschiedene Player Bilder der Gr? 5 * 3
		for(x = 0; x < 5; x++){
			for(y = 0; y < 3; y++){
				if(pgm_read_byte(&player[t%4][x][y])) {
					setPixel(x+player_pos.x, y+player_pos.y, pgm_read_byte(&player[t%4][x][y]));
				}
			}
		}

		// Pause
		wait(FRAMESPEED);
	}
	
	// ***************************************************
	// *** Jetzt ist der Player auf (7, 7)             ***
	// *** und oben im Bild ist die 45 Yard Linie      ***
	// ***************************************************	
	t=0;
	yard=109;
	while(t < 255) {
		clear_screen(0);

		drawField(yard);

		// Animation vom laufenden Player
		// 4 verschiedene Player Bilder der Gr? 5 * 3
		for(x = 0; x < 5; x++){
			for(y = 0; y < 3; y++){
				if(pgm_read_byte(&player[t%4][x][y])) {
					setPixel(x+player_pos.x, y+player_pos.y, pgm_read_byte(&player[t%4][x][y]));
				}
			}
		}

//		// Animation vom laufenden Player 2		
//		for(x = 0; x < 3; x++){
//			for(y = 0; y < 4; y++){
//				if(pgm_read_byte(&player2[t%2][x][y])) {
//					setPixel(x+player_pos.x, y+player_pos.y, pgm_read_byte(&player2[t%2][x][y]));
//				}
//			}
//		}

		if(yard < 8) break;
		yard--;
		t++;		
		wait(FRAMESPEED);
	}
	wait(1000);
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

 void rhein_fire(int duration){

 	unsigned char y, x;
	unsigned int  t;
	unsigned char world[NUM_COLS][FEUER_Y];   // double buffer
	
	clear_screen(0);
	for(t=0; t<duration; t++) {
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
 
