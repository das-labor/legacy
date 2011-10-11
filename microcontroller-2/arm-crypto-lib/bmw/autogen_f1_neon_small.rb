# autogen f1 function for BMW
=begin
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
=end


header = <<EOF
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
EOF

footer = <<EOF
}

/* END of automatic generated code */

EOF


=begin
  uint32_t r;
  /* r = 0x05555555*(j+16); */
  r = (   ROTL32(((uint32_t*)m)[j],      ((j+0))+1  )
         + ROTL32(((uint32_t*)m)[(j+3)],  ((j+3))+1  )
         - ROTL32(((uint32_t*)m)[(j+10)], ((j+10))+1 )
         + k_lut[j]
       ) ^ ((uint32_t*)h)[(j+7)];
  r += S32_1(q[j+ 0]) + S32_2(q[j+ 1]) + S32_3(q[j+ 2]) + S32_0(q[j+ 3]) +
     S32_1(q[j+ 4]) + S32_2(q[j+ 5]) + S32_3(q[j+ 6]) + S32_0(q[j+ 7]) +
     S32_1(q[j+ 8]) + S32_2(q[j+ 9]) + S32_3(q[j+10]) + S32_0(q[j+11]) +
     S32_1(q[j+12]) + S32_2(q[j+13]) + S32_3(q[j+14]) + S32_0(q[j+15]);
=end
def gen_addElement(i)
  j = i-16;
  s  = sprintf("\n  /* addElement for q%d .. q%d */\n", i, i+3);
  s += sprintf("  qm0 = *((uint32x4_t*)&(m[%2d]));\n", j);
  s += sprintf("  qm1 = *((uint32x4_t*)&(m[%2d]));\n", j+3);
  s += sprintf("  qm2 = *((uint32x4_t*)&(m[%2d]));\n", j+10);
  s += sprintf("  qm0 = veorq_u32(vshlq_u32(qm0,(int32x4_t){%2d, %2d, %2d, %2d}),vshlq_u32(qm0,(int32x4_t){%2d, %2d, %2d, %2d}));\n",
                  (j+0)%16+1, (j+1)%16+1, (j+2)%16+1, (j+3)%16+1, 
                  -(32-((j+0)%16+1)), -(32-((j+1)%16+1)), -(32-((j+2)%16+1)), -(32-((j+3)%16+1)))
  s += sprintf("  qm1 = veorq_u32(vshlq_u32(qm1,(int32x4_t){%2d, %2d, %2d, %2d}),vshlq_u32(qm1,(int32x4_t){%2d, %2d, %2d, %2d}));\n",
                  (j+3)%16+1, (j+4)%16+1, (j+5)%16+1, (j+6)%16+1,
                  -(32-((j+3)%16+1)), -(32-((j+4)%16+1)), -(32-((j+5)%16+1)), -(32-((j+6)%16+1)))
  s += sprintf("  qm2 = veorq_u32(vshlq_u32(qm2,(int32x4_t){%2d, %2d, %2d, %2d}),vshlq_u32(qm2,(int32x4_t){%2d, %2d, %2d, %2d}));\n",
                  (j+10)%16+1, (j+11)%16+1, (j+12)%16+1, (j+13)%16+1, 
                  -(32-((j+10)%16+1)), -(32-((j+11)%16+1)), -(32-((j+12)%16+1)), -(32-((j+13)%16+1)))
  s += sprintf("  qq%d = veorq_u32(vaddq_u32(vaddq_u32(qm0, qm1),vsubq_u32(qk, qm2)), *((uint32x4_t*)&(h[%2d])));\n",
                  i, (j+7)%16)
  s += sprintf("  qk = vaddq_u32(qk, qkadd);\n");                
  return s
end

=begin
  r += S32_1(q[j+ 0]) + S32_2(q[j+ 1]) + S32_3(q[j+ 2]) + S32_0(q[j+ 3]) +
     S32_1(q[j+ 4]) + S32_2(q[j+ 5]) + S32_3(q[j+ 6]) + S32_0(q[j+ 7]) +
     S32_1(q[j+ 8]) + S32_2(q[j+ 9]) + S32_3(q[j+10]) + S32_0(q[j+11]) +
     S32_1(q[j+12]) + S32_2(q[j+13]) + S32_3(q[j+14]) + S32_0(q[j+15]);
 
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
=end

def gen_expand_1(i)
  s  = sprintf("\n  /* expand1(%2d) */\n", i)
  s += sprintf("  qm0 = *((uint32x4_t*)&(q[%2d]));\n", i);
  s += sprintf("  qm1 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),\n" \
               "                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),\n" \
               "                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),\n" \
               "                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),\n" \
               "                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}), \n" \
               "                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));\n")
  s += sprintf("  qm0 = *((uint32x4_t*)&(q[%2d]));\n", i+4);
  s += sprintf("  qm2 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),\n" \
               "                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),\n" \
               "                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),\n" \
               "                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),\n" \
               "                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}),\n" \
               "                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));\n")
  s += sprintf("  qm2 = vaddq_u32(qm2, qm1);\n")                                                   
  s += sprintf("  qm0 = *((uint32x4_t*)&(q[%2d]));\n", i+8);
  s += sprintf("  qm1 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),\n" \
               "                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),\n" \
               "                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),\n" \
               "                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),\n" \
               "                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}),\n" \
               "                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));\n")
  s += sprintf("  qm2 = vaddq_u32(qm2, qm1);\n")                                                   
  s += sprintf("  qm0 = *((uint32x4_t*)&(q[%2d]));\n", i+12);
  s += sprintf("  qm1 = veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){ -1, -2, -2, -1}),\n" \
               "                            vshlq_u32(qm0,(int32x4_t){  2,  1,  2,  3})),\n" \
               "                  veorq_u32(veorq_u32(vshlq_u32(qm0,(int32x4_t){  8, 12, 15,  4}),\n" \
               "                                      vshlq_u32(qm0,(int32x4_t){-24,-20,-17,-28})),\n" \
               "                            veorq_u32(vshlq_u32(qm0,(int32x4_t){ 23, 25, 29, 19}),\n" \
               "                                      vshlq_u32(qm0,(int32x4_t){ -9, -7, -3,-13}))));\n")
  s += sprintf("  qm2 = vaddq_u32(qm2, qm1);\n")                                                   
  s += sprintf("  dtmp0 = vadd_u32(vget_high_u32(qm2), vget_low_u32(qm2));\n")
  s += sprintf("  q[%2d] = vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1) + vgetq_lane_u32(qq%d, %2d);\n",
                 i+16, (i+16)&0xFC, i&3)
  return s
end

=begin     
     + ( even =  q[ 2] + q[ 4] + q[ 6]
               + q[ 8] + q[10] + q[12] + q[14] )
     + R32_1(q[ 3]) + R32_2(q[ 5]) + R32_3(q[ 7])
     + R32_4(q[ 9]) + R32_5(q[11]) + R32_6(q[13])
     + R32_7(q[15]) + S32_4(q[16]) + S32_5(q[17]);
     + ( odd  =  q[ 3] + q[ 5] + q[ 7]
               + q[ 9] + q[11] + q[13] + q[15] )
     + R32_1(q[ 4]) + R32_2(q[ 6]) + R32_3(q[ 8])
     + R32_4(q[10]) + R32_5(q[12]) + R32_6(q[14])
     + R32_7(q[16]) + S32_4(q[17]) + S32_5(q[18]);

#define S32_4(x) ( (SHR32((x),   1)) ^ (x))
#define S32_5(x) ( (SHR32((x),   2)) ^ (x))

#define R32_1(x)   (ROTL32((x),  3))
#define R32_2(x)   (ROTL32((x),  7))
#define R32_3(x)   (ROTL32((x), 13))
#define R32_4(x)   (ROTL32((x), 16))
#define R32_5(x)   (ROTR32((x), 13))
#define R32_6(x)   (ROTR32((x),  9))
#define R32_7(x)   (ROTR32((x),  5))
=end

def gen_expand_2(i, start)
  s  = sprintf("\n  /* expand2(%2d) */\n", i)
  s += sprintf("  q2tmp0 = vld2q_u32(&q[%2d]);\n", i)
  s += sprintf("  q2tmp1 = vld2q_u32(&q[%2d]);\n", i+8)
  if i-1<=start
    s += sprintf("  q2tmp1.val[0] = vsetq_lane_u32(0, q2tmp1.val[0], 3);\n")
    s += sprintf("  q2tmp0.val[0] = vaddq_u32(q2tmp0.val[0], q2tmp1.val[0]);\n")
    s += sprintf("  dtmp0 = vadd_u32(vget_high_u32(q2tmp0.val[0]), vget_low_u32(q2tmp0.val[0]));\n")
    s += sprintf("  %s = vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);\n",(i%2==0)?"even":"odd ")
  else
    s += sprintf("  %s += q[%2d] - q[%2d];\n",(i%2==0)?"even":"odd ", i+12, i-2)  
  end
  s += sprintf("  q[%2d] = %s + ((q[%2d]>>1)|q[%2d]);\n", i+16, (i%2==0)?"even":"odd ", i+14, i+14)  
  s += sprintf("  qm0 = veorq_u32(vshlq_u32(q2tmp0.val[1],(int32x4_t){  3,  7, 13, 16}),\n" \
               "                  vshlq_u32(q2tmp0.val[1],(int32x4_t){-29,-25,-19,-16}));\n")
  s += sprintf("  qm1 = veorq_u32(vshlq_u32(q2tmp1.val[1],(int32x4_t){ 19, 23, 27,  0}),\n" \
               "                  vshlq_u32(q2tmp1.val[1],(int32x4_t){-13, -9, -5, -2}));\n")
  s += sprintf("  qm1 = vaddq_u32(qm1, qm0);\n")
  s += sprintf("  dtmp0 = vadd_u32(vget_high_u32(qm1), vget_low_u32(qm1));\n")
  s += sprintf("  q[%2d] += vget_lane_u32(dtmp0, 0) + vget_lane_u32(dtmp0, 1);\n", i+16)  
  
  return s
end


puts header
[16,20,24,28].each {|x| puts gen_addElement(x)}
(0..1).each  {|x| puts gen_expand_1(x)}
(2..15).each {|x| puts gen_expand_2(x, 2)}
puts footer