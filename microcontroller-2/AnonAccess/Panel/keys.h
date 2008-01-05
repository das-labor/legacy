#ifndef KEYS_H_
#define KEYS_H_

#include <stdint.h>
#include <avr/eeprom.h>

#define KEYSIZE  256
#define KEYSIZEB ((KEYSIZE+7)/8)
#define KEY_NUM 1

void delete_key(void* key, uint16_t length);
void load_absignkey(void* dest);


#endif /*KEYS_H_*/
