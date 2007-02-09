#include <stdlib.h>

#include "pixel.h"
#include "util.h"
#include "config.h"

#ifdef AVR
	#include <avr/io.h>
#endif

#define RANDOM8() ((uint8_t)random())

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
	for(;;) wait(100);
}

void test_level2(){
	unsigned char x,y;
	for (y=0;y<NUM_ROWS;y++){
		for (x=0;x<NUM_COLS;x++){
			setpixel((pixel){x,y}, 2);
			wait(5);
		}
	}
	for(;;) wait(100);
}

void test_level3(){
	unsigned char x,y;
	for (y=0;y<NUM_ROWS;y++){
		for (x=0;x<NUM_COLS;x++){
			setpixel((pixel){x,y}, 3);
			wait(5);
		}
	}
	for(;;) wait(100);
}



void off()
{
	clear_screen(0);

	while(1)
		wait(100);

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

void schachbrett(unsigned char times){
	clear_screen(0);
	unsigned char j;
	for(j=0;j<times;j++){
		unsigned char i, x;
		for(i = 0; i<NUM_ROWS; i++){
			for(x=0;x<LINEBYTES;x++)
				pixmap[2][i][x] = 0x55<<(i&0x01);
		}
		wait(200);
		for(i = 0; i<NUM_ROWS; i++){
			for(x=0;x<LINEBYTES;x++)
				pixmap[2][i][x] = 0xAA>>(i&0x01);
		}
		wait(200);
	}
}

void fadein()
{
	unsigned char value, x, y;

	for(value=1; value < 4; value++) {
	 	for(y=0; y<NUM_ROWS; y++)
			for(x=0; x<NUM_COLS; x++) {
				setpixel( (pixel){x,y}, value );
				wait(10);
			}
	}

}

typedef struct{
	pixel start;
	unsigned char len;
	unsigned char decay;
	unsigned char index;
	unsigned char speed;
} streamer;

void matrix() {
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
		
		for(y=0;y<NUM_ROWS;y++)
			for(x=0;x<NUM_COLS;x++)
				setpixel((pixel){x,y}, matrix_bright[x][y]>>6);
		
		unsigned char nsc;
		for(nsc=0;nsc<6;nsc++){
			if(streamer_num<STREAMER_NUM){
				unsigned char sy = RANDOM8()%(2*NUM_ROWS);
				if (sy>NUM_ROWS-1) sy=0;
				streamers[streamer_num] = (streamer){{RANDOM8()%NUM_COLS, sy}, 0, (RANDOM8()%8)+12, index++,(RANDOM8()%16)+3};
				streamer_num++;	
			}
		}
		wait(80);	
		
	
	
	}
}

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

void tetris(){
	PORTB = 0x0f;
	clear_screen(0);
	unsigned char i;
	for (i=0;i<NUM_ROWS;i++){
		setpixel((pixel){8,i},3);
	}
	struct block{
		pixel pix[4];
	};	
	struct block press (struct block foobar,unsigned char taste){
			unsigned char go;
			struct block aktuell=foobar;
			if(taste == 14){
			
			}
			if(taste == 7){
				for(i=0;i<4;i++){
					if((get_pixel((pixel){aktuell.pix[i].x+1,aktuell.pix[i].y}))){
						go=0;
						break;
					}
					else {
						go=1;
					}
				}
				if(go){
					for(i=0;i<4;i++){
						aktuell.pix[i].x++;
					}
				}
			}
			if(taste == 11){
				for(i=0;i<4;i++){
					if(aktuell.pix[i].x==0){
						go=0;
						break;
					}
					else {
						go=1;
					}
				}
				if(go){
					for(i=0;i<4;i++){
						aktuell.pix[i].x--;
					}
				}
			}
		
		return aktuell;
	}
	struct block line = {{{0,0},{0,1},{0,2},{0,3}}};
	struct block square={{{0,0},{0,1},{1,0},{1,1}}};	
	struct block left={{{1,0},{1,1},{0,1},{0,2}}};
	void dropin(struct block foobar){
		struct block aktuell = foobar;
			unsigned char i;
			unsigned char h=1;
			unsigned char taste = PINB & 0x0F;
			aktuell = press(aktuell,taste);
			struct block next=aktuell;
			for(i=0;i<4;i++){
				setpixel(aktuell.pix[i],3);
			}
			wait(450);
			for(i=0;i<4;i++){
				clearpixel(aktuell.pix[i]);
			}
			for(i=0;i<4;i++){
				next.pix[i].y++;
			}
			for(i=0;i<4;i++){
				h = get_pixel(next.pix[i]);
				if(h){break;}
			}
			if (!h){
	  				  dropin(next);
			}else{
				for(i=0;i<4;i++){
					setpixel(aktuell.pix[i],3);
				}
			}
	}
	dropin(square);	
	dropin(line);
	dropin(left);
	dropin(square);	
	dropin(square);	
	dropin(line);
	dropin(left);
	dropin(square);	
	dropin(line);
	dropin(left);
	dropin(line);
	dropin(left);
}

