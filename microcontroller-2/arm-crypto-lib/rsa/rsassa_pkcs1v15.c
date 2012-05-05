/* rsassa_pkcs1v15.c */
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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "rsa_basic.h"
#include "bigint.h"
#include "rsassa_pkcs1v15.h"

#include "cli.h"

const uint8_t md5_prefix[] =
{ 0x30, 0x20, 0x30, 0x0c, 0x06, 0x08, 0x2a, 0x86,
  0x48, 0x86, 0xf7, 0x0d, 0x02, 0x05, 0x05, 0x00,
  0x04, 0x10
};

const pkcs1v15_algo_prefix_t pkcs1v15_md5_prefix = {
	18, md5_prefix
};

const uint8_t sha1_prefix[] =
{ 0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e,
  0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14
};

const pkcs1v15_algo_prefix_t pkcs1v15_sha1_prefix = {
	15, sha1_prefix
};

const uint8_t sha224_prefix[] =
{ 0x30, 0x2d, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
  0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x04, 0x05,
  0x00, 0x04, 0x1c
};

const pkcs1v15_algo_prefix_t pkcs1v15_sha224_prefix = {
	19, sha224_prefix
};

const uint8_t sha256_prefix[] =
{ 0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
  0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05,
  0x00, 0x04, 0x20
};

const pkcs1v15_algo_prefix_t pkcs1v15_sha256_prefix = {
	19, sha256_prefix
};

const uint8_t sha384_prefix[] =
{ 0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
  0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02, 0x05,
  0x00, 0x04, 0x30
};

const pkcs1v15_algo_prefix_t pkcs1v15_sha384_prefix = {
	19, sha384_prefix
};

const uint8_t sha512_prefix[] =
{ 0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86,
  0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03, 0x05,
  0x00, 0x04, 0x40
};

const pkcs1v15_algo_prefix_t pkcs1v15_sha512_prefix = {
	19, sha512_prefix
};
/*

MD2:     (0x)30 20 30 0c 06 08 2a 86 48 86 f7 0d 02 02 05 00 04 10
MD5:     (0x)30 20 30 0c 06 08 2a 86 48 86 f7 0d 02 05 05 00 04 10
SHA-1:   (0x)30 21 30 09 06 05 2b 0e 03 02 1a 05 00 04 14
SHA-224: (0x)30 2d 30 0d 06 09 60 86 48 01 65 03 04 02 04 05 00 04 1c
SHA-256: (0x)30 51 30 0d 06 09 60 86 48 01 65 03 04 02 01 05 00 04 20
SHA-384: (0x)30 51 30 0d 06 09 60 86 48 01 65 03 04 02 02 05 00 04 30
SHA-512: (0x)30 51 30 0d 06 09 60 86 48 01 65 03 04 02 03 05 00 04 40

*/

static
uint8_t emsa_pkcs1v15_encode(void* dest, uint16_t dest_length_B, const void* hash,
		uint16_t hash_length_B, const pkcs1v15_algo_prefix_t* algo_prefix){
	uint16_t ps_length;

	if(dest_length_B < algo_prefix->length + hash_length_B + 3 + 8){
		return 1;
	}
	ps_length = dest_length_B - (algo_prefix->length + hash_length_B + 3);
	((uint8_t*)dest)[0] = 0x00;
	((uint8_t*)dest)[1] = 0x01;
	((uint8_t*)dest)[2 + ps_length] = 0x00;
	memset((uint8_t*)dest + 2, 0xff, ps_length);
	memcpy((uint8_t*)dest + 3 + ps_length, algo_prefix->data, algo_prefix->length);
	memcpy((uint8_t*)dest + 3 + ps_length + algo_prefix->length, hash, hash_length_B);
	return 0;
}

uint8_t rsa_sign_pkcs1v15(void* dest, uint16_t* out_length_B, const void* hash,
		uint16_t hash_length_B, const rsa_privatekey_t* key,
		const pkcs1v15_algo_prefix_t* algo_prefix){
	uint8_t r;
	uint16_t modulus_length;
	bigint_t x;
	modulus_length = bigint_get_first_set_bit(&key->modulus) / 8 + 1;
	r = emsa_pkcs1v15_encode(dest, modulus_length, hash, hash_length_B, algo_prefix);
	if(r){
		return r;
	}
	x.wordv = dest;
	rsa_os2ip(&x, NULL, modulus_length);
	rsa_dec(&x, key);
	rsa_i2osp(NULL, &x, out_length_B);

	return 0;
}


uint8_t rsa_verify_pkcs1v15(const void* signature, uint16_t signature_length_B,
		const void* hash, uint16_t hash_length_B, const rsa_publickey_t* key,
		const pkcs1v15_algo_prefix_t* algo_prefix){
	uint16_t modulus_length;
	uint16_t signature_em_length, ps_length;
	bigint_t x;

	modulus_length = bigint_get_first_set_bit(&key->modulus) / 8 + 1;
#if PREFERE_HEAP
	uint8_t *buffer;
	buffer = malloc(bigint_length_B(&key->modulus));
	if(!buffer){
		return 0x80;
	}
#else
	uint8_t buffer[bigint_length_B(&key->modulus)];
#endif
/*
	cli_putstr("\r\nDBG: signature_length_B: 0x");
	cli_hexdump_rev(&signature_length_B, 2);
	cli_putstr("\r\nDBG: modulus_length_B: 0x");
	cli_hexdump_rev(&modulus_length, 2);
*/
	x.wordv = (bigint_word_t*)buffer;
	rsa_os2ip(&x, signature, signature_length_B);
	rsa_enc(&x, key);
	rsa_i2osp(NULL, &x, &signature_em_length);
/*
	cli_putstr("\r\nDBG: signature_em_length_B: 0x");
	cli_hexdump_rev(&signature_em_length, 2);
*/
	if(signature_em_length + 1 != modulus_length){
		return 1;
	}
	if(memcmp(buffer + modulus_length - hash_length_B - 1, hash, hash_length_B)){
		return 2;
	}
	ps_length = modulus_length - (algo_prefix->length + hash_length_B + 3);
	if((int16_t)ps_length < 8){
		return 3;
	}
	if(memcmp(buffer + 2 + ps_length, algo_prefix->data, algo_prefix->length)){
		return 4;
	}
	if(buffer[0] != 1){
		return 6;
	}
	if(buffer[1 + ps_length] != 0){
		return 7;
	}
	do{
		if(buffer[ps_length] != 0xff){
			return 8;
		}
	}while(--ps_length);
#if PREFERE_HEAP
	free(buffer);
#endif

	return 0;
}
