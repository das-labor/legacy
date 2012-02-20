
/* rsa_oaep.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2012 Daniel Otte (daniel.otte@rub.de)

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
#include "memxor.h"
#include "mgf1.h"
#include "bigint.h"
#include "rsa_basic.h"
#include "rsa_oaep.h"

#include "random_dummy.h"

#include "hfal/hfal_sha1.h"

mgf1_parameter_t mgf1_default_parameter = {
		&sha1_desc
};

rsa_oaep_parameter_t rsa_oaep_default_parameter = {
		mgf1,
		&sha1_desc,
		&mgf1_default_parameter
};

rsa_label_t rsa_oaep_default_label = {
		0, NULL
};

uint8_t rsa_encrypt_oaep(void* dest, uint16_t* out_length,
		              const void* src, uint16_t length_B,
		              rsa_publickey_t* key, const rsa_oaep_parameter_t *p,
		              const rsa_label_t* label, const void* seed){

	if(!p){
		p = &rsa_oaep_default_parameter;
	}
	if(!label){
		label = &rsa_oaep_default_label;
	}
	uint16_t hv_len = (hfal_hash_getHashsize(p->hf)+7)/8;
	if(length_B > bigint_length_B(key->modulus) - 2*hv_len - 2){
		/* message too long */
		return 1;
	}
	uint16_t buffer_len = bigint_length_B(key->modulus);
	uint8_t* buffer = (uint8_t*)dest;
	uint8_t off;
	/* the following needs some explanation:
	 * off is the offset which is used for compensating the effect of
	 * changeendian() when it operates on multi-byte words.
	 * */
	off = (sizeof(bigint_word_t) -(bigint_get_first_set_bit(key->modulus)/8+1)%(sizeof(bigint_word_t)*8))
			% (sizeof(bigint_word_t));
	buffer += off;
    buffer_len -= off;
	uint8_t* seed_buffer = buffer + 1;
	uint16_t db_len = buffer_len - hv_len - 1;
	uint8_t* db = seed_buffer + hv_len;
	uint16_t maskbuffer_len = db_len>hv_len?db_len:hv_len;
	uint8_t maskbuffer[maskbuffer_len];
	bigint_t x;

	memset(buffer, 0, seed_buffer - buffer);
	memset(db + hv_len, 0, db_len - hv_len - length_B -1);
	hfal_hash_mem(p->hf, db, label->label, label->length_b);
	db[db_len - length_B - 1] = 0x01;
	memcpy(db+db_len - length_B, src, length_B);
	if(seed){
		memcpy(seed_buffer, seed, hv_len);
	}else{
		/* generate random seed */
		if(!prng_get_byte){
			return 2; /* ERROR: no random generator specified */
		}
		uint16_t i;
		for(i=0; i<hv_len; ++i){
			seed_buffer[i] = prng_get_byte();
		}
	}

	p->mgf(maskbuffer, seed_buffer, hv_len, db_len, p->mgf_parameter);
	memxor(db, maskbuffer, db_len);
	p->mgf(maskbuffer, db, db_len, hv_len, p->mgf_parameter);
	memxor(seed_buffer, maskbuffer, hv_len);

	x.wordv = dest;
	x.length_B = key->modulus->length_B;
	bigint_adjust(&x);

	rsa_os2ip(&x, NULL, bigint_length_B(key->modulus));
	rsa_enc(&x, key);
	rsa_i2osp(NULL, &x, out_length);
	return 0;
}

uint8_t rsa_decrypt_oaep(void* dest, uint16_t* out_length,
		              const void* src, uint16_t length_B,
		              rsa_privatekey_t* key, const rsa_oaep_parameter_t *p,
		              const rsa_label_t* label, void* seed){

	if(!label){
		label = &rsa_oaep_default_label;
	}
	if(!p){
		p = &rsa_oaep_default_parameter;
	}
	uint8_t *buffer =  dest;
	uint16_t x_len, data_len;
	bigint_t x;
	uint16_t hv_len = hfal_hash_getHashsize(p->hf)/8;
	uint8_t label_hv[hv_len];
	uint16_t msg_len = (bigint_get_first_set_bit(key->modulus)+7)/8;
	uint16_t db_len = msg_len - 1 - hv_len;
	uint8_t maskbuffer[db_len>hv_len?db_len:hv_len];

	uint8_t *seed_buffer = buffer + 1;
	uint8_t *db_buffer = seed_buffer + hv_len;

	x_len = bigint_length_B(key->modulus);
	memset(dest, 0, x_len - length_B);
	buffer = (uint8_t*)dest + x_len - length_B;
	memcpy(buffer, src, length_B);

	x.wordv = dest;
	x.length_B = key->modulus->length_B;
	bigint_adjust(&x);

	rsa_os2ip(&x, NULL, bigint_length_B(key->modulus));
	rsa_dec(&x, key);
	rsa_i2osp(NULL, &x, &data_len);
/*
	if(data_len != x_len){
		memmove(buffer + x_len - data_len, buffer, data_len);
		memset(buffer, 0, x_len - data_len);
	}
*/
	if(data_len > msg_len){
		return 7;
	}

	memmove(buffer + msg_len - data_len, buffer, data_len);

	hfal_hash_mem(p->hf, label_hv, label->label, label->length_b);
/*
	if(buffer[0] != 0){
		return 1;
	}
*/
	p->mgf(maskbuffer, db_buffer, db_len, hv_len, p->mgf_parameter);
	memxor(seed_buffer, maskbuffer, hv_len);
	p->mgf(maskbuffer, seed_buffer, hv_len, db_len, p->mgf_parameter);
	memxor(db_buffer, maskbuffer, db_len);

	if(memcmp(label_hv, db_buffer, hv_len)){
		return 2;
	}

	uint16_t ps_len=0;
	while(db_buffer[hv_len + ps_len++] == 0)
		;

	--ps_len;
	if(db_buffer[hv_len + ps_len] != 1){
		return 3;
	}

	if(seed){
		memcpy(seed, seed_buffer, hv_len);
	}

	msg_len = db_len - hv_len - 1 - ps_len;
	memmove(dest, db_buffer + hv_len + ps_len + 1, msg_len);

	*out_length = msg_len;

	return 0;
}


