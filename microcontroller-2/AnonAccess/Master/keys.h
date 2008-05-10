#ifndef KEYS_H_
#define KEYS_H_

#include <stdint.h>
#include <avr/eeprom.h>

#define KEYSIZE  256
#define KEYSIZEB ((KEYSIZE+7)/8)
#define KEY_NUM 9

void delete_key(void* key, uint16_t length);
void load_ticketkey(void* dest);
void load_ticketkeydiv1(void* dest);
void load_ticketkeydiv2(void* dest);
void load_absignkey(void* dest);
void load_ridkey(void* dest);
void load_nickkey(void* dest);
void load_timestampkey(void* dest);
void load_pinmac_key(void* dest);
void load_pinenc_key(void* dest);
void load_eeprom_crypt_key(void* dest);
void do_keymigrate(void);

extern uint8_t eeprom_key[32];

#endif /*KEYS_H_*/
