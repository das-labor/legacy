#include "config.h"
#include "lcd_tools.h"
#include "keypad.h"

#include <avr/pgmspace.h>

char keypad_tab [] PROGMEM = { 
							   '0', '1', '2', '3',
	                           '4', '5', '6', '7',
	                           '8', '9', 'A', 'B',
	                           'C', 'D', 'E', 'F'
	                           } ;


char read_keypad(void){
	uint8_t i, j;
	uint8_t rowx[4], collx[4];

	//defines Uebergeben
	rowx[0] = ROW1;
	rowx[1] = ROW2;
	rowx[2] = ROW3;
	rowx[3] = ROW4;
	collx[0] = COLL1;
	collx[1] = COLL2;
	collx[2] = COLL3;
	collx[3] = COLL4;

	//Zeilen und Spalten abfragen
	for (i=0; i<4; i++){
		DDR_ROWS |= (1 << rowx[i]);
		for(j=0; j<4; j++){
			if ((PIN_COLLS & (1 << collx[j])) == 0){
				DDR_ROWS &= ~(1 << rowx[i]);
				return pgm_read_byte(keypad_tab+j+(i*4));
			}
		}
		DDR_ROWS &= ~(1 << rowx[i]);
	}
	
	return ' ';
}

void keypad_init(void){
	PORT_COLLS |= (1 << COLL1) | (1 << COLL2) | (1 << COLL3) | (1 << COLL4);
}

