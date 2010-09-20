/*
 * Copyright:      Fabian Maximilian Thiele  mailto:me@apetech.de
 * Author:         Fabian Maximilian Thiele
 * Remarks:        this Copyright must be included
 * known Problems: none
 * Version:        1.1
 * Description:    Graphic Library for KS0108- (and compatible) based LCDs
 * Modified:	   by siro 2.05.10 added transparency
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "ks0108.h"

lcdCoord			ks0108Coord;
uint8_t				ks0108Inverted=0;
ks0108FontCallback	ks0108FontRead;
uint8_t				ks0108FontColor;
const uint8_t*		ks0108Font;
uint8_t			ks0108FontTransparent=1;
void ks0108DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
	uint8_t length, i, y, yAlt, xTmp, yTmp;
	int16_t m;
	
	//
	// vertical line
	//
	if(x1 == x2) {
		// x1|y1 must be the upper point
		if(y1 > y2) {
			yTmp = y1;
			y1 = y2;
			y2 = yTmp;
		}
		ks0108DrawVertLine(x1, y1, y2-y1, color);
	
	//
	// horizontal line
	//
	} else if(y1 == y2) {	
		// x1|y1 must be the left point
		if(x1 > x2) {
			xTmp = x1;
			x1 = x2;
			x2 = xTmp;
		}
		ks0108DrawHoriLine(x1, y1, x2-x1, color);
	
	//
	// schiefe line :)
	//
	} else {
		// angle >= 45°
		if((y2-y1) >= (x2-x1) || (y1-y2) >= (x2-x1)) {
			// x1 must be smaller than x2
			if(x1 > x2) {
				xTmp = x1;
				yTmp = y1;
				x1 = x2;
				y1 = y2;
				x2 = xTmp;
				y2 = yTmp;
			}
		
			length = x2-x1;		// not really the length :)
			m = ((y2-y1)*200)/length;
			yAlt = y1;
			
			for(i=0; i<=length; i++) {
				y = ((m*i)/200)+y1;
				
				if((m*i)%200 >= 100)
					y++;
				else if((m*i)%200 <= -100)
					y--;
				
				ks0108DrawLine(x1+i, yAlt, x1+i, y, color);
				
				if(length <= (y2-y1) && y1 < y2)
					yAlt = y+1;
				else if(length <= (y1-y2) && y1 > y2)
					yAlt = y-1;
				else
					yAlt = y;
			}
		
		// angle < 45°
		} else {
			// y1 must be smaller than y2
			if(y1 > y2) {
				xTmp = x1;
				yTmp = y1;
				x1 = x2;
				y1 = y2;
				x2 = xTmp;
				y2 = yTmp;
			}
			
			length = y2-y1;
			m = ((x2-x1)*200)/length;
			yAlt = x1;
			
			for(i=0; i<=length; i++) {
				y = ((m*i)/200)+x1;
				
				if((m*i)%200 >= 100)
					y++;
				else if((m*i)%200 <= -100)
					y--;
				
				ks0108DrawLine(yAlt, y1+i, y, y1+i, color);
				if(length <= (x2-x1) && x1 < x2)
					yAlt = y+1;
				else if(length <= (x1-x2) && x1 > x2)
					yAlt = y-1;
				else
					yAlt = y;
			}
		}
	}
}

void ks0108DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
	ks0108DrawHoriLine(x, y, width, color);				// top
	ks0108DrawHoriLine(x, y+height, width, color);		// bottom
	ks0108DrawVertLine(x, y, height, color);			// left
	ks0108DrawVertLine(x+width, y, height, color);		// right
}

void ks0108DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color) {
  	int16_t tSwitch, x1 = 0, y1 = radius;
  	tSwitch = 3 - 2 * radius;
	
	while (x1 <= y1) {
	    ks0108SetDot(x+radius - x1, y+radius - y1, color);
	    ks0108SetDot(x+radius - y1, y+radius - x1, color);

	    ks0108SetDot(x+width-radius + x1, y+radius - y1, color);
	    ks0108SetDot(x+width-radius + y1, y+radius - x1, color);
	    
	    ks0108SetDot(x+width-radius + x1, y+height-radius + y1, color);
	    ks0108SetDot(x+width-radius + y1, y+height-radius + x1, color);

	    ks0108SetDot(x+radius - x1, y+height-radius + y1, color);
	    ks0108SetDot(x+radius - y1, y+height-radius + x1, color);

	    if (tSwitch < 0) {
	    	tSwitch += (4 * x1 + 6);
	    } else {
	    	tSwitch += (4 * (x1 - y1) + 10);
	    	y1--;
	    }
	    x1++;
	}
	  	
	ks0108DrawHoriLine(x+radius, y, width-(2*radius), color);			// top
	ks0108DrawHoriLine(x+radius, y+height, width-(2*radius), color);	// bottom
	ks0108DrawVertLine(x, y+radius, height-(2*radius), color);			// left
	ks0108DrawVertLine(x+width, y+radius, height-(2*radius), color);	// right
}

/*
 * Hardware-Functions 
 */
void ks0108FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
	uint8_t mask, pageOffset, h, i, data;
	height++;
	
	pageOffset = y%8;
	y -= pageOffset;
	mask = 0xFF;
	if(height < 8-pageOffset) {
		mask >>= (8-height);
		h = height;
	} else {
		h = 8-pageOffset;
	}
	mask <<= pageOffset;
	
	ks0108GotoXY(x, y);
	for(i=0; i<=width; i++) {
		data = ks0108ReadData();
		
		if(color == BLACK) {
			data |= mask;
		} else {
			data &= ~mask;
		}

		ks0108WriteData(data);
	}
	
	while(h+8 <= height) {
		h += 8;
		y += 8;
		ks0108GotoXY(x, y);
		
		for(i=0; i<=width; i++) {
			ks0108WriteData(color);
		}
	}
	
	if(h < height) {
		mask = ~(0xFF << (height-h));
		ks0108GotoXY(x, y+8);
		
		for(i=0; i<=width; i++) {
			data = ks0108ReadData();
		
			if(color == BLACK) {
				data |= mask;
			} else {
				data &= ~mask;
			}
	
			ks0108WriteData(data);
		}
	}
}

void ks0108InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	uint8_t mask, pageOffset, h, i, data, tmpData;
	height++;
	
	pageOffset = y%8;
	y -= pageOffset;
	mask = 0xFF;
	if(height < 8-pageOffset) {
		mask >>= (8-height);
		h = height;
	} else {
		h = 8-pageOffset;
	}
	mask <<= pageOffset;
	
	ks0108GotoXY(x, y);
	for(i=0; i<=width; i++) {
		data = ks0108ReadData();
		tmpData = ~data;
		data = (tmpData & mask) | (data & ~mask);
		ks0108WriteData(data);
	}
	
	while(h+8 <= height) {
		h += 8;
		y += 8;
		ks0108GotoXY(x, y);
		
		for(i=0; i<=width; i++) {
			data = ks0108ReadData();
			ks0108WriteData(~data);
		}
	}
	
	if(h < height) {
		mask = ~(0xFF << (height-h));
		ks0108GotoXY(x, y+8);
		
		for(i=0; i<=width; i++) {
			data = ks0108ReadData();
			tmpData = ~data;
			data = (tmpData & mask) | (data & ~mask);
			ks0108WriteData(data);
		}
	}
}

void ks0108SetInverted(uint8_t invert) {
	if(ks0108Inverted != invert) {
		ks0108InvertRect(0,0,127,63);
		ks0108Inverted = invert;
	}
}

void ks0108SetDot(uint8_t x, uint8_t y, uint8_t color) {
	uint8_t data;
	
	ks0108GotoXY(x, y-y%8);					// read data from display memory
	data = ks0108ReadData();
	
	if(color == BLACK) {
		data |= 0x01 << (y%8);				// set dot
	} else {
		data &= ~(0x01 << (y%8));			// clear dot
	}
	
	ks0108WriteData(data);					// write data back to display
}

//
// Font Functions
//

uint8_t ks0108ReadFontData(const uint8_t* ptr) {
	return pgm_read_byte(ptr);
}

void ks0108SelectFont(const uint8_t* font, ks0108FontCallback callback, uint8_t color) {
	ks0108Font = font;
	ks0108FontRead = callback;
	ks0108FontColor = color;
}

int ks0108PutChar(char c) {
	uint8_t width = 0;
	uint8_t height = ks0108FontRead(ks0108Font+FONT_HEIGHT);
	uint8_t bytes = (height+7)/8;
	
	uint8_t firstChar = ks0108FontRead(ks0108Font+FONT_FIRST_CHAR);
	uint8_t charCount = ks0108FontRead(ks0108Font+FONT_CHAR_COUNT);
	
	uint16_t index = 0;
	uint8_t x = ks0108Coord.x, y = ks0108Coord.y;
	
	if(c < firstChar || c >= (firstChar+charCount)) {
		return 1;
	}
	c-= firstChar;
	
	uint8_t i=0;
	// read width data, to get the index
	for(i=0; i<c; i++) {
		index += ks0108FontRead(ks0108Font+FONT_WIDTH_TABLE+i);
	}
	index = index*bytes+charCount+FONT_WIDTH_TABLE;
	width = ks0108FontRead(ks0108Font+FONT_WIDTH_TABLE+c);
	
	// last but not least, draw the character
	for(i=0; i<bytes; i++) {
		uint8_t page = i*width;
		uint8_t j=0;
		for(j=0; j<width; j++) {
			uint8_t data = ks0108FontRead(ks0108Font+index+page+j);
			
			if(height < (i+1)*8) {
				data >>= (i+1)*8-height;
			}
			
			if(ks0108FontColor == BLACK) {
				ks0108WriteData(data);				
			} else {
				ks0108WriteData(~data);
				}
		}
		// 1px gap between chars
		if(ks0108FontColor == BLACK) {
			ks0108WriteData(0x00);
		} else {
			ks0108WriteData(0xFF);
		}
		ks0108GotoXY(x, ks0108Coord.y+8);
	}
	ks0108GotoXY(x+width+1, y);
	
	return 0;
}

void ks0108Puts(char* str) {
	int x = ks0108Coord.x;
	while(*str != 0) {
		if(*str == '\n') {
			ks0108GotoXY(x, ks0108Coord.y+ks0108FontRead(ks0108Font+FONT_HEIGHT));
		} else {
			ks0108PutChar(*str);
		}
		str++;
	}
}

void ks0108Puts_P(PGM_P str) {
	int x = ks0108Coord.x;
	while(pgm_read_byte(str) != 0) {
		if(pgm_read_byte(str) == '\n') {
			ks0108GotoXY(x, ks0108Coord.y+ks0108FontRead(ks0108Font+FONT_HEIGHT));
		} else {
			ks0108PutChar(pgm_read_byte(str));
		}
		str++;
	}
}

uint8_t ks0108CharWidth(char c) {
	uint8_t width = 0;
	uint8_t firstChar = ks0108FontRead(ks0108Font+FONT_FIRST_CHAR);
	uint8_t charCount = ks0108FontRead(ks0108Font+FONT_CHAR_COUNT);
	
	// read width data
	if(c >= firstChar && c < (firstChar+charCount)) {
		c -= firstChar;
		width = ks0108FontRead(ks0108Font+FONT_WIDTH_TABLE+c)+1;
	}
	
	return width;
}

uint16_t ks0108StringWidth(char* str) {
	uint16_t width = 0;
	
	while(*str != 0) {
		width += ks0108CharWidth(*str++);
	}
	
	return width;
}

uint16_t ks0108StringWidth_P(PGM_P str) {
	uint16_t width = 0;
	
	while(pgm_read_byte(str) != 0) {
		width += ks0108CharWidth(pgm_read_byte(str++));
	}
	
	return width;
}

void ks0108GotoXY(uint8_t x, uint8_t y) {
	uint8_t chip = CHIP1, cmd;
	
	if(x > 127) x = 0;								// ensure that coordinates are legal
	if(y > 63)  y = 0;
	
	ks0108Coord.x = x;								// save new coordinates
	ks0108Coord.y = y;
	ks0108Coord.page = y/8;
	
	if(x >= 64) {									// select the right chip
		x -= 64;
		chip = CHIP2;
	}
	cmd = LCD_SET_ADD | x;
	ks0108WriteCommand(cmd, chip);					// set x address on active chip
	
	cmd = LCD_SET_PAGE | ks0108Coord.page;			// set y address on both chips
	ks0108WriteCommand(cmd, CHIP1);
	ks0108WriteCommand(cmd, CHIP2);
}

void ks0108Init(uint8_t invert) {
	// Wait a little while the display starts up
	volatile uint16_t i=0;
	for( i=0; i<15000; i++);

	ks0108Coord.x = 0;
	ks0108Coord.y = 0;
	ks0108Coord.page = 0;
	
	ks0108Inverted = invert;
	
	LCD_CMD_DIR = 0xFF;								// command port is output
	ks0108WriteCommand(LCD_ON, CHIP1);				// power on
	ks0108WriteCommand(LCD_ON, CHIP2);
	
	ks0108WriteCommand(LCD_DISP_START, CHIP1);		// display start line = 0
	ks0108WriteCommand(LCD_DISP_START, CHIP2);
	ks0108ClearScreen();							// display clear
	ks0108GotoXY(0,0);
}

inline void ks0108Enable(void) {
  uint8_t i=0;
  EN_H
    for(i=0;i<4;i++)
      asm volatile("nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"::);


  EN_L
    for(i=0;i<8;i++)
      asm volatile("nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"::);
}

uint8_t ks0108DoReadData(uint8_t first) {
	uint8_t data;
	volatile uint8_t i;
	
	LCD_DATA_OUT = 0x00;
	LCD_DATA_DIR = 0x00;							// data port is input
	
	 if(ks0108Coord.x < 64) {
	  CS2_L		// deselect chip 2
	  CS1_H		// select chip 1
	} else if(ks0108Coord.x >= 64) {
	  CS1_L		// deselect chip 1
	  CS2_H		// select chip 2
	}

	if(ks0108Coord.x == 64 && first) {				// chip2 X-address = 0
		ks0108WriteCommand(LCD_SET_ADD, CHIP2); 	// wuff wuff
	}
	
	DI_H					// D/I = 1
	RW_H					// R/W = 1
	
	EN_H
	  for(i=0;i<4;i++)
	  asm volatile("nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"::);					// EN high level width: min. 450ns

	data = LCD_DATA_IN;								// read Data			 
	
	EN_L
	  for(i=0;i<4;i++)
	  asm volatile("nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"
	     "nop\n\t"::);

	
	LCD_DATA_DIR = 0xFF;
	
	ks0108GotoXY(ks0108Coord.x, ks0108Coord.y);
	
	if(ks0108Inverted)
		data = ~data;
	return data;
}

uint8_t ks0108ReadData(void) {
	ks0108DoReadData(1);							// dummy read
	return ks0108DoReadData(0);						// "real" read
}

void ks0108WriteCommand(uint8_t cmd, uint8_t chip) {
	if(chip == CHIP1) {
		CS2_L		// deselect chip 2
		CS1_H		// select chip 1
	} else if(chip == CHIP2) {
		CS1_L			// deselect chip 1
		CS2_H				// select chip 2
	}
	

	DI_L					// D/I = 0
	RW_L					// R/W = 0
	LCD_DATA_DIR = 0xFF;							// data port is output
	LCD_DATA_OUT = cmd;								// write command
	ks0108Enable();									// enable
	LCD_DATA_OUT = 0x00;
}

void ks0108SetTransparency(uint8_t data) {
	if(data < 2)
		ks0108FontTransparent = data;
}	

void ks0108WriteData(uint8_t data) {
  uint8_t displayData, yOffset, cmdPort,cmdPortC;
	
#ifdef DEBUG
	volatile uint16_t i;
	for(i=0; i<5000; i++);
#endif

	if(ks0108Coord.x >= 128)
		return;

	if(ks0108Coord.x < 64) {
	  CS2_L		// deselect chip 2
	  CS1_H		// select chip 1
	} else if(ks0108Coord.x >= 64) {
	  CS1_L		// deselect chip 1
	  CS2_H		// select chip 2
	}
	if(ks0108Coord.x == 64)							// chip2 X-address = 0
		ks0108WriteCommand(LCD_SET_ADD, CHIP2);
	
	
	DI_H					// D/I = 1
	RW_L					// R/W = 0
	LCD_DATA_DIR = 0xFF;							// data port is output
	
	yOffset = ks0108Coord.y%8;
	if(yOffset != 0) {
		// first page
	  cmdPort = PORTB & (_BV(CSEL1)|_BV(CSEL2));
	  cmdPortC = PORTC & (_BV(R_W)|_BV(D_I)|_BV(EN));
		if(ks0108FontTransparent == TRANSPARENT)					// save command port
			displayData = ks0108ReadData();
		else
			displayData=0;

		PORTB &= ~(_BV(CSEL1)|_BV(CSEL2));  //restore command port
		PORTB |= cmdPort; // restore command port
		LCD_CMD_PORT &=~(_BV(R_W)|_BV(D_I)|_BV(EN));// restore command port
		LCD_CMD_PORT |= cmdPortC; // restore command port
		
		
		LCD_DATA_DIR = 0xFF;						// data port is output
		
		displayData |= data << yOffset;
		if(ks0108Inverted)
			displayData = ~displayData;
		LCD_DATA_OUT = displayData;					// write data
		ks0108Enable();								// enable
		
		// second page
		ks0108GotoXY(ks0108Coord.x, ks0108Coord.y+8);
		
		if(ks0108FontTransparent == TRANSPARENT)					// save command port
			displayData = ks0108ReadData();
		else
			displayData=0;


		PORTB &= ~(_BV(CSEL1)|_BV(CSEL2));  //restore command port
		PORTB |= cmdPort; // restore command port
		
		LCD_CMD_PORT &=~(_BV(R_W)|_BV(D_I)|_BV(EN)); // restore command port
		LCD_CMD_PORT |= cmdPortC; // restore command port

		LCD_DATA_DIR = 0xFF;						// data port is output
		
		displayData |= data >> (8-yOffset);
		
		if(ks0108Inverted)
			displayData = ~displayData;
		LCD_DATA_OUT = displayData;					// write data
		ks0108Enable();								// enable
		
		ks0108GotoXY(ks0108Coord.x+1, ks0108Coord.y-8);
	} else {
		if(ks0108Inverted)
			data = ~data;
		LCD_DATA_OUT = data;						// write data
		ks0108Enable();								// enable
		ks0108Coord.x++;
	}
	LCD_DATA_OUT = 0x00;
}
