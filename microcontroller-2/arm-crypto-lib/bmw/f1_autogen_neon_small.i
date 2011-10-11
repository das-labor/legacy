/* BEGIN of automatic generated code */
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

static inline
void bmw_small_f1(uint32_t* q, const uint32_t* m, const uint32_t* h){ 
  uint32_t even, odd;
  uint32x4_t qq16, qq20, qq24, qq28;
  uint32x4_t qm0, qm1, qm2;
  uint32x4_t qk={0x55555550UL, 0x5aaaaaa5UL, 0x5ffffffaUL, 0x6555554fUL};
  uint32x4_t qkadd={0x15555554UL, 0x15555554UL, 0x15555554UL, 0x15555554UL};
  uint32x2_t dtmp0;
  uint32x4x2_t q2tmp0, q2tmp1;   

  /* addElement for q16 .. q19 */
  qm0 = *((uint32x4_t*)&(m[ 0]));
  qm1 = *((uint32x4_t*)&(m[ 3]));
  qm2 = *((uint32x4_t*)&(m[10]));
  qm0 = veorq_u32(vshlq_u32(qm0,(int32x4_t){ 1,  2,  3,  4}),vshlq_u32(qm0,(int32x4_t){-31, -30, -29, -28}));
  qm1 = veorq_u32(vshlq_u32(qm1,(int32x4_t){ 4,  5,  6,  7}),vshlq_u32(qm1,(int32x4_t){-28, -27, -26, -25}));
  qm2 = veorq_u32(vshlq_u32(qm2,(int32x4_t){11, 12, 13, 14}),vshlq_u32(qm2,(int32x4_t){-21, -20, -19, -18}));
  qq16 = veorq_u32(vaddq_u32(vaddq_u32(qm0, qm1),vsubq_u32(qk, qm2)), *((uint32x4_t*)&(h[ 7])));
  qk = vaddq_u32(qk, qkadd);

  /* addElement for q20 .. q23 */
  qm0 = *((uint32x4_t*)&(m[ 4]));
  qm1 = *((uint32x4_t*)&(m[ 7]));
  qm2 = *((uint32x4_t*)&(m[14]));
  qm0 = veorq_u32(vshlq_u32(qm0,(int32x4_t){ 5,  6,  7,  8}),vshlq_u32(qm0,(int32x4_t){-27, -26, -25, -24}));
  qm1 = veorq_u32(vshlq_u32(qm1,(int32x4_t){ 8,  9, 10, 11}),vshlq_u32(qm1,(int32x4_t){-24, -23, -22, -21}));
  qm2 = veorq_u32(vshlq_u32(qm2,(int32x4_t){15, 16,  1,  2}),vshlq_u32(qm2,(int32x4_t){-17, -16, -31, -30}));
  qq20 = veorq_u32(vaddq_u32(vaddq_u32(qm0, qm1),vsubq_u32(qk, qm2)), *((uint32x4_t*)&(h[11])));
  qk = vaddq_u32(qk, qkadd);

  /* addElement for q24 .. q27 */
  qm0 = *((uint32x4_t*)&(m[ 8]));
  qm1 = *((uint32x4_t*)&(m[11]));
  qm2 = *((uint32x4_t*)&(m[18]));
  qm0 = veorq_u32(vshlq_u32(qm0,(int32x4_t){ 9, 10, 11, 12}),vshlq_u32(qm0,(int32x4_t){-23, -22, -21, -20}));
  qm1 = veorq_u32(vshlq_u32(qm1,(int32x4_t){12, 13, 14, 15}),vshlq_u32(qm1,(int32x4_t){-20, -19, -18, -17}));
  qm2 = veorq_u32(vshlq_u32(qm2,(int32x4_t){ 3,  4,  5,  6}),vshlq_u32(qm2,(int32x4_t){-29, -28, -27, -26}));
  qq24 = veorq_u32(vaddq_u32(vaddq_u32(qm0, qm1),vsubq_u32(qk, qm2)), *((uint32x4_t*)&(h[15])));
  qk = vaddq_u32(qk, qkadd);

  /* addElement for q28 .. q31 */
  qm0 = *((uint32x4_t*)&(m[12]));
  qm1 = *((uint32x4_t*)&(m[15]));
  qm2 = *((uint32x4_t*)&(m[22]));
  qm0 = veorq_u32(vshlq_u32(qm0,(int32x4_t){13, 14, 15, 16}),vshlq_u32(qm0,(int32x4_t){-19, -18, -17, -16}));
  qm1 = veorq_u32(vshlq_u32(qm1,(int32x4_t){16,  1,  2,  3}),vshlq_u32(qm1,(int32x4_t){-16, -31, -30, -29}));
  qm2 = veorq_u32(vshlq_u32(qm2,(int32x4_t){ 7,  8,  9, 10}),vshlq_u32(qm2,(int32x4_t){-25, -24, -23, -22}));
  qq28 = veorq_u32(vaddq_u32(vaddq_u32(qm0, qm1),vsubq_u32(qk, qm2)), *((uint32x4_t*)&(h[ 3])));
  qk = vaddq_u32(qk, qkadd);

  /* expand1( 0) */
  qm0 = *((uint32x4_t*)&(q[ 0]));
  qm1 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),
                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),
                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),
                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),
                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}), 
                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));
  qm0 = *((uint32x4_t*)&(q[ 4]));
  qm2 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),
                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),
                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),
                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),
                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}),
                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));
  qm2 = vaddq_u32(qm2, qm1);
  qm0 = *((uint32x4_t*)&(q[ 8]));
  qm1 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),
                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),
                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),
                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),
                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}),
                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));
  qm2 = vaddq_u32(qm2, qm1);
  qm0 = *((uint32x4_t*)&(q[12]));
  qm1 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),
                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),
                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),
                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),
                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}),
                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));
  qm2 = vaddq_u32(qm2, qm1);
  dtmp0 = vadd_u32(vget_high_u32(qm2), vget_low_u32(qm2));
  q[16] = vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1) + vgetq_lane_u32(qq16,  0);

  /* expand1( 1) */
  qm0 = *((uint32x4_t*)&(q[ 1]));
  qm1 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),
                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),
                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),
                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),
                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}), 
                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));
  qm0 = *((uint32x4_t*)&(q[ 5]));
  qm2 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),
                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),
                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),
                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),
                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}),
                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));
  qm2 = vaddq_u32(qm2, qm1);
  qm0 = *((uint32x4_t*)&(q[ 9]));
  qm1 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),
                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),
                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),
                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),
                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}),
                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));
  qm2 = vaddq_u32(qm2, qm1);
  qm0 = *((uint32x4_t*)&(q[13]));
  qm1 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),
                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),
                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),
                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),
                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}),
                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));
  qm2 = vaddq_u32(qm2, qm1);
  dtmp0 = vadd_u32(vget_high_u32(qm2), vget_low_u32(qm2));
  q[17] = vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1) + vgetq_lane_u32(qq16,  1);

  /* expand2( 2) */
  q2tmp0 = vld2q_u32(&q[ 2]);
  q2tmp1 = vld2q_u32(&q[10]);
  q2tmp1.val[0] = vsetq_lane_u32(0, q2tmp1.val[0], 3);
  q2tmp0.val[0] = vaddq_u32(q2tmp0.val[0], q2tmp1.val[0]);
  dtmp0 = vadd_u32(vget_high_u32(q2tmp0.val[0]), vget_low_u32(q2tmp0.val[0]));
  even = vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);
  q[18] = even + ((q[16]>>1)|q[16]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[18] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2( 3) */
  q2tmp0 = vld2q_u32(&q[ 3]);
  q2tmp1 = vld2q_u32(&q[11]);
  q2tmp1.val[0] = vsetq_lane_u32(0, q2tmp1.val[0], 3);
  q2tmp0.val[0] = vaddq_u32(q2tmp0.val[0], q2tmp1.val[0]);
  dtmp0 = vadd_u32(vget_high_u32(q2tmp0.val[0]), vget_low_u32(q2tmp0.val[0]));
  odd  = vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);
  q[19] = odd  + ((q[17]>>1)|q[17]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[19] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2( 4) */
  q2tmp0 = vld2q_u32(&q[ 4]);
  q2tmp1 = vld2q_u32(&q[12]);
  even += q[16] - q[ 2];
  q[20] = even + ((q[18]>>1)|q[18]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[20] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2( 5) */
  q2tmp0 = vld2q_u32(&q[ 5]);
  q2tmp1 = vld2q_u32(&q[13]);
  odd  += q[17] - q[ 3];
  q[21] = odd  + ((q[19]>>1)|q[19]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[21] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2( 6) */
  q2tmp0 = vld2q_u32(&q[ 6]);
  q2tmp1 = vld2q_u32(&q[14]);
  even += q[18] - q[ 4];
  q[22] = even + ((q[20]>>1)|q[20]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[22] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2( 7) */
  q2tmp0 = vld2q_u32(&q[ 7]);
  q2tmp1 = vld2q_u32(&q[15]);
  odd  += q[19] - q[ 5];
  q[23] = odd  + ((q[21]>>1)|q[21]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[23] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2( 8) */
  q2tmp0 = vld2q_u32(&q[ 8]);
  q2tmp1 = vld2q_u32(&q[16]);
  even += q[20] - q[ 6];
  q[24] = even + ((q[22]>>1)|q[22]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[24] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2( 9) */
  q2tmp0 = vld2q_u32(&q[ 9]);
  q2tmp1 = vld2q_u32(&q[17]);
  odd  += q[21] - q[ 7];
  q[25] = odd  + ((q[23]>>1)|q[23]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[25] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2(10) */
  q2tmp0 = vld2q_u32(&q[10]);
  q2tmp1 = vld2q_u32(&q[18]);
  even += q[22] - q[ 8];
  q[26] = even + ((q[24]>>1)|q[24]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[26] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2(11) */
  q2tmp0 = vld2q_u32(&q[11]);
  q2tmp1 = vld2q_u32(&q[19]);
  odd  += q[23] - q[ 9];
  q[27] = odd  + ((q[25]>>1)|q[25]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[27] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2(12) */
  q2tmp0 = vld2q_u32(&q[12]);
  q2tmp1 = vld2q_u32(&q[20]);
  even += q[24] - q[10];
  q[28] = even + ((q[26]>>1)|q[26]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[28] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2(13) */
  q2tmp0 = vld2q_u32(&q[13]);
  q2tmp1 = vld2q_u32(&q[21]);
  odd  += q[25] - q[11];
  q[29] = odd  + ((q[27]>>1)|q[27]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[29] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2(14) */
  q2tmp0 = vld2q_u32(&q[14]);
  q2tmp1 = vld2q_u32(&q[22]);
  even += q[26] - q[12];
  q[30] = even + ((q[28]>>1)|q[28]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[30] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);

  /* expand2(15) */
  q2tmp0 = vld2q_u32(&q[15]);
  q2tmp1 = vld2q_u32(&q[23]);
  odd  += q[27] - q[13];
  q[31] = odd  + ((q[29]>>1)|q[29]);
  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),
                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));
  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),
                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));
  qm1 = vaddq_u32(qm1, qm0);
  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));
  q[31] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);
}

/* END of automatic generated code */

