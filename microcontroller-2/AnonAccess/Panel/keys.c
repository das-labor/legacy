/* keys.c */
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



#include "keys.h"
#include "hmac-sha256.h"
#include <string.h>

/****************************************************
 ****************************************************/

uint8_t master_key[32] EEMEM = {0x00, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};

uint8_t absign_key[32] EEMEM = {0x9f, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};

 
void delete_key(void* key, uint16_t length){
	memset(key, 0x00, length);
	memset(key, 0xFF, length);
	memset(key, 0xAA, length);
	memset(key, 0x55, length);
	memset(key, 0xFF, length);
	memset(key, 0x00, length);
	memset(key, 0xFF, length);
} 
                   
void load_key(void* dest, void* key){
	uint8_t a[32],b[32];
	eeprom_read_block(a, key, 32);
	eeprom_read_block(b, master_key, 32);
	hmac_sha256(dest,b,256,a,256);
	delete_key(a,32);
	delete_key(b,32);
}                                

void load_absignkey(void* dest){
	load_key(dest, absign_key);
}

