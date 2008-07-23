/* cardio.c */
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


/**
 * author: Daniel Otte 
 * License: GPLv3
 * 
 * 
 * 
 */
 

#include "config.h" 
#include "24C04.h"
#include "types.h"
#include "hmac-sha256.h"
#include "keys.h"
#include <util/delay.h>
#include <avr/io.h>

#define TERM_SIGN_KEYSIZEB 8

#define ASN1_TAG (0x40 | 23) /* Application specific, number 23 */
#define ASN1_LENGTH (sizeof(authblock_t)+TERM_SIGN_KEYSIZEB) /* 0<=ASN1_LENGTH<128 */

void cardio_init(void){
	DDRC &= ~(_BV(7));
	PORTC |= _BV(7); /* enable pullup resistor */
}

bool card_inserated(void){
	return (PINC&_BV(7))?false:true;
} 

bool card_readAB(authblock_t * ab){
	if(!card_inserated()){
		return false;
	}
	E24C04_init();
	uint8_t buffer[1+1+sizeof(authblock_t)+TERM_SIGN_KEYSIZEB];
	E24C04_block_read(0xA0, 0, buffer, 1+1+sizeof(authblock_t)+TERM_SIGN_KEYSIZEB);
	if(buffer[0]!=ASN1_TAG)
		return false;
	if(buffer[1]!=ASN1_LENGTH)
		return false;
	/* ok ASN.1 header seems to be right */
	/* now we check the signature */
	uint8_t key[32];
	uint8_t hmac[32];
	load_absignkey(key);
	hmac_sha256(hmac, key, 256, buffer, (2+sizeof(authblock_t))*8);
	delete_key(key, 32);
	if(memcmp(hmac, buffer+2+sizeof(authblock_t), TERM_SIGN_KEYSIZEB))
		return false;
	/* signature seems to be ok */
	memcpy(ab, buffer+2, sizeof(authblock_t));
	return true;
}


bool card_writeAB(authblock_t * ab){
	if(card_inserated()==false){
		return false;
	}
	E24C04_init();
	uint8_t buffer[1+1+sizeof(authblock_t)+32];
	buffer[0] = ASN1_TAG;
	buffer[1] = ASN1_LENGTH;
	memcpy(buffer+2, ab, sizeof(authblock_t));
	uint8_t key[32];
	load_absignkey(key);
	hmac_sha256(buffer+2+sizeof(authblock_t), key, 256, buffer, (2+sizeof(authblock_t))*8);
	delete_key(key, 32);
	int16_t i;
	for(i=0; i+8<1+1+sizeof(authblock_t)+TERM_SIGN_KEYSIZEB; i+=8){
		E24C04_page_write(0xA0, i, buffer+i, 8);
		_delay_ms(10);
	}
	E24C04_page_write(0xA0, i, buffer+i, (1+1+sizeof(authblock_t)+TERM_SIGN_KEYSIZEB)%8);
	_delay_ms(10);
	return true;
}

