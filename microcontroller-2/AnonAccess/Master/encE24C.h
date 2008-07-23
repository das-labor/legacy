/* encE24C.h */
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
