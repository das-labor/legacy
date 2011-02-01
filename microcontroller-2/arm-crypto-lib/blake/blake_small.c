/* blake_small.c */
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
/*
 * \file    blake_small.c
 * \author  Daniel Otte
 * \email   daniel.otte@rub.de
 * \date    2009-05-04
 * \license GPLv3 or later
 *
 */

#include <stdint.h>
#include <string.h>
#include "memxor.h"
#include "blake_small.h"
#include "blake_common.h"

static const
uint32_t blake_c[] = {
   0x243F6A88, 0x85A308D3,
   0x13198A2E, 0x03707344,
   0xA4093822, 0x299F31D0,
   0x082EFA98, 0xEC4E6C89,
   0x452821E6, 0x38D01377,
   0xBE5466CF, 0x34E90C6C,
   0xC0AC29B7, 0xC97C50DD,
   0x3F84D5B5, 0xB5470917
};

#define ROTL32(a, n) (((a)<<(n))|((a)>>(32-(n))))
#define ROTR32(a, n) (((a)>>(n))|((a)<<(32-(n))))
#define CHANGE_ENDIAN32(a) (((a)<<24)| \
                            ((0x0000ff00&(a))<<8)| \
						    ((0x00ff0000&(a))>>8)| \
						    (a)>>24 )
static
void blake_small_expand(uint32_t* v, const blake_small_ctx_t* ctx){
	uint8_t i;
	memcpy(v, ctx->h, 8*4);
	for(i=0; i<8; ++i){
		v[8+i] = blake_c[i];
	}
	memxor((uint8_t*)v+8, ctx->s, 4*4);

}

static
void blake_small_changeendian(void* dest, const void* src){
	uint8_t i;
	for(i=0; i<16; ++i){
		((uint32_t*)dest)[i] = CHANGE_ENDIAN32(((uint32_t*)src)[i]);
	}
}

#define A (v[idx.v8[0]])
#define B (v[idx.v8[1]])
#define C (v[idx.v8[2]])
#define D (v[idx.v8[3]])

static
void blake_small_compress(uint32_t* v,const void* m){
	uint8_t r,i;
	uint16_t s, *p=(uint16_t*)blake_sigma;
	union {
		uint32_t v32;
		uint8_t v8[4];
	} idx;
	for(r=0; r<14; ++r){
		for(i=0; i<8; ++i){
			idx.v32 = ((uint32_t*)blake_index_lut)[i];
			s = *p++;
			if(p==(uint16_t*)(blake_sigma+160)){
				p=(uint16_t*)blake_sigma;
			}
			A += B + (((uint32_t*)m)[s&0xff] ^ blake_c[s>>8]);
			D  = ROTR32(A^D, 16);
			C += D;
			B  = ROTR32(B^C, 12);
			A += B + (((uint32_t*)m)[s>>8] ^ blake_c[s&0xff]);
			D  = ROTR32(A^D, 8);
			C += D;
			B  = ROTR32(B^C, 7);
		}
	}
}

static
void blake_small_collapse(blake_small_ctx_t* ctx, uint32_t* v){
	uint8_t i;
	for(i=0; i<8; ++i){
		ctx->h[i] ^= ctx->s[i%4] ^ v[i] ^ v[8+i];
	}
}

void blake_small_nextBlock(blake_small_ctx_t* ctx, const void* msg){
	uint32_t v[16];
	uint32_t m[16];
	union {
		uint64_t v64;
		uint32_t v32[2];
	}ctr;
	blake_small_expand(v,ctx);
	ctx->counter++;
	ctr.v64 = ctx->counter*512;
	v[12] ^= ctr.v32[0];
	v[13] ^= ctr.v32[0];
	v[14] ^= ctr.v32[1];
	v[15] ^= ctr.v32[1];
	blake_small_changeendian(m, msg);
	blake_small_compress(v, m);
	blake_small_collapse(ctx, v);
}

void blake_small_lastBlock(blake_small_ctx_t* ctx, const void* msg, uint16_t length_b){
	while(length_b>=BLAKE_SMALL_BLOCKSIZE){
		blake_small_nextBlock(ctx, msg);
		msg = (uint8_t*)msg + BLAKE_SMALL_BLOCKSIZE_B;
		length_b -= BLAKE_SMALL_BLOCKSIZE;
	}
	uint8_t buffer[64];
	uint32_t v[16];
	union {
		uint64_t v64;
		uint32_t v32[2];
	}ctr;
	ctr.v64 = ctx->counter*512+length_b;
	memset(buffer, 0, 64);
	memcpy(buffer, msg, (length_b+7)/8);
	buffer[length_b/8] |= 0x80 >> (length_b&0x7);
	blake_small_changeendian(buffer, buffer);
	blake_small_expand(v, ctx);
	if(length_b>512-64-2){
		v[12] ^= ctr.v32[0];
		v[13] ^= ctr.v32[0];
		v[14] ^= ctr.v32[1];
		v[15] ^= ctr.v32[1];
		blake_small_compress(v, buffer);
		blake_small_collapse(ctx, v);
		memset(buffer, 0, 64-8);
		blake_small_expand(v, ctx);
	}else{
		if(length_b){
			v[12] ^= ctr.v32[0];
			v[13] ^= ctr.v32[0];
			v[14] ^= ctr.v32[1];
			v[15] ^= ctr.v32[1];
		}
	}
	if(ctx->appendone)
		buffer[64-8-4] |= 0x01;
	*((uint32_t*)(&(buffer[64-8]))) = ctr.v32[1];
	*((uint32_t*)(&(buffer[64-4]))) = ctr.v32[0];
	blake_small_compress(v, buffer);
	blake_small_collapse(ctx, v);

}

static const
uint32_t blake256_iv[] = {
	0x6A09E667L, 0xBB67AE85,
	0x3C6EF372L, 0xA54FF53A,
	0x510E527FL, 0x9B05688C,
	0x1F83D9ABL, 0x5BE0CD19
};

void blake256_init(blake256_ctx_t* ctx){
	uint8_t i;
	for(i=0; i<8; ++i){
		ctx->h[i] = blake256_iv[i];
	}
	memset(ctx->s, 0, 4*4);
	ctx->counter = 0;
	ctx->appendone = 1;
}

static const
uint32_t blake224_iv[] = {
	0xC1059ED8, 0x367CD507,
	0x3070DD17, 0xF70E5939,
	0xFFC00B31, 0x68581511,
	0x64F98FA7, 0xBEFA4FA4
};

void blake224_init(blake224_ctx_t* ctx){
	uint8_t i;
	for(i=0; i<8; ++i){
		ctx->h[i] = blake224_iv[i];
	}
	memset(ctx->s, 0, 4*4);
	ctx->counter = 0;
	ctx->appendone = 0;
}

void blake256_ctx2hash(void* dest, const blake256_ctx_t* ctx){
	uint8_t i;
	for(i=0; i<8; ++i){
		((uint32_t*)dest)[i] = CHANGE_ENDIAN32(ctx->h[i]);
	}
}

void blake224_ctx2hash(void* dest, const blake224_ctx_t* ctx){
	uint8_t i;
	for(i=0; i<7; ++i){
		((uint32_t*)dest)[i] = CHANGE_ENDIAN32(ctx->h[i]);
	}
}

void blake256_nextBlock(blake256_ctx_t* ctx, const void* block){
	blake_small_nextBlock(ctx, block);
}

void blake224_nextBlock(blake224_ctx_t* ctx, const void* block){
	blake_small_nextBlock(ctx, block);
}

void blake256_lastBlock(blake256_ctx_t* ctx, const void* block, uint16_t length_b){
	blake_small_lastBlock(ctx, block, length_b);
}

void blake224_lastBlock(blake224_ctx_t* ctx, const void* block, uint16_t length_b){
	blake_small_lastBlock(ctx, block, length_b);
}

void blake256(void* dest, const void* msg, uint32_t length_b){
	blake_small_ctx_t ctx;
	blake256_init(&ctx);
	while(length_b>=BLAKE_SMALL_BLOCKSIZE){
		blake_small_nextBlock(&ctx, msg);
		msg = (uint8_t*)msg + BLAKE_SMALL_BLOCKSIZE_B;
		length_b -= BLAKE_SMALL_BLOCKSIZE;
	}
	blake_small_lastBlock(&ctx, msg, length_b);
	blake256_ctx2hash(dest, &ctx);
}

void blake224(void* dest, const void* msg, uint32_t length_b){
	blake_small_ctx_t ctx;
	blake224_init(&ctx);
	while(length_b>=BLAKE_SMALL_BLOCKSIZE){
		blake_small_nextBlock(&ctx, msg);
		msg = (uint8_t*)msg + BLAKE_SMALL_BLOCKSIZE_B;
		length_b -= BLAKE_SMALL_BLOCKSIZE;
	}
	blake_small_lastBlock(&ctx, msg, length_b);
	blake224_ctx2hash(dest, &ctx);
}
