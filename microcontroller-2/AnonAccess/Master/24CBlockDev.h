/* 24CBlockDev.h */
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


#ifndef _24CBLOCKDEV_H_
#define _24CBLOCKDEV_H_

#include <stdint.h>

#define E24CBLOCKDEV_SIZE (1024/8)  /* size in KibiByte*/

typedef uint32_t blockdev_ptr_t;

void E24C_blockdev_init(void);
void E24C_blockdev_writeByte(blockdev_ptr_t addr, uint8_t data);
void E24C_blockdev_writeBlock(blockdev_ptr_t addr,  void* block, uint16_t length);
void E24C_blockdev_setBlock(blockdev_ptr_t addr, uint8_t b, uint32_t length);

uint8_t E24C_blockdev_readByte(blockdev_ptr_t addr);
void E24C_blockdev_readBlock(blockdev_ptr_t addr, void* block, uint16_t length);


#endif /*_24CBLOCKDEV_H_*/
