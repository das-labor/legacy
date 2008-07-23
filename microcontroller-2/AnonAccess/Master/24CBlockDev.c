/* 24CBlockDev.c */
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


/*
 * Author:	Daniel Otte
 * Date:		06.06.2006
 * License:	GPLv3
 * 
 **/

#include "24Cxxx.h"
#include "24CBlockDev.h"
#include "config.h"
#include "debug.h"

i2c_addr_t getDevAddr(blockdev_ptr_t baddr){
	i2c_addr_t devaddr;
	uint8_t devmap[8] = { /* map logical addr to phy. dev addr */
		0xA0, 0xA8,
		0xA2, 0xAA,
		0xA4, 0xAC,
		0xA6, 0xAE };
	
	devaddr = ((uint8_t*)&baddr)[2]; /* a device has 64KByte storage space */
//	devaddr = ((devaddr<<1) |  (devaddr>>2)) & 0x3;
//	devaddr += 0xA0;
	devaddr = devmap[devaddr&0x3];
	return devaddr;
}

void E24C_blockdev_init(void){
}

void E24C_blockdev_writeByte(blockdev_ptr_t addr, uint8_t data){
/*	DEBUG_S("\r\nWriting byte ");
	DEBUG_B(data);
	DEBUG_S(" to dev: ");
	DEBUG_B(getDevAddr(addr));
	DEBUG_S(" @ ");
	DEBUG_B(((uint8_t*)&addr)[3]);
	DEBUG_B(((uint8_t*)&addr)[2]);
	DEBUG_B(((uint8_t*)&addr)[1]);
	DEBUG_B(((uint8_t*)&addr)[0]);
*/	E24C_byte_write(getDevAddr(addr), (uint16_t)addr, data);
}
/*
void E24C_blockdev_setBlock(blockdev_ptr_t addr, b, uint32_t length){
	if (((uint32_t)length + (addr & 0xffff))&0x10000){ / * if spreading over two devices * /
		E24C_page_write(getDevAddr(addr), (uint16_t)addr, block, 0x10000-length-(addr&0xffff));		
		E24C_page_write(getDevAddr(addr+(0x10000-length-(addr&0xffff))),
				 (uint16_t)addr, block+(0x10000-length-(addr&0xffff)), length-(0x10000-length-(addr&0xffff)));
	} else {
		DEBUG_S("\r\n pagewrite");
		DEBUG_S(" to dev: ");
		DEBUG_B(getDevAddr(addr));
		DEBUG_S(" @ ");
		DEBUG_B(((uint8_t*)&addr)[3]);
		DEBUG_B(((uint8_t*)&addr)[2]);
		DEBUG_B(((uint8_t*)&addr)[1]);
		DEBUG_B(((uint8_t*)&addr)[0]);
		DEBUG_S(" <");
		DEBUG_B(((uint8_t*)&length)[1]);
		DEBUG_B(((uint8_t*)&length)[0]);
		DEBUG_S("> ");
		
		E24C_page_set(getDevAddr(addr), (uint16_t)addr, block, length);
	}
}
*/
/* this is buggy, we should think about pagesize & Co.
void E24C_blockdev_writeBlock(blockdev_ptr_t addr, void* block, uint16_t length){
	if (((uint32_t)length + (addr & 0xffff))&0x10000){ / * if spreading over two devices * /
		E24C_page_write(getDevAddr(addr), (uint16_t)addr, block, 0x10000-length-(addr&0xffff));		
		E24C_page_write(getDevAddr(addr+(0x10000-length-(addr&0xffff))),
				 (uint16_t)addr, block+(0x10000-length-(addr&0xffff)), length-(0x10000-length-(addr&0xffff)));
	} else {
		DEBUG_S("\r\n pagewrite");
		DEBUG_S(" to dev: ");
		DEBUG_B(getDevAddr(addr));
		DEBUG_S(" @ ");
		DEBUG_B(((uint8_t*)&addr)[3]);
		DEBUG_B(((uint8_t*)&addr)[2]);
		DEBUG_B(((uint8_t*)&addr)[1]);
		DEBUG_B(((uint8_t*)&addr)[0]);
		DEBUG_S(" <");
		DEBUG_B(((uint8_t*)&length)[1]);
		DEBUG_B(((uint8_t*)&length)[0]);
		DEBUG_S("> ");
		
		E24C_page_write(getDevAddr(addr), (uint16_t)addr, block, length);
	}
}
*/
void E24C_blockdev_writeBlock(blockdev_ptr_t addr, void* block, uint16_t length){
	uint32_t pbl, pbh; /* page boundary low/high */
	uint32_t l1,l2,l3;
	uint32_t i;

	pbl = (addr & ~(E24C_PAGESIZE-1)) + E24C_PAGESIZE;
	pbh = ((addr+length) & ~(E24C_PAGESIZE-1));
	
	l1 = (pbl>=addr+length)?length:pbl-addr;
	l3 = (pbh<=addr)?0:addr+length-pbh;
	l2 = length-l1-l3;
	
	/** DEBUG STUFF **/
/*	DEBUG_S("\r\n blockwrite");
	DEBUG_S(" to dev: ");
	DEBUG_B(getDevAddr(addr));
	DEBUG_S(" @ ");
	DEBUG_B(((uint8_t*)&addr)[3]);
	DEBUG_B(((uint8_t*)&addr)[2]);
	DEBUG_B(((uint8_t*)&addr)[1]);
	DEBUG_B(((uint8_t*)&addr)[0]);
	DEBUG_S(" <");
	DEBUG_B(((uint8_t*)&length)[1]);
	DEBUG_B(((uint8_t*)&length)[0]);
	DEBUG_S("> ");	
	
	DEBUG_S("\r\n pbh: ");
	DEBUG_B(((uint8_t*)&pbh)[3]);
	DEBUG_B(((uint8_t*)&pbh)[2]);
	DEBUG_B(((uint8_t*)&pbh)[1]);
	DEBUG_B(((uint8_t*)&pbh)[0]);
	
	DEBUG_S(" pbl: ");
	DEBUG_B(((uint8_t*)&pbl)[3]);
	DEBUG_B(((uint8_t*)&pbl)[2]);
	DEBUG_B(((uint8_t*)&pbl)[1]);
	DEBUG_B(((uint8_t*)&pbl)[0]);
	
	DEBUG_S("\r\n l1: ");
	DEBUG_B(((uint8_t*)&l1)[1]);
	DEBUG_B(((uint8_t*)&l1)[0]);
	
	DEBUG_S(" l2: ");
	DEBUG_B(((uint8_t*)&l2)[1]);
	DEBUG_B(((uint8_t*)&l2)[0]);
	
	DEBUG_S(" l3: ");
	DEBUG_B(((uint8_t*)&l3)[1]);
	DEBUG_B(((uint8_t*)&l3)[0]);
*/
	/* write the first page */
	E24C_page_write(getDevAddr(addr),(uint16_t)addr,block, l1);
	addr += l1; block = (uint8_t*)block + l1;
	
	/* write middle pages */
	for(i=0; i<l2/E24C_PAGESIZE; ++i){
		E24C_page_write(getDevAddr(addr),(uint16_t)addr,block, E24C_PAGESIZE);
		addr += E24C_PAGESIZE; block = (uint8_t*)+E24C_PAGESIZE;
	}
	/* write the last page */
	if(l3==0)
		return;
	E24C_page_write(getDevAddr(addr),(uint16_t)addr,block, l3);
	
	return;
}



void E24C_blockdev_setBlock(blockdev_ptr_t addr, uint8_t b, uint32_t length){
	uint32_t pbl, pbh; /* page boundary low/high */
	uint32_t l1,l2,l3;
	uint32_t i;

	pbl = (addr & ~(E24C_PAGESIZE-1)) + E24C_PAGESIZE;
	pbh = ((addr+length) & ~(E24C_PAGESIZE-1));
	
	l1 = (pbl>=addr+length)?length:pbl-addr;
	l3 = (pbh<=addr)?0:addr+length-pbh;
	l2 = length-l1-l3;
	
	/** DEBUG STUFF **/
/*	DEBUG_S("\r\n blockset");
	DEBUG_S(" to dev: ");
	DEBUG_B(getDevAddr(addr));
	DEBUG_S(" @ ");
	DEBUG_B(((uint8_t*)&addr)[3]);
	DEBUG_B(((uint8_t*)&addr)[2]);
	DEBUG_B(((uint8_t*)&addr)[1]);
	DEBUG_B(((uint8_t*)&addr)[0]);
	DEBUG_S(" <");
	DEBUG_B(((uint8_t*)&length)[1]);
	DEBUG_B(((uint8_t*)&length)[0]);
	DEBUG_S("> ");	
	
	DEBUG_S("\r\n pbh: ");
	DEBUG_B(((uint8_t*)&pbh)[3]);
	DEBUG_B(((uint8_t*)&pbh)[2]);
	DEBUG_B(((uint8_t*)&pbh)[1]);
	DEBUG_B(((uint8_t*)&pbh)[0]);
	
	DEBUG_S(" pbl: ");
	DEBUG_B(((uint8_t*)&pbl)[3]);
	DEBUG_B(((uint8_t*)&pbl)[2]);
	DEBUG_B(((uint8_t*)&pbl)[1]);
	DEBUG_B(((uint8_t*)&pbl)[0]);
	
	DEBUG_S("\r\n l1: ");
	DEBUG_B(((uint8_t*)&l1)[1]);
	DEBUG_B(((uint8_t*)&l1)[0]);
	
	DEBUG_S(" l2: ");
	DEBUG_B(((uint8_t*)&l2)[1]);
	DEBUG_B(((uint8_t*)&l2)[0]);
	
	DEBUG_S(" l3: ");
	DEBUG_B(((uint8_t*)&l3)[1]);
	DEBUG_B(((uint8_t*)&l3)[0]);
*/	
	/* write the first page */
	E24C_page_set(getDevAddr(addr),(uint16_t)addr,b, l1);
	addr += l1;
	/* write middle pages */
	for(i=0; i<l2/E24C_PAGESIZE; ++i){
		E24C_page_set(getDevAddr(addr),(uint16_t)addr,b, E24C_PAGESIZE);
		addr += E24C_PAGESIZE;
	}
	/* write the last page */
	if(l3==0)
		return;
	E24C_page_set(getDevAddr(addr),(uint16_t)addr,b, l3);
	
	return;
}


uint8_t E24C_blockdev_readByte(blockdev_ptr_t addr){
	return E24C_byte_read(getDevAddr(addr), (uint16_t)addr); 
}

void E24C_blockdev_readBlock(blockdev_ptr_t addr, void* block, uint16_t length){
	if (((uint32_t)length + (addr & 0xffff))&0x10000){ /* if spreading over two devices */
		E24C_block_read(getDevAddr(addr), (uint16_t)addr, block, 0x10000-length-(addr&0xffff));		
		E24C_block_read(getDevAddr(addr+(0x10000-length-(addr&0xffff))),
				 (uint16_t)addr, (uint8_t*)block+(0x10000-length-(addr&0xffff)), length-(0x10000-length-(addr&0xffff)));
	} else {
		E24C_block_read(getDevAddr(addr), (uint16_t)addr, block, length);
	}
}

