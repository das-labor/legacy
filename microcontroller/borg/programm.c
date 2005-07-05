
#include "pixel.h"
#include "util.h"

void test1(){
unsigned char x,y;
	for (y=0;y<8;y++){
		for (x=0;x<8;x++){
			setpixel((pixel){x,y}, 3);
			wait(200);
		}
	}
}


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
	set_cursor(&cur, (pixel){4,3});
	
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

			if( (apple_num<10) && (random()<10) ){
				pixel new_apple = (pixel){random()%8,random()%8};
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


void joern1(){
unsigned char i, j;
	for(i = 0; i< 80;i++){
		for(j = 0 ;j < 8; j++){
			pixmap[0][i%8]=shl_table[j];	
			pixmap[0][(i+1)%8]=shl_table[(j+1)%8];
			pixmap[0][(i+2)%8]=shl_table[(j+2)%8];
			pixmap[0][(i+3)%8]=shl_table[(j+3)%8];
			pixmap[0][(i+4)%8]=shl_table[(j+4)%8];
			pixmap[0][(i+5)%8]=shl_table[(j+5)%8];
			pixmap[0][(i+6)%8]=shl_table[(j+6)%8];
			pixmap[0][(i+7)%8]=shl_table[(j+7)%8];
		}
		wait(100);
	}
}

void joern2(){
	unsigned char i;
	for(i=0;i<20;i++){
		pixmap[0][0] = 0x0f;
		pixmap[0][1] = 0xf0;
		pixmap[0][2] = 0x0f;
		pixmap[0][3] = 0xf0;
		pixmap[0][4] = 0x0f;
		pixmap[0][5] = 0xf0;
		pixmap[0][6] = 0x0f;
		pixmap[0][7] = 0xf0;
		wait (100);
		pixmap[0][0] = 0xf0;
                pixmap[0][1] = 0x0f;
                pixmap[0][2] = 0xf0;
                pixmap[0][3] = 0x0f;
                pixmap[0][4] = 0xf0;
                pixmap[0][5] = 0x0f;
                pixmap[0][6] = 0xf0;
                pixmap[0][7] = 0x0f;
		wait (100);
	}
}

void draw_line( unsigned char yabs, signed char delta){
	cursor cur;
	cur.mode = set;
	set_cursor(&cur, (pixel){yabs, 0});
	unsigned char hr_x = cur.pos.x << 5;
	
		while ((cur.pos.x<8)&&(cur.pos.y<7)){
		hr_x +=delta;
		set_cursor(&cur, (pixel){hr_x>>5,cur.pos.y+1});
	};

}

void testline(){
	unsigned char i;
	for(i = 0; i<255; i++){
		draw_line(0,i);
		wait(100);
		clear_screen(0);
	}
		
}

void schachbrett(unsigned char times){
	unsigned char j;
	for(j=0;j<times;j++){
		unsigned char i;
		for(i = 0; i<8; i++){
			pixmap[0][i] = 0x55<<(i&0x01);
		}
		wait(200);
		for(i = 0; i<8; i++){
			pixmap[0][i] = 0xAA>>(i&0x01);
		}
		wait(200);
	}
}

void fadein()
{
	unsigned char value, x, y;

	for(value=1; value < 4; value++) {
	 	for(y=0; y<8; y++)
			for(x=0; x<8; x++) {
				setpixel( (pixel){x,y}, value );
				wait(20);
			}
	}

}


#define STREAMER_NUM 30

typedef struct{
	pixel start;
	unsigned char len;
	unsigned char decay;
	unsigned char index;
	unsigned char speed;
}streamer;

void matrix1(){
	int counter = 500;//run 1000 cycles
	streamer streamers[STREAMER_NUM];
	unsigned char matrix_bright[8][8];
	unsigned char x, y;
	unsigned char index = 0;

	unsigned char draw;

	unsigned char streamer_num = 0;
	while(counter--){
		unsigned char i, j;
		for(x=0;x<8;x++)
			for(y=0;y<8;y++)
				matrix_bright[x][y]=0;
		
		for(i=0;i<streamer_num;i++){
			streamer str = streamers[i];
			
			unsigned char bright = 255; draw = 0;
			for(j=(str.len>>4);j!=0xFF;j--){ //Draw streamer
				if(j+str.start.y<8){
					if(bright>>6) draw = 1;
					if(bright > matrix_bright[str.start.x][str.start.y+j]){
						setpixel((pixel){str.start.x, str.start.y+j}, bright>>6);
						matrix_bright[str.start.x][str.start.y+j] = bright;
					}
				}
				bright-=((bright>>5)*str.decay);
			}
			str.len+=str.speed;
			streamers[i] = str;
			if(!draw){
				for(j=i;j<streamer_num-1;j++){
					streamers[j] = streamers[j+1];
				}
				streamer_num--;
				i--;
			};
						
		}
		unsigned char nsc;
		for(nsc=0;nsc<6;nsc++){
			if(streamer_num<STREAMER_NUM){
				unsigned char sy = random()%16;
				if (sy>7) sy=0;
				streamers[streamer_num] = (streamer){{random()%8, sy}, 0, (random()%8)+12, index++,(random()%16)+3};
				streamer_num++;	
			}
		}
		wait(80);	
		
	
	
	}
}

void feuer()
{
	unsigned char y, x;
	unsigned int  t;
	unsigned char world[8][16];   // double buffer

	for(t=0; t<800; t++) {
		// diffuse
		for(y=1; y<16; y++) {
			for(x=1; x<7; x++) {
				world[x][y-1] = (2*world[x-1][y] + 8*world[x][y] + 2*world[x+1][y]) / 14;
			};

			world[0][y-1] = (2*world[7][y] + 8*world[0][y] + 2*world[1][y]) / 15;
			world[7][y-1] = (2*world[0][y] + 8*world[7][y] + 2*world[6][y]) / 15;
		};

		// update lowest line
		for(x=0; x<8; x++) {
			world[x][15] = random();
		};
	
		// copy to screen
		for(y=0; y<8; y++) {
			for(x=0; x<8; x++) {
				setpixel( (pixel){x,y}, (world[x][y+5] / 40) );
			}		
		};

		wait(70);
	}
}

