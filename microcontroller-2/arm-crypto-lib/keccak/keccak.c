/* keecak.c */
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
#include <stdlib.h>
#include <string.h>
#include "memxor.h"
#include "keccak.h"

#ifdef DEBUG
#  undef DEBUG
#endif

#define DEBUG 0

#if DEBUG
#include "cli.h"

void keccak_dump_state(uint64_t a[5][5]){
	uint8_t i,j;
	for(i=0; i<5; ++i){
		cli_putstr("\r\n");
		cli_putc('0'+i);
		cli_putstr(": ");
		for(j=0; j<5; ++j){
			cli_hexdump_rev(&(a[i][j]), 8);
			cli_putc(' ');
		}
	}
}

void keccak_dump_ctx(keccak_ctx_t* ctx){
	keccak_dump_state(ctx->a);
	cli_putstr("\r\nDBG: r: ");
	cli_hexdump_rev(&(ctx->r), 2);
	cli_putstr("\t c: ");
	cli_hexdump_rev(&(ctx->c), 2);
	cli_putstr("\t d: ");
	cli_hexdump(&(ctx->d), 1);
	cli_putstr("\t bs: ");
	cli_hexdump(&(ctx->bs), 1);
}

#endif

static const uint64_t rc[] = {
       0x0000000000000001LL, 0x0000000000008082LL,
       0x800000000000808ALL, 0x8000000080008000LL,
       0x000000000000808BLL, 0x0000000080000001LL,
       0x8000000080008081LL, 0x8000000000008009LL,
       0x000000000000008ALL, 0x0000000000000088LL,
       0x0000000080008009LL, 0x000000008000000ALL,
       0x000000008000808BLL, 0x800000000000008BLL,
       0x8000000000008089LL, 0x8000000000008003LL,
       0x8000000000008002LL, 0x8000000000000080LL,
       0x000000000000800ALL, 0x800000008000000ALL,
       0x8000000080008081LL, 0x8000000000008080LL,
       0x0000000080000001LL, 0x8000000080008008LL
};

static inline
uint64_t rotl64(uint64_t a, uint8_t r){
	 return (a<<r)|(a>>(64-r));
}

static const uint8_t r[5][5] = {
		{  0, 36,  3, 41, 18 },
		{  1, 44, 10, 45,  2 },
		{ 62,  6, 43, 15, 61 },
		{ 28, 55, 25, 21, 56 },
		{ 27, 20, 39,  8, 14 }
};

void keccak_round(uint64_t a[5][5], uint8_t rci){
	uint64_t b[5][5];
	uint8_t i,j;
	/* theta */
	for(i=0; i<5; ++i){
		b[i][0] = a[0][i] ^ a[1][i] ^ a[2][i] ^ a[3][i] ^ a[4][i];
	}
	for(i=0; i<5; ++i){
		b[i][1] = b[(4+i)%5][0] ^ rotl64(b[(i+1)%5][0], 1);
		for(j=0; j<5; ++j){
			a[j][i] ^= b[i][1];
		}
	}
#if DEBUG
	cli_putstr("\r\nAfter theta:");
	keccak_dump_state(a);
#endif
	/* rho & pi */
	for(i=0; i<5; ++i){
		for(j=0; j<5; ++j){
			b[(2*i+3*j)%5][j] = rotl64(a[j][i], r[i][j]);
		}
	}
#if DEBUG
	cli_putstr("\r\n--- after rho & pi ---");
	keccak_dump_state(a);
#endif
	/* chi */
	for(i=0; i<5; ++i){
		for(j=0; j<5; ++j){
			a[j][i] =  b[j][i] ^ ((~(b[j][(i+1)%5]))&(b[j][(i+2)%5]));
		}
	}
#if DEBUG
	cli_putstr("\r\nAfter chi:");
	keccak_dump_state(a);
#endif
	/* iota */
	uint64_t t;
	t= rc[rci];
	a[0][0] ^= t;
#if DEBUG
	cli_putstr("\r\nAfter iota:");
	keccak_dump_state(a);
#endif
}

void keccak_f1600(uint64_t a[5][5]){
	uint8_t i=0;
	do{
#if DEBUG
		cli_putstr("\r\n\r\n--- Round ");
		cli_hexdump(&i, 1);
		cli_putstr(" ---");
#endif
		keccak_round(a, i);
	}while(++i<24);
}

void keccak_nextBlock(keccak_ctx_t* ctx, const void* block){
	memxor(ctx->a, block, ctx->bs);
	keccak_f1600(ctx->a);
}

void keccak_lastBlock(keccak_ctx_t* ctx, const void* block, uint16_t length_b){
	while(length_b>=ctx->r){
		keccak_nextBlock(ctx, block);
		block = (uint8_t*)block + ctx->bs;
		length_b -=  ctx->r;
	}
	uint8_t tmp[ctx->bs];
	uint8_t pad[3];
	memset(tmp, 0x00, ctx->bs);
	memcpy(tmp, block, (length_b+7)/8);
	/* appand 1 */
	if(length_b&7){
		/* we have some single bits */
		uint8_t t;
		t = tmp[length_b/8]>>(8-(length_b&7));
		t |= 0x01<<(length_b&7);
		tmp[length_b/8] = t;
	}else{
		tmp[length_b/8] = 0x01;
	}
	pad[0] = ctx->d;
	pad[1] = ctx->bs;
	pad[2] = 0x01;
	if(length_b/8+1+3<=ctx->bs){
		memcpy(tmp+length_b/8+1, pad, 3);
	}else{
		if(length_b/8+1+2<=ctx->bs){
			memcpy(tmp+length_b/8+1, pad, 2);
			keccak_nextBlock(ctx, tmp);
			memset(tmp, 0x00, ctx->bs);
			tmp[0]=0x01;
		}else{
			if(length_b/8+1+1<=ctx->bs){
				memcpy(tmp+length_b/8+1, pad, 1);
				keccak_nextBlock(ctx, tmp);
				memset(tmp, 0x00, ctx->bs);
				tmp[0] = ctx->bs;
				tmp[1] = 0x01;
			}else{
				keccak_nextBlock(ctx, tmp);
				memset(tmp, 0x00, ctx->bs);
				tmp[0] = ctx->d;
				tmp[1] = ctx->bs;
				tmp[2] = 0x01;
			}
		}
	}
	keccak_nextBlock(ctx, tmp);
}

void keccak_ctx2hash(void* dest, uint16_t length_b, keccak_ctx_t* ctx){
	while(length_b>=ctx->r){
		memcpy(dest, ctx->a, ctx->bs);
		dest = (uint8_t*)dest + ctx->bs;
		length_b -= ctx->r;
		keccak_f1600(ctx->a);
	}
	memcpy(dest, ctx->a, (length_b+7)/8);
}

void keccak224_ctx2hash(void* dest, keccak_ctx_t* ctx){
	keccak_ctx2hash(dest, 224, ctx);
}

void keccak256_ctx2hash(void* dest, keccak_ctx_t* ctx){
	keccak_ctx2hash(dest, 256, ctx);
}

void keccak384_ctx2hash(void* dest, keccak_ctx_t* ctx){
	keccak_ctx2hash(dest, 384, ctx);
}

void keccak512_ctx2hash(void* dest, keccak_ctx_t* ctx){
	keccak_ctx2hash(dest, 512, ctx);
}

/*
1. SHA3-224: ⌊Keccak[r = 1152, c = 448, d = 28]⌋224
2. SHA3-256: ⌊Keccak[r = 1088, c = 512, d = 32]⌋256
3. SHA3-384: ⌊Keccak[r = 832, c = 768, d = 48]⌋384
4. SHA3-512: ⌊Keccak[r = 576, c = 1024, d = 64]⌋512
*/
void keccak_init(uint16_t r, uint16_t c, uint8_t d, keccak_ctx_t* ctx){
	memset(ctx->a, 0x00, 5*5*8);
	ctx->r = r;
	ctx->c = c;
	ctx->d = d;
	ctx->bs = (uint8_t)(r/8);
}

void keccak224_init(keccak_ctx_t* ctx){
	keccak_init(1152, 448, 28, ctx);
}

void keccak256_init(keccak_ctx_t* ctx){
	keccak_init(1088, 512, 32, ctx);
}

void keccak384_init(keccak_ctx_t* ctx){
	keccak_init( 832, 768, 48, ctx);
}

void keccak512_init(keccak_ctx_t* ctx){
	keccak_init( 576, 1024, 64, ctx);
}
