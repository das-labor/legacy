#include "pixel3d.h"
#include "programm.h"

void test1() {
	unsigned char x, y, z;
	clear_screen(0);
	for (z = 0; z < NUM_COLS; z++) {	
		for (y = 0; y < NUM_ROWS; y++) {
			for (x = 0; x < NUM_PLANES; x++) {
				setpixel3d((pixel3d){x,y,z}, 3);
				wait(15);
			}
		}
	}
}

void test2() {
	unsigned char i, j, k;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){i,j,k}, 3);
			}
		}
		wait(40);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){i,j,k}, 0);
			}
		}
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j,k,i}, 3);
			}
		}
		wait(40);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j,k,i}, 0);
			}
		}
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,i,j}, 3);
			}
		}
		wait(50);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,i,j}, 0);
			}
		}
	}
	for (i = 7; i < 8; i--) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,j,i}, 3);
			}
		}
		wait(40);
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,j,i}, 0);
			}
		}
	}
}

void test3() {
	unsigned char i, j, k;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){i,j,k}, 3);
			}
		}
		wait(40);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){i,j,k}, 0);
			}
		}
		wait(40);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j,k,i}, 3);
			}
		}
		wait(40);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j,k,i}, 0);
			}
		}
		wait(40);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,i,j}, 3);
			}
		}
		wait(50);
	}
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,i,j}, 0);
			}
		}
		wait(50);
	}
	for (i = 7; i < 8; i--) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,j,i}, 3);
			}
		}
		wait(40);
	}
	for (i = 7; i < 8; i--) {
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){k,j,i}, 0);
			}
		}
		wait(40);
	}
}


void growingCubeFilled() {
	unsigned char x, y, z;
	unsigned char min = 3, max = 4;
	while (min < NUM_PLANES && max < NUM_PLANES) {
		for (x = min; x <= max; x++) {
			for (y = min; y <= max; y++) {
				for (z = min; z <= max; z++) {
					setpixel3d((pixel3d){x,y,z}, 3);
				}
			}
		}
		wait(150);
		min--;
		max++;
	}
	min = 0;
	max = 7;
	while (min < 5 && max > 3) {
		for (x = min; x <= max; x++) {
			for (y = min; y <= max; y++) {
				for (z = min; z <= max; z++) {
					if (x == min || x == max || y == min || y == max || z == min || z == max)
						setpixel3d((pixel3d){x,y,z}, 0);
				}
			}
	
		}
		wait(150);
		min++;
		max--;
	}	
}

void coolAnim() {
	unsigned char x, y, z;
	unsigned char min = 3, max = 4;
	while (min < NUM_PLANES && max < NUM_PLANES) {
		for (x = 0; x <= 8; x++) {
			for (y = 0; y <= 8; y++) {
				for (z = 0; z <= 8; z++) {
					if (x == min || x == max || y == min || y == max || z == min || z == max)
						setpixel3d((pixel3d){x,y,z}, 3);
					else 
						setpixel3d((pixel3d){x,y,z}, 0);
				}
			}
		}
		wait(150);
		min--;
		max++;
	}
	min = 0;
	max = 7;
	while (min < 5 && max > 3) {
		for (x = 0; x <= 8; x++) {
			for (y = 0; y <= 8; y++) {
				for (z = 0; z <= 8; z++) {
					if (x == min || x == max || y == min || y == max || z == min || z == max)
						setpixel3d((pixel3d){x,y,z}, 3);
					else 
						setpixel3d((pixel3d){x,y,z}, 0);
				}
			}
	
		}
		wait(150);
		min++;
		max--;
	}	
}


#define BIT_S(var,b) ((var&(1<<b))?1:0)

unsigned char myrandom(){
	static unsigned int muh = 0xAA;
	unsigned char x;
	for (x=0; x<8; x++) {
		muh = (muh<<1) ^ BIT_S(muh,1) ^ BIT_S(muh,8) ^ BIT_S(muh,9) ^ BIT_S(muh,13) ^ BIT_S(muh,15);
	}
	return (unsigned char) muh;

}

#define SNAKE_LEN 256

void snake3d(){
	pixel3d pixels[SNAKE_LEN] = {{1, 1, 0},{1, 2, 0}};
	pixel3d *head = &pixels[1];
	pixel3d *tail = &pixels[0];
	pixel3d old_head;

	pixel3d apples[10];
	unsigned char apple_num = 0;
	
	direction dir = forward;

	clear_screen(0);

	unsigned char x = 0, dead = 0;
	while (1) {
		x++;
		old_head = *head;
		if(++head == pixels + SNAKE_LEN) head = pixels;
		
		unsigned char dead_cnt=0;
		
		unsigned char apple_found = 0, j;
		for(j=0;j<apple_num;j++){
			unsigned char i;
			for(i=0;i<6;i++){
				if ((next_pixel3d(old_head, (direction)i).x == apples[j].x) && 
					(next_pixel3d(old_head, (direction)i).y == apples[j].y) &&
				    (next_pixel3d(old_head, (direction)i).z == apples[j].z)) {
					apple_found = 1;
					dir = (direction)i;
					for(; j < apple_num-1; j++){
						apples[j] = apples[j+1];
					}
					apple_num--;
					goto apple_se;
				}
			}
		}
		apple_se:
		if (apple_found) {
		} else {
			while (get_next_pixel3d(old_head, dir)) {
				if ((dead_cnt++) == 4) {
					dead = 1;
					break;
				}
				dir = direction_r(dir);
			}
		}
		if (!dead) {
			*head = next_pixel3d(old_head, dir);
			setpixel3d(*head, 3);
			if (myrandom() < 80) {
				dir = (direction) (myrandom() % 6);
			}
			if((apple_num<10) && (myrandom()<10)) {
				pixel3d new_apple = (pixel3d){myrandom()%NUM_PLANES,
					                          myrandom()%NUM_ROWS,
					                          myrandom()%NUM_COLS};
				if (!get_pixel3d(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
			if(!apple_found){
				clearpixel3d(*tail);
				if(++tail == pixels + SNAKE_LEN) tail = pixels;
			}
		} else {
			while(tail != head){
				clearpixel3d(*tail);
				if((++tail)>pixels+SNAKE_LEN) tail = pixels;
				wait(60);
			}
			break;
		}
		for (j = 0; j < apple_num; j++) {
			if (x % 2){
				setpixel3d(apples[j], 3);
			} else {
				clearpixel3d(apples[j]);
			}
		}
		wait(5);
	}
}

/*
#define JOYUP		PD0	
#define JOYDOWN		PD1
#define JOYRIGHT	PD2
#define JOYLEFT		PD3
#define JOYFIRE		PD4
#define JOYPIN	 	PIND
#define JOYPORT	 	PORTD
#define JOYDDR  	DDRD

void snake3dJoystick(){
	pixel3d pixels[64] = {{1, 1, 0},{1, 2, 0}};
	pixel3d *head = &pixels[1];
	pixel3d *tail = &pixels[0];
	pixel3d old_head;
	
	JOYPORT = 0xFF;
	JOYDDR = 0x00;
	

	pixel3d apples[10];
	unsigned char apple_num = 0, tmp, j;
	unsigned char x = 0, dead = 0;
	unsigned char apple_found = 0;

	direction dir = forward;

	clear_screen(0);

	while (1) {
		x++;
		old_head = *head;
		if (++head == pixels + 64) 
			head = pixels;
		apple_found = 0;

		tmp = ~JOYPIN;
		if (tmp & shl_table[JOYUP]) {
			dir = turn_up(dir);
		} else if (tmp & shl_table[JOYDOWN]) {
			dir = turn_down(dir);
		} else if (tmp & shl_table[JOYRIGHT]) {
			dir = turn_left(dir);
		} else if (tmp & shl_table[JOYLEFT]) {
			dir = turn_right(dir);
		}
		if (!dead) {
			*head = next_pixel3d(old_head, dir);
			setpixel3d(*head, 3);
			for(j = 0; j < apple_num; j++) {
				if ((next_pixel3d(old_head, dir).x == apples[j].x) && 
					(next_pixel3d(old_head, dir).y == apples[j].y) &&
					(next_pixel3d(old_head, dir).z == apples[j].z)) {
					apple_found = 1;
				}
			}

			if ((apple_num<10) && (myrandom()<10)) {
				pixel3d new_apple = (pixel3d){myrandom()%NUM_PLANES,
					                          myrandom()%NUM_ROWS,
					                          myrandom()%NUM_COLS};
				if (!get_pixel3d(new_apple)){
					apples[apple_num++] = new_apple;
				}
			}
			if (!apple_found){
				clearpixel3d(*tail);
				if (++tail == pixels + 64) tail = pixels;
			}
		} else {
			while (tail != head){
				clearpixel3d(*tail);
				if ((++tail)>pixels+64) tail = pixels;
				wait(60);
			}
			break;
		}
		for (j = 0; j < apple_num; j++) {
			if (x % 2){
				setpixel3d(apples[j], 3);
			} else {
				clearpixel3d(apples[j]);
			}
		}
		wait(200);
	}
}
*/

typedef struct {
	pixel3d start;
	unsigned char len;
	unsigned char decay;
	unsigned char index;
	unsigned char speed;
} streamer;

void matrix() {
	unsigned int counter = 500; //run 500 cycles
	streamer streamers[STREAMER_NUM];
	unsigned char matrix_bright[NUM_PLANES*NUM_ROWS][NUM_COLS];
	unsigned char x, y;
	unsigned char index = 0;
	unsigned char draw;
	unsigned char streamer_num = 0;
	
	while (counter--) {
		unsigned char i, j;
		for (x = 0; x < (NUM_ROWS * NUM_PLANES); x++)
			for (y = 0; y < (NUM_COLS); y++)
				matrix_bright[x][y] = 0;
		
		for (i = 0; i < streamer_num; i++) {
			streamer str = streamers[i];
			
			unsigned char bright = 255; draw = 0;
			for(j = (str.len >> 3); j != 0xFF; j--) { //Draw streamer
				if (j+str.start.y < (NUM_COLS)){
					if (bright >> 6) 
						draw = 1;
					if (bright > matrix_bright[str.start.x][str.start.y+j]) {
						matrix_bright[str.start.x][str.start.y+j] = bright;
					}
				}
				bright -= ((bright >> 5) * str.decay);
			}
			str.len += str.speed>>1;
			streamers[i] = str;
			if (!draw) {
				for(j=i;j<streamer_num-1;j++){
					streamers[j] = streamers[j+1];
				}
				streamer_num--;
				i--;
			}			
		}
		
		for (y = 0; y < NUM_COLS; y++)
			for (x = 0; x < (NUM_ROWS*NUM_PLANES); x++)
				setpixel3d((pixel3d){x % NUM_ROWS, x / NUM_PLANES, NUM_COLS-y-1}, 
				                     matrix_bright[x][y] >> 6);
		
		unsigned char nsc;
		for (nsc = 0; nsc < 6; nsc++) {
			if (streamer_num < STREAMER_NUM){
				unsigned char sy = myrandom() % (2*NUM_COLS);
				if (sy > NUM_COLS-1) 
					sy=0;
				streamers[streamer_num] = 
				 			  (streamer){{myrandom()%(NUM_ROWS*NUM_PLANES), sy}, 
				                         0, (myrandom()%8)+12, index++,
										 (myrandom()%16)+3
										};
				streamer_num++;	
			}
		}
		wait(80);	
	}
} 

void drawLineZ(char x1, char y1, char x2, char y2, char z, char level) {
	int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;
	
	dx = x2 - x1;      // the horizontal distance of the line
	dy = y2 - y1;      // the vertical distance of the line 
	dxabs = dx >= 0 ? dx: -dx; //abs
	dyabs = dy >= 0 ? dy: -dy; //abs
	sdx = dx >= 0 ? 1: -1;     //sgn
	sdy = dy >= 0 ? 1: -1;     //sgn
	x = dyabs >> 1;
	y = dxabs >> 1;
	px = x1;
	py = y1;
	setpixel3d((pixel3d){x1, y1, z}, level);
	if (dxabs >= dyabs) { // the line is more horizontal than vertical  
		for (i = 0; i < dxabs; i++) {
			y += dyabs; 
			if (y >= dxabs) {
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			setpixel3d((pixel3d){px, py, z}, level);
		}
	} else { // the line is more vertical than horizontal
		for (i = 0; i < dyabs; i++) {
			x += dxabs;
			if (x >= dyabs) {
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			setpixel3d((pixel3d){px, py, z}, level);
		}
	}
}	  

void drawLineZAngle(unsigned char angle, unsigned char z, unsigned char value) {
	// could be optimised in programcode
	unsigned char x1[14] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6};
	unsigned char y1[14] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7};
	unsigned char x2[14] = {7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1};
	unsigned char y2[14] = {7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0};
	drawLineZ(x1[angle], y1[angle], x2[angle], y2[angle], z, value);	
}

void spirale() {
	unsigned char z, angle, count, angleAdd;
	for (count = 0, angleAdd = 0; angleAdd < 12; count++) {
		for (angle = 0; angle < 14; angle++) {
			for (z = 0; z < 8; z++) {
				drawLineZAngle((angle+(angleAdd*z/4))%14, z, 3);		
			}
			wait(30);
			clear_screen(0);
		
			if (count > 5) { 
				angleAdd++;
				count = 0;
			}
		}
	}
}

void spirale2() {
	unsigned char z, angle, count, angleAdd;
	for (count = 0, angleAdd = 0; angleAdd < 8; count++) {
		for (angle = 0; angle < 14; angle++) {
			for (z = 0; z < 8; z++) {
				drawLineZAngle((angle+((angleAdd*z)/4))%14, z, 3);
				drawLineZAngle((angle+7+((angleAdd*z)/4))%14, z, 3);
				
			}
			wait(40);
			clear_screen(0);
		
			if (count > 5) { 
				angleAdd++;
				count = 0;
			}
	 	}
	}
}


void *display_loop(void * unused) {
	while (1) {	
		spirale();
		spirale2();
		matrix();
		//snake3dJoystick();
		test3();
		coolAnim();
		growingCubeFilled();
		growingCubeFilled();
		coolAnim();
		test2();
		test2();
		test2();
		test2();
		snake3d();
	}
}
