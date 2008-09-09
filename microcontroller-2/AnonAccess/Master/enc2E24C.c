/* enc2E24C.c */
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


/***
 * encrypted E24device
 * Author: Daniel Otte
 * License: GPLv3 or later
 * 
 * 
 */

#include <stdint.h>
#include <string.h>
#include "config.h"
#include "24Cxxx.h"
#include "debug.h"
#include "uart.h"
#include "enc2E24C.h"
#include "memxor.h"


#ifdef EEPROM_ENC_SHABEA
 #include "sha256.h"
 #include "shabea.h"
 #include "hmac-sha256.h"
#endif

#ifdef EEPROM_ENC_NOEKEON
 #include "noekeon.h"
 #include "memxor.h"
#endif


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

#ifdef EEPROM_ENC_SHABEA
void encrypt_E24Cblock(void *dest, blockdev_ptr_t addr, uint8_t * crypt_key){
	uint8_t key[CRYPTKEY_SIZE/8+sizeof(blockdev_ptr_t)];
	
	addr &= ~(ENC24C_BLOCKSIZE-1); /* set addr to the blocks base address*/
	memcpy(key, crypt_key, CRYPTKEY_SIZE/8);
	memcpy(key + CRYPTKEY_SIZE/8, &addr, sizeof(blockdev_ptr_t));
	shabea256(dest, key, CRYPTKEY_SIZE+8*sizeof(blockdev_ptr_t), 1, 8);
	memset(key, 0, CRYPTKEY_SIZE/8+sizeof(blockdev_ptr_t));
	E24C_page_write(getDevAddr(addr), addr&0xFFFF, dest, ENC24C_BLOCKSIZE);
}

void decrypt_E24Cblock(void *dest, blockdev_ptr_t addr, uint8_t * crypt_key){
	uint8_t key[CRYPTKEY_SIZE/8+sizeof(blockdev_ptr_t)];
	
	addr &= ~(ENC24C_BLOCKSIZE-1);
	E24C_block_read(getDevAddr(addr), addr&0xFFFF, dest, ENC24C_BLOCKSIZE);
	memcpy(key, crypt_key, CRYPTKEY_SIZE/8);
	memcpy(key + CRYPTKEY_SIZE/8, &addr, sizeof(blockdev_ptr_t));
	shabea256(dest, key, CRYPTKEY_SIZE+8*sizeof(blockdev_ptr_t), 0, 8);
	memset(key, 0, CRYPTKEY_SIZE/8+sizeof(blockdev_ptr_t));
}
#else
#ifdef EEPROM_ENC_NOEKEON
void encrypt_E24Cblock(void *dest, blockdev_ptr_t addr, const uint8_t * crypt_key){
	uint8_t key[16];
	
	addr &= ~(ENC24C_BLOCKSIZE-1); /* set addr to the blocks base address*/
	memset(key, 'B', 16); 
	memxor(key+16-sizeof(blockdev_ptr_t), &addr, sizeof(blockdev_ptr_t));
	noekeon_enc(dest, crypt_key);
	noekeon_enc(dest, key);
	noekeon_enc(dest, crypt_key+16);
	E24C_page_write(getDevAddr(addr), addr&0xFFFF, dest, ENC24C_BLOCKSIZE);
}

void decrypt_E24Cblock(void *dest, blockdev_ptr_t addr, const uint8_t * crypt_key){
	uint8_t key[16];
	
	addr &= ~(ENC24C_BLOCKSIZE-1);
	E24C_block_read(getDevAddr(addr), addr&0xFFFF, dest, ENC24C_BLOCKSIZE);
	memset(key, 'B', 16); 
	memxor(key+16-sizeof(blockdev_ptr_t), &addr, sizeof(blockdev_ptr_t));
	noekeon_dec(dest, crypt_key+16);
	noekeon_dec(dest, key);
	noekeon_dec(dest, crypt_key);
}
#endif
#endif


void crypto_read_block(void * dest, blockdev_ptr_t addr, uint16_t length, 
                       void * crypt_key){
	uint8_t page[ENC24C_BLOCKSIZE];
	uint8_t rl;
	
	decrypt_E24Cblock(page, addr&~(ENC24C_BLOCKSIZE-1), crypt_key);
	rl = ENC24C_BLOCKSIZE - (addr&(ENC24C_BLOCKSIZE-1));/* space left to blockend */
	if(rl>length){
		rl=length;
	}

	memcpy(dest, page+(addr&(ENC24C_BLOCKSIZE-1)), rl);
	dest = (uint8_t*)dest + rl;
	length -= rl;
	addr += ENC24C_BLOCKSIZE; /* now addr addresses blocks */
	while(length){
		decrypt_E24Cblock(page, addr, crypt_key);
		addr += ENC24C_BLOCKSIZE;
		rl = (length>ENC24C_BLOCKSIZE)?ENC24C_BLOCKSIZE:length;
		memcpy(dest, page, rl);
		dest = (uint8_t*)dest + rl;
		length -= rl;
	}
}


void crypto_set_block(uint8_t value, blockdev_ptr_t addr, uint32_t length, 
                      void * crypt_key){
	uint8_t page[ENC24C_BLOCKSIZE];
	uint8_t rl;
	
	decrypt_E24Cblock(page, addr&(~(ENC24C_BLOCKSIZE-1)), crypt_key);
	rl = ENC24C_BLOCKSIZE - (addr&(ENC24C_BLOCKSIZE-1));/* space left to blockend */
	memset(page+ENC24C_BLOCKSIZE-rl, value, (rl<length)?rl:length);
	encrypt_E24Cblock(page, addr&(~(ENC24C_BLOCKSIZE-1)), crypt_key);
	if(rl>length){
		rl=length;
	}
	length -= rl;
	addr += ENC24C_BLOCKSIZE; /* now addr addressesn blocks */
	while(length>=ENC24C_BLOCKSIZE){
		memset(page, value, ENC24C_BLOCKSIZE);
		encrypt_E24Cblock(page, addr, crypt_key);
		addr += ENC24C_BLOCKSIZE;
		length -= ENC24C_BLOCKSIZE;
	}
	if(length){
		decrypt_E24Cblock(page, addr, crypt_key);
		memset(page, value, length);
		encrypt_E24Cblock(page, addr, crypt_key);
	}
}



void crypto_write_block(void *dest, blockdev_ptr_t addr, uint16_t length, 
                        void * crypt_key){
	uint8_t page[ENC24C_BLOCKSIZE];
	uint8_t rl;
	
	decrypt_E24Cblock(page, addr&~(ENC24C_BLOCKSIZE-1), crypt_key);
	rl = ENC24C_BLOCKSIZE - (addr&(ENC24C_BLOCKSIZE-1));/* space left to blockend */
	memcpy(page+ENC24C_BLOCKSIZE-rl, dest, (rl>length)?length:rl);
	encrypt_E24Cblock(page, addr&~(ENC24C_BLOCKSIZE-1), crypt_key);
	if(rl>length){
		rl=length;
	}
	length -= rl;
	dest = (uint8_t*)dest +rl;
	addr += ENC24C_BLOCKSIZE; /* now addr addressesn blocks */
	while(length>=ENC24C_BLOCKSIZE){
		memcpy(page, dest, ENC24C_BLOCKSIZE);
		dest = (uint8_t*)dest+ENC24C_BLOCKSIZE;
		encrypt_E24Cblock(page, addr, crypt_key);
		addr += ENC24C_BLOCKSIZE;
		length -= ENC24C_BLOCKSIZE;
	}
	if(length){
		decrypt_E24Cblock(page, addr, crypt_key);
		memcpy(page, dest, length);
		encrypt_E24Cblock(page, addr, crypt_key);
	}
}

