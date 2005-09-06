#include "pixel.h"
#include "util.h"
#include "config.h"
#include "borg_hw.h"
#include <curses.h>
//#include <avr/io.h>
void test1(){
unsigned char x,y;
	for (y=0;y<NUM_ROWS;y++){
		for (x=0;x<NUM_COLS;x++){
			setpixel((pixel){x,y}, 3);
			wait(100);
		}
	}
}

/*
void spirale(unsigned int delay){
	clear_screen(0);

	cursor cur;
	cur.dir = right;
	cur.mode = set;
	set_cursor (&cur, (pixel){0,0});
	
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
	set_cursor(&cur, (pixel){NUM_COLS/2,(NUM_ROWS/2)-1});
	
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
*/

#define BIT_S(var,b) ((var&(1<<b))?1:0)

unsigned char random(){
	static unsigned int muh = 0xAA;
	unsigned char x;
	for(x=0;x<8;x++){
		muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ BIT_S(muh,13) ^ BIT_S(muh,15);
	}
	return (unsigned char) muh;

}
void snake(){
	pixel pixels[64] = {{4,6},{4,5}};
	pixel * head = &pixels[1];
	pixel * tail = &pixels[0];
	pixel old_head;

	pixel apples[10];
	unsigned char apple_num = 0;
	
	direction dir = up;

	clear_screen(0);

	unsigned char x=0, dead=0;
	while(1){
		x++;
		old_head = *head;
		if(++head == pixels + 64) head = pixels;
		
		unsigned char dead_cnt=0;
		
		unsigned char apple_found = 0, j;
		for(j=0;j<apple_num;j++){
			unsigned char i;
			for(i=0;i<4;i++){
				if ( (next_pixel(old_head, i).x == apples[j].x) && (next_pixel(old_head, i).y == apples[j].y) ){
					apple_found = 1;
					dir = i;
					for(;j<apple_num-1;j++){
						apples[j]=apples[j+1];
					}
					apple_num--;
					goto apple_se;
				}
			}
		}
		apple_se:

		if(apple_found){
		
		}else{
			while(get_next_pixel(old_head, dir)){
				if((dead_cnt++)==4){
					dead = 1;
					break;
				}
				dir = direction_r(dir);
			}
		}

		if(!dead){
			*head = next_pixel(old_head, dir);
			setpixel(*head, 3);
		
			if(random()<80){
				dir = random()%4;
			}

			if( (apple_num<9) && (random()<10) ){
				pixel new_apple = (pixel){random()%NUM_COLS,random()%NUM_ROWS};
				if(!get_pixel(new_apple)){
					apples[apple_num++]=new_apple;
				}
			}

			if(!apple_found){
				clearpixel(*tail);
				if(++tail == pixels + 64) tail = pixels;
			}
		}else{
			while(tail != head){
				clearpixel(*tail);
				if((++tail)>pixels+64) tail = pixels;
				wait (60);
			}
			break;
		}
		
		for(j=0;j<apple_num;j++){
			if(x%2){
				setpixel(apples[j], 3);
			}else{
				clearpixel(apples[j]);
			}
		}
		
		wait (100);
	}

}

/*
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
*/
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
/*
*/
typedef struct{
	pixel start;
	unsigned char len;
	unsigned char decay;
	unsigned char index;
	unsigned char speed;
}streamer;

void matrix(){
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
				unsigned char sy = random()%(2*NUM_ROWS);
				if (sy>NUM_ROWS-1) sy=0;
				streamers[streamer_num] = (streamer){{random()%NUM_COLS, sy}, 0, (random()%8)+12, index++,(random()%16)+3};
				streamer_num++;	
			}
		}
		//wait(80);	
		
	
	
	}
}

#define FEUER_Y (NUM_ROWS + 3)
#define FEUER_S 30
#define FEUER_N 5
#define FEUER_DIV 44
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
			world[x][FEUER_Y-1] = random();
		};
	
		// copy to screen
		for(y=0; y<NUM_ROWS; y++) {
			for(x=0; x<NUM_COLS; x++) {
				setpixel( (pixel){x,y}, (world[x][y] >> 5) );
			}		
		};

		wait(70);
	}
}

#define TET_WIDTH 8
void tetris(){
	clear_screen(0);
	unsigned char i;
	for (i=0;i<NUM_ROWS;i++){
		setpixel((pixel){TET_WIDTH,i},3);
	}
	struct block{
		pixel pix[4];
	};	
	struct block press (struct block foobar,int taste){
			unsigned char go;
			struct block aktuell=foobar;
			if(taste == 14){
			
			}
			if(taste == 's'){
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
			if(taste == 'a'){
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
	struct block right={{{1,0},{1,1},{0,1},{0,2}}};
	void dropin(struct block foobar){
		struct block aktuell = foobar;
			unsigned char i;
			unsigned char h=1;
			int taste = getch();
			aktuell = press(aktuell,taste);
			struct block next=aktuell;
			for(i=0;i<4;i++){
				setpixel(aktuell.pix[i],3);
			}
			wait(10000);
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
	void move_down(unsigned char l){
		unsigned char i,j;
		unsigned char k=0;
		for(i=l;i>1;i--){
			for(j=0;j<TET_WIDTH;j++){
				setpixel((pixel){j,i},get_pixel((pixel){j,i-1})*3);
			}	
		} 
		for(i=0;i<TET_WIDTH;i++){
			setpixel((pixel){0,i},0);
		}
		for(j=0;j<TET_WIDTH;j++){
			i = get_pixel((pixel){j,l});
			if(i == 1){
					  k++;
			}
		}
		if (k==TET_WIDTH){
			move_down(l);
		}
	}
	void checklines(void){
		unsigned char i,j,k=0;
		for(i=NUM_ROWS;i>0;i--){
			k=0;
			for(j=0;j<TET_WIDTH;j++){
				unsigned char l;
				l = get_pixel((pixel){j,i});
				if(l == 1){
						  k++;
				}
			}
			if (k==TET_WIDTH){
				move_down(i);
			}
		}
	}
	dropin(square);	
	checklines();
	dropin(line);
	checklines();
	dropin(left);
	checklines();
	dropin(square);	
	checklines();
	dropin(square);	
	checklines();
	dropin(line);
	checklines();
	dropin(left);
	checklines();
	dropin(square);	
	checklines();
	dropin(line);
	checklines();
	dropin(left);
	checklines();
	dropin(line);
	checklines();
	dropin(left);
	checklines();
}

