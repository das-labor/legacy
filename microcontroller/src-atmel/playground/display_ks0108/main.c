/*
 * Copyright:      Fabian Maximilian Thiele  mailto:me@apetech.de
 * Author:         Fabian Maximilian Thiele
 * Remarks:        this Copyright must be included
 * known Problems: none
 * Version:        1.1
 * Description:    KS0108 Library Demo Program 
 * 
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "ks0108.h"
#include "arial_bold_14.h"
#include "corsiva_12.h"

int main(void) {
	
	// Wait a little while the display starts up
	volatile uint16_t i;
	for(i=0; i<15000; i++);
		
	displayInit();
	
	setPixel(5,5, 1);
	setPixel(5,6,1);
	setPixel(5,7,1);
	setPixel(6,5,1);
	setPixel(7,5,1);
	setPixel(8,5,1);
	setPixel(9,5,1);
	

/*	
	// Initialize the LCD
	ks0108Init(0);
	
	// Select a font
	ks0108SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	// Set a position
	ks0108GotoXY(15,10);
	// Print some text
	ks0108Puts_P(PSTR("KS0108-Treiber"));
	// a nice little round rect
	ks0108DrawRoundRect(5, 5, 117, 20, 8, BLACK);

	// Once again :)
	// Select a font
	ks0108SelectFont(Corsiva_12, ks0108ReadFontData, BLACK);
	// Set a position
	ks0108GotoXY(5,30);
	// Print some text
	ks0108Puts_P(PSTR("http://www.apetech.de\nmailto:me@apetech.de"));
*/
	while(1);
}
