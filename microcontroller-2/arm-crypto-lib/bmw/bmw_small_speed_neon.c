/* bmw_small.c */
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
 * \file    bmw_small.c
 * \author  Daniel Otte
 * \email   daniel.otte@rub.de
 * \date    2009-04-27
 * \license GPLv3 or later
 *
 */

#include <stdint.h>
#include <string.h>
#include <arm_neon.h>
#include "bmw_small.h"

#define SHL32(a,n) ((a)<<(n))
#define SHR32(a,n) ((a)>>(n))
#define ROTL32(a,n) (((a)<<(n))|((a)>>(32-(n))))
#define ROTR32(a,n) (((a)>>(n))|((a)<<(32-(n))))


#define DEBUG 0


#if DEBUG
 #include "cli.h"

 void ctx_dump(const bmw_small_ctx_t* ctx){
 	uint8_t i;
	cli_putstr("\r\n==== ctx dump ====");
	for(i=0; i<16;++i){
		cli_putstr("\r\n h[");
		cli_hexdump(&i, 1);
		cli_putstr("] = ");
		cli_hexdump_rev(&(ctx->h[i]), 4);
	}
	cli_putstr("\r\n counter = ");
	cli_hexdump(&(ctx->counter), 4);
 }

 void dump_x(const uint32_t* q, uint8_t elements, char x){
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
		cli_hexdump_rev(&(q[i]), 4);
	}
 }
#else
 #define ctx_dump(x)
 #define dump_x(a,b,c)
#endif

#define S32_0(x) ( (SHR32((x),   1)) ^ \
	               (SHL32((x),   3)) ^ \
	               (ROTL32((x),  4)) ^ \
	               (ROTR32((x), 13)) )

#define S32_1(x) ( (SHR32((x),   1)) ^ \
	               (SHL32((x),   2)) ^ \
	               (ROTL32((x),  8)) ^ \
	               (ROTR32((x),  9)) )

#define S32_2(x) ( (SHR32((x),   2)) ^ \
	               (SHL32((x),   1)) ^ \
	               (ROTL32((x), 12)) ^ \
	               (ROTR32((x),  7)) )

#define S32_3(x) ( (SHR32((x),   2)) ^ \
	               (SHL32((x),   2)) ^ \
	               (ROTL32((x), 15)) ^ \
	               (ROTR32((x),  3)) )

#define S32_4(x) ( (SHR32((x),   1)) ^ (x))

#define S32_5(x) ( (SHR32((x),   2)) ^ (x))

#define R32_1(x)   (ROTL32((x),  3))
#define R32_2(x)   (ROTL32((x),  7))
#define R32_3(x)   (ROTL32((x), 13))
#define R32_4(x)   (ROTL32((x), 16))
#define R32_5(x)   (ROTR32((x), 13))
#define R32_6(x)   (ROTR32((x),  9))
#define R32_7(x)   (ROTR32((x),  5))


#include "f1_autogen_neon_small.i"

static inline
void bmw_small_f0(uint32_t* q, uint32_t* h, const uint32_t* m){
	uint32_t t[16+4];
	uint32x4_t qa0,qa1,qa2,qa3,qb0,qb1,qb2,qb3,qc0,qc1,qc2,qc3;

	/* load qa0..3 with h*/
	qa0 = ((uint32x4_t*)h)[0];
	qa1 = ((uint32x4_t*)h)[1];
	qa2 = ((uint32x4_t*)h)[2];
	qa3 = ((uint32x4_t*)h)[3];

	/* load qb0..3 with m*/
	qb0 = ((uint32x4_t*)m)[0];
	qb1 = ((uint32x4_t*)m)[1];
	qb2 = ((uint32x4_t*)m)[2];
	qb3 = ((uint32x4_t*)m)[3];

	/* qc0..3 = qa0..3 xor qb0..3 */
	qc0 = veorq_u32(qa0, qb0);
	qc1 = veorq_u32(qa1, qb1);
	qc2 = veorq_u32(qa2, qb2);
	qc3 = veorq_u32(qa3, qb3);

	((uint32x4_t*)t)[0] = qc0;
	((uint32x4_t*)t)[1] = qc1;
	((uint32x4_t*)t)[2] = qc2;
	((uint32x4_t*)t)[3] = qc3;
	((uint32x4_t*)t)[4] = qc0;

	dump_x(t, 16, 'T');
/*           0        B        4        0        A
	W_{ 0} = + T_{5}  - T_{7}  + T_{10} + T_{13} + T_{14} -- 1
    W_{ 1} = + T_{6}  - T_{8}  + T_{11} + T_{14} - T_{15} -- 2
    W_{ 2} = + T_{7}  + T_{9}  - T_{12} + T_{15} + T_{0}  -- 1
    W_{ 3} = + T_{8}  - T_{10} + T_{13} + T_{0}  - T_{1}  -- 2
             C        B        5        E        8
    W_{ 4} = + T_{9}  - T_{11} - T_{14} + T_{1}  + T_{2}  -- 2
    W_{ 5} = + T_{10} - T_{12} + T_{15} - T_{2}  + T_{3}  -- 2
    W_{ 6} = - T_{11} + T_{13} - T_{0}  - T_{3}  + T_{4}  -- 3
    W_{ 7} = - T_{12} - T_{14} + T_{1}  - T_{4}  - T_{5}  -- 4
             8        D        E        5        3
    W_{ 8} = + T_{13} - T_{15} + T_{2}  - T_{5}  - T_{6}  -- 3
    W_{ 9} = + T_{14} + T_{0}  - T_{3}  + T_{6}  - T_{7}  -- 2
    W_{10} = + T_{15} - T_{1}  - T_{4}  - T_{7}  + T_{8}  -- 3
    W_{11} = - T_{0}  - T_{2}  - T_{5}  + T_{8}  + T_{9}  -- 3
             8        C        9        5        4
    W_{12} = + T_{1}  + T_{3}  - T_{6}  - T_{9}  + T_{10} -- 2
    W_{13} = + T_{2}  + T_{4}  + T_{7}  + T_{10} + T_{11} -- 0
    W_{14} = + T_{3}  - T_{5}  + T_{8}  - T_{11} - T_{12} -- 3
    W_{15} = - T_{4}  - T_{6}  - T_{9}  + T_{12} + T_{13} -- 3
    0: 2; 1: 0; 2: 0; 3: 1: 4: 2; 5: 3; 6: 0; 7: 0;
    8: 3; 9: 1; A: 1; B: 2; C: 2; D: 1; E: 2; F: 0;
*/
	qa0 = *((uint32x4_t*)&(t[ 5]));
	qa1 = *((uint32x4_t*)&(t[13]));
	qb0 = vaddq_u32(qa0, qa1);
	qb2 = vaddq_u32(veorq_u32(qa0, (uint32x4_t){0, 0xffffffff, 0, 0xffffffff}),
			        veorq_u32(qa1, (uint32x4_t){0xffffffff, 0, 0, 0}));
	qa0 = *((uint32x4_t*)&(t[ 9]));
	qa1 = *((uint32x4_t*)&(t[ 1]));
	qb1 = vaddq_u32(veorq_u32(qa0, (uint32x4_t){0xffffffff, 0xffffffff, 0, 0}),
			        veorq_u32(qa1, (uint32x4_t){0xffffffff, 0xffffffff, 0xffffffff, 0}));
	qb3 = vaddq_u32(veorq_u32(qa0, (uint32x4_t){0, 0xffffffff, 0, 0xffffffff}),
			        veorq_u32(qa1, (uint32x4_t){0xffffffff, 0, 0, 0}));
	qa0 = *((uint32x4_t*)&(t[10]));
	qa1 = *((uint32x4_t*)&(t[14]));
	qa2 = *((uint32x4_t*)&(t[ 2]));
	qa3 = *((uint32x4_t*)&(t[ 6]));
	qc0 = vaddq_u32(veorq_u32(qa0, (uint32x4_t){0, 0xffffffff, 0, 0}),
			        veorq_u32(qa1, (uint32x4_t){0xffffffff, 0, 0xffffffff, 0}));
	qc1 = vaddq_u32(veorq_u32(qa1, (uint32x4_t){0, 0xffffffff, 0, 0xffffffff}),
			        veorq_u32(qa2, (uint32x4_t){0xffffffff, 0, 0, 0}));
	qc2 = vaddq_u32(veorq_u32(qa2, (uint32x4_t){0xffffffff, 0xffffffff, 0xffffffff, 0}),
			        veorq_u32(qa3, (uint32x4_t){0, 0, 0xffffffff, 0xffffffff}));
	qc3 = vaddq_u32(veorq_u32(qa3, (uint32x4_t){0xffffffff, 0, 0, 0xffffffff}),
			        veorq_u32(qa0, (uint32x4_t){0, 0xffffffff, 0, 0}));
	qb0 = vaddq_u32(qb0, qc0);
	qb1 = vaddq_u32(qb1, qc1);
	qb2 = vaddq_u32(qb2, qc2);
	qb3 = vaddq_u32(qb3, qc3);
	qa0 = *((uint32x4_t*)&(t[ 7]));
	qa1 = *((uint32x4_t*)&(t[11]));
	qa2 = *((uint32x4_t*)&(t[15]));
	qa3 = *((uint32x4_t*)&(t[ 3]));
	qc0 = vaddq_u32(veorq_u32(qa0, (uint32x4_t){0xffffffff, 0, 0xffffffff, 0xffffffff}),
			       (uint32x4_t){2,1,2,1});
	qc1 = vaddq_u32(veorq_u32(qa1, (uint32x4_t){0xffffffff, 0, 0xffffffff, 0xffffffff}),
			       (uint32x4_t){4,3,2,2});
	qc2 = vaddq_u32(veorq_u32(qa2, (uint32x4_t){0xffffffff, 0xffffffff, 0, 0xffffffff}),
			       (uint32x4_t){3,3,2,3});
	qc3 = vaddq_u32(veorq_u32(qa3, (uint32x4_t){0xffffffff, 0xffffffff, 0, 0}),
				       (uint32x4_t){3,3,0,2});
	qb0 = vaddq_u32(qb0, qc0);
	qb1 = vaddq_u32(qb1, qc1);
	qb2 = vaddq_u32(qb2, qc2);
	qb3 = vaddq_u32(qb3, qc3);

#if DEBUG
	{
	uin32_t q[16];
	*((uint32x4_t*)&(t[ 0])) = 	qb0;
	*((uint32x4_t*)&(t[ 4])) = 	qb1;
	*((uint32x4_t*)&(t[ 8])) = 	qb2;
	*((uint32x4_t*)&(t[12])) = 	qb3;
	dump_x(q, 16, 'W');
	}
#endif
	/* s0..s3 */
	qa0 = veorq_u32(veorq_u32(vshlq_u32(qb0, (int32x4_t){-1, -1, -2, -2}),
			                  vshlq_u32(qb0, (int32x4_t){3, 2, 1, 2}) ),
			        veorq_u32(veorq_u32(vshlq_u32(qb0,(int32x4_t){4,8,12,15}),
			        		            vshlq_u32(qb0,(int32x4_t){-28,-24,-20,-17})),
			        		  veorq_u32(vshlq_u32(qb0,(int32x4_t){19,23,25,29}),
			        		            vshlq_u32(qb0,(int32x4_t){-13,-9,-7,-3}))));
    /* s4..s2 */
	qa1 = veorq_u32(veorq_u32(vshlq_u32(qb1, (int32x4_t){-1, -1, -1, -2}),
			                  vshlq_u32(qb1, (int32x4_t){0, 3, 2, 1}) ),
			        veorq_u32(veorq_u32(vshlq_u32(qb1,(int32x4_t){0,4,8,12}),
			        		            vshlq_u32(qb1,(int32x4_t){-0, -28, -24, -20}) ),
			        		  veorq_u32(vshlq_u32(qb1,(int32x4_t){0,19,23,25}),
			        		            vshlq_u32(qb1,(int32x4_t){-0, -13, -9, -7}) )));
	/* s3..s1 */
	qa2 = veorq_u32(veorq_u32(vshlq_u32(qb2, (int32x4_t){-2, -1, -1, -1}),
				              vshlq_u32(qb2, (int32x4_t){2, 0, 3, 2}) ),
				        veorq_u32(veorq_u32(vshlq_u32(qb2,(int32x4_t){15,0,4,8}),
				        		            vshlq_u32(qb2,(int32x4_t){-17, -0, -28, -24}) ),
				        		  veorq_u32(vshlq_u32(qb2,(int32x4_t){29,0,19,23}),
				        		            vshlq_u32(qb2,(int32x4_t){-3, -0, -13, -9}) )));
	/* s2..s0 */
	qa3 = veorq_u32(veorq_u32(vshlq_u32(qb3, (int32x4_t){-2, -2, -1, -1}),
						      vshlq_u32(qb3, (int32x4_t){1, 2, 0, 3}) ),
						veorq_u32(veorq_u32(vshlq_u32(qb3,(int32x4_t){12, 15,0,4}),
											vshlq_u32(qb3,(int32x4_t){-20, -17, -0, -28}) ),
								  veorq_u32(vshlq_u32(qb3,(int32x4_t){25, 29,0,19}),
											vshlq_u32(qb3,(int32x4_t){-7, -3, -0, -13}) )));
	qb0 = *((uint32x4_t*)&(h[ 1]));
	qb1 = *((uint32x4_t*)&(h[ 5]));
	qb2 = *((uint32x4_t*)&(h[ 9]));
	qb3 = *((uint32x4_t*)&(h[13]));
	qb3 = vsetq_lane_u32(h[0], qb3, 3);
	qa0 = vaddq_u32(qa0, qb0);
	qa1 = vaddq_u32(qa0, qb1);
	qa2 = vaddq_u32(qa0, qb2);
	qa3 = vaddq_u32(qa0, qb3);
	*((uint32x4_t*)&(q[ 0])) = qa0;
	*((uint32x4_t*)&(q[ 4])) = qa1;
	*((uint32x4_t*)&(q[ 8])) = qa2;
	*((uint32x4_t*)&(q[12])) = qa3;
}

static inline
void bmw_small_f2(uint32_t* h, uint32_t* q, const uint32_t* m){
	uint32_t xl, xh;
	uint32x4_t qxl, qxh, qq16, qq20, qq24, qq28;
	uint32x2_t dtmp0, dtmp1;
	qq16 = *((uint32x4_t*)(&(q[16])));
	qq20 = *((uint32x4_t*)(&(q[20])));
	qq24 = *((uint32x4_t*)(&(q[24])));
	qq28 = *((uint32x4_t*)(&(q[28])));
	qxl = veorq_u32(qq16, qq20);
	qxh = veorq_u32(veorq_u32(qq24, qq28), qxl);
	dtmp0 = veor_u32(vget_low_u32(qxl), vget_high_u32(qxl));
	dtmp0 = veor_u32(dtmp0, vreinterpret_u32_u64(vshr_n_u64(vreinterpret_u64_u32(dtmp0), 32)));
	qxl = vdupq_lane_u32(dtmp0, 0);
	dtmp0 = veor_u32(vget_low_u32(qxh), vget_high_u32(qxh));
	dtmp0 = veor_u32(dtmp0, vreinterpret_u32_u64(vshr_n_u64(vreinterpret_u64_u32(dtmp0), 32)));
	qxh = vdupq_lane_u32(dtmp0, 0);

/*
	xl =      q[16] ^ q[17] ^ q[18] ^ q[19] ^ q[20] ^ q[21] ^ q[22] ^ q[23];
	xh = xl ^ q[24] ^ q[25] ^ q[26] ^ q[27] ^ q[28] ^ q[29] ^ q[30] ^ q[31];
*/
#if DEBUG
	cli_putstr("\r\n XL = ");
	cli_hexdump_rev(&xl, 4);
	cli_putstr("\r\n XH = ");
	cli_hexdump_rev(&xh, 4);
#endif
	uint32x4_t qh0, qh4, qh8, qh12;

	qh0 = vaddq_u32(veorq_u32(veorq_u32(vshlq_u32(qxh,  (int32x4_t){5, -7, -5, -1}),
			                            vshlq_u32(qq16, (int32x4_t){-5, 8, 5, 5})	),
			                  *((uint32x4_t*)(&(m[0])))),
			        veorq_u32(veorq_u32(qxl, qq24), *((uint32x4_t*)(&(q[0]))) ));
	qh4 = vaddq_u32(veorq_u32(veorq_u32(vshlq_u32(qxh,  (int32x4_t){-3, 6, -4, -11}),
			                            vshlq_u32(qq20, (int32x4_t){0, -6, 6, 2})	),
			                  *((uint32x4_t*)(&(m[4])))),
			        veorq_u32(veorq_u32(qxl, qq28), *((uint32x4_t*)(&(q[4]))) ));
	qq16 = *((uint32x4_t*)&(q[15]));
	qq16 = vsetq_lane_u32(q[23], qq16, 0);
	qh8 = vaddq_u32(veorq_u32(vshlq_u32(qh4, (int32x4_t){  9, 10, 11, 12}),
			                  vshlq_u32(qh4, (int32x4_t){-23,-22,-21,-20})),
			        vaddq_u32(veorq_u32(qxh, veorq_u32(qq24, *((uint32x4_t*)&(m[8])) )),
                              veorq_u32(vshlq_u32(qxl,(int32x4_t){8,-6,6,4}),
                              veorq_u32(qq16,*((uint32x4_t*)&(q[8]))) )));
	qh12 = vaddq_u32(veorq_u32(vshlq_u32(qh0, (int32x4_t){ 13, 14, 15, 16}),
			                   vshlq_u32(qh0, (int32x4_t){-19,-18,-17,-16})),
			         vaddq_u32(veorq_u32(qxh, veorq_u32(qq28, *((uint32x4_t*)&(m[12])) )),
                               veorq_u32(vshlq_u32(qxl,(int32x4_t){-3,-4,-7,-2}),
                               veorq_u32(*((uint32x4_t*)&(q[19])),*((uint32x4_t*)&(q[12]))) )));

	*((uint32x4_t*)&(h[ 0])) = qh0;
	*((uint32x4_t*)&(h[ 4])) = qh4;
	*((uint32x4_t*)&(h[ 8])) = qh8;
	*((uint32x4_t*)&(h[12])) = qh12;
	/*
	h[0] = (SHL32(xh, 5) ^ SHR32(q[16], 5) ^ m[ 0]) + (xl ^ q[24] ^ q[ 0]);
	h[1] = (SHR32(xh, 7) ^ SHL32(q[17], 8) ^ m[ 1]) + (xl ^ q[25] ^ q[ 1]);
	h[2] = (SHR32(xh, 5) ^ SHL32(q[18], 5) ^ m[ 2]) + (xl ^ q[26] ^ q[ 2]);
	h[3] = (SHR32(xh, 1) ^ SHL32(q[19], 5) ^ m[ 3]) + (xl ^ q[27] ^ q[ 3]);
	h[4] = (SHR32(xh, 3) ^ q[20]           ^ m[ 4]) + (xl ^ q[28] ^ q[ 4]);
	h[5] = (SHL32(xh, 6) ^ SHR32(q[21], 6) ^ m[ 5]) + (xl ^ q[29] ^ q[ 5]);
	h[6] = (SHR32(xh, 4) ^ SHL32(q[22], 6) ^ m[ 6]) + (xl ^ q[30] ^ q[ 6]);
	h[7] = (SHR32(xh,11) ^ SHL32(q[23], 2) ^ m[ 7]) + (xl ^ q[31] ^ q[ 7]);

	h[ 8] = ROTL32(h[4],  9) + (xh ^ q[24] ^ m[ 8]) + (SHL32(xl, 8) ^ q[23] ^ q[ 8]);
	h[ 9] = ROTL32(h[5], 10) + (xh ^ q[25] ^ m[ 9]) + (SHR32(xl, 6) ^ q[16] ^ q[ 9]);
	h[10] = ROTL32(h[6], 11) + (xh ^ q[26] ^ m[10]) + (SHL32(xl, 6) ^ q[17] ^ q[10]);
	h[11] = ROTL32(h[7], 12) + (xh ^ q[27] ^ m[11]) + (SHL32(xl, 4) ^ q[18] ^ q[11]);
	h[12] = ROTL32(h[0], 13) + (xh ^ q[28] ^ m[12]) + (SHR32(xl, 3) ^ q[19] ^ q[12]);
	h[13] = ROTL32(h[1], 14) + (xh ^ q[29] ^ m[13]) + (SHR32(xl, 4) ^ q[20] ^ q[13]);
	h[14] = ROTL32(h[2], 15) + (xh ^ q[30] ^ m[14]) + (SHR32(xl, 7) ^ q[21] ^ q[14]);
	h[15] = ROTL32(h[3], 16) + (xh ^ q[31] ^ m[15]) + (SHR32(xl, 2) ^ q[22] ^ q[15]);
*/
}

void bmw_small_nextBlock(bmw_small_ctx_t* ctx, const void* block){
	uint32_t q[32];
	dump_x(block, 16, 'M');
	bmw_small_f0(q, ctx->h, block);
	dump_x(q, 16, 'Q');
	bmw_small_f1(q, block, ctx->h);
	dump_x(q+16, 16, 'Q');
	bmw_small_f2(ctx->h, q, block);
	ctx->counter += 1;
	ctx_dump(ctx);
}

void bmw_small_lastBlock(bmw_small_ctx_t* ctx, const void* block, uint16_t length_b){
	uint8_t buffer[64];
	while(length_b >= BMW_SMALL_BLOCKSIZE){
		bmw_small_nextBlock(ctx, block);
		length_b -= BMW_SMALL_BLOCKSIZE;
		block = (uint8_t*)block + BMW_SMALL_BLOCKSIZE_B;
	}
	memset(buffer, 0, 64);
	memcpy(buffer, block, (length_b+7)/8);
	buffer[length_b>>3] |= 0x80 >> (length_b&0x07);
	if(length_b+1>64*8-64){
		bmw_small_nextBlock(ctx, buffer);
		memset(buffer, 0, 64-8);
		ctx->counter -= 1;
	}
	*((uint64_t*)&(buffer[64-8])) = (uint64_t)(ctx->counter*512LL)+(uint64_t)length_b;
	bmw_small_nextBlock(ctx, buffer);
	uint8_t i;
	uint32_t q[32];
	memset(buffer, 0xaa, 64);
	for(i=0; i<16;++i){
		buffer[i*4] = i+0xa0;
	}
//	dump_x(buffer, 16, 'A');
	dump_x(ctx->h, 16, 'M');
	bmw_small_f0(q, (uint32_t*)buffer, ctx->h);
	dump_x(buffer, 16, 'a');
	dump_x(q, 16, 'Q');
	bmw_small_f1(q, ctx->h, (uint32_t*)buffer);
	dump_x(q, 32, 'Q');
	bmw_small_f2((uint32_t*)buffer, q, ctx->h);
	memcpy(ctx->h, buffer, 64);
}

void bmw224_init(bmw224_ctx_t* ctx){
	uint8_t i;
	ctx->h[0] = 0x00010203;
	for(i=1; i<16; ++i){
		ctx->h[i] = ctx->h[i-1]+ 0x04040404;
	}
	ctx->counter=0;
	ctx_dump(ctx);
}

void bmw256_init(bmw256_ctx_t* ctx){
	uint8_t i;
	ctx->h[0] = 0x40414243;
	for(i=1; i<16; ++i){
		ctx->h[i] = ctx->h[i-1]+ 0x04040404;
	}
	ctx->counter=0;
	ctx_dump(ctx);
}

void bmw224_nextBlock(bmw224_ctx_t* ctx, const void* block){
	bmw_small_nextBlock(ctx, block);
}

void bmw256_nextBlock(bmw256_ctx_t* ctx, const void* block){
	bmw_small_nextBlock(ctx, block);
}

void bmw224_lastBlock(bmw224_ctx_t* ctx, const void* block, uint16_t length_b){
	bmw_small_lastBlock(ctx, block, length_b);
}

void bmw256_lastBlock(bmw256_ctx_t* ctx, const void* block, uint16_t length_b){
	bmw_small_lastBlock(ctx, block, length_b);
}

void bmw224_ctx2hash(void* dest, const bmw224_ctx_t* ctx){
	memcpy(dest, &(ctx->h[9]), 224/8);
}

void bmw256_ctx2hash(void* dest, const bmw256_ctx_t* ctx){
	memcpy(dest, &(ctx->h[8]), 256/8);
}

void bmw224(void* dest, const void* msg, uint32_t length_b){
	bmw_small_ctx_t ctx;
	bmw224_init(&ctx);
	while(length_b>=BMW_SMALL_BLOCKSIZE){
		bmw_small_nextBlock(&ctx, msg);
		length_b -= BMW_SMALL_BLOCKSIZE;
		msg = (uint8_t*)msg + BMW_SMALL_BLOCKSIZE_B;
	}
	bmw_small_lastBlock(&ctx, msg, length_b);
	bmw224_ctx2hash(dest, &ctx);
}

void bmw256(void* dest, const void* msg, uint32_t length_b){
	bmw_small_ctx_t ctx;
	bmw256_init(&ctx);
	while(length_b>=BMW_SMALL_BLOCKSIZE){
		bmw_small_nextBlock(&ctx, msg);
		length_b -= BMW_SMALL_BLOCKSIZE;
		msg = (uint8_t*)msg + BMW_SMALL_BLOCKSIZE_B;
	}
	bmw_small_lastBlock(&ctx, msg, length_b);
	bmw256_ctx2hash(dest, &ctx);
}

