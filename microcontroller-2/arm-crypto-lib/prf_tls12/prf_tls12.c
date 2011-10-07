/* prf_tls12.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

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
/*
 * \file    prf_tls12.c
 * \author  Daniel Otte
 * \email   daniel.otte@rub.de
 * \date    2011-10-06
 * \license GPLv3 or later
 *
 */
/* from rfc5246


      P_hash(secret, seed) = HMAC_hash(secret, A(1) + seed) +
                             HMAC_hash(secret, A(2) + seed) +
                             HMAC_hash(secret, A(3) + seed) + ...

   where + indicates concatenation.

   A() is defined as:

      A(0) = seed
      A(i) = HMAC_hash(secret, A(i-1))

   P_hash can be iterated as many times as necessary to produce the
   required quantity of data.  For example, if P_SHA256 is being used to
   create 80 bytes of data, it will have to be iterated three times
   (through A(3)), creating 96 bytes of output data; the last 16 bytes
   of the final iteration will then be discarded, leaving 80 bytes of
   output data.

   TLS's PRF is created by applying P_hash to the secret as:

      PRF(secret, label, seed) = P_<hash>(secret, label + seed)

*/
/* long story short:
 * P(k,s) = H(k, A(1) | s) | H(k, A(2) | s) | ... | H(k, A(n) | s)
 * A(0) = s
 * A(i) = H(k, A(i-1))
 *
 * PRF(k,l,s) = P(k, l | s)
 *
 */

/* This implementation is limited to hashfunctions which return a hash value
 * of a length (in bits) which is divideable by 8.
 *
 * Also note that our HMAC implementation may fail on hashfunction which
 * return a larger hash value then their nativ blocksize
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <crypto/hashfunction_descriptor.h>
#include <crypto/hfal-basic.h>
#include <crypto/hfal-hmac.h>
#include "prf_tls12.h"

uint8_t prf_tls12_init(prf_tls12_ctx_t* ctx, const hfdesc_t* hash,
	const void* key, uint16_t keylength_b,
	const void* seed, uint16_t seedlength_b){
	hfhmacgen_ctx_t tmp_ctx;
	ctx->blocklength_b = hfal_hash_getHashsize(hash);
	ctx->seed_buffer = malloc(ctx->blocklength_b/8+(seedlength_b+7)/8);
	if(!ctx->seed_buffer){
		return 1;
	}
	ctx->bufferlength_b = ctx->blocklength_b + seedlength_b;
	memcpy(ctx->seed_buffer+ctx->blocklength_b/8, seed, seedlength_b/8);
	if(hfal_hmac_init(hash, &(ctx->mainctx), key, keylength_b)){
		return 2;
	}
	if(hfal_hmac_ctxcopy(&tmp_ctx, &(ctx->mainctx))){
		prf_tls12_free(ctx);
		return 3;
	}
	hfal_hmac_lastBlock(&tmp_ctx, seed, seedlength_b);
	hfal_hmac_ctx2mac(ctx->seed_buffer, &tmp_ctx);
	hfal_hmac_free(&tmp_ctx);
	return 0;
}

uint8_t prf_tls12_init_w_label(prf_tls12_ctx_t* ctx, const hfdesc_t* hash,
	const void* key, uint16_t keylength_b,
	const void* label, uint16_t labellength_B,
	const void* seed, uint16_t seedlength_b){

	uint8_t buffer[labellength_B+(seedlength_b+7)/8];
	memcpy(buffer, label, labellength_B);
	memcpy(buffer+labellength_B, seed, (seedlength_b+7)/8);
	return prf_tls12_init(ctx, hash, key, keylength_b, buffer, labellength_B*8+seedlength_b);
}


void prf_tls12_free(prf_tls12_ctx_t* ctx){
	free(ctx->seed_buffer);
	hfal_hmac_free(&(ctx->mainctx));
}

uint8_t prf_tls12_next(void* dest, prf_tls12_ctx_t* ctx){
	hfhmacgen_ctx_t tmp_ctx;
	if(hfal_hmac_ctxcopy(&tmp_ctx, &(ctx->mainctx))){
		return 1;
	}
	hfal_hmac_lastBlock(&tmp_ctx, ctx->seed_buffer, ctx->bufferlength_b);
	hfal_hmac_ctx2mac(dest, &tmp_ctx);
	hfal_hmac_free(&tmp_ctx);
	if(hfal_hmac_ctxcopy(&tmp_ctx, &(ctx->mainctx))){
		return 2;
	}
	hfal_hmac_lastBlock(&tmp_ctx, ctx->seed_buffer, ctx->blocklength_b);
	hfal_hmac_ctx2mac(ctx->seed_buffer, &tmp_ctx);
	hfal_hmac_free(&tmp_ctx);
	return 0;
}

uint8_t prf_tls12_fill(void* dest, uint16_t length_B, prf_tls12_ctx_t* ctx){
	uint16_t bs = ctx->blocklength_b/8;
	while(length_B>=bs){
		if(prf_tls12_next(dest, ctx)){
			return 1;
		}
		length_B -= bs;
		dest = (uint8_t*)dest + bs;
	}
	if(length_B){
		uint8_t buffer[bs];
		if(prf_tls12_next(buffer, ctx)){
			return 2;
		}
		memcpy(dest, buffer, length_B);
	}
	return 0;
}
