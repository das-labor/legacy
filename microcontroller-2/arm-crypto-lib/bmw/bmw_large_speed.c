/* bmw_large_speed.c */
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
 * \file    bmw_large.c
 * \author  Daniel Otte
 * \email   daniel.otte@rub.de
 * \date    2009-04-27
 * \license GPLv3 or later
 *
 */

#include <stdint.h>
#include <string.h>
#include "bmw_large.h"

#define SHL64(a,n)  ((a)<<(n))
#define SHR64(a,n)  ((a)>>(n))
#define ROTL64(a,n) (((a)<<(n))|((a)>>(64-(n))))
#define ROTR64(a,n) (((a)>>(n))|((a)<<(64-(n))))


#define DEBUG   0

#if DEBUG
 #include "cli.h"

 void ctx_dump(const bmw_large_ctx_t* ctx){
 	uint8_t i;
	cli_putstr("\r\n==== ctx dump ====");
	for(i=0; i<16;++i){
		cli_putstr("\r\n h[");
		cli_hexdump(&i, 1);
		cli_putstr("] = ");
		cli_hexdump_rev(&(ctx->h[i]), 8);
	}
	cli_putstr("\r\n counter = ");
	cli_hexdump(&(ctx->counter), 4);
 }

 void dump_x(const uint64_t* q, uint8_t elements, char x){
	uint8_t i;
 	cli_putstr("\r\n==== ");
	cli_putc(x);
	cli_putstr(" dump ====");
	for(i=0; i<elements;++i){
		cli_putstr("\r\n ");
		cli_putc(x);
		cli_putstr("[");
		cli_hexdump(&i, 1);
		cli_putstr("] = ");
		cli_hexdump_rev(&(q[i]), 8);
	}
 }
#else
 #define ctx_dump(x)
 #define dump_x(a,b,c)
#endif

#define S64_0(x) ( (SHR64((x),   1)) ^ \
	               (SHL64((x),   3)) ^ \
	               (ROTL64((x),  4)) ^ \
	               (ROTR64((x), 27)) )

#define S64_1(x) ( (SHR64((x),   1)) ^ \
	               (SHL64((x),   2)) ^ \
	               (ROTL64((x), 13)) ^ \
	               (ROTR64((x), 21)) )

#define S64_2(x) ( (SHR64((x),   2)) ^ \
	               (SHL64((x),   1)) ^ \
	               (ROTL64((x), 19)) ^ \
	               (ROTR64((x), 11)) )

#define S64_3(x) ( (SHR64((x),   2)) ^ \
	               (SHL64((x),   2)) ^ \
	               (ROTL64((x), 28)) ^ \
	               (ROTR64((x),  5)) )

#define S64_4(x) ( (SHR64((x),   1)) ^ x)

#define S64_5(x) ( (SHR64((x),   2)) ^ x)

#define R64_1(x)    (ROTL64((x),  5))
#define R64_2(x)    (ROTL64((x), 11))
#define R64_3(x)    (ROTL64((x), 27))
#define R64_4(x)    (ROTL64((x), 32))
#define R64_5(x)    (ROTR64((x), 27))
#define R64_6(x)    (ROTR64((x), 21))
#define R64_7(x)    (ROTR64((x), 11))

/*
#define K    0x0555555555555555LL
#define MASK 0xFFFFFFFFFFFFFFFFLL
static
uint64_t k_lut[] PROGMEM = {
	16LL*K, 17LL*K, 18LL*K, 19LL*K,
	20LL*K, 21LL*K, 22LL*K, 23LL*K,
	24LL*K, 25LL*K, 26LL*K, 27LL*K,
	28LL*K, 29LL*K, 30LL*K, 31LL*K };
*/
/* the same as above but precomputed to avoid compiler warnings */
static const
uint64_t k_lut[] = {
	0x5555555555555550LL, 0x5aaaaaaaaaaaaaa5LL, 0x5ffffffffffffffaLL,
	0x655555555555554fLL, 0x6aaaaaaaaaaaaaa4LL, 0x6ffffffffffffff9LL,
	0x755555555555554eLL, 0x7aaaaaaaaaaaaaa3LL, 0x7ffffffffffffff8LL,
	0x855555555555554dLL, 0x8aaaaaaaaaaaaaa2LL, 0x8ffffffffffffff7LL,
	0x955555555555554cLL, 0x9aaaaaaaaaaaaaa1LL, 0x9ffffffffffffff6LL,
	0xa55555555555554bLL };

static
uint64_t bmw_large_expand1(uint8_t j, const uint64_t* q, const void* m, const void* h){
	uint64_t r;
	/* r = 0x0555555555555555LL*(j+16); */
	r  = (   ROTL64(((uint64_t*)m)[(j)&0xf],   ((j+ 0)&0xf)+1)
	       + ROTL64(((uint64_t*)m)[(j+3)&0xf], ((j+ 3)&0xf)+1)
	       + k_lut[j]
	       - ROTL64(((uint64_t*)m)[(j+10)&0xf],((j+10)&0xf)+1)
	     ) ^ ((uint64_t*)h)[(j+7)&0xf];
	r += S64_1(q[j+ 0]) + S64_2(q[j+ 1]) + S64_3(q[j+ 2]) + S64_0(q[j+ 3]) +
		 S64_1(q[j+ 4]) + S64_2(q[j+ 5]) + S64_3(q[j+ 6]) + S64_0(q[j+ 7]) +
		 S64_1(q[j+ 8]) + S64_2(q[j+ 9]) + S64_3(q[j+10]) + S64_0(q[j+11]) +
		 S64_1(q[j+12]) + S64_2(q[j+13]) + S64_3(q[j+14]) + S64_0(q[j+15]);

	return r;
}

static
uint64_t bmw_large_expand2(uint8_t j, const uint64_t* q, const void* m, const void* h){
	uint64_t r=0;
	r  = (   ROTL64(((uint64_t*)m)[(j)&0xf],   ((j+ 0)&0xf)+1)
	       + ROTL64(((uint64_t*)m)[(j+3)&0xf], ((j+ 3)&0xf)+1)
	       + k_lut[j]
	       - ROTL64(((uint64_t*)m)[(j+10)&0xf],((j+10)&0xf)+1)
	     ) ^ ((uint64_t*)h)[(j+7)&0xf];
	r += (q[j+ 0]) + R64_1(q[j+ 1]) + (q[j+ 2]) + R64_2(q[j+ 3]) +
		 (q[j+ 4]) + R64_3(q[j+ 5]) + (q[j+ 6]) + R64_4(q[j+ 7]) +
		 (q[j+ 8]) + R64_5(q[j+ 9]) + (q[j+10]) + R64_6(q[j+11]) +
		 (q[j+12]) + R64_7(q[j+13]) + S64_4(q[j+14]) + S64_5(q[j+15]);

	return r;
}

static
void bmw_large_f0(uint64_t* q, const uint64_t* h, const void* m){
	uint8_t i;
	for(i=0; i<16; ++i){
		((uint64_t*)h)[i] ^= ((uint64_t*)m)[i];
	}
//	dump_x(t, 16, 'T');
	q[ 0] = (h[ 5] - h[ 7] + h[10] + h[13] + h[14]);
	q[ 1] = (h[ 6] - h[ 8] + h[11] + h[14] - h[15]);
	q[ 2] = (h[ 0] + h[ 7] + h[ 9] - h[12] + h[15]);
	q[ 3] = (h[ 0] - h[ 1] + h[ 8] - h[10] + h[13]);
	q[ 4] = (h[ 1] + h[ 2] + h[ 9] - h[11] - h[14]);
	q[ 5] = (h[ 3] - h[ 2] + h[10] - h[12] + h[15]);
	q[ 6] = (h[ 4] - h[ 0] - h[ 3] - h[11] + h[13]);
	q[ 7] = (h[ 1] - h[ 4] - h[ 5] - h[12] - h[14]);
	q[ 8] = (h[ 2] - h[ 5] - h[ 6] + h[13] - h[15]);
	q[ 9] = (h[ 0] - h[ 3] + h[ 6] - h[ 7] + h[14]);
	q[10] = (h[ 8] - h[ 1] - h[ 4] - h[ 7] + h[15]);
	q[11] = (h[ 8] - h[ 0] - h[ 2] - h[ 5] + h[ 9]);
	q[12] = (h[ 1] + h[ 3] - h[ 6] - h[ 9] + h[10]);
	q[13] = (h[ 2] + h[ 4] + h[ 7] + h[10] + h[11]);
	q[14] = (h[ 3] - h[ 5] + h[ 8] - h[11] - h[12]);
	q[15] = (h[12] - h[ 4] - h[ 6] - h[ 9] + h[13]);
	dump_x(q, 16, 'W');
	q[ 0] = S64_0(q[ 0]); q[ 1] = S64_1(q[ 1]); q[ 2] = S64_2(q[ 2]); q[ 3] = S64_3(q[ 3]); q[ 4] = S64_4(q[ 4]);
	q[ 5] = S64_0(q[ 5]); q[ 6] = S64_1(q[ 6]); q[ 7] = S64_2(q[ 7]); q[ 8] = S64_3(q[ 8]); q[ 9] = S64_4(q[ 9]);
	q[10] = S64_0(q[10]); q[11] = S64_1(q[11]); q[12] = S64_2(q[12]); q[13] = S64_3(q[13]); q[14] = S64_4(q[14]);
	q[15] = S64_0(q[15]);

	for(i=0; i<16; ++i){
		((uint64_t*)h)[i] ^= ((uint64_t*)m)[i];
	}
	for(i=0; i<16; ++i){
		q[i] += h[(i+1)&0xf];
	}
}

static
void bmw_large_f1(uint64_t* q, const void* m, const uint64_t* h){
	uint8_t i;
	q[16] = bmw_large_expand1(0, q, m, h);
	q[17] = bmw_large_expand1(1, q, m, h);
	for(i=2; i<16; ++i){
		q[16+i] = bmw_large_expand2(i, q, m, h);
	}
}

static
void bmw_large_f2(uint64_t* h, const uint64_t* q, const uint64_t* m){

	uint64_t xl, xh;
	xl =      q[16] ^ q[17] ^ q[18] ^ q[19] ^ q[20] ^ q[21] ^ q[22] ^ q[23];
	xh = xl ^ q[24] ^ q[25] ^ q[26] ^ q[27] ^ q[28] ^ q[29] ^ q[30] ^ q[31];
#if DEBUG
	cli_putstr("\r\n XL = ");
	cli_hexdump_rev(&xl, 8);
	cli_putstr("\r\n XH = ");
	cli_hexdump_rev(&xh, 8);
#endif

	h[0] = (SHL64(xh, 5) ^ SHR64(q[16], 5) ^ m[ 0]) + (xl ^ q[24] ^ q[ 0]);
	h[1] = (SHR64(xh, 7) ^ SHL64(q[17], 8) ^ m[ 1]) + (xl ^ q[25] ^ q[ 1]);
	h[2] = (SHR64(xh, 5) ^ SHL64(q[18], 5) ^ m[ 2]) + (xl ^ q[26] ^ q[ 2]);
	h[3] = (SHR64(xh, 1) ^ SHL64(q[19], 5) ^ m[ 3]) + (xl ^ q[27] ^ q[ 3]);
	h[4] = (SHR64(xh, 3) ^ q[20]           ^ m[ 4]) + (xl ^ q[28] ^ q[ 4]);
	h[5] = (SHL64(xh, 6) ^ SHR64(q[21], 6) ^ m[ 5]) + (xl ^ q[29] ^ q[ 5]);
	h[6] = (SHR64(xh, 4) ^ SHL64(q[22], 6) ^ m[ 6]) + (xl ^ q[30] ^ q[ 6]);
	h[7] = (SHR64(xh,11) ^ SHL64(q[23], 2) ^ m[ 7]) + (xl ^ q[31] ^ q[ 7]);

	h[ 8] = ROTL64(h[4],  9) + (xh ^ q[24] ^ m[ 8]) + (SHL64(xl, 8) ^ q[23] ^ q[ 8]);
	h[ 9] = ROTL64(h[5], 10) + (xh ^ q[25] ^ m[ 9]) + (SHR64(xl, 6) ^ q[16] ^ q[ 9]);
	h[10] = ROTL64(h[6], 11) + (xh ^ q[26] ^ m[10]) + (SHL64(xl, 6) ^ q[17] ^ q[10]);
	h[11] = ROTL64(h[7], 12) + (xh ^ q[27] ^ m[11]) + (SHL64(xl, 4) ^ q[18] ^ q[11]);
	h[12] = ROTL64(h[0], 13) + (xh ^ q[28] ^ m[12]) + (SHR64(xl, 3) ^ q[19] ^ q[12]);
	h[13] = ROTL64(h[1], 14) + (xh ^ q[29] ^ m[13]) + (SHR64(xl, 4) ^ q[20] ^ q[13]);
	h[14] = ROTL64(h[2], 15) + (xh ^ q[30] ^ m[14]) + (SHR64(xl, 7) ^ q[21] ^ q[14]);
	h[15] = ROTL64(h[3], 16) + (xh ^ q[31] ^ m[15]) + (SHR64(xl, 2) ^ q[22] ^ q[15]);
}

void bmw_large_nextBlock(bmw_large_ctx_t* ctx, const void* block){
	uint64_t q[32];
	dump_x(block, 16, 'M');
	bmw_large_f0(q, ctx->h, block);
	dump_x(q, 16, 'Q');
	bmw_large_f1(q, block, ctx->h);
	dump_x(q, 32, 'Q');
	bmw_large_f2(ctx->h, q, block);
	ctx->counter += 1;
	ctx_dump(ctx);
}

void bmw_large_lastBlock(bmw_large_ctx_t* ctx, const void* block, uint16_t length_b){
	uint8_t buffer[128];
	while(length_b >= BMW_LARGE_BLOCKSIZE){
		bmw_large_nextBlock(ctx, block);
		length_b -= BMW_LARGE_BLOCKSIZE;
		block = (uint8_t*)block + BMW_LARGE_BLOCKSIZE_B;
	}
	memset(buffer, 0, 128);
	memcpy(buffer, block, (length_b+7)/8);
	buffer[length_b>>3] |= 0x80 >> (length_b&0x07);
	if(length_b+1>128*8-64){
		bmw_large_nextBlock(ctx, buffer);
		memset(buffer, 0, 128-8);
		ctx->counter -= 1;
	}
	*((uint64_t*)&(buffer[128-8])) = (uint64_t)(ctx->counter*1024LL)+(uint64_t)length_b;
	bmw_large_nextBlock(ctx, buffer);
	uint8_t i;
	uint64_t q[32];
	memset(buffer, 0xaa, 128);
	for(i=0; i<16; ++i){
		buffer[8*i] = i + 0xa0;
	}
	bmw_large_f0(q, (uint64_t*)buffer, ctx->h);
	bmw_large_f1(q, ctx->h, (uint64_t*)buffer);
	bmw_large_f2((uint64_t*)buffer, q, ctx->h);
	memcpy(ctx->h, buffer, 128);
}

void bmw384_init(bmw384_ctx_t* ctx){
	uint8_t i;
	ctx->h[0] = 0x0001020304050607LL;
	for(i=1; i<16; ++i){
		ctx->h[i] = ctx->h[i-1]+ 0x0808080808080808LL;
	}
#if BUG24
	ctx->h[6] = 0x3031323324353637LL;
#endif
	ctx->counter=0;
	ctx_dump(ctx);
}

void bmw512_init(bmw512_ctx_t* ctx){
	uint8_t i;
	ctx->h[0] = 0x8081828384858687LL;
	for(i=1; i<16; ++i){
		ctx->h[i] = ctx->h[i-1]+ 0x0808080808080808LL;
	}
	ctx->counter=0;
	ctx_dump(ctx);
}

void bmw384_nextBlock(bmw384_ctx_t* ctx, const void* block){
	bmw_large_nextBlock(ctx, block);
}

void bmw512_nextBlock(bmw512_ctx_t* ctx, const void* block){
	bmw_large_nextBlock(ctx, block);
}

void bmw384_lastBlock(bmw384_ctx_t* ctx, const void* block, uint16_t length_b){
	bmw_large_lastBlock(ctx, block, length_b);
}

void bmw512_lastBlock(bmw512_ctx_t* ctx, const void* block, uint16_t length_b){
	bmw_large_lastBlock(ctx, block, length_b);
}

void bmw384_ctx2hash(void* dest, const bmw384_ctx_t* ctx){
	memcpy(dest, &(ctx->h[10]), 384/8);
}

void bmw512_ctx2hash(void* dest, const bmw512_ctx_t* ctx){
	memcpy(dest, &(ctx->h[8]), 512/8);
}

void bmw384(void* dest, const void* msg, uint32_t length_b){
	bmw_large_ctx_t ctx;
	bmw384_init(&ctx);
	while(length_b>=BMW_LARGE_BLOCKSIZE){
		bmw_large_nextBlock(&ctx, msg);
		length_b -= BMW_LARGE_BLOCKSIZE;
		msg = (uint8_t*)msg + BMW_LARGE_BLOCKSIZE_B;
	}
	bmw_large_lastBlock(&ctx, msg, length_b);
	bmw384_ctx2hash(dest, &ctx);
}

void bmw512(void* dest, const void* msg, uint32_t length_b){
	bmw_large_ctx_t ctx;
	bmw512_init(&ctx);
	while(length_b>=BMW_LARGE_BLOCKSIZE){
		bmw_large_nextBlock(&ctx, msg);
		length_b -= BMW_LARGE_BLOCKSIZE;
		msg = (uint8_t*)msg + BMW_LARGE_BLOCKSIZE_B;
	}
	bmw_large_lastBlock(&ctx, msg, length_b);
	bmw512_ctx2hash(dest, &ctx);
}

