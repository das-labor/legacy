/* enc2E24C.h */
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


#ifndef ENC2E24C_H_
#define ENC2E24C_H_
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


#define EEPROM_ENC_NOEKEON

#ifdef EEPROM_ENC_SHABEA
 #ifdef EEPROM_ENC_NOEKEON
 #error "Either EEPROM_ENC_SHABEA or EEPROM_ENC_NOEKEON must be defined"
 #endif
#else
 #ifndef EEPROM_ENC_NOEKEON
 #error "Either EEPROM_ENC_SHABEA or EEPROM_ENC_NOEKEON must be defined"
 #endif
#endif

#ifdef EEPROM_ENC_SHABEA
 #define ENC24C_BLOCKSIZE 32
 #define CRYPTKEY_SIZE 256
#endif

#ifdef EEPROM_ENC_NOEKEON
 #define ENC24C_BLOCKSIZE 16
 #define CRYPTKEY_SIZE 256
#endif



#ifndef _24CBLOCKDEV_H_
 typedef uint32_t blockdev_ptr_t;
#endif 

typedef uint16_t blockaddr_t;
 

void encrypt_E24Cblock(void *dest, blockdev_ptr_t addr, const uint8_t * crypt_key);
void decrypt_E24Cblock(void *dest, blockdev_ptr_t addr, const uint8_t * crypt_key);

void crypto_read_block(void * dest, blockdev_ptr_t addr, uint16_t length, 
                       void * crypt_key);
void crypto_set_block(uint8_t value, blockdev_ptr_t addr, uint32_t length, 
                      void * crypt_key);
void crypto_write_block(void *dest, blockdev_ptr_t addr, uint16_t length, 
                        void * crypt_key);
                     
#endif /*ENC2E24C_H_*/

