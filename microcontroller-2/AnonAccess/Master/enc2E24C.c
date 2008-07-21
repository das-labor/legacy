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
#include "enc2E24C.h"
#include "memxor.h"

#define BLOCKSIZE 32
#define CRYPTKEY_SIZE 256


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

void encrypt_E24Cblock(void *dest, blockdev_ptr_t addr, uint8_t * crypt_key){
	uint8_t key[CRYPTKEY_SIZE/8+sizeof(blockdev_ptr_t)];
	
	addr &= ~(BLOCKSIZE-1); /* set addr to the blocks base address*/
	memcpy(key, crypt_key, CRYPTKEY_SIZE/8);
	memcpy(key + CRYPTKEY_SIZE/8, &addr, sizeof(blockdev_ptr_t));
	shabea256(dest, key, CRYPTKEY_SIZE+8*sizeof(blockdev_ptr_t), 1, 8);
	memset(key, 0, CRYPTKEY_SIZE/8+sizeof(blockdev_ptr_t));
	E24C_page_write(getDevAddr(addr), addr&0xFFFF, dest, BLOCKSIZE);
}

void decrypt_E24Cblock(void *dest, blockdev_ptr_t addr, uint8_t * crypt_key){
	uint8_t key[CRYPTKEY_SIZE/8+sizeof(blockdev_ptr_t)];
	
	addr &= ~(BLOCKSIZE-1);
	E24C_block_read(getDevAddr(addr), addr&0xFFFF, dest, BLOCKSIZE);
	memcpy(key, crypt_key, CRYPTKEY_SIZE/8);
	memcpy(key + CRYPTKEY_SIZE/8, &addr, sizeof(blockdev_ptr_t));
	shabea256(dest, key, CRYPTKEY_SIZE+8*sizeof(blockdev_ptr_t), 0, 8);
	memset(key, 0, CRYPTKEY_SIZE/8+sizeof(blockdev_ptr_t));
}


void crypto_read_block(void * dest, blockdev_ptr_t addr, uint16_t length, 
                       void * crypt_key){
	uint8_t page[BLOCKSIZE];
	uint8_t rl;
	
	decrypt_E24Cblock(page, addr&~(BLOCKSIZE-1), crypt_key);
	rl = BLOCKSIZE - (addr&(BLOCKSIZE-1));/* space left to blockend */
	if(rl>length){
		rl=length;
	}

	memcpy(dest, page+(addr&(BLOCKSIZE-1)), rl);
	dest = (uint8_t*)dest + rl;
	length -= rl;
	addr += BLOCKSIZE; /* now addr addresses blocks */
	while(length){
		decrypt_E24Cblock(page, addr, crypt_key);
		addr += BLOCKSIZE;
		rl = (length>BLOCKSIZE)?BLOCKSIZE:length;
		memcpy(dest, page, rl);
		dest = (uint8_t*)dest + rl;
		length -= rl;
	}
}


void crypto_set_block(uint8_t value, blockdev_ptr_t addr, uint32_t length, 
                      void * crypt_key){
	uint8_t page[BLOCKSIZE];
	uint8_t rl;
	
	decrypt_E24Cblock(page, addr&(~(BLOCKSIZE-1)), crypt_key);
	rl = BLOCKSIZE - (addr&(BLOCKSIZE-1));/* space left to blockend */
	memset(page+BLOCKSIZE-rl, value, (rl<length)?rl:length);
	encrypt_E24Cblock(page, addr&(~(BLOCKSIZE-1)), crypt_key);
	if(rl>length){
		rl=length;
	}
	length -= rl;
	addr += BLOCKSIZE; /* now addr addressesn blocks */
	while(length>=BLOCKSIZE){
		memset(page, value, BLOCKSIZE);
		encrypt_E24Cblock(page, addr, crypt_key);
		addr += BLOCKSIZE;
		length -= BLOCKSIZE;
	}
	if(length){
		decrypt_E24Cblock(page, addr, crypt_key);
		memset(page, value, length);
		encrypt_E24Cblock(page, addr, crypt_key);
	}
}



void crypto_write_block(void *dest, blockdev_ptr_t addr, uint16_t length, 
                        void * crypt_key){
	uint8_t page[BLOCKSIZE];
	uint8_t rl;
	
	decrypt_E24Cblock(page, addr&~(BLOCKSIZE-1), crypt_key);
	rl = BLOCKSIZE - (addr&(BLOCKSIZE-1));/* space left to blockend */
	memcpy(page+BLOCKSIZE-rl, dest, (rl>length)?length:rl);
	encrypt_E24Cblock(page, addr&~(BLOCKSIZE-1), crypt_key);
	if(rl>length){
		rl=length;
	}
	length -= rl;
	dest = (uint8_t*)dest +rl;
	addr += BLOCKSIZE; /* now addr addressesn blocks */
	while(length>=BLOCKSIZE){
		memcpy(page, dest, BLOCKSIZE);
		dest = (uint8_t*)dest+BLOCKSIZE;
		encrypt_E24Cblock(page, addr, crypt_key);
		addr += BLOCKSIZE;
		length -= BLOCKSIZE;
	}
	if(length){
		decrypt_E24Cblock(page, addr, crypt_key);
		memcpy(page, dest, length);
		encrypt_E24Cblock(page, addr, crypt_key);
	}
}

