/* rsa_basic.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2011 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "bigint.h"
#include "rsa_basic.h"

#include "cli.h"

void rsa_enc(bigint_t* data, rsa_publickey_t* key){
	bigint_expmod_u(data, data, key->exponent, key->modulus);
}

void rsa_dec(bigint_t* data, rsa_privatekey_t* key){
	bigint_expmod_u(data, data, key->exponent, key->modulus);
}

void rsa_os2ip(bigint_t* dest, const void* data, uint32_t length_B){
	uint8_t off;
	off = length_B % sizeof(bigint_word_t);
	if(!data){
		if(off){
			dest->wordv = realloc(dest->wordv, length_B + sizeof(bigint_word_t) - off);
			memmove((uint8_t*)dest->wordv+off, dest->wordv, length_B);
			memset(dest->wordv, 0, off);
		}
	}else{
		if(off){
			memcpy((uint8_t*)dest->wordv + off, data, length_B);
			memset(dest, 0, off);
		}else{
			memcpy(dest->wordv, data, length_B);
		}
	}
	dest->length_B = (length_B + sizeof(bigint_word_t) - 1) / sizeof(bigint_word_t);
	bigint_changeendianess(dest);
	bigint_adjust(dest);
}

void rsa_i2osp(void* dest, bigint_t* src, uint16_t* out_length_B){
	*out_length_B = bigint_get_first_set_bit(src) / 8 + 1;
	if(dest){
		uint16_t i;
		for(i=*out_length_B; i>0; --i){
			*((uint8_t*)dest) = ((uint8_t*)src->wordv)[i-1];
			dest = (uint8_t*)dest + 1;
		}
	}else{
		uint8_t off;
		bigint_changeendianess(src);
		bigint_adjust(src);

		off = bigint_get_last_set_bit(src)/8;
		if(off){
			memmove(src->wordv, (uint8_t*)src->wordv+off, *out_length_B);
		}
	}
}

