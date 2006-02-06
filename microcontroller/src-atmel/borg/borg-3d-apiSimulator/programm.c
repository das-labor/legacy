#include "pixel3d.h"
#include "programm.h"
#include "scrolltext2.h"
#include "joystick.h"

extern unsigned char shl_table[];


#ifndef AVR
#	define itoa(buf,i) sprintf(buf, "%d", i)
#endif

extern char joy1_up, joy1_down, joy1_right, joy1_left;

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

#define SNAKE_LEN 256

void snake3d(){
	pixel3d pixels[SNAKE_LEN] = {{1, 1, 0},{1, 2, 0}};
	pixel3d *head = &pixels[1];
	pixel3d *tail = &pixels[0];
	pixel3d old_head;

	pixel3d apples[10];
	unsigned char apple_num = 0;
	unsigned char x = 0, dead = 0;
	unsigned char dead_cnt;
	unsigned char apple_found = 0, j;
	
	direction dir = forward;

	clear_screen(0);
	
	while (1) {
		x++;
		old_head = *head;
		if(++head == pixels + SNAKE_LEN) head = pixels;
		
		dead_cnt=0;
		apple_found = 0;
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
			if (easyRandom() < 80) {
				dir = (direction) (easyRandom() % 6);
			}
			if((apple_num<10) && (easyRandom()<10)) {
				pixel3d new_apple = (pixel3d){easyRandom()%NUM_PLANES,
					                          easyRandom()%NUM_ROWS,
					                          easyRandom()%NUM_COLS};
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

			if ((apple_num<10) && (easyRandom()<10)) {
				pixel3d new_apple = (pixel3d){easyRandom()%NUM_PLANES,
					                          easyRandom()%NUM_ROWS,
					                          easyRandom()%NUM_COLS};
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
	unsigned char nsc;
	unsigned char bright;
	
	while (counter--) {
		unsigned char i, j;
		for (x = 0; x < (NUM_ROWS * NUM_PLANES); x++)
			for (y = 0; y < (NUM_COLS); y++)
				matrix_bright[x][y] = 0;
		
		for (i = 0; i < streamer_num; i++) {
			streamer str = streamers[i];
			
			bright = 255;
			draw = 0;
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
		
		
		for (nsc = 0; nsc < 6; nsc++) {
			if (streamer_num < STREAMER_NUM){
				unsigned char sy = easyRandom() % (2*NUM_COLS);
				if (sy > NUM_COLS-1) 
					sy=0;
				streamers[streamer_num] = 
				 			  (streamer){{easyRandom()%(NUM_ROWS*NUM_PLANES), sy}, 
				                         0, (easyRandom()%8)+12, index++,
										 (easyRandom()%16)+3
										};
				streamer_num++;	
			}
		}
		wait(80);	
	}
}   

void drawLineZAngle(unsigned char angle, unsigned char z, unsigned char value) {
	// could be optimised in programcode
	unsigned char x1[14] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6};
	unsigned char y1[14] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7};
	unsigned char x2[14] = {7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1};
	unsigned char y2[14] = {7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0};
	drawLine3D(x1[angle], y1[angle], z, x2[angle], y2[angle], z, value);	
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

void joern1(){
unsigned char i, j, x;
	unsigned char rolr=1 , rol, rol2;
	clear_screen(0);
	for (i = 0; i< 150;i++){
		rol2 = rolr;
		for (j = 0; j < NUM_PLANES; j++) {
			rol = rol2;
            for (x=0; x < PLANEBYTES; x++) { 
				pixmap[2][j][x] = rol;
			    if((rol<<=1)==0) rol = 1;
            }
            if((rol2<<=1)==0) rol2 = 1;
		}
		if ((rolr<<=1) == 0) rolr = 1;
		wait(100);
	}
	clear_screen(0);
}

void gameOfLife(unsigned char anim, unsigned int delay) {
	unsigned char gen, erg, seven = 1, maxGen;
	signed char x, y, z, neighC, r1 = 3, r2 = 4, r3 = 3, r4 = 2;
	signed char i, j, k;
	char neighs[NUM_PLANES][NUM_ROWS][NUM_COLS];
	
	clear_screen(0);

	switch (anim) {
	case 0:	maxGen = 27;
		setpixel3d((pixel3d){3, 4, 4}, 3);
		setpixel3d((pixel3d){4, 4, 4}, 3);
		setpixel3d((pixel3d){5, 4, 4}, 3);
		break;
	case 1:	maxGen = 30; // other rules
		r3 = 2;
		r4 = 1;
		setpixel3d((pixel3d){3, 4, 4}, 3);
		setpixel3d((pixel3d){4, 4, 4}, 3);
		setpixel3d((pixel3d){5, 4, 4}, 3);
		break;
	case 2: maxGen = 200;
		seven = 0;
		setpixel3d((pixel3d){2, 4, 4}, 3);
		setpixel3d((pixel3d){3, 4, 4}, 3);
		setpixel3d((pixel3d){4, 4, 4}, 3);
		setpixel3d((pixel3d){5, 4, 4}, 3);
		break;
	default:
		return;
	}
	
	for (gen = 0; gen < maxGen; gen++) {	
		for (x = seven; x < NUM_PLANES; x++) {	
			for (y = seven; y < NUM_ROWS; y++) {
				for (z = seven; z < NUM_COLS; z++) {
					neighC = 0;
					for (i = -1; i < 2; i++) {
						for (j = -1; j < 2; j++) {
							for (k = -1; k < 2; k++) {
								if (i != 0 || j != 0 || k != 0) {
									if ((x+i >= seven && x+i < NUM_COLS) && 
										(y+j >= seven && y+j < NUM_ROWS) && 
										(z+k >= seven && z+k < NUM_PLANES)) {
										erg = get_pixel3d((pixel3d){x+i, y+j, z+k});
									} else {
										erg = 0;
									}
									if (erg && erg != 255) {
										neighC++;	
									}
								}
							}
						}
					}
					neighs[x][y][z] = neighC;
				}
			}
		}
		for (x = 1; x < NUM_PLANES; x++) {	
			for (y = 1; y < NUM_ROWS; y++) {
				for (z = 1; z < NUM_COLS; z++) {
					if (get_pixel3d((pixel3d){x, y, z})) { // Feld gesetzt
						if (neighs[x][y][z] > r3 || neighs[x][y][z] < r4) {
							setpixel3d((pixel3d){x, y, z}, 0);
						} 
					} else {
						if (neighs[x][y][z] >= r1 && neighs[x][y][z] <= r2) {
							setpixel3d((pixel3d){x, y, z}, 3);
						}
					}
				}
			}
		}
		wait(delay);
	}			
}


void movingArrows() {
     unsigned char i, j, cnt;
     for (cnt = 0; cnt < 100; cnt++) {
         shift3d(up);
         for (i = 0; i < NUM_ROWS; i++) {
             for (j = 0; j < NUM_COLS; j++) {
                 setpixel3d((pixel3d) {i, j, 0}, ((3-i == cnt%4 || i-4 == cnt%4)
										 && (3-j == cnt%4 || j-4 == cnt%4))?3:0);
             }
         }
         wait(120);
     }
     clear_screen(0);

}

#define FEUER_Y (NUM_ROWS + 3)
#define FEUER_S 25
#define FEUER_N 2
#define FEUER_N2 1
#define FEUER_DIV 42;

void feuer()
{
	unsigned char z, y, x;
	unsigned int  t;
	unsigned char world[NUM_COLS][NUM_ROWS][FEUER_Y];   // double buffer

	for(t=0; t<800; t++) {
		// update lowest line
		for(x=0; x<NUM_COLS; x++) {
            for(y=0; y<NUM_ROWS; y++) {
                world[x][y][FEUER_Y-1] = easyRandom();
            }
		}

		// diffuse
		for(z=1; z<FEUER_Y; z++) {
			for(x=0; x<NUM_COLS; x++) {
                for (y=0; y<NUM_ROWS; y++) {
    				world[x][y][z-1] = (FEUER_N*world[(x-1)%NUM_COLS][y][z] + FEUER_N*world[(x+1)%NUM_COLS][y][z] + 
                                        FEUER_N*world[x][(y-1)%NUM_ROWS][z] + FEUER_N*world[x][(y+1)%NUM_ROWS][z] +
                                        FEUER_N2*world[(x-1)%NUM_COLS][(y-1)%NUM_ROWS][z] + FEUER_N2*world[(x+1)%NUM_COLS][(y-1)%NUM_ROWS][z] + 
                                        FEUER_N2*world[(x-1)%NUM_COLS][(y+1)%NUM_ROWS][z] + FEUER_N2*world[(x+1)%NUM_COLS][(y+1)%NUM_ROWS][z] + 
                                        FEUER_S*world[x][y][z]) / FEUER_DIV;
                }
			}
		}

		// copy to screen
		for(z=0; z<NUM_ROWS; z++) {
			for(x=0; x<NUM_COLS; x++) {
                for(y=0; y<NUM_ROWS; y++) {
				    setpixel3d((pixel3d){x,y,7-z}, world[x][y][z] >> 5 );
                }
			}		
		}
		wait(80);
	}
}


/*
void pong() {
    int erg;
    unsigned char posy1 = 4, posz1 = 3, lives = 3;
	char ran, diff, i, j, comp, hstr[] = "3 lives left";	
	pixel3d ballPos128 = {3*16, 4*16, 5*16}, ballPos, ballPosOld, ballDir = {4, 2, 1};
	//scrolltext("play PONG3D?  Then press UP" , 70);
	while (!joy1_up) wait(5);
 	for (i = 5; i >= 0; i--) {
		itoa(hstr, i);
		scrolltext(hstr, 110);
	}*//*
	for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            setpixel3d((pixel3d) {0, posy1+i, posz1+j}, 1);   
        }
    }
	while (lives) {
		ballPos.x = (ballPos128.x+8) / 16;
		ballPos.y = (ballPos128.y+8) / 16;
		ballPos.z = (ballPos128.z+8) / 16;
		//if (ballDir.z > -7)
		//	ballDir.z--;
		if (ballPos.x == 0) 
            if (ballPos.y >= posy1 && ballPos.y < posy1+4 && 
				ballPos.z >= posz1 && ballPos.z < posz1+4) {

				ballDir.y += ballPos.y - posy1 + 1;					
				ballDir.z += ballPos.z - posz1 + 1;
					
				ballDir.x = - ballDir.x;
				switch (ballPos.y - posy1) {
                    case 0: ballDir.y -= ballDir.y > -7 ? 1 : 0; 
                            ballDir.z += ballDir.z <  7 ? 1 : 0; break;
                    case 2: ballDir.y += ballDir.y <  7 ? 1 : 0;
                            ballDir.z -= ballDir.z > -7 ? 1 : 0; break;
                }
                switch (ballPos.z - posz1) {
                    case 0: ballDir.z -= ballDir.z > -7 ? 1 : 0;
                            ballDir.x += ballDir.x <  7 ? 1 : 0; break;
                    case 2: ballDir.z += ballDir.z <  7 ? 1 : 0;
                            ballDir.x -= ballDir.x > -7 ? 1 : 0; break;
                }
			} else {
				if (--lives) {
					hstr[0] = lives + '0';
					hstr[1] = ' ';
					scrolltext(hstr, 110);
					ballPos128 = (pixel3d) 	{3*16, 4*16, 5*16};
					ballDir = (pixel3d) {4, 2, 1};
				} else {
					scrolltext("GAME OVER", 110);
				}
			}
		if (ballPos.x == 7) 
			ballDir.x = - ballDir.x;			
		if (ballPos.y == 0 || ballPos.y == 7)
			ballDir.y = - ballDir.y;
		if (ballPos.z == 0 || ballPos.z == 7)
			ballDir.z = - ballDir.z;
		
		//printf("x=%d y=%d z=%d   dirz = %d \n", ballPos.x, ballPos.y, ballPos.z, ballDir.z);

		setpixel3d(ballPos, 3);
		
		if (joy1_up > 0 || joy1_down > 0 || joy1_right > 0 || joy1_left > 0) {
        	for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    setpixel3d((pixel3d) {0, posy1+i, posz1+j}, 0);   
                }
            }
            if (joy1_up) {
               if (posz1 < 5) posz1++;
               joy1_up = 0;
            }
            if (joy1_down) {
               if (posz1 > 0) posz1--;
               joy1_down = 0;
            }             
            if (joy1_right) {
               if (posy1 > 0) posy1--;
               joy1_right = 0;
            }
            if (joy1_left) {
               if (posy1 < 5) posy1++;
               joy1_left = 0;
            }
           	for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    setpixel3d((pixel3d) {0, posy1+i, posz1+j}, 1);   
                }
            }            
        }
        wait(70);
        ballPos128.x += ballDir.x;
		ballPos128.y += ballDir.y;
		ballPos128.z += ballDir.z ;
		clearpixel3d(ballPos);
		ballPosOld = ballPos;
	}
}
*/
void drawPixmapZ(char x1, char y1, char x2, char y2, unsigned char* pixmap, char level) {
	signed char i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;
	unsigned char z, j=0;
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
	for (z = 0; z < 8; z++) {
		if (pixmap[z] & shl_table[j])
			setpixel3d((pixel3d){x1, y1, z}, level);
	}
	j++;
	
	if (dxabs >= dyabs) { // the line is more horizontal than vertical  
		for (i = 0; i < dxabs; i++) {
			y += dyabs; 
			if (y >= dxabs) {
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			for (z = 0; z < 8; z++) {
				if (pixmap[z] & shl_table[j])
					setpixel3d((pixel3d){px, py, z}, level);
			}
			j++;
		}
	} else { // the line is more vertical than horizontal
		for (i = 0; i < dyabs; i++) {
			x += dxabs;
			if (x >= dyabs) {
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			for (z = 0; z < 8; z++) {
				if (pixmap[z] & shl_table[j])
					setpixel3d((pixel3d){px, py, z}, level);	
			}
			j++;
		}
	}
}	  

void drawPixmapZAngle(unsigned char angle, unsigned char* pixmap, unsigned char value) {
	// could be optimised in programcode
	unsigned char x1[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 
						  7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1};
	unsigned char y1[] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 
		                  7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0};
	unsigned char x2[] = {7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1, 
		                  0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6};
	unsigned char y2[] = {7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 
		                  0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7};
	drawPixmapZ(x1[angle], y1[angle], x2[angle], y2[angle], pixmap, value);	
}

// Rotate pixmap
void rotatePixmap(char animatioNo) {
	// Eine super tolle neue Animation
	                          //unten                              oben                                                           
	unsigned char yingYang[8] = {0x3c, 0x46, 0xa7, 0x8f, 0x9f, 0x9b, 0x5e, 0x3c}; 
	unsigned char heart[8] =    {0x18, 0x3c, 0x7e, 0xff, 0xff, 0xff, 0x66, 0x00}; 
    unsigned char L[8] =        {0xff, 0x81, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xff};
	unsigned char* pixmap;
	unsigned char i;
	
	switch (animatioNo) {
           case  0: pixmap = heart; break;
           case  1: pixmap = L; break;
           default: pixmap = yingYang; break;
    }
	
	for (i = 0; i < 255; i++) {
		drawPixmapZAngle(i%28, pixmap, 3);
		wait(50);
		clear_screen(0);
	}
    drawPixmapZAngle(i%28, pixmap, 3);
	wait(50);
	clear_screen(0);
    wait(50);
    drawPixmapZAngle(i%28, pixmap, 3);
	wait(50);
    clear_screen(0);
    wait(50);
    drawPixmapZAngle(i%28, pixmap, 3);
   	wait(50);
    clear_screen(0);
    wait(50);
    drawPixmapZAngle(i%28, pixmap, 3);
	wait(500);
	clear_screen(0);
}

void newCoolAnim() {
	unsigned char indexX[] = {1, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 6, 5, 4, 3, 2};
	unsigned char indexY[] = {1, 2, 3, 4, 5, 6 ,7, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0};
	unsigned char i, j;
		
	clear_screen(0);
	for (i = 0; i < 200; i++) {
		for (j = 0; j < 20; j++) {
			if (j < 6 || (j < 14 && j > 9))
				setpixel3d((pixel3d) {indexX[(j+i)%20], indexY[(j+i)%20], 0}, 3);
		}
		wait(50);
		shift3d(up);
	}
}

#define BWIDTH 8
void fadedPlaneScan(direction dir, unsigned char i)
{
		if(i < BWIDTH) set_plane(dir, 	i, 	3);
		if(i >= 1 && i < BWIDTH+1) set_plane(dir, 	i-1, 	2);
		if(i >= 2 && i < BWIDTH+2) set_plane(dir, 	i-2, 	1);
		if(i >= 3 && i < BWIDTH+3) set_plane(dir, 	i-3, 	0);
}

void planeAnimation2(unsigned char ms)
{
	unsigned char i, j; 
	for (j = 0; j < 4; ++j) {
		for(i=0; i < BWIDTH + 3; ++i) {
			fadedPlaneScan(left, i);
			fadedPlaneScan(down, i);
			fadedPlaneScan(back, i);
			wait(ms);
		}
		
		for(i=0; i < BWIDTH + 3; ++i) {
			fadedPlaneScan(right, i);
			fadedPlaneScan(up, i);
			fadedPlaneScan(forward, i);
			wait(ms);
		}
	}
}


#define NPOINTS 8
#define NLINES 12
void rotatedScaledCube() {
	pixel3d org[NPOINTS] = {{0x10, 0x10, 0x10}, // 0
							{0x60, 0x10, 0x10}, // 1
							{0x60, 0x60, 0x10}, // 2 
							{0x10, 0x60, 0x10}, // 3
							{0x10, 0x10, 0x60}, // 4
							{0x60, 0x10, 0x60}, // 5
							{0x60, 0x60, 0x60}, // 6
							{0x10, 0x60, 0x60}  // 7
							};							
	unsigned char drawlist[NLINES*2] = {0, 1,
										0, 4,
										1, 2,
										1, 5,
										2, 3,
										2, 6,
										3, 0,
										3, 7,
										4, 5,
										5, 6,
										6, 7,
										7, 4
										};		
	
	pixel3d rot[NPOINTS], sca[NPOINTS], h1, h2; 
	unsigned int  a;
	unsigned char i;								
											
	for (a = 0; a < 1500; a++) {
		scale(a/35+40, a/35+40, a/35+40, org, sca, NPOINTS, (pixel3d) {0x30, 0x30, 0x30});
		rotate(a/3, a/5 , a/4, sca, rot, NPOINTS, (pixel3d) {0x38, 0x38, 0x38});
		
		for (i = 0; i < NLINES*2; i += 2) {
			h1 = rot[drawlist[i]];
			h2 = rot[drawlist[i+1]];
			//printf("%d %d %d %d %d %d \n", (h1.x+8+0x40)/16-4, (h1.y+8+0x40)/16-4, 
            //      (h1.z+8+0x40)/16-4, (h2.x+8+0x40)/16-4, (h2.y+8+0x40)/16-4, 
            //       (h2.z+8+0x40)/16-4);
			drawLine3D(((char)h1.x+8)/16, ((char)h1.y+8)/16, ((char)h1.z+8)/16,
					   ((char)h2.x+8)/16, ((char)h2.y+8)/16, ((char)h2.z+8)/16, 3);
		}
		wait(5); 
		clear_screen(0);
	}	
}



void testLine3D() {
	clear_screen(0);
	drawLine3D(-3, -3, -3, 9, 9, 9, 3);
	wait(2000); 
	clear_screen(0);	
	drawLine3D(0, 3, 0, 3, 7, 7, 3);
	wait(2000);
	clear_screen(0);	
	drawLine3D(-2, -1, -5, 9, 8, 9, 3);
	wait(2000); 
	clear_screen(0);
	drawLine3D(0, 0, 0, 7, 7, 7, 3);
	wait(2000); 
	clear_screen(0);
	
}

void drawPanel(unsigned char posy, unsigned char pos128x, unsigned char pos128z) {
	unsigned char i, j;
	for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            setpixel3d((pixel3d) {(pos128x+8)/16+i, posy, (pos128z+8)/16+j}, 3);
        }
    }
}

typedef struct {
	int x, y, z;
} ball;

char waitForFire = 0;

#define LEN_Y 8
#define INIT_DIR {28, 56, 14} 
void pong() {
    unsigned char posx0 = 64, posz0 = 64, posx1 = 64, posz1 = 64;
	unsigned char lives0 = 5, lives1 = 5, ballblink = 0, score = 0;
	unsigned char counter = 8, joy0 = 0, joy1 = 0, ballV, i;
	ball ballPos128 = {4*128, 2*128, 4*128};
	pixel3d helpDir, ballDir = INIT_DIR, ballPos, ballPosOld;
	waitForFire = 0;
	clear_screen(0);
	
	while (lives0 && lives1) {
		if (!joy0) {
			JOYUSE0();
			JOYUSE0();
			if (JOYISFIRE) {
				joy0 = 1;
				lives0 = lives1;
			}
		}
		if (!joy1) {
			JOYUSE1();
			JOYUSE1();
			if (JOYISFIRE) {
				joy1 = 1;
				lives1 = lives0;
			}
		}	
		//if((!joy0) && (!joy1))
		//		break;//no player, so there was a glitch on the fire line.
		ballV = 5 + score % 2;
		if (!--counter) {
			ballPos.x = ballPos128.x / 128;
			ballPos.y = ballPos128.y / 128;
			ballPos.z = ballPos128.z / 128;
			if (ballPos128.y >= (LEN_Y*128-33) && !joy0) {
				ballDir.y = (char)-ballDir.y;
			}
			if (ballPos128.y >= (LEN_Y*128-150) && joy0) {
				if (ballPos128.x >= posx0 && ballPos128.x < posx0+48 && 
					ballPos128.z >= posz0 && ballPos128.z < posz0+48) {
	
					rotate((char)ballPos.x - ((char)posx1+8)/16 + 1, 0, 
						   (char)ballPos.z - ((char)posz1+8)/16 + 1,
						   &ballDir, &helpDir, 1, (pixel3d) {0, 0, 0});
					ballDir = helpDir;						   
					ballDir.y = (char)-ballDir.y;
					score++;
				} else {
					if (--lives0) {
						set_plane(right, 0, 3);
		
						wait(30);
						set_plane(right, 0, 0);
						wait(30);
						set_plane(right, 0, 3);
						wait(20);
						set_plane(right, 0, 2);
						wait(20);
						set_plane(right, 0, 1);
						wait(20);
						set_plane(right, 0, 0);
						ballPos128 = (ball) {4*128, 2*128, 4*128};
						ballDir = (pixel3d) INIT_DIR;
						wait(800);
					} else {
						set_plane(right, 0, 3);
						wait(100);
						set_plane(right, 0, 0);
						wait(100);
						set_plane(right, 0, 3);
						wait(100);
						set_plane(right, 0, 0);
						wait(100);
					}
				}
			}
			if (ballPos128.y <= 32 && !joy1) 
				ballDir.y = (char)-ballDir.y;
			if (ballPos128.y <= 150 && joy1) {
				if (ballPos128.x >= posx1 && ballPos128.x < posx1+48 && 
					ballPos128.z >= posz1 && ballPos128.z < posz1+48) {			
					rotate(((char)ballPos.z - ((char)posz1+8)/16 + 1)*4, 0, 
						   ((char)ballPos.x - ((char)posx1+8)/16 + 1)*4,
						   &ballDir, &helpDir, 1, (pixel3d) {0, 0, 0});
					ballDir = helpDir;
				    ballDir.y = (char)-ballDir.y;
					score++;
				} else {
					if (--lives1) {
					    for (i = lives1+1; i; --i) {
							set_plane(left, 0, 3);
							wait(50);
							set_plane(left, 0, 0);
							wait(75);
						}
						ballPos128 = (ball) {4*128, 2*128, 4*128};
						ballDir = (pixel3d) INIT_DIR;
						wait(800);
					} else {
						set_plane(left, 0, 3);
						wait(150);
						set_plane(left, 0, 0);
						wait(150);
						set_plane(left, 0, 3);
						wait(100);
						set_plane(left, 0, 0);
						wait(100);
					}
				}
			}
			if (ballPos128.x <= 64 || ballPos128.x >= (LEN_Y*128-64) )
				ballDir.x = (char)-ballDir.x;
			if (ballPos128.z <=64 || ballPos128.z >= (LEN_Y*128-64) )
				ballDir.z = (char)-ballDir.z;
		}	
		
		if (ballblink > 4) {
			setpixel3d(ballPos, 3);
		}

		if (++ballblink > 15) {
			ballblink = 0;
		}
			
		if (joy0) {
			JOYUSE0();
			JOYUSE0();
			
			if (JOYISUP) {
			   if (posz0 < 80) posz0++;
			}
			if (JOYISDOWN) {
			   if (posz0 > 0) posz0--;
			}             
			if (JOYISLEFT) {
				if (posx0 < 80) posx0++;
			}
			if (JOYISRIGHT) {
				if (posx0 > 0) posx0--;
			}
        	
			set_plane(right, 0, 0);
			drawPanel(LEN_Y-1, posx0, posz0);
		}
		if (joy1) {
			JOYUSE1();
			JOYUSE1();
		
			if (JOYISUP) {
			   if (posz1 < 80) posz1++;
			}
			if (JOYISDOWN) {
			   if (posz1 > 0) posz1--;
			}             
			if (JOYISRIGHT) {
			   if (posx1 < 80) posx1++; 
			}
			if (JOYISLEFT) {
			   if (posx1 > 0) posx1--;
			}
			set_plane(left, 0, 0);
			drawPanel(0, posx1, posz1);
		}
		
        wait(23);
		if (!counter) {
			printf("%d %d %d pos %d %d %d dir %d %d %d addDirV %d %d %d v %d\n", 
					ballPos.x, ballPos.y, ballPos.z, ballPos128.x, ballPos128.y, ballPos128.z, 
					ballDir.x, ballDir.y, ballDir.z, ((char)ballDir.x * ballV)/4,
					((char)ballDir.y * ballV)/4, ((char)ballDir.z * ballV)/4, ballV);
			ballPos128.x += ((char)ballDir.x * ballV)/4;
			ballPos128.y += ((char)ballDir.y * ballV)/4;
			ballPos128.z += ((char)ballDir.z * ballV)/4;
			counter = 8;
		}
		clearpixel3d(ballPos);
		ballPosOld = ballPos;
	}
	waitForFire = 1;
}

void *display_loop(void * unused) {
	while (1) {
		//pong();
		//spirale();
		//testLine3D();
		
		rotatedScaledCube();
		
		wait(30);
		//pong();
		
		growingCubeFilled();
		
		planeAnimation2(80);
		newCoolAnim();
		//pong();
		//feuer();
		rotatePixmap(1);
		rotatePixmap(2);
		growingCubeFilled();
		rotatePixmap(0);
        movingArrows();
        growingCubeFilled();
        scrolltext("Borg 3D!!!", 120);
        growingCubeFilled();
        joern1();
		growingCubeFilled();
		//gameOfLife(1, 400);
		growingCubeFilled();
		gameOfLife(0, 400);
		growingCubeFilled();
		growingCubeFilled();
		//gameOfLife(2, 150);
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		spirale();
		//spirale2();
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
		feuer();
	}
}
