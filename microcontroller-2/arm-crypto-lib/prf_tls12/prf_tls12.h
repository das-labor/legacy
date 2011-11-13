/* prf_tls12.h */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2011 Daniel Otte (daniel.otte@rub.de)

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

#ifndef PRF_TLS12_H_
#define PRF_TLS12_H_

#include <stdint.h>
#include "hashfunction_descriptor.h"
#include "hfal-basic.h"
#include "hfal-hmac.h"

typedef struct{
	hfhmacgen_ctx_t mainctx;
	uint16_t blocklength_b;
	uint16_t bufferlength_b;
	uint8_t* seed_buffer;
} prf_tls12_ctx_t;

uint8_t prf_tls12_init(prf_tls12_ctx_t* ctx, const hfdesc_t* hash,
	const void* key, uint16_t keylength_b,
	const void* seed, uint16_t seedlength_b);

uint8_t prf_tls12_init_w_label(prf_tls12_ctx_t* ctx, const hfdesc_t* hash,
	const void* key, uint16_t keylength_b,
	const void* label, uint16_t labellength_B,
	const void* seed, uint16_t seedlength_b);

void prf_tls12_free(prf_tls12_ctx_t* ctx);
uint8_t prf_tls12_next(void* dest, prf_tls12_ctx_t* ctx);
uint8_t prf_tls12_fill(void* dest, uint16_t length_B, prf_tls12_ctx_t* ctx);


#endif /* PRF_TLS12_H_ */
