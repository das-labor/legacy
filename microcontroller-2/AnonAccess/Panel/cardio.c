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

