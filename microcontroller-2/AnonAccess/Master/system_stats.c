/* system_stats.c */
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




#include "config.h"
#include "sha256.h"
#include <stdint.h>
#include <avr/pgmspace.h>

#ifndef FLASHSIZE
 #ifdef ATMEGA644
  #define FLASHSIZE (64L*1024)
 #endif
 #ifdef ATMEGA64
   #define FLASHSIZE (64L*1024)
 #endif
 #ifdef ATMEGA32
   #define FLASHSIZE (32*1024)
 #endif
#endif

#ifndef FLASHSIZE
 #error "FLASHSIZE not specifiyed"
#endif

void system_hash(sha256_hash_t syshash){
	sha256_ctx_t ctx;
	uint8_t buffer[SHA256_BLOCK_BYTES];
	uint16_t i;
	uint8_t j, endsize;
	sha256_init(&ctx);
	for(i=0; i<(FLASHSIZE/SHA256_BLOCK_BYTES)-1; ++i){
		for(j=0; j<SHA256_BLOCK_BYTES; ++j){
			buffer[j] = pgm_read_byte(i*SHA256_BLOCK_BYTES+j);
		}
		sha256_nextBlock(&ctx, buffer);
	}
	endsize=(FLASHSIZE%SHA256_BLOCK_BYTES);
	if(endsize==0)
		endsize=SHA256_BLOCK_BYTES;
	for(j=0; j<endsize; ++j){
		buffer[j] = pgm_read_byte(i*SHA256_BLOCK_BYTES+j);
	}
	sha256_lastBlock(&ctx, buffer, endsize*8);
	sha256_ctx2hash((sha256_hash_t*)syshash, &ctx);
}


