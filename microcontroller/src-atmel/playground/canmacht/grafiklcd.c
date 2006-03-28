#include <avr/io.h>
#include "s1d1565.h"
#include "grafiklcd.h"

void dispSetPix(uint8_t x, uint8_t y, uint8_t color)
{
	if (x < NUM_COLS && y < NUM_ROWS) {
		
		DISPSETPAGE(y / 8);

		//set read/mod/write (read doesn't increment column)
		dispWriteCommand(0xe0);

		// select column addr
		dispWriteCommand(0x10|(x/16));
		dispWriteCommand(x&15);

		//dummy read
		dispReadByte();
		//write pix
		dispWriteByte(color?(0x01 << (y % 8)) | dispReadByte():
	                        ~(0x01 << (y % 8)) & dispReadByte());

		//end read/mod/write
		dispWriteCommand(0xee);
	}
}


//drav horizontal line, x & x2 inclusive
void dispHLine(uint8_t x, uint8_t y, uint8_t x2, uint8_t color)
{
	//set read/mod/write (read doesn't increment column)
	dispWriteCommand(0xe0);

	DISPSETPAGE(y / 8);

	// select column addr
	dispWriteCommand(0x10|(x/16));
	dispWriteCommand(x&15);

	uint8_t mask = 1 << (y%8);

	if(color){
		while(x++<=x2)
		{
			//dummy read
			dispReadByte();
			//write pix
			dispWriteByte( mask | dispReadByte() );
		}
	}else{
		mask = ~mask;
		while(x++<=x2)
			{
			//dummy read
			dispReadByte();
			//write pix
			dispWriteByte( mask & dispReadByte() );
		}
	}

	while(x++<=x2)
	{
		//dummy read
		dispReadByte();
		//write pix
		dispWriteByte(color?(0x01 << (y % 8)) | dispReadByte():
	                        ~(0x01 << (y % 8)) & dispReadByte());
	}
	//end read/mod/write
	dispWriteCommand(0xee);
}

//drav vertical line, x & x2 inclusive
void dispVLine(uint8_t x, uint8_t y, uint8_t y2) 
{
	uint8_t sp, ep, mask;
	sp=y/8;
	ep=y2/8;

	//set read/mod/write (read doesn't increment column)
	dispWriteCommand(0xe0);


	mask = 0xff << (y%8);
	
	while(sp <= ep){
		if(sp == ep){
			mask &= 0xff >> (7-(y2%8));		
		}
		DISPSETPAGE(sp);
		// select column addr		dispWriteCommand(0x10|(x/16));
		dispWriteCommand(x&15);
		//dummy read
		dispReadByte();
		//write pix
		dispWriteByte(mask | dispReadByte());
		mask = 0xff;
		sp++;
	}
	//end read/mod/write
	dispWriteCommand(0xee);
}

void fillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) 
{
	uint8_t sp, ep, mask;
	uint8_t y2 = y + height - 1;
	sp=y/8;
	ep=y2/8;

	//set read/mod/write (read doesn't increment column)
	dispWriteCommand(0xe0);
	
	mask = 0xff << (y%8);
	
	while(sp <= ep){
		uint8_t xi;
		if(sp == ep){
			mask &= 0xff >> (7-(y2%8));		
		}
		DISPSETPAGE(sp);
		// select column addr		
		for(xi = x; xi < x+width; xi++){		
			dispWriteCommand(0x10|(xi/16));
			dispWriteCommand(xi&15);
			//dummy read
			dispReadByte();
			//write pix
			if(color){
				dispWriteByte(mask | dispReadByte());
			}else{
				dispWriteByte(~mask & dispReadByte());
			}		
		}
		mask = 0xff;
		sp++;
	}
	//end read/mod/write	dispWriteCommand(0xee);
}



void dispDrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
	  	
	dispHLine(x, y, x + width, color);		// top
	dispHLine(x, y+height, x + width, color);	// bottom
	dispVLine(x, y , y + height);			// left
	dispVLine(x+width, y, y + height);		// right
}

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
