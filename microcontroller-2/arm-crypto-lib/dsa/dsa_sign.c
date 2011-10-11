/* dsa_sign.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

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
#include <string.h>
#include "bigint.h"
#include "dsa.h"
#include "hashfunction_descriptor.h"
#include "hfal-basic.h"

#define MAX(a,b) (((a)>(b))?(a):(b))

uint8_t dsa_sign_bigint(dsa_signature_t* s, const bigint_t* m,
		                const dsa_ctx_t* ctx, const bigint_t* k){
	bigint_t tmp, tmp2;
	bigint_word_t tmp_b[ctx->domain.p.length_B+5], tmp2_b[ctx->domain.q.length_B+5];
	tmp.wordv= tmp_b;
	tmp2.wordv = tmp2_b;
	bigint_expmod_u(&tmp, &(ctx->domain.g), k, &(ctx->domain.p));
	bigint_reduce(&tmp, &(ctx->domain.q));
	bigint_copy(&(s->r), &tmp);
	bigint_mul_u(&tmp, &tmp, &(ctx->priv));
	bigint_add_u(&tmp, &tmp, m);
	bigint_inverse(&tmp2, k, &(ctx->domain.q));
	bigint_mul_u(&tmp, &tmp, &tmp2);
	bigint_reduce(&tmp, &(ctx->domain.q));
	bigint_copy(&(s->s), &tmp);

	if(s->s.length_B==0 || s->r.length_B==0){
		return 1;
	}

	return 0;
}

uint8_t dsa_sign_message(dsa_signature_t* s, const void* m, uint16_t m_len_b,
		                const hfdesc_t* hash_desc, const dsa_ctx_t* ctx,
		                uint8_t(*rand_in)(void)){
	bigint_t z, k;
	uint16_t i, n_B = ctx->domain.q.length_B;
	unsigned hash_length = MAX(n_B,(hfal_hash_getHashsize(hash_desc)+sizeof(bigint_word_t)*8-1)/(sizeof(bigint_word_t)*8));
	bigint_word_t hash_value[hash_length];
	bigint_word_t k_b[n_B];
	memset(hash_value, 0, hash_length*sizeof(bigint_word_t));
	hfal_hash_mem(hash_desc, hash_value, m, m_len_b);
	z.wordv = hash_value;
	z.length_B = n_B;
	bigint_changeendianess(&z);
	k.wordv = k_b;
	k.length_B = n_B;
	do{
		for(i=0; i<n_B*sizeof(bigint_word_t); ++i){
			((uint8_t*)k_b)[i] = rand_in();
		}
		k.length_B = n_B;
		bigint_adjust(&k);
	}while(dsa_sign_bigint(s, &z, ctx, &k));
	return 0;
}

