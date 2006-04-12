#include <avr/io.h>

#include "sed1565.h"

#define RS		0x02
#define	CS		0x01
#define A0		0x03
#define RW		0x05
#define EN		0x04

#define LCD_DATA_OUT	PORTD	//Port für D0-7
#define LCD_DATA_IN	PIND
#define LCDDD	DDRD

#define LCD_CMD_PORT	PORTB	//Port für Steuerleitungen
#define LCDCDD	DDRB

void dispWriteCommand (unsigned char byte) {
  LCD_CMD_PORT &= ~(0x01 << A0);
  LCD_DATA_OUT = byte;
  LCD_CMD_PORT &= ~(0x01 << CS);
  dispEnable();
  LCD_CMD_PORT |= (0x01 << CS);
}

void dispWriteByte (unsigned char byte) {
  LCD_CMD_PORT |= (0x01 << A0);
  LCD_DATA_OUT = byte;
  LCD_CMD_PORT &= ~(0x01 << CS);
  dispEnable();
  LCD_CMD_PORT |= (0x01 << CS);
}

void dispInit(void) {        //Initialize LCD
  LCDDD = 0xFF;
  LCDCDD |= (0x01 << EN) | (0x01 << A0) | (0x01 << CS) | (0x01 << RS) | 0x01 << RW;
  LCD_CMD_PORT |= (0x01 << CS) | (0x01 << RS);

	dispWriteCommand(0x40);		// Start Display Line = 0
	dispWriteCommand(0xA0);		// ADC = normal (Anzeigerichtung nicht gespiegelt)
	dispWriteCommand(0xA3);		// LCD-Bias = 1/7 für 4,5-5,5V    1/9 für 3,3-4,5V
	dispWriteCommand(0xC0);		// Output Mode = nicht gespiegelt
	dispWriteCommand(0x2F);		// LCD-Spannungserzeugung/Regelung einschalten
	dispWriteCommand(0x20);		// V5 Regulator Resistor Ratio = 0
	dispWriteCommand(0xAC);		// Static Indicator off
	dispWriteCommand(0x81);		// Kontrasteinstellung auswählen
	dispWriteCommand(32);		// Kontrast = mittel (für Bias 1/7: 20-50, für 1/9: 2-10)
	dispWriteCommand(0xA4);		// Displaytest aus
	dispWriteCommand(0xAF);		// Display ein
	dispWriteCommand(0xA6);		// Display normal (nicht invertiert)
	dispClear();
}

void dispClear(void) {
  unsigned char x, y;
  for (y = 0; y < 8; y++) {
    dispGotoXY (0, y);
	for (x = 0; x < 128; x++)
	  dispWriteByte(0);
  }
  dispGotoXY (0, 0);
}

void dispGotoXY (uint8_t x, uint8_t y) {
	dispWriteCommand(0xB0|y);
	dispWriteCommand(0x10|(x/16));
	dispWriteCommand(x&15);
}

void dispEnable (void) {
	LCD_CMD_PORT |= (0x01 << EN);
	LCD_CMD_PORT &= ~(0x01 << EN);
}

unsigned char dispReadByte (void) {
  unsigned char byte;

  LCD_CMD_PORT |= (0x01 << A0);

  LCD_DATA_OUT = 0x00;
  LCDDD = 0x00;
  LCD_CMD_PORT &= ~(0x01 << CS);
  LCD_CMD_PORT |= (0x01 << RW);
  LCD_CMD_PORT |= (0x01 << EN);

  byte = LCD_DATA_IN;
  LCD_CMD_PORT &= ~(0x01 << EN) & ~(0x01 << RW);
  LCD_CMD_PORT |= (0x01 << CS);
  LCDDD = 0xff;
  return byte;
}

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

void dispFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) 
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

