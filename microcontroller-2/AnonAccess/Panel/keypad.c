/* keypad.c */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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

