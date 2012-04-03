/* rsa_pkcs15.c */
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

#define DEBUG 0

#if DEBUG
#include "bigint_io.h"
#include "cli.h"
#endif

#include "random_dummy.h"

uint8_t rsa_encrypt_pkcs15(void* dest, uint16_t* out_length, const void* src,
	uint16_t length_B, rsa_publickey_t* key, const void* pad){
	int16_t pad_length;
	bigint_t x;
	pad_length = (bigint_get_first_set_bit(key->modulus) + 7) / 8 - length_B - 3;
	if(pad_length<8){
#if DEBUG
		cli_putstr("\r\nERROR: pad_length<8; pad_length: ");
		cli_hexdump_rev(&pad_length, 2);
#endif
		return 2; /* message to long */
	}
	if(!pad){
		uint16_t i;
		uint8_t c;
		for(i=0; i<pad_length; ++i){
			do{
				c = prng_get_byte();
			}while(c==0);
			((uint8_t*)dest)[i+2] = c;
		}
	}else{
		memcpy((uint8_t*)dest + 2, pad, pad_length);
	}
	((uint8_t*)dest)[0] = 0x00;
	((uint8_t*)dest)[1] = 0x02;
	((uint8_t*)dest)[2+pad_length] = 0x00;
	memcpy((uint8_t*)dest+3+pad_length, src, length_B);
	x.wordv = dest;
	x.length_B = (length_B+pad_length+3+sizeof(bigint_word_t)-1)/sizeof(bigint_word_t);
	bigint_adjust(&x);
	rsa_os2ip(&x, NULL, length_B+pad_length+3);
	rsa_enc(&x, key);
	rsa_i2osp(NULL, &x, out_length);
	return 0;
}

uint8_t rsa_decrypt_pkcs15(void* dest, uint16_t* out_length, const void* src,
	uint16_t length_B, rsa_privatekey_t* key, void* pad){
	bigint_t x;
	uint16_t m_length, pad_length=0, idx=0;
	x.wordv = dest;
	rsa_os2ip(&x, src, length_B);
#if DEBUG
	cli_putstr("\r\ncalling rsa_dec() ...");
#endif
	rsa_dec(&x, key);
#if DEBUG
	cli_putstr("\r\nfinished rsa_dec() ...");
#endif
	rsa_i2osp(NULL, &x, &m_length);
	while(((uint8_t*)x.wordv)[idx]==0 && idx<m_length){
		++idx;
	}
	if(((uint8_t*)x.wordv)[idx]!=2 || idx>=m_length){
		return 1;
	}
	++idx;
	while(((uint8_t*)x.wordv)[idx+pad_length]!=0  && (idx+pad_length)<m_length){
		++pad_length;
	}
	if(pad_length<8 || (idx+pad_length)>=m_length){
		return 2;
	}
	*out_length = m_length - idx - pad_length - 1;
	if(pad){
		memcpy(pad, ((uint8_t*)x.wordv)+idx, pad_length);
	}
	memcpy(dest, ((uint8_t*)x.wordv) + idx + pad_length + 1, m_length - idx - pad_length - 1);
	return 0;
}

