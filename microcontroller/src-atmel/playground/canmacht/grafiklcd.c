#include <avr/io.h>

#include "grafiklcd.h"


void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
	signed int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;
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
    	dispSetPix(px, py, color);
	
	if (dxabs >= dyabs) { // the line is more horizontal than vertical  
		for (i = 0; i < dxabs; i++) {
			y += dyabs; 
			if (y >= dxabs) {
				y -= dxabs;
				py += sdy;
			}
			px += sdx;
			dispSetPix(px, py, color);
		}
	} else { // the line is more vertical than horizontal
		for (i = 0; i < dyabs; i++) {
			x += dxabs;
			if (x >= dyabs) {
				x -= dyabs;
				px += sdx;
			}
			py += sdy;
			dispSetPix(px, py, color);
		}
	}
}	  
