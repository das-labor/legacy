/*
 * Author:	Daniel Otte
 * Date:		06.06.2006
 * License:	GPL
 * 
 **/

#include "config.h"
#include "debug.h"
#include "24Cxxx.h"
#include "24CBlockDev.h"

i2c_addr_t getDevAddr(blockdev_ptr_t baddr){
	i2c_addr_t devaddr;
	devaddr = ((uint8_t*)&baddr)[2];
	devaddr = ((devaddr<<1) |  (devaddr>>2)) & 0x3;
	devaddr += 0xA0;
	return devaddr;
}

void E24C_blockdev_init(){
}

void E24C_blockdev_writeByte(blockdev_ptr_t addr, uint8_t data){
	DEBUG_S("\r\nWriting byte ");
	DEBUG_B(data);
	DEBUG_S(" to dev: ");
	DEBUG_B(getDevAddr(addr));
	E24C_byte_write(getDevAddr(addr), (uint16_t)addr, data);
}

void E24C_blockdev_writeBlock(blockdev_ptr_t addr, const void* block, uint16_t length){
	/* now we can do nice aligned accesses but have to care about device boundarys */
	if (((uint32_t)length + addr & 0xffff)&0x10000){ /* if spreading over two devices */
		uint16_t l1;
		l1= 0x10000-((uint16_t)addr);
		E24C_blockdev_writeBlock(getDevAddr(addr), (uint16_t)addr, block, l1);
		addr &= 0xffff0000;
		addr += 0x10000;
		E24C_blockdev_writeBlock(getDevAddr(addr), (uint16_t)addr, block+l1, length-l1);
	} else {
		i2c_addr_t devaddr = getDevAddr(addr);
		uint16_t l1;
		l1 = 128-(addr%128);
		E24C_page_write(devaddr, (uint16_t)addr, block, (length<l1)?length:l1);
		if (length<l1){
			return void;
		} else {
			block += l1;
			length -= l1;
		}
		do{
			E24C_page_write(devaddr, (uint16_t)addr, block, (length<128)?length:128);
			length -= (length<128)?length:128;
			block  += (length<128)?length:128;
		}while (length);
	}

}

uint8_t E24C_blockdev_readByte(blockdev_ptr_t addr){
	return E24C_byte_read(getDevAddr(addr), (uint16_t)addr); 
}

void E24C_blockdev_readBlock(blockdev_ptr_t addr, void* block, uint16_t length){
	uint16_t l1 = 0x10000 - (uint16_t)addr;
	E24C_block_read(getDevAddr(addr), (uint16_t)addr, block, (length<l1)?length:l1);
	if (length>l1){
		block += l1;
		addr  += length;
		E24C_block_read(getDevAddr(addr), (uint16_t)addr, block, length-l1);
	}
}

