#include "pixel.h"
#include "util.h"
#include "config.h"
#include "borg_hw.h"
#include "uart.h"
#include "joystick.h"
#include "scrolltext.h"
#include "programs.h"

#include <avr/io.h>

#define NPOINTS 9
#define NLINES 12
void rotatedScaledCube() {
	// Drehender Quader
	pixel3d org[NPOINTS] = {{0x10, 0x10, 0x10}, // 0
							{0x60, 0x10, 0x10}, // 1
							{0x60, 0x60, 0x10}, // 2 
							{0x10, 0x60, 0x10}, // 3
							{0x10, 0x10, 0x60}, // 4
							{0x60, 0x10, 0x60}, // 5
							{0x60, 0x60, 0x60}, // 6
							{0x10, 0x60, 0x60}, // 7
							{0x00, 0x00, 0x00}  // 8
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
		scale(a/35+40, a/35+40, a/35+40, org, sca, 8, (pixel3d) {0x30, 0x30, 0x30});
		rotate(a/3, a/5 , a/4, sca, rot, NPOINTS, (pixel3d) {0x38, 0x38, 0x38});
		
		for (i = 0; i < NLINES*2; i += 2) {
			h1 = rot[drawlist[i]];
			h2 = rot[drawlist[i+1]];
			drawLine3D(((char)h1.x+8)/16, ((char)h1.y+8)/16, ((char)h1.z+8)/16,
					   ((char)h2.x+8)/16, ((char)h2.y+8)/16, ((char)h2.z+8)/16, 3);
		}
		wait(6); 
		clear_screen(0);
	}
	wait(20);
	// Drehende Phyramide
	org[2] = (pixel3d) {0x37, 0x54, 0x10};
	org[3] = (pixel3d) {0x37, 0x37, 0x48};
	drawlist[0]  = 0; drawlist[1]  = 1;
	drawlist[2]  = 1; drawlist[3]  = 2;
	drawlist[4]  = 2; drawlist[5]  = 0;
	drawlist[6]  = 0; drawlist[7]  = 3;
	drawlist[8]  = 1; drawlist[9]  = 3;
	drawlist[10] = 2; drawlist[11] = 3;	
	for (a = 0; a < 1500; a++) {
		scale(a/35+40, a/35+40, a/35+40, org, sca, 4, (pixel3d) {0x30, 0x30, 0x30});
		rotate(a/3, a/4 , a/5, sca, rot, 4, (pixel3d) {0x38, 0x38, 0x38});
		
		for (i = 0; i < 12; i += 2) {
			h1 = rot[drawlist[i]];
			h2 = rot[drawlist[i+1]];
			drawLine3D(((char)h1.x+8)/16, ((char)h1.y+8)/16, ((char)h1.z+8)/16,
					   ((char)h2.x+8)/16, ((char)h2.y+8)/16, ((char)h2.z+8)/16, 3);
		}
		wait(6); 
		clear_screen(0);
	}
	wait(20);
	for (a = 0	; a < 4; a++) {
		org[a].x += 9;
		org[a].y += 9;
		org[a].z += 20;
	}	
	org[4] = (pixel3d) {0x40, 0x40, 0x40};
	scale(40, 40, 40, org, &org[5], 3, org[4]);
	drawlist[0]  = 0; drawlist[1]  = 4;
	drawlist[2]  = 1; drawlist[3]  = 4;
	drawlist[4]  = 2; drawlist[5]  = 4;
	drawlist[6]  = 3; drawlist[7]  = 4;
	drawlist[8]  = 0; drawlist[9]  = 5;
	drawlist[10] = 1; drawlist[11] = 6;	
	drawlist[12] = 2; drawlist[13] = 7;	
	drawlist[14] = 3; drawlist[15] = 8;	
	for (a = 0; a < 3000; a++) {
		rotate(a/3,  a/3 , a/4	,  org, rot, NPOINTS, org[4]);		
		rotate(a/20, a/20, a/20, &rot[5], &rot[5], 3, org[4]);
		for (i = 0; i < 16; i += 2) {
			h1 = rot[drawlist[i]];
			h2 = rot[drawlist[i+1]];
			drawLine3D(((char)h1.x+8)/16, ((char)h1.y+8)/16, ((char)h1.z+8)/16,
					   ((char)h2.x+8)/16, ((char)h2.y+8)/16, ((char)h2.z+8)/16, 3);
		}
		wait(6); 
		clear_screen(0);
	}
}

#undef NPOINTS
#define NPOINTS 32
void testRotate() {
	pixel3d org[NPOINTS] = {{0x20, 0x20, 0x20},
							{0x30, 0x20, 0x20},
							{0x40, 0x20, 0x20},
							{0x50, 0x20, 0x20},
							{0x50, 0x30, 0x20},
							{0x50, 0x40, 0x20},
							{0x50, 0x50, 0x20},
							{0x40, 0x50, 0x20},
							{0x30, 0x50, 0x20},
							{0x20, 0x50, 0x20},
							{0x20, 0x40, 0x20},
							{0x20, 0x30, 0x20},
							{0x20, 0x20, 0x50},
							{0x30, 0x20, 0x50},
							{0x40, 0x20, 0x50},
							{0x50, 0x20, 0x50},
							{0x50, 0x30, 0x50},
							{0x50, 0x40, 0x50},
							{0x50, 0x50, 0x50},
							{0x40, 0x50, 0x50},
							{0x30, 0x50, 0x50},
							{0x20, 0x50, 0x50},
							{0x20, 0x40, 0x50},
							{0x20, 0x30, 0x50},
							{0x20, 0x20, 0x30},
							{0x20, 0x20, 0x40},
							{0x20, 0x50, 0x30},
							{0x20, 0x50, 0x40},
							{0x50, 0x20, 0x30},
							{0x50, 0x20, 0x40},
							{0x50, 0x50, 0x30},
							{0x50, 0x50, 0x40}}; 
	pixel3d rot[NPOINTS]; 
	unsigned char i, a;
	//unsigned char hx, hy, hz;
	//int a;
	
	for (a = 0; a < 196; a++) {
		rotate(a, 0, 0, org, rot, NPOINTS, (pixel3d) {0x48, 0x48, 0x48});
		for (i = 0; i < NPOINTS; i++) {
			setpixel3d((pixel3d) {(rot[i].x+8)/16, (rot[i].y+8)/16, (rot[i].z+8)/16}, 3);
		}
		wait(15);
		clear_screen(0);
	}	
	for (a = 0; a < 196; a++) {
		rotate(0, a, 0, org, rot, NPOINTS, (pixel3d) {0x48, 0x48, 0x48});
		for (i = 0; i < NPOINTS; i++) {
			setpixel3d((pixel3d) {(rot[i].x+8)/16, (rot[i].y+8)/16, (rot[i].z+7)/16}, 3);
		}
		wait(15);
		clear_screen(0);
	}
	for (a = 0; a < 196; a++) {
		rotate(0, 0, a, org, rot, NPOINTS, (pixel3d) {0x48, 0x48, 0x48});
		for (i = 0; i < NPOINTS; i++) {
			setpixel3d((pixel3d) {(rot[i].x+8)/16, (rot[i].y+8)/16, (rot[i].z+8)/16}, 3);
		}
		wait(15);
		clear_screen(0);
	}
	for (a = 0; a < 196; a++) {
		rotate(a/4, a/2, a, org, rot, NPOINTS, (pixel3d) {0x48, 0x48, 0x48});
		for (i = 0; i < NPOINTS; i++) {
			/*
			hx = rot[i].x % 16;
			hy = rot[i].y % 16;
			hz = rot[i].z % 16;
			hx = hx > 4 ? 5 - hx: hx;
			hx += hy > 4 ? 5 - hy: hy;
			hx += hz > 4 ? 5 - hz: hz; */
			setpixel3d((pixel3d) {(rot[i].x+8)/16, (rot[i].y+8)/16, (rot[i].z+8)/16}, 3);
		}
		wait(20);
		clear_screen(0);
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
	unsigned char z, angle, count = 0, angleAdd;
	for (angleAdd = 0; angleAdd < 12; count++) {
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
	unsigned char x1[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1};
	unsigned char y1[] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0};
	unsigned char x2[] = {7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6};
	unsigned char y2[] = {7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 7, 7, 7};
	drawPixmapZ(x1[angle], y1[angle], x2[angle], y2[angle], pixmap, value);	
}

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

void laborZylinder() {
	unsigned char indexX[] = {1, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 7, 6, 5, 4, 3, 2};
	unsigned char indexY[] = {1, 2, 3, 4, 5, 6 ,7, 7, 7, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0};
	unsigned char i, j;
		
	clear_screen(0);
	for (i = 0; i < 200; i++) {
		for (j = 0; j < 20; j++) {
			if (j < 5 || (j < 15 && j > 9))
				setpixel3d((pixel3d) {indexX[(j+i)%20], indexY[(j+i)%20], 0}, 3);
		}
		wait(50);
		shift3d(up);
	}
}
