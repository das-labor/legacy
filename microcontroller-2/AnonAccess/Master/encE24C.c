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
#include "24Cxxx.h"
#include "sha256.h"
#include "shabea.h"
#include "hmac-sha256.h"
#include "debug.h"
#include "uart.h"
#include "memxor.h"

#define BLOCKSIZE 128
#define CRYPTKEY_SIZE 256
#define ESSIVKEY_SIZE 256

typedef uint32_t blockdev_ptr_t;
typedef uint16_t blockaddr_t;

#if 0

/* already include in 24CBlockDev.c */
i2c_addr_t getDevAddr(blockdev_ptr_t baddr){
	i2c_addr_t devaddr;
	uint8_t devmap[8] = { /* map logical addr to phy. dev addr */
		0xA0, 0xA8,
		0xA2, 0xAA,
		0xA4, 0xAC,
		0xA6, 0xAE };
	
	devaddr = ((uint8_t*)&baddr)[2]; /* a device has 64KByte storage space */
	devaddr = devmap[devaddr&0x3];
	return devaddr;
}


#else

i2c_addr_t getDevAddr(blockdev_ptr_t baddr);

#endif

void encrypt_E24Cblock(void *dest, blockaddr_t blockaddr, uint8_t * essiv_key, uint8_t * crypt_key){
	uint8_t essiv[32];
	uint8_t i;
	hmac_sha256(essiv, essiv_key, ESSIVKEY_SIZE, &blockaddr, 8*sizeof(blockaddr_t));
	memxor(dest, essiv, 32);
	memset(essiv, 0, 32); /* kill essiv for security */
	shabea256(dest,crypt_key, CRYPTKEY_SIZE, 1, 16);
	dest = (uint8_t*)dest + 32;
	for(i=0; i<BLOCKSIZE/32-1; ++i){
		memxor(dest, (uint8_t*)dest-32, 32);
		shabea256(dest,crypt_key, CRYPTKEY_SIZE, 1, 16);
		dest = (uint8_t*)dest + 32;
	}
	E24C_page_write(getDevAddr(blockaddr * BLOCKSIZE), (blockaddr*BLOCKSIZE)&0xFFFF, (uint8_t*)dest-BLOCKSIZE, BLOCKSIZE);
	uart_putc(',');
}

void decrypt_E24Cblock(void *dest, blockaddr_t blockaddr, uint8_t * essiv_key, uint8_t * crypt_key){
	uint8_t essiv[32];
	uint8_t i;
	E24C_block_read(getDevAddr(blockaddr * BLOCKSIZE), (blockaddr*BLOCKSIZE)&0xFFFF, (uint8_t*)dest, BLOCKSIZE);	
	dest = (uint8_t*)dest + BLOCKSIZE;
	for(i=0; i<BLOCKSIZE/32-1; ++i){
		dest = (uint8_t*)dest - 32;
		shabea256(dest,crypt_key, CRYPTKEY_SIZE, 0, 16);
		memxor(dest, (uint8_t*)dest-32, 32);
	}
	dest = (uint8_t*)dest - 32;
	shabea256(dest,crypt_key, CRYPTKEY_SIZE, 0, 16);
	hmac_sha256(essiv, essiv_key, ESSIVKEY_SIZE, &blockaddr, 8*sizeof(blockaddr_t));
	memxor(dest, essiv, 32);
	memset(essiv, 0, 32);
}


void crypto_read_block(void * dest, blockdev_ptr_t addr, uint16_t length, 
                       void * essiv_key, void * crypt_key){
	uint8_t page[BLOCKSIZE];
	uint8_t rl;
	
	decrypt_E24Cblock(page, addr/BLOCKSIZE, essiv_key, crypt_key);
	rl = BLOCKSIZE - (addr&(BLOCKSIZE-1));/* space left to blockend */
	if(rl>length){
		rl=length;
	}

	memcpy(dest, page+(addr&(BLOCKSIZE-1)), rl);
	dest = (uint8_t*)dest + rl;
	length -= rl;
	addr = addr/BLOCKSIZE +1; /* now addr addresses blocks */
	while(length){
		decrypt_E24Cblock(page, addr++, essiv_key, crypt_key);
		rl = (length>128)?128:length;
		memcpy(dest, page, rl);
		dest = (uint8_t*)dest + rl;
		length -= rl;
	}
}


void crypto_set_block(uint8_t value, blockdev_ptr_t addr, uint16_t length, 
                       void * essiv_key, void * crypt_key){
	uint8_t page[BLOCKSIZE];
	uint8_t rl;
	
	decrypt_E24Cblock(page, addr/BLOCKSIZE, essiv_key, crypt_key);
	rl = BLOCKSIZE - (addr&(BLOCKSIZE-1));/* space left to blockend */
	if(rl>length){
		rl=length;
	}
	memset(page+BLOCKSIZE-rl, value, rl);
	encrypt_E24Cblock(page, addr/BLOCKSIZE, essiv_key, crypt_key);
	length -= rl;
	addr = addr/BLOCKSIZE +1; /* now addr addressesn blocks */
	while(length>=BLOCKSIZE){
		memset(page, value, BLOCKSIZE);
		encrypt_E24Cblock(page, addr++, essiv_key, crypt_key);
		rl = (length>128)?128:length;
		length -= rl;
	}
	decrypt_E24Cblock(page, addr, essiv_key, crypt_key);
	memset(page, value, rl);
	encrypt_E24Cblock(page, addr, essiv_key, crypt_key);
}



void crypto_write_block(void *dest, blockdev_ptr_t addr, uint16_t length, 
                       void * essiv_key, void * crypt_key){
	uint8_t page[BLOCKSIZE];
	uint8_t rl;
	
	decrypt_E24Cblock(page, addr/BLOCKSIZE, essiv_key, crypt_key);
	rl = BLOCKSIZE - (addr&(BLOCKSIZE-1));/* space left to blockend */
	if(rl>length){
		rl=length;
	}
	memcpy(page+BLOCKSIZE-rl, dest, rl);
	encrypt_E24Cblock(page, addr/BLOCKSIZE, essiv_key, crypt_key);
	length -= rl;
	dest = (uint8_t*)dest +rl;
	addr = addr/BLOCKSIZE +1; /* now addr addressesn blocks */
	while(length>=BLOCKSIZE){
		memcpy(page, dest, BLOCKSIZE);
		encrypt_E24Cblock(page, addr++, essiv_key, crypt_key);
		rl = (length>128)?128:length;
		length -= rl;
	}
	decrypt_E24Cblock(page, addr, essiv_key, crypt_key);
	memcpy(page, dest, rl);
	encrypt_E24Cblock(page, addr, essiv_key, crypt_key);
}

