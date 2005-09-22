#include "pixel3d.h"
#include "programm.h"
#include "scrolltext2.h"

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
	signed char i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;
	
	dx = x2 - x1;      // the horizontal distance of the line
	dy = y2 - y1;      // the vertical distance of the line 
	dxabs = dx >= 0 ? dx: -dx; //abs
	dyabs = dy >= 0 ? dy: -dy; //abs
	sdx = dx >= 0 ? 1: -1;     //sgn
	sdy = dy >= 0 ? 1: -1;     //sgn
	x = dyabs >> 1;			   // *2
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

	}
	for (gen = 0; gen < maxGen; gen++) {
		wait(delay);	
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
	}			
}


void movingArrows() {
     unsigned char i, j, cnt;
     for (cnt = 0; cnt < 100; cnt++) {
         shift3d(up);
         for (i = 0; i < NUM_ROWS; i++) {
             for (j = 0; j < NUM_COLS; j++) {
                 setpixel3d((pixel3d) {i, j, 0}, ((3-i == cnt%4 || i-4 == cnt%4) && (3-j == cnt%4 || j-4 == cnt%4))?3:0);
             }
         }
         wait(120);
     }
     clear_screen(0);

}

#define FEUER_Y (NUM_ROWS + 3)
#define FEUER_S 30
#define FEUER_N 5
#define FEUER_DIV 47;
void feuer()
{
	unsigned char y, x;
	unsigned int  t;
	unsigned char world[NUM_COLS*NUM_ROWS][FEUER_Y];   // double buffer

	for(t=0; t<800; t++) {
		// diffuse
		for(y=1; y<FEUER_Y; y++) {
			for(x=1; x<NUM_COLS*NUM_ROWS; x++) {
				world[x][y-1] = (FEUER_N*world[x-1][y] + FEUER_S*world[x][y] + FEUER_N*world[x+1][y]) / FEUER_DIV;
			};

			world[0][y-1] = (FEUER_N*world[NUM_COLS-1][y] + FEUER_S*world[0][y] + FEUER_N*world[1][y]) / FEUER_DIV;
			world[NUM_COLS-1][y-1] = (FEUER_N*world[0][y] + FEUER_S*world[NUM_COLS-1][y] + FEUER_N*world[NUM_COLS-2][y]) / FEUER_DIV;
		};

		// update lowest line
		for(x=0; x<NUM_COLS*NUM_ROWS; x++) {
			world[x][FEUER_Y-1] = myrandom();
		};
	
		// copy to screen
		for(y=0; y<NUM_ROWS; y++) {
			for(x=0; x<NUM_COLS*NUM_ROWS; x++) {
				setpixel3d( (pixel3d){x/8,x%8,7-y}, (world[x][y] >> 5) );
			}		
		};
		wait(70);
	}
}




void pong() {
	// ToDo
	/*
	 Als erstes muss die Steuerfl‰che geprogt werden, zuerst eine mit tastatursteuerung.
	   ok dann mach ich halt den Ball zuerst.
	 Als zweites kommt der Ball dran
	   einfachen algorithmus für die flugbahn des Balles
	   sowie des Aufprall- und Abprallwinkel.
	   an wenden sowie beim schläger abprallen
	 Game over gewinner
	 geschicktes einbauen der scrolltextengine
	 Spielfläche 1 und 2 durch feststehende Zahlen kennzeichen.

	 Die Ballpositionn ist genauer als die Auflösung      gravity ist abhängig von x
	 
	 Direction ist ein auf 8 Normierter Vectoren
	 irgentwie die gravitation mit einbauen
	 und andere Abpreall winkel für die ecken den schlägers
 	 
	 */
    int erg;
    
    unsigned char posy1 = 4, posz1 = 3;
    
    
    char ran, diff, i, j, comp;	
	pixel3d ballPos128 = {3*16, 3*16, 3*16}, ballPos, ballPosOld, ballDir = {4, 2, 1};
	//scrolltext("Do you want to play with PONG3D ??? Then press Fire the joystick." , 70);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            setpixel3d((pixel3d) {0, posy1+i, posz1+j}, 1);   
        }
    }
	while (1) {
		ballPos.x = ballPos128.x+8 >> 4; 
		ballPos.y = ballPos128.y+8 >> 4;  
		ballPos.z = ballPos128.z+8 >> 4;
		
		if (ballPos.x == 0 || ballPos.x == 7) 
			ballDir.x = - ballDir.x;			
		if (ballPos.y == 0 || ballPos.y == 7)
			ballDir.y = - ballDir.y;
		if (ballPos.z == 0 || ballPos.z == 7)
			ballDir.z = - ballDir.z;
		
		setpixel3d(ballPos, 3);
		// keine gute idee
		/*
		if ((ran = myrandom()) > 80) {
           switch (ran%2) {
           case 0: 
                ballDir.y = ballDir.y + myrandom()%2 ? 1:-1;
                break;
           case 1: 
                ballDir.z = ballDir.z + myrandom()%2 ? 1:-1;
                break;                           
           }
           //erg = ballDir.y * ballDir.y + ballDir.z * ballDir.z;
           // normalize, so that the speed not changes    has bad effect nor correct
           //for (comp = 0, i = 0; i < 8 || (erg - comp >= 0 ? erg - comp: comp - erg) > 4 ; i++, comp += 8);
           //ballDir.x = i - 1; 
        }
		*/
		// display schl‰ger 1
		
		
		
		if (joy1_up > 0 || joy1_down > 0 || joy1_right > 0 || joy1_left > 0) {
        	for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    setpixel3d((pixel3d) {0, posy1+i, posz1+j}, 0);   
                }
            }
            if (joy1_up > 0) {
               if (posz1 < 5) posz1++;
               joy1_up = 0;
            }
            if (joy1_down > 0) {
               if (posz1 > 0) posz1--;
               joy1_down = 0;
            }             
            if (joy1_right > 0) {
               if (posy1 < 5) posy1++;
               joy1_right = 0;
            }
            if (joy1_left > 0) {
               if (posy1 > 0) posy1--;
               joy1_left = 0;
            }
           	for (i = 0; i < 3; i++) {
                for (j = 0; j < 3; j++) {
                    setpixel3d((pixel3d) {0, posy1+i, posz1+j}, 1);   
                }
            }            
        }
        wait(30);
        ballPos128.x += ballDir.x;
		ballPos128.y += ballDir.y;
		ballPos128.z += ballDir.z ;
		clearpixel3d(ballPos);
		//ballPosOld = ballPos;
	}
}

void *display_loop(void * unused) {
	while (1) {
		pong();
        movingArrows();
        growingCubeFilled();
        scrolltext("b0rg3d wID3rStanD ist ZWECKLOS !!! from: martin@labor", 120);
        growingCubeFilled();
        joern1();
		growingCubeFilled();
		gameOfLife(1, 400);
		growingCubeFilled();
		gameOfLife(0, 400);
		growingCubeFilled();
		growingCubeFilled();
		gameOfLife(2, 150);
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
