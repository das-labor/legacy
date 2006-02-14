#ifdef AVR
#  include "config.h"
#  include "borg_hw.h"
#  include "uart.h"
#  include "pixel.h"
#  include "scrolltext.h"
#  include <avr/io.h>
#else
#  include "pixel3d.h"
#  include "scrolltext2.h"
#endif

#include "util.h"
#include "joystick.h"
#include "programs.h"

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



void movingArrows() {
     unsigned char i, j, cnt;
     for (cnt = 0; cnt < 100; cnt++) {
         shift3d(up);
         for (i = 0; i < NUM_ROWS; i++) {
             for (j = 0; j < NUM_COLS; j++) {
                 setpixel3d((pixel3d) {i, j, 0}, 
				 	(j == cnt%4 && i >= cnt%4 && i < 7 - cnt%4) ||
				 	(j == 7 - cnt%4 && i >= cnt%4 && i < 7 - cnt%4) ||
				 	(i == cnt%4 && j >= cnt%4 && j < 7 - cnt%4) ||
				 	(i == 7 - cnt%4 && j >= cnt%4 && j < 7 - cnt%4)
				 	? cnt%3+1:0);
             }
         }
         wait(120);
     }
     clear_screen(0);
}



#define BWIDTH 8
void fadedPlaneScan(direction dir, unsigned char i)
{
		if (i < BWIDTH) set_plane(dir, i, 3);
		if (i >= 1 && i < BWIDTH+1) set_plane(dir, i-1, 2);
		if (i >= 2 && i < BWIDTH+2) set_plane(dir, i-2, 1);
		if (i >= 3 && i < BWIDTH+3) set_plane(dir, i-3, 0);
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

void waves() {
	signed char data[8][8];
	unsigned char i, j, k;
	for (i = 0; i < 254; i++) {
		for (j = 0; j < 8; j++) {
			data[j][0] = Sin((i+j)%16 *4);
			data[0][j] = Sin((i+j)%16 *4);
		}
		for (j = 1; j < 8; j++) {
			for (k = 1; k < 8; k++) {
				data[j][k] = (data[j-1][k] + data[j][k-1] + data[j-1][k-1])/3;
			}
		}
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				setpixel3d((pixel3d){j, k, (data[j][k]+64)/16}, 3);
			}
		}
		wait(80);
		clear_screen(0);
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
}

void serialStream() {
#	ifdef AVR
	unsigned char i, j, k, tmp, esc = 0;
	int count = 0;
	while (count++ < 500) {
		tmp = uart_getc();
		if (esc && tmp == UART_SOF) {
			for (i = 0; i < NUM_LEVELS; i++) {
				for (j = 0; j < NUM_PLANES; j++) {
					for (k = 0; k < PLANEBYTES; k++) {
						if (tmp == UART_ESCAPE) {
							tmp = uart_getc();
							if (tmp == UART_SS) {
								return;
							}
						}
						if (tmp)
						pixmap[i][j][k] = uart_getc();
					}
				} 
			}
			count = 0;
		} else if (esc && tmp == UART_SS) {
			return;
		}
		
		if (tmp == UART_ESCAPE)
			esc = 1;
		else
			esc = 0;
	}
#	else
	// Animaionsfiles could be loaded here
#	endif
}

void gameOfLife(unsigned char anim, unsigned int delay) {
	unsigned char gen, erg, seven = 1, maxGen = 0;
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
									//printf("- %d %d %d  neigh=%d erg=%d\n", x+i, y+j, z+k, neighC, erg);
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
					//if (neighs[x][y][z])		
					//	printf("%d %d %d neigh=%d %d\n", x, y, z, neighs[x][y][z], get_pixel3d((pixel3d){x, y, z}));
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
		//printf("============ Gerneration %d \n", gen);
	}			
}
