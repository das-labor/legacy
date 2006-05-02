#include "pixel.h"
#include "util.h"
#include <avr/pgmspace.h>

#define PW(a) pgm_read_word(&(a))
#define PB(a) pgm_read_byte(&(a))


unsigned char bp_image[3][18][8] PROGMEM = {
  {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF}
 , {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF}
 , {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF}
 , {0xFF, 0x33, 0xDF, 0xFF, 0x7F, 0x08, 0xF0, 0xFF}
 , {0x1F, 0x3B, 0x9F, 0x3F, 0x10, 0x99, 0xF1, 0xFF}
 , {0x33, 0x33, 0x87, 0x3C, 0xF3, 0x11, 0x73, 0xFF}
 , {0x3B, 0x03, 0x87, 0x60, 0xF0, 0x33, 0x61, 0xFF}
 , {0xC3, 0xFF, 0xB7, 0xCC, 0xE3, 0xF3, 0x7F, 0xFE}
 , {0xFF, 0x7F, 0xB6, 0xE1, 0xFF, 0xFF, 0x7F, 0xFD}
 , {0xFF, 0x7F, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 0xC1}
 , {0xFF, 0x7F, 0xFE, 0x8C, 0x01, 0x04, 0x80, 0xE7}
 , {0xFF, 0x1F, 0x04, 0x8C, 0x71, 0x8C, 0x83, 0xF7}
 , {0xFF, 0x7F, 0xC6, 0x18, 0x71, 0x0C, 0x07, 0xF7}
 , {0xFF, 0x7F, 0xC6, 0x18, 0x61, 0x08, 0x07, 0xE0}
 , {0xFF, 0x7F, 0xCE, 0x18, 0x63, 0x08, 0x07, 0xC4}
 , {0xFF, 0x3F, 0xCE, 0x18, 0x03, 0x18, 0x00, 0xDE}
 , {0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF}
 , {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF}
 }
 , {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF}
 , {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF}
 , {0xFF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF}
 , {0xFF, 0x01, 0xDF, 0xFF, 0x3F, 0x08, 0xF0, 0xFF}
 , {0x1F, 0x31, 0x9F, 0x37, 0x10, 0x99, 0xF1, 0xFF}
 , {0x31, 0x31, 0x87, 0x30, 0xF3, 0x11, 0x73, 0xFF}
 , {0x33, 0x03, 0x87, 0x60, 0xE0, 0x13, 0x60, 0xFF}
 , {0xC3, 0xFF, 0xB7, 0xCC, 0xE0, 0xF3, 0x7F, 0xFE}
 , {0xFF, 0x3F, 0xB6, 0xE0, 0xFF, 0xFF, 0x7F, 0x9C}
 , {0xFF, 0x3F, 0xFE, 0xFF, 0xFF, 0x0F, 0x00, 0xC1}
 , {0xFF, 0x3F, 0x3E, 0x8C, 0x01, 0x04, 0x80, 0xC7}
 , {0xFF, 0x1F, 0x04, 0x88, 0x31, 0x8C, 0x03, 0xE7}
 , {0xFF, 0x7F, 0xC6, 0x18, 0x71, 0x0C, 0x03, 0xF7}
 , {0xFF, 0x7F, 0xC6, 0x18, 0x61, 0x08, 0x07, 0xE0}
 , {0xFF, 0x7F, 0xC4, 0x18, 0x63, 0x08, 0x07, 0xC4}
 , {0xFF, 0x3F, 0xCE, 0x18, 0x03, 0x18, 0x00, 0xCE}
 , {0xFF, 0x3F, 0xCE, 0x39, 0x07, 0x7C, 0x00, 0xBF}
 , {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF}
 }
 , {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF}
 , {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF}
 , {0xFF, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF}
 , {0xBF, 0x01, 0x9F, 0xFF, 0x1F, 0x08, 0xF0, 0xFF}
 , {0x1F, 0x31, 0x9F, 0x27, 0x10, 0x91, 0xB1, 0xFF}
 , {0x01, 0x31, 0x86, 0x20, 0xB3, 0x11, 0x61, 0xFF}
 , {0x31, 0x01, 0x87, 0x00, 0xE0, 0x11, 0x60, 0xFF}
 , {0x83, 0xFF, 0x87, 0xC4, 0xE0, 0x33, 0x7C, 0xFE}
 , {0xFF, 0x3F, 0x92, 0xE0, 0xFC, 0xFF, 0x3F, 0x80}
 , {0xFF, 0x3F, 0xFE, 0xFF, 0x7F, 0x06, 0x00, 0x81}
 , {0xFF, 0x3F, 0x0E, 0x8C, 0x01, 0x04, 0x80, 0xC7}
 , {0xFF, 0x1F, 0x00, 0x08, 0x31, 0x04, 0x03, 0xE3}
 , {0xFF, 0x7F, 0xC6, 0x08, 0x61, 0x08, 0x03, 0xF7}
 , {0xFF, 0x7F, 0xC4, 0x18, 0x61, 0x08, 0x07, 0xE0}
 , {0xFF, 0x7F, 0xC4, 0x18, 0x61, 0x08, 0x02, 0xC0}
 , {0xFF, 0x3F, 0xCC, 0x18, 0x03, 0x18, 0x00, 0xCE}
 , {0xFF, 0x3F, 0xCE, 0x19, 0x07, 0x3C, 0x00, 0xBF}
 , {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFE}}
 };

 
// Position in 
//	  i[][0][0]	i[][0][1]
//	  
// 
//    p[][0][0] p[][0][1] 
//    10001001  10010101 

// image


void drawPixmapPos(int x, int y) {
	unsigned char p, i;
	union{uint32_t l; uint8_t b[4];} buf;
	
	for (p = 0; p < 3; p++) {
			for (i = 0; i < 16; i++) {
				buf.b[0] = 0;
				buf.b[1] = PB(bp_image[p][(i+y)][(x/8)]);
				buf.b[2] = 0;
				
				if (x < 58)
					buf.b[2] = PB(bp_image[p][(i+y)][(1 + x/8)]); 
				if (x > 7)
					buf.b[0] = PB(bp_image[p][(i+y)][(-1 + x/8)]);
				pixmap[p][i][1] = (uint8_t) (buf.l >> ((x%8)+8));
				pixmap[p][i][0] = (uint8_t) (buf.l >> ((x%8)+0));
		}
	}
}

void breakpoint() {
	unsigned char p, x, i, runs=2;
	clear_screen(3);
	for (i = 63; i >= 8; i--) {
		drawPixmapPos(i, 0)	;
		wait(45);
	}	
	while (runs-- >= 1){
		for (i = 8; i <= 56; i++) {
			drawPixmapPos(i, 0)	;
			wait(45);
		}
		wait(100);
		for (i = 56; i >= 8 ; i--) {
			drawPixmapPos(i, 1)	;
			wait(45);
		}
		wait(100);	
	}
	wait(900);
		for (x = 0; x < 4; x++) {
			for (p = 0; p < 3; p++) {
				for (i = 0; i < 16; i++) {
					pixmap[p][i][0] = ((pixmap[p][i][0]) & 0xf0) << 1 |
					                  ((pixmap[p][i][0]) & 0x0f) >> 1;
					pixmap[p][i][1] = ((pixmap[p][i][1]) & 0xf0) << 1 |
					                  ((pixmap[p][i][1]) & 0x0f) >> 1;				  
									
				}
		}
		wait(200);
	}
	clear_screen(0);
	wait(100);
}


/************************************************************************/

/** Table for the calculation of sinus. It only has one half wave, because
 *  the others can be generated of it.
 *  
 *  64 = sin(90 grad) = 1.0 = sinTab[16]
 */
char sinTab[] = {0, 6, 12, 19, 24, 30, 36, 41, 45, 49, 53, 56, 59, 61, 63, 64, 64};

/** Sin(64) = sin(360 grad) and sin(90 grad) = Sin(16) = 64. Its using the one halfwave
 *  of sinTab to generate a whole sinus. It e
 */		 
char Sin(unsigned char a) {
	a %= 64;
	if (a < 17) {
		return  sinTab[a];
	} else if (a < 33) {
		return  sinTab[32-a];
	} else if (a < 49) {
		return -sinTab[a-32];
	} else {
		return -sinTab[64-a];
	}
}	

inline char Cos(unsigned char a) {
	return Sin(a+16);
}

#define NUM_CIRCLE 5

void BProtationgPoints() {
	pixel circlePoints[NUM_CIRCLE][8];
	unsigned char  i, j, firstRadius = 80, helpRadius, angle = 0;
	unsigned int k;
	// init data
	for (k = 0; k < 400; k++) {	
		helpRadius = firstRadius;
		for (i = 0; i < NUM_CIRCLE; i++) {
			for (j = 0; j < 8; j++) {
				if (i & 1) {
					circlePoints[i][j].x = 64 + (Cos(angle + i*8)*helpRadius)/64;
					circlePoints[i][j].y = 64 + (Sin(angle + i*8)*helpRadius)/64;
				
				} else {
					circlePoints[i][j].x = 64 + (Cos(angle + i*8 + 4)*helpRadius)/64;
					circlePoints[i][j].y = 64 + (Sin(angle + i*8 + 4)*helpRadius)/64;
				} 
				// only for testing
				setpixel((pixel) {circlePoints[i][j].x/8, circlePoints[i][j].y/8}, 3);
			}
			helpRadius = (helpRadius*2)/3; 
		}
		wait(50);
		clear_screen(0);
		angle++;
		firstRadius += 5;
		if (firstRadius > 119)
			firstRadius = 80;
	}
}
