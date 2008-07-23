/* 24Cxxx.h */
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


#ifndef __E24CXXX_H_
#define __E24CXXX_H_

#include <stdint.h>
#include "i2c.h"

typedef uint16_t E24C_addr_t;

#define E24C_PAGESIZE 128 /* ==0x80 this implementation works only with pagesizes which are powers of 2 */

void 	E24C_init(void);
void 	E24C_byte_write (i2c_addr_t devaddr, E24C_addr_t addr, uint8_t data);
void 	E24C_page_write (i2c_addr_t devaddr, E24C_addr_t addr, uint8_t *data, uint16_t length);
void 	E24C_page_set (i2c_addr_t devaddr, E24C_addr_t addr, uint8_t data, uint16_t length);
uint8_t E24C_cur_byte_read(i2c_addr_t devaddr);
void 	E24C_cur_block_read(i2c_addr_t devaddr, E24C_addr_t addr, uint8_t *buffer, uint16_t length);
uint8_t E24C_byte_read(i2c_addr_t devaddr, E24C_addr_t addr);
void 	E24C_block_read(i2c_addr_t devaddr, E24C_addr_t addr, uint8_t *buffer, uint16_t length);


#endif /*__E24CXXX_H_*/

