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

#define ASN1_TAG (0x40 | 23) /* Application specific, number 23 */
#define ASN1_LENGTH (sizeof(authblock_t)+32) /* 127<ASN1_LENGTH<256*/

void cardio_init(void){
	DDRC &= ~(_BV(7));
}

bool cardinserated(void){
	return (PINC&_BV(7))?true:false;
} 

bool readABfromCard(authblock_t * ab){
	if(!cardinserated()){
		return false;
	}
	uint8_t buffer[1+2+sizeof(authblock_t)+32];
	E24C04_block_read(0xA0, 0, buffer, 1+2+sizeof(authblock_t)+32);
	if(buffer[0]!=ASN1_TAG)
		return false;
	if(buffer[1]!=0x81)
		return false;
	if(buffer[2]!=ASN1_LENGTH)
		return false;
	/* ok ASN.1 header seems to be right */
	/* now we check the signature */
	uint8_t key[32];
	uint8_t hmac[32];
	load_absignkey(key);
	hmac_sha256(hmac, key, 256, buffer+3, sizeof(authblock_t)*8);
	delete_key(key, 32);
	if(memcmp(hmac, buffer+3+sizeof(authblock_t), 32))
		return false;
	/* signature seems to be ok */
	memcpy(ab, buffer+3, sizeof(authblock_t));
	return true;
}


bool writeABtoCard(authblock_t * ab){
	if(cardinserated()==false){
		return false;
	}
	uint8_t buffer[1+2+sizeof(authblock_t)+32];
	buffer[0] = ASN1_TAG;
	buffer[1] = 0x81;
	buffer[2] = ASN1_LENGTH;
	memcpy(buffer+3, ab, sizeof(authblock_t));
	uint8_t key[32];
	load_absignkey(key);
	hmac_sha256(buffer+3+sizeof(authblock_t), key, 256, ab, sizeof(authblock_t)*8);
	delete_key(key, 32);
	int16_t i;
	for(i=0; i+8<1+2+sizeof(authblock_t)+32; i+=8){
		E24C04_page_write(0xA0, i, buffer+i, 8);
		_delay_ms(10);
	}
	E24C04_page_write(0xA0, i, buffer+i, (1+2+sizeof(authblock_t)+32)%8);
	_delay_ms(10);
	return true;
}

