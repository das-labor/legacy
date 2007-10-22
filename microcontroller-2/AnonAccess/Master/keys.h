#ifndef KEYS_H_
#define KEYS_H_

#include <stdint.h>
#include <avr/eeprom.h>

void delete_key(uint8_t * key, uint16_t length);
void load_ticketkey(uint8_t * dest);
void load_absignkey(uint8_t * dest);
void load_ridkey(uint8_t * dest);
void load_nickkey(uint8_t * dest);
void load_timestampkey(uint8_t * dest);
void load_eeprom_crypt_key(uint8_t * dest);
void load_eeprom_essiv_key(uint8_t * dest);

#endif /*KEYS_H_*/
