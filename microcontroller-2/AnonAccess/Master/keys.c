
#include "keys.h"
#include <avr/eeprom.h>
#include <string.h>

/****************************************************
 ****************************************************/

uint8_t ticket_key[32] EEMEM = {0x8c, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};

uint8_t absign_key[32] EEMEM = {0x9c, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};

uint8_t rid_key[32] EEMEM    = {0xac, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};

uint8_t nick_key[32] EEMEM   = {0xbc, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};

uint8_t timestamp_key[32] EEMEM   = {0xcc, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};
 
uint8_t eeprom_essiv_key[32] EEMEM   = {0xdc, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};
 
uint8_t eeprom_crypt_key[32] EEMEM   = {0xec, 0x40, 0xc8, 0x3c, 0x84, 0x11, 0x8a, 0xec,
                                0x66, 0x66, 0x5d, 0x9f, 0x3b, 0x79, 0x55, 0x63,
                                0x68, 0x3e, 0xc3, 0xb2, 0x20, 0xc6, 0xa7, 0x7e,
                                0x13, 0x54, 0x88, 0x77, 0x02, 0x9b, 0xb9, 0x8a};
 
uint8_t eeprom_key[32]; 
 
 
void delete_key(uint8_t * key, uint16_t length){
	memset(key, 0x00, length);
	memset(key, 0xFF, length);
} 
                                
void load_ticketkey(uint8_t * dest){
	eeprom_read_block(dest, ticket_key, 32);
} 

void load_absignkey(uint8_t * dest){
	eeprom_read_block(dest, absign_key, 32);
}

void load_ridkey(uint8_t * dest){
	eeprom_read_block(dest, rid_key, 32);
}

void load_nickkey(uint8_t * dest){
	eeprom_read_block(dest, nick_key, 32);
}

void load_timestampkey(uint8_t * dest){
	eeprom_read_block(dest, timestamp_key, 32);
}

void load_eeprom_crypt_key(uint8_t * dest){
	eeprom_read_block(dest, eeprom_crypt_key, 32);
}

void load_eeprom_essiv_key(uint8_t * dest){
	eeprom_read_block(dest, eeprom_essiv_key, 32);
}
