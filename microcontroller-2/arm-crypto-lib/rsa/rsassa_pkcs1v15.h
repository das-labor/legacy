/* rsassa_pkcs1v15.h */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2012 Daniel Otte (daniel.otte@rub.de)

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

#ifndef RSASSA_PKCS1V15_H_
#define RSASSA_PKCS1V15_H_

#include <stdint.h>
#include "rsa_basic.h"

typedef struct {
	const uint8_t length;
	const uint8_t *data;
} pkcs1v15_algo_prefix_t;

extern const pkcs1v15_algo_prefix_t pkcs1v15_md5_prefix;
extern const pkcs1v15_algo_prefix_t pkcs1v15_sha1_prefix;
extern const pkcs1v15_algo_prefix_t pkcs1v15_sha224_prefix;
extern const pkcs1v15_algo_prefix_t pkcs1v15_sha256_prefix;
extern const pkcs1v15_algo_prefix_t pkcs1v15_sha384_prefix;
extern const pkcs1v15_algo_prefix_t pkcs1v15_sha512_prefix;

uint8_t rsa_sign_pkcs1v15(void* dest, uint16_t* out_length_B, const void* hash,
		uint16_t hash_length_B, const rsa_privatekey_t* key,
		const pkcs1v15_algo_prefix_t* algo_prefix);

uint8_t rsa_verify_pkcs1v15(const void* signature, uint16_t signature_length_B,
		const void* hash, uint16_t hash_length_B, const rsa_publickey_t* key,
		const pkcs1v15_algo_prefix_t* algo_prefix);

#endif /* RSASSA_PKCS1V15_H_ */
