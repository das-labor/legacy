#ifndef _24CBLOCKDEV_H_
#define _24CBLOCKDEV_H_

#include <stdint.h>

#define E24CBLOCKDEV_SIZE 1024/8  /* size in KibiByte*/

typedef uint32_t blockdev_ptr_t;

void E24C_blockdev_init();
void E24C_blockdev_writeByte(blockdev_ptr_t addr, uint8_t data);
void E24C_blockdev_writeBlock(blockdev_ptr_t addr, const void* block, uint16_t length);
uint8_t E24C_blockdev_readByte(blockdev_ptr_t addr);
void E24C_blockdev_readBlock(blockdev_ptr_t addr, void* block, uint16_t length);


#endif /*_24CBLOCKDEV_H_*/
