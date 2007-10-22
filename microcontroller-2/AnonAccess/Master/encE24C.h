#ifndef ENCE24C_H_
#define ENCE24C_H_

/***
 * encrypted E24device using ESSIV
 * Author: Daniel Otte
 * License: GPLv3
 * 
 * 
 */

#include <stdint.h>
#include <string.h>
#include "config.h"

#define BLOCKSIZE 128
#define CRYPTKEY_SIZE 256
#define ESSIVKEY_SIZE 256

#ifndef _24CBLOCKDEV_H_
 typedef uint32_t blockdev_ptr_t;
#endif 

typedef uint16_t blockaddr_t;
 

void encrypt_E24Cblock(void *dest, blockaddr_t blockaddr, uint8_t * essiv_key, uint8_t * crypt_key);
void decrypt_E24Cblock(void *dest, blockaddr_t blockaddr, uint8_t * essiv_key, uint8_t * crypt_key);

void crypto_read_block(void * dest, blockdev_ptr_t addr, uint16_t length, 
                       void * essiv_key, void * crypt_key);
void crypto_set_block(uint8_t value, blockdev_ptr_t addr, uint16_t length, 
                       void * essiv_key, void * crypt_key);
void crypto_write_block(void *dest, blockdev_ptr_t addr, uint16_t length, 
                       void * essiv_key, void * crypt_key);

#endif /*ENCE24C_H_*/
