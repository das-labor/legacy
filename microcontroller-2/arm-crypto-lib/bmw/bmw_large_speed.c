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

#include "f1_autogen_large.i"

static inline
void bmw_large_f0(uint64_t* q, uint64_t* h, const uint64_t* m){
	uint64_t t[16];
	uint64_t tr0, tr1, tr2;
	t[ 0] = h[ 0] ^ m[ 0];
	t[ 1] = h[ 1] ^ m[ 1];
	t[ 2] = h[ 2] ^ m[ 2];
	t[ 3] = h[ 3] ^ m[ 3];
	t[ 4] = h[ 4] ^ m[ 4];
	t[ 5] = h[ 5] ^ m[ 5];
	t[ 6] = h[ 6] ^ m[ 6];
	t[ 7] = h[ 7] ^ m[ 7];
	t[ 8] = h[ 8] ^ m[ 8];
	t[ 9] = h[ 9] ^ m[ 9];
	t[10] = h[10] ^ m[10];
	t[11] = h[11] ^ m[11];
	t[12] = h[12] ^ m[12];
	t[13] = h[13] ^ m[13];
	t[14] = h[14] ^ m[14];
	t[15] = h[15] ^ m[15];
	//	dump_x(t, 16, 'T');
	/*
	q[ 0] = (t[ 5] - t[ 7] + t[10] + t[13] + t[14]);
	q[ 1] = (t[ 6] - t[ 8] + t[11] + t[14] - t[15]);
	q[ 2] = (t[ 0] + t[ 7] + t[ 9] - t[12] + t[15]);
	q[ 3] = (t[ 0] - t[ 1] + t[ 8] - t[10] + t[13]);
	q[ 4] = (t[ 1] + t[ 2] + t[ 9] - t[11] - t[14]);
	q[ 5] = (t[ 3] - t[ 2] + t[10] - t[12] + t[15]);
	q[ 6] = (t[ 4] - t[ 0] - t[ 3] - t[11] + t[13]);
	q[ 7] = (t[ 1] - t[ 4] - t[ 5] - t[12] - t[14]);
	q[ 8] = (t[ 2] - t[ 5] - t[ 6] + t[13] - t[15]);
	q[ 9] = (t[ 0] - t[ 3] + t[ 6] - t[ 7] + t[14]);
	q[10] = (t[ 8] - t[ 1] - t[ 4] - t[ 7] + t[15]);
	q[11] = (t[ 8] - t[ 0] - t[ 2] - t[ 5] + t[ 9]);
	q[12] = (t[ 1] + t[ 3] - t[ 6] - t[ 9] + t[10]);
	q[13] = (t[ 2] + t[ 4] + t[ 7] + t[10] + t[11]);
	q[14] = (t[ 3] - t[ 5] + t[ 8] - t[11] - t[12]);
	q[15] = (t[12] - t[ 4] - t[ 6] - t[ 9] + t[13]);
    */
	q[ 0] = +t[ 5] +t[10] +t[13] +(tr1=-t[ 7]+t[14]) ;
	q[ 3] = +t[ 8] +t[13] +t[ 0] +(tr2=-t[ 1]-t[10]) ;
	q[ 6] = -t[11] +t[13] -t[ 0] -t[ 3] +t[ 4] ;
	q[ 9] = +t[ 0] +(tr0=-t[ 3]+t[ 6]) +(tr1) ;
	q[12] = -t[ 9] -(tr0) -(tr2) ;
	q[15] = -t[ 4] +(tr0=-t[ 9]+t[12]) +(tr1=-t[ 6]+t[13]) ;
	q[ 2] = +t[ 7] +t[15] +t[ 0] -(tr0) ;
	q[ 5] = +t[10] +(tr0=-t[ 2]+t[15]) +(tr2=+t[ 3]-t[12]) ;
	q[ 8] = -t[ 5] -(tr0) +(tr1) ;
	q[11] = -t[ 0] -t[ 2] +t[ 9] +(tr0=-t[ 5]+t[ 8]) ;
	q[14] = -t[11] +(tr0) +(tr2) ;
	q[ 1] = +t[ 6] +(tr0=+t[11]+t[14]) +(tr1=-t[ 8]-t[15]) ;
	q[ 4] = +t[ 9] +t[ 1] +t[ 2] -(tr0) ;
	q[ 7] = -t[12] -t[14] +t[ 1] -t[ 4] -t[ 5] ;
	q[10] = -t[ 1] +(tr0=-t[ 4]-t[ 7]) -(tr1) ;
	q[13] = +t[ 2] +t[10] +t[11] -(tr0) ;
	dump_x(q, 16, 'W');

	q[ 0] = S64_0(q[ 0]) + h[ 1];
	q[ 1] = S64_1(q[ 1]) + h[ 2];
	q[ 2] = S64_2(q[ 2]) + h[ 3];
	q[ 3] = S64_3(q[ 3]) + h[ 4];
	q[ 4] = S64_4(q[ 4]) + h[ 5];
	q[ 5] = S64_0(q[ 5]) + h[ 6];
	q[ 6] = S64_1(q[ 6]) + h[ 7];
	q[ 7] = S64_2(q[ 7]) + h[ 8];
	q[ 8] = S64_3(q[ 8]) + h[ 9];
	q[ 9] = S64_4(q[ 9]) + h[10];
	q[10] = S64_0(q[10]) + h[11];
	q[11] = S64_1(q[11]) + h[12];
	q[12] = S64_2(q[12]) + h[13];
	q[13] = S64_3(q[13]) + h[14];
	q[14] = S64_4(q[14]) + h[15];
	q[15] = S64_0(q[15]) + h[ 0];
}

static inline
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
	union {
		uint8_t   v8[128];
		uint64_t v64[ 16];
	} buffer;
	while(length_b >= BMW_LARGE_BLOCKSIZE){
		bmw_large_nextBlock(ctx, block);
		length_b -= BMW_LARGE_BLOCKSIZE;
		block = (uint8_t*)block + BMW_LARGE_BLOCKSIZE_B;
	}
	memset(buffer.v8, 0, 128);
	memcpy(buffer.v8, block, (length_b+7)/8);
	buffer.v8[length_b>>3] |= 0x80 >> (length_b&0x07);
	if(length_b+1>128*8-64){
		bmw_large_nextBlock(ctx, buffer.v8);
		memset(buffer.v8, 0, 128-8);
		ctx->counter -= 1;
	}
	buffer.v64[15] = (uint64_t)(ctx->counter*1024LL)+(uint64_t)length_b;
	bmw_large_nextBlock(ctx, buffer.v8);
	uint8_t i;
	uint64_t q[32];
	memset(buffer.v8, 0xaa, 128);
	for(i=0; i<16; ++i){
		buffer.v8[8*i] = i + 0xa0;
	}
	bmw_large_f0(q, buffer.v64, ctx->h);
	bmw_large_f1(q, ctx->h, buffer.v64);
	bmw_large_f2(buffer.v64, q, ctx->h);
	memcpy(ctx->h, buffer.v8, 128);
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

